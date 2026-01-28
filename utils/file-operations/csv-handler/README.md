# CSV Handler

This directory contains CSV file handling utilities.

## Contents

- **CSV Reader**: Read and parse CSV files
- **CSV Writer**: Write data to CSV files
- **CSV to JSON**: Convert CSV to JSON format
- **JSON to CSV**: Convert JSON to CSV format
- **CSV to Array**: Parse CSV to array/list
- **Array to CSV**: Convert array to CSV
- **Column Mapping**: Map CSV columns to objects
- **Large File Processing**: Stream-based CSV processing
- **Excel CSV**: Handle Excel-formatted CSV files
- **Custom Delimiters**: Support various delimiters (tab, semicolon, etc.)
- **Header Handling**: Process files with/without headers
- **Quote Handling**: Handle quoted fields with delimiters

## Features

- RFC 4180 compliance
- Encoding detection and conversion
- Memory-efficient streaming
- Custom data type conversion
- Error handling for malformed data

## Usage Examples

```python
# Read CSV file
data = readCSV('data.csv')

# Write CSV file
writeCSV('output.csv', data, headers=['name', 'age', 'email'])

# Stream large CSV
for row in streamCSV('large_file.csv'):
    process(row)
```
