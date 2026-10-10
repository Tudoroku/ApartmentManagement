#ifndef LAB6_7_LISTANOTIFICARECRUDGUI_H
#define LAB6_7_LISTANOTIFICARECRUDGUI_H

#include "../service/serviceLocatar.h"
#include "../utils/Observer.h"
#include "ListaNotificareListModel.h"
#include <QLineEdit>
#include <QListView>
#include <QWidget>
#include <functional>

class ListaNotificareCRUDGUI : public QWidget, public Observer
{
private:
    ServiceLocatar& service;
    QListView* view{};
    ListaNotificareListModel* model{};
    QLineEdit* txtApartament{};
    QLineEdit* txtNumarRandom{};
    QLineEdit* txtFisierExport{};

    void initGUI();
    void connectSignals();
    int readInt(QLineEdit* input, const QString& fieldName) const;
    void runAction(const std::function<void()>& action);

public:
    explicit ListaNotificareCRUDGUI(ServiceLocatar& service, QWidget* parent = nullptr);
    ~ListaNotificareCRUDGUI() override;

    void update() override;
};

#endif // LAB6_7_LISTANOTIFICARECRUDGUI_H
