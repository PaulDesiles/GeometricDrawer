#include "renderarea.h"
#include "Controller/maincontroller.h"
#include "Model/mainmodel.h"
#include <QDebug>

MainController* controller;

RenderArea::RenderArea(MainController* _controller, QWidget *parent) :
    QWidget(parent)
{
    controller = _controller;
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    setMouseTracking(true);
}

std::vector<Form*> RenderArea::forms() {
    return controller->model()->forms();
}

std::vector<Guide*> RenderArea::guides() {
    return controller->model()->guides();
}

std::vector<QPointF> RenderArea::intersections() {
    return controller->model()->intersections();
}

void RenderArea::paintEvent(QPaintEvent *) {
    int f;
    QPainter* painter = new QPainter(this);
    painter->setRenderHint(QPainter::Antialiasing, true);

    painter->setPen(QPen(Qt::black, 0));
    for (f=0; f < (int)forms().size();f++) {
        Form* form = forms()[f];
        if (form->getSize() > 1) {
            painter->save();
            setColorForForm(f, painter);
            painter->drawConvexPolygon(form->getPolygon());
            painter->restore();
        }
    }

    painter->save();

    QColor color = Qt::red;
    color.setAlphaF(0.5);
    painter->setPen(QPen(QBrush(color), 0));

    for (f=0; f < (int)guides().size();f++) {
        Guide* guide = guides()[f];
        painter->drawLine(guide->getA(), guide-> getB());
    }
    painter->restore();

    paint(painter);
}


void RenderArea::mousePressEvent(QMouseEvent * mouseEvent) {
    if (controller->currentState() == MainController::WAIT) {
        controller->setCurrentState(controller->selectedState());
    }
    mousePress(mouseEvent);
    update();
}

void RenderArea::mouseMoveEvent(QMouseEvent * mouseEvent) {
    mouseMove(mouseEvent);
    update();
}

void RenderArea::mouseReleaseEvent(QMouseEvent * mouseEvent) {
    mouseRelease(mouseEvent);
    update();
}

void RenderArea::setColorForForm(int i, QPainter* painter) {
    painter->setBrush(QBrush(Qt::black));
}

MainController* RenderArea::getController() {
    return controller;
}

//CURVES


//painter.setPen(QPen(Qt::black, 2));
//QPainterPath path;
//path.moveTo(startPoint);
//path.cubicTo(startPoint.x() - 100, startPoint.y(), endPoint.x() + 100, endPoint.y(), endPoint.x(), endPoint.y());
//painter.drawPath(path);

