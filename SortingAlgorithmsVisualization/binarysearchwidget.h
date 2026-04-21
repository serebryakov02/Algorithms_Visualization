#ifndef BINARYSEARCHWIDGET_H
#define BINARYSEARCHWIDGET_H

#include <QColor>
#include <QString>
#include <QTimer>
#include <QVector>
#include <QWidget>

class QPainter;

class BinarySearchWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BinarySearchWidget(QWidget *parent = nullptr);

    int valueCount() const;
    int minimumValue() const;
    int maximumValue() const;
    int targetValue() const;
    int randomExistingValue() const;

    void setBarColor(const QColor &color);
    void setBackgroundColor(const QColor &color);
    void setValueCount(int count);
    void setTargetValue(int value);

signals:
    void valuesGenerated(int minimum, int maximum, int suggestedTarget);

public slots:
    void generateValues();
    void runBinarySearch();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void showNextStep();

private:
    struct SearchStep
    {
        int low {-1};
        int high {-1};
        int mid {-1};
        QString statusText;
        QString resultText;
        bool found {false};
        bool finished {false};
    };

    QVector<int> values;
    QVector<SearchStep> searchSteps;
    QColor barColor {"#22D3EE"};
    QColor backgroundColor {"#111827"};
    QTimer searchTimer;
    SearchStep currentStep;
    int requestedCount {16};
    int target {0};
    int searchStepIndex {0};

    void resetSearchState();
    void drawStatusOverlay(QPainter &painter) const;
};

#endif // BINARYSEARCHWIDGET_H
