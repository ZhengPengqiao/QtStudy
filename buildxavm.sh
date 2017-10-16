#!/bin/bash
source /opt/fsl-imx-fb/3.14.52-1.1.0/environment-setup-cortexa9hf-vfp-neon-poky-linux-gnueabi
qmake /home/zpq/workspaces/QtStudy/xavm/xavm.pro -r CONFIG+=debug CONFIG+=qml_debug
make
