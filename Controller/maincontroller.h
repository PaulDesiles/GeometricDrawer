#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include "Model/mainmodel.h"

class MainController
{
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
};

#endif // MAINCONTROLLER_H
