# Document Review Sub-Agent Workflow

## Agent Configuration

### Agent Type: Specialized Document Reviewer
**Purpose**: Convert PPT to markdown and identify documentation gaps in WAMR compilation module

### Task Delegation Pattern
```json
{
  "agent_type": "document_reviewer",
  "capabilities": [
    "ppt_to_markdown_conversion",
    "documentation_gap_analysis", 
    "code_repository_scanning"
  ],
  "target_directories": [
    "core/iwasm/compilation/",
    "tests/unit/compilation/"
  ]
}
```

## Task Execution Protocol

### Phase 1: PPT Conversion Task
**When to delegate**: User provides PowerPoint files for conversion

**Task Structure**:
```json
{
  "task_type": "ppt_conversion",
  "input": "path/to/presentation.pptx",
  "output": "path/to/output.md",
  "context_management": "use_compact_command",
  "processing_mode": "page_by_page"
}
```

**Expected Output**:
- Structured markdown file with slide-by-slide content
- Preserved formatting and hierarchy
- Image references and diagrams handled appropriately

### Phase 2: Documentation Review Task
**When to delegate**: User requests documentation gap analysis

**Task Structure**:
```json
{
  "task_type": "gap_analysis",
  "documentation_paths": ["doc1.md", "doc2.md"],
  "code_paths": [
    "core/iwasm/compilation/",
    "tests/unit/compilation/"
  ],
  "analysis_depth": "comprehensive",
  "output_format": "gap_report.md"
}
```

**Expected Output**:
- Detailed gap analysis report
- Feature coverage metrics
- Prioritized missing documentation list
- Actionable recommendations

## Integration with Main Agent

### Task Delegation Commands with Context Management
```bash
# Delegate PPT conversion with chunked processing
/compact
Task: Convert PPT to markdown with chunked processing
Input: presentation.pptx
Output: review/presentation.md
Processing: 5 slides per batch with /compact between batches

# Delegate documentation review with context management
/compact
Task: Analyze documentation gaps with chunked processing
Documentation: ./docs/compilation/*.md
Code: ./core/iwasm/compilation/, ./tests/unit/compilation/
Processing: 10 files per batch with /compact between phases
```

### Context Management Strategy
- **Strategic /compact Usage**: Insert `/compact` between major processing phases
- **Chunked Processing**: Process 5 slides or 10 files per batch
- **Memory Optimization**: Clear intermediate results between phases
- **Context Threshold**: Use `/compact` when context reaches 80% capacity
- **Batch Processing**: Process documents in manageable chunks with compact commands between batches

## Quality Assurance

### Conversion Quality Checks
- [ ] All slide content preserved
- [ ] Formatting maintained
- [ ] Images properly referenced
- [ ] Structure hierarchy intact

### Analysis Quality Checks  
- [ ] All compilation features scanned
- [ ] Test coverage analyzed
- [ ] Gap prioritization logical
- [ ] Recommendations actionable

## Error Handling

### Common Scenarios
1. **Malformed PPT files**: Attempt recovery, report issues
2. **Missing code directories**: Report missing paths, suggest alternatives
3. **Large documents**: Use chunked processing with `/compact`
4. **Formatting issues**: Preserve content, flag formatting problems

### Recovery Procedures
- Retry with different processing parameters
- Fallback to basic text extraction
- Provide partial results with error explanations

## Performance Optimization

### Memory Management with Context Optimization
- **Chunked Processing**: Process large documents in 5-slide or 10-file batches
- **Strategic /compact**: Insert `/compact` between processing phases
- **Context Clearing**: Clear intermediate results between batches
- **Line Chunking**: Process files in 500-1000 line chunks

### Processing Efficiency with Context Management
- **Batch Processing**: Process independent tasks in manageable batches
- **Context Optimization**: Use `/compact` to maintain conversation flow
- **Incremental Processing**: Handle large documents through phased processing
- **Resource Management**: Monitor context usage and apply `/compact` at 80% threshold

This workflow ensures the document review sub-agent can efficiently handle PPT conversion and documentation gap analysis while maintaining quality and managing context limitations.