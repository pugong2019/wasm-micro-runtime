---
name: review-code
description: A code reviewer to check if necessary contents are fulfild
aliases: [rc, review-code]
argNames: [file_name]
enabled: true
hidden: false
progressMessage: reviewing the {file_name} code...
---

## Step 1: Check if test files 
- review for {file_name}. refer info by ut file: "aot_compiler_test.cc"
- summary the "Required Fixes" or similar conclusion to a "TODO List"
## Step 2: Do the necessary fix
- execute "TODO List" step-by-step
## Step 3: Generate a report file
- generate a report file as xxx_review_report.md, xxx shall refer to the prefix of {file_name}