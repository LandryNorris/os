
mkdir ../cmake-build-debug
cd ../cmake-build-debug && cmake -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ .. && cmake --build .
