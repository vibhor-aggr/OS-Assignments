#!/usr/bin/bash

cd /root/new_kernel/compile3/linux-6.0

echo "Executing make for 3st compilation ..."
make -j$(nproc)
