# YAML Parser

This directory contains YAML file parsing utilities.

## Contents

- **YAML Reader**: Read and parse YAML files
- **YAML Writer**: Write data to YAML files
- **YAML to JSON**: Convert YAML to JSON format
- **JSON to YAML**: Convert JSON to YAML format
- **YAML Validator**: Validate YAML syntax
- **YAML Merge**: Merge multiple YAML files
- **Multi-Document YAML**: Handle multiple documents in one file
- **YAML Anchors & Aliases**: Handle references
- **Safe Loading**: Secure YAML parsing
- **Custom Tag Handling**: Process custom YAML tags

## Features

- Support for YAML 1.1 and 1.2
- Complex data structure handling
- Comment preservation
- Secure parsing (avoid code execution)
- Stream processing for large files

## Usage Examples

```python
# Load YAML file
config = loadYAML('config.yaml')

# Write YAML file
saveYAML('output.yaml', data)

# Convert to JSON
json_data = yamlToJSON(yaml_string)
```
