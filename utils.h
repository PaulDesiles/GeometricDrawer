#ifndef UTILS_H
#define UTILS_H

#include "qpoint.h"
#include <vector>

#define EPSYLON 0.1

namespace utils
{
    qreal GetSquaredDistance(QPointF p1, QPointF p2);

    QPointF GetIntersection(QPointF A1, QPointF A2, QPointF B1, QPointF B2);

    bool AreEquivalent(QPointF A, QPointF B);

    bool AreEquivalent(qreal A, qreal B);
}
#endif // UTILS_H
