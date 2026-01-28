# XML Parser

This directory contains XML file parsing and manipulation utilities.

## Contents

- **XML Reader**: Read and parse XML files
- **XML Writer**: Write data to XML files
- **XML to JSON**: Convert XML to JSON format
- **JSON to XML**: Convert JSON to XML format
- **XML Validator**: Validate XML against DTD/XSD
- **XPath Query**: Query XML using XPath expressions
- **XML Node Manipulation**: Add, remove, update nodes
- **XML Attribute Handling**: Manage XML attributes
- **XML Namespace Handling**: Handle XML namespaces
- **Pretty Print**: Format XML with indentation
- **SOAP Parser**: Parse SOAP XML messages

## Features

- DOM and SAX parsing support
- XSD schema validation
- XSLT transformation support
- Encoding handling (UTF-8, UTF-16, etc.)
- Error handling and recovery

## Usage Examples

```python
# Parse XML file
doc = parseXML('data.xml')

# Query with XPath
nodes = doc.xpath('//book[@category="web"]')

# Convert to JSON
json_data = xmlToJSON(doc)
```
