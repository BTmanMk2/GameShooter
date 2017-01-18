#pragma once
#include<QtXml\qdom.h>
#include<qfile.h>
#include<qvector.h>
#include<qmap.h>
#include<sstream>
#include<qpolygon.h>
#include<qbrush.h>

class SVGData
{
public:
	/*
	* Error Code
	* 
	* Description
	*	This part define the error code will be used here, including initial error
	* and other we could encounter. Use SVGData::CONTENT to confirm the status of 
	* the working spare data.
	*
	* Content Def.
	*	OK						status all right
	*	INIT_FAILURE			fail in the constructor
	**/
	enum Error
	{
		OK = 0x0, INIT_FAILURE
	};

	struct SVGTriangle
	{
		// QPoint x1, x2, x3;
		QPolygonF triangle;

		// filler index
		int fillIndex = -1;
		double opacity = 1;

		// turbulence
		QVector<QPair<qreal, bool>> turbTar;
		QVector<QPair<qreal, bool>> turbArc;
	};

	struct SVGFill
	{
		QBrush brush;
		SVGFill(QBrush& q)
			:brush(q){}
		SVGFill() {}
	};


	SVGData(const char* filename, QVector<SVGTriangle>& tr, QVector<SVGFill>& fill, double turb = 5);
	void test();
	QVector<SVGTriangle>& getTris() { return tris; }

	double maxX, maxY, minX, minY;
private:
	// status attribute
	int error;
	double turb;
	bool firstIn = true;

	// data processing func.
	int processData();
	int getTriangle(QDomElement&);
	int getFillLineGradient(QDomElement&);

	// data attributes
	QDomDocument raw;
	QVector<SVGTriangle>& tris;
	QVector<SVGFill>& fills;
	QMap<std::string, int> refs;
	std::stringstream ss;

	// 
	unsigned int triscounter = 0;
	unsigned int findcounter = 0;
};