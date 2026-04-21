#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QColor>
#include <QPointF>
#include <QTimer>
#include <QVector>
#include <QWidget>

class GraphWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GraphWidget(QWidget *parent = nullptr);

    int nodeCount() const;
    void setNodeCount(int count);
    void setNodeColor(const QColor &color);
    void setBackgroundColor(const QColor &color);

public slots:
    void shuffleNodes();
    void connectNodes();
    void runDijkstra();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void showNextDijkstraStep();

private:
    struct Edge
    {
        int from;
        int to;
        int weight;
    };

    enum class StepType
    {
        None,
        SelectCurrent,
        CheckNeighbor,
        UpdateDistance,
        FinalizeNode,
        ShowPath
    };

    struct AnimationStep
    {
        StepType type {StepType::None};
        int node {-1};
        int from {-1};
        int to {-1};
        int distance {0};
    };

    struct PathEdge
    {
        int from;
        int to;
    };

    QVector<QPointF> nodes;
    QVector<Edge> edges;
    QVector<int> visibleDistances;
    QVector<bool> finalizedNodes;
    QVector<PathEdge> shortestPath;
    QVector<AnimationStep> dijkstraSteps;
    QColor nodeColor {"#22D3EE"};
    QColor backgroundColor {"#111827"};

    QTimer dijkstraTimer;
    AnimationStep currentStep;
    int dijkstraStepIndex {0};

    QPointF randomNodePosition() const;
    void clearDijkstraState();
    bool isHighlightedEdge(const Edge &edge, const PathEdge &pathEdge) const;
};

#endif // GRAPHWIDGET_H
