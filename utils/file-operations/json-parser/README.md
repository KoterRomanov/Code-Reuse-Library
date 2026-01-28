# JSON Parser

This directory contains JSON configuration file parsing utilities.

## Contents

- **JSON Reader**: Read and parse JSON files
- **JSON Writer**: Write objects to JSON files
- **JSON Validator**: Validate JSON structure
- **JSON Schema Validation**: Validate against JSON schema
- **JSON Path Query**: Query JSON data using path expressions
- **JSON Merge**: Merge multiple JSON objects
- **JSON Diff**: Compare JSON objects
- **Pretty Print**: Format JSON with indentation
- **JSON to Object Mapping**: Convert JSON to language objects
- **Stream JSON Parser**: Parse large JSON files efficiently
- **JSON Comments**: Handle JSON with comments (JSON5)

## Features

- Error handling and validation
- Support for nested structures
- Large file handling
- Type-safe parsing
- Custom serialization/deserialization

## Usage Examples

```javascript
// Read JSON file
const config = readJSON('config.json');

// Write JSON file
writeJSON('output.json', data, { pretty: true });

// Validate JSON
if (validateJSON(jsonString)) {
    // Process valid JSON
}
```
