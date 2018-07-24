#ifndef MAINMODEL_H
#define MAINMODEL_H

#include "form.h"
#include "guide.h"

class MainModel
{
public:
    MainModel();
    QVector<Form*> forms();
    Form* getForm(int i);
    void addForm(Form* form);
    void deleteForm(int index);

    QVector<Guide*> guides();
    Guide* getGuide(int i);
    void addGuide(Guide* guide);

    QVector<QPointF> intersections();
    QPointF getIntersection(int i);
};

#endif // MAINMODEL_H
