//
// Created by tudor on 4/1/2026.
//

#ifndef LAB6_7_REPOLOCATAR_H
#define LAB6_7_REPOLOCATAR_H

#include "../domain/locatar.h"
#include <map>
#include <string>
#include <vector>

using std::string;

class RepoLocatari
{
public:
    virtual ~RepoLocatari() = default;

    /*
     * Adauga un locatar in colectie.
     *
     * arunca:
     *  RepoException daca exista deja locatar cu acelasi apartament
     */
    virtual void add(int apartament, const string& nume_proprietar, float suprafata, const string& tip_apartament) = 0;

    /*
     * Returneaza toate entitatile stocate.
     */
    [[nodiscard]] virtual std::vector<Locatar> getAll() const = 0;

    /*
     * Sterge locatarul cu apartamentul dat.
     *
     * arunca:
     *  RepoException daca locatarul nu exista
     */
    virtual void remove(int apartament) = 0;

    /*
     * Modifica numele locatarului identificat prin apartament.
     *
     * arunca:
     *  RepoException daca locatarul nu exista
     */
    virtual void update(int apartament, const string& nume_nou) = 0;

    /*
     * Cauta locatarul dupa apartament.
     *
     * return:
     *  referinta constanta la locatarul gasit
     *
     * arunca:
     *  RepoException daca locatarul nu exista
     */
    [[nodiscard]] virtual const Locatar& find(int apartament) const = 0;
};

class RepoLocatariVector : public RepoLocatari
{
private:
    std::vector<Locatar> locatari;

public:
    void add(int apartament, const string& nume_proprietar, float suprafata, const string& tip_apartament) override;
    [[nodiscard]] std::vector<Locatar> getAll() const override;
    void remove(int apartament) override;
    void update(int apartament, const string& nume_nou) override;
    [[nodiscard]] const Locatar& find(int apartament) const override;
};

class RepoLocatariMap final : public RepoLocatari
{
private:
    std::map<int, Locatar> locatari;

public:
    void add(int apartament, const string& nume_proprietar, float suprafata, const string& tip_apartament) override;
    [[nodiscard]] std::vector<Locatar> getAll() const override;
    void remove(int apartament) override;
    void update(int apartament, const string& nume_nou) override;
    [[nodiscard]] const Locatar& find(int apartament) const override;
};

#endif //LAB6_7_REPOLOCATAR_H
