#!/bin/bash
# only need for first time of init repo
#python3 ./build-scripts/build_llvm.py

echo "***** check for current work dir*****"
pwd

#echo make all unit test case
echo "*****  cmake -S . -B build -DCOLLECT_CODE_COVERAGE=1... ***** "
cmake -S . -B build -DCOLLECT_CODE_COVERAGE=1

# make 
echo "*****  cmake --build build... ***** "
mkdir ./build/logs
touch ./build/logs/cmake.log
cmake --build build 2>&1 | tee ./build/logs/cmake.log

# build
echo "*****  ctest --test-dir build... ***** "
mkdir ./build/logs
touch ./build/logs/ctest.log
ctest --test-dir build 2>&1 | tee ./build/logs/ctest.log

# coverage
echo "***** calculate coverage *****"
../wamr-test-suites/spec-test-script/collect_coverage.sh unit.lcov ./build/
unzip wamr-lcov.zip

# print coverage in stdout
#lcov --summary ./build/unit.lcov 
#lcov --list build/unit.lcov


# print SIMD module coverage
lcov --list build/unit.lcov | grep iwasm/compilation/simd/

#lcov --list build/unit.lcov | grep iwasm/compilation/simd/ > ./compilation/docs/ut_coverage/SIMD_coverage.log 2>&1

# lcov --capture --directory . --output-file step_coverage.info && genhtml step_coverage.info --output-directory step_coverage_report