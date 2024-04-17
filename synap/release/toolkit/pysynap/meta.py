#!/usr/bin/env python3
#
# NDA AND NEED-TO-KNOW REQUIRED
#
# Copyright (C) 2013-2022 Synaptics Incorporated. All rights reserved.
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
import re

from .exceptions import *
from .acuitylib_path import *
import acuitylib
from enum import Enum

class DataLayout(Enum):
    DEFAULT = 'default'
    NCHW = 'nchw'
    NHWC = 'nhwc'


# Check that the fields dict doesn't contain any unknown entry
def _check_fields(map:dict, valid_fields:list, mandatory_fields:list=[]):
    fields = set(list(map.keys()))
    diff = fields - set(valid_fields + mandatory_fields)
    if diff:
        raise ConversionError(f"Invalid field(s) in yaml file: " + ", ".join(list(diff)))
    if mandatory_fields:
        diff =  set(mandatory_fields) - fields
        if diff:
            raise ConversionError(f"Missing mandatory field(s) in yaml file: " + ", ".join(list(diff)))


# Check that file exists
def _check_file_exists(file_name:str, desc:str):
    if not os.path.isfile(file_name):
        raise ConversionError(f"{desc} not found: {file_name}")


# Replace {FILE:filename} in string with the content of file "filename"
# filename is absolute or relative to the directory containing the metafile
def _preprocess_file(s:str, base_path:str):
    if not s:
        return ''
    while True:
        match = re.search('\${FILE:([^}]*)}', s)
        if not match:
            break
        filename = match.group(1)
        filepath = os.path.join(base_path, filename)
        _check_file_exists(filepath, f"File in string")
        with open(filepath,"r") as f:
            s = s.replace('${FILE:'+ filename + '}', f.read())
    return s


# Replace {ENV:varname} in a string with the content the corresponding environment variable
# If the environment variable is not defined it is assumed the empty string
def _preprocess_env_var(s:str):
    if not s:
        return ''
    while True:
        match = re.search('\${ENV:([^}]*)}', s)
        if not match:
            break
        var_name = match.group(1)
        var_content = os.environ.get(var_name, '')
        s = s.replace('${ENV:' + var_name + '}', var_content)
    return s


# Load, preprocess and parse a yaml file
def _load_yaml(file_name:str, desc:str='yaml file'):
    _check_file_exists(file_name, desc)
    try:
        with open(file_name, "r") as fp:
            yaml_content = acuitylib.yaml_parser._setup_yaml().load(_preprocess_env_var(fp.read()))
    except Exception as e:
        print(e, file=sys.stderr)
        raise ConversionError(f"Error parsing {desc}: {file_name}")
    if yaml_content is None:
        yaml_content = {}
    elif not isinstance(yaml_content, dict):
        raise ConversionError(f"Invalid format for {desc}: {file_name}")
    return yaml_content


class InputInfo:
    default_security = 'any'
    def __init__(self, name: str, shape: list, means: list, scale: float, format: str, security: str):
        self.name = name
        self.shape = shape
        self.means = means
        self.scale = scale
        self.format = format
        self.security = security if security else self.default_security
        if self.shape is not None:
            # The following test is not actually needed in caffe, so check separately
            #if not self.name:
            #    raise ConversionError("Input name is mandatory when shape specified: " + str(shape))
            if not isinstance(shape, list) or not all(isinstance(n, int) for n in shape):
                raise ConversionError("Shape for input '" + name + "' must be a list of integers: " + str(shape))
            if len(shape) < 2:
                raise ConversionError("Shape for input '" + name + "' doesn't include batch dimension: " + str(shape))
        if self.name and not self.shape:
            raise ConversionError("Input shape is mandatory when name specified: " + str(name))

class OutputInfo:
    default_security = 'secure-if-input-secure'
    def __init__(self, name:str, dequantize: bool, format: str, security:str):
        self.name = name
        self.dequantize = dequantize
        self.format = format
        self.security = security if security else self.default_security


class QuantizationInfo:
    def __init__(self, dataset:str, scheme:str, data_type:str):
        self.dataset = dataset
        self.scheme = scheme
        self.data_type = data_type


class SecurityInfo:
    def __init__(self, encryption_key:str, signature_key:str, model_certificate:str, vendor_certificate:str):
        self.encryption_key = encryption_key
        self.signature_key = signature_key
        self.model_certificate = model_certificate
        self.vendor_certificate = vendor_certificate


class MetaInfo:
    def __init__(self, meta_file:str):
        # Default settings
        self.layout = DataLayout.DEFAULT
        self.input_format = ''
        self.output_format = ''
        self.optimize = True
        self.dequantize_outputs = False
        self.inputs = []
        self.outputs = []
        self.quantization = None
        self.security = None

        if not meta_file:
            # No meta-file specified, just use the default values
            return
        meta_file = os.path.abspath(meta_file)
        metafile_path = os.path.dirname(meta_file)
        model_meta = _load_yaml(meta_file, 'model metafile')

        # Check fields in meta-file
        _check_fields(model_meta, [
            'optimize',
            'data_layout',
            'input_format',
            'output_format',
            'denormalize_outputs',
            'dequantize_outputs',
            'inputs',
            'outputs',
            'quantization',
            'security',
            'custom'
        ])

        # Get input info from meta-file
        self.optimize = model_meta.get('optimize', True)
        layout_str = str(model_meta.get('data_layout', '')).lower()
        self.input_format = str(model_meta.get('input_format', '')).lower()
        self.output_format = str(model_meta.get('output_format', '')).lower()
        self.dequantize_outputs = ( model_meta.get('denormalize_outputs', False) or 
                                    model_meta.get('dequantize_outputs', False))
        if layout_str:
            try:
                self.layout = DataLayout(layout_str)
            except:
                raise ConversionError('Invalid data_format specified: ' + layout_str)

        inputs = model_meta.get("inputs")
        if inputs is not None:
            for input in inputs:
                _check_fields(input, ['name', 'shape', 'means', 'scale', 'format', 'security', 'custom'])
                self.inputs.append(
                    InputInfo(
                        name=input.get('name'),
                        shape=input.get('shape'),
                        means=input.get('means'),
                        scale=input.get('scale'),
                        format=_preprocess_file(input.get('format', ''), metafile_path),
                        security=input.get('security')
                    )
                )

        input_names = [i.name for i in self.inputs if i.name]
        if  len(input_names) != len(set(input_names)):
            raise ConversionError(f"Duplicate input names in: {meta_file}")

        # Get output info from meta-file
        outputs = model_meta.get("outputs")
        if outputs is not None:
            for output in outputs:
                _check_fields(output, ['name', 'dequantize', 'format', 'security', 'custom'])
                self.outputs.append(
                    OutputInfo(
                        name=output.get("name"),
                        dequantize=output.get('dequantize', False),
                        format=_preprocess_file(output.get('format', ''), metafile_path),
                        security=output.get('security')
                    )
                )

        output_names = [o.name for o in self.outputs if o.name]
        if  len(output_names) != len(set(output_names)):
            raise ConversionError(f"Duplicate output names in: {meta_file}")

        # Get quantization info from meta-file
        quantization = model_meta.get('quantization')
        if quantization is not None:
            _check_fields(quantization, ['dataset', 'scheme', 'data_type'])
            self.quantization = QuantizationInfo(
                dataset=quantization.get('dataset'), 
                scheme=quantization.get('scheme', 'asymmetric_affine'),
                data_type=quantization.get('data_type', 'uint8')
            )
            q = self.quantization
            if q.dataset is None:
                # Disable quantization if no dataset
                self.quantization = None
            else:
                # Handle string as single-item list
                if isinstance(q.dataset, str):
                    q.dataset = [q.dataset]
                # Make dataset paths absolute if not already so
                base_dir = os.path.dirname(meta_file)
                q.dataset = [os.path.abspath(os.path.join(base_dir, d)) for d in q.dataset]
                if len(self.inputs) != len(q.dataset):
                    raise ConversionError(f"Metafile has {len(self.inputs)} input(s) but {len(q.dataset)} quantization dataset")

        # Get security info from meta-file or security file
        security = model_meta.get('security')
        if security is not None:
            sec_fields = ['encryption_key', 'signature_key', 'model_certificate', 'vendor_certificate']
            secure = security.get('secure', True)
            security_base_path = metafile_path
            security_file = security.get('file')
            if security_file is not None:
                _check_fields(security, ['secure', 'file'])
                security_file_path = os.path.abspath(os.path.join(metafile_path, security_file))
                security_base_path = os.path.dirname(security_file_path)
                security = _load_yaml(security_file_path, 'security file')
                _check_fields(security, [], sec_fields)
            else:
                _check_fields(security, ['secure'], sec_fields)
            if secure != False:
                self.security = SecurityInfo(
                    encryption_key=os.path.join(security_base_path, security.get('encryption_key')),
                    signature_key=os.path.join(security_base_path, security.get('signature_key')),
                    model_certificate=os.path.join(security_base_path, security.get('model_certificate')),
                    vendor_certificate=os.path.join(security_base_path, security.get('vendor_certificate'))
                )
                _check_file_exists(self.security.encryption_key, 'encryption_key file')
                _check_file_exists(self.security.signature_key, 'signature_key file')
                _check_file_exists(self.security.model_certificate, 'model_certificate file')
                _check_file_exists(self.security.vendor_certificate, 'vendor_certificate file')


    @staticmethod
    def _names_str(desc, items, mandatory: bool = True) -> str:
        if mandatory:
            if not items:
                raise ConversionError(f"Missing required '{desc}s' field in meta file")
            if any(not i.name for i in items):
                raise ConversionError(f"Missing required {desc} 'name' field in meta file")
        if any(i.name for i in items) and any(not i.name for i in items):
            raise ConversionError(f"All {desc} names or none must be specified in meta file")
        names = " ".join(i.name for i in items) if any(i.name for i in items) else None
        return names


    def input_shapes_str(self, mandatory: bool = True) -> str:
        # Check all required info have been provided
        if mandatory:
            if not self.inputs:
                raise ConversionError(f"Missing required 'inputs' field in meta file")
            if any(i.shape is None for i in self.inputs):
                raise ConversionError(f"Missing required input 'shape' field in meta file")
        if any(i.shape for i in self.inputs) and any(not i.shape for i in self.inputs):
            raise ConversionError(f"All input shapes or none must be specified in meta file")
        if not any(i.shape for i in self.inputs):
            return None
        # Remove 1st dimension (N) from shapes and concatenate all inputs with # separator
        # eg for 2 inputs [1, 224, 224, 3] -> "224,224,3#224,224,3"
        shapes = "#".join([','.join([str(s) for s in i.shape[1:]]) for i in self.inputs])
        return shapes


    def input_names_str(self, mandatory: bool = True) -> str:
        return self._names_str("input", self.inputs, mandatory)


    def output_names_str(self, mandatory: bool = True) -> str:
        return self._names_str("output", self.outputs, mandatory)
    
    def check_inputs_with_shape_have_name(self):
        for i in self.inputs:
            if i.shape is not None and not i.name:
                raise ConversionError("Input name is mandatory when shape specified: " + str(i.shape))

