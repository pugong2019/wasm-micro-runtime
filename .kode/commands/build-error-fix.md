---
name: build-error-fix
description: A build-error-fix command
aliases: [bef, be-fix]
argNames: [error_info]
enabled: true
hidden: false
progressMessage: try to fix the build/execute error...
---

## fix error
search {error_info} at `./wasm-micro-runtime/tests/unit/build/Testing/Temporary/LastTest.log`, do following step:
1. make a todo list first
2. read the log chunk-by-chunk, in case too large to beyond model's maximum context length
3. analyze why some test cases failed
4. try to find a solution to fix them
5. summary all the fix and action in a report, put it under `works/wasm-micro-runtime/tests/unit/ai_generated_docs/`