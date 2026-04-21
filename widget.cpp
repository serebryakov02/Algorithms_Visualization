#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include <QLinearGradient>
#include <QPen>
#include <algorithm> // swap() C++11
#include <QTimer>
#include <QElapsedTimer>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , size {2}
    , bubbleSortPressed {false}
    , selectionSortPressed {false}
    , insertionSortPressed{false}
    , gnomeSortPressed{false}
    , cocktailSortPressed {false}
    , oddEvenSortPressed{false}
    , isSorted {false}
    , i{0}, j{0}, swapped{false}
    , selectedColor{""}
{
    ui->setupUi(this);

    connect(&timer, &QTimer::timeout, this, &Widget::performSortStep);

    initGui();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::bubbleSort()
{
    timer.start(1);
}

void Widget::activateBubbleSort()
{
    bubbleSortPressed = true;
    i = j = 0;
    bubbleSort();
}

void Widget::selectionSort()
{
    timer.start(50);
}

void Widget::activateSelectionSort()
{
    selectionSortPressed = true;
    i = j = 0;
    selectionSort();
}

void Widget::insertionSort()
{
    timer.start(50);
}

void Widget::activateInsertionSort()
{
    insertionSortPressed = true;
    i = 1;
    j = 0;
    insertionSort();
}

void Widget::gnomeSort()
{
    timer.start(5);
}

void Widget::activateGnomeSort()
{
    gnomeSortPressed = true;
    i = 0;
    gnomeSort();
}

void Widget::cocktailSort()
{
    timer.start(50);
}

void Widget::activateCocktailSort()
{
    cocktailSortPressed = true;
    swapped = true;
    start = 0;
    end = size - 1;
    cocktailSort();
}

void Widget::oddEvenSort()
{
    timer.start(50);
}

void Widget::activateOddEvenSort()
{
    oddEvenSortPressed = true;
    i = 0;
    oddEvenSort();
}

void Widget::stopSorting()
{
    timer.stop();

    bubbleSortPressed = false;
    selectionSortPressed = false;
    insertionSortPressed = false;
    gnomeSortPressed = false;
    cocktailSortPressed = false;
    oddEvenSortPressed = false;

    i = 0;
    j = 0;
    start = 0;
    end = 0;
    swapped = false;
    isSorted = false;
}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.save();
    painter.setPen(Qt::NoPen);

    const QColor backgroundBase = backgroundColor.isValid()
                                      ? backgroundColor
                                      : QColor("#111827");

    QLinearGradient backgroundGradient(0, 0, 0, height());
    backgroundGradient.setColorAt(0.0, backgroundBase.lighter(125));
    backgroundGradient.setColorAt(1.0, backgroundBase.darker(140));
    painter.fillRect(rect(), backgroundGradient);

    const QColor barBase = selectedColor.isValid()
                               ? selectedColor
                               : QColor("#22D3EE");
    const QColor barOutline = barBase.darker(140);
    QBrush barBrush(barBase);
    painter.setPen(QPen(barOutline, 1));

    size_t offset = 0;
    for (size_t i {0}; i < size; ++i) {
        const QRect barRect(offset, height() - bars.at(i), width() / size, bars.at(i));
        painter.fillRect(barRect, barBrush);
        painter.drawRect(barRect.adjusted(0, 0, -1, -1));
        offset += width() / size ;
    }

    painter.restore();

    return QWidget::paintEvent(event);
}


void Widget::initGui()
{
    setFixedSize(800, 600);

    QPalette palette;
    palette.setColor(QPalette::Window, Qt::black);
    setAutoFillBackground(true);
    setPalette(palette);

}

void Widget::setBackgroundColor(const QColor &newBackgroundColor)
{
    backgroundColor = newBackgroundColor;

    QPalette palette;
    palette.setColor(QPalette::Window, newBackgroundColor.name());
    setAutoFillBackground(true);
    setPalette(palette);
}

