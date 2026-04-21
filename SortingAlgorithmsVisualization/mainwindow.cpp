#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initGui();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initGui()
{
    setWindowTitle("Algorithms Visualization");

    setFixedSize(1280, 768);

    widgetBars = new Widget;

    ui->horizontalSliderSize->setMinimum(2);
    ui->horizontalSliderSize->setMaximum(100);
    ui->horizontalSliderSize->setValue(widgetBars->getSize());

    ui->spinBoxSize->setMinimum(2);
    ui->spinBoxSize->setMaximum(100);

    auto hLayout = new QHBoxLayout;
    hLayout->addWidget(widgetBars);

    auto vLayout = new QVBoxLayout;
    vLayout->addWidget(ui->widgetSettings);
    vLayout->addWidget(ui->widgetStyling);

    // Create a vertical spacer
    auto vSpacer = new QSpacerItem(20, 40,
                                   QSizePolicy::Minimum, QSizePolicy::Expanding);
    vLayout->addSpacerItem(vSpacer);

    hLayout->addLayout(vLayout);

    ui->mainWidget->setLayout(hLayout);

    setCentralWidget(ui->mainWidget);
}

void MainWindow::on_horizontalSliderSize_valueChanged(int value)
{
    specialSlot(value);
}

void MainWindow::on_spinBox_valueChanged(int value)
{
    specialSlot(value);
}

void MainWindow::specialSlot(int value)
{
    widgetBars->stopSorting();

    widgetBars->getBars().clear();

    widgetBars->setSize(value);

    srand(time(0));

    for (size_t i {0}; i < widgetBars->getSize(); ++i) {
        widgetBars->getBars().push_back(static_cast<size_t>(rand() % 600 + 1));
    }

    widgetBars->update();
}

void MainWindow::on_btnChooseFillColor_clicked()
{
    const QColor color = QColorDialog::getColor(Qt::green, this, "Select Color");

    if (color.isValid())
        widgetBars->setBarsColor(color);
}

void MainWindow::on_btnBubbleSort_clicked()
{
    widgetBars->activateBubbleSort();
}

void MainWindow::on_btnSelectionSort_clicked()
{
    widgetBars->activateSelectionSort();
}

void MainWindow::on_btnInsertionSort_clicked()
{
    widgetBars->activateInsertionSort();
}

void MainWindow::on_btnGnomeSort_clicked()
{
    widgetBars->activateGnomeSort();
}

void MainWindow::on_btnCocktailSort_clicked()
{
    widgetBars->activateCocktailSort();
}

void MainWindow::on_btnOddEvenSort_clicked()
{
    widgetBars->activateOddEvenSort();
}

void MainWindow::on_btnChooseBackgroundColor_clicked()
{
    const QColor color = QColorDialog::getColor(Qt::green, this, "Select Color");

    if (color.isValid())
        widgetBars->setBackgroundColor(color);
}
