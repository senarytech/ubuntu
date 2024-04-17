#!/usr/bin/env python3
#
# NDA AND NEED-TO-KNOW REQUIRED
#
# Copyright (C) 2013-2020 Synaptics Incorporated. All rights reserved.
#
# This file contains information that is proprietary to Synaptics
# Incorporated ("Synaptics"). The holder of this file shall treat all
# information contained herein as confidential, shall use the
# information only for its intended purpose, and shall not duplicate,
# disclose, or disseminate any of this information in any manner
# unless Synaptics has otherwise provided express, written
# permission.
#
# Use of the materials may require a license of intellectual property
# from a third party or from Synaptics. This file conveys no express
# or implied licenses to any intellectual property rights belonging
# to Synaptics.
#
# INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS", AND
# SYNAPTICS EXPRESSLY DISCLAIMS ALL EXPRESS AND IMPLIED WARRANTIES,
# INCLUDING ANY IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE, AND ANY WARRANTIES OF NON-INFRINGEMENT OF ANY
# INTELLECTUAL PROPERTY RIGHTS. IN NO EVENT SHALL SYNAPTICS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, PUNITIVE, OR
# CONSEQUENTIAL DAMAGES ARISING OUT OF OR IN CONNECTION WITH THE USE
# OF THE INFORMATION CONTAINED IN THIS DOCUMENT, HOWEVER CAUSED AND
# BASED ON ANY THEORY OF LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
# NEGLIGENCE OR OTHER TORTIOUS ACTION, AND EVEN IF SYNAPTICS WAS
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. IF A TRIBUNAL OF
# COMPETENT JURISDICTION DOES NOT PERMIT THE DISCLAIMER OF DIRECT
# DAMAGES OR ANY OTHER DAMAGES, SYNAPTICS' TOTAL CUMULATIVE LIABILITY
# TO ANY PARTY SHALL NOT EXCEED ONE HUNDRED U.S. DOLLARS.
#

import os
import io
import re
import json
import glob
import shutil
import tempfile
from collections import OrderedDict
import subprocess

from .exceptions import *
from .meta import *
from .redirector import Redirector
from .acuitylib_path import *
import acuitylib
import acuitylib.vsi_nn
import acuitylib.net_output_meta
import logging
from acuitylib.acuitynet import AcuityNet

version = '2.5.0'


# Implements copytree with dirs_exist_ok=True (available only from python 3.8)
def copytree(src, dst):
    for item in os.listdir(src):
        s = os.path.join(src, item)
        d = os.path.join(dst, item)
        if os.path.isdir(s):
            shutil.copytree(s, d)
        else:
            shutil.copy2(s, d)


