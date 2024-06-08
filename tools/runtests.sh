#!/bin/sh

mkdir ../build-tests
cd ../build-tests

cmake -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ .. && cmake --build . && \

./kernel_test --gtest_output=xml:report/report-kernel.xml && \
./libc_test --gtest_output=xml:report/report-libc.xml && \
./libk_test --gtest_output=xml:report/report-libk.xml
