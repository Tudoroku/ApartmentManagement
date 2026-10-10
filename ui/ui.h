//
// Created by tudor on 4/1/2026.
//

#ifndef LAB6_7_UI_H
#define LAB6_7_UI_H

#include "../service/serviceLocatar.h"
#include <string>
#include <vector>

class UI
{
private:
    ServiceLocatar& service;

    /*
     * Afiseaza meniul principal al aplicatiei.
     */
    void printMenu() const;

    /*
     * Afiseaza o colectie de locatari.
     */
    void printLocatari(const std::vector<Locatar>& locatari) const;

    /*
     * Citeste un numar intreg de la tastatura.
     *
     * return:
     *  true daca citirea a reusit
     *  false daca input-ul nu este valid
     */
    bool readInt(const std::string& mesaj, int& valoare) const;

    /*
     * Citeste un numar real de la tastatura.
     *
     * return:
     *  true daca citirea a reusit
     *  false daca input-ul nu este valid
     */
    bool readFloat(const std::string& mesaj, float& valoare) const;

    /*
     * Citeste datele pentru adaugare si apeleaza service-ul.
     */
    void uiAdd();

    /*
     * Citeste apartamentul si sterge locatarul.
     */
    void uiRemove();

    /*
     * Citeste apartamentul si noul nume, apoi face update partial.
     */
    void uiUpdate();

    /*
     * Citeste apartamentul si afiseaza locatarul gasit.
     */
    void uiFind() const;

    /*
     * Afiseaza toti locatarii.
     */
    void uiPrintAll() const;

    /*
     * Citeste tipul si afiseaza rezultatul filtrarii.
     */
    void uiFilterByTip();

    /*
     * Citeste pragul de suprafata si afiseaza rezultatul filtrarii.
     */
    void uiFilterBySuprafata();

    /*
     * Afiseaza locatarii sortati dupa nume.
     */
    void uiSortByNume() const;

    /*
     * Afiseaza locatarii sortati dupa suprafata.
     */
    void uiSortBySuprafata() const;

    /*
     * Afiseaza locatarii sortati dupa tip si suprafata.
     */
    void uiSortByTipSiSuprafata() const;

    void uiClearListaNotificare();
    void uiAddApartamentInListaNotificare();
    void uiGenereazaListaNotificare();
    void uiAfiseazaListaNotificare() const;
    void uiExportListaNotificareCSV();
    void uiRaportTipApartament() const;
    void uiUndo();

public:
    /*
     * Initializeaza UI-ul cu service-ul dat.
     */
    UI(ServiceLocatar& service) : service(service) {}

    /*
     * Ruleaza bucla principala a aplicatiei.
     */
    void run();
};

#endif // LAB6_7_UI_H
