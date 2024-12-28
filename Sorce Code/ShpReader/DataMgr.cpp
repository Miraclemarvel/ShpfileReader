#include "DataMgr.h"
#include <QDebug>

// 反转字节顺序
int DataTool::ReverseBytes(int value) {
	return ((value & 0x000000FF) << 24) |
		   ((value & 0x0000FF00) << 8) |
		   ((value & 0x00FF0000) >> 8) |
		   ((value & 0xFF000000) >> 24);
}

// 读取点要素
void DataTool::ReadPoint(std::ifstream& shpFile, int contentLength, std::vector<Geometry::Point>& points) {
	if (shpFile.eof()) {
		return;
	}
	int shapeType;
	double x, y;
	shpFile.read(reinterpret_cast<char*>(&shapeType), sizeof(int));
	shapeType = ReverseBytes(shapeType);
	shpFile.read(reinterpret_cast<char*>(&x), sizeof(double));
	shpFile.read(reinterpret_cast<char*>(&y), sizeof(double));
	points.push_back({ x, y });
}

// 读取线要素
void DataTool::ReadPolyLine(std::ifstream& shpFile, int contentLength, std::vector<Geometry::PolyLine>& lines) {
	if (shpFile.eof()) {
		return;
	}
	int shapeType, numParts, numPoints;
	double box[4];
	std::vector<int> parts;
	std::vector<Geometry::Point> points;

	shpFile.read(reinterpret_cast<char*>(&shapeType), sizeof(int));
	for (int i = 0; i < 4; i++) {
		shpFile.read(reinterpret_cast<char*>(&box[i]), sizeof(double));
	}
	shpFile.read(reinterpret_cast<char*>(&numParts), sizeof(int));
	shpFile.read(reinterpret_cast<char*>(&numPoints), sizeof(int));

	parts.resize(numParts);
	for (int i = 0; i < numParts; i++) {
		shpFile.read(reinterpret_cast<char*>(&parts[i]), sizeof(int));
	}

	points.resize(numPoints);
	for (int i = 0; i < numPoints; i++) {
		shpFile.read(reinterpret_cast<char*>(&points[i].x), sizeof(double));
		shpFile.read(reinterpret_cast<char*>(&points[i].y), sizeof(double));
	}
	//qDebug() << "PolyLine with " << numParts << " parts and " << numPoints << " points:" << endl;
	lines.emplace_back(Geometry::PolyLine{ box, numParts, numPoints, parts, points });
}

// 读取面要素
void DataTool::ReadPolygon(std::ifstream& shpFile, int contentLength, std::vector<Geometry::Polygon>& polygons) {
	if (shpFile.eof()) {
		return;
	}
	int shapeType, numParts, numPoints;
	double box[4];
	std::vector<int> parts;
	std::vector<Geometry::Point> points;

	shpFile.read(reinterpret_cast<char*>(&shapeType), sizeof(int));
	for (int i = 0; i < 4; i++) {
		shpFile.read(reinterpret_cast<char*>(&box[i]), sizeof(double));
	}
	shpFile.read(reinterpret_cast<char*>(&numParts), sizeof(int));
	shpFile.read(reinterpret_cast<char*>(&numPoints), sizeof(int));

	parts.resize(numParts);
	for (int i = 0; i < numParts; i++) {
		shpFile.read(reinterpret_cast<char*>(&parts[i]), sizeof(int));
	}

	points.resize(numPoints);
	for (int i = 0; i < numPoints; i++) {
		shpFile.read(reinterpret_cast<char*>(&points[i].x), sizeof(double));
		shpFile.read(reinterpret_cast<char*>(&points[i].y), sizeof(double));
	}

	//qDebug() << "Polygon with " << numParts << " parts and " << numPoints << " points:" << endl;
	polygons.emplace_back(Geometry::Polygon{ box, numParts, numPoints, parts, points });
}

//解析shp文件
myTuple DataTool::ParseShpFile(QString& filePath) {
	auto path = filePath.toStdString();

	// 打开shp文件
	std::ifstream shpFile(path, std::ios::binary);
	if (!shpFile.is_open()) {
		qWarning() << "Failed to open the shp file." << endl;
		return myTuple();
	}

	// 读取shp文件头
	int fileCode;
	shpFile.read(reinterpret_cast<char*>(&fileCode), sizeof(int));
	fileCode = ReverseBytes(fileCode);
	if (fileCode != 9994) {
		qWarning() << "Invalid file code." << endl;
		return myTuple();
	}

	// 跳过接下来的5个整数
	int skip[5];
	shpFile.read(reinterpret_cast<char*>(skip), sizeof(int) * 5);

	// 读取文件长度和版本信息
	int fileLength, version;
	shpFile.read(reinterpret_cast<char*>(&fileLength), sizeof(int));
	fileLength = ReverseBytes(fileLength);
	shpFile.read(reinterpret_cast<char*>(&version), sizeof(int));
	//version = ReverseBytes(version);

	// 读取形状类型
	int shapeType;
	shpFile.read(reinterpret_cast<char*>(&shapeType), sizeof(int));
	auto shpType = static_cast<GeoType>(shapeType);

	// 读取边界框
	double bounds[4];
	shpFile.read(reinterpret_cast<char*>(bounds), sizeof(double) * 4);

	// 读取z和m的范围（如果需要）
	double zRange[2], mRange[2];
	shpFile.read(reinterpret_cast<char*>(zRange), sizeof(double) * 2);
	shpFile.read(reinterpret_cast<char*>(mRange), sizeof(double) * 2);

	// 输出读取的信息
	qDebug() << "File Code: " << fileCode << endl;
	qDebug() << "File Length: " << fileLength << endl;
	qDebug() << "Version: " << version << endl;
	qDebug() << "Shape Type: " << (shpType == GeoType::POINT ? "Point" : (shpType == GeoType::POLYLINE ? "PolyLine" : "Polygon")) << endl;
	qDebug() << "Bounds: " << bounds[0] << ", " << bounds[1] << ", " << bounds[2] << ", " << bounds[3] << endl;

	std::vector<Geometry::Point> points;
	std::vector<Geometry::PolyLine> lines;
	std::vector<Geometry::Polygon> polygons;

	// 读取每个形状的几何数据
	while (!shpFile.eof()) {
		int recordNumber, contentLength;
		shpFile.read(reinterpret_cast<char*>(&recordNumber), sizeof(int));
		shpFile.read(reinterpret_cast<char*>(&contentLength), sizeof(int));
		recordNumber = ReverseBytes(recordNumber);
		contentLength = ReverseBytes(contentLength);
		//std::cout << "Record Number: " << recordNumber << ", Content Length: " << contentLength << std::endl;
		// 根据shapeType读取具体的几何数据
		// 这里需要根据shapeType的值来决定如何读取和解析几何数据
		// 例如，对于点类型（shapeType = 1），需要读取x和y坐标
		// 根据shapeType读取具体的几何数据
		switch (shpType) {
		case GeoType::POINT: // 点
			ReadPoint(shpFile, contentLength, points);
			break;
		case GeoType::POLYLINE: // 线
			ReadPolyLine(shpFile, contentLength, lines);
			break;
		case GeoType::POLYGON: // 面
			ReadPolygon(shpFile, contentLength, polygons);
			break;
		default:
			qDebug() << "Unsupported shape type: " << shapeType << endl;
			break;
		}
	}
	shpFile.close();
	return std::make_tuple(shpType, std::vector<double>{bounds[0], bounds[1], bounds[2], bounds[3]}, points, lines, polygons);
}