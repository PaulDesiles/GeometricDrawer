#ifndef GUIDE_H
#define GUIDE_H

#include <QObject>
#include "qpoint.h"

#define SQUARED_SNAP_DISTANCE 400

class Guide : public QObject
{
    Q_OBJECT
public:
    Guide(QPointF a, QPointF b);
    QPointF getA();
    QPointF getB();
    void setA(QPointF newA);
    void setB(QPointF newB);
    QPointF getSnapped(QPointF p, bool onlyInsideTheSegment);
private:
    QPointF A;
    QPointF B;
};



#endif // GUIDE_H
