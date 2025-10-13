#include <QPen>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QMessageBox>

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
  setupControls();
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

void MapWindow::drawArrow(QPainter& painter,
                          const QPointF& p1,
                          const QPointF& p2) const {
  QLineF line(p1, p2);
  double angle = std::atan2(-line.dy(), line.dx());
  const double arrowSize = 6.0;

  QPointF arrowP1 = p2 - QPointF(std::cos(angle + M_PI / 6) * arrowSize,
                                 -std::sin(angle + M_PI / 6) * arrowSize);
  QPointF arrowP2 = p2 - QPointF(std::cos(angle - M_PI / 6) * arrowSize,
                                 -std::sin(angle - M_PI / 6) * arrowSize);

  QPolygonF arrowHead;
  arrowHead << p2 << arrowP1 << arrowP2;

  painter.save();
  painter.setBrush(QColor("#FFF176"));
  painter.setPen(QPen(Qt::black, 1));
  painter.drawPolygon(arrowHead);
  painter.restore();
}

void MapWindow::setupControls() {
  QWidget* panel = new QWidget(this);
  panel->setFixedHeight(80);

  startLatInput = new QLineEdit();
  startLatInput->setPlaceholderText("Start latitude");

  startLonInput = new QLineEdit();
  startLonInput->setPlaceholderText("Start longitude");

  endLatInput = new QLineEdit();
  endLatInput->setPlaceholderText("End latitude");

  endLonInput = new QLineEdit();
  endLonInput->setPlaceholderText("End longitude");

  calcButton = new QPushButton("Find route");

  QHBoxLayout* leftLayout = new QHBoxLayout();
  leftLayout->addWidget(startLatInput);
  leftLayout->addWidget(startLonInput);

  QHBoxLayout* rightLayout = new QHBoxLayout();
  rightLayout->addWidget(endLatInput);
  rightLayout->addWidget(endLonInput);

  QHBoxLayout* mainLayout = new QHBoxLayout();
  mainLayout->addLayout(leftLayout);
  mainLayout->addStretch();
  mainLayout->addWidget(calcButton);
  mainLayout->addStretch();
  mainLayout->addLayout(rightLayout);

  panel->setLayout(mainLayout);

  QWidget* central = new QWidget(this);
  QVBoxLayout* verticalLayout = new QVBoxLayout(central);
  verticalLayout->addWidget(panel);
  verticalLayout->addStretch();
  setCentralWidget(central);

  connect(calcButton, &QPushButton::clicked,
          this, &MapWindow::onCalculateClicked);

  QString style = R"(
    QLineEdit {
      background-color: #FFF176;
      border: 1px solid #FBC02D;
      border-radius: 4px;
      padding: 6px;
      font-size: 14px;
      color: black;
    }

    QLineEdit:focus {
      border: 1px solid #F9A825;
      background-color: #FFEE58;
    }

    QLineEdit::placeholder {
      color: #555;
      font-style: italic;
    }

    QPushButton {
      background-color: #FBC02D;
      color: black;
      border-radius: 6px;
      padding: 6px 14px;
      font-weight: bold;
    }

    QPushButton:hover {
      background-color: #F9A825;
      color: black;
    }

    QPushButton:pressed {
      background-color: #F57F17;
      color: white;
    }
  )";
  panel->setStyleSheet(style);
}

void MapWindow::onCalculateClicked() {
  bool ok1, ok2, ok3, ok4;
  double startLat = startLatInput->text().toDouble(&ok1);
  double startLon = startLonInput->text().toDouble(&ok2);
  double endLat = endLatInput->text().toDouble(&ok3);
  double endLon = endLonInput->text().toDouble(&ok4);

  if (!(ok1 && ok2 && ok3 && ok4)) {
    QMessageBox::warning(this, "Error",
                         "Not correct values inserted!");
    return;
  }

  path = map->findShortestPathToDestination(
    startLat, startLon, endLat, endLon, "Driving");

  update();
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
  painter.fillRect(rect(), QColor(204, 255, 204));

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
      drawArrow(painter, p1, p2);
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
