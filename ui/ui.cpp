//
// Created by tudor on 4/1/2026.
//

#include "ui.h"

#include <iostream>
#include <limits>
#include <string>

using std::cin;
using std::cout;
using std::string;

bool UI::readInt(const string& mesaj, int& valoare) const
{
    cout << mesaj;
    if (!(cin >> valoare))
    {
        cout << "Eroare: trebuie introdus un numar intreg valid.\n";
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return true;
}

bool UI::readFloat(const string& mesaj, float& valoare) const
{
    cout << mesaj;
    if (!(cin >> valoare))
    {
        cout << "Eroare: trebuie introdus un numar real valid.\n";
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return true;
}

void UI::printMenu() const
{
    cout << "\n=== MENIU ===\n";
    cout << "1. Adauga locatar\n";
    cout << "2. Sterge locatar\n";
    cout << "3. Modifica locatar\n";
    cout << "4. Cauta locatar\n";
    cout << "5. Afiseaza toti locatarii\n";
    cout << "6. Filtreaza dupa tip apartament\n";
    cout << "7. Filtreaza dupa suprafata (< valoare)\n";
    cout << "8. Sorteaza dupa nume proprietar\n";
    cout << "9. Sorteaza dupa suprafata\n";
    cout << "10. Sorteaza dupa tip + suprafata\n";
    cout << "11. Goleste lista de notificare\n";
    cout << "12. Adauga apartament in lista de notificare\n";
    cout << "13. Genereaza aleator lista de notificare\n";
    cout << "14. Afiseaza lista de notificare\n";
    cout << "15. Raport numar apartamente pe tip\n";
    cout << "16. Exporta lista de notificare CSV\n";
    cout << "17. Undo\n";
    cout << "0. Iesire\n";
    cout << "Optiunea ta: ";
}

void UI::printLocatari(const std::vector<Locatar>& locatari) const
{
    if (locatari.empty())
    {
        cout << "Nu exista locatari.\n";
        return;
    }

    for (const auto& locatar : locatari)
    {
        cout << "-------------------------\n";
        cout << "Apartament: " << locatar.getApartament() << '\n';
        cout << "Nume proprietar: " << locatar.getNumeProprietar() << '\n';
        cout << "Suprafata: " << locatar.getSuprafata() << '\n';
        cout << "Tip apartament: " << locatar.getTipApartament() << '\n';
    }
    cout << "-------------------------\n";
}

void UI::uiAdd()
{
    int apartament;
    string nume_proprietar;
    float suprafata;
    string tip_apartament;

    if (!readInt("Apartament: ", apartament))
    {
        return;
    }

    cout << "Nume proprietar: ";
    std::getline(cin, nume_proprietar);

    if (!readFloat("Suprafata: ", suprafata))
    {
        return;
    }

    cout << "Tip apartament: ";
    std::getline(cin, tip_apartament);

    service.add(apartament, nume_proprietar, suprafata, tip_apartament);

    cout << "Locatar adaugat cu succes.\n";
}

void UI::uiRemove()
{
    int apartament;

    if (!readInt("Apartamentul locatarului de sters: ", apartament))
    {
        return;
    }

    service.remove(apartament);

    cout << "Locatar sters cu succes.\n";
}

void UI::uiUpdate()
{
    int apartament;
    string nume_nou;

    if (!readInt("Apartamentul locatarului de modificat: ", apartament))
    {
        return;
    }

    cout << "Noul nume al proprietarului: ";
    std::getline(cin, nume_nou);

    service.update(apartament, nume_nou);

    cout << "Locatar modificat cu succes.\n";
}

void UI::uiFind() const
{
    int apartament;

    if (!readInt("Apartamentul cautat: ", apartament))
    {
        return;
    }

    const Locatar& locatar = service.find(apartament);

    cout << "Locatar gasit:\n";
    cout << "Apartament: " << locatar.getApartament() << '\n';
    cout << "Nume proprietar: " << locatar.getNumeProprietar() << '\n';
    cout << "Suprafata: " << locatar.getSuprafata() << '\n';
    cout << "Tip apartament: " << locatar.getTipApartament() << '\n';
}

void UI::uiPrintAll() const
{
    printLocatari(service.getAll());
}

void UI::uiFilterByTip()
{
    string tip_apartament;

    cout << "Tip apartament: ";
    std::getline(cin, tip_apartament);

    const auto rezultat = service.filterByTip(tip_apartament);
    printLocatari(rezultat);
}

void UI::uiFilterBySuprafata()
{
    float suprafata_maxima;
    if (!readFloat("Suprafata maxima (strict <): ", suprafata_maxima))
    {
        return;
    }

    const auto rezultat = service.filterBySuprafata(suprafata_maxima);
    printLocatari(rezultat);
}

void UI::uiSortByNume() const
{
    const auto rezultat = service.sortByNumeProprietar();
    printLocatari(rezultat);
}

void UI::uiSortBySuprafata() const
{
    const auto rezultat = service.sortBySuprafata();
    printLocatari(rezultat);
}

void UI::uiSortByTipSiSuprafata() const
{
    const auto rezultat = service.sortByTipSiSuprafata();
    printLocatari(rezultat);
}

void UI::uiClearListaNotificare()
{
    service.clearListaNotificare();
    cout << "Lista de notificare a fost golita.\n";
    cout << "Total apartamente in lista de notificare: " << service.getListaNotificareSize() << '\n';
}

void UI::uiAddApartamentInListaNotificare()
{
    int apartament;
    if (!readInt("Apartament de adaugat in lista de notificare: ", apartament))
    {
        return;
    }

    service.addApartamentInListaNotificare(apartament);
    cout << "Apartamentul a fost adaugat in lista de notificare.\n";
    cout << "Total apartamente in lista de notificare: " << service.getListaNotificareSize() << '\n';
}

void UI::uiGenereazaListaNotificare()
{
    int n;
    if (!readInt("Numarul de apartamente pentru generare: ", n))
    {
        return;
    }

    service.genereazaListaNotificare(n);
    cout << "Lista de notificare a fost generata aleator.\n";
    cout << "Total apartamente in lista de notificare: " << service.getListaNotificareSize() << '\n';
}

void UI::uiAfiseazaListaNotificare() const
{
    printLocatari(service.getListaNotificare());
    cout << "Total apartamente in lista de notificare: " << service.getListaNotificareSize() << '\n';
}

void UI::uiExportListaNotificareCSV()
{
    string filename;
    cout << "Nume fisier CSV: ";
    std::getline(cin, filename);

    service.exportListaNotificareCSV(filename);
    cout << "Export realizat cu succes.\n";
}

void UI::uiRaportTipApartament() const
{
    const auto raport = service.raportTipApartament();
    if (raport.empty())
    {
        cout << "Nu exista locatari pentru raport.\n";
        return;
    }

    cout << "Raport tip apartament:\n";
    for (const auto& dto : raport)
    {
        cout << dto.tipApartament << " -> " << dto.numarAparitii << '\n';
    }
}

void UI::uiUndo()
{
    service.undo();
    cout << "Undo realizat cu succes.\n";
}

void UI::run()
{
    while (true)
    {
        printMenu();

        int optiune;
        if (!(cin >> optiune))
        {
            cout << "Eroare: optiunea trebuie sa fie un numar intreg.\n";
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        try
        {
            switch (optiune)
            {
                case 1:
                    uiAdd();
                    break;
                case 2:
                    uiRemove();
                    break;
                case 3:
                    uiUpdate();
                    break;
                case 4:
                    uiFind();
                    break;
                case 5:
                    uiPrintAll();
                    break;
                case 6:
                    uiFilterByTip();
                    break;
                case 7:
                    uiFilterBySuprafata();
                    break;
                case 8:
                    uiSortByNume();
                    break;
                case 9:
                    uiSortBySuprafata();
                    break;
                case 10:
                    uiSortByTipSiSuprafata();
                    break;
                case 11:
                    uiClearListaNotificare();
                    break;
                case 12:
                    uiAddApartamentInListaNotificare();
                    break;
                case 13:
                    uiGenereazaListaNotificare();
                    break;
                case 14:
                    uiAfiseazaListaNotificare();
                    break;
                case 15:
                    uiRaportTipApartament();
                    break;
                case 16:
                    uiExportListaNotificareCSV();
                    break;
                case 17:
                    uiUndo();
                    break;
                case 0:
                    cout << "La revedere.\n";
                    return;
                default:
                    cout << "Optiune invalida.\n";
            }
        }
        catch (const std::exception& ex)
        {
            cout << "Eroare: " << ex.what() << '\n';
        }
    }
}
