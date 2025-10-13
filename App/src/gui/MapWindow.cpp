#include <QPen>
#include <QWheelEvent>
#include <QMouseEvent>
#include <algorithm>
#include <cmath>

#include "../../include/utils/types.hpp"
#include "../../include/map/Edge.hpp"
#include "../../include/map/Node.hpp"
#include "../../include/gui/MapWindow.hpp"

MapWindow::MapWindow(Map* map, std::vector<nodePtr> path, QWidget* parent)
  : QMainWindow(parent), map(map), path(std::move(path)) {
  resize(1000, 800);
  calculateBounds();
}

void MapWindow::calculateBounds() {
  minLat = 90.0;
  maxLat = -90.0;
  minLon = 180.0;
  maxLon = -180.0;

  for (const auto& [id, node] : map->getNodeRegistry()) {
    if (!node) continue;
    minLat = std::min(minLat, static_cast<double>(node->getLatitude()));
    maxLat = std::max(maxLat, static_cast<double>(node->getLatitude()));
    minLon = std::min(minLon, static_cast<double>(node->getLongitude()));
    maxLon = std::max(maxLon, static_cast<double>(node->getLongitude()));
  }
}

void MapWindow::wheelEvent(QWheelEvent* event) {
  const double zoomStep = 1.15;
  QPointF mousePos = event->position();

  QPointF beforeZoom = (mousePos - panOffset) / zoomFactor;

  if (event->angleDelta().y() > 0)
    zoomFactor *= zoomStep;
  else
    zoomFactor /= zoomStep;

  zoomFactor = std::clamp(zoomFactor, 0.1, 50.0);

  QPointF afterZoom = beforeZoom * zoomFactor + panOffset;
  panOffset += (mousePos - afterZoom);

  update();
}

void MapWindow::mousePressEvent(QMouseEvent* event) {
  if (event->button() == Qt::LeftButton) {
    isPanning = true;
    lastMousePos = event->pos();
    setCursor(Qt::ClosedHandCursor);
  }
}

void MapWindow::mouseMoveEvent(QMouseEvent* event) {
  if (isPanning) {
    QPointF delta = event->pos() - lastMousePos;
    lastMousePos = event->pos();

    panOffset += delta;
    update();
  }
}

void MapWindow::mouseReleaseEvent(QMouseEvent* event) {
  if (event->button() == Qt::LeftButton) {
    isPanning = false;
    setCursor(Qt::ArrowCursor);
  }
}

void MapWindow::paintEvent(QPaintEvent*) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.fillRect(rect(), Qt::white);

  double w = width();
  double h = height();

  auto transformCoord = [&](double lon, double lat) -> QPointF {
    double x = (lon - minLon) / (maxLon - minLon) * w;
    double y = h - (lat - minLat) / (maxLat - minLat) * h;

    return QPointF(x * zoomFactor + panOffset.x(),
                   y * zoomFactor + panOffset.y());
  };

  QPen edgePen(QColor(180, 180, 180));
  edgePen.setWidth(1);
  painter.setPen(edgePen);

  for (const auto& [id, node] : map->getNodeRegistry()) {
    if (!node) continue;

    QPointF p1 = transformCoord(node->getLongitude(),
                                node->getLatitude());

    for (const auto& edge : node->edges) {
      const nodePtr neighbor = edge.getNeighborNode();
      if (!neighbor) continue;

      QPointF p2 = transformCoord(neighbor->getLongitude(),
                                  neighbor->getLatitude());
      painter.drawLine(p1, p2);
    }
  }

  QPen nodePen(Qt::black);
  nodePen.setWidth(2);
  painter.setPen(nodePen);

  for (const auto& [id, node] : map->getNodeRegistry()) {
    if (!node) continue;
    QPointF p = transformCoord(node->getLongitude(),
                               node->getLatitude());
    painter.drawEllipse(p, 2, 2);
  }

  if (!path.empty()) {
    QPen pathPen(Qt::red);
    pathPen.setWidth(3);
    painter.setPen(pathPen);

    for (size_t i = 1; i < path.size(); ++i) {
      QPointF p1 = transformCoord(path[i - 1]->getLongitude(),
                                  path[i - 1]->getLatitude());
      QPointF p2 = transformCoord(path[i]->getLongitude(),
                                  path[i]->getLatitude());
      painter.drawLine(p1, p2);
    }
  }

  painter.setPen(QPen(Qt::darkBlue, 2));
}
