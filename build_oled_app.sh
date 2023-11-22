#!/bin/bash

RK3568_IP="192.168.9.245"

if [ ! -d "build" ];then
	mkdir build
fi

cd build

cmake ..

make

sshpass -p gv002869 scp -o StrictHostKeyChecking=no /home/xgj/workspace/ip_box/ip-box/apps/display/build/display root@${RK3568_IP}:/root