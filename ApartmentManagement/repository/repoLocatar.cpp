//
// Created by tudor on 4/1/2026.
//

#include "repoLocatar.h"

#include "../domain/exceptions.h"
#include <algorithm>

void RepoLocatariVector::add(int apartament, const string& nume_proprietar, float suprafata, const string& tip_apartament)
{
    const auto it = std::find_if(locatari.begin(), locatari.end(),
                                 [apartament](const Locatar& locatar)
                                 {
                                     return locatar.getApartament() == apartament;
                                 });

    if (it != locatari.end())
    {
        throw RepoException("Exista deja un locatar cu acest apartament.");
    }

    locatari.push_back(Locatar(apartament, nume_proprietar, suprafata, tip_apartament));
}

std::vector<Locatar> RepoLocatariVector::getAll() const
{
    return locatari;
}

void RepoLocatariVector::remove(int apartament)
{
    const auto it = std::find_if(locatari.begin(), locatari.end(),
                                 [apartament](const Locatar& locatar)
                                 {
                                     return locatar.getApartament() == apartament;
                                 });

    if (it == locatari.end())
    {
        throw RepoException("Locatarul nu a fost gasit.");
    }

    locatari.erase(it);
}

void RepoLocatariVector::update(int apartament, const string& nume_nou)
{
    const auto it = std::find_if(locatari.begin(), locatari.end(),
                                 [apartament](const Locatar& locatar)
                                 {
                                     return locatar.getApartament() == apartament;
                                 });

    if (it == locatari.end())
    {
        throw RepoException("Locatarul nu a fost gasit.");
    }

    it->setNumeProprietar(nume_nou);
}

const Locatar& RepoLocatariVector::find(int apartament) const
{
    const auto it = std::find_if(locatari.begin(), locatari.end(),
                                 [apartament](const Locatar& locatar)
                                 {
                                     return locatar.getApartament() == apartament;
                                 });

    if (it == locatari.end())
    {
        throw RepoException("Locatarul nu a fost gasit.");
    }

    return *it;
}

void RepoLocatariMap::add(int apartament, const string& nume_proprietar, float suprafata, const string& tip_apartament)
{
    if (locatari.find(apartament) != locatari.end())
    {
        throw RepoException("Exista deja un locatar cu acest apartament.");
    }
    locatari.emplace(apartament, Locatar(apartament, nume_proprietar, suprafata, tip_apartament));
}

std::vector<Locatar> RepoLocatariMap::getAll() const
{
    std::vector<Locatar> rezultat;
    rezultat.reserve(locatari.size());
    for (const auto& entry : locatari)
    {
        rezultat.push_back(entry.second);
    }
    return rezultat;
}

void RepoLocatariMap::remove(int apartament)
{
    const auto it = locatari.find(apartament);
    if (it == locatari.end())
    {
        throw RepoException("Locatarul nu a fost gasit.");
    }
    locatari.erase(it);
}

void RepoLocatariMap::update(int apartament, const string& nume_nou)
{
    const auto it = locatari.find(apartament);
    if (it == locatari.end())
    {
        throw RepoException("Locatarul nu a fost gasit.");
    }
    it->second.setNumeProprietar(nume_nou);
}

const Locatar& RepoLocatariMap::find(int apartament) const
{
    const auto it = locatari.find(apartament);
    if (it == locatari.end())
    {
        throw RepoException("Locatarul nu a fost gasit.");
    }
    return it->second;
}
