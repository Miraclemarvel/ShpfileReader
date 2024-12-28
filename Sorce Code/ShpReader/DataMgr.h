#pragma once
#include <vector>
#include <fstream>
#include <QString>

//数据管理器
namespace Geometry{
	// 点要素
	struct Point {
		double x;
		double y;
	};

	// 线要素
	struct PolyLine {
		double box[4];  // 边界框
		int numParts;   // 线段数量
		int numPoints;  // 点的数量
		std::vector<int> parts;   // 每个线段的起始索引
		std::vector<Point> points; // 点坐标
		// 构造函数
		PolyLine(double* box, int numParts, int numPoints, std::vector<int>& parts, std::vector<Point>& points) :
			box{ box[0], box[1], box[2], box[3] },
			numParts(numParts),
			numPoints(numPoints),
			parts(parts),
			points(points)
		{}
	};

	// 面要素
	struct Polygon {
		double box[4];    // 边界框
		int numParts;     // 线段数量
		int numPoints;    // 点的数量
		std::vector<int> parts;   // 每个线段的起始索引
		std::vector<Point> points; // 点坐标
		// 构造函数
		Polygon(double* box, int numParts, int numPoints, std::vector<int>& parts, std::vector<Point>& points) :
			box{ box[0], box[1], box[2], box[3] },
			numParts(numParts),
			numPoints(numPoints),
			parts(parts),
			points(points)
		{}
	};
};

// 要素类型枚举
enum GeoType {
	POINT = 1,		// 点
	POLYLINE = 3,	// 线
	POLYGON = 5		// 面
};

using myTuple = std::tuple<GeoType, std::vector<double> ,std::vector<Geometry::Point>, std::vector<Geometry::PolyLine>, std::vector<Geometry::Polygon>>;
namespace DataTool {
	// 反转字节顺序，因为shp文件中的整数是大端格式
	int ReverseBytes(int value);
	// 读取点要素
	void ReadPoint(std::ifstream& shpFile, int contentLength, std::vector<Geometry::Point>& points);
	// 读取线要素
	void ReadPolyLine(std::ifstream& shpFile, int contentLength, std::vector<Geometry::PolyLine>& lines);
	// 读取面要素
	void ReadPolygon(std::ifstream& shpFile, int contentLength, std::vector<Geometry::Polygon>& polygons);

	//解析shp文件
	myTuple ParseShpFile(QString& filePath);
}