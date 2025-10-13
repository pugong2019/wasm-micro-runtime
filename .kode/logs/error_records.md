# build error 1:
/home/chengnie/works/wasm-micro-runtime/tests/unit/compilation/build/_deps/googletest-src/googlemock/src/gmock-all.cc:42:10: fatal error: src/gmock-cardinalities.cc: No such file or directory
   42 | #include "src/gmock-cardinalities.cc"
      |          ^~~~~~~~~~~~~~~~~~~~~~~~~~~~
compilation terminated.
gmake[2]: *** [compilation/CMakeFiles/compilation_test.dir/build.make:230: compilation/CMakeFiles/compilation_test.dir/build/_deps/googletest-src/googlemock/src/gmock-all.cc.o] Error 1
gmake[1]: *** [CMakeFiles/Makefile2:1420: compilation/CMakeFiles/compilation_test.dir/all] Error 2
gmake: *** [Makefile:146: all] Error 2