#include "Controller/maincontroller.h"

MainModel* _model;

MainController::State _selectedState;
MainController::State _currentState;
bool controlPressed = false;

MainController::MainController(MainModel* m)
    : QObject(0)
{
    _model = m;
    _currentState = MainController::WAIT;
    _selectedState = MainController::WAIT;
}

MainModel* MainController::model() {
    return _model;
}

//USELESS ?
void MainController::setState(State newState) {
    _selectedState = newState;
    if (_currentState != MainController::WAIT)
        _currentState = newState;
}

MainController::State MainController::selectedState(){
    return _selectedState;
}

MainController::State MainController::currentState() {
    return _currentState;
}

void MainController::setSelectedState(MainController::State state){
    _selectedState = state;
}

void MainController::setCurrentState(MainController::State state) {
    _currentState = state;
}

void MainController::ControlPressed(bool pressed)
{
    controlPressed = pressed;
}

bool MainController::IsControlPressed()
{
    return controlPressed;
}

void MainController::showMessage(QString message)
{
    emit messageChanged(message);
}

