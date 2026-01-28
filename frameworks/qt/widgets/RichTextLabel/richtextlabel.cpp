/**
 * @file richtextlabel.cpp
 * @brief Implementation of label widget supporting multi-style text
 */

#include "richtextlabel.h"

#include <QDebug>
#include <QMouseEvent>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>

RichTextLabel::RichTextLabel(QWidget *parent)
    : QLabel(parent)
    , m_defaultFont(QFont())
    , m_defaultColor(Qt::black)
{
    // Enable rich text support
    setTextFormat(Qt::RichText);
    
    // Set text interaction flags (allow text selection)
    setTextInteractionFlags(Qt::TextBrowserInteraction);
    
    // Enable word wrap
    setWordWrap(true);
    
    // Enable anti-aliasing rendering to eliminate text jaggies
    QFont font = this->font();
    font.setStyleStrategy(QFont::PreferAntialias);
    setFont(font);
}

RichTextLabel::~RichTextLabel()
{
}

void RichTextLabel::addSegment(const QString &text, const QFont &font, const QColor &color)
{
    m_segments.append(TextSegment(text, font, color));
}

void RichTextLabel::addSegment(const QString &text, const QFont &font, const GradientInfo &gradient)
{
    m_segments.append(TextSegment(text, font, gradient));
}

void RichTextLabel::addSegment(const QString &text, const GradientInfo &gradient)
{
    m_segments.append(TextSegment(text, m_defaultFont, gradient));
}

void RichTextLabel::addGradientSegment(const QString &text, const QFont &font, 
                                      const QColor &startColor, const QColor &endColor,
                                      GradientDirection direction)
{
    GradientInfo gradient(startColor, endColor, direction);
    m_segments.append(TextSegment(text, font, gradient));
}

void RichTextLabel::addSegment(const QString &text, const QColor &color)
{
    m_segments.append(TextSegment(text, m_defaultFont, color));
}

void RichTextLabel::addSegment(const QString &text, const QFont &font)
{
    m_segments.append(TextSegment(text, font, m_defaultColor));
}

void RichTextLabel::addSegment(const QString &text)
{
    m_segments.append(TextSegment(text, m_defaultFont, m_defaultColor));
}

void RichTextLabel::addClickableSegment(const QString &text, const QFont &font, const QColor &color, const QString &clickId)
{
    TextSegment segment(text, font, color);
    segment.isClickable = true;
    segment.clickId = clickId;
    m_segments.append(segment);
}

void RichTextLabel::addClickableSegment(const QString &text, const QColor &color, const QString &clickId)
{
    TextSegment segment(text, m_defaultFont, color);
    segment.isClickable = true;
    segment.clickId = clickId;
    m_segments.append(segment);
}

void RichTextLabel::clearSegments()
{
    m_segments.clear();
    setText(QString());
}

void RichTextLabel::updateText()
{
    if (m_segments.isEmpty()) {
        setText(QString());
        return;
    }
    
    QString html = generateRichTextHtml();
    setText(html);
}

void RichTextLabel::setDefaultFont(const QFont &font)
{
    m_defaultFont = font;
    
    // Ensure font has anti-aliasing enabled
    QFont antialiasedFont = font;
    antialiasedFont.setStyleStrategy(QFont::PreferAntialias);
    
    QLabel::setFont(antialiasedFont);
}

void RichTextLabel::setDefaultColor(const QColor &color)
{
    m_defaultColor = color;
}

QFont RichTextLabel::defaultFont() const
{
    return m_defaultFont;
}

QColor RichTextLabel::defaultColor() const
{
    return m_defaultColor;
}

void RichTextLabel::setSegments(const QList<TextSegment> &segments)
{
    m_segments = segments;
}

QList<TextSegment> RichTextLabel::segments() const
{
    return m_segments;
}

QString RichTextLabel::generateRichTextHtml() const
{
    QString html;
    
    for (int i = 0; i < m_segments.size(); ++i) {
        const TextSegment &segment = m_segments[i];
        
        // Escape HTML special characters
        QString escapedText = segment.text;
        escapedText.replace("&", "&amp;");
        escapedText.replace("<", "&lt;");
        escapedText.replace(">", "&gt;");
        escapedText.replace("\"", "&quot;");
        escapedText.replace("'", "&#39;");
        
        // Build span tag
        QString colorStyle;
        if (segment.useGradient && segment.gradient.isValid()) {
            colorStyle = gradientToCss(segment.gradient);
        } else {
            colorStyle = colorToCss(segment.color);
        }
        
        QString extraStyle;
        if (segment.isClickable) {
            // Add style and data attributes for clickable segments (no underline)
            extraStyle = " cursor: pointer; text-decoration: none;";
        }
        
        QString spanTag = QString("<span style=\"%1 %2%3\" data-segment-index=\"%4\" data-clickable=\"%5\" data-click-id=\"%6\">%7</span>")
                              .arg(fontToCss(segment.font))
                              .arg(colorStyle)
                              .arg(extraStyle)
                              .arg(i)
                              .arg(segment.isClickable ? "true" : "false")
                              .arg(segment.clickId)
                              .arg(escapedText);
        
        html += spanTag;
    }
    
    return html;
}

