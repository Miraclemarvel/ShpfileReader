#include "DataMgr.h"
#include <QDebug>

// ��ת�ֽ�˳��
int DataTool::ReverseBytes(int value) {
	return ((value & 0x000000FF) << 24) |
		   ((value & 0x0000FF00) << 8) |
		   ((value & 0x00FF0000) >> 8) |
		   ((value & 0xFF000000) >> 24);
}

// ��ȡ��Ҫ��
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

// ��ȡ��Ҫ��
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

// ��ȡ��Ҫ��
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

//����shp�ļ�
myTuple DataTool::ParseShpFile(QString& filePath) {
	auto path = filePath.toStdString();

	// ��shp�ļ�
	std::ifstream shpFile(path, std::ios::binary);
	if (!shpFile.is_open()) {
		qWarning() << "Failed to open the shp file." << endl;
		return myTuple();
	}

	// ��ȡshp�ļ�ͷ
	int fileCode;
	shpFile.read(reinterpret_cast<char*>(&fileCode), sizeof(int));
	fileCode = ReverseBytes(fileCode);
	if (fileCode != 9994) {
		qWarning() << "Invalid file code." << endl;
		return myTuple();
	}

	// ������������5������
	int skip[5];
	shpFile.read(reinterpret_cast<char*>(skip), sizeof(int) * 5);

	// ��ȡ�ļ����ȺͰ汾��Ϣ
	int fileLength, version;
	shpFile.read(reinterpret_cast<char*>(&fileLength), sizeof(int));
	fileLength = ReverseBytes(fileLength);
	shpFile.read(reinterpret_cast<char*>(&version), sizeof(int));
	//version = ReverseBytes(version);

	// ��ȡ��״����
	int shapeType;
	shpFile.read(reinterpret_cast<char*>(&shapeType), sizeof(int));
	auto shpType = static_cast<GeoType>(shapeType);

	// ��ȡ�߽��
	double bounds[4];
	shpFile.read(reinterpret_cast<char*>(bounds), sizeof(double) * 4);

	// ��ȡz��m�ķ�Χ�������Ҫ��
	double zRange[2], mRange[2];
	shpFile.read(reinterpret_cast<char*>(zRange), sizeof(double) * 2);
	shpFile.read(reinterpret_cast<char*>(mRange), sizeof(double) * 2);

	// �����ȡ����Ϣ
	qDebug() << "File Code: " << fileCode << endl;
	qDebug() << "File Length: " << fileLength << endl;
	qDebug() << "Version: " << version << endl;
	qDebug() << "Shape Type: " << (shpType == GeoType::POINT ? "Point" : (shpType == GeoType::POLYLINE ? "PolyLine" : "Polygon")) << endl;
	qDebug() << "Bounds: " << bounds[0] << ", " << bounds[1] << ", " << bounds[2] << ", " << bounds[3] << endl;

	std::vector<Geometry::Point> points;
	std::vector<Geometry::PolyLine> lines;
	std::vector<Geometry::Polygon> polygons;

	// ��ȡÿ����״�ļ�������
	while (!shpFile.eof()) {
		int recordNumber, contentLength;
		shpFile.read(reinterpret_cast<char*>(&recordNumber), sizeof(int));
		shpFile.read(reinterpret_cast<char*>(&contentLength), sizeof(int));
		recordNumber = ReverseBytes(recordNumber);
		contentLength = ReverseBytes(contentLength);
		//std::cout << "Record Number: " << recordNumber << ", Content Length: " << contentLength << std::endl;
		// ����shapeType��ȡ����ļ�������
		// ������Ҫ����shapeType��ֵ��������ζ�ȡ�ͽ�����������
		// ���磬���ڵ����ͣ�shapeType = 1������Ҫ��ȡx��y����
		// ����shapeType��ȡ����ļ�������
		switch (shpType) {
		case GeoType::POINT: // ��
			ReadPoint(shpFile, contentLength, points);
			break;
		case GeoType::POLYLINE: // ��
			ReadPolyLine(shpFile, contentLength, lines);
			break;
		case GeoType::POLYGON: // ��
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