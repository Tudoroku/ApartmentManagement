#ifndef LAB6_7_RAPORTGUI_H
#define LAB6_7_RAPORTGUI_H

#include "../service/serviceLocatar.h"
#include <QTableWidget>
#include <QWidget>

class RaportGUI : public QWidget
{
private:
    ServiceLocatar& service;
    QTableWidget* tabelRaport;

    void initGUI();
    void loadData();

public:
    explicit RaportGUI(ServiceLocatar& service, QWidget* parent = nullptr);
};

#endif
