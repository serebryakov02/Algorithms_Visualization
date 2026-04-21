#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QPointF>
#include <QVector>
#include <QWidget>

class GraphWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GraphWidget(QWidget *parent = nullptr);

    int nodeCount() const;
    void setNodeCount(int count);

public slots:
    void shuffleNodes();
    void connectNodes();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    struct Edge
    {
        int from;
        int to;
        int weight;
    };

    QVector<QPointF> nodes;
    QVector<Edge> edges;

    QPointF randomNodePosition() const;
};

#endif // GRAPHWIDGET_H
