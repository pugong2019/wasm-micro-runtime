# Enhanced Unit Test CMakeLists.txt
cmake_minimum_required(VERSION 3.12)

add_subdirectory(posix)
# Add AOT enhanced coverage tests
add_subdirectory(aot)
add_subdirectory(runtime-common)
# Add enhanced unit test subdirectories
add_subdirectory(interpreter)
add_subdirectory(aot-arch)
add_subdirectory(shared-utils)
add_subdirectory(thread-mgr)
add_subdirectory(ssp)
