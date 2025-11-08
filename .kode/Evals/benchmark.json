{
    "benchmark_system": "Performance Metrics Definition",
    "metrics": [
        {
            "category": "Correctness",
            "metrics": [
                {
                    "metric": "Compile Success Rate (CSR)",
                    "definition": "Ratio of compilable tests",
                    "evaluation_method": "compiled / total"
                },
                {
                    "metric": "Execution Pass Rate (EPR)",
                    "definition": "Ratio of passing tests",
                    "evaluation_method": "passed / compiled"
                },
                {
                    "metric": "Logic Validity Score (LVS)",
                    "definition": "Scored 0-1 by Review Agent or human (logical soundness, relevance, non-triviality)",
                    "evaluation_method": "√-"
                }
            ]
        },
        {
            "category": "Coverage",
            "metrics": [
                {
                    "metric": "Line Coverage (LC)",
                    "definition": "% of source lines executed",
                    "evaluation_method": "from gcov/lcov"
                },
                {
                    "metric": "Branch Coverage (BC)",
                    "definition": "% of branches covered",
                    "evaluation_method": "from gcov/lcov"
                },
                {
                    "metric": "Function Coverage (FC)",
                    "definition": "% of functions covered",
                    "evaluation_method": "from gcov/lcov"
                }
            ]
        },
        {
            "category": "Effectiveness",
            "metrics": [
                {
                    "metric": "Bug Detection Rate (BDR)",
                    "definition": "Detected real or injected bugs",
                    "evaluation_method": "found / injected"
                },
                {
                    "metric": "Assertion Density (AD)",
                    "definition": "Assertions per test function",
                    "evaluation_method": "#asserts / #tests"
                },
                {
                    "metric": "Mutation Score (MS)",
                    "definition": "% of mutants killed in mutation testing",
                    "evaluation_method": "via mutmut or mull"
                }
            ]
        },
        {
            "category": "Maintainability",
            "metrics": [
                {
                    "metric": "Readability Score (RS)",
                    "definition": "Static-analysis readability metric",
                    "evaluation_method": "from clang-tidy/pvlint(0-10)"
                },
                {
                    "metric": "Cyclomatic Simplicity (CS)",
                    "definition": "Inverse of average cyclomatic complexity",
                    "evaluation_method": "1 / complexity"
                },
                {
                    "metric": "Consistency Index (CI)",
                    "definition": "Naming, format, and style consistency",
                    "evaluation_method": "Auto-scored 0-1"
                }
            ]
        },
        {
            "category": "Efficiency",
            "metrics": [
                {
                    "metric": "Generation Latency (GL)",
                    "definition": "Time to produce first runnable test",
                    "evaluation_method": "seconds"
                },
                {
                    "metric": "Repair Iteration Count (RIC)",
                    "definition": "Avg. number of repair loops per test",
                    "evaluation_method": "integer"
                },
                {
                    "metric": "Resource Cost (RC)",
                    "definition": "Tokens or GPU time per test",
                    "evaluation_method": "normalized cost"
                }
            ]
        }
    ],
    "purpose": "Evaluating Multi-Agent systems across key testing dimensions"
}
