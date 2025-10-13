---
name: build-and-execute-test
description: A command that used to build and execute all unit test code
aliases: [b-all, build-unit_test]
argNames: []
enabled: true
hidden: false
progressMessage: build and test all unit test case...
---

## Execute below bash command one-by-one
``` bash
cd /wasm-micro-runtime/tests/unit
pwd /wasm-micro-runtime/tests/unit

# generate llvm (only needed at first time)
python3 ./build-scripts/build_llvm.py

# make
cmake -S . -B build -DCOLLECT_CODE_COVERAGE=1

# (makesure execute this before: 
#  cd /xxx/wasm-micro-runtime && python3 ./build-scripts/build_llvm.py)

# build
cmake --build build 

# execute
ctest --test-dir build

# generate report
../wamr-test-suites/spec-test-script/collect_coverage.sh unit.lcov ./build/

# show coverage info
lcov --summary ./build/unit.lcov
lcov --list build/unit.lcov
```