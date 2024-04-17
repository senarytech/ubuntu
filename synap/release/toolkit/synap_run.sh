#!/bin/bash

# Execute commands inside synap docker.
# The docker container is built if not already present
set -x
set -e

BASE_PATH="$( cd "$( dirname "${BASH_SOURCE[0]}" )/.." >/dev/null 2>&1 && pwd )"
WD_PATH="$( pwd )"
MOUNT_ROOT_PATH="$( pwd )"
DOCKER_IMAGE_NAME=synap

NO_DOCKER_BUILD=
QUIET=--quiet

# Parse and remove builder arguments
while [[ $# -gt 0 ]]; do
  # Use shift to pop the next argument from the argument list
  case "$1" in
    --mrd)
      shift
      MOUNT_ROOT_PATH=`cd "${1}" && pwd`
      shift
      ;;
    --wd)
      shift
      WD_PATH=`cd "${1}" && pwd`
      cd $WD_PATH
      shift
      ;;
    --no-docker-build)
      shift
      NO_DOCKER_BUILD="1"
      ;;
    --verbose)
      shift
      QUIET=""
      ;;
    --help|-h)
      echo "usage: $(basename -- $0) [args..] \"command[s] to be executed inside docker\""
      echo "  --help              Show this help message and exit "
      echo "  --mrd <dir>         Root directory to mount"
      echo "  --wd <dir>          Working directory from where to execute commands"
      echo "  --no-docker-build   Skip docker build"
      echo "  --verbose           Verbose docker build"
      echo "Example:"
      echo "  $(basename -- $0) \"synap_convert.py --model mobilenet_v2_1.0_224_quant.tflite --target VS680 --out-dir mnv2\""
      exit 0
      ;;
    *)
      break
      ;;
  esac
done

# create build context directory
# create fake passwd and group files to have valid username and groupname in the docker
BUILD_CONTEXT_DIR=$(mktemp -d)
PASSWD_FILE=$(mktemp)
GROUP_FILE=$(mktemp)

function cleanup() {
    rm -f "${PASSWD_FILE}" "${GROUP_FILE}"
    rm -rf "${BUILD_CONTEXT_DIR}"
}

trap cleanup EXIT

# Build synap docker
if [[ "${NO_DOCKER_BUILD}" == "" ]] ; then
  cp ${BASE_PATH}/toolkit/pysynap/requirements.txt ${BUILD_CONTEXT_DIR}/pysynap_requirements.txt
  DOCKER_SCAN_SUGGEST=false docker build $QUIET -f ${BASE_PATH}/dockerfiles/Dockerfile_synap -t ${DOCKER_IMAGE_NAME}:latest ${BUILD_CONTEXT_DIR} > /dev/null
fi


USERID=$(id -u ${USER})
GROUPID=$(id -g ${USER})

cat > "${PASSWD_FILE}" << EOF
root:x:0:0::/root:/bin/bash
${USER}-${DOCKER_IMAGE_NAME}:x:${USERID}:${GROUPID}:${USER},,,:/home/${USER}:/bin/bash
EOF

cat > "${GROUP_FILE}" << EOF
root:x:0:root
${USER}:x:${GROUPID}:${USER}-${DOCKER_IMAGE_NAME}
EOF


# Run interactive shell if needed
if [ -t 0 ] ; then
    INTERACTIVE="-it"
fi

RUN_IN_BUILDER="docker run --rm -u $(id -u ${USER}):$(id -g ${USER}) ${INTERACTIVE} \
                -v ${GROUP_FILE}:/etc/group \
                -v ${PASSWD_FILE}:/etc/passwd \
                -v ${BASE_PATH}:${BASE_PATH} \
                -v ${MOUNT_ROOT_PATH}:${MOUNT_ROOT_PATH} \
                -v ${WD_PATH}:${WD_PATH} \
                --workdir $WD_PATH
                ${DOCKER_IMAGE_NAME} bash -c "
${RUN_IN_BUILDER} "export PATH=\$PATH:$BASE_PATH/toolkit; $*"
