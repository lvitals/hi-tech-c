#!/bin/sh
# set -x

# Script to build test.com from test.c
# Ensure required tools are in the correct paths

# Step 1: Preprocess the source file
echo "Step 1: Preprocess the source file"
../cpp/cpp -Dcpm -Dhi_tech_c -Dz80 -I ../share/include/ -L ../share/lib/  test.c test1.tmp || {
    echo "Preprocessing failed!"
    exit 1
}

# Step 2: Run CP/M compiler
echo "Step 2: Run CP/M compiler"
../p1/p1 test1.tmp test2.tmp || {
    echo "CP/M compilation failed!"
    exit 1
}

# Step 3: Generate assembly code
echo "Step 3: Generate assembly code"
../cgen/cgen test2.tmp test1.tmp || {
    echo "Code generation failed!"
    exit 1
}

# Step 4: Apply optimization to the generated code (this is the new step)
echo "Step 4: Apply optimization"
../optim/optim test1.tmp test2.tmp || {
    echo "Optimization failed!"
    exit 1
}

# Step 5: Assemble the code
echo "Step 5: Assemble the code"
../zas/zas -n -otest.obj test2.tmp || {
    echo "Assembly failed!"
    exit 1
}

# Step 6: Link the object files
echo "Step 6: Link the object files"
../link/link -z -ptext=0,data,bss -c100h -otest.com ../share/lib/crtcpm.obj test.obj ../share/lib/libc.lib ../share/lib/libf.lib || {
    echo "Linking failed!"
    exit 1
}

# Step 7: Run the program on CP/M
echo "Step 7: Run the program on CP/M"
../cpm/cpm test.com || {
    echo "Execution on CP/M failed!"
    exit 1
}

echo "Build successful! Output: test.com"
