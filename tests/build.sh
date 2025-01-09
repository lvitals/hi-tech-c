#!/bin/sh
# set -x

# Script to build HELLO.COM from HELLO.C
# Ensure required tools are in the correct paths

# Step 1: Preprocess the source file
echo "Step 1: Preprocess the source file"
../cpp/cpp -DCPM -DHI_TECH_C -Dz80 -I HELLO.C HELLO1.TMP || {
    echo "Preprocessing failed!"
    exit 1
}

# Step 2: Run CP/M compiler
echo "Step 2: Run CP/M compiler"
../p1/p1 HELLO1.TMP HELLO2.TMP || {
    echo "CP/M compilation failed!"
    exit 1
}

echo "Step 3: Generate assembly code"
# Step 3: Generate assembly code
../cgen/cgen HELLO2.TMP HELLO1.TMP || {
    echo "Code generation failed!"
    exit 1
}

echo "Step 4: Assemble the code"
# Step 4: Assemble the code
../zas/zas -N -oHELLO.OBJ HELLO1.TMP || {
    echo "Assembly failed!"
    exit 1
}

echo "Step 5: Link the object files"
# Step 5: Link the object files
../link/link -Z -Ptext=0,data,bss -C100H -OHELLO.COM CRTCPM.OBJ HELLO.OBJ LIBC.LIB || {
    echo "Linking failed!"
    exit 1
}

echo "Build successful! Output: HELLO.COM"

../cpm/cpm HELLO.COM
