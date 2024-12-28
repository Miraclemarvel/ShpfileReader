#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ShpReader.h"
#include "DataMgr.h"

class ShpReader : public QMainWindow
{
    Q_OBJECT

public:
    ShpReader(QWidget *parent = nullptr);
    ~ShpReader();

public slots:
    void on_actionImport_triggered();       // 导入文件
    void on_actionAbout_triggered();        // 关于
    void resizeEvent(QResizeEvent *event);  // 重置窗口大小
    

private:
    Ui::ShpReaderClass ui;

    std::vector<double> mvBounds;               // 边界框
    std::vector<Geometry::Point> mvPoints;      // 点数据
    std::vector<Geometry::PolyLine> mvLines;    // 线数据
    std::vector<Geometry::Polygon> mvPolygons;  // 面数据

    // 显示数据
    int display();
    int drawPoint(double ratio);
    int drawLine(double ratio);
    int drawPolygon(double ratio);
};
