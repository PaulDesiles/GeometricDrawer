#include "mainmodel.h"
#include <QDebug>
#include "utils.h"

std::vector<Form*> _forms;
std::vector<Guide*> _guides;
std::vector<QPointF> _intersections;

MainModel::MainModel()
{
}

std::vector<Form*> MainModel::forms() {
    return _forms;
}

Form* MainModel::getForm(int i){
    return _forms[i];
}

void MainModel::addForm(Form *form) {
    _forms.push_back(form);
}

void MainModel::deleteForm(int index) {
    if( index >= 0 && index < (int)_forms.size())
        _forms.erase(_forms.begin() + index);
}


std::vector<Guide*> MainModel::guides()
{
    return _guides;
}

Guide* MainModel::getGuide(int i)
{
    return _guides[i];
}

void MainModel::addGuide(Guide *guide)
{
    int i = 0;
    for (i=0; i < (int)_guides.size(); i++) {
        QPointF intersection = utils::GetIntersection(guide->getA(), guide->getB(), getGuide(i)->getA(), getGuide(i)->getB());

        int j = 0;
        bool alreadyContains = false;
        for (j=0;j < (int)_intersections.size();j++) {
            if (intersection.x() == getIntersection(j).x() &&
                intersection.y() == getIntersection(j).y())
            {
                alreadyContains = true;
                break;
            }
        }

        if (!alreadyContains)
            _intersections.push_back(intersection);
    }

    _guides.push_back(guide);
}

std::vector<QPointF> MainModel::intersections()
{
    return _intersections;
}

QPointF MainModel::getIntersection(int i)
{
    return _intersections[i];
}
