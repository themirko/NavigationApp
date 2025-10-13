#pragma once

#include <QMainWindow>
#include <QPainter>
#include "../../include/map/Map.hpp"

class MapWindow : public QMainWindow {
    Q_OBJECT

  public:
    explicit MapWindow(Map* map,
                       std::vector<nodePtr> path = {},
                       QWidget* parent = nullptr);
    ~MapWindow() override = default;

protected:
    void paintEvent(QPaintEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    Map* map;
    std::vector<nodePtr> path;
    double minLat, maxLat, minLon, maxLon;

    double zoomFactor = 1.0;
    QPointF panOffset = {0, 0};
    QPoint lastMousePos;
    bool isPanning = false;

    void calculateBounds();
};
