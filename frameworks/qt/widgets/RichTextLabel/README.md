# RichTextLabel Widget

A Qt widget that extends QLabel to support multi-style text segments with different fonts, colors, and even clickable regions.

## Overview

`RichTextLabel` allows you to display text with different styling for each segment without manually constructing HTML. It's perfect for:
- Highlighting important numbers or keywords in a sentence
- Creating multi-colored text displays
- Adding clickable text segments with custom actions
- Status messages with varying emphasis

## Features

- ✅ **Multi-style segments**: Different fonts, sizes, colors per segment
- ✅ **Gradient colors**: Approximate gradient display (shown as middle color)
- ✅ **Clickable segments**: Add click handlers to specific text parts
- ✅ **Anti-aliasing**: Smooth, high-quality text rendering
- ✅ **Easy API**: Simple, chainable interface
- ✅ **HTML escaping**: Automatic escaping of special characters
- ✅ **Word wrap**: Automatic text wrapping support

## Installation

1. Copy `richtextlabel.h` and `richtextlabel.cpp` to your project
2. Add to your `.pro` file:
```qmake
HEADERS += richtextlabel.h
SOURCES += richtextlabel.cpp
```

Or for CMake:
```cmake
target_sources(your_target PRIVATE
    richtextlabel.h
    richtextlabel.cpp
)
```

## Basic Usage

### Example 1: Simple Colored Text

```cpp
#include "richtextlabel.h"

RichTextLabel *label = new RichTextLabel(this);

// Add segments with different styles
label->addSegment("Your computer has been secured for ", 
                  QFont("Arial", 12), Qt::black);
label->addSegment("100", 
                  QFont("Arial", 16, QFont::Bold), Qt::red);
label->addSegment(" days", 
                  QFont("Arial", 12), Qt::black);

// Update display
label->updateText();
```

**Result**: "Your computer has been secured for **<span style="color:red">100</span>** days"

### Example 2: Using Default Font

```cpp
RichTextLabel *label = new RichTextLabel(this);

// Set default font and color
label->setDefaultFont(QFont("Arial", 12));
label->setDefaultColor(Qt::black);

// Add segments (will use defaults)
label->addSegment("Regular text, ");
label->addSegment("colored text", Qt::blue);
label->addSegment(", and ");
label->addSegment("bold text", QFont("Arial", 12, QFont::Bold));

label->updateText();
```

### Example 3: Gradient Colors (Approximate)

```cpp
RichTextLabel *label = new RichTextLabel(this);

label->addSegment("Temperature: ", QFont("Arial", 12), Qt::black);

// Method 1: Using GradientInfo
GradientInfo gradient(QColor("#FF6B6B"), QColor("#4ECDC4"), 
                     GradientDirection::Horizontal);
label->addSegment("72°F", QFont("Arial", 16, QFont::Bold), gradient);

// Method 2: Using simplified method
label->addGradientSegment("72°F", QFont("Arial", 16, QFont::Bold),
                         QColor("#FF6B6B"), QColor("#4ECDC4"));

label->updateText();
```

**Note**: Due to Qt's QLabel limitations, gradients are approximated using the middle color between start and end colors.

### Example 4: Clickable Segments

```cpp
RichTextLabel *label = new RichTextLabel(this);

label->addSegment("By continuing, you agree to our ", 
                  QFont("Arial", 10), Qt::black);
label->addClickableSegment("Terms of Service", 
                           QFont("Arial", 10), Qt::blue, "terms");
label->addSegment(" and ", QFont("Arial", 10), Qt::black);
label->addClickableSegment("Privacy Policy", 
                           QFont("Arial", 10), Qt::blue, "privacy");

label->updateText();

// Connect click signal
connect(label, &RichTextLabel::segmentClicked, 
        [](const QString &clickId, const QString &text) {
    if (clickId == "terms") {
        // Open terms dialog
    } else if (clickId == "privacy") {
        // Open privacy dialog
    }
});
```

### Example 5: Dynamic Content

```cpp
RichTextLabel *label = new RichTextLabel(this);

// Update content dynamically
void updateStats(int downloads, double rating) {
    label->clearSegments();
    
    label->addSegment("Downloads: ");
    label->addSegment(QString::number(downloads), 
                      QFont("Arial", 14, QFont::Bold), Qt::green);
    label->addSegment(" | Rating: ");
    label->addSegment(QString::number(rating, 'f', 1), 
                      QFont("Arial", 14, QFont::Bold), Qt::orange);
    label->addSegment("/5.0");
    
    label->updateText();
}

// Call periodically
updateStats(12345, 4.7);
```

## API Reference

### Adding Segments

```cpp
// Basic segment with font and color
void addSegment(const QString &text, const QFont &font, const QColor &color);

// Segment with color only (uses default font)
void addSegment(const QString &text, const QColor &color);

// Segment with font only (uses default color)
void addSegment(const QString &text, const QFont &font);

// Plain segment (uses default font and color)
void addSegment(const QString &text);

// Gradient segment (approximate display)
void addSegment(const QString &text, const QFont &font, const GradientInfo &gradient);
void addGradientSegment(const QString &text, const QFont &font, 
                       const QColor &startColor, const QColor &endColor,
                       GradientDirection direction = GradientDirection::Horizontal);

// Clickable segment
void addClickableSegment(const QString &text, const QFont &font, 
                         const QColor &color, const QString &clickId);
void addClickableSegment(const QString &text, const QColor &color, 
                         const QString &clickId);
```

