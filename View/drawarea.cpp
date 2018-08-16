#include "drawarea.h"
#include <ctime>
#include <QDebug>
#include "math.h"
#include <QDebug>

QPointF tempEndPoint;
QVector<QPointF> tempForm;
bool overFirst = false;
clock_t lastClicReleaseTime;

QVector<Guide*> tempGuides;
QVector<Guide*> snappingGuides;

DrawArea::DrawArea(MainController* controller) :
    RenderArea(controller)
{
    srand(time(NULL));
    lastClicReleaseTime = clock();
}

void DrawArea::paint(QPainter* painter) {
    int i;

// draw guides and intersections while holding CTRL

//    if (getController()->IsControlPressed()) {
//        if (guides().size() >= 2) {
//            painter->setPen(QColor(0,100,200));
//            for (i=0; i<(int)guides().size(); i++) {
//                painter->drawLine(guides()[i]->getA(), guides()[i]->getB());
//            }
//        }

//        painter->save();
//        painter->setPen(QColor(0,0,0,0));
//        painter->setBrush(QColor(0,100,200));
//        painter->setOpacity(0.6);

//        for (i=0; i< (int)intersections().size(); i++) {
//            painter->drawEllipse(intersections()[i].x() - CONTROL_SIZE, intersections()[i].y() - CONTROL_SIZE, CONTROL_SIZE*2, CONTROL_SIZE*2);
//        }

//        painter->restore();
//    }


    painter->save();

    QColor color = Qt::red;
    color.setAlphaF(0.5);
    painter->setPen(QPen(QBrush(color), 1));

    for (i=0; i < (int)snappingGuides.size();i++) {
        Guide* guide = snappingGuides[i];
        painter->drawLine(guide->getA(), guide-> getB());
    }

    color = Qt::black;
    color.setAlphaF(0.3);
    painter->setPen(QPen(QBrush(color), 0));

    for (i=0; i < (int)guides().size();i++) {
        Guide* guide = guides()[i];
        if (!snappingGuides.contains(guide))
            painter->drawLine(guide->getA(), guide-> getB());
    }
    painter->restore();


    //pointer
    if (!tempEndPoint.isNull()) {
        if (!overFirst) {
            painter->save();
            painter->setPen(QColor(255,0,0));
            painter->setBrush(QColor(255,0,0,0));
            painter->drawEllipse(tempEndPoint, CONTROL_SIZE / 2, CONTROL_SIZE / 2);
            painter->restore();
        }
    }

    //futur line
    if (!tempEndPoint.isNull() && tempForm.size() >= 1) {
        painter->setPen(QPen(Qt::gray, 2));
        painter->drawLine(tempForm[tempForm.size() - 1], tempEndPoint);
    }

    //rest of the current form
    painter->setPen(QPen(Qt::black, 2));
    for (i=0; i<(tempForm.size() - 1); i++)
        painter->drawLine(tempForm[i], tempForm[i+1]);

    //endForm marker
    if (tempForm.size() > 2) {
        if (overFirst) {
            painter->save();
            painter->setPen(QColor(255,170,2));
            painter->setBrush(QColor(255,238,2));
            painter->setOpacity(0.8);
            painter->drawRect((int)tempForm[0].x() - CONTROL_SIZE, (int)tempForm[0].y() - CONTROL_SIZE, CONTROL_SIZE*2, CONTROL_SIZE*2);
            painter->restore();
        }
        painter->setPen(QPen(Qt::gray, 0));
        painter->drawRect((int)tempForm[0].x() - CONTROL_SIZE, (int)tempForm[0].y() - CONTROL_SIZE, CONTROL_SIZE*2, CONTROL_SIZE*2);
    }
}

void DrawArea::mousePress(QMouseEvent* mouseEvent){
    tempEndPoint = getSnappedPosition(mouseEvent->pos());
}

void DrawArea::mouseMove(QMouseEvent* mouseEvent){
    tempEndPoint = getSnappedPosition(mouseEvent->pos());
    if (tempForm.size() > 2)
    {
        qreal distance = utils::GetSquaredDistance(tempEndPoint, tempForm[0]);
        overFirst = distance < SQUARED_SELECT_SIZE;
    }
    else
        overFirst = false;
}

void DrawArea::mouseRelease(QMouseEvent* mouseEvent){
    //Avoid Double-Clic
    double elapsedTime = (double)(clock() - lastClicReleaseTime)/CLOCKS_PER_SEC;
    if (elapsedTime > 0.008f)
        handleMouseRelease(mouseEvent->pos());

    lastClicReleaseTime  = clock();
}

void DrawArea::handleMouseRelease(QPoint position)
{
    QPointF snapped = getSnappedPosition(position);
    if (tempForm.size() > 2)
    {
        qreal distance = utils::GetSquaredDistance(snapped, tempForm[0]);
        if (distance < SQUARED_SELECT_SIZE)
        {
            validateForm();
            return;
        }
    }

    tempEndPoint = snapped;
    tempForm.push_back(tempEndPoint);
}


