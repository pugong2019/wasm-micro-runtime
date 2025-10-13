---
name: review-plan
description: A plan review to check if necessary contents are fulfild
aliases: [rp, review-plan]
argNames: [module_name, plan_name]
enabled: true
hidden: false
progressMessage: review/checking test coverage plan {plan_name} for {module_name} module targeting...
---

## Check Point 1: Check if test files 
- read every generated `xxx_test.c` file name in {plan_name}
- list the file which not have the same prefix name with original source code file

## Check Point 2: Update information based on `Check Point 1` result
- generate a new plan file, name it as `{plan_name}_update_by_review.md` with "SIMD Test File Naming ConventionAnalysis" result generate by `Check Point 1`
- Remove "Mismatched Test Files", add "Missing Test Files" with necessay test step as "Properly Matched Test Files" does.