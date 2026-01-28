/**
 * @file richtextlabel.h
 * @brief Label widget supporting multi-style text
 */

#ifndef RICHTEXTLABEL_H
#define RICHTEXTLABEL_H

#include <QLabel>
#include <QString>
#include <QFont>
#include <QColor>
#include <QList>
#include <QLinearGradient>
#include <QMouseEvent>
#include <QMouseEvent>

/**
 * @brief Gradient direction enumeration
 */
enum class GradientDirection {
    Horizontal,    ///< Horizontal gradient (left to right)
    Vertical,      ///< Vertical gradient (top to bottom)
    Diagonal       ///< Diagonal gradient (top-left to bottom-right)
};

/**
 * @brief Gradient information structure
 */
struct GradientInfo {
    QColor startColor;              ///< Start color
    QColor endColor;                ///< End color
    GradientDirection direction;    ///< Gradient direction
    
    GradientInfo(const QColor &start = Qt::black, 
                 const QColor &end = Qt::black,
                 GradientDirection dir = GradientDirection::Horizontal)
        : startColor(start), endColor(end), direction(dir) {}
    
    bool isValid() const {
        return startColor.isValid() && endColor.isValid();
    }
};

/**
 * @brief Text segment style structure
 */
struct TextSegment {
    QString text;           ///< Text content
    QFont font;            ///< Font
    QColor color;          ///< Solid color (when not using gradient)
    GradientInfo gradient; ///< Gradient information (optional)
    bool useGradient;      ///< Whether to use gradient color
    bool isClickable;      ///< Whether clickable
    QString clickId;       ///< Click identifier (to distinguish different clickable segments)
    
    TextSegment(const QString &t = QString(), 
                const QFont &f = QFont(), 
                const QColor &c = Qt::black)
        : text(t), font(f), color(c), useGradient(false), isClickable(false) {}
    
    TextSegment(const QString &t, 
                const QFont &f, 
                const GradientInfo &grad)
        : text(t), font(f), gradient(grad), useGradient(true), isClickable(false) {}
};

/**
 * @brief Rich text label widget
 * 
 * Inherits from QLabel, supports setting different fonts and colors for different text segments.
 * Supports solid colors and gradient colors (gradients are displayed using approximate middle color).
 * For example: "Secured your computer for 100 days" can have special styling for "100".
 * 
 * @note Due to limited CSS gradient support in Qt's QLabel rich text renderer,
 *       gradient colors are approximated by displaying the middle color between start and end colors.
 *       This ensures text is visible on any background, but it's not a true gradient effect.
 * 
 * @note The widget has anti-aliasing enabled to ensure smooth and clear text display.
 * 
 * Usage example (solid color):
 * @code
 * RichTextLabel *label = new RichTextLabel(this);
 * label->addSegment("Secured your computer for ", QFont("Arial", 12), Qt::black);
 * label->addSegment("100", QFont("Arial", 16, QFont::Bold), Qt::red);
 * label->addSegment(" days", QFont("Arial", 12), Qt::black);
 * label->updateText();
 * @endcode
 * 
 * Usage example (gradient color - displayed as middle color):
 * @code
 * RichTextLabel *label = new RichTextLabel(this);
 * label->addSegment("Secured your computer for ", QFont("Arial", 12), Qt::black);
 * 
 * // Method 1: Using GradientInfo (will display as middle color)
 * GradientInfo gradient(QColor("#FF6B6B"), QColor("#4ECDC4"), GradientDirection::Horizontal);
 * label->addSegment("100", QFont("Arial", 16, QFont::Bold), gradient);
 * 
 * // Method 2: Using simplified method (will display as middle color)
 * label->addGradientSegment("100", QFont("Arial", 16, QFont::Bold), 
 *                          QColor("#FF6B6B"), QColor("#4ECDC4"), 
 *                          GradientDirection::Horizontal);
 * 
 * label->addSegment(" days", QFont("Arial", 12), Qt::black);
 * label->updateText();
 * @endcode
 */
class RichTextLabel : public QLabel
{
    Q_OBJECT

public:
    /**
     * @brief Constructor
     * @param parent Parent widget
     */
    explicit RichTextLabel(QWidget *parent = nullptr);
    
