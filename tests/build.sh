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

# Step 3: Generate assembly code
echo "Step 3: Generate assembly code"
../cgen/cgen HELLO2.TMP HELLO1.TMP || {
    echo "Code generation failed!"
    exit 1
}

# Step 4: Apply optimization to the generated code (this is the new step)
echo "Step 4: Apply optimization"
../optim/optim HELLO1.TMP HELLO2.TMP || {
    echo "Optimization failed!"
    exit 1
}

# Step 5: Assemble the code
echo "Step 5: Assemble the code"
../zas/zas -N -oHELLO.OBJ HELLO2.TMP || {
    echo "Assembly failed!"
    exit 1
}

# Step 6: Link the object files
echo "Step 6: Link the object files"
../link/link -Z -Ptext=0,data,bss -C100H -OHELLO.COM CRTCPM.OBJ HELLO.OBJ LIBC.LIB || {
    echo "Linking failed!"
    exit 1
}

# Step 7: Run the program on CP/M
echo "Step 7: Run the program on CP/M"
../cpm/cpm HELLO.COM || {
    echo "Execution on CP/M failed!"
    exit 1
}

echo "Build successful! Output: HELLO.COM"
