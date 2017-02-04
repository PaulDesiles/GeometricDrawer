#ifndef EDITAREA_H
#define EDITAREA_H

#include "renderarea.h"
#include "Controller/maincontroller.h"

class EditArea : public RenderArea
{
public:
    EditArea(MainController* controller);
    void deleteSelected();

protected:    
    virtual void paint(QPainter* painter);
    virtual void mousePress(QMouseEvent* mouseEvent);
    virtual void mouseMove(QMouseEvent* mouseEvent);
    virtual void mouseRelease(QMouseEvent* mouseEvent);
    virtual void setColorForForm(int i, QPainter* painter);
};

#endif // EDITAREA_H
