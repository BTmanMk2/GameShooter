#pragma once

#include"MySVG.h"
#include<qgraphicsitem.h>
#include<qpainter.h>

class MultiTriItem : public QGraphicsItem
{
public:
	void setTris(QVector<SVGData::SVGTriangle>& tts);
	QRectF boundingRect() const;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
private:
	QVector<SVGData::SVGTriangle> tris;
	QVector<SVGData::SVGFill> fills;
	QRectF boundingbox;
};