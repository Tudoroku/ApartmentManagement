#include "ListaNotificareListModel.h"

ListaNotificareListModel::ListaNotificareListModel(ServiceLocatar& service, QObject* parent)
    : QAbstractListModel(parent), service(service)
{
}

int ListaNotificareListModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
    {
        return 0;
    }
    return static_cast<int>(service.getListaNotificare().size());
}

QVariant ListaNotificareListModel::data(const QModelIndex& index, int role) const
{
    if (role != Qt::DisplayRole)
    {
        return {};
    }
    if (!index.isValid())
    {
        return {};
    }

    const auto& locatari = service.getListaNotificare();
    const auto row = index.row();
    if (row < 0 || row >= static_cast<int>(locatari.size()))
    {
        return {};
    }

    const auto& locatar = locatari[static_cast<std::size_t>(row)];
    return QString("Apartament %1 | %2 | %3 mp | %4")
        .arg(locatar.getApartament())
        .arg(QString::fromStdString(locatar.getNumeProprietar()))
        .arg(locatar.getSuprafata(), 0, 'f', 2)
        .arg(QString::fromStdString(locatar.getTipApartament()));
}

void ListaNotificareListModel::refresh()
{
    beginResetModel();
    endResetModel();
}