void Widget::performSortStep()
{
    if (bubbleSortPressed) {
        if (i < size - 1)
        {
            if (j < size - i - 1)
            {
                if (bars[j] > bars[j + 1])
                {
                    std::swap(bars[j], bars[j + 1]);
                    swapped = true;
                    update();
                }
                j++;
            }
            else
            {
                i++;
                j = 0;
            }
        }
        else
        {
            // If there were no exchanges in the entire outer loop, complete the sorting.
            if (!swapped)
            {
                timer.stop(); // Stop the timer
                bubbleSortPressed = false;
            }
            else
            {
                swapped = false;
                i = 0;
                j = 0;
            }
        }
    } else if (selectionSortPressed) {
        if (i < size - 1) {
            size_t min_idx {i};

            // Find the index of the minimum element in the rest of the array
            for (j = i + 1; j < size; j++) {
                if (bars[j] < bars[min_idx]) {
                    min_idx = j;
                }
            }

            std::swap(bars[min_idx], bars[i]);
            update();
            i++;
        } else {
            // Finish sorting
            timer.stop();
            selectionSortPressed = false;
        }
    } else if (insertionSortPressed) {
        size_t key {};

        if (i < size) {
            key = bars[i];
            j = i - 1;

            // Move elements of bars[0..i-1],
            // that are greater than key,
            // to one position ahead of their
            // current position
            while (j >= 0 && bars[j] > key) {
                bars[j + 1] = bars[j];
                j = j - 1;
                update();
            }
            bars[j + 1] = key;

            i++;
        } else {
            // Finish sorting
            timer.stop();
            insertionSortPressed = false;
        }
    } else if (gnomeSortPressed) {
        if (i < size) {
            if (i == 0)
                i++;
            if (bars[i] >= bars[i - 1])
                i++;
            else {
                std::swap(bars[i], bars[i - 1]);
                update();
                i--;
            }
        } else {
            // Finish sorting
            timer.stop();
            gnomeSortPressed = false;
        }
    } else if (cocktailSortPressed) {
        if (swapped) {
            // reset the swapped flag on entering
            // the loop, because it might be true from
            // a previous iteration.
            swapped = false;

            // loop from left to right same as
            // the bubble sort
            for (int i = start; i < end; ++i) {
                if (bars[i] > bars[i + 1]) {
                    std::swap(bars[i], bars[i + 1]);
                    swapped = true;
                    update();
                }
            }

            // if nothing moved, then array is sorted.
            if (!swapped) {
                // Finish sorting
                timer.stop();
                cocktailSortPressed = false;
            }

            // otherwise, reset the swapped flag so that it
            // can be used in the next stage
            swapped = false;

            // move the end point back by one, because
            // item at the end is in its rightful spot
            --end;

            // from right to left, doing the
            // same comparison as in the previous stage
            for (int i = end - 1; i >= start; --i) {
                if (bars[i] > bars[i + 1]) {
                    std::swap(bars[i], bars[i + 1]);
                    swapped = true;
                    update();
                }
            }

            // increase the starting point, because
            // the last stage would have moved the next
            // smallest number to its rightful spot.
            ++start;
        } else {
            // Finish sorting
            timer.stop();
            cocktailSortPressed = false;
        }
    } else if (oddEvenSortPressed) {
        if (!isSorted) {
            isSorted = true;

            // Perform Bubble sort on odd indexed element
            for (i=1; i<=size-2; i=i+2)
            {
                if (bars[i] > bars[i+1])
                {
                    std::swap(bars[i], bars[i+1]);
                    isSorted = false;
                    update();
                }
            }

            // Perform Bubble sort on even indexed element
            for (i=0; i<=size-2; i=i+2)
            {
                if (bars[i] > bars[i+1])
                {
                    std::swap(bars[i], bars[i+1]);
                    isSorted = false;
                    update();
                }
            }

        } else {
            // Finish sorting
            timer.stop();
            oddEvenSortPressed = false;
            isSorted = false;
        }
    }
}

void Widget::on_horizontalSlider_valueChanged(int value)
{
    stopSorting();

    bars.clear();

    size = value;

    srand(time(0));

    for (size_t i {0}; i < size; ++i) {
        bars.push_back(static_cast<size_t>(rand() % 600 + 1));
    }

    update();
}

QVector<size_t> &Widget::getBars()
{
    return bars;
}

void Widget::setBars(const QVector<size_t> &newBars)
{
    bars = newBars;
}

void Widget::setBarsColor(const QColor &color)
{
    selectedColor = color;
    update();
}

void Widget::setSize(size_t newSize)
{
    size = newSize;
}

size_t Widget::getSize() const
{
    return size;
}
