---
name: review-coverage
description: A single file ut coverage review to enhance the coverage
aliases: [rc, review-c]
argNames: [ut_file]
enabled: true
hidden: false
progressMessage: review/checking {ut_file} coverage ongoing...
---

## Review work
Execute below step-by-step:

### Step 1
read {ut_file}, then split/parse the string as two part
- part1:  generate a temp str as {souce_code_file_name}, it generate by {ut_file} remove the "_test.x", change it suffix as "_test.c"
- part2:  module_name as {ut_file}

### Step 2
Read the source code, which file name is similar as {ut_file}, but remove the "-test" part in the file name, Read the target UT code at {ut_file}

### Step 3
review {ut_file}, manage to make a coverage enhancement plan to increase the coverage for simd_bitwise_ops.c, name the coverage enhancement plan as "updated_enhancement_{ut_file}_plan.md"