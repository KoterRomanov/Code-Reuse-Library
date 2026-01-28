# Qt Model-View Patterns

This directory contains implementations of Qt's Model-View framework patterns.

## Contents

### Custom Models
- **TableModel**: Custom QAbstractTableModel implementations
- **ListModel**: Custom QAbstractListModel implementations
- **TreeModel**: Custom QAbstractItemModel for tree structures
- **SQLModel**: Models for SQL data
- **ProxyModel**: Custom sorting/filtering proxy models

### Custom Delegates
- **ComboBoxDelegate**: Dropdown editor in views
- **SpinBoxDelegate**: Numeric spinner editor
- **DateDelegate**: Date picker editor
- **CheckBoxDelegate**: Checkbox editor
- **ProgressBarDelegate**: Progress bar display
- **CustomPaintDelegate**: Custom rendering delegate

### View Utilities
- **TableView**: Enhanced QTableView
- **TreeView**: Enhanced QTreeView
- **ListView**: Enhanced QListView
- **HeaderView**: Custom header with filtering
- **SelectionHelper**: Selection management utilities

### Model Utilities
- **ModelTester**: Test model correctness
- **ModelSerializer**: Save/load model data
- **ModelExporter**: Export to CSV, JSON, XML
- **UndoableModel**: Model with undo/redo support
- **CachedModel**: Model with caching layer

## Model Responsibilities

- Data storage and management
- Data access interface
- Notify views of changes
- Handle data editing
- Provide item flags (editable, selectable, etc.)

## View Responsibilities

- Display model data
- Handle user interaction
- Request data from model
- Delegate item rendering/editing

## Delegate Responsibilities

- Render items in views
- Provide editors for items
- Update model with edited data

## Usage Examples

### Custom Table Model
```cpp
class ContactModel : public QAbstractTableModel {
    Q_OBJECT
public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        return contacts.size();
    }
    
    int columnCount(const QModelIndex &parent = QModelIndex()) const override {
        return 3; // Name, Email, Phone
    }
    
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        if (role == Qt::DisplayRole) {
            const Contact &contact = contacts[index.row()];
            switch (index.column()) {
                case 0: return contact.name;
                case 1: return contact.email;
                case 2: return contact.phone;
            }
        }
        return QVariant();
    }
    
private:
    QVector<Contact> contacts;
};
```

### Custom Delegate
```cpp
class ComboBoxDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override {
        QComboBox *editor = new QComboBox(parent);
        editor->addItems({"Option 1", "Option 2", "Option 3"});
        return editor;
    }
    
    void setEditorData(QWidget *editor, const QModelIndex &index) const override {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        QString value = index.model()->data(index, Qt::EditRole).toString();
        comboBox->setCurrentText(value);
    }
    
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        model->setData(index, comboBox->currentText(), Qt::EditRole);
    }
};
```

### Using Model-View
```cpp
// Create model
ContactModel *model = new ContactModel(this);

// Create view
QTableView *view = new QTableView(this);
view->setModel(model);

// Set custom delegate for specific column
view->setItemDelegateForColumn(1, new ComboBoxDelegate(this));
```

## Best Practices

- Emit proper signals when data changes
- Use beginInsertRows/endInsertRows for insertions
- Use beginRemoveRows/endRemoveRows for deletions
- Implement data() for multiple roles
- Use proxy models for sorting/filtering
- Keep models lightweight (lazy loading for large datasets)
- Test models with QAbstractItemModelTester