class Converter:

    def __init__(self, verbose:bool, work_dir:str):
        self._verbose = verbose
        self._base_work_dir = os.path.dirname(work_dir)
        self._work_dir = work_dir
        self._vsi_nn = acuitylib.vsi_nn.VSInn()
        self._acuity_out = None
        self._acuity_err = None
        self._quantization_done = False
        self._framework_data_layout = DataLayout.NCHW
        self._synap_cache_path = os.path.join(self._base_work_dir, '.synap_cache.bin')
        self._conversion_info_path = os.path.join(self._base_work_dir, '.conversion_info.json')
        logging.getLogger("acuityLogger").setLevel(acuitylib.acuitylog._CRITICAL)

        os.environ['SYNAP_CACHE_CAPACITY'] = '1000'
        os.environ['SYNAP_CACHE_PATH'] = self._synap_cache_path
        os.environ['VIV_VX_DEBUG_LEVEL'] = '0'
        # os.environ['VIV_VX_ENABLE_INSERT_CMD'] = '0' # Disable additional layers at the end

        if verbose:
            # Enable TensorFlow errors and warning logs
            os.environ['TF_CPP_MIN_LOG_LEVEL'] = '1'
        else:
            # Capture stdout and stderr
            self._acuity_out = io.StringIO()
            self._acuity_err = io.StringIO()

            # Generate as much info as possible in the generated log file
            # Enable all TensorFlow logs
            os.environ['TF_CPP_MIN_LOG_LEVEL'] = '0'

            # Generate network compilation details
            os.environ['VIV_VX_DEBUG_LEVEL'] = '1'
            os.environ['NN_EXT_SHOW_PERF'] = '1'
            # os.environ['VSI_NN_LOG_LEVEL']='5'


    def convert(self, model_file:str, weights_file:str, meta:MetaInfo) -> AcuityNet:
        shutil.rmtree(os.path.join(self._base_work_dir, ".work_nbg_unify"), ignore_errors=True)
        shutil.rmtree(self._work_dir, ignore_errors=True)
        os.makedirs(self._work_dir, exist_ok=True)
        if not model_file:
            raise ConversionError("Model file not specified")
        _, ext = os.path.splitext(os.path.basename(model_file))
        ext = ext.lower()

        with Redirector(self._acuity_out, self._acuity_err):
            if ext == '.prototxt':
                if not weights_file:
                    raise ConversionError("Weights file not specified")
                net = self._vsi_nn.load_caffe(model_file, weights_file)
            elif ext == '.tflite':
                meta.check_inputs_with_shape_have_name()
                self._framework_data_layout = DataLayout.NHWC
                net = self._vsi_nn.load_tflite(
                    model_file,
                    inputs=meta.input_names_str(False),
                    input_size_list=meta.input_shapes_str(False),
                    outputs=meta.output_names_str(False)
                )
            elif ext == '.pb':
                meta.check_inputs_with_shape_have_name()
                self._framework_data_layout = DataLayout.NHWC
                if not weights_file:
                    # Assume tensorflow model
                    net = self._vsi_nn.load_tensorflow(
                        model_file, meta.input_names_str(), meta.input_shapes_str(), meta.output_names_str()
                    )
                else:
                    # Assume caffe2 model
                    self._caffe2_to_onnx(model_file, "caffe2_network", weights_file, None,
                                         os.path.join(self._work_dir, "model.onnx"))

            elif ext == '.onnx':
                meta.check_inputs_with_shape_have_name()
                if meta.input_shapes_str(False) and not meta.input_names_str(False):
                    raise ConversionError("Input names mandatory when input shape specified.")
                net = self._vsi_nn.load_onnx(
                    model_file, meta.input_names_str(False), meta.output_names_str(False), meta.input_shapes_str(False)
                )
            else:
                raise ConversionError(f"Unsupported model file: {model_file}")

            if self._framework_data_layout == DataLayout.NCHW and meta.layout == DataLayout.NHWC:
                raise ConversionError(f"{meta.layout} layout not supported with {ext} models.")
            if meta.layout == DataLayout.DEFAULT:
                meta.layout = self._framework_data_layout
            if meta.quantization:
                net = self._quantize(net, meta)

            self._vsi_nn.generate_inputmeta(net)
        return net

    def generate(self, net:AcuityNet, meta:MetaInfo, target:str, out_dir:str, tools_dir:str,
                 vssdk_dir:str, ebg:bool, profiling_mode:bool, cpu_profiling_mode:bool, debug:bool):
        os.makedirs(out_dir, exist_ok=True)

        if tools_dir is None:
            # Assume the directory layout of the synap install directory
            tools_dir = os.path.join(toolkit_dir, "..")

        tools_bin_dir=os.path.join(tools_dir, 'bin/x86_64-linux-gcc')
        if not os.path.isdir(tools_bin_dir):
            raise ConversionError(f"Can't find host tool directory: {tools_bin_dir}")

        npu_id, ddr_bw, vssdk_encrypt_subdir, soc_name = self._npu_info(target)
        viv_sdk_dir = tools_dir
        tools_lib_dir=os.path.join(tools_dir, 'lib/x86_64-linux-gcc')
        gen_nbinfo = os.path.join(tools_bin_dir, 'nbinfo')
        gen_ebg = os.path.join(tools_bin_dir, 'synap_cli_nb')
        os.environ['SYNAP_OVXLIB_DIR'] = tools_lib_dir
        # This feature can only work with internal tree with ovxlib static libraries.
        os.environ['SYNAP_ENABLE_CPU_PROFILING'] = "1" if cpu_profiling_mode and internal_tree else "0"
        os.environ['SYNA_SOC'] = soc_name
        # FIXME: We should probably get that from cmake variable at some point: VXK_INSTALL_SUBDIR
        os.environ['SYNAP_VXK_PATH'] = os.path.join(tools_dir, 'vxk', soc_name)

        if not os.path.isfile(gen_nbinfo):
            raise ConversionError(f"Can't find nbg info tool: {gen_nbinfo}")
        if not os.path.isfile(gen_ebg):
            raise ConversionError(f"Can't find ebg converter tool: {gen_ebg}")

        # If we quantized the model, save quantization info (for reference only)
        if self._quantization_done:
            self._vsi_nn.save_model_quantize(net, out_dir + "/quantization_info.json")

        self._add_postprocessing(net, meta)

        if meta.optimize and ddr_bw >= 0:
            os.environ['NN_EXT_DDR_READ_BW_LIMIT'] = str(ddr_bw)
            os.environ['NN_EXT_DDR_WRITE_BW_LIMIT'] = str(ddr_bw)
            os.environ['NN_EXT_DDR_TOTAL_BW_LIMIT'] = str(ddr_bw)

        remove_permute = (self._framework_data_layout == DataLayout.NHWC) and (meta.layout == DataLayout.NCHW)

        acuity_521 = False
        # Set parent of working directory. Here is where temporary directories are generated.
        if acuity_521:
            os.chdir(self._work_dir)
        else:
            os.chdir(self._base_work_dir)

        # Clean synap cache if conversion options changed
        conversion_info = { 'npu_id' : npu_id, 'synap_version': version }
        prev_conversion_info = {}
        if os.path.isfile(self._conversion_info_path):
            prev_conversion_info = json.load(open(self._conversion_info_path))
            json.dump({}, open(self._conversion_info_path, 'w'))
        if prev_conversion_info != conversion_info and os.path.isfile(self._synap_cache_path):
            os.remove(self._synap_cache_path)

        with tempfile.TemporaryDirectory() as tmp_out_dir:
            with Redirector(self._acuity_out, self._acuity_err):
                if debug:
                    os.environ['VIV_VX_DEBUG_LEVEL'] = '1'
                self._vsi_nn.export_ovxlib(net,
                                           output_path=tmp_out_dir + '/.work/',
                                           optimize=npu_id,
                                           dtype="quantized" if True else "float",  # FIXME
                                           pack_nbg_unify=True,
                                           viv_sdk=viv_sdk_dir,
                                           force_remove_permute=remove_permute)
            copytree(tmp_out_dir + '/.work/', self._work_dir)
            if cpu_profiling_mode or debug:
                gen_nbg_path = os.path.join(self._work_dir, "gen_nbg")
                gen_nbg_data = os.path.join(self._work_dir, ".export.data")
                gen_nbg_input = os.path.join(self._work_dir, "input_0_0.tensor")
                gen_nbg_run = [gen_nbg_path, gen_nbg_data, gen_nbg_input]
                # Force enable debugging logs
                os.environ['VIV_VX_DEBUG_LEVEL'] = '1'
                # we just want to profile OpenVX export
                os.environ['VIV_VX_ENABLE_SAVE_NETWORK_BINARY'] = "0"
                print("Generating NBG without cache: ")
                print(" ".join(gen_nbg_run))
                # Run once without cache
                os.environ['SYNAP_CACHE_CAPACITY'] = "0"
                subprocess.run(gen_nbg_run)
            if cpu_profiling_mode:
                # Get cpu profiling with cache enabled
                print("Generating NBG with cache: ")
                os.environ['SYNAP_CACHE_CAPACITY'] = "10000"
                subprocess.run(gen_nbg_run)
                gen_nbg_gmon = os.path.join(self._work_dir, "gmon.out")
                grof_report = os.path.join(self._work_dir, "profile.txt")
                gprof_run = ["gprof", gen_nbg_path, gen_nbg_gmon]
                with open(grof_report , "w") as report:
                    subprocess.run(gprof_run, stdout=report, check=True)

            shutil.copytree(tmp_out_dir + '/.work_nbg_unify', self._base_work_dir + "/.work_nbg_unify")

        # Remember conversion options used
        json.dump(conversion_info, open(self._conversion_info_path, 'w'))

        if acuity_521:
            generated_model_dir = os.path.join(self._work_dir, os.path.basename(self._work_dir) + "_nbg_unify")
        else:
            generated_model_dir = self._work_dir + "_nbg_unify"
        generated_nbg_path = os.path.join(generated_model_dir, 'network_binary.nb')
        generated_ebg_path = os.path.join(generated_model_dir, 'model.ebg')
        generated_encrypted_path = os.path.join(generated_model_dir, 'model.ebge')
        destination_bg_path = os.path.join(out_dir, 'model.nb')
        destination_bginfo_path = os.path.join(out_dir, 'model.info.txt')

        # Generate network info
        with Redirector(None, self._acuity_err):
            with open(destination_bginfo_path, "w") as infofile:
                subprocess.run([gen_nbinfo, '-a', generated_nbg_path], stdout=infofile, check=True)

        # Generate anchors rounded to 6 decimals (more decimals are useless)
        anchors_string = '[]'
        if hasattr(net, 'anchors') and net.anchors:
            anchors_string = json.dumps([round(a, 6) for a in net.anchors], separators=(',', ':'))
            with open(os.path.join(self._work_dir, 'anchors.json'), 'w') as f:
                f.write(anchors_string)
                if self._verbose:
                    print("Model anchors generated in:", f.name)
        for out in meta.outputs:
            out.format = out.format.replace('${ANCHORS}', anchors_string)

        generated_model = generated_nbg_path
        if ebg:
            # Convert network to ebg
            gen_ebg_command = [gen_ebg, '--nb', generated_model, '--to-ebg', generated_ebg_path]
            if profiling_mode:
                if self._verbose:
                    print("Profiling mode enabled")
                gen_ebg_command.append('--profile')
            with Redirector(self._acuity_out, self._acuity_err):
                subprocess.run(gen_ebg_command, check=True)
            generated_model = generated_ebg_path

            if meta.security:
                if not vssdk_encrypt_subdir:
                    raise ConversionError("Secure models not supported by the target")
                if vssdk_dir is None:
                    # Assume the directory layout of the synap install directory in vssdk
                    vssdk_dir = os.path.abspath(os.path.join(toolkit_dir, "..", "..", ".."))
                if not os.path.isdir(vssdk_dir):
                    raise ConversionError(f"Cannot find vssdk directory: {vssdk_dir}")
                enc_tool = os.path.join(vssdk_dir, 'factory', 'scripts', vssdk_encrypt_subdir, 'npu', 'bin', 'genx_img')
                if not os.path.isfile(enc_tool):
                    raise ConversionError(f"Can't find encryption tool: {enc_tool}")
                self._encrypt_model(net, generated_model, generated_encrypted_path, meta, enc_tool)
                generated_model = generated_encrypted_path

        shutil.copy(generated_model, destination_bg_path)

        self._create_metafile(generated_model_dir + '/nbg_meta.json', out_dir + '/model.json', meta)

        # Save conversion output log (empty in verbose mode)
        with open(os.path.join(self._work_dir, 'conversion_log.txt'), "w") as log_file:
            log_file.write(self.output())


    def errors(self) -> str:
        return self._acuity_err.getvalue() if self._acuity_err else ""


    def output(self) -> str:
        return self._acuity_out.getvalue() if self._acuity_out else ""


    def _quantize(self, net:AcuityNet, meta:MetaInfo) -> AcuityNet:
        q = meta.quantization
        dataset_file = os.path.join(self._work_dir, 'quantization_dataset.txt')
        dataset_size = self._create_quantization_dataset(meta, dataset_file)
        self._vsi_nn.set_database(net, dataset_file, dataset_type='TEXT')
        if self._verbose:
            print(f"Quantization scheme: {q.scheme}")
            print(f"Quantization data_type: {q.data_type}")
            print(f"Quantization dataset: {q.dataset}")
            print(f"Quantization steps: {dataset_size}")

        preprocessing_options = {} # OrderedDict
        in_layers =  net.get_input_layers()
        if meta.input_names_str(False):
            # Get list of input layers in the order specified by the user
            in_layers = [next(l for l in in_layers if l.name == lyr.name) for lyr in meta.inputs]
        for in_index, l in enumerate(in_layers):
            input = meta.inputs[in_index]
            has_numpy_samples = meta.quantization.dataset[in_index].lower().endswith('.npy')
            prep_opts = {
                'reverse_channel' : input.format.lower() == 'bgr'
            }

            if input.shape is not None:
                if not isinstance(input.shape, list):
                    raise ConversionError(f"invalid shape provided for input: {l.name}: {input.shape}")
                prep_opts['shape'] = input.shape

            if input.means is not None:
                if not isinstance(input.means, (list, int, float)):
                    raise ConversionError(f"invalid means provided for input: {l.name}: {input.means}")
                if l.get_output().shape.rank != 4:
                    raise ConversionError(f"input means can only be specified for 4-dimensional tensors")
                ch_ix = 1 if meta.layout == DataLayout.NCHW else 3
                input_means = input.means if isinstance(input.means, list) else [input.means] * l.get_output().shape.dims[ch_ix]
                if has_numpy_samples and any(m for m in input_means):
                    raise ConversionError(f"input means not supported with NumPy quantization files")
                prep_opts['mean'] = input_means

            if input.scale is not None:
                if not isinstance(input.scale, (int, float)) or input.scale == 0:
                    raise ConversionError(f"invalid scale provided for input: {l.name}: {input.scale}")
                if has_numpy_samples and input.scale != 1:
                    raise ConversionError(f"input scale not supported with NumPy quantization files")
                prep_opts['scale'] = 1 / input.scale

            preprocessing_options[l.lid] = prep_opts

        self._vsi_nn.set_preprocess(net, preprocessing_options, set_by_lid=True)
        qnet = self._vsi_nn.quantize(net,
            quantizer=q.scheme,
            qtype=q.data_type,
            hybrid=False,
            rebuild=True,
            rebuild_all=False,
            iterations=dataset_size
        )
        self._quantization_done = True
        return qnet

    @staticmethod
    def _create_quantization_dataset(meta:MetaInfo, dataset_file: str) -> int:
        # Expand each dataset glob
        dataset = [glob.glob(d) for d in meta.quantization.dataset]
        for d in dataset:
            for filename in d:
                if ' ' in filename:
                    raise ConversionError("Spaces not allowed in quantization dataset: " + filename)

        # Check all expanded datasets have the same number of files
        dataset_lengths = set([len(d) for d in dataset])
        if len(dataset_lengths) > 1:
            raise ConversionError("Quantization datasets must have the same number of files")
        dataset_len = dataset_lengths.pop()
        if dataset_len == 0:
            raise ConversionError("Quantization dataset is empty")

        # Generate dataset file in acuity format
        with open(dataset_file, "w") as f:
            for i in range(dataset_len):
                print(" ".join([d[i] for d in dataset]), file=f)

        return dataset_len

    def _add_postprocessing(self, net:AcuityNet, meta:MetaInfo):
        if not meta.dequantize_outputs and not any(out.dequantize for out in meta.outputs):
            return None

        postprocess_list = []
        out_layers = net.get_output_layers()
        # Layer names from ONNX are not the actual names, skip reordering in that case
        if meta.output_names_str(False) and net.get_org_platform() != 'onnx':
            # Get list of output layers in the order specified by the user
            selected_outputs = []
            for lyr in meta.outputs:
                try:
                    selected_outputs.append(next(l for l in out_layers if l.name == lyr.name))
                except:
                    raise ConversionError(f"Specified output: {lyr.name} not in model outputs: "
                                          f"{[l.name for l in  net.get_output_layers()]}")
            out_layers = selected_outputs
        for out_index, l in enumerate(out_layers):
            if meta.dequantize_outputs or meta.outputs[out_index].dequantize:
                postprocess_list.append([l.lid, [{
                    'add_postproc_node': True,
                    'perm': list(range(len(l.get_output().shape.dims))),
                    'force_float32': True
                }]])
        if postprocess_list:
            self._vsi_nn.set_app_postprocess(net, postprocess_list, set_by_lid=True)


    # Add additional information to vsi json file:
    # - the 'data_format' for each input and output tensor (for example 'rgb', 'bgr')
    # Fix the 'format' attribute which is incorrect when the 'remove_permute' option is used
    def _create_metafile(self, vsi_meta_path:str, synap_meta_path:str, meta:MetaInfo):
        meta_json = json.load(open(vsi_meta_path), object_pairs_hook=OrderedDict)
        meta_json['secure'] = meta.security is not None
        for i, (name, info) in enumerate(meta_json['Inputs'].items()):
            item_format = meta.inputs[i].format if i < len(meta.inputs) else None
            info['data_format'] =  item_format if item_format else meta.input_format
            if meta.security:
                info['security'] =  meta.inputs[i].security if i < len(meta.inputs) else InputInfo.default_security
            info['format'] = meta.layout.value
        for i, (name, info) in enumerate(meta_json['Outputs'].items()):
            item_format = meta.outputs[i].format if i < len(meta.outputs) else None
            info['data_format'] = item_format if item_format else meta.output_format
            if meta.security:
                info['security'] =  meta.outputs[i].security if i < len(meta.outputs) else OutputInfo.default_security
            info['format'] = meta.layout.value
        with open(synap_meta_path, 'w') as f:
            json.dump(meta_json, f, indent = 4)

    def _encrypt_model(self, net:AcuityNet, clear_path:str, encrypted_path:str, meta:MetaInfo, enc_tool:str):
        encrypt_model_tool =  os.path.join(toolkit_dir, 'encrypt-model-ebg.py')
        # Create encryption json metadata file from conversion meta info
        with tempfile.NamedTemporaryFile(mode='w') as encryption_meta:
            default_in_security = [InputInfo.default_security] * len(net.get_input_layers())
            default_out_security = [OutputInfo.default_security] * len(net.get_output_layers())
            json.dump({
                "inputs": [i.security for i in meta.inputs] if meta.inputs else default_in_security,
                "outputs": [o.security for o in meta.outputs] if meta.outputs else default_out_security
            }, encryption_meta)
            encryption_meta.seek(0)

            with Redirector(self._acuity_out, self._acuity_err):
                subprocess.run([encrypt_model_tool,
                                '--enc-tool', enc_tool,
                                '--security-policy', encryption_meta.name,
                                '--encryption-key', meta.security.encryption_key,
                                '--signature-key', meta.security.signature_key,
                                '--model-certificate', meta.security.model_certificate,
                                '--vendor-certificate', meta.security.vendor_certificate,
                                clear_path,
                                encrypted_path], check=True)


    @staticmethod
    def _caffe2_to_onnx(caffe2_net,
                       caffe2_net_name,
                       caffe2_init_net,
                       value_info,
                       output_file):
        # https://caffe2.ai/doxygen-python/html/conversion_8py_source.html

        # Import caffe2 ony when needed to avoid spurious annoying warning messages about no GPU
        from caffe2.proto import caffe2_pb2
        from caffe2.python.onnx.backend import Caffe2Backend as c2
        import caffe2.python.onnx.frontend as c2_onnx

        c2_net_proto = caffe2_pb2.NetDef()
        with open(caffe2_net, 'rb') as f:
            c2_net_proto.ParseFromString(f.read())
        if not c2_net_proto.name and not caffe2_net_name:
            raise ConversionError('The input caffe2 net does not have name')
        c2_net_proto.name = caffe2_net_name or c2_net_proto.name
        if caffe2_init_net:
            c2_init_net_proto = caffe2_pb2.NetDef()
            with open(caffe2_init_net, 'rb') as f:
                c2_init_net_proto.ParseFromString(f.read())
            c2_init_net_proto.name = '{}_init'.format(caffe2_net_name)
        else:
            c2_init_net_proto = None

        if value_info:
            value_info = json.loads(value_info)

        onnx_model = c2_onnx.caffe2_net_to_onnx_model(
            predict_net=c2_net_proto,
            init_net=c2_init_net_proto,
            value_info=value_info)

        with open(output_file, 'wb') as f:
            f.write(onnx_model.SerializeToString())


    @staticmethod
    def _npu_info(target_name: str):
        target = target_name.upper()
        if target == 'VS680A0' or target == 'VS680':
            return ("VIP9000NANOQI_PLUS_PID0XC1", 22, 'vs680', 'VS680A0')
        elif target == 'VS640A0' or target == 'VS640':
            return ("VIP9000NANOSI_PID0XC2", 10, 'vs640', 'VS640A0')
        raise ConversionError(f"Unknown target: {target}")

