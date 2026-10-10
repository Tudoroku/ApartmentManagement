#include "ListaNotificareCRUDGUI.h"

#include "style.h"
#include <QAbstractItemView>
#include <QFormLayout>
#include <QFrame>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <stdexcept>

ListaNotificareCRUDGUI::ListaNotificareCRUDGUI(ServiceLocatar& service, QWidget* parent) : QWidget(parent), service(service)
{
    initGUI();
    connectSignals();
    service.addListaNotificareObserver(this);
    model->refresh();
}

ListaNotificareCRUDGUI::~ListaNotificareCRUDGUI()
{
    service.removeListaNotificareObserver(this);
}

void ListaNotificareCRUDGUI::initGUI()
{
    setWindowTitle("Lista notificare CRUD");
    resize(860, 560);
    setStyleSheet(appStyle());

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(28, 28, 28, 28);
    mainLayout->setSpacing(18);

    auto* title = new QLabel("Lista notificare CRUD");
    title->setObjectName("Title");
    auto* subtitle = new QLabel("Vizualizare si operatii pe lista de notificare");
    subtitle->setObjectName("Subtitle");

    view = new QListView;
    view->setAlternatingRowColors(true);
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->setFocusPolicy(Qt::NoFocus);

    model = new ListaNotificareListModel(service, this);
    view->setModel(model);

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

    btnAdd->setProperty("action", "add");
    btnGenerate->setProperty("action", "generate");
    btnClear->setProperty("action", "clear");
    btnExport->setProperty("action", "export");

    formLayout->addRow("Apartament", txtApartament);
    formLayout->addRow("Numar random", txtNumarRandom);
    formLayout->addRow("Fisier export", txtFisierExport);
    formLayout->addRow(btnAdd, btnGenerate);
    formLayout->addRow(btnExport, btnClear);

    mainLayout->addWidget(title);
    mainLayout->addWidget(subtitle);
    mainLayout->addWidget(view, 1);
    mainLayout->addWidget(card);
}

void ListaNotificareCRUDGUI::connectSignals()
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

int ListaNotificareCRUDGUI::readInt(QLineEdit* input, const QString& fieldName) const
{
    bool ok = false;
    const int value = input->text().trimmed().toInt(&ok);
    if (!ok)
    {
        throw std::runtime_error((fieldName + " trebuie sa fie numar intreg.").toStdString());
    }
    return value;
}

void ListaNotificareCRUDGUI::runAction(const std::function<void()>& action)
{
    try
    {
        action();
    }
    catch (const std::exception& ex)
    {
        QMessageBox::warning(this, "Eroare", ex.what());
    }
}

void ListaNotificareCRUDGUI::update()
{
    model->refresh();
}
