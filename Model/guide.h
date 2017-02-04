#ifndef GUIDE_H
#define GUIDE_H

#include <QObject>
#include "qpoint.h"

#define SNAP_DISTANCE 20

class Guide : public QObject
{
    Q_OBJECT
public:
    Guide(QPointF a, QPointF b);
    QPointF getA();
    QPointF getB();
    void setA(QPointF newA);
    void setB(QPointF newB);
    QPointF isPointSnapped(QPointF p, bool onlyInsideTheSegment);
private:
    QPointF A;
    QPointF B;
};



#endif // GUIDE_H
