#include "fragment.h"
#include <QGradient>

double randoma() {
	int a = 10, b = 250;

	int c = qrand() % (b - a + 1) + a;
	qDebug("rand:%f\n", (double)c/100.0);
	return (double)c / 100.0;
}

void fragment::objDrawer(QPainter* painter)
{
	for (int i = 0; i < stones.size(); i++) {
		for (int j = 0; j < stones[i].tris.size(); j++) {
			painter->setOpacity(stones[i].tris[j].opacity);
			stones[i].tris[j].opacity *= 0.7; //-= (1.0 ./(double) maxStep);

			painter->setBrush(stones[i].fills[j].brush);
			painter->drawPolygon(stones[i].tris[j].triangle);
		}

			
		if (stones[i].tris.size() > 0)
			qDebug() << "Opacity: " << stones[i].tris[0].opacity;
	}
	

	//update frags
	for (int i = 0; i < stones.size(); i++) {
		if (stones[i].stepcnt >= 0) {
			for (int j = 0; j < stones[i].tris.size(); j++) {
				QPointF randdir = stones[i].directions[j];
				stones[i].directions[j] *= 0.8;
				for (int k = 0; k < stones[i].tris[j].triangle.size(); k++) {
					stones[i].tris[j].triangle[k] += randdir;
				}				
				
			}
			stones[i].stepcnt--;
		}
		else {
			stones[i].stepcnt--;
			stones[i].tris.clear();
			stones[i].fills.clear();
		}
	}
	
	return;
}

/*void fragment::shuffle1()
{
	//painter->drawRect(QRect(-500, -500, 1500, 1500));
	return;
}*/

QRectF fragment::boundingRect() const
{
	return bounding;
}

void fragment::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->setRenderHint(QPainter::Antialiasing, true);
	painter->setPen(QPen(QColor(255, 255, 255, 0), 1));
	painter->setBrush(QBrush(QColor(125, 125, 125, 120)));
	objDrawer(painter);
}

void fragment::timerEvent(QTimerEvent* timer)
{
	//shuffle1();
	update();
}

void fragment::keyReleaseEvent(QKeyEvent* event)
{

}

fragment::fragment(int type, QPointF pos, QGraphicsItem* parent):bounding(2, 0, 200, 256)
{
	int a = 0;
	stoneType = type;
	birthpos = pos;
	bounding.setHeight(1024);
	bounding.setWidth(512);

	// load fragment SVGs
	QVector<SVGData::SVGTriangle> temptris;
	QVector<SVGData::SVGFill> tempfills;
	for (int i = 0; i < stoneTypes ; i++) {
		temptris.clear();
		tempfills.clear();

		std::stringstream ss;
		if (i < 9) {
			ss << '0';
		}
		ss << (i + 1);

		//std::string stoneName = STONE_PREFIX + ss.str() + STONE_SUFFIX;
		std::string stoneSVGName = STONE_PREFIX + ss.str() + STONE_SVG;

		SVGData tempMySVG(stoneSVGName.c_str(), temptris, tempfills);

		stoneTris.push_back(temptris);
		stoneFills.push_back(tempfills);
	}

	setZValue(LAYER_FRAG);
	setPos(pos);
	startTimer(0);
}

void fragment::addFrag(int type, QPointF pos)
{
	QVector<SVGData::SVGTriangle>temptris = stoneTris[type];
	QVector<SVGData::SVGFill>tempfills = stoneFills[type];

	//rescale&reset position
	QPointF cntr(0, 0);
	for (int i = 0; i < temptris.size(); i++) {
		cntr += temptris[i].triangle[0];
		cntr += temptris[i].triangle[1];
		cntr += temptris[i].triangle[2];
		
	}
	cntr /= (3 * temptris.size());
	QPointF off = pos - cntr;
	std::vector<QPointF>tempdir;
	//set scale
	for (int i = 0; i < temptris.size(); i++) {
		QPointF fragvec(0, 0);
		for (int j = 0; j < temptris[i].triangle.size(); j++) {
			QPointF offvec = cntr - temptris[i].triangle[j];
			offvec *= scalep;
			temptris[i].triangle[j] += offvec;

			//move to pos
			temptris[i].triangle[j] += off;
			fragvec += temptris[i].triangle[j];
		}
		//set direction and speed
		fragvec /= temptris[i].triangle.size();
		fragvec = fragvec - pos;
		tempdir.push_back((fragvec / maxStep)*randoma()*4);
	}

	fragStone tempstone;
	tempstone.tris = temptris;
	tempstone.fills = tempfills;
	tempstone.center = pos;
	tempstone.directions = tempdir;
	tempstone.stepcnt = maxStep;

	int i = 0;
	for (; i < stones.size(); i++) {
		if (stones[i].stepcnt < 0) {
			stones[i] = tempstone;
		}
	}
	if (i == stones.size()) {
		stones.push_back(tempstone);
	}


}

fragment::~fragment() {

	return ;
}


