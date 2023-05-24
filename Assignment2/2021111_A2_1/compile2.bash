#!/usr/bin/bash

cd /root/new_kernel/compile2/linux-6.0

echo "Executing make for 2st compilation ..."
make -j$(nproc)
