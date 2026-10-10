//
// Created by tudor on 4/1/2026.
//

#ifndef LAB6_7_SERVICELOCATAR_H
#define LAB6_7_SERVICELOCATAR_H

#include "../domain/locatar.h"
#include "../domain/validatorLocatar.h"
#include "../repository/repoLocatar.h"
#include "listaNotificare.h"
#include "raportTipApartamentDTO.h"
#include "undo.h"
#include "../utils/Observer.h"
#include <cstddef>
#include <memory>
#include <random>
#include <string>
#include <vector>

using std::string;

class ServiceLocatar
{
private:
    RepoLocatari& repo;
    ValidatorLocatar validator;
    ListaNotificare listaNotificare;
    std::vector<std::unique_ptr<ActiuneUndo>> undoActions;
    double probabilitateEsec;
    std::mt19937 generator;
    std::uniform_real_distribution<double> distributie;

    void verificaEsecProbabilistic();

public:
    /*
     * Initializeaza service-ul cu repository-ul dat.
     */
    ServiceLocatar(RepoLocatari& repo, double probabilitateEsec = 0.0);

    /*
     * Valideaza si adauga un locatar nou.
     *
     * arunca:
     *  ValidationException pentru date invalide
     *  RepoException daca apartamentul exista deja
     */
    void add(int apartament, const string& nume_proprietar, float suprafata, const string& tip_apartament);

    /*
     * Returneaza toate entitatile din repository.
     */
    [[nodiscard]] std::vector<Locatar> getAll() const;

    /*
     * Sterge locatarul cu apartamentul dat.
     *
     * arunca:
     *  RepoException daca locatarul nu exista
     */
    void remove(int apartament);

    /*
     * Modifica doar numele locatarului (update partial).
     *
     * arunca:
     *  ValidationException daca numele nou este invalid
     *  RepoException daca locatarul nu exista
     */
    void update(int apartament, const string& nume_proprietar);

    /*
     * Reface ultima operatie de add/remove/update.
     *
     * arunca:
     *  ServiceException daca nu mai exista operatii disponibile
     */
    void undo();

    /*
     * Returneaza locatarul gasit dupa apartament.
     *
     * arunca:
     *  RepoException daca locatarul nu exista
     */
    [[nodiscard]] const Locatar& find(int apartament) const;

    /*
     * Returneaza locatarii care au tipul apartamentului egal cu cel cerut.
     */
    [[nodiscard]] std::vector<Locatar> filterByTip(const string& tip_apartament) const;

    /*
     * Returneaza locatarii cu suprafata strict mai mica decat pragul dat.
     */
    [[nodiscard]] std::vector<Locatar> filterBySuprafata(float suprafata_maxima) const;

    /*
     * Returneaza locatarii sortati crescator dupa nume proprietar.
     */
    [[nodiscard]] std::vector<Locatar> sortByNumeProprietar() const;

    /*
     * Returneaza locatarii sortati crescator dupa suprafata.
     */
    [[nodiscard]] std::vector<Locatar> sortBySuprafata() const;

    /*
     * Returneaza locatarii sortati dupa tip, apoi dupa suprafata.
     */
    [[nodiscard]] std::vector<Locatar> sortByTipSiSuprafata() const;

    /*
     * Goleste lista de notificare.
     */
    void clearListaNotificare();

    /*
     * Adauga in lista de notificare apartamentul existent cu numarul dat.
     *
     * arunca:
     *  ServiceException daca apartamentul nu exista in lista principala
     */
    void addApartamentInListaNotificare(int apartament);

    /*
     * Genereaza aleator o lista de notificare cu n apartamente distincte.
     *
     * arunca:
     *  ServiceException daca n <= 0, lista principala e goala sau n depaseste nr de locatari
     */
    void genereazaListaNotificare(int n);

    /*
     * Returneaza lista curenta de notificare.
     */
    [[nodiscard]] const std::vector<Locatar>& getListaNotificare() const;

    /*
     * Returneaza numarul de apartamente din lista de notificare.
     */
    [[nodiscard]] std::size_t getListaNotificareSize() const;

    /*
     * Exporta lista de notificare in format CSV.
     */
    void exportListaNotificareCSV(const string& filename) const;

    void addListaNotificareObserver(Observer* observer);
    void removeListaNotificareObserver(Observer* observer);

    /*
     * Construieste raportul pe tip de apartament si il returneaza ca DTO-uri.
     */
    [[nodiscard]] std::vector<RaportTipApartamentDTO> raportTipApartament() const;
    std::vector<Locatar> sortByApartament() const;
};

#endif // LAB6_7_SERVICELOCATAR_H
