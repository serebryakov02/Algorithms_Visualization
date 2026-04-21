#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void bubbleSort();
    void activateBubbleSort();

    void selectionSort();
    void activateSelectionSort();

    void insertionSort();
    void activateInsertionSort();

    void gnomeSort();
    void activateGnomeSort();

    void cocktailSort();
    void activateCocktailSort();

    void oddEvenSort();
    void activateOddEvenSort();

    void stopSorting();

    size_t getSize() const;

    void setSize(size_t newSize);

    QVector<size_t> &getBars();
    void setBars(const QVector<size_t> &newBars);

    void setBarsColor(const QColor &color);

    void setBackgroundColor(const QColor &newBackgroundColor);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void performSortStep();
    void on_horizontalSlider_valueChanged(int value);


private:
    Ui::Widget *ui;

    QVector<size_t> bars;
    size_t size;

    void initGui();

    QTimer timer;

    bool bubbleSortPressed;
    bool selectionSortPressed;
    bool insertionSortPressed;
    bool gnomeSortPressed;
    bool cocktailSortPressed;
    bool oddEvenSortPressed;

    int start {}; // for cocktail sort
    int end {}; // for cocktail sort

    size_t i;
    int j;
    bool swapped;

    bool isSorted; // for odd-even sort

    QColor selectedColor;
    QColor backgroundColor;
};

#endif // WIDGET_H
