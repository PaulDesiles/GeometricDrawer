#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include "Model/mainmodel.h"

class MainController: public QObject
{
    Q_OBJECT

public:
    MainController(MainModel* _model);
    enum State { WAIT, DRAW, EDIT };
    void setState(State newState);
    MainModel* model();
    State selectedState();
    State currentState();
    void setSelectedState(State state);
    void setCurrentState(State state);
    void ControlPressed(bool pressed);
    bool IsControlPressed();
    void showMessage(QString message);

signals:
    void messageChanged(QString message);
};

#endif // MAINCONTROLLER_H