    /**
     * @brief Destructor
     */
    ~RichTextLabel() override;
    
    /**
     * @brief Add text segment
     * @param text Text content
     * @param font Font
     * @param color Color
     */
    void addSegment(const QString &text, const QFont &font, const QColor &color);
    
    /**
     * @brief Add gradient text segment
     * @param text Text content
     * @param font Font
     * @param gradient Gradient information
     */
    void addSegment(const QString &text, const QFont &font, const GradientInfo &gradient);
    
    /**
     * @brief Add gradient text segment (using default font)
     * @param text Text content
     * @param gradient Gradient information
     */
    void addSegment(const QString &text, const GradientInfo &gradient);
    
    /**
     * @brief Add gradient text segment (simplified version, specifying start and end colors)
     * @param text Text content
     * @param font Font
     * @param startColor Start color
     * @param endColor End color
     * @param direction Gradient direction (default horizontal)
     */
    void addGradientSegment(const QString &text, const QFont &font, 
                           const QColor &startColor, const QColor &endColor,
                           GradientDirection direction = GradientDirection::Horizontal);
    
    /**
     * @brief Add text segment (using default font)
     * @param text Text content
     * @param color Color
     */
    void addSegment(const QString &text, const QColor &color);
    
    /**
     * @brief Add text segment (using default color)
     * @param text Text content
     * @param font Font
     */
    void addSegment(const QString &text, const QFont &font);
    
    /**
     * @brief Add plain text segment (using default font and color)
     * @param text Text content
     */
    void addSegment(const QString &text);
    
    /**
     * @brief Add clickable text segment
     * @param text Text content
     * @param font Font
     * @param color Color
     * @param clickId Click identifier
     */
    void addClickableSegment(const QString &text, const QFont &font, const QColor &color, const QString &clickId);
    
    /**
     * @brief Add clickable text segment (using default font)
     * @param text Text content
     * @param color Color
     * @param clickId Click identifier
     */
    void addClickableSegment(const QString &text, const QColor &color, const QString &clickId);
    
    /**
     * @brief Clear all text segments
     */
    void clearSegments();
    
    /**
     * @brief Update label display (apply all text segments)
     */
    void updateText();
    
    /**
     * @brief Set default font
     * @param font Font
     */
    void setDefaultFont(const QFont &font);
    
    /**
     * @brief Set default color
     * @param color Color
     */
    void setDefaultColor(const QColor &color);
    
    /**
     * @brief Get default font
     * @return Default font
     */
    QFont defaultFont() const;
    
    /**
     * @brief Get default color
     * @return Default color
     */
    QColor defaultColor() const;
    
    /**
     * @brief Set text segment list
     * @param segments Text segment list
     */
    void setSegments(const QList<TextSegment> &segments);
    
    /**
     * @brief Get text segment list
     * @return Text segment list
     */
    QList<TextSegment> segments() const;

signals:
    /**
     * @brief Text segment clicked signal
     * @param clickId Identifier of the clicked segment
     * @param text Text content of the clicked segment
     */
    void segmentClicked(const QString &clickId, const QString &text);

protected:
    /**
     * @brief Mouse press event
     * @param event Mouse event
     */
    void mousePressEvent(QMouseEvent *event) override;

private:
    /**
     * @brief Find clickable segment at mouse position
     * @param pos Mouse position
     * @return Segment index, -1 if no clickable segment found
     */
    int findClickableSegmentAt(const QPoint &pos) const;
    
    /**
     * @brief Generate rich text HTML
     * @return HTML string
     */
    QString generateRichTextHtml() const;
    
    /**
     * @brief Convert font to CSS style
     * @param font Font
     * @return CSS style string
     */
    QString fontToCss(const QFont &font) const;
    
    /**
     * @brief Convert color to CSS style
     * @param color Color
     * @return CSS color string
     */
    QString colorToCss(const QColor &color) const;
    
    /**
     * @brief Convert gradient to CSS style
     * @param gradient Gradient information
     * @return CSS gradient style string
     */
    QString gradientToCss(const GradientInfo &gradient) const;

private:
    QList<TextSegment> m_segments;  ///< Text segment list
    QFont m_defaultFont;            ///< Default font
    QColor m_defaultColor;          ///< Default color
};

#endif // RICHTEXTLABEL_H
