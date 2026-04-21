#include "graphwidget.h"

#include <QLinearGradient>
#include <QPainter>
#include <QRandomGenerator>
#include <QStringList>
#include <functional>

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
    searchTimer.setInterval(450);
    connect(&searchTimer, &QTimer::timeout, this, &GraphWidget::showNextSearchStep);
    setNodeCount(8);
}

int GraphWidget::nodeCount() const
{
    return nodes.size();
}

void GraphWidget::setNodeCount(int count)
{
    clearSearchState();

    nodes.clear();
    edges.clear();

    for (int i = 0; i < count; ++i) {
        nodes.push_back(randomNodePosition());
    }

    sourceNode = qBound(0, sourceNode, nodes.size() - 1);
    targetNode = qBound(0, targetNode, nodes.size() - 1);

    clearSearchState();
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

void GraphWidget::setSourceNode(int node)
{
    sourceNode = qBound(0, node - 1, nodes.size() - 1);
    clearSearchState();
    update();
}

void GraphWidget::setTargetNode(int node)
{
    targetNode = qBound(0, node - 1, nodes.size() - 1);
    clearSearchState();
    update();
}

void GraphWidget::setWeightedGraph(bool weighted)
{
    weightedGraph = weighted;
    clearSearchState();
    update();
}

void GraphWidget::setStatusOverlayVisible(bool visible)
{
    statusOverlayVisible = visible;
    update();
}

void GraphWidget::shuffleNodes()
{
    clearSearchState();

    for (auto &node : nodes) {
        node = randomNodePosition();
    }

    update();
}

void GraphWidget::connectNodes()
{
    clearSearchState();
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
    clearSearchState();
    activeAlgorithm = AlgorithmType::Dijkstra;
    statusText = "Preparing Dijkstra";

    if (nodes.isEmpty()) {
        update();
        return;
    }

    const int source = sourceNode;
    const int target = targetNode;

    QVector<int> distances(nodes.size(), InfiniteDistance);
    QVector<int> previous(nodes.size(), -1);
    QVector<bool> visited(nodes.size(), false);
    QVector<QVector<AdjacentEdge>> adjacency(nodes.size());
    distances[source] = 0;

    for (const auto &edge : edges) {
        adjacency[edge.from].push_back({edge.to, edge.weight});
        adjacency[edge.to].push_back({edge.from, edge.weight});
    }

    searchSteps.push_back({StepType::UpdateDistance, source, -1, -1, 0});

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

        searchSteps.push_back({StepType::SelectCurrent, current, -1, -1, distances[current]});

        for (const auto &edge : adjacency.at(current)) {
            if (visited[edge.to]) {
                continue;
            }

            searchSteps.push_back({StepType::CheckNeighbor, edge.to, current, edge.to,
                                     distances[edge.to]});

            const int candidateDistance = distances[current] + edge.weight;
            if (candidateDistance < distances[edge.to]) {
                distances[edge.to] = candidateDistance;
                previous[edge.to] = current;
                searchSteps.push_back({StepType::UpdateDistance, edge.to, current, edge.to,
                                         candidateDistance});
            }
        }

        visited[current] = true;
        searchSteps.push_back({StepType::FinalizeNode, current, -1, -1, distances[current]});
    }

    if (distances[target] != InfiniteDistance) {
        QVector<int> path;

        for (int node = target; node != -1; node = previous[node]) {
            path.prepend(node);
        }

        pendingResultText = QString("Shortest path: %1\nDistance: %2")
                                .arg(formatPath(path))
                                .arg(distances[target]);

        for (int i = 1; i < path.size(); ++i) {
            searchSteps.push_back({StepType::ShowPath, -1, path.at(i - 1), path.at(i), 0});
        }
    } else {
        pendingResultText = "No path found";
    }

    showNextSearchStep();
    searchTimer.start();
}