### Control Methods

```cpp
// Update display after adding/modifying segments
void updateText();

// Clear all segments
void clearSegments();

// Set/get default font
void setDefaultFont(const QFont &font);
QFont defaultFont() const;

// Set/get default color
void setDefaultColor(const QColor &color);
QColor defaultColor() const;

// Direct segment manipulation
void setSegments(const QList<TextSegment> &segments);
QList<TextSegment> segments() const;
```

### Signals

```cpp
// Emitted when a clickable segment is clicked
void segmentClicked(const QString &clickId, const QString &text);
```

## Advanced Usage

### Custom Styling

```cpp
RichTextLabel *label = new RichTextLabel(this);

// Create segment with multiple font properties
QFont font("Arial", 14);
font.setBold(true);
font.setItalic(true);
font.setUnderline(true);

label->addSegment("Styled Text", font, Qt::darkBlue);
label->updateText();
```

### Building Complex Layouts

```cpp
void createStatusMessage(RichTextLabel *label, const QString &user, int points) {
    label->clearSegments();
    
    label->addSegment("User ");
    label->addSegment(user, QFont("Arial", 12, QFont::Bold), Qt::blue);
    label->addSegment(" earned ");
    label->addSegment(QString::number(points), 
                      QFont("Arial", 14, QFont::Bold), Qt::green);
    label->addSegment(" points!");
    
    label->updateText();
}
```

### Performance Considerations

```cpp
// For frequently updated content, batch updates:
label->clearSegments();

// Add all segments
for (const auto &item : items) {
    label->addSegment(item.text, item.font, item.color);
}

// Single update call
label->updateText();  // Only rebuild HTML once
```

## Limitations

1. **Gradient Display**: True CSS gradients are not supported by QLabel's rich text renderer. Gradients are approximated using the middle color between start and end colors.

2. **Click Detection**: Click detection for segments is approximate and based on character position estimation.

3. **Text Selection**: Full text selection is enabled. To disable:
   ```cpp
   label->setTextInteractionFlags(Qt::NoTextInteraction);
   ```

## Complete Example Application

Here's a complete example application demonstrating all features:

