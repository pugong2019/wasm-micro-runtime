---
name: deep-research
description: A research command for analyze how to enhance the test coverage with human-in-the-loop
aliases: [deep-r]
argNames: [source_file, ut_module_path]
enabled: true
hidden: false
progressMessage: research on going...
---

# Step-by-Step Actions
1. **Human-in-the-Loop Confirmation**: Ask user to confirm before proceeding
2. review {source_file}, and realted test case `xxx_test.c`, refer to `Code Analysis.yaml`, understand current test case coverage and what has not been covered, list all the research result under {ut_module_path} \ `current_coverage.report`

3. read `current_coverage.report`, and learn nencessary knowledge at `test_case_generate_guide.md`, `works/wasm-micro-runtime/.kode/Prompt/code_generation_prompt.yaml`, summary the analyze result to {ut_module_path}\ `coverage_plan.report`

4. read `coverage_plan.report`, generate enhanced test case into `xxx_test.c`

# Principle
- Always generate a Todo List before take action
- Using structured format to output information
- **MUST get user confirmation before starting analysis**