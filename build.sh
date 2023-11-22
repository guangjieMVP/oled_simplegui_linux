#!/bin/sh


export PATH=~/genvict/ip-box/toolchian/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin:$PATH

# aarch64-linux-gnu-gcc -v

RK3568_IP="192.168.5.245"

if [ $# -eq 1 ]; then
    RK3568_IP=$1
fi



if [ -f "display" ]; then
    rm  display
fi

# aarch64-linux-gnu-gcc -o display display.c st7567.c  oled.c ascii_fonts.c
mkdir build 
cd build
cmake ..
make clean
make
if [ $? -ne 0 ]; then
    echo "build display failed"
    exit 1
fi

sshpass -p gv002869 scp -o StrictHostKeyChecking=no display root@${RK3568_IP}:/root
# sshpass -p root scp -o StrictHostKeyChecking=no display root@${RK3568_IP}:/root
if [ $? -ne 0 ]; then
    echo "scp display failed"
    exit 1
fi