#include "utils.h"

namespace utils {

    bool AreNear(QPointF p1, QPointF p2, int threshold)
    {
        qreal x = (p1.x() - p2.x());
        qreal y = (p1.y() - p2.y());

        return (x*x + y*y) < (threshold * threshold);
    }

    QPointF GetIntersection(QPointF A1, QPointF A2, QPointF B1, QPointF B2)
    {
        qreal x1 = A1.x();
        qreal x2 = A2.x();
        qreal x3 = B1.x();
        qreal x4 = B2.x();

        qreal y1 = A1.y();
        qreal y2 = A2.y();
        qreal y3 = B1.y();
        qreal y4 = B2.y();

        qreal u = ((y4-y3)*(x1-x3)-(x4-x3)*(y1-y3)) / ((x4-x3)*(y2-y1)-(y4-y3)*(x2-x1));

        QPointF res =  QPointF(x1 + (x2-x1) * u, y1 + (y2-y1) * u);

        return res;
    }
}
