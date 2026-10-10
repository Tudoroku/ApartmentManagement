#ifndef LAB6_7_LOCATARIGUI_H
#define LAB6_7_LOCATARIGUI_H

#include "../domain/locatar.h"
#include "../service/serviceLocatar.h"
#include <QLineEdit>
#include <QTableWidget>
#include <QWidget>
#include <functional>
#include <vector>

class LocatariGUI : public QWidget
{
private:
    ServiceLocatar& service;
    QTableWidget* tabel;
    QLineEdit* txtApartament;
    QLineEdit* txtNume;
    QLineEdit* txtSuprafata;
    QLineEdit* txtTip;
    QLineEdit* txtFilterTip;
    QLineEdit* txtFilterSuprafata;
    QLineEdit* txtNrNotificari;

    void initGUI();
    void connectSignals();
    void loadTable(const std::vector<Locatar>& locatari);
    void loadAll();
    void fillFormFromSelectedRow();
    int readApartament() const;
    int readInt(QLineEdit* input, const QString& fieldName) const;
    float readSuprafata(QLineEdit* input, const QString& fieldName) const;
    void runAction(const std::function<void()>& action, bool refresh = true);
    void clearFields();

public:
    explicit LocatariGUI(ServiceLocatar& service, QWidget* parent = nullptr);
};

#endif