void DrawArea::validateForm() {
    Form* newForm = new Form(tempForm, true);
    getController()->model()->addForm(newForm);

    //Add corresponding guides
    int i;
    for (i=0; i < tempForm.size() - 1; i++) {
        addTempParallelGuide(tempForm[i], tempForm[i+1], newForm->isClockwiseOriented());
    }
    addTempParallelGuide(tempForm[i], tempForm[0], newForm->isClockwiseOriented());

    finalizeGuides();

    tempForm.clear();
    getController()->setCurrentState(MainController::WAIT);
    update();
}

QPointF DrawArea::getSnappedPosition(QPointF cursorPosition) {
    snappingGuides.clear();
    int i;
    if (guides().size() >= 1) {
        QVector<QPointF> snappedPoints;
        QVector<qreal> snappedDistances;
        QPointF projected;

        for (i=0; i<(int)guides().size(); i++) {
            projected = guides()[i]->getSnapped(cursorPosition, true);
            snappedPoints.push_back(projected);
            snappedDistances.push_back(utils::GetSquaredDistance(cursorPosition, projected));
        }

        float minDistance = 100000;
        int minDistanceIndex = -1;

        for (i=0; i<(int)guides().size(); i++) {
            if (minDistance > snappedDistances[i])
            {
                minDistanceIndex = i;
                minDistance = snappedDistances[i];
            }
        }

        if (minDistanceIndex != -1 && minDistance < SQUARED_SNAP_DISTANCE)
        {
            snappingGuides.push_back(guides()[minDistanceIndex]);

            float secondMin = 100000;
            int secondMinIndex = -1;

            for (i=0; i<(int)guides().size(); i++) {
                if (i != minDistanceIndex && secondMin > snappedDistances[i])
                {
                    secondMinIndex = i;
                    secondMin = snappedDistances[i];
                }
            }

            if (secondMinIndex != -1 && secondMin < SQUARED_SNAP_DISTANCE)
            {
                QPointF intersection = utils::GetIntersection(guides()[minDistanceIndex]->getA(), guides()[minDistanceIndex]->getB(),
                                       guides()[secondMinIndex]->getA(), guides()[secondMinIndex]->getB());

                if (utils::GetSquaredDistance(cursorPosition, intersection) < SQUARED_SNAP_DISTANCE)
                {
                    snappingGuides.push_back(guides()[secondMinIndex]);
                    return intersection;
                }
            }

            return snappedPoints[minDistanceIndex];
        }
    }

    return cursorPosition;
}

//Add two temporary guides corresponding to the segment and its parallel at a defined distance outside the form
void DrawArea::addTempParallelGuide(QPointF A, QPointF B, bool clockwise) {
    qreal deltaX = B.x() - A.x();
    qreal deltaY = B.y() - A.y();
    qreal x = -1 *  deltaY / deltaX;
    qreal sizeFactor = FORMS_DISTANCE / sqrt(x*x + 1);

    if ((deltaX > 0 && clockwise) || (deltaX < 0 && !clockwise))
        sizeFactor = -sizeFactor;

    QPointF C = QPointF(sizeFactor * x + A.x(), sizeFactor + A.y());
    QPointF D = QPointF(sizeFactor * x + B.x(), sizeFactor + B.y());

    tempGuides.push_back(new Guide(A,B));
    tempGuides.push_back(new Guide(C,D));
}



// get final guides from temporary ones by computing their intersections with the drawing borders
void DrawArea::finalizeGuides() {
    QVector<QPointF> drawingCorners;
    QPointF side;
    QPointF newA;
    QPointF newB;

    drawingCorners.push_back(QPointF(0, 0));
    drawingCorners.push_back(QPointF(DRAWING_WIDTH, 0));
    drawingCorners.push_back(QPointF(DRAWING_WIDTH, DRAWING_HEIGHT));
    drawingCorners.push_back(QPointF(0, DRAWING_HEIGHT));

    int i;
    int j;
    for (i=0; i < tempGuides.size(); i++) {
        newA = QPointF(-1, -1);
        newB = QPointF(-1, -1);

        for (j=0; j < drawingCorners.size(); j++) {
            uint j2 = (j + 1) % drawingCorners.size();
            side = utils::GetIntersection(
                        drawingCorners[j],
                        drawingCorners[j2],
                        tempGuides[i]->getA(),
                        tempGuides[i]->getB());

            if (side.x() >= 0 && side.x() <= DRAWING_WIDTH && side.y() >=0 && side.y() <= DRAWING_HEIGHT) {
                if (newA.x() < 0) {
                    newA = side;
                }
                else {
                    newB = side;
                    break;
                }
            }
        }

        getController()->model()->addGuide(new Guide(newA, newB));
    }

    tempGuides.clear();
}
