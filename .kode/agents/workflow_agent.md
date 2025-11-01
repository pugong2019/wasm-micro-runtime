# Workflow Agent - Multi-Agent Orchestrator

## Purpose
This workflow agent coordinates and executes multiple specialized sub-agents in sequence to automate complex WAMR testing and documentation workflows.

## Architecture

### Agent Registry
```json
{
  "plan_designer": {
    "type": "test_plan_designer",
    "capabilities": ["test_strategy", "coverage_planning", "feature_analysis"],
    "input": "module_name",
    "output": "test_plan.md"
  },
  "plan_executor": {
    "type": "test_implementation", 
    "capabilities": ["code_generation", "test_execution", "coverage_validation"],
    "input": "test_plan.md",
    "output": "test_results.json"
  },
  "document_reviewer": {
    "type": "documentation_analysis",
    "capabilities": ["ppt_conversion", "gap_analysis", "report_generation"],
    "input": ["docs", "code_paths"],
    "output": "gap_report.md"
  }
}
```

## Workflow Templates

### 1. Full Test Coverage Workflow
```yaml
workflow_id: "test_coverage_full"
steps:
  1:
    agent: "plan_designer"
    task: "Design comprehensive test plan for compilation module"
    input: "compilation"
    output: "compilation_test_plan.md"
    
  2:
    agent: "plan_executor"  
    task: "Implement test cases from plan"
    input: "compilation_test_plan.md"
    output: "test_results.json"
    
  3:
    agent: "document_reviewer"
    task: "Analyze documentation gaps"
    input: 
      docs: ["docs/compilation/*.md"]
      code_paths: ["core/iwasm/compilation/", "tests/unit/compilation/"]
    output: "documentation_gaps.md"
```

### 2. Documentation Review Workflow
```yaml
workflow_id: "doc_review_automated"
steps:
  1:
    agent: "document_reviewer"
    task: "Convert PPT to markdown"
    input: "presentation.pptx"
    output: "presentation.md"
    
  2:
    agent: "document_reviewer"
    task: "Analyze documentation completeness"
    input:
      docs: ["presentation.md", "existing_docs/*.md"]
      code_paths: ["core/iwasm/compilation/"]
    output: "completeness_report.md"
```

## Implementation Strategy

### Workflow Execution Engine
```python
class WorkflowAgent:
    def __init__(self):
        self.agents = self.load_agent_registry()
        self.workflows = self.load_workflow_templates()
    
    def execute_workflow(self, workflow_id, parameters):
        """Execute a predefined workflow"""
        workflow = self.workflows[workflow_id]
        results = {}
        
        for step_num, step_config in workflow['steps'].items():
            agent = self.agents[step_config['agent']]
            task_result = self.delegate_to_agent(agent, step_config, parameters)
            results[step_num] = task_result
            
            # Pass output to next step if needed
            if 'output' in step_config:
                parameters[step_config['output']] = task_result
        
        return results
    
    def delegate_to_agent(self, agent_config, step_config, parameters):
        """Delegate task to specific agent"""
        # Build task prompt based on agent type and step configuration
        task_prompt = self.build_task_prompt(agent_config, step_config, parameters)
        
        # Execute agent task
        if agent_config['type'] == 'plan_designer':
            return self.execute_plan_designer(task_prompt)
        elif agent_config['type'] == 'plan_executor':
            return self.execute_plan_executor(task_prompt)
        elif agent_config['type'] == 'document_reviewer':
            return self.execute_document_reviewer(task_prompt)
    
    def build_task_prompt(self, agent_config, step_config, parameters):
        """Build specific task prompt for each agent"""
        base_prompt = f"Task: {step_config['task']}\n"
        
        # Add input parameters
        if 'input' in step_config:
            if isinstance(step_config['input'], list):
                for input_item in step_config['input']:
                    base_prompt += f"Input {input_item}: {parameters.get(input_item, '')}\n"
            else:
                base_prompt += f"Input: {parameters.get(step_config['input'], '')}\n"
        
        # Add output specification
        if 'output' in step_config:
            base_prompt += f"Output: {step_config['output']}\n"
        
        return base_prompt
```

### Agent Execution Methods
```python
def execute_plan_designer(self, task_prompt):
    """Execute plan designer agent"""
    # Use Task tool to delegate to plan-designer
    result = Task(
        description="Design test plan",
        prompt=task_prompt,
        subagent_type="plan-designer"
    )
    return result

def execute_plan_executor(self, task_prompt):
    """Execute plan executor agent"""
    result = Task(
        description="Implement tests", 
        prompt=task_prompt,
        subagent_type="plan-executor"
    )
    return result

def execute_document_reviewer(self, task_prompt):
    """Execute document reviewer agent"""
    result = Task(
        description="Review documentation",
        prompt=task_prompt,
        subagent_type="document_reviewer"
    )
    return result
```

## Usage Examples

### Command Line Interface
```bash
# Execute full test coverage workflow
workflow_agent execute --workflow test_coverage_full --module compilation

# Execute documentation review workflow  
workflow_agent execute --workflow doc_review_automated --ppt presentation.pptx

# List available workflows
workflow_agent list-workflows

# Show workflow details
workflow_agent describe --workflow test_coverage_full
```

### Python API
```python
from workflow_agent import WorkflowAgent

agent = WorkflowAgent()

# Execute workflow
results = agent.execute_workflow(
    workflow_id="test_coverage_full",
    parameters={
        "module": "compilation",
        "target_coverage": "75%"
    }
)

print(f"Workflow completed with {len(results)} steps")
```

## Configuration

### Workflow Definition Files
Store workflows as YAML/JSON files in `.kode/agents/workflows/`:
- `test_coverage_full.yaml`
- `doc_review_automated.yaml` 
- `quick_validation.yaml`

### Agent Configuration
Each agent's capabilities and requirements defined in:
- `.kode/agents/agent_registry.json`
- `.kode/agents/agent_capabilities/`

## Benefits

1. **Automation**: Eliminates manual task delegation
2. **Consistency**: Standardized workflows across projects
3. **Reusability**: Predefined workflows for common scenarios
4. **Monitoring**: Track progress across multiple agents
5. **Error Handling**: Centralized error management

This workflow agent enables true multi-agent automation by coordinating specialized agents through predefined workflows.