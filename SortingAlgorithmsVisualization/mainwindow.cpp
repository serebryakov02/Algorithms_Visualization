#include "mainwindow.h"
#include "graphwidget.h"
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

    auto visualizationGroup = new QGroupBox("Visualization");
    visualizationGroup->setAlignment(Qt::AlignHCenter);

    auto visualizationLayout = new QVBoxLayout;
    visualizationLayout->addWidget(widgetBars);
    visualizationGroup->setLayout(visualizationLayout);

    auto settingsGroup = new QGroupBox("Settings");
    settingsGroup->setAlignment(Qt::AlignHCenter);

    auto settingsLayout = new QVBoxLayout;
    settingsLayout->addWidget(ui->widgetSettings);
    settingsLayout->addWidget(ui->widgetStyling);

    // Create a vertical spacer
    auto vSpacer = new QSpacerItem(20, 40,
                                   QSizePolicy::Minimum, QSizePolicy::Expanding);
    settingsLayout->addSpacerItem(vSpacer);
    settingsGroup->setLayout(settingsLayout);

    auto hLayout = new QHBoxLayout;
    hLayout->addWidget(visualizationGroup);
    hLayout->addWidget(settingsGroup);

    ui->mainWidget->setLayout(hLayout);

    auto graphWidget = new GraphWidget;

    auto graphGroup = new QGroupBox("Graph");
    graphGroup->setAlignment(Qt::AlignHCenter);

    auto graphLayout = new QVBoxLayout;
    graphLayout->addWidget(graphWidget);
    graphGroup->setLayout(graphLayout);

    auto graphSettingsGroup = new QGroupBox("Settings");
    graphSettingsGroup->setAlignment(Qt::AlignHCenter);

    auto nodesLabel = new QLabel("Vertices");
    auto nodesSpinBox = new QSpinBox;
    nodesSpinBox->setMinimum(2);
    nodesSpinBox->setMaximum(30);
    nodesSpinBox->setValue(graphWidget->nodeCount());

    auto nodesLayout = new QHBoxLayout;
    nodesLayout->addWidget(nodesLabel);
    nodesLayout->addWidget(nodesSpinBox);

    auto shuffleButton = new QPushButton("Shuffle");
    auto connectButton = new QPushButton("Connect");

    auto graphSettingsLayout = new QVBoxLayout;
    graphSettingsLayout->addLayout(nodesLayout);
    graphSettingsLayout->addWidget(shuffleButton);
    graphSettingsLayout->addWidget(connectButton);
    graphSettingsLayout->addStretch();
    graphSettingsGroup->setLayout(graphSettingsLayout);

    connect(nodesSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            graphWidget, &GraphWidget::setNodeCount);
    connect(shuffleButton, &QPushButton::clicked, graphWidget, &GraphWidget::shuffleNodes);
    connect(connectButton, &QPushButton::clicked, graphWidget, &GraphWidget::connectNodes);

    auto graphTab = new QWidget;
    auto graphTabLayout = new QHBoxLayout;
    graphTabLayout->addWidget(graphGroup);
    graphTabLayout->addWidget(graphSettingsGroup);
    graphTab->setLayout(graphTabLayout);

    auto tabWidget = new QTabWidget;
    tabWidget->addTab(ui->mainWidget, "Sorting Algorithms");
    tabWidget->addTab(graphTab, "Graph Visualizer");

    setCentralWidget(tabWidget);
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
