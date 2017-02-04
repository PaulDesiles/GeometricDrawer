#include "form.h"
#include <QDebug>

QPolygonF polygon;
bool closed;
bool clockwise;

Form::Form(QVector<QPointF> _points, bool _closed) :
    QObject(0)
{
    polygon = QPolygonF(_points);
    closed = _closed;
    computeFormOrientation();
}

int Form::getSize() {
    return polygon.length();
}

bool Form::isClosed() {
    return closed;
}

QPointF Form::getPoint(int index) {
    return polygon[index];
}

QPolygonF Form::getPolygon() {
    return polygon;
}

void Form::setPoint(int index, const QPointF newValue) {
    if (index >= 0 && index < polygon.length())
        polygon.replace(index, newValue);
}

bool Form::isClockwiseOriented() {
    return clockwise;
}

bool Form::containsPoint(const QPointF point) {
    return closed && polygon.containsPoint(point, Qt::OddEvenFill);
}

void Form::computeFormOrientation() {
    //To know later which side of an edge is inside
    // > the right side of an edge (points[i], points[i+1]) will be inside if clockwise is true

    //Sum over the edges, (x2-x1)(y2+y1)
    //http://en.wikipedia.org/wiki/Shoelace_formula
    //http://stackoverflow.com/questions/1165647/how-to-determine-if-a-list-of-polygon-points-are-in-clockwise-order/1165943#1165943

    int i;
    qreal sum = 0;
    for (i=0; i< polygon.length()-1; i++) {
        sum += (polygon[i+1].x() - polygon[i].x()) * (polygon[i+1].y() + polygon[i].y());
    }
    sum += (polygon[0].x() - polygon[i].x()) * (polygon[0].y() + polygon[i].y());

    clockwise = sum <= 0;
}


//Wich side of the edge is inside ?

//http://en.wikipedia.org/wiki/Shoelace_formula
//http://stackoverflow.com/questions/1165647/how-to-determine-if-a-list-of-polygon-points-are-in-clockwise-order/1165943#1165943

//point[0] = (5,0)   edge[0]: (6-5)(4+0) =   4
//point[1] = (6,4)   edge[1]: (4-6)(5+4) = -18
//point[2] = (4,5)   edge[2]: (1-4)(5+5) = -30
//point[3] = (1,5)   edge[3]: (1-1)(0+5) =   0
//point[4] = (1,0)   edge[4]: (5-1)(0+0) =   0
//                                         ---
//                                         -44  counter-clockwise


//QRectF Form::getBounds() {
//    if (!boundsUpToDate) {
//        qreal xMin = points[0].x();
//        qreal yMin = points[0].y();
//        qreal xMax = points[0].x();
//        qreal yMax = points[0].y();

//        int i;
//        for (i=1; i < size; i++) {
//            QPoint p = points[i];

//            if (p.x() < xMin)
//                xMin = p.x();
//            else if (p.x() > xMax)
//                xMax = p.x();

//            if (p.y() < yMin)
//                yMin = p.y();
//            else if (p.y() > yMax)
//                yMax = p.y();
//        }
//        bounds = QRectF(xMin, yMin, xMax-xMin, yMax-yMin);
//        boundsUpToDate = true;
//    }
//    return bounds;
//}




//if (getBounds().contains(point.x(), point.y())) {
//            /// Ray Cast approch
//            /// code from http://alienryderflex.com/polygon/

//            int i, j;
//            j = size-1;
//            bool  oddNodes= false;
//            float ix, iy, jx,jy;
//            float x = point.x();
//            float y = point.y();

//            for (i=0; i < size-1; i++) {
//                ix = points[i].x();
//                iy = points[i].y();
//                jx = points[j].x();
//                jy = points[j].y();

//                if ((   (iy < y && jy >= y)
//                    ||  (jy < y && iy >= y))
//                        )
//                    &&  (points[i].x() <= point.x() || points[j].x() <= point.x()))
//                {
//                    float n = (ix + (y - iy) / (jy - iy) * (jx - ix) );
//                    if (n < x)
//                        oddNodes = !oddNodes;
//                }
//                j=i;
//            }
//            return oddNodes;
//        } else
//            return false;