QString RichTextLabel::fontToCss(const QFont &font) const
{
    QString css;
    
    // Font family
    if (!font.family().isEmpty()) {
        css += QString("font-family: '%1';").arg(font.family());
    }
    
    // Font size
    if (font.pointSize() > 0) {
        css += QString(" font-size: %1pt;").arg(font.pointSize());
    } else if (font.pixelSize() > 0) {
        css += QString(" font-size: %1px;").arg(font.pixelSize());
    }
    
    // Font weight
    if (font.bold()) {
        css += " font-weight: bold;";
    } else {
        css += QString(" font-weight: %1;").arg(font.weight() * 10);
    }
    
    // Italic
    if (font.italic()) {
        css += " font-style: italic;";
    }
    
    // Underline
    if (font.underline()) {
        css += " text-decoration: underline;";
    }
    
    // Strikeout
    if (font.strikeOut()) {
        if (font.underline()) {
            css += " line-through;";
        } else {
            css += " text-decoration: line-through;";
        }
    }
    
    // Add anti-aliasing rendering hints (CSS level)
    css += " -webkit-font-smoothing: antialiased;";
    css += " -moz-osx-font-smoothing: grayscale;";
    css += " text-rendering: optimizeLegibility;";
    
    return css;
}

QString RichTextLabel::colorToCss(const QColor &color) const
{
    if (!color.isValid()) {
        return QString();
    }
    
    // Use RGBA format to support transparency
    return QString("color: rgba(%1, %2, %3, %4);")
               .arg(color.red())
               .arg(color.green())
               .arg(color.blue())
               .arg(color.alphaF(), 0, 'f', 2);
}

QString RichTextLabel::gradientToCss(const GradientInfo &gradient) const
{
    if (!gradient.isValid()) {
        return QString();
    }
    
    // Note: Qt's QLabel rich text renderer has limited support for CSS gradients (background-clip: text)
    // This would make text transparent and invisible on white backgrounds
    // Here we use the middle color of the gradient for approximate display, ensuring text visibility
    
    // Calculate middle color of gradient (blend start and end colors)
    int r = (gradient.startColor.red() + gradient.endColor.red()) / 2;
    int g = (gradient.startColor.green() + gradient.endColor.green()) / 2;
    int b = (gradient.startColor.blue() + gradient.endColor.blue()) / 2;
    qreal a = (gradient.startColor.alphaF() + gradient.endColor.alphaF()) / 2.0;
    
    QColor middleColor(r, g, b);
    middleColor.setAlphaF(a);
    
    // Return middle color CSS (using solid color display)
    return colorToCss(middleColor);
}

void RichTextLabel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        int segmentIndex = findClickableSegmentAt(event->pos());
        if (segmentIndex >= 0 && segmentIndex < m_segments.size()) {
            const TextSegment &segment = m_segments[segmentIndex];
            if (segment.isClickable && !segment.clickId.isEmpty()) {
                emit segmentClicked(segment.clickId, segment.text);
                event->accept();
                return;
            }
        }
    }
    
    QLabel::mousePressEvent(event);
}

int RichTextLabel::findClickableSegmentAt(const QPoint &pos) const
{
    // Simplified version: Due to complex QLabel rich text rendering, use approximate method
    // Estimate segment corresponding to mouse click position based on text length
    
    QString fullText = text();
    if (fullText.isEmpty() || m_segments.isEmpty()) {
        return -1;
    }
    
    // Get character position (simplified calculation)
    QTextDocument doc;
    doc.setHtml(fullText);
    doc.setTextWidth(width());
    
    QPointF docPos = pos;
    int charIndex = doc.documentLayout()->hitTest(docPos, Qt::FuzzyHit);
    
    if (charIndex < 0) {
        return -1;
    }
    
    // Find corresponding segment based on character position
    int currentPos = 0;
    for (int i = 0; i < m_segments.size(); ++i) {
        const TextSegment &segment = m_segments[i];
        if (charIndex >= currentPos && charIndex < currentPos + segment.text.length()) {
            return segment.isClickable ? i : -1;
        }
        currentPos += segment.text.length();
    }
    
    return -1;
}
