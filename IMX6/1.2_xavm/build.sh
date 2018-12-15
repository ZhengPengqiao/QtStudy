#!/bin/bash

TARGET=`grep -i "TARGET" *.pro | gawk '{print $3}'`
echo TARGET=$TARGET

PROFILE=`ls *.pro`
echo PROFILE=$PROFILE

if [ $# == 0 ]
then
    echo "complite programe"
    source /opt/fsl-imx-fb/3.14.52-1.1.0/environment-setup-cortexa9hf-vfp-neon-poky-linux-gnueabi
    qmake ./$PROFILE -r CONFIG+=debug CONFIG+=qml_debug
    make
    echo "complite programe Ok"
    exit
fi

if [ "$1" == "clean" ]
then
    echo "clean programe"
    if [ -e "./Makefile" ]
    then
        echo "Makefile is Exist"        
        make clean
        rm -rf Makefile
    else
        echo "Makefile not Exist"        
    fi
    echo "rm -rf $TARGET"
    rm -rf  $TARGET
    echo "complite programe OK"
    exit
fi

echo make sure you param is Ok??
