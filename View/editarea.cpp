#include "editarea.h"

int capturedPoint = -1;
int capturedForm = -1;

EditArea::EditArea(MainController* controller) :
    RenderArea(controller)
{
}

void EditArea::paint(QPainter* painter) {
    if (capturedForm != -1) {
        int i;
        Form* captured = forms()[capturedForm];
        if (capturedPoint != -1 && captured->getSize() > capturedPoint) {

            //Draw the selected corners
            painter->save();
            painter->setBrush(QBrush(Qt::yellow));
            painter->setOpacity(0.5);
            painter->drawEllipse(captured->getPoint(capturedPoint), CONTROL_SIZE, CONTROL_SIZE);
            painter->restore();
        }

        painter->setPen(QPen(Qt::gray, 0));
        for (i=0; i<captured->getSize(); i++)
            painter->drawEllipse(captured->getPoint(i), CONTROL_SIZE, CONTROL_SIZE);
    }
}

void EditArea::setColorForForm(int i, QPainter* painter) {
    if (i == capturedForm)
        painter->setBrush(QBrush(Qt::gray));
    else
        painter->setBrush(QBrush(Qt::black));
}

void EditArea::mousePress(QMouseEvent* mouseEvent){
    int i;
    for (i=0; i < (int)forms().size(); i++) {
        if (forms()[i]->containsPoint(mouseEvent->pos())) {
            if (capturedForm != i) {
                capturedForm = i;
                capturedPoint = -1;
            }
            break;
        }
    }

    if (capturedPoint == -1 && capturedForm != -1 && capturedForm < (int)forms().size()) {
        int i;
        for (i=0; i< forms()[capturedForm]->getSize(); i++) {
            qreal distance = utils::GetSquaredDistance(forms()[capturedForm]->getPoint(i), mouseEvent->pos());
            if (distance < SQUARED_SELECT_SIZE) {
                capturedPoint = i;
                break;
            }
        }
    }
}

void EditArea::mouseMove(QMouseEvent* mouseEvent){
    if (capturedPoint != -1 && capturedForm != -1 && capturedForm < (int)forms().size()) {
        forms()[capturedForm]->setPoint(capturedPoint, mouseEvent->pos());
        update();
    }
}

void EditArea::mouseRelease(QMouseEvent* mouseEvent){
    if (capturedPoint != -1 && capturedForm != -1 && capturedForm < (int)forms().size()) {
        forms()[capturedForm]->setPoint(capturedPoint, mouseEvent->pos());
        capturedPoint = -1;
    }
}

void EditArea::deleteSelected() {
    if (capturedForm != -1 && capturedForm < (int)forms().size()) {
        getController()->model()->deleteForm(capturedForm);
        capturedForm = -1;
        update();
    }
}
