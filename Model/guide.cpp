#include "guide.h"
#include "utils.h"
#include <QDebug>

QPointF A;
QPointF B;

Guide::Guide (QPointF _a, QPointF _b) :
    QObject(0)
{    
    A = QPointF(_a.x(), _a.y());
    B = QPointF(_b.x(), _b.y());
}

QPointF Guide::getA() {
    return A;
}

QPointF Guide::getB() {
    return B;
}

void Guide::setA(QPointF newA) {
    A = QPointF(newA.x(), newA.y());
}

void Guide::setB(QPointF newB) {
    B = QPointF(newB.x(), newB.y());
}

QPointF Guide::getSnapped(QPointF p, bool onlyInsideTheSegment) {
    //return the projected point of p on the Guide if p is near the Guide
    //otherwise return (-1,-1)
    // source : http://stackoverflow.com/questions/849211/shortest-distance-between-a-point-and-a-line-segment

    qreal x = A.x()-B.x();
    qreal y = A.y()-B.y();
    qreal l2 = x * x + y * y;  // i.e. |a-b|^2 -  avoid a sqrt
    qreal t = p.dotProduct(p - A, B - A) / l2;

    if (!onlyInsideTheSegment || (t >= 0 && t <= 1))
        return A + t * (B - A);  // Projection falls on the line

    return QPointF(-1,-1);
}







//autres pistes :
//https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line

// source : http://stackoverflow.com/questions/849211/shortest-distance-between-a-point-and-a-line-segment
