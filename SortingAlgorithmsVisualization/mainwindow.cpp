#include "mainwindow.h"
#include "binarysearchwidget.h"
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

    auto weightedCheckBox = new QCheckBox("Weighted graph");
    weightedCheckBox->setChecked(true);
    auto showStatusCheckBox = new QCheckBox("Show algorithm status");
    showStatusCheckBox->setChecked(true);

    auto shuffleButton = new QPushButton("Shuffle");
    auto connectButton = new QPushButton("Connect");
    auto dijkstraButton = new QPushButton("Run Dijkstra");
    auto bfsButton = new QPushButton("Run BFS");
    bfsButton->setToolTip("Breadth-First Search");
    auto dfsButton = new QPushButton("Run DFS");
    dfsButton->setToolTip("Depth-First Search");
    bfsButton->setEnabled(false);
    dfsButton->setEnabled(false);

    auto graphSetupGroup = new QGroupBox("Graph Setup");
    graphSetupGroup->setAlignment(Qt::AlignCenter);

    auto graphSetupLayout = new QVBoxLayout;
    graphSetupLayout->addLayout(nodesLayout);
    graphSetupLayout->addWidget(weightedCheckBox);
    graphSetupLayout->addWidget(showStatusCheckBox);
    graphSetupLayout->addWidget(shuffleButton);
    graphSetupLayout->addWidget(connectButton);
    graphSetupGroup->setLayout(graphSetupLayout);

    auto graphAlgorithmsGroup = new QGroupBox("Pathfinding Algorithms");
    graphAlgorithmsGroup->setAlignment(Qt::AlignCenter);

    auto sourceLabel = new QLabel("From");
    auto sourceSpinBox = new QSpinBox;
    sourceSpinBox->setMinimum(1);
    sourceSpinBox->setMaximum(graphWidget->nodeCount());
    sourceSpinBox->setValue(1);

    auto targetLabel = new QLabel("To");
    auto targetSpinBox = new QSpinBox;
    targetSpinBox->setMinimum(1);
    targetSpinBox->setMaximum(graphWidget->nodeCount());
    targetSpinBox->setValue(graphWidget->nodeCount());

    auto pathNodesLayout = new QHBoxLayout;
    pathNodesLayout->addWidget(sourceLabel);
    pathNodesLayout->addWidget(sourceSpinBox);
    pathNodesLayout->addWidget(targetLabel);
    pathNodesLayout->addWidget(targetSpinBox);

    auto graphAlgorithmHeaderLayout = new QHBoxLayout;
    auto graphAlgorithmHeader = new QLabel("Algorithm");
    auto graphComplexityHeader = new QLabel("Worst case");
    graphAlgorithmHeader->setAlignment(Qt::AlignCenter);
    graphComplexityHeader->setAlignment(Qt::AlignCenter);
    graphAlgorithmHeaderLayout->addWidget(graphAlgorithmHeader);
    graphAlgorithmHeaderLayout->addWidget(graphComplexityHeader);

    auto graphAlgorithmLayout = new QHBoxLayout;
    auto dijkstraComplexityLabel = new QLabel("O(V^2 + E)");
    dijkstraComplexityLabel->setAlignment(Qt::AlignCenter);
    graphAlgorithmLayout->addWidget(dijkstraButton);
    graphAlgorithmLayout->addWidget(dijkstraComplexityLabel);

    auto bfsAlgorithmLayout = new QHBoxLayout;
    auto bfsComplexityLabel = new QLabel("O(V + E)");
    bfsComplexityLabel->setAlignment(Qt::AlignCenter);
    bfsAlgorithmLayout->addWidget(bfsButton);
    bfsAlgorithmLayout->addWidget(bfsComplexityLabel);

    auto dfsAlgorithmLayout = new QHBoxLayout;
    auto dfsComplexityLabel = new QLabel("O(V + E)");
    dfsComplexityLabel->setAlignment(Qt::AlignCenter);
    dfsAlgorithmLayout->addWidget(dfsButton);
    dfsAlgorithmLayout->addWidget(dfsComplexityLabel);

    auto graphAlgorithmsLayout = new QVBoxLayout;
    graphAlgorithmsLayout->addLayout(pathNodesLayout);
    graphAlgorithmsLayout->addLayout(graphAlgorithmHeaderLayout);
    graphAlgorithmsLayout->addLayout(graphAlgorithmLayout);
    graphAlgorithmsLayout->addLayout(bfsAlgorithmLayout);
    graphAlgorithmsLayout->addLayout(dfsAlgorithmLayout);
    graphAlgorithmsGroup->setLayout(graphAlgorithmsLayout);

    auto graphAppearanceGroup = new QGroupBox("Appearance");
    graphAppearanceGroup->setAlignment(Qt::AlignCenter);

    auto nodeColorButton = new QPushButton("Choose...");
    auto graphBackgroundButton = new QPushButton("Choose...");

    auto nodeColorLayout = new QHBoxLayout;
    nodeColorLayout->addWidget(new QLabel("Node color"));
    nodeColorLayout->addWidget(nodeColorButton);

    auto graphBackgroundLayout = new QHBoxLayout;
    graphBackgroundLayout->addWidget(new QLabel("Background color"));
    graphBackgroundLayout->addWidget(graphBackgroundButton);

    auto graphAppearanceLayout = new QVBoxLayout;
    graphAppearanceLayout->addLayout(nodeColorLayout);
    graphAppearanceLayout->addLayout(graphBackgroundLayout);
    graphAppearanceGroup->setLayout(graphAppearanceLayout);

    auto graphSettingsLayout = new QVBoxLayout;
    graphSettingsLayout->addWidget(graphSetupGroup);
    graphSettingsLayout->addWidget(graphAlgorithmsGroup);
    graphSettingsLayout->addWidget(graphAppearanceGroup);
    graphSettingsLayout->addStretch();
    graphSettingsGroup->setLayout(graphSettingsLayout);

    connect(nodesSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, [graphWidget, sourceSpinBox, targetSpinBox](int value) {
        graphWidget->setNodeCount(value);
        sourceSpinBox->setMaximum(value);
        targetSpinBox->setMaximum(value);

        if (sourceSpinBox->value() > value) {
            sourceSpinBox->setValue(value);
        }

        if (targetSpinBox->value() > value) {
            targetSpinBox->setValue(value);
        }
    });
    connect(sourceSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            graphWidget, &GraphWidget::setSourceNode);
    connect(targetSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            graphWidget, &GraphWidget::setTargetNode);
    connect(weightedCheckBox, &QCheckBox::toggled, this,
            [graphWidget, dijkstraButton, bfsButton, dfsButton](bool weighted) {
        graphWidget->setWeightedGraph(weighted);
        dijkstraButton->setEnabled(weighted);
        bfsButton->setEnabled(!weighted);
        dfsButton->setEnabled(!weighted);
    });
    connect(showStatusCheckBox, &QCheckBox::toggled,
            graphWidget, &GraphWidget::setStatusOverlayVisible);
    connect(shuffleButton, &QPushButton::clicked, graphWidget, &GraphWidget::shuffleNodes);
    connect(connectButton, &QPushButton::clicked, graphWidget, &GraphWidget::connectNodes);
    connect(dijkstraButton, &QPushButton::clicked, graphWidget, &GraphWidget::runDijkstra);
    connect(bfsButton, &QPushButton::clicked, graphWidget, &GraphWidget::runBfs);
    connect(dfsButton, &QPushButton::clicked, graphWidget, &GraphWidget::runDfs);
    connect(nodeColorButton, &QPushButton::clicked, this, [this, graphWidget]() {
        const QColor color = QColorDialog::getColor(Qt::cyan, this, "Select Color");
        graphWidget->setNodeColor(color);
    });
    connect(graphBackgroundButton, &QPushButton::clicked, this, [this, graphWidget]() {
        const QColor color = QColorDialog::getColor(Qt::black, this, "Select Color");
        graphWidget->setBackgroundColor(color);
    });

    auto graphTab = new QWidget;
    auto graphTabLayout = new QHBoxLayout;
    graphTabLayout->addWidget(graphGroup);
    graphTabLayout->addWidget(graphSettingsGroup);
    graphTab->setLayout(graphTabLayout);

    auto binarySearchWidget = new BinarySearchWidget;

    auto searchGroup = new QGroupBox("Binary Search");
    searchGroup->setAlignment(Qt::AlignHCenter);

    auto searchLayout = new QVBoxLayout;
    searchLayout->addWidget(binarySearchWidget);
    searchGroup->setLayout(searchLayout);

    auto searchSettingsGroup = new QGroupBox("Settings");
    searchSettingsGroup->setAlignment(Qt::AlignHCenter);

    auto elementCountLabel = new QLabel("Elements");
    auto elementCountSlider = new QSlider(Qt::Horizontal);
    elementCountSlider->setMinimum(2);
    elementCountSlider->setMaximum(100);
    auto elementCountSpinBox = new QSpinBox;
    elementCountSpinBox->setMinimum(2);
    elementCountSpinBox->setMaximum(100);
    elementCountSpinBox->setValue(binarySearchWidget->valueCount());
    elementCountSlider->setValue(binarySearchWidget->valueCount());

    auto elementCountLayout = new QHBoxLayout;
    elementCountLayout->addWidget(elementCountLabel);
    elementCountLayout->addWidget(elementCountSlider);
    elementCountLayout->addWidget(elementCountSpinBox);

    auto targetValueLabel = new QLabel("Target");
    auto targetValueSpinBox = new QSpinBox;
    targetValueSpinBox->setMinimum(binarySearchWidget->minimumValue());
    targetValueSpinBox->setMaximum(binarySearchWidget->maximumValue());
    targetValueSpinBox->setValue(binarySearchWidget->targetValue());

    auto targetValueLayout = new QHBoxLayout;
    targetValueLayout->addWidget(targetValueLabel);
    targetValueLayout->addWidget(targetValueSpinBox);

    auto pickExistingTargetButton = new QPushButton("Pick Existing");
    auto generateArrayButton = new QPushButton("Generate Sorted Array");

    auto searchSetupGroup = new QGroupBox("Search Setup");
    searchSetupGroup->setAlignment(Qt::AlignCenter);

    auto searchSetupLayout = new QVBoxLayout;
    searchSetupLayout->addLayout(elementCountLayout);
    searchSetupLayout->addLayout(targetValueLayout);
    searchSetupLayout->addWidget(pickExistingTargetButton);
    searchSetupLayout->addWidget(generateArrayButton);
    searchSetupGroup->setLayout(searchSetupLayout);

    auto searchAlgorithmsGroup = new QGroupBox("Search Algorithms");
    searchAlgorithmsGroup->setAlignment(Qt::AlignCenter);

    auto runBinarySearchButton = new QPushButton("Run Binary Search");
    auto searchAlgorithmHeader = new QLabel("Algorithm");
    auto searchComplexityHeader = new QLabel("Worst case");
    searchAlgorithmHeader->setAlignment(Qt::AlignCenter);
    searchComplexityHeader->setAlignment(Qt::AlignCenter);

    auto searchHeaderLayout = new QHBoxLayout;
    searchHeaderLayout->addWidget(searchAlgorithmHeader);
    searchHeaderLayout->addWidget(searchComplexityHeader);

    auto binarySearchLayout = new QHBoxLayout;
    auto binarySearchComplexityLabel = new QLabel("O(log n)");
    binarySearchComplexityLabel->setAlignment(Qt::AlignCenter);
    binarySearchLayout->addWidget(runBinarySearchButton);
    binarySearchLayout->addWidget(binarySearchComplexityLabel);

    auto searchAlgorithmsLayout = new QVBoxLayout;
    searchAlgorithmsLayout->addLayout(searchHeaderLayout);
    searchAlgorithmsLayout->addLayout(binarySearchLayout);
    searchAlgorithmsGroup->setLayout(searchAlgorithmsLayout);

    auto searchAppearanceGroup = new QGroupBox("Appearance");
    searchAppearanceGroup->setAlignment(Qt::AlignCenter);

    auto searchBarColorButton = new QPushButton("Choose...");
    auto searchBackgroundButton = new QPushButton("Choose...");

    auto searchBarColorLayout = new QHBoxLayout;
    searchBarColorLayout->addWidget(new QLabel("Bar color"));
    searchBarColorLayout->addWidget(searchBarColorButton);

    auto searchBackgroundLayout = new QHBoxLayout;
    searchBackgroundLayout->addWidget(new QLabel("Background color"));
    searchBackgroundLayout->addWidget(searchBackgroundButton);

    auto searchAppearanceLayout = new QVBoxLayout;
    searchAppearanceLayout->addLayout(searchBarColorLayout);
    searchAppearanceLayout->addLayout(searchBackgroundLayout);
    searchAppearanceGroup->setLayout(searchAppearanceLayout);

    auto searchSettingsLayout = new QVBoxLayout;
    searchSettingsLayout->addWidget(searchSetupGroup);
    searchSettingsLayout->addWidget(searchAlgorithmsGroup);
    searchSettingsLayout->addWidget(searchAppearanceGroup);
    searchSettingsLayout->addStretch();
    searchSettingsGroup->setLayout(searchSettingsLayout);

    connect(binarySearchWidget, &BinarySearchWidget::valuesGenerated, this,
            [binarySearchWidget, targetValueSpinBox](int minimum, int maximum, int suggestedTarget) {
        targetValueSpinBox->blockSignals(true);
        targetValueSpinBox->setRange(minimum, maximum);
        targetValueSpinBox->setValue(suggestedTarget);
        targetValueSpinBox->blockSignals(false);
        binarySearchWidget->setTargetValue(targetValueSpinBox->value());
    });
    connect(elementCountSlider, &QSlider::valueChanged,
            elementCountSpinBox, &QSpinBox::setValue);
    connect(elementCountSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, [binarySearchWidget, elementCountSlider](int value) {
        elementCountSlider->blockSignals(true);
        elementCountSlider->setValue(value);
        elementCountSlider->blockSignals(false);
        binarySearchWidget->setValueCount(value);
    });
    connect(targetValueSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            binarySearchWidget, &BinarySearchWidget::setTargetValue);
    connect(pickExistingTargetButton, &QPushButton::clicked, this,
            [binarySearchWidget, targetValueSpinBox]() {
        targetValueSpinBox->setValue(binarySearchWidget->randomExistingValue());
    });
    connect(generateArrayButton, &QPushButton::clicked,
            binarySearchWidget, &BinarySearchWidget::generateValues);
    connect(runBinarySearchButton, &QPushButton::clicked,
            binarySearchWidget, &BinarySearchWidget::runBinarySearch);
    connect(searchBarColorButton, &QPushButton::clicked, this, [this, binarySearchWidget]() {
        const QColor color = QColorDialog::getColor(Qt::cyan, this, "Select Color");
        binarySearchWidget->setBarColor(color);
    });
    connect(searchBackgroundButton, &QPushButton::clicked, this,
            [this, binarySearchWidget]() {
        const QColor color = QColorDialog::getColor(Qt::black, this, "Select Color");
        binarySearchWidget->setBackgroundColor(color);
    });

    auto searchTab = new QWidget;
    auto searchTabLayout = new QHBoxLayout;
    searchTabLayout->addWidget(searchGroup);
    searchTabLayout->addWidget(searchSettingsGroup);
    searchTab->setLayout(searchTabLayout);

    auto tabWidget = new QTabWidget;
    tabWidget->addTab(ui->mainWidget, "Sorting Algorithms");
    tabWidget->addTab(graphTab, "Graph Visualizer");
    tabWidget->addTab(searchTab, "Search Algorithms");

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
