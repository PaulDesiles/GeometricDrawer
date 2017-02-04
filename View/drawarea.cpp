#include "drawarea.h"
#include <ctime>
#include <QDebug>
#include "math.h"
#include <QDebug>

QPointF tempEndPoint;
QVector<QPointF> tempForm;
bool overFirst = false;
clock_t lastClicReleaseTime;

std::vector<Guide*> tempGuides;

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

    if (!tempEndPoint.isNull()) {
        if (overFirst) {
            painter->save();
            painter->setPen(QColor(255,0,0));
            painter->setBrush(QColor(255,0,0,0));
            painter->drawEllipse(tempEndPoint, CONTROL_SIZE / 2, CONTROL_SIZE / 2);
            painter->restore();
        }
    }

    if (!tempEndPoint.isNull() && tempForm.size() >= 1) {
        painter->setPen(QPen(Qt::gray, 1));
        painter->drawLine(tempForm[tempForm.size() - 1], tempEndPoint);
    }

    painter->setPen(QPen(Qt::black, 2));
    for (i=0; i<(tempForm.size() - 1); i++)
        painter->drawLine(tempForm[i], tempForm[i+1]);

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
        overFirst = utils::AreNear(tempEndPoint, tempForm[0], SELECT_SIZE);
}

void DrawArea::mouseRelease(QMouseEvent* mouseEvent){
    //Avoid Double-Clic
    double elapsedTime = (double)(clock() - lastClicReleaseTime)/CLOCKS_PER_SEC;
    printf("%f", elapsedTime);
    if (elapsedTime > 0.008f) {
        if (tempForm.size() > 2 && utils::AreNear(mouseEvent->pos(), tempForm[0], SELECT_SIZE))
            validateForm();
        else {
            tempEndPoint = getSnappedPosition(mouseEvent->pos());
            tempForm.push_back(tempEndPoint);
        }
    }
    lastClicReleaseTime  = clock();
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


//QPointF DrawArea::getSnappedPosition(QPoint cursorPosition) {
//    return getSnappedPosition(QPointF(cursorPosition));
//}


QPointF DrawArea::getSnappedPosition(QPointF cursorPosition) {
    int i;
    if (guides().size() >=1) {
        for (i=0; i<(int)guides().size(); i++) {
            QPointF snappedPoint = guides()[i]->isPointSnapped(cursorPosition, true);
            if (snappedPoint.x() != -1)
                return snappedPoint;
        }
    }

    return cursorPosition;
}

//calcul le guides temporaire correpondant à un segment déplacés d'une longueur l vers l'exterieur de la forme
void DrawArea::addTempParallelGuide(QPointF A, QPointF B, bool clockwise) {
    int l = 10;
    qreal deltaX = B.x() - A.x();
    qreal deltaY = B.y() - A.y();
    qreal x = -1 *  deltaY / deltaX;
    qreal sizeFactor = l / sqrt(x*x + 1);

    if ((deltaX > 0 && clockwise) || (deltaX < 0 && !clockwise))
        sizeFactor = -sizeFactor;

    QPointF C = QPointF(sizeFactor * x + A.x(), sizeFactor + A.y());
    QPointF D = QPointF(sizeFactor * x + B.x(), sizeFactor + B.y());

    //qDebug() << "_dx " << deltaX <<  "\t_clkws " << clockwise;


    //getController()->model()->addGuide(new Guide(C,D));
    tempGuides.push_back(new Guide(C,D));
}


// réccupère les guides précédements stockés pour calculer leurs intersections et obtenir ainsi les guides finaux
void DrawArea::finalizeGuides() {
    int i;
    for (i=0; i < tempGuides.size() - 1; i++) {
        QPointF intersection = utils::GetIntersection(tempGuides[i]->getA(), tempGuides[i]->getB(), tempGuides[i+1]->getA(), tempGuides[i+1]->getB());
        tempGuides[i]->setB(intersection);
        tempGuides[i+1]->setA(intersection);
    }
    QPointF intersection = utils::GetIntersection(tempGuides[i]->getA(), tempGuides[i]->getB(), tempGuides[0]->getA(), tempGuides[0]->getB());
    tempGuides[i]->setB(intersection);
    tempGuides[0]->setA(intersection);

    for (i=0; i < (int)tempGuides.size(); i++)
        getController()->model()->addGuide(tempGuides[i]);

    tempGuides.clear();
}
