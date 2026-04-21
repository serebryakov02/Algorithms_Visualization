#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QColor>
#include <QPointF>
#include <QString>
#include <QTimer>
#include <QVector>
#include <QWidget>

class QPainter;

class GraphWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GraphWidget(QWidget *parent = nullptr);

    int nodeCount() const;
    void setNodeCount(int count);
    void setNodeColor(const QColor &color);
    void setBackgroundColor(const QColor &color);
    void setSourceNode(int node);
    void setTargetNode(int node);
    void setWeightedGraph(bool weighted);
    void setStatusOverlayVisible(bool visible);

public slots:
    void shuffleNodes();
    void connectNodes();
    void runDijkstra();
    void runBfs();
    void runDfs();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void showNextSearchStep();

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
        ShowPath,
        DfsVisit,
        DfsAdvance,
        DfsBacktrack,
        DfsFinish
    };

    enum class AlgorithmType
    {
        None,
        Dijkstra,
        Bfs,
        Dfs
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
    QVector<bool> activeNodes;
    QVector<PathEdge> activeEdges;
    QVector<PathEdge> backtrackedEdges;
    QVector<AnimationStep> searchSteps;
    AlgorithmType activeAlgorithm {AlgorithmType::None};
    QColor nodeColor {"#22D3EE"};
    QColor backgroundColor {"#111827"};
    QString statusText;
    QString resultText;
    QString pendingResultText;
    int sourceNode {0};
    int targetNode {7};
    bool weightedGraph {true};
    bool statusOverlayVisible {true};

    QTimer searchTimer;
    AnimationStep currentStep;
    int searchStepIndex {0};

    QPointF randomNodePosition() const;
    QString algorithmName() const;
    QString formatPath(const QVector<int> &path) const;
    QString stepDescription(const AnimationStep &step) const;
    void clearSearchState();
    void drawStatusOverlay(QPainter &painter) const;
    bool isHighlightedEdge(const Edge &edge, const PathEdge &pathEdge) const;
};

#endif // GRAPHWIDGET_H
