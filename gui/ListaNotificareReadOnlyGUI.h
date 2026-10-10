#ifndef LAB6_7_LISTANOTIFICAREREADONLYGUI_H
#define LAB6_7_LISTANOTIFICAREREADONLYGUI_H

#include "../service/serviceLocatar.h"
#include "../utils/Observer.h"
#include <QPaintEvent>
#include <QPoint>
#include <QResizeEvent>
#include <QWidget>
#include <vector>

class ListaNotificareReadOnlyGUI : public QWidget, public Observer
{
private:
    ServiceLocatar& service;
    std::vector<QPoint> pozitii;
    std::vector<int> dimensiuni;

    void regenereazaFiguri();

public:
    explicit ListaNotificareReadOnlyGUI(ServiceLocatar& service, QWidget* parent = nullptr);
    ~ListaNotificareReadOnlyGUI() override;

    void update() override;

protected:
    void paintEvent(QPaintEvent* ev) override;
    void resizeEvent(QResizeEvent* ev) override;
};

#endif // LAB6_7_LISTANOTIFICAREREADONLYGUI_H
