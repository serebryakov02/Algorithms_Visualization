#include "graphwidget.h"

#include <QLinearGradient>
#include <QPainter>
#include <QRandomGenerator>

namespace {
constexpr int InfiniteDistance = 1000000000;

struct AdjacentEdge
{
    int to;
    int weight;
};
}

GraphWidget::GraphWidget(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(800, 600);
    dijkstraTimer.setInterval(450);
    connect(&dijkstraTimer, &QTimer::timeout, this, &GraphWidget::showNextDijkstraStep);
    setNodeCount(8);
}

int GraphWidget::nodeCount() const
{
    return nodes.size();
}

void GraphWidget::setNodeCount(int count)
{
    clearDijkstraState();

    nodes.clear();
    edges.clear();

    for (int i = 0; i < count; ++i) {
        nodes.push_back(randomNodePosition());
    }

    clearDijkstraState();
    update();
}

void GraphWidget::setNodeColor(const QColor &color)
{
    if (!color.isValid()) {
        return;
    }

    nodeColor = color;
    update();
}

void GraphWidget::setBackgroundColor(const QColor &color)
{
    if (!color.isValid()) {
        return;
    }

    backgroundColor = color;
    update();
}

void GraphWidget::shuffleNodes()
{
    clearDijkstraState();

    for (auto &node : nodes) {
        node = randomNodePosition();
    }

    update();
}

void GraphWidget::connectNodes()
{
    clearDijkstraState();
    edges.clear();

    if (nodes.size() < 2) {
        update();
        return;
    }

    QVector<int> order;
    for (int i = 0; i < nodes.size(); ++i) {
        order.push_back(i);
    }

    for (int i = order.size() - 1; i > 0; --i) {
        const int j = QRandomGenerator::global()->bounded(i + 1);
        order.swapItemsAt(i, j);
    }

    auto addEdge = [this](int from, int to) {
        const int weight = QRandomGenerator::global()->bounded(1, 21);
        edges.push_back({from, to, weight});
    };

    auto hasEdge = [this](int from, int to) {
        for (const auto &edge : edges) {
            if ((edge.from == from && edge.to == to) || (edge.from == to && edge.to == from)) {
                return true;
            }
        }

        return false;
    };

    for (int i = 1; i < order.size(); ++i) {
        addEdge(order[i - 1], order[i]);
    }

    for (int from = 0; from < nodes.size(); ++from) {
        for (int to = from + 1; to < nodes.size(); ++to) {
            if (!hasEdge(from, to) && QRandomGenerator::global()->bounded(100) < 30) {
                addEdge(from, to);
            }
        }
    }

    update();
}

void GraphWidget::runDijkstra()
{
    clearDijkstraState();

    if (nodes.isEmpty()) {
        update();
        return;
    }

    const int source = 0;
    const int target = nodes.size() - 1;

    QVector<int> distances(nodes.size(), InfiniteDistance);
    QVector<int> previous(nodes.size(), -1);
    QVector<bool> visited(nodes.size(), false);
    QVector<QVector<AdjacentEdge>> adjacency(nodes.size());
    distances[source] = 0;

    for (const auto &edge : edges) {
        adjacency[edge.from].push_back({edge.to, edge.weight});
        adjacency[edge.to].push_back({edge.from, edge.weight});
    }

    dijkstraSteps.push_back({StepType::UpdateDistance, source, -1, -1, 0});

    for (int step = 0; step < nodes.size(); ++step) {
        int current = -1;
        int bestDistance = InfiniteDistance;

        for (int node = 0; node < nodes.size(); ++node) {
            if (!visited[node] && distances[node] < bestDistance) {
                current = node;
                bestDistance = distances[node];
            }
        }

        if (current == -1) {
            break;
        }

        dijkstraSteps.push_back({StepType::SelectCurrent, current, -1, -1, distances[current]});

        for (const auto &edge : adjacency.at(current)) {
            if (visited[edge.to]) {
                continue;
            }

            dijkstraSteps.push_back({StepType::CheckNeighbor, edge.to, current, edge.to,
                                     distances[edge.to]});

            const int candidateDistance = distances[current] + edge.weight;
            if (candidateDistance < distances[edge.to]) {
                distances[edge.to] = candidateDistance;
                previous[edge.to] = current;
                dijkstraSteps.push_back({StepType::UpdateDistance, edge.to, current, edge.to,
                                         candidateDistance});
            }
        }

        visited[current] = true;
        dijkstraSteps.push_back({StepType::FinalizeNode, current, -1, -1, distances[current]});
    }

    if (distances[target] != InfiniteDistance) {
        QVector<PathEdge> path;

        for (int node = target; previous[node] != -1; node = previous[node]) {
            path.prepend({previous[node], node});
        }

        for (const auto &edge : path) {
            dijkstraSteps.push_back({StepType::ShowPath, -1, edge.from, edge.to, 0});
        }
    }

    showNextDijkstraStep();
    dijkstraTimer.start();
}

void GraphWidget::showNextDijkstraStep()
{
    if (dijkstraStepIndex >= dijkstraSteps.size()) {
        dijkstraTimer.stop();
        currentStep = {};
        update();
        return;
    }

    currentStep = dijkstraSteps.at(dijkstraStepIndex);
    ++dijkstraStepIndex;

    if (currentStep.type == StepType::UpdateDistance) {
        visibleDistances[currentStep.node] = currentStep.distance;
    } else if (currentStep.type == StepType::FinalizeNode) {
        finalizedNodes[currentStep.node] = true;
    } else if (currentStep.type == StepType::ShowPath) {
        shortestPath.push_back({currentStep.from, currentStep.to});
    }

    update();
}

void GraphWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QLinearGradient background(0, 0, 0, height());
    background.setColorAt(0.0, backgroundColor.lighter(125));
    background.setColorAt(1.0, backgroundColor.darker(140));
    painter.fillRect(rect(), background);

    constexpr int radius = 16;

    auto pointForNode = [this](const QPointF &node) {
        constexpr int margin = 54;
        return QPointF(margin + node.x() * (width() - margin * 2),
                       margin + node.y() * (height() - margin * 2));
    };

    for (const auto &edge : edges) {
        const QPointF from = pointForNode(nodes.at(edge.from));
        const QPointF to = pointForNode(nodes.at(edge.to));

        QColor edgeColor("#CBD5E1");
        int edgeWidth = 2;

        for (const auto &pathEdge : shortestPath) {
            if (isHighlightedEdge(edge, pathEdge)) {
                edgeColor = QColor("#EF4444");
                edgeWidth = 5;
                break;
            }
        }

        if ((currentStep.type == StepType::CheckNeighbor
             || currentStep.type == StepType::UpdateDistance)
            && isHighlightedEdge(edge, {currentStep.from, currentStep.to})) {
            edgeColor = QColor("#3B82F6");
            edgeWidth = 4;
        }

        painter.setPen(QPen(edgeColor, edgeWidth));
        painter.drawLine(from, to);

        const QPointF center = (from + to) / 2;
        const QRectF labelRect(center.x() - 15, center.y() - 11, 30, 22);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(15, 23, 42, 210));
        painter.drawRoundedRect(labelRect, 6, 6);
        painter.setPen(QColor("#F8FAFC"));
        painter.drawText(labelRect, Qt::AlignCenter, QString::number(edge.weight));
    }

    painter.setPen(QPen(QColor("#F8FAFC"), 2));
    for (int i = 0; i < nodes.size(); ++i) {
        const QPointF point = pointForNode(nodes.at(i));
        const QRectF nodeRect(point.x() - radius, point.y() - radius, radius * 2, radius * 2);

        QColor currentNodeColor = nodeColor;

        if (finalizedNodes.value(i)) {
            currentNodeColor = QColor("#22C55E");
        }

        if (currentStep.node == i && currentStep.type == StepType::SelectCurrent) {
            currentNodeColor = QColor("#FACC15");
        } else if (currentStep.node == i && currentStep.type == StepType::UpdateDistance) {
            currentNodeColor = QColor("#3B82F6");
        }

        painter.setBrush(currentNodeColor);
        painter.setPen(QPen(QColor("#F8FAFC"), 2));
        painter.drawEllipse(nodeRect);
        painter.drawText(nodeRect, Qt::AlignCenter, QString::number(i + 1));

        if (visibleDistances.value(i, InfiniteDistance) != InfiniteDistance) {
            const QRectF distanceRect(point.x() - 28, point.y() + radius + 5, 56, 20);
            painter.setPen(Qt::NoPen);
            painter.setBrush(QColor(15, 23, 42, 210));
            painter.drawRoundedRect(distanceRect, 5, 5);
            painter.setPen(QColor("#F8FAFC"));
            painter.drawText(distanceRect, Qt::AlignCenter,
                             QString("d=%1").arg(visibleDistances.at(i)));
        }
    }
}

QPointF GraphWidget::randomNodePosition() const
{
    const double x = 0.08 + QRandomGenerator::global()->bounded(840) / 1000.0;
    const double y = 0.08 + QRandomGenerator::global()->bounded(840) / 1000.0;
    return QPointF(x, y);
}

void GraphWidget::clearDijkstraState()
{
    dijkstraTimer.stop();
    dijkstraSteps.clear();
    shortestPath.clear();
    dijkstraStepIndex = 0;
    currentStep = {};

    visibleDistances = QVector<int>(nodes.size(), InfiniteDistance);
    finalizedNodes = QVector<bool>(nodes.size(), false);
}

bool GraphWidget::isHighlightedEdge(const Edge &edge, const PathEdge &pathEdge) const
{
    return (edge.from == pathEdge.from && edge.to == pathEdge.to)
           || (edge.from == pathEdge.to && edge.to == pathEdge.from);
}
