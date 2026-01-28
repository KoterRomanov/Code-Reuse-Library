# Qt GUI Components

This directory contains reusable Qt GUI components and utilities.

## Contents

### Main Window Components
- **ModernMainWindow**: Modern styled main window
- **RibbonInterface**: Microsoft Office-style ribbon
- **MenuBarBuilder**: Fluent menu bar construction
- **ToolBarBuilder**: Custom toolbar builder
- **StatusBarHelper**: Enhanced status bar utilities

### Dialogs
- **WizardDialog**: Multi-step wizard dialog
- **SettingsDialog**: Application settings dialog
- **PreferencesDialog**: User preferences dialog
- **LoginDialog**: Login/authentication dialog
- **AboutDialog**: About application dialog
- **ColorPickerDialog**: Color selection dialog
- **FontDialog**: Font selection dialog

### Layout Helpers
- **FlowLayout**: Flow layout implementation
- **GridBuilder**: Simplified grid layout
- **FormBuilder**: Form layout builder
- **ResponsiveLayout**: Responsive layout container
- **StackedWidget**: Enhanced stacked widget

### Widgets
- **SplashScreen**: Application splash screen
- **DockWidget**: Custom dockable widget
- **TabWidget**: Enhanced tab widget
- **TreeWidget**: Enhanced tree widget
- **TableWidget**: Enhanced table widget
- **NavigationBar**: Navigation sidebar
- **BreadcrumbWidget**: Breadcrumb navigation

### Charts and Visualization
- **LineChart**: Line chart widget
- **BarChart**: Bar chart widget
- **PieChart**: Pie chart widget
- **ScatterPlot**: Scatter plot widget
- **Gauge**: Gauge/meter widget
- **Histogram**: Histogram widget

### Input Components
- **SearchBar**: Global search bar
- **FilterWidget**: Data filtering widget
- **TagInput**: Tag input widget
- **RatingWidget**: Star rating widget
- **SliderWithLabel**: Slider with value label
- **ColorPicker**: Color picker widget

### Media Components
- **ImageViewer**: Image viewer with zoom
- **VideoPlayer**: Video player widget
- **AudioPlayer**: Audio player widget
- **Thumbnail**: Image thumbnail widget
- **Gallery**: Image gallery widget

### Utility Components
- **LoadingOverlay**: Loading screen overlay
- **ErrorWidget**: Error display widget
- **EmptyStateWidget**: Empty state placeholder
- **NotificationCenter**: Notification manager
- **ContextMenu**: Custom context menu
- **Tooltip**: Custom tooltip widget

## Features

- Modern, flat design
- Dark/light theme support
- Responsive and adaptive
- Accessibility support
- Customizable styling
- Animation support
- Cross-platform compatibility

## Usage Examples

### Main Window with Toolbar
```cpp
#include "ModernMainWindow.h"
#include "ToolBarBuilder.h"

ModernMainWindow window;
window.setTitle("My Application");

// Build toolbar
ToolBarBuilder toolbar;
toolbar.addAction(QIcon(":/icons/new.png"), "New", [&]() {
    // New action
});
toolbar.addAction(QIcon(":/icons/open.png"), "Open", [&]() {
    // Open action
});
toolbar.addSeparator();
toolbar.addAction(QIcon(":/icons/save.png"), "Save", [&]() {
    // Save action
});

window.addToolBar(toolbar.build());
window.show();
```

### Settings Dialog
```cpp
#include "SettingsDialog.h"

SettingsDialog dialog;

dialog.addSection("General")
    .addCheckBox("startup", "Start on system startup")
    .addCheckBox("updates", "Check for updates automatically");

dialog.addSection("Appearance")
    .addComboBox("theme", "Theme", {"Light", "Dark", "Auto"})
    .addSpinBox("fontSize", "Font Size", 8, 24, 12);

if (dialog.exec() == QDialog::Accepted) {
    QVariantMap settings = dialog.settings();
    // Apply settings
}
```

### Chart Widget
```cpp
#include "LineChart.h"

LineChart chart;
chart.setTitle("Sales Data");
chart.setXAxisLabel("Month");
chart.setYAxisLabel("Revenue ($)");

QVector<QPointF> data = {
    {1, 1000}, {2, 1500}, {3, 1200},
    {4, 1800}, {5, 2200}, {6, 2500}
};

chart.addSeries("Sales", data, Qt::blue);
chart.show();
```

### Navigation Sidebar
```cpp
#include "NavigationBar.h"

NavigationBar nav;

nav.addItem(QIcon(":/icons/home.png"), "Home", []() {
    // Show home page
});
nav.addItem(QIcon(":/icons/settings.png"), "Settings", []() {
    // Show settings page
});
nav.addSeparator();
nav.addItem(QIcon(":/icons/about.png"), "About", []() {
    // Show about page
});

mainWindow->addDockWidget(Qt::LeftDockWidgetArea, nav.dockWidget());
```

### Notification System
```cpp
#include "NotificationCenter.h"

NotificationCenter &notifications = NotificationCenter::instance();

// Show success notification
notifications.success("File saved successfully!");

// Show error notification
notifications.error("Failed to load file", 5000); // 5 seconds

// Show info notification with action
notifications.info("Update available")
    .action("Update", []() {
        // Start update
    })
    .action("Later", []() {
        // Dismiss
    });
```

## Styling

All components support Qt Style Sheets:

```cpp
component->setStyleSheet(R"(
    QWidget {
        background-color: #ffffff;
        color: #333333;
        font-family: 'Segoe UI', Arial;
        font-size: 14px;
    }
)");
```

## Best Practices

- Use layouts instead of fixed positions
- Implement proper parent-child relationships
- Handle widget deletion properly
- Use resource files for images and icons
- Support high DPI displays
- Test on multiple platforms
- Follow platform UI guidelines
- Implement keyboard shortcuts
- Provide tooltips and help text
- Support accessibility features
