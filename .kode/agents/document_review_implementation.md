# Document Review Sub-Agent Implementation Guide

## Core Implementation Components

### 1. PPT to Markdown Converter with Chunked Processing

**Input Processing with Context Management**:
```python
def convert_ppt_to_markdown(ppt_file_path, output_md_path):
    """
    Convert PowerPoint presentation to markdown format with chunked processing
    """
    # Extract slides using python-pptx or similar library
    slides = extract_slides(ppt_file_path)
    
    markdown_content = []
    markdown_content.append(f"# {get_presentation_title(slides)}")
    
    # Process slides in chunks of 5 to manage context
    batch_size = 5
    for batch_start in range(0, len(slides), batch_size):
        batch_end = min(batch_start + batch_size, len(slides))
        batch_slides = slides[batch_start:batch_end]
        
        # Add compact command between batches
        if batch_start > 0:
            markdown_content.append("<!-- /compact -->")
        
        for i, slide in enumerate(batch_slides, batch_start + 1):
            slide_content = process_slide(slide)
            markdown_content.append(f"\n## Slide {i}: {slide.title}")
            markdown_content.append(slide_content)
            
            # Handle images
            for image in slide.images:
                markdown_content.append(f"![{image.alt_text}]({image.path})")
    
    save_markdown(markdown_content, output_md_path)
```

**Slide Processing**:
```python
def process_slide(slide):
    """Process individual slide content"""
    content = []
    
    # Process text boxes
    for shape in slide.shapes:
        if hasattr(shape, "text"):
            text = clean_text(shape.text)
            if is_title(shape):
                content.append(f"### {text}")
            elif is_bullet_list(shape):
                content.extend(process_bullets(text))
            else:
                content.append(text)
    
    return "\n".join(content)
```

### 2. Documentation Gap Analyzer with Chunked Processing

**Code Repository Scanner with Context Management**:
```python
def scan_compilation_module():
    """Scan WAMR compilation module for features and APIs with chunked processing"""
    features = {}
    
    # Scan core compilation directory in batches
    compilation_path = "core/iwasm/compilation/"
    compilation_files = glob.glob(f"{compilation_path}**/*.c") + glob.glob(f"{compilation_path}**/*.h")
    
    # Process files in batches of 10
    batch_size = 10
    for batch_start in range(0, len(compilation_files), batch_size):
        batch_end = min(batch_start + batch_size, len(compilation_files))
        batch_files = compilation_files[batch_start:batch_end]
        
        for file_path in batch_files:
            file_features = extract_features_from_file(file_path)
            features.update(file_features)
        
        # Clear intermediate context between batches
        if batch_start + batch_size < len(compilation_files):
            print("<!-- /compact -->")  # Signal for context clearing
    
    # Scan test directory in batches
    test_path = "tests/unit/compilation/"
    test_files = glob.glob(f"{test_path}**/*.cc") + glob.glob(f"{test_path}**/*.h")
    
    for batch_start in range(0, len(test_files), batch_size):
        batch_end = min(batch_start + batch_size, len(test_files))
        batch_files = test_files[batch_start:batch_end]
        
        for file_path in batch_files:
            test_features = extract_test_features(file_path)
            features.update(test_features)
        
        if batch_start + batch_size < len(test_files):
            print("<!-- /compact -->")  # Signal for context clearing
    
    return features

def extract_features_from_file(file_path):
    """Extract compilation features from source file with line chunking"""
    features = {}
    
    with open(file_path, 'r') as f:
        lines = f.readlines()
        
        # Process file in chunks of 500 lines
        chunk_size = 500
        for chunk_start in range(0, len(lines), chunk_size):
            chunk_end = min(chunk_start + chunk_size, len(lines))
            chunk_content = ''.join(lines[chunk_start:chunk_end])
            
            # Extract function definitions
            functions = re.findall(r'\w+\s+\w+\([^)]*\)', chunk_content)
            for func in functions:
                features[func] = {
                    'file': file_path,
                    'type': 'function',
                    'implemented': True
                }
            
            # Extract data structures
            structs = re.findall(r'typedef\s+struct\s+\w+', chunk_content)
            for struct in structs:
                features[struct] = {
                    'file': file_path,
                    'type': 'structure',
                    'implemented': True
                }
    
    return features
```

**Documentation Parser with Chunked Processing**:
```python
def parse_documentation(doc_paths):
    """Parse existing documentation for documented features with chunked processing"""
    documented_features = {}
    
    # Process documents in batches
    batch_size = 3  # Process 3 documents at a time
    for batch_start in range(0, len(doc_paths), batch_size):
        batch_end = min(batch_start + batch_size, len(doc_paths))
        batch_docs = doc_paths[batch_start:batch_end]
        
        for doc_path in batch_docs:
            # Read and process document in chunks
            with open(doc_path, 'r') as f:
                lines = f.readlines()
                
                # Process 1000 lines at a time
                chunk_size = 1000
                for chunk_start in range(0, len(lines), chunk_size):
                    chunk_end = min(chunk_start + chunk_size, len(lines))
                    chunk_content = ''.join(lines[chunk_start:chunk_end])
                    
                    # Extract documented APIs and features
                    api_matches = re.findall(r'`(\w+\([^)]*\))`', chunk_content)
                    for api in api_matches:
                        documented_features[api] = {
                            'doc_file': doc_path,
                            'documented': True
                        }
        
        # Clear context between document batches
        if batch_start + batch_size < len(doc_paths):
            print("<!-- /compact -->")  # Signal for context clearing
    
    return documented_features
```

