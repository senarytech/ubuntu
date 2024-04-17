#!/bin/bash
# Benchmark a model on Android using NNAPI
#
# First install the apk as explained in https://www.tensorflow.org/lite/performance/measurement:
# 1) download https://storage.googleapis.com/tensorflow-nightly-public/prod/tensorflow/release/lite/tools/nightly/latest/android_arm_benchmark_model.apk
# 2) adb install -r -d -g android_arm_benchmark_model.apk

script_name="$0"

function print_help() {
    echo "usage: $(basename -- $script_name) [args..]"
    echo "  --model         Model file (.tflite)"
    echo "  --use_cpu       Inference on CPU with 4 parallel threads"
    echo "  --use_npu       Execute inference on NPU"
    echo "  --use_gpu       Execute inference on GPU"
    echo "  --use_nnapi     Execute with NNAPI"
    echo "  --verbose       Print detailed information logs"
    echo "  --num_runs=N    Average benchmark on N executions"
    echo "  ...             Any other standard benchmark_model option"
    echo "  --help          Show this help"
}

# Parse arguments
while [[ $# -gt 0 ]]; do
  # Use shift to pop the next argument from the argument list
  case "$1" in
    --model|-m)
      shift
      model_host_path="${1}"
      shift
      ;;
    --use_cpu)
      extra_options="$extra_options --num_threads=4"
      shift
      ;;
    --use_gpu)
      extra_options="$extra_options --use_gpu=true"
      shift
      ;;
    --use_npu)
      extra_options="$extra_options --use_nnapi=true --nnapi_accelerator_name=synap-npu"
      shift
      ;;
    --use_nnapi)
      extra_options="$extra_options --use_nnapi=true"
      shift
      ;;
    --verbose)
      verbose="1"
      shift
      ;;
    --help|-h)
      print_help
      exit 0
      ;;
    *)
      extra_options="$extra_options ${1}"
      shift
      ;;
  esac
done

if [[ "${model_host_path}" == "" ]] ; then
    echo "Error, model not specified."
    print_help   
    exit 1
fi

set -e

# Upload model
model_dir=/data/local/tmp
model_filename="$(basename -- "${model_host_path}")"
adb push --sync ${model_host_path} $model_dir > /dev/null

# Execute benchmark activity
echo "Benchmarking model: $model_host_path:"
echo "Benchmarking options: $extra_options"
adb logcat -c
err=$(adb shell am start -S \
    -n org.tensorflow.lite.benchmark/.BenchmarkModelActivity \
    --es args "\"--graph=$model_dir/$model_filename $extra_options \"" 2>&1 > /dev/null)

if [ ! -z "$err" ]; then
    >&2 echo "$err"
    exit 1
fi

# Capture logs until inference message
#adb logcat | tee log-nnapi.txt | while read -r line
adb logcat | while read -r line
do
    if [[ $line == *"Average inference"* ]]; then
        echo $line 
        kill $$
        exit 0
    fi
    if [[ "${verbose}" == "1" ]] ; then
        if [[ $line == *"timeOnDevice"* ]]; then
            echo $line 
        elif [[ "$line" == *"synap-npu"* || "$line" == *"vpm_"*  || "$line" == *"nnapi"*  || "$line" == *"NNAPI"* ]]; then
           echo $line 
        fi
        if [[ "$line" == *" E BROKER"* || "$line" == *" E NPU"* || "$line" == *" E tflite"* ]]; then
            echo $line 
        fi
    fi
done
