#include "WaterLevel.h"

WaterLevel::WaterLevel(const char * filename, int x, int y, int heightMax,
	GameProtocol layer, QGraphicsItem* parent)
	:svg(filename, tris, fills), QGraphicsItem(parent), 
	bounding(2, -50, svg.maxX - svg.minX, svg.maxY - svg.minY + 256)
{
	// set the bounding box
	bounding.setHeight(heightMax - y);
	qDebug("Item size: %f", svg.maxX);
	setPos(x, y);
	setZValue(layer);
	riseUp(600);
	//startTimer(20);
	//qsrand(time(NULL));
}

void WaterLevel::riseUp(qreal dis)
{
	riseDis += dis;
	QTransform trans;
	trans.translate(0, dis);
	setTransform(trans, true);
	bounding.setHeight(bounding.height() - dis);
}

QRectF WaterLevel::boundingRect() const
{
	return bounding;
}

void WaterLevel::objDrawer(QPainter * painter)
{
	int amount = tris.size();
	for (unsigned int i = 0; i < amount; i++)
	{
		painter->setRenderHint(QPainter::Antialiasing, true);
		painter->setPen(QPen(QColor(255, 255, 255, 0), 0));
		painter->setOpacity(tris[i].opacity);
		if (tris[i].fillIndex >= 0) 
		{
			painter->setBrush(fills[tris[i].fillIndex].brush);
		}
		QPolygonF qp = tris[i].triangle;
		unsigned int v_amount = qp.size();
		for (unsigned int j = 0; j < v_amount; j++)
		{
			if (tris[i].acute[j]) continue;
			if (tris[i].turbArc[j].second = true)
				qp[j].setY(qp[j].y() + tris[i].turbArc[j].first);
			else
				qp[j].setY(qp[j].y() - tris[i].turbArc[j].first);
		}
		painter->drawPolygon(qp);
		//painter->drawRect(QRect(-500, -500, 1500, 1500));
	}
	//painter->setBrush(Qt::NoBrush);
	//painter->setPen(QPen(QColor("#A2A2A2"), 2));
	//painter->drawRect(bounding);
}

void WaterLevel::shuffle1()
{
	int amount = tris.size();
	for (unsigned int i = 0; i < amount; i++)
	{
		unsigned int v_amount = tris[i].triangle.size();
		for (unsigned int j = 0; j < v_amount; j++)
		{
			if(tris[i].acute[j]) continue;
			if (tris[i].turbTar[j].second &&
				tris[i].turbArc[j].first <= 0)
			{
				// turb perform done, reset all
				tris[i].turbTar[j].second = false;
				double turbx = myRand() * turb;
				tris[i].turbTar[j].first = turbx;
				tris[i].turbArc[j].first = 0;
				if (myRand() > 0.5) tris[i].turbArc[j].second = true;
				else tris[i].turbArc[j].second = false;
			}
			else if (tris[i].turbTar[j].second)
			{
				// on the way back
				tris[i].turbArc[j].first -= step;
			}
			else if (tris[i].turbArc[j].first >= tris[i].turbTar[j].first)
			{
				// ready to turn back
				tris[i].turbTar[j].second = true;
			}
			else
			{
				// just on the way to the peak
				tris[i].turbArc[j].first += step;
			}
		}
	}
}

void WaterLevel::shuffle2()
{
}

void WaterLevel::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	painter->setRenderHint(QPainter::Antialiasing, true);
	painter->setPen(QPen(QColor(255, 255, 255, 0), 0));
	objDrawer(painter);
	//// 反走样

	//// 设置渐变色
	//QLinearGradient linear(QPointF(80, 80), QPointF(150, 150));
	//linear.setColorAt(0, QColr);
	//linear.setColorAt(1, Qt::white);

	//// 设置显示模式
	//linear.setSpread(QGradient::PadSpread);

	//// 设置画笔颜色、宽度

	//// 设置画刷填充
	//painter->setBrush(linear);

	//// 绘制椭圆
	//painter->drawRect(QRect(40, 40, 180, 180));
}

void WaterLevel::timerEvent(QTimerEvent * timer)
{
	
}

void WaterLevel::Update() {
	shuffle1();
	update();
}

void WaterLevel::keyReleaseEvent(QKeyEvent * event)
{

}

void WaterLevel::addRgb(char r, char g, char b)
{
	unsigned int i = fills.size();
	for (unsigned int j = 0; j < i; j++)
	{
		
	}
}

void WaterLevel::saveRgb()
{
	QFile file("color.txt");
	file.open(QIODevice::ReadWrite);
	file.write("");
	file.close();
}
