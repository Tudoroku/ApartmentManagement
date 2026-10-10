#ifndef LAB6_7_LISTANOTIFICARELISTMODEL_H
#define LAB6_7_LISTANOTIFICARELISTMODEL_H

#include "../service/serviceLocatar.h"
#include <QAbstractListModel>

class ListaNotificareListModel : public QAbstractListModel
{
private:
    ServiceLocatar& service;

public:
    explicit ListaNotificareListModel(ServiceLocatar& service, QObject* parent = nullptr);

    [[nodiscard]] int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    void refresh();
};

#endif // LAB6_7_LISTANOTIFICARELISTMODEL_H
