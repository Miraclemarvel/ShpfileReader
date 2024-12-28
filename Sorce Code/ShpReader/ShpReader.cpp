#include "ShpReader.h"
#include <QDebug>
#include <QFileDialog>
#include <QGraphicsEllipseItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsPathItem>
#include <QMessageBox>

ShpReader::ShpReader(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	ui.graphicsView->setScene(new QGraphicsScene(this));
	ui.graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ui.graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

}

ShpReader::~ShpReader()
{}

void ShpReader::on_actionImport_triggered() {
	// 获取shp文件路径
    QString filePath = QFileDialog::getOpenFileName(this, QStringLiteral("打开shpfile"), "./data", "Shapefiles (*.shp)");
    if (filePath.isEmpty()) return;
	// 解析shp文件
    auto ret = DataTool::ParseShpFile(filePath);
    auto type = std::get<0>(ret);
    mvBounds = std::get<1>(ret);
    if (type == GeoType::POINT) {
        mvPoints = std::get<2>(ret);
        qDebug() << "point count: " << mvPoints.size();
    }
    else if (type == GeoType::POLYLINE) {
        mvLines = std::get<3>(ret);
        qDebug() << "line count: " << mvLines.size();
    }
    else if (type == GeoType::POLYGON) {
        mvPolygons = std::get<4>(ret);
        qDebug() << "polygon count: " << mvPolygons.size();
    }
	display();
    qDebug() << "bounds: " << mvBounds[0] << " " << mvBounds[1] << " " << mvBounds[2] << " " <<  mvBounds[3] << " " << endl;
}

int ShpReader::display() {
	if (mvPoints.size() == 0 && mvLines.size() == 0 && mvPolygons.size() == 0 || mvBounds.empty()) return -1;
	// 计算缩放比例
	double minX = mvBounds[0], minY = mvBounds[1];
	double maxX = mvBounds[2], maxY = mvBounds[3];

	auto w = ui.graphicsView->width() - 50;
	auto h = ui.graphicsView->height() - 50;
	double m = maxY - minY, n = maxX - minX;
	auto ratio = std::min(h / m, w / n);
	auto sx = 25 + (w - n * ratio) / 2;
	auto sy = 25 + (h - m * ratio) / 2;
	ui.graphicsView->scene()->clear();
	ui.graphicsView->setSceneRect(minX * ratio - sx, minY * ratio - sy, w + 50, h + 50);    //设置显示范围

	// 绘制点
	if (mvPoints.size() > 0) {
		drawPoint(ratio);
	}
	// 绘制线
	if (mvLines.size() > 0) {
		drawLine(ratio);
	}
	// 绘制面
	if (mvPolygons.size() > 0) {
		drawPolygon(ratio);
	}
	return 0;
}

int ShpReader::drawPoint(double ratio) {
	for (auto& p : mvPoints) {
		auto x = p.x * ratio;
		auto y = p.y * ratio;
		auto item = new QGraphicsEllipseItem(x, y, 4, 4);
		item->setBrush(QBrush(Qt::red));
		ui.graphicsView->scene()->addItem(item);
	}
	return 0;
}
int ShpReader::drawLine(double ratio) {
	for (auto& line : mvLines) {
		QPainterPath path;
		for (int i = 0; i < line.points.size(); i++) {
			if (i == 0) {
				path.moveTo(line.points[i].x * ratio, line.points[i].y * ratio);
			}
			else {
				path.lineTo(line.points[i].x * ratio, line.points[i].y * ratio);
			}
		}
		auto item = new QGraphicsPathItem(path);
		item->setPen(QPen(Qt::blue, 2));
		ui.graphicsView->scene()->addItem(item);
	}
	return 0;
}
int ShpReader::drawPolygon(double ratio) {
	for (auto& polygon : mvPolygons) {
		int n = polygon.parts.size();
		for (int i = 0; i < n; i++) {
			int start = polygon.parts[i];
			int end = (i == n - 1) ? polygon.points.size() : polygon.parts[i + 1];
			QPolygonF poly;
			for (int j = start; j < end; j++) {
				poly.append(QPointF(polygon.points[j].x * ratio, polygon.points[j].y * ratio));
			}
			auto item = new QGraphicsPolygonItem(poly);
			item->setPen(QPen(Qt::black, 2));
			item->setBrush(QBrush(Qt::cyan));
			item->setOpacity(0.5);
			ui.graphicsView->scene()->addItem(item);
		}
	}
	return 0;
}

void ShpReader::resizeEvent(QResizeEvent* event) {
	display();
}

void ShpReader::on_actionAbout_triggered() {
    QMessageBox::about(this, QStringLiteral("关于"), QStringLiteral("ShpReader v1.0\nAuthor: SK\nEmail: sk@elysium.org.cn\n目前仅支持简单点线面的读取"));
}