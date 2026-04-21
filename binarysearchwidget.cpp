#include "binarysearchwidget.h"

#include <QFontMetrics>
#include <QLinearGradient>
#include <QPainter>
#include <QPen>
#include <QRandomGenerator>
#include <QStringList>

BinarySearchWidget::BinarySearchWidget(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(800, 600);

    searchTimer.setInterval(450);
    connect(&searchTimer, &QTimer::timeout, this, &BinarySearchWidget::showNextStep);

    generateValues();
}

int BinarySearchWidget::valueCount() const
{
    return values.size();
}

int BinarySearchWidget::minimumValue() const
{
    return values.isEmpty() ? 0 : values.first();
}

int BinarySearchWidget::maximumValue() const
{
    return values.isEmpty() ? 0 : values.last();
}

int BinarySearchWidget::targetValue() const
{
    return target;
}

int BinarySearchWidget::randomExistingValue() const
{
    if (values.isEmpty()) {
        return target;
    }

    const int index = QRandomGenerator::global()->bounded(values.size());
    return values.at(index);
}

void BinarySearchWidget::setBarColor(const QColor &color)
{
    if (!color.isValid()) {
        return;
    }

    barColor = color;
    update();
}

void BinarySearchWidget::setBackgroundColor(const QColor &color)
{
    if (!color.isValid()) {
        return;
    }

    backgroundColor = color;
    update();
}

void BinarySearchWidget::setValueCount(int count)
{
    requestedCount = qMax(2, count);
    generateValues();
}

void BinarySearchWidget::setTargetValue(int value)
{
    target = value;
    resetSearchState();
    update();
}

void BinarySearchWidget::generateValues()
{
    resetSearchState();
    values.clear();

    int currentValue = QRandomGenerator::global()->bounded(8, 24);

    for (int i = 0; i < requestedCount; ++i) {
        currentValue += QRandomGenerator::global()->bounded(6, 22);
        values.push_back(currentValue);
    }

    if (!values.isEmpty()) {
        target = values.at(values.size() / 2);
        emit valuesGenerated(values.first(), values.last(), target);
    }

    update();
}

void BinarySearchWidget::runBinarySearch()
{
    resetSearchState();

    if (values.isEmpty()) {
        update();
        return;
    }

    int low = 0;
    int high = values.size() - 1;

    while (low <= high) {
        const int mid = low + (high - low) / 2;
        const int midValue = values.at(mid);

        searchSteps.push_back({
            low,
            high,
            mid,
            QString("Compare target %1 with value %2 at index %3")
                .arg(target)
                .arg(midValue)
                .arg(mid),
            {},
            false,
            false
        });

        if (midValue == target) {
            searchSteps.push_back({
                low,
                high,
                mid,
                QString("Value %1 matches the middle element").arg(target),
                QString("Found at index %1").arg(mid),
                true,
                true
            });
            break;
        }

        if (midValue < target) {
            searchSteps.push_back({
                mid + 1,
                high,
                -1,
                QString("Target is greater than %1, discard the left half").arg(midValue),
                {},
                false,
                false
            });
            low = mid + 1;
        } else {
            searchSteps.push_back({
                low,
                mid - 1,
                -1,
                QString("Target is smaller than %1, discard the right half").arg(midValue),
                {},
                false,
                false
            });
            high = mid - 1;
        }
    }

    if (!searchSteps.isEmpty() && !searchSteps.last().found) {
        searchSteps.push_back({
            low,
            high,
            -1,
            QString("Value %1 is not present in the array").arg(target),
            "Not found",
            false,
            true
        });
    }

    if (searchSteps.isEmpty()) {
        update();
        return;
    }

    showNextStep();
    searchTimer.start();
}

void BinarySearchWidget::showNextStep()
{
    if (searchStepIndex >= searchSteps.size()) {
        searchTimer.stop();
        update();
        return;
    }

    currentStep = searchSteps.at(searchStepIndex);
    ++searchStepIndex;
    update();
}

void BinarySearchWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QLinearGradient background(0, 0, 0, height());
    background.setColorAt(0.0, backgroundColor.lighter(125));
    background.setColorAt(1.0, backgroundColor.darker(140));
    painter.fillRect(rect(), background);

    if (values.isEmpty()) {
        drawStatusOverlay(painter);
        return;
    }

    const QRectF plotRect(28, 110, width() - 56, height() - 180);
    const int maxValue = values.last();
    const qreal spacing = values.size() > 24 ? 2.0 : 4.0;
    const qreal barWidth = (plotRect.width() - spacing * (values.size() - 1)) / values.size();
    const int activeLow = currentStep.low == -1 ? 0 : currentStep.low;
    const int activeHigh = currentStep.high == -1 ? values.size() - 1 : currentStep.high;

    for (int index = 0; index < values.size(); ++index) {
        const qreal normalizedHeight = static_cast<qreal>(values.at(index)) / maxValue;
        const qreal barHeight = plotRect.height() * normalizedHeight;
        const QRectF barRect(plotRect.left() + index * (barWidth + spacing),
                             plotRect.bottom() - barHeight,
                             barWidth,
                             barHeight);

        QColor fillColor = barColor;
        if (activeHigh < activeLow || index < activeLow || index > activeHigh) {
            fillColor = barColor.darker(230);
            fillColor.setAlpha(170);
        }

        if (index == currentStep.low || index == currentStep.high) {
            fillColor = QColor("#3B82F6");
        }

        if (index == currentStep.mid) {
            fillColor = currentStep.found ? QColor("#22C55E") : QColor("#FACC15");
        }

        const QColor outlineColor = fillColor.darker(145);
        painter.setBrush(fillColor);
        painter.setPen(QPen(outlineColor, 1));
        painter.drawRect(barRect.adjusted(0, 0, -1, -1));

        const bool showValueLabel = values.size() <= 12
                                    || index == currentStep.low
                                    || index == currentStep.high
                                    || index == currentStep.mid;

        if (showValueLabel) {
            const QRectF valueRect(barRect.center().x() - 24, barRect.top() - 28, 48, 20);
            painter.setPen(Qt::NoPen);
            painter.setBrush(QColor(15, 23, 42, 210));
            painter.drawRoundedRect(valueRect, 5, 5);
            painter.setPen(QColor("#F8FAFC"));
            painter.drawText(valueRect, Qt::AlignCenter, QString::number(values.at(index)));

            const QRectF indexRect(barRect.center().x() - 18, plotRect.bottom() + 10, 36, 18);
            painter.setPen(Qt::NoPen);
            painter.setBrush(QColor(15, 23, 42, 180));
            painter.drawRoundedRect(indexRect, 5, 5);
            painter.setPen(QColor("#CBD5E1"));
            painter.drawText(indexRect, Qt::AlignCenter, QString::number(index));
        }
    }

    drawStatusOverlay(painter);
}

void BinarySearchWidget::resetSearchState()
{
    searchTimer.stop();
    searchSteps.clear();
    currentStep = {};
    searchStepIndex = 0;
}

void BinarySearchWidget::drawStatusOverlay(QPainter &painter) const
{
    QStringList lines;
    lines << "Binary Search";
    lines << QString("Target: %1").arg(target);

    if (!currentStep.statusText.isEmpty()) {
        lines << currentStep.statusText;
    } else {
        lines << QString("Ready to search in %1 sorted values").arg(values.size());
    }

    if (currentStep.low != -1 && currentStep.high != -1 && currentStep.high >= currentStep.low) {
        lines << QString("Range: indexes %1 - %2")
                     .arg(currentStep.low)
                     .arg(currentStep.high);
    }

    if (currentStep.mid != -1) {
        lines << QString("Middle: index %1, value %2")
                     .arg(currentStep.mid)
                     .arg(values.at(currentStep.mid));
    }

    if (!currentStep.resultText.isEmpty()) {
        lines << currentStep.resultText;
    }

    painter.save();

    QFont font = painter.font();
    font.setPointSize(12);
    painter.setFont(font);

    const QFontMetrics metrics(font);
    constexpr int margin = 16;
    constexpr int padding = 12;
    const int textWidth = qMin(380, width() - margin * 2 - padding * 2);
    int textHeight = 0;

    for (const auto &line : lines) {
        const QRect textRect = metrics.boundingRect(QRect(0, 0, textWidth, 1000),
                                                    Qt::TextWordWrap, line);
        textHeight += textRect.height() + 4;
    }

    const QRectF panel(margin, margin, textWidth + padding * 2, textHeight + padding * 2);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(15, 23, 42, 210));
    painter.drawRoundedRect(panel, 6, 6);

    painter.setPen(QColor("#F8FAFC"));
    int y = panel.top() + padding;

    for (const auto &line : lines) {
        const QRect textRect = metrics.boundingRect(QRect(0, 0, textWidth, 1000),
                                                    Qt::TextWordWrap, line);
        painter.drawText(QRectF(panel.left() + padding, y, textWidth, textRect.height()),
                         Qt::TextWordWrap, line);
        y += textRect.height() + 4;
    }

    painter.restore();
}
