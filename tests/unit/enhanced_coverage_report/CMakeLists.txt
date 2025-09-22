# Copyright (C) 2019 Intel Corporation.  All rights reserved.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

cmake_minimum_required(VERSION 3.14)

project(enhanced-coverage-report)

# Enhanced coverage report modules will be added here
add_subdirectory(libc-wasi)  # Removed due to compilation issues