```cpp
/**
 * @file main.cpp
 * @brief Complete example demonstrating RichTextLabel features
 */

#include "richtextlabel.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QLabel>

/**
 * @brief Example window demonstrating RichTextLabel features
 */
class ExampleWindow : public QWidget {
    Q_OBJECT

public:
    ExampleWindow(QWidget *parent = nullptr) : QWidget(parent) {
        setWindowTitle("RichTextLabel Examples");
        resize(600, 500);
        
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        
        // Example 1: Basic colored text
        createBasicExample(mainLayout);
        
        // Example 2: Gradient colors (approximate)
        createGradientExample(mainLayout);
        
        // Example 3: Clickable segments
        createClickableExample(mainLayout);
        
        // Example 4: Dynamic content
        createDynamicExample(mainLayout);
        
        setLayout(mainLayout);
    }

private:
    void createBasicExample(QVBoxLayout *layout) {
        QGroupBox *group = new QGroupBox("Example 1: Basic Colored Text", this);
        QVBoxLayout *groupLayout = new QVBoxLayout(group);
        
        RichTextLabel *label = new RichTextLabel(this);
        label->setDefaultFont(QFont("Arial", 12));
        label->setDefaultColor(Qt::black);
        
        label->addSegment("Your computer has been secured for ");
        label->addSegment("100", QFont("Arial", 18, QFont::Bold), Qt::red);
        label->addSegment(" days");
        label->updateText();
        
        groupLayout->addWidget(label);
        layout->addWidget(group);
    }
    
    void createGradientExample(QVBoxLayout *layout) {
        QGroupBox *group = new QGroupBox("Example 2: Gradient Colors (Approximate)", this);
        QVBoxLayout *groupLayout = new QVBoxLayout(group);
        
        RichTextLabel *label = new RichTextLabel(this);
        
        label->addSegment("Temperature: ", QFont("Arial", 12), Qt::black);
        
        // Using gradient (will display as middle color)
        label->addGradientSegment("72°F", QFont("Arial", 16, QFont::Bold),
                                 QColor("#FF6B6B"), QColor("#4ECDC4"),
                                 GradientDirection::Horizontal);
        
        label->updateText();
        
        groupLayout->addWidget(label);
        
        // Note about gradient limitation
        QLabel *note = new QLabel("Note: Gradients are approximated as middle color due to Qt limitations");
        note->setStyleSheet("color: gray; font-size: 9pt; font-style: italic;");
        groupLayout->addWidget(note);
        
        layout->addWidget(group);
    }
    
    void createClickableExample(QVBoxLayout *layout) {
        QGroupBox *group = new QGroupBox("Example 3: Clickable Segments", this);
        QVBoxLayout *groupLayout = new QVBoxLayout(group);
        
        RichTextLabel *label = new RichTextLabel(this);
        
        label->addSegment("By continuing, you agree to our ", QFont("Arial", 10), Qt::black);
        label->addClickableSegment("Terms of Service", QFont("Arial", 10, QFont::Bold), 
                                  Qt::blue, "terms");
        label->addSegment(" and ", QFont("Arial", 10), Qt::black);
        label->addClickableSegment("Privacy Policy", QFont("Arial", 10, QFont::Bold), 
                                  Qt::blue, "privacy");
        label->addSegment(".", QFont("Arial", 10), Qt::black);
        label->updateText();
        
        connect(label, &RichTextLabel::segmentClicked, this, &ExampleWindow::onLinkClicked);
        
        groupLayout->addWidget(label);
        layout->addWidget(group);
    }
    
    void createDynamicExample(QVBoxLayout *layout) {
        QGroupBox *group = new QGroupBox("Example 4: Dynamic Content", this);
        QVBoxLayout *groupLayout = new QVBoxLayout(group);
        
        // Create label
        dynamicLabel = new RichTextLabel(this);
        groupLayout->addWidget(dynamicLabel);
        
        // Create controls
        QHBoxLayout *controlLayout = new QHBoxLayout();
        
        QLabel *downloadsLabel = new QLabel("Downloads:", this);
        QSpinBox *downloadsSpinBox = new QSpinBox(this);
        downloadsSpinBox->setRange(0, 999999);
        downloadsSpinBox->setValue(12345);
        
        QLabel *ratingLabel = new QLabel("Rating:", this);
        QDoubleSpinBox *ratingSpinBox = new QDoubleSpinBox(this);
        ratingSpinBox->setRange(0.0, 5.0);
        ratingSpinBox->setSingleStep(0.1);
        ratingSpinBox->setValue(4.7);
        
        QPushButton *updateButton = new QPushButton("Update", this);
        
        controlLayout->addWidget(downloadsLabel);
        controlLayout->addWidget(downloadsSpinBox);
        controlLayout->addWidget(ratingLabel);
        controlLayout->addWidget(ratingSpinBox);
        controlLayout->addWidget(updateButton);
        controlLayout->addStretch();
        
        groupLayout->addLayout(controlLayout);
        
        // Connect update button
        connect(updateButton, &QPushButton::clicked, [=]() {
            updateDynamicContent(downloadsSpinBox->value(), ratingSpinBox->value());
        });
        
        // Initial update
        updateDynamicContent(12345, 4.7);
        
        layout->addWidget(group);
    }
    
    void updateDynamicContent(int downloads, double rating) {
        dynamicLabel->clearSegments();
        
        dynamicLabel->addSegment("Downloads: ", QFont("Arial", 11), Qt::black);
        dynamicLabel->addSegment(QString::number(downloads), 
                                QFont("Arial", 14, QFont::Bold), Qt::green);
        dynamicLabel->addSegment(" | Rating: ", QFont("Arial", 11), Qt::black);
        
        // Color rating based on value
        QColor ratingColor = Qt::red;
        if (rating >= 4.0) ratingColor = Qt::green;
        else if (rating >= 3.0) ratingColor = Qt::orange;
        
        dynamicLabel->addSegment(QString::number(rating, 'f', 1), 
                                QFont("Arial", 14, QFont::Bold), ratingColor);
        dynamicLabel->addSegment("/5.0", QFont("Arial", 11), Qt::black);
        
        dynamicLabel->updateText();
    }
    
    void onLinkClicked(const QString &clickId, const QString &text) {
        QString message;
        if (clickId == "terms") {
            message = "Opening Terms of Service...";
        } else if (clickId == "privacy") {
            message = "Opening Privacy Policy...";
        } else {
            message = QString("Clicked: %1 (%2)").arg(text, clickId);
        }
        
        QMessageBox::information(this, "Link Clicked", message);
    }

private:
    RichTextLabel *dynamicLabel;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    ExampleWindow window;
    window.show();
    
    return app.exec();
}

#include "main.moc"
```

### To Build and Run:

**Using qmake:**
```bash
# Create a project file (example.pro)
QT += widgets
SOURCES += main.cpp richtextlabel.cpp
HEADERS += richtextlabel.h
TARGET = richtextlabel_example

# Build
qmake
make
./richtextlabel_example
```

**Using CMake:**
```cmake
cmake_minimum_required(VERSION 3.16)
project(RichTextLabelExample)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

add_executable(richtextlabel_example
    main.cpp
    richtextlabel.cpp
    richtextlabel.h
)

target_link_libraries(richtextlabel_example Qt5::Widgets)
```

## License

This component is part of the Code Reuse Library. See repository license for details.

## Contributing

Contributions are welcome! Please ensure:
- Code follows Qt coding conventions
- Comments are in English
- Examples are provided for new features
- Changes are tested on multiple platforms
