# Copyright (C) 2019 Intel Corporation.  All rights reserved.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

cmake_minimum_required(VERSION 3.14)

project(enhanced-coverage-report)

add_subdirectory(posix)
# Add AOT enhanced coverage tests
add_subdirectory(aot)
add_subdirectory(runtime-common)
# Add enhanced unit test subdirectories
add_subdirectory(interpreter)
add_subdirectory(aot-arch)
add_subdirectory(shared-utils)
add_subdirectory(thread-mgr)
