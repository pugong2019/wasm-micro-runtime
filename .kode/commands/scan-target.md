---
name: scan-target
description: scan for target folder
aliases: [scan-target, st]
argNames: [target_folder, ut_folder, module]
enabled: true
hidden: false
progressMessage: scan target folder for all c/c++ source files
---
# step 1
- list all target `xxx.c` and `xxx.cpp` files under {target_folder}
- add check-box for each file name as a task plan checker
- update all these information into file `{module}_target_plan.md` under {ut_folder}

# step 2
- read `{module}_target_plan.md`, scan {ut_folder}, check if any 
  unit test file missing, if the file exist, please mark √ in `{module}_target_plan.md`, 
  otherwise, leave it as no touch