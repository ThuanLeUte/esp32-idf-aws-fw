#!/bin/bash

# CURR_DIR=${PWD}
CURR_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
CURR_SHELL="$(basename -- $SHELL)"
SHELL_CFG="$HOME/.${CURR_SHELL:-bash}rc"
XTENSA_PATH="sdk/xtensa-esp32-elf/bin"

sudo apt-get install -y gcc git wget make libncurses-dev flex bison gperf python python-pip python-setuptools python-serial python-cryptography python-future python-pyparsing libffi-dev libssl-dev

git submodule update --init --recursive

echo "INFO: CURR_SHELL = $CURR_SHELL"
echo "INFO: SHELL_CFG = $SHELL_CFG"

# Check if IDF_PATH existed or not
if [[ -z "${IDF_PATH}" ]];
then
    echo "INFO: IDF_PATH NOT FOUND. Exporting..."
    echo -e "export IDF_PATH=$CURR_DIR/sdk/esp-idf" >> "$SHELL_CFG"
else
    echo "INFO: IDF_PATH FOUND!"
fi

# Check if xtensa path is exported or not
if [[ "$PATH" == *"$XTENSA_PATH"* ]]; then
  echo "INFO: XTENSA FOUND"
else
  echo "INFO: XTENSA NOT FOUND. Exporting..."
  echo -e "export PATH=\"\$PATH:$CURR_DIR/$XTENSA_PATH\"" >> "$SHELL_CFG"
fi

sudo usermod -a -G dialout $USER

python -m pip install --user -r $CURR_DIR/sdk/esp-idf/requirements.txt

# Check if ADF_PATH existed or not
if [[ -z "${ADF_PATH}" ]];
then
    echo "INFO: ADF_PATH NOT FOUND. Exporting..."
    echo -e "export ADF_PATH=$CURR_DIR/sdk/esp-adf" >> "$SHELL_CFG"
    bash $CURR_DIR/sdk/esp-idf/install.sh
    echo -e "source $CURR_DIR/sdk/esp-idf/export.sh" >> "$SHELL_CFG"
else
    echo "INFO: ADF_PATH FOUND!"
fi

source $SHELL_CFG

# ✔: \xe2\x9c\x93
# ✗: \xe2\x9c\x97
echo -e "\xe2\x9c\x93 Setup OK"
