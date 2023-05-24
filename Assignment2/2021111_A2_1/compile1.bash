#!/usr/bin/bash

cd /root/new_kernel/compile1/linux-6.0

echo "Executing make for 1st compilation ..."
make -j$(nproc)
