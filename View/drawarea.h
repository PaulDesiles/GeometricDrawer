#ifndef DRAWAREA_H
#define DRAWAREA_H

#include "renderarea.h"
#include "Controller/maincontroller.h"

#define FORMS_DISTANCE 10

class DrawArea : public RenderArea
{
public:
    DrawArea(MainController* controller);
    void validateForm();

protected:
    virtual void paint(QPainter* painter);
    virtual void mousePress(QMouseEvent* mouseEvent);
    virtual void mouseMove(QMouseEvent* mouseEvent);
    virtual void mouseRelease(QMouseEvent* mouseEvent);

private:
    QPointF getSnappedPosition(QPointF cursorPosition);
    void addTempParallelGuide(QPointF A, QPointF B, bool clockwise);
    void finalizeGuides();
    QPointF getIntersection(QPointF A1, QPointF A2, QPointF B1, QPointF B2);
    void handleMouseRelease(QPoint position);
};

#endif // DRAWAREA_H
