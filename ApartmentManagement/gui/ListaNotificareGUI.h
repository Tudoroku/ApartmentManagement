#ifndef LAB6_7_LISTANOTIFICAREGUI_H
#define LAB6_7_LISTANOTIFICAREGUI_H

#include "../service/serviceLocatar.h"
#include <QLineEdit>
#include <QTableWidget>
#include <QWidget>
#include <QListWidget>

class ListaNotificareGUI : public QWidget
{
private:
    ServiceLocatar& service;
    QTableWidget* tabel;
    QListWidget* lista;
    QLineEdit* txtApartament;
    QLineEdit* txtNumarRandom;
    QLineEdit* txtFisierExport;

    void initGUI();
    void connectSignals();
    void loadData();
    int readInt(QLineEdit* input, const QString& fieldName) const;
    void runAction(const std::function<void()>& action);

public:
    explicit ListaNotificareGUI(ServiceLocatar& service, QWidget* parent = nullptr);
};

#endif