void GraphWidget::runBfs()
{
    clearSearchState();
    activeAlgorithm = AlgorithmType::Bfs;
    statusText = "Preparing BFS";

    if (nodes.isEmpty()) {
        update();
        return;
    }

    QVector<QVector<int>> adjacency(nodes.size());
    for (const auto &edge : edges) {
        adjacency[edge.from].push_back(edge.to);
        adjacency[edge.to].push_back(edge.from);
    }

    QVector<int> distances(nodes.size(), InfiniteDistance);
    QVector<int> previous(nodes.size(), -1);
    QVector<bool> discovered(nodes.size(), false);
    QVector<int> queue;
    int queueHead = 0;

    distances[sourceNode] = 0;
    discovered[sourceNode] = true;
    queue.push_back(sourceNode);
    searchSteps.push_back({StepType::UpdateDistance, sourceNode, -1, -1, 0});

    while (queueHead < queue.size()) {
        const int current = queue.at(queueHead);
        ++queueHead;

        searchSteps.push_back({StepType::SelectCurrent, current, -1, -1, distances[current]});

        if (current == targetNode) {
            searchSteps.push_back({StepType::FinalizeNode, current, -1, -1, distances[current]});
            break;
        }

        for (int neighbor : adjacency.at(current)) {
            searchSteps.push_back({StepType::CheckNeighbor, neighbor, current, neighbor,
                                     distances[neighbor]});

            if (discovered[neighbor]) {
                continue;
            }

            discovered[neighbor] = true;
            distances[neighbor] = distances[current] + 1;
            previous[neighbor] = current;
            queue.push_back(neighbor);
            searchSteps.push_back({StepType::UpdateDistance, neighbor, current, neighbor,
                                     distances[neighbor]});
        }

        searchSteps.push_back({StepType::FinalizeNode, current, -1, -1, distances[current]});
    }

    if (distances[targetNode] != InfiniteDistance) {
        QVector<int> path;

        for (int node = targetNode; node != -1; node = previous[node]) {
            path.prepend(node);
        }

        pendingResultText = QString("Shortest path: %1\nEdges: %2")
                                .arg(formatPath(path))
                                .arg(distances[targetNode]);

        for (int i = 1; i < path.size(); ++i) {
            searchSteps.push_back({StepType::ShowPath, -1, path.at(i - 1), path.at(i), 0});
        }
    } else {
        pendingResultText = "No path found";
    }

    showNextSearchStep();
    searchTimer.start();
}

void GraphWidget::runDfs()
{
    clearSearchState();
    activeAlgorithm = AlgorithmType::Dfs;
    statusText = "Preparing DFS";

    if (nodes.isEmpty()) {
        update();
        return;
    }

    QVector<QVector<int>> adjacency(nodes.size());
    for (const auto &edge : edges) {
        adjacency[edge.from].push_back(edge.to);
        adjacency[edge.to].push_back(edge.from);
    }

    QVector<bool> visited(nodes.size(), false);
    QVector<int> currentPath;
    QVector<int> foundPath;

    std::function<bool(int)> dfs = [&](int node) {
        visited[node] = true;
        currentPath.push_back(node);
        searchSteps.push_back({StepType::DfsVisit, node, -1, -1, 0});

        if (node == targetNode) {
            foundPath = currentPath;
            return true;
        }

        for (int neighbor : adjacency.at(node)) {
            if (visited[neighbor]) {
                continue;
            }

            searchSteps.push_back({StepType::DfsAdvance, neighbor, node, neighbor, 0});

            if (dfs(neighbor)) {
                return true;
            }

            searchSteps.push_back({StepType::DfsBacktrack, node, node, neighbor, 0});
        }

        searchSteps.push_back({StepType::DfsFinish, node, -1, -1, 0});
        currentPath.removeLast();
        return false;
    };

    if (dfs(sourceNode)) {
        pendingResultText = QString("Path found: %1").arg(formatPath(foundPath));
    } else {
        pendingResultText = "No path found";
    }

    showNextSearchStep();
    searchTimer.start();
}

