#include "ListaNotificareGUI.h"
#include "style.h"

#include <QFileDialog>
#include <QFormLayout>
#include <QFrame>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <functional>
#include <stdexcept>

ListaNotificareGUI::ListaNotificareGUI(ServiceLocatar& service, QWidget* parent) : QWidget(parent), service(service)
{
    initGUI();
    connectSignals();
    loadData();
}

void ListaNotificareGUI::initGUI()
{
    setWindowTitle("Lista notificare");
    resize(860, 560);
    setStyleSheet(appStyle());

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(28, 28, 28, 28);
    mainLayout->setSpacing(18);

    auto* title = new QLabel("Lista notificare");
    title->setObjectName("Title");
    auto* subtitle = new QLabel("Gestioneaza lista temporara si exportul CSV");
    subtitle->setObjectName("Subtitle");

    /*tabel = new QTableWidget;
    tabel->setColumnCount(4);
    tabel->setHorizontalHeaderLabels({"Apartament", "Proprietar", "Suprafata", "Tip"});
    tabel->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tabel->verticalHeader()->setVisible(false);
    tabel->setSelectionBehavior(QAbstractItemView::SelectRows);
    tabel->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tabel->setAlternatingRowColors(true);
    tabel->setFocusPolicy(Qt::NoFocus);*/
    lista = new QListWidget;
    lista->setSelectionMode(QAbstractItemView::SingleSelection);
    lista->setEditTriggers(QAbstractItemView::NoEditTriggers);
    lista->setAlternatingRowColors(true);
    lista->setFocusPolicy(Qt::NoFocus);

    auto* card = new QFrame;
    card->setObjectName("Card");
    auto* formLayout = new QFormLayout(card);
    formLayout->setContentsMargins(22, 22, 22, 22);
    formLayout->setSpacing(12);

    txtApartament = new QLineEdit;
    txtApartament->setPlaceholderText("ex: 12");
    txtNumarRandom = new QLineEdit;
    txtNumarRandom->setPlaceholderText("ex: 3");
    txtFisierExport = new QLineEdit("notificari.csv");

    auto* btnAdd = new QPushButton("Adauga apartament");
    auto* btnGenerate = new QPushButton("Genereaza random");
    auto* btnClear = new QPushButton("Goleste lista");
    btnClear->setObjectName("DangerButton");
    auto* btnExport = new QPushButton("Export CSV");
    btnExport->setObjectName("SecondaryButton");

    formLayout->addRow("Apartament", txtApartament);
    formLayout->addRow("Numar random", txtNumarRandom);
    formLayout->addRow("Fisier export", txtFisierExport);
    formLayout->addRow(btnAdd, btnGenerate);
    formLayout->addRow(btnExport, btnClear);

    btnAdd->setProperty("action", "add");
    btnGenerate->setProperty("action", "generate");
    btnClear->setProperty("action", "clear");
    btnExport->setProperty("action", "export");

    mainLayout->addWidget(title);
    mainLayout->addWidget(subtitle);
    //mainLayout->addWidget(tabel, 1);
    mainLayout->addWidget(lista, 1);
    mainLayout->addWidget(card);
}

void ListaNotificareGUI::connectSignals()
{
    const auto buttons = findChildren<QPushButton*>();
    for (auto* button : buttons)
    {
        const QString action = button->property("action").toString();
        if (action == "add")
        {
            QObject::connect(button, &QPushButton::clicked, this, [this]
            {
                runAction([this] { service.addApartamentInListaNotificare(readInt(txtApartament, "Apartament")); });
            });
        }
        else if (action == "generate")
        {
            QObject::connect(button, &QPushButton::clicked, this, [this]
            {
                runAction([this] { service.genereazaListaNotificare(readInt(txtNumarRandom, "Numar random")); });
            });
        }
        else if (action == "clear")
        {
            QObject::connect(button, &QPushButton::clicked, this, [this]
            {
                runAction([this] { service.clearListaNotificare(); });
            });
        }
        else if (action == "export")
        {
            QObject::connect(button, &QPushButton::clicked, this, [this]
            {
                runAction([this]
                {
                    const auto filename = txtFisierExport->text().trimmed();
                    if (filename.isEmpty())
                    {
                        throw std::runtime_error("Fisierul de export este obligatoriu.");
                    }
                    service.exportListaNotificareCSV(filename.toStdString());
                });
            });
        }
    }
}

/*void ListaNotificareGUI::loadData()
{
    const auto& locatari = service.getListaNotificare();
    tabel->setRowCount(static_cast<int>(locatari.size()));

    for (int i = 0; i < static_cast<int>(locatari.size()); ++i)
    {
        const auto& l = locatari[i];
        tabel->setItem(i, 0, new QTableWidgetItem(QString::number(l.getApartament())));
        tabel->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(l.getNumeProprietar())));
        tabel->setItem(i, 2, new QTableWidgetItem(QString::number(l.getSuprafata(), 'f', 2)));
        tabel->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(l.getTipApartament())));
    }
}*/

void ListaNotificareGUI::loadData()
{   //list ver
    lista->clear();

    const auto& locatari = service.getListaNotificare();
    for (const auto& l : locatari)
    {
        const QString itemText = QString("Apartament | %1 | %2 | %3 mp | %4").arg(l.getApartament()).arg(l.getNumeProprietar()).arg(l.getSuprafata()).arg(l.getTipApartament());
        lista->addItem(itemText);
    }
}

int ListaNotificareGUI::readInt(QLineEdit* input, const QString& fieldName) const
{
    bool ok = false;
    const int value = input->text().trimmed().toInt(&ok);
    if (!ok)
    {
        throw std::runtime_error((fieldName + " trebuie sa fie numar intreg.").toStdString());
    }
    return value;
}

void ListaNotificareGUI::runAction(const std::function<void()>& action)
{
    try
    {
        action();
        loadData();
    }
    catch (const std::exception& ex)
    {
        QMessageBox::warning(this, "Eroare", ex.what());
    }
}
