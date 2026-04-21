#include "graphwidget.h"

#include <QLinearGradient>
#include <QPainter>
#include <QRandomGenerator>

GraphWidget::GraphWidget(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(800, 600);
    setNodeCount(8);
}

int GraphWidget::nodeCount() const
{
    return nodes.size();
}

void GraphWidget::setNodeCount(int count)
{
    nodes.clear();
    edges.clear();

    for (int i = 0; i < count; ++i) {
        nodes.push_back(randomNodePosition());
    }

    update();
}

void GraphWidget::shuffleNodes()
{
    for (auto &node : nodes) {
        node = randomNodePosition();
    }

    update();
}

void GraphWidget::connectNodes()
{
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

void GraphWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QLinearGradient background(0, 0, 0, height());
    background.setColorAt(0.0, QColor("#111827").lighter(125));
    background.setColorAt(1.0, QColor("#111827").darker(140));
    painter.fillRect(rect(), background);

    constexpr int radius = 16;

    auto pointForNode = [this](const QPointF &node) {
        constexpr int margin = 54;
        return QPointF(margin + node.x() * (width() - margin * 2),
                       margin + node.y() * (height() - margin * 2));
    };

    painter.setPen(QPen(QColor("#CBD5E1"), 2));
    for (const auto &edge : edges) {
        const QPointF from = pointForNode(nodes.at(edge.from));
        const QPointF to = pointForNode(nodes.at(edge.to));
        painter.drawLine(from, to);

        const QPointF center = (from + to) / 2;
        const QRectF labelRect(center.x() - 15, center.y() - 11, 30, 22);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(15, 23, 42, 210));
        painter.drawRoundedRect(labelRect, 6, 6);
        painter.setPen(QColor("#F8FAFC"));
        painter.drawText(labelRect, Qt::AlignCenter, QString::number(edge.weight));
        painter.setPen(QPen(QColor("#CBD5E1"), 2));
    }

    painter.setPen(QPen(QColor("#F8FAFC"), 2));
    painter.setBrush(QColor("#22D3EE"));
    for (int i = 0; i < nodes.size(); ++i) {
        const QPointF point = pointForNode(nodes.at(i));
        const QRectF nodeRect(point.x() - radius, point.y() - radius, radius * 2, radius * 2);

        painter.drawEllipse(nodeRect);
        painter.drawText(nodeRect, Qt::AlignCenter, QString::number(i + 1));
    }
}

QPointF GraphWidget::randomNodePosition() const
{
    const double x = 0.08 + QRandomGenerator::global()->bounded(840) / 1000.0;
    const double y = 0.08 + QRandomGenerator::global()->bounded(840) / 1000.0;
    return QPointF(x, y);
}
