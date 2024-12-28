#pragma once
#include <vector>
#include <fstream>
#include <QString>

//���ݹ�����
namespace Geometry{
	// ��Ҫ��
	struct Point {
		double x;
		double y;
	};

	// ��Ҫ��
	struct PolyLine {
		double box[4];  // �߽��
		int numParts;   // �߶�����
		int numPoints;  // �������
		std::vector<int> parts;   // ÿ���߶ε���ʼ����
		std::vector<Point> points; // ������
		// ���캯��
		PolyLine(double* box, int numParts, int numPoints, std::vector<int>& parts, std::vector<Point>& points) :
			box{ box[0], box[1], box[2], box[3] },
			numParts(numParts),
			numPoints(numPoints),
			parts(parts),
			points(points)
		{}
	};

	// ��Ҫ��
	struct Polygon {
		double box[4];    // �߽��
		int numParts;     // �߶�����
		int numPoints;    // �������
		std::vector<int> parts;   // ÿ���߶ε���ʼ����
		std::vector<Point> points; // ������
		// ���캯��
		Polygon(double* box, int numParts, int numPoints, std::vector<int>& parts, std::vector<Point>& points) :
			box{ box[0], box[1], box[2], box[3] },
			numParts(numParts),
			numPoints(numPoints),
			parts(parts),
			points(points)
		{}
	};
};

// Ҫ������ö��
enum GeoType {
	POINT = 1,		// ��
	POLYLINE = 3,	// ��
	POLYGON = 5		// ��
};

using myTuple = std::tuple<GeoType, std::vector<double> ,std::vector<Geometry::Point>, std::vector<Geometry::PolyLine>, std::vector<Geometry::Polygon>>;
namespace DataTool {
	// ��ת�ֽ�˳����Ϊshp�ļ��е������Ǵ�˸�ʽ
	int ReverseBytes(int value);
	// ��ȡ��Ҫ��
	void ReadPoint(std::ifstream& shpFile, int contentLength, std::vector<Geometry::Point>& points);
	// ��ȡ��Ҫ��
	void ReadPolyLine(std::ifstream& shpFile, int contentLength, std::vector<Geometry::PolyLine>& lines);
	// ��ȡ��Ҫ��
	void ReadPolygon(std::ifstream& shpFile, int contentLength, std::vector<Geometry::Polygon>& polygons);

	//����shp�ļ�
	myTuple ParseShpFile(QString& filePath);
}