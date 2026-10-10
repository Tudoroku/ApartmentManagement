#include "LocatariGUI.h"
#include "ListaNotificareCRUDGUI.h"
#include "ListaNotificareReadOnlyGUI.h"
#include "RaportGUI.h"
#include "style.h"

#include <QFormLayout>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <stdexcept>

LocatariGUI::LocatariGUI(ServiceLocatar& service, QWidget* parent) : QWidget(parent), service(service)
{
    initGUI();
    connectSignals();
    loadAll();
}
//cerinta de laborator: daca folosim list sa schimbam in table, daca avem doar table,
// sa schimbam pe o fereastra in list
void LocatariGUI::initGUI()
{
    setWindowTitle("Administrator Bloc");
    resize(1180, 720);
    setMinimumSize(1040, 640);
    setStyleSheet(appStyle());

    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(32, 28, 32, 28);
    root->setSpacing(22);

    auto* title = new QLabel("Administrator Bloc");
    title->setObjectName("Title");
    auto* subtitle = new QLabel("Gestiune locatari, raport pe tipuri, undo si lista de notificare");
    subtitle->setObjectName("Subtitle");

    auto* header = new QVBoxLayout;
    header->setSpacing(4);
    header->addWidget(title);
    header->addWidget(subtitle);

    auto* body = new QHBoxLayout;
    body->setSpacing(22);

    auto* tableCard = new QFrame;
    tableCard->setObjectName("Card");
    auto* tableLayout = new QVBoxLayout(tableCard);
    tableLayout->setContentsMargins(20, 20, 20, 20);
    tableLayout->setSpacing(14);

    auto* tableTitle = new QLabel("Locatari");
    tableTitle->setObjectName("SectionTitle");

    tabel = new QTableWidget;
    tabel->setColumnCount(4);
    tabel->setHorizontalHeaderLabels({"Apartament", "Proprietar", "Suprafata", "Tip"});
    tabel->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tabel->verticalHeader()->setVisible(false);
    tabel->setSelectionBehavior(QAbstractItemView::SelectRows);
    tabel->setSelectionMode(QAbstractItemView::SingleSelection);
    tabel->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tabel->setAlternatingRowColors(true);
    tabel->setFocusPolicy(Qt::NoFocus);

    auto* toolbar = new QGridLayout;
    toolbar->setSpacing(10);

    txtFilterTip = new QLineEdit;
    txtFilterTip->setPlaceholderText("filtru tip");
    txtFilterSuprafata = new QLineEdit;
    txtFilterSuprafata->setPlaceholderText("suprafata maxima");
    txtNrNotificari = new QLineEdit;
    txtNrNotificari->setPlaceholderText("nr. notificari");

    auto* btnReload = new QPushButton("Reload");
    auto* btnSortNume = new QPushButton("Sort nume");
    auto* btnSortSuprafata = new QPushButton("Sort suprafata");
    auto* btnSortTipSup = new QPushButton("Sort tip + suprafata");
    auto* btnFilterTip = new QPushButton("Filtreaza tip");
    auto* btnFilterSuprafata = new QPushButton("Filtreaza suprafata");
    auto* btnRaport = new QPushButton("Raport");
    auto* btnListaCRUD = new QPushButton("Lista notificare CRUD");
    auto* btnListaDesenata = new QPushButton("Lista notificare desenata");
    auto* btnAddNotificare = new QPushButton("Adauga in notificare");
    auto* btnGenNotificare = new QPushButton("Genereaza notificare");
    auto* btnClearNotificare = new QPushButton("Goleste notificare");

    for (auto* btn : {btnReload, btnSortNume, btnSortSuprafata, btnSortTipSup, btnFilterTip, btnFilterSuprafata, btnRaport, btnListaCRUD, btnListaDesenata, btnAddNotificare, btnGenNotificare, btnClearNotificare})
    {
        btn->setObjectName("SecondaryButton");
    }
   // btnRaport->setVisible(false);
   // btnLista->setVisible(false);
    toolbar->addWidget(btnReload, 0, 0);
    toolbar->addWidget(btnSortNume, 0, 1);
    toolbar->addWidget(btnSortSuprafata, 0, 2);
    toolbar->addWidget(btnSortTipSup, 0, 3);
    toolbar->addWidget(txtFilterTip, 1, 0);
    toolbar->addWidget(btnFilterTip, 1, 1);
    toolbar->addWidget(txtFilterSuprafata, 1, 2);
    toolbar->addWidget(btnFilterSuprafata, 1, 3);
    toolbar->addWidget(btnRaport, 2, 0, 1, 2);
    toolbar->addWidget(btnListaCRUD, 2, 2);
    toolbar->addWidget(btnListaDesenata, 2, 3);
    toolbar->addWidget(btnAddNotificare, 3, 0);
    toolbar->addWidget(txtNrNotificari, 3, 1);
    toolbar->addWidget(btnGenNotificare, 3, 2);
    toolbar->addWidget(btnClearNotificare, 3, 3);

    tableLayout->addWidget(tableTitle);
    tableLayout->addWidget(tabel, 1);
    tableLayout->addLayout(toolbar);

    auto* formCard = new QFrame;
    formCard->setObjectName("Card");
    formCard->setFixedWidth(360);
    auto* formRoot = new QVBoxLayout(formCard);
    formRoot->setContentsMargins(22, 22, 22, 22);
    formRoot->setSpacing(18);

    auto* formTitle = new QLabel("Actiuni");
    formTitle->setObjectName("SectionTitle");

    auto* form = new QFormLayout;
    form->setSpacing(12);

    txtApartament = new QLineEdit;
    txtApartament->setPlaceholderText("ex: 14");
    txtNume = new QLineEdit;
    txtNume->setPlaceholderText("ex: Pop Tudor");
    txtSuprafata = new QLineEdit;
    txtSuprafata->setPlaceholderText("ex: 56.5");
    txtTip = new QLineEdit;
    txtTip->setPlaceholderText("ex: 2 camere");

    form->addRow("Apartament", txtApartament);
    form->addRow("Proprietar", txtNume);
    form->addRow("Suprafata", txtSuprafata);
    form->addRow("Tip", txtTip);

    auto* actions = new QGridLayout;
    actions->setSpacing(10);

    auto* btnAdd = new QPushButton("Adauga");
    auto* btnUpdate = new QPushButton("Modifica nume");
    auto* btnRemove = new QPushButton("Sterge");
    btnRemove->setObjectName("DangerButton");
    auto* btnUndo = new QPushButton("Undo");
    btnUndo->setObjectName("SecondaryButton");

    actions->addWidget(btnAdd, 0, 0, 1, 2);
    actions->addWidget(btnUpdate, 1, 0, 1, 2);
    actions->addWidget(btnRemove, 2, 0);
    actions->addWidget(btnUndo, 2, 1);

    formRoot->addWidget(formTitle);
    formRoot->addLayout(form);
    formRoot->addStretch();
    formRoot->addLayout(actions);

    body->addWidget(tableCard, 1);
    body->addWidget(formCard);

    root->addLayout(header);
    root->addLayout(body, 1);

    btnAdd->setProperty("action", "add");
    btnUpdate->setProperty("action", "update");
    btnRemove->setProperty("action", "remove");
    btnUndo->setProperty("action", "undo");
    btnReload->setProperty("action", "reload");
    btnSortNume->setProperty("action", "sortNume");
    btnSortSuprafata->setProperty("action", "sortSuprafata");
    btnSortTipSup->setProperty("action", "sortTipSup");
    btnFilterTip->setProperty("action", "filterTip");
    btnFilterSuprafata->setProperty("action", "filterSuprafata");
    btnRaport->setProperty("action", "raport");
    btnListaCRUD->setProperty("action", "listaCrud");
    btnListaDesenata->setProperty("action", "listaDesenata");
    btnAddNotificare->setProperty("action", "addNotificare");
    btnGenNotificare->setProperty("action", "genNotificare");
    btnClearNotificare->setProperty("action", "clearNotificare");

}

void LocatariGUI::connectSignals()
{
    QObject::connect(tabel, &QTableWidget::itemSelectionChanged, this, [this] { fillFormFromSelectedRow(); });

    const auto buttons = findChildren<QPushButton*>();
    for (auto* button : buttons)
    {
        const QString action = button->property("action").toString();
        if (action == "add")
        {
            QObject::connect(button, &QPushButton::clicked, this, [this]
            {
                runAction([this]
                {
                    service.add(readApartament(), txtNume->text().trimmed().toStdString(), readSuprafata(txtSuprafata, "Suprafata"), txtTip->text().trimmed().toStdString());
                });
            });
        }
        else if (action == "update")
        {
            QObject::connect(button, &QPushButton::clicked, this, [this]
            {
                runAction([this]
                {
                    service.update(readApartament(), txtNume->text().trimmed().toStdString());
                });
            });
        }
        else if (action == "remove")
        {
            QObject::connect(button, &QPushButton::clicked, this, [this]
            {
                runAction([this]
                {
                    service.remove(readApartament());
                });
            });
        }
        else if (action == "undo")
        {
            QObject::connect(button, &QPushButton::clicked, this, [this]
            {
                runAction([this] { service.undo(); });
            });
        }
        else if (action == "reload")
        {
            QObject::connect(button, &QPushButton::clicked, this, [this] { loadAll(); });
        }
        else if (action == "sortNume")
        {
            QObject::connect(button, &QPushButton::clicked, this, [this] { loadTable(service.sortByNumeProprietar()); });
        }
        else if (action == "sortSuprafata")
        {
            QObject::connect(button, &QPushButton::clicked, this, [this] { loadTable(service.sortBySuprafata()); });
        }
        else if (action == "sortTipSup")
        {
            QObject::connect(button, &QPushButton::clicked, this, [this] { loadTable(service.sortByTipSiSuprafata()); });
        }
        else if (action == "filterTip")
        {
            QObject::connect(button, &QPushButton::clicked, this, [this]
            {
                loadTable(service.filterByTip(txtFilterTip->text().trimmed().toStdString()));
            });
        }
        else if (action == "filterSuprafata")
        {
            QObject::connect(button, &QPushButton::clicked, this, [this]
            {
                runAction([this]
                {
                    loadTable(service.filterBySuprafata(readSuprafata(txtFilterSuprafata, "Suprafata maxima")));
                }, false);
            });
        }
        else if (action == "raport")
        {
            QObject::connect(button, &QPushButton::clicked, this, [this]
            {
                auto* raport = new RaportGUI(service);
                raport->setAttribute(Qt::WA_DeleteOnClose);
                raport->show();
            });
        }
        else if (action == "listaCrud")
        {
            QObject::connect(button, &QPushButton::clicked, this, [this]
            {
                auto* lista = new ListaNotificareCRUDGUI(service);
                lista->setAttribute(Qt::WA_DeleteOnClose);
                lista->show();
            });
        }
        else if (action == "listaDesenata")
        {
            QObject::connect(button, &QPushButton::clicked, this, [this]
            {
                auto* lista = new ListaNotificareReadOnlyGUI(service);
                lista->setAttribute(Qt::WA_DeleteOnClose);
                lista->show();
            });
        }
        else if (action == "addNotificare")
        {
            QObject::connect(button, &QPushButton::clicked, this, [this]
            {
                runAction([this] { service.addApartamentInListaNotificare(readApartament()); }, false);
            });
        }
        else if (action == "genNotificare")
        {
            QObject::connect(button, &QPushButton::clicked, this, [this]
            {
                runAction([this] { service.genereazaListaNotificare(readInt(txtNrNotificari, "Numar notificari")); }, false);
            });
        }
        else if (action == "clearNotificare")
        {
            QObject::connect(button, &QPushButton::clicked, this, [this]
            {
                runAction([this] { service.clearListaNotificare(); }, false);
            });
        }
    }
}

void LocatariGUI::clearFields()
{
    txtApartament->clear();
    txtNume->clear();
    txtSuprafata->clear();
    txtTip->clear();
}

void LocatariGUI::loadTable(const std::vector<Locatar>& locatari)
{
    tabel->clearSelection();
    tabel->setRowCount(static_cast<int>(locatari.size()));

    for (int i = 0; i < static_cast<int>(locatari.size()); ++i)
    {
        const auto& l = locatari[i];
        tabel->setItem(i, 0, new QTableWidgetItem(QString::number(l.getApartament())));
        tabel->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(l.getNumeProprietar())));
        tabel->setItem(i, 2, new QTableWidgetItem(QString::number(l.getSuprafata(), 'f', 2)));
        tabel->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(l.getTipApartament())));
    }

    if (tabel->rowCount() > 0)
    {
        tabel->selectRow(0);
        fillFormFromSelectedRow();
    }
    else
    {
        clearFields();
    }
}

void LocatariGUI::loadAll()
{
    loadTable(service.sortByApartament());
}

void LocatariGUI::fillFormFromSelectedRow()
{
    const auto selected = tabel->selectedItems();
    if (selected.size() < 4)
    {
        return;
    }

    const int row = selected.first()->row();
    txtApartament->setText(tabel->item(row, 0)->text());
    txtNume->setText(tabel->item(row, 1)->text());
    txtSuprafata->setText(tabel->item(row, 2)->text());
    txtTip->setText(tabel->item(row, 3)->text());
}

int LocatariGUI::readApartament() const
{
    bool ok = false;
    const int value = txtApartament->text().trimmed().toInt(&ok);
    if (!ok)
    {
        throw std::runtime_error("Apartamentul trebuie sa fie numar intreg.");
    }
    return value;
}

int LocatariGUI::readInt(QLineEdit* input, const QString& fieldName) const
{
    bool ok = false;
    const int value = input->text().trimmed().toInt(&ok);
    if (!ok)
    {
        throw std::runtime_error((fieldName + " trebuie sa fie numar intreg.").toStdString());
    }
    return value;
}

float LocatariGUI::readSuprafata(QLineEdit* input, const QString& fieldName) const
{
    bool ok = false;
    const float value = input->text().trimmed().toFloat(&ok);
    if (!ok)
    {
        throw std::runtime_error((fieldName + " trebuie sa fie numar real.").toStdString());
    }
    return value;
}

void LocatariGUI::runAction(const std::function<void()>& action, bool refresh)
{
    try
    {
        action();
        if (refresh)
        {
            loadAll();
        }
    }
    catch (const std::exception& ex)
    {
        QMessageBox::warning(this, "Eroare", ex.what());
    }
}