void GraphWidget::showNextSearchStep()
{
    if (searchStepIndex >= searchSteps.size()) {
        searchTimer.stop();
        currentStep = {};
        statusText = activeAlgorithm == AlgorithmType::None ? QString() : "Finished";

        if (!pendingResultText.isEmpty()) {
            resultText = pendingResultText;
        }

        update();
        return;
    }

    currentStep = searchSteps.at(searchStepIndex);
    ++searchStepIndex;
    statusText = stepDescription(currentStep);

    if (currentStep.type == StepType::UpdateDistance) {
        visibleDistances[currentStep.node] = currentStep.distance;
    } else if (currentStep.type == StepType::FinalizeNode) {
        finalizedNodes[currentStep.node] = true;
    } else if (currentStep.type == StepType::ShowPath) {
        shortestPath.push_back({currentStep.from, currentStep.to});
    } else if (currentStep.type == StepType::DfsVisit) {
        activeNodes[currentStep.node] = true;
    } else if (currentStep.type == StepType::DfsAdvance) {
        activeEdges.push_back({currentStep.from, currentStep.to});
    } else if (currentStep.type == StepType::DfsBacktrack) {
        for (int i = activeEdges.size() - 1; i >= 0; --i) {
            const auto &edge = activeEdges.at(i);
            if ((edge.from == currentStep.from && edge.to == currentStep.to)
                || (edge.from == currentStep.to && edge.to == currentStep.from)) {
                activeEdges.removeAt(i);
                break;
            }
        }

        activeNodes[currentStep.to] = false;
        backtrackedEdges.push_back({currentStep.from, currentStep.to});
    } else if (currentStep.type == StepType::DfsFinish) {
        finalizedNodes[currentStep.node] = true;
        activeNodes[currentStep.node] = false;
    }

    if (currentStep.type == StepType::ShowPath && !pendingResultText.isEmpty()) {
        resultText = pendingResultText;
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

        for (const auto &backtrackedEdge : backtrackedEdges) {
            if (isHighlightedEdge(edge, backtrackedEdge)) {
                edgeColor = QColor("#EF4444");
                edgeWidth = 3;
                break;
            }
        }

        for (const auto &activeEdge : activeEdges) {
            if (isHighlightedEdge(edge, activeEdge)) {
                edgeColor = QColor("#3B82F6");
                edgeWidth = 4;
                break;
            }
        }

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
        } else if (currentStep.type == StepType::DfsAdvance
                   && isHighlightedEdge(edge, {currentStep.from, currentStep.to})) {
            edgeColor = QColor("#3B82F6");
            edgeWidth = 5;
        } else if (currentStep.type == StepType::DfsBacktrack
                   && isHighlightedEdge(edge, {currentStep.from, currentStep.to})) {
            edgeColor = QColor("#EF4444");
            edgeWidth = 5;
        }

        painter.setPen(QPen(edgeColor, edgeWidth));
        painter.drawLine(from, to);

        if (weightedGraph) {
            const QPointF center = (from + to) / 2;
            const QRectF labelRect(center.x() - 15, center.y() - 11, 30, 22);
            painter.setPen(Qt::NoPen);
            painter.setBrush(QColor(15, 23, 42, 210));
            painter.drawRoundedRect(labelRect, 6, 6);
            painter.setPen(QColor("#F8FAFC"));
            painter.drawText(labelRect, Qt::AlignCenter, QString::number(edge.weight));
        }
    }

    painter.setPen(QPen(QColor("#F8FAFC"), 2));
    for (int i = 0; i < nodes.size(); ++i) {
        const QPointF point = pointForNode(nodes.at(i));
        const QRectF nodeRect(point.x() - radius, point.y() - radius, radius * 2, radius * 2);

        QColor currentNodeColor = nodeColor;

        if (finalizedNodes.value(i)) {
            currentNodeColor = QColor("#22C55E");
        }

        if (activeNodes.value(i)) {
            currentNodeColor = QColor("#3B82F6");
        }

        if (currentStep.node == i && currentStep.type == StepType::SelectCurrent) {
            currentNodeColor = QColor("#FACC15");
        } else if (currentStep.node == i && currentStep.type == StepType::UpdateDistance) {
            currentNodeColor = QColor("#3B82F6");
        } else if (currentStep.node == i
                   && (currentStep.type == StepType::DfsVisit
                       || currentStep.type == StepType::DfsBacktrack)) {
            currentNodeColor = QColor("#FACC15");
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

    drawStatusOverlay(painter);
}

QPointF GraphWidget::randomNodePosition() const
{
    const double x = 0.08 + QRandomGenerator::global()->bounded(840) / 1000.0;
    const double y = 0.08 + QRandomGenerator::global()->bounded(840) / 1000.0;
    return QPointF(x, y);
}

QString GraphWidget::algorithmName() const
{
    switch (activeAlgorithm) {
    case AlgorithmType::Dijkstra:
        return "Dijkstra";
    case AlgorithmType::Bfs:
        return "BFS";
    case AlgorithmType::Dfs:
        return "DFS";
    case AlgorithmType::None:
        return {};
    }

    return {};
}

QString GraphWidget::formatPath(const QVector<int> &path) const
{
    QString text;

    for (int i = 0; i < path.size(); ++i) {
        if (i > 0) {
            text += " -> ";
        }

        text += QString::number(path.at(i) + 1);
    }

    return text;
}

QString GraphWidget::stepDescription(const AnimationStep &step) const
{
    const auto nodeLabel = [](int node) {
        return QString::number(node + 1);
    };

    switch (step.type) {
    case StepType::SelectCurrent:
        return QString("Current: select node %1").arg(nodeLabel(step.node));
    case StepType::CheckNeighbor:
        return QString("Current: check edge %1 -> %2")
            .arg(nodeLabel(step.from), nodeLabel(step.to));
    case StepType::UpdateDistance:
        if (step.from == -1) {
            return QString("Current: start at node %1").arg(nodeLabel(step.node));
        }

        if (activeAlgorithm == AlgorithmType::Bfs) {
            return QString("Current: discover node %1 at level %2")
                .arg(nodeLabel(step.node))
                .arg(step.distance);
        }

        return QString("Current: update node %1 to distance %2")
            .arg(nodeLabel(step.node))
            .arg(step.distance);
    case StepType::FinalizeNode:
        return QString("Current: finish node %1").arg(nodeLabel(step.node));
    case StepType::ShowPath:
        return QString("Current: draw path edge %1 -> %2")
            .arg(nodeLabel(step.from), nodeLabel(step.to));
    case StepType::DfsVisit:
        return QString("Current: visit node %1").arg(nodeLabel(step.node));
    case StepType::DfsAdvance:
        return QString("Current: follow edge %1 -> %2")
            .arg(nodeLabel(step.from), nodeLabel(step.to));
    case StepType::DfsBacktrack:
        return QString("Current: backtrack from %1 to %2")
            .arg(nodeLabel(step.to), nodeLabel(step.from));
    case StepType::DfsFinish:
        return QString("Current: finish node %1").arg(nodeLabel(step.node));
    case StepType::None:
        return {};
    }

    return {};
}

void GraphWidget::clearSearchState()
{
    searchTimer.stop();
    searchSteps.clear();
    shortestPath.clear();
    activeEdges.clear();
    backtrackedEdges.clear();
    activeAlgorithm = AlgorithmType::None;
    statusText.clear();
    resultText.clear();
    pendingResultText.clear();
    searchStepIndex = 0;
    currentStep = {};

    visibleDistances = QVector<int>(nodes.size(), InfiniteDistance);
    finalizedNodes = QVector<bool>(nodes.size(), false);
    activeNodes = QVector<bool>(nodes.size(), false);
}

void GraphWidget::drawStatusOverlay(QPainter &painter) const
{
    if (!statusOverlayVisible) {
        return;
    }

    QStringList lines;
    const QString name = algorithmName();

    if (!name.isEmpty()) {
        lines.push_back(name);
    }

    if (!statusText.isEmpty()) {
        lines.push_back(statusText);
    }

    if (!resultText.isEmpty()) {
        lines.append(resultText.split('\n'));
    }

    if (lines.isEmpty()) {
        return;
    }

    painter.save();

    QFont font = painter.font();
    font.setPointSize(12);
    painter.setFont(font);

    const QFontMetrics metrics(font);
    constexpr int margin = 16;
    constexpr int padding = 12;
    const int textWidth = qMin(360, width() - margin * 2 - padding * 2);
    int textHeight = 0;

    for (const auto &line : lines) {
        const QRect textRect = metrics.boundingRect(QRect(0, 0, textWidth, 1000),
                                                    Qt::TextWordWrap, line);
        textHeight += textRect.height() + 4;
    }

    QRectF panel(margin, margin, textWidth + padding * 2, textHeight + padding * 2);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(15, 23, 42, 210));
    painter.drawRoundedRect(panel, 6, 6);

    painter.setPen(QColor("#F8FAFC"));
    int y = panel.top() + padding;

    for (const auto &line : lines) {
        const QRect textRect = metrics.boundingRect(QRect(0, 0, textWidth, 1000),
                                                    Qt::TextWordWrap, line);
        painter.drawText(QRectF(panel.left() + padding, y, textWidth, textRect.height()),
                         Qt::TextWordWrap, line);
        y += textRect.height() + 4;
    }

    painter.restore();
}

bool GraphWidget::isHighlightedEdge(const Edge &edge, const PathEdge &pathEdge) const
{
    return (edge.from == pathEdge.from && edge.to == pathEdge.to)
           || (edge.from == pathEdge.to && edge.to == pathEdge.from);
}
