#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "widget.h"
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void btnBubbleSortClicked();

private slots:
    void on_btnBubbleSort_clicked();

    void on_btnSelectionSort_clicked();

    void on_horizontalSliderSize_valueChanged(int value);

    void on_spinBox_valueChanged(int arg1);

    void specialSlot(int value); // just for now I did not mind any good name for it :-(

    void on_btnChooseFillColor_clicked();

    void on_btnInsertionSort_clicked();


    void on_btnGnomeSort_clicked();

    void on_btnChooseBackgroundColor_clicked();

    void on_btnCocktailSort_clicked();

    void on_btnOddEvenSort_clicked();

private:
    Ui::MainWindow *ui;

    Widget *widgetBars;

    void initGui();
};

#endif // MAINWINDOW_H
