#include "RaportGUI.h"
#include "style.h"

#include <QHeaderView>
#include <QLabel>
#include <QVBoxLayout>

RaportGUI::RaportGUI(ServiceLocatar& service, QWidget* parent) : QWidget(parent), service(service)
{
    initGUI();
    loadData();
}

void RaportGUI::initGUI()
{
    setWindowTitle("Raport pe tipuri");
    resize(520, 420);
    setStyleSheet(appStyle());

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(28, 28, 28, 28);
    mainLayout->setSpacing(18);

    auto* title = new QLabel("Raport tip apartament");
    title->setObjectName("Title");

    auto* subtitle = new QLabel("Frecventa fiecarui tip de apartament din repository");
    subtitle->setObjectName("Subtitle");

    tabelRaport = new QTableWidget;
    tabelRaport->setColumnCount(2);
    tabelRaport->setHorizontalHeaderLabels({"Tip apartament", "Numar"});
    tabelRaport->horizontalHeader()->setStretchLastSection(true);
    tabelRaport->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tabelRaport->verticalHeader()->setVisible(false);
    tabelRaport->setSelectionBehavior(QAbstractItemView::SelectRows);
    tabelRaport->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tabelRaport->setAlternatingRowColors(true);
    tabelRaport->setFocusPolicy(Qt::NoFocus);

    mainLayout->addWidget(title);
    mainLayout->addWidget(subtitle);
    mainLayout->addWidget(tabelRaport, 1);
}

void RaportGUI::loadData()
{
    const auto raport = service.raportTipApartament();
    tabelRaport->setRowCount(static_cast<int>(raport.size()));

    for (int i = 0; i < static_cast<int>(raport.size()); ++i)
    {
        tabelRaport->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(raport[i].tipApartament)));
        tabelRaport->setItem(i, 1, new QTableWidgetItem(QString::number(raport[i].numarAparitii)));
    }
}
