#include "mainmodel.h"
#include <QDebug>
#include "utils.h"

QVector<Form*> _forms;
QVector<Guide*> _guides;
QVector<QPointF> _intersections;

MainModel::MainModel()
{
}

QVector<Form*> MainModel::forms() {
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


QVector<Guide*> MainModel::guides()
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
    qreal eps = 0.1;
    bool containsGuide = false;
    for (i=0; i < (int)_guides.size(); i++) {
        QPointF gA = guide->getA();
        QPointF gB = guide->getB();
        QPointF iA = getGuide(i)->getA();
        QPointF iB = getGuide(i)->getB();

        if ((utils::AreEquivalent(gA, iA) && utils::AreEquivalent(gB, iB)) ||
            (utils::AreEquivalent(gA, iB) && utils::AreEquivalent(gB, iA)))
        {
            containsGuide = true;
            break;
        }

        QPointF intersection = utils::GetIntersection(gA, gB, iA, iB);

        int j = 0;
        bool containsIntersection = false;
        for (j=0;j < (int)_intersections.size();j++) {
            if (utils::AreEquivalent(intersection, getIntersection(j)))
            {
                containsIntersection = true;
                break;
            }
        }

        if (!containsIntersection)
            _intersections.push_back(intersection);
    }

    if (!containsGuide)
        _guides.push_back(guide);
}

QVector<QPointF> MainModel::intersections()
{
    return _intersections;
}

QPointF MainModel::getIntersection(int i)
{
    return _intersections[i];
}
