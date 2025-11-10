#!/bin/bash
# only need for first time clone repo
echo "***** check for current work dir*****"
cd tests/unit/
pwd
python3 ./build-scripts/build_llvm.py

#echo make all unit test case
echo "*****  cmake -S . -B build -DCOLLECT_CODE_COVERAGE=1... ***** "
cmake -S . -B build -DCOLLECT_CODE_COVERAGE=1

# make 
echo "*****  cmake --build build... ***** "
cmake --build build 

# build
echo "*****  ctest --test-dir build... ***** "
ctest --test-dir build

# coverage
echo "***** calculate coverage *****"
../wamr-test-suites/spec-test-script/collect_coverage.sh unit.lcov ./build/

lcov --summary ./build/unit.lcov
lcov --list build/unit.lcov