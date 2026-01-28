# Unit Test Templates

This directory contains templates and utilities for writing unit tests.

## Contents

- **Test Templates**: Boilerplate test code for different languages
- **Mock Objects**: Templates for creating mocks
- **Test Fixtures**: Reusable test data and setup
- **Assertion Helpers**: Custom assertion functions
- **Test Utilities**: Helper functions for testing
- **Parameterized Tests**: Data-driven test templates
- **Test Organization**: Best practices for structuring tests

## Testing Frameworks Covered

- **JavaScript/TypeScript**: Jest, Mocha, Jasmine
- **Python**: pytest, unittest, nose2
- **Java**: JUnit, TestNG
- **C#**: NUnit, xUnit, MSTest
- **Go**: testing package
- **Rust**: built-in test framework

## Test Structure (AAA Pattern)

```
// Arrange: Set up test data and conditions
const input = createTestData();

// Act: Execute the code being tested
const result = functionUnderTest(input);

// Assert: Verify the results
expect(result).toBe(expectedValue);
```

## Best Practices

- One assertion per test
- Clear test names
- Independent tests
- Fast execution
- Deterministic results
- Clean up after tests

## Usage Examples

```python
# Test template
def test_function_name_should_return_expected_value():
    # Arrange
    input_data = TestData.create_sample()
    expected = "expected_result"
    
    # Act
    result = function_under_test(input_data)
    
    # Assert
    assert result == expected
```
