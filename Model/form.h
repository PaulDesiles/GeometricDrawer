#ifndef FORM_H
#define FORM_H

#include <QObject>
#include "qpoint.h"
#include "qrect.h"
#include "qpolygon.h"

class Form : public QObject
{
    Q_OBJECT
public:
    Form(QVector<QPointF> _points, bool _isClosed);
    int getSize();
    bool isClosed();
    QPointF getPoint(int index);
    QPolygonF getPolygon();
    void setPoint(int index, const QPointF newValue);
    bool containsPoint(const QPointF point);
    void computeFormOrientation();
    bool isClockwiseOriented();

private:
    QPolygonF polygon;
    bool closed;
    bool clockwise;
};

#endif // FORM_H
