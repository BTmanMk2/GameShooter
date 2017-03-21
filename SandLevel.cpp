#include "SandLevel.h"
#include "Game.h"

SandLevel::SandLevel(const char * filename, int x, int y, int heightMax,
	GameProtocol layer, QGraphicsItem * parent)
	:/*svg(filename, tris, fills, 0.648),*/ QGraphicsItem(parent),
	bounding(0, 0, 512, heightMax)
{
	// set the bounding box
	bounding.setHeight(heightMax - y);

	setPos(x, y);
	setZValue(layer);
	setFlag(QGraphicsItem::ItemIsFocusable);
	setFocus();
	colorRepo.readColors();
	initSand();
	riseUp(600);
	bounding.setHeight(heightMax);
	//startTimer(3000);
}

void SandLevel::riseUp(qreal dis)
{
	riseDis -= dis;
	qDebug("Now: %f %f", riseDis, supportDis);
	while (riseDis > supportDis)
	{
		generateSandLine();
	}
	QTransform trans;
	trans.translate(0, dis);
	setTransform(trans, true);
	bounding.setHeight(bounding.height() - dis);

	unsigned int line_amount = lines.size();
	for (unsigned int i = 1; i < line_amount; i++)
	{
		computeSandLine(i - 1, i);
	}
	computeSandTurb();
	update();
}

void SandLevel::initSand()
{
	// init first line
	generateSandLine();
	qDebug("First Line Success");
	generateSandLine();
	qDebug("Second Line Success");
	generateSandLine();
	qDebug("Third Line Success");
	generateSandLine();
	qDebug("4th Line Success");
	supportDis = 0;
}

void SandLevel::generateSandLine()
{
	supportDis += stepY - turbY;
	// init a line
	int lineSize = myIntRand(maxC - minC) + minC;
	int base_step = maxX / (lineSize - 1);
	int cur_x = 0, cur_y = currentY;
	QVector<MyPoint> aLine;
	aLine.resize(lineSize);
	for (unsigned int i = 0; i < lineSize - 1; i++)
	{
		aLine[i].point.setX(cur_x);
		aLine[i].point.setY(cur_y);
		if (i > 0) aLine[i].turb.setX(mySignedRand(turbX));
		else aLine[i].turb.setX(0);
		aLine[i].turb.setY(mySignedRand(turbY));
		cur_x += base_step + myIntRand(turbX);
		cur_x = cur_x > maxX ? maxX : cur_x;
	}
	aLine[lineSize - 1].point.setX(maxX);
	aLine[lineSize - 1].point.setY(cur_y);
	aLine[lineSize - 1].turb.setX(0);
	aLine[lineSize - 1].turb.setY(mySignedRand(turbY));
	currentY += stepY;
	lines.push_back(aLine);

	if (lines.size() > 1)
	{
		computeSandLine(lines.size() - 2, lines.size() - 1);
	}
}

void SandLevel::computeSandLine(int up, int down)
{
	QVector<MyPoint>& below = lines[down];
	QVector<MyPoint>& above = lines[up];
	unsigned int below_size = below.size();
	unsigned int above_size = above.size() - 1;
	unsigned int below_loc = 0, above_loc = 0;
	for (; below_loc < below_size - 1; below_loc++)
	{
		int accer = myIntRand(maxConTri);
		accer = accer + above_loc > above_size ? above_size - above_loc : accer;
		above_loc += accer;
		below[below_loc].flags = accer;
	}
	// do the final one
	below[below_size - 1].flags = above_size - above_loc;
}

void SandLevel::computeSandTurb()
{
	unsigned int line_amount = lines.size();
	for (unsigned int i = 0; i < line_amount; i++)
	{
		unsigned int p_amount = lines[i].size() - 1;
		lines[i][0].turb.setY(mySignedRand(turbY));
		for (unsigned int j = 1; j < p_amount; j++)
		{
			lines[i][j].turb.setX(mySignedRand(turbX));
			lines[i][j].turb.setY(mySignedRand(turbY));
		}
		lines[i][p_amount].turb.setY(mySignedRand(turbY));
	}
}

QRectF SandLevel::boundingRect() const
{
	return bounding;
}

void SandLevel::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	painter->setPen(QPen(QColor(255, 255, 255, 0), 0));
	painter->setRenderHint(QPainter::Antialiasing, true);
	unsigned int lineAmount = lines.size();
	for (unsigned int i = 1; i < lineAmount; i++)
	{
		QVector<MyPoint>& below = lines[i];
		QVector<MyPoint>& above = lines[i - 1];
		unsigned int above_loc = 0, above_size = above.size();
		unsigned int below_loc = 0, below_size = below.size();
		for (; below_loc < below_size - 1; below_loc++)
		{
			int flag = below[below_loc].flags;
			while (flag > 0)
			{
				QPolygonF qp;
				qp.push_back(above[above_loc].getReal());
				qp.push_back(above[above_loc + 1].getReal());
				qp.push_back(below[below_loc].getReal());
				flag--;
				above_loc++;
				painter->setBrush(QColor(colorRepo.getRandomColor()));
				painter->drawPolygon(qp);
			}
			QPolygonF qp;
			qp.push_back(below[below_loc].getReal());
			qp.push_back(below[below_loc + 1].getReal());
			qp.push_back(above[above_loc].getReal());
			painter->setBrush(QColor(colorRepo.getRandomColor()));
			painter->drawPolygon(qp);
		}
		int flag = lines[i][below_loc].flags;
		while (flag > 0)
		{
			QPolygonF qp;
			qp.push_back(above[above_loc].getReal());
			qp.push_back(above[above_loc + 1].getReal());
			qp.push_back(below[below_loc].getReal());
			flag--;
			above_loc++;
			painter->setBrush(QColor(colorRepo.getRandomColor()));
			painter->drawPolygon(qp);
		}
	}
}

void SandLevel::timerEvent(QTimerEvent * timer)
{
	//update();
}

void SandLevel::keyReleaseEvent(QKeyEvent * event)
{
	riseUp(-5);
}

void ColorRepo::readColors()
{
	if (colors.size() > 0) return;
	QFile clrfile(DES_COLOR_FILE);
	if (!clrfile.open(QIODevice::ReadOnly))
	{
		return;
	}
	QTextStream qts(&clrfile);
	QString tmp;
	while (qts.readLineInto(&tmp))
	{
		colors.push_back(tmp);
	}
}

int ColorRepo::getSize()
{
	return colors.size();
}

QString ColorRepo::getRandomColor()
{
	if (colors.size() == 0) return "";
	int loc = qrand() % colors.size();
	return colors[loc];
}