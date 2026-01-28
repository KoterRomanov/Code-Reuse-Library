# Qt Widgets Utilities

This directory contains custom Qt widgets and widget enhancement utilities.

## Contents

### Custom Buttons
- **IconButton**: Button with icon and text
- **ToggleButton**: Two-state toggle button
- **SplitButton**: Button with dropdown menu
- **ColorButton**: Button for color selection
- **ImageButton**: Button with custom images

### Input Widgets
- **SearchLineEdit**: Line edit with search icon and clear button
- **PasswordLineEdit**: Line edit with show/hide password
- **NumericLineEdit**: Line edit for numeric input only
- **DateTimeEdit**: Enhanced date/time picker
- **AutoCompleteLineEdit**: Line edit with autocomplete

### Display Widgets
- **LoadingSpinner**: Animated loading indicator
- **ProgressRing**: Circular progress indicator
- **Badge**: Notification badge widget
- **Toast**: Temporary notification widget
- **Tag**: Label with close button

### Container Widgets
- **CollapsibleWidget**: Expandable/collapsible container
- **CardWidget**: Material design card widget
- **SidebarWidget**: Collapsible sidebar
- **TabBarWidget**: Enhanced tab bar
- **SplitterWidget**: Customized splitter

### Dialog Widgets
- **MessageDialog**: Custom message box
- **ConfirmDialog**: Confirmation dialog
- **InputDialog**: Input dialog with validation
- **ProgressDialog**: Long operation progress dialog
- **AboutDialog**: Application about dialog

## Features

- Modern, customizable styling
- Support for dark/light themes
- Animation support
- Responsive design
- Accessibility features
- Signal-slot integration

## Usage Example

```cpp
#include "SearchLineEdit.h"

SearchLineEdit *searchBox = new SearchLineEdit(this);
searchBox->setPlaceholderText("Search...");

connect(searchBox, &SearchLineEdit::textChanged, [](const QString &text) {
    // Perform search
    qDebug() << "Searching for:" << text;
});
```

## Styling

Widgets support Qt Style Sheets (QSS) for easy customization:

```cpp
button->setStyleSheet(R"(
    QPushButton {
        background-color: #007bff;
        color: white;
        border-radius: 4px;
        padding: 8px 16px;
    }
    QPushButton:hover {
        background-color: #0056b3;
    }
)");
```