**Gap Analysis Engine**:
```python
def analyze_documentation_gaps(implemented_features, documented_features):
    """Identify gaps between implementation and documentation"""
    gaps = {
        'high_priority': [],
        'medium_priority': [],
        'low_priority': []
    }
    
    for feature, details in implemented_features.items():
        if feature not in documented_features:
            # Prioritize based on feature type and usage
            priority = prioritize_gap(feature, details)
            gaps[priority].append({
                'feature': feature,
                'file': details['file'],
                'type': details['type']
            })
    
    return gaps

def prioritize_gap(feature, details):
    """Determine priority level for documentation gap"""
    if details['type'] == 'function':
        # High priority for public APIs
        if is_public_api(feature):
            return 'high_priority'
        # Medium for internal functions
        elif is_internal_function(feature):
            return 'medium_priority'
    
    return 'low_priority'
```

### 3. Report Generator

**Markdown Report Format**:
```python
def generate_gap_report(gaps, coverage_stats):
    """Generate comprehensive gap analysis report"""
    report = []
    
    report.append("# WAMR Compilation Module Documentation Gap Analysis")
    report.append(f"\nGenerated: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")
    
    # Summary section
    report.append("\n## Summary")
    report.append(f"- Total Features Found: {coverage_stats['total_features']}")
    report.append(f"- Documented Features: {coverage_stats['documented_features']}")
    report.append(f"- Documentation Coverage: {coverage_stats['coverage_percentage']:.1f}%")
    
    # Gap details
    for priority in ['high_priority', 'medium_priority', 'low_priority']:
        if gaps[priority]:
            report.append(f"\n## {priority.replace('_', ' ').title()}")
            for gap in gaps[priority]:
                report.append(f"\n### {gap['feature']}")
                report.append(f"- **Type**: {gap['type']}")
                report.append(f"- **Location**: {gap['file']}")
    
    # Recommendations
    report.append("\n## Recommendations")
    report.append("1. Document all high-priority APIs and functions")
    report.append("2. Add usage examples for complex features")
    report.append("3. Update documentation when new features are added")
    
    return "\n".join(report)
```

## Integration with Main Agent

### Task Delegation Interface with Context Management
```python
class DocumentReviewAgent:
    def __init__(self):
        self.compilation_paths = [
            "core/iwasm/compilation/",
            "tests/unit/compilation/"
        ]
    
    def handle_task(self, task_type, **kwargs):
        if task_type == "ppt_conversion":
            return self.convert_ppt(**kwargs)
        elif task_type == "gap_analysis":
            return self.analyze_gaps(**kwargs)
    
    def convert_ppt(self, input_file, output_file):
        """Delegate PPT conversion task with context management"""
        # Use /compact for large files
        if get_file_size(input_file) > 10 * 1024 * 1024:  # 10MB
            return "Use /compact command for large presentations"
        
        # Process in chunks with compact commands
        result = convert_ppt_to_markdown(input_file, output_file)
        print("<!-- /compact -->")  # Clear context after processing
        return result
    
    def analyze_gaps(self, doc_paths, output_file):
        """Delegate documentation gap analysis with context management"""
        # Clear context before starting analysis
        print("<!-- /compact -->")
        
        implemented = scan_compilation_module()
        
        # Clear context between phases
        print("<!-- /compact -->")
        
        documented = parse_documentation(doc_paths)
        gaps = analyze_documentation_gaps(implemented, documented)
        
        coverage_stats = calculate_coverage(implemented, documented)
        report = generate_gap_report(gaps, coverage_stats)
        
        with open(output_file, 'w') as f:
            f.write(report)
        
        return f"Gap analysis complete: {output_file}"
```

## Usage Examples with Context Management

### PPT Conversion with Chunking
```bash
# Main agent delegates PPT conversion with context management
/compact
Task: Convert presentation.pptx to markdown format
Input: ./presentations/wamr_compilation.pptx
Output: ./docs/review/wamr_compilation.md
Processing: 5 slides per batch with /compact between batches
```

### Documentation Review with Context Management
```bash
# Main agent delegates gap analysis with context management
/compact  
Task: Analyze documentation gaps for compilation module
Documentation: ./docs/compilation/*.md
Code: ./core/iwasm/compilation/, ./tests/unit/compilation/
Output: ./reports/compilation_gaps.md
Processing: 10 files per batch with /compact between phases
```

## Configuration and Dependencies

### Required Dependencies
- `python-pptx` for PPT processing
- Regular expression libraries for text parsing
- File system utilities for directory scanning

### Context Management Parameters
- Maximum file size for processing: 50MB
- Processing batch size: 5 slides or 10 files per batch
- Line chunking: 500-1000 lines per chunk
- Context clearing threshold: Use /compact at 80% capacity

This implementation provides a complete framework for the document review sub-agent to handle PPT conversion and documentation gap analysis tasks efficiently while managing context limitations through strategic chunking and compact commands.