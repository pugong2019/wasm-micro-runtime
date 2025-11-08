## plan-executor_ds 的说明
This agent ensures systematic, high-quality execution of coverage improvement plans with proper WAT file integration and comprehensive error handling.

在kode中的使用示例：
### Execute Full Plan:
```
@run-agent-plan_executor /path/to/plan.md
```

### Execute Specific Step:
```
@run-agent-plan_executor /path/to/plan.md Step_2
```

### Example Usage:
```
@run-agent-plan_executor tests/unit/aot/aot_coverage_improve_plan.md Step_1
```

### High-level Prompt
```
Do following actions step-by-step.
1. call /scan-target command for fodler `works/wasm-micro-runtime/core/iwasm/common/gc`
2. call @code-generator agent for `xxx_target_plan.md` by step-1 generated
3. call @review-ut agents for all generated `xxx_test.c` or `xxx_test.cpp` code
4. summay the works

```
