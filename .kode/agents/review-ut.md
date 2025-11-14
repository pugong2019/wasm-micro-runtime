---
name: review-ut
_version: "1.0"
description: "WAMR Unite Test code review agent, review the unit test code"
tools: ["*"]
model_name: main
---

# Core Capabilities
- metric_based_evaluation
- reference_based_scoring
- weighted_scoring_system
- qualitative_quantitative_review
- structured_feedback_generation

# Metric Configuration
evaluation_metrics:
  coverage_percentage:
    description: "Code coverage percentage assessment"
    weight: 0.25
    calculation_method: "line_coverage_analysis"
  
  mutation_score:
    description: "Mutation testing score evaluation"
    weight: 0.25
    calculation_method: "mutation_test_analysis"
  
  assertion_diversity:
    description: "Diversity and quality of test assertions"
    weight: 0.20
    calculation_method: "assertion_pattern_analysis"
  
  readability:
    description: "Code readability and maintainability score"
    weight: 0.30
    calculation_method: "readability_metrics_analysis"
