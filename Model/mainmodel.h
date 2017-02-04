#ifndef MAINMODEL_H
#define MAINMODEL_H

#include "form.h"
#include "guide.h"

class MainModel
{
public:
    MainModel();
    std::vector<Form*> forms();
    Form* getForm(int i);
    void addForm(Form* form);
    void deleteForm(int index);

    std::vector<Guide*> guides();
    Guide* getGuide(int i);
    void addGuide(Guide* guide);

    std::vector<QPointF> intersections();
    QPointF getIntersection(int i);
};

#endif // MAINMODEL_H
