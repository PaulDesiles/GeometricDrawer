#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>
#include <vector>
#include <QMouseEvent>
#include "qpainter.h"
#include "Model/form.h"
#include "Controller/maincontroller.h"
#include "utils.h"

#define CONTROL_SIZE 4
#define SELECT_SIZE 8

#define DRAWING_WIDTH 800
#define DRAWING_HEIGHT 800

class RenderArea : public QWidget
{
    Q_OBJECT
public:
    explicit RenderArea(MainController* controller, QWidget *parent = 0);
    MainController* getController();

private:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent * mouseEvent);
    void mouseMoveEvent(QMouseEvent * mouseEvent);
    void mouseReleaseEvent(QMouseEvent * mouseEvent);

protected:
    std::vector<Form*> forms();
    std::vector<Guide*> guides();
    std::vector<QPointF> intersections();
    virtual void paint(QPainter* painter) = 0;
    virtual void mousePress(QMouseEvent* mouseEvent) = 0;
    virtual void mouseMove(QMouseEvent* mouseEvent) = 0;
    virtual void mouseRelease(QMouseEvent* mouseEvent) = 0;
    virtual void setColorForForm(int i, QPainter* painter);
};

#endif // RENDERAREA_H
