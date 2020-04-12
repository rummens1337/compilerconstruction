#!/usr/bin/env bash
CIVAS="civas"
CIVVM="civvm"
CIVCC="civcc"
CFLAGS=""

file="/home/hamza/Documents/compiler_construction/test/basic/functional/bool_op.cvc"
$CIVCC $CGLAGS -o tmp.s $file

echo -------------------------------

$CIVAS tmp.s -o tmp.o

echo -------------------------------

$CIVVM tmp.o
# mv tmp.out tmp.res &&
# diff tmp.res $expect_file --side-by-side --ignore-space-change > tmp.out 2>&1