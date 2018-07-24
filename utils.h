#ifndef UTILS_H
#define UTILS_H

#include "qpoint.h"
#include <vector>

namespace utils
{
    qreal GetSquaredDistance(QPointF p1, QPointF p2);

    QPointF GetIntersection(QPointF A1, QPointF A2, QPointF B1, QPointF B2);
}
#endif // UTILS_H
