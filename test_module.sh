#!/bin/bash

## Used for module testing
## Compiles module, loads module, checks to see if module was loaded
## Checks output of module, removes module, and then clears kernel log buffer

## Must run following command to make batch executable: chmod a+x test_module.sh
## Must edit this batch by inserting the name of the module you want to test
## Must also ensure the Makefile is modified for the same module
## Then run the executable: ./test_module.sh
echo "Running module...."
make
sudo insmod linear_traversal.ko
sudo rmmod linear_traversal
sudo dmesg -c
make clean
