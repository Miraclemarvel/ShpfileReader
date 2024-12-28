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
    void on_actionImport_triggered();       // �����ļ�
    void on_actionAbout_triggered();        // ����
    void resizeEvent(QResizeEvent *event);  // ���ô��ڴ�С
    

private:
    Ui::ShpReaderClass ui;

    std::vector<double> mvBounds;               // �߽��
    std::vector<Geometry::Point> mvPoints;      // ������
    std::vector<Geometry::PolyLine> mvLines;    // ������
    std::vector<Geometry::Polygon> mvPolygons;  // ������

    // ��ʾ����
    int display();
    int drawPoint(double ratio);
    int drawLine(double ratio);
    int drawPolygon(double ratio);
};
