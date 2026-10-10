#include "repoLocatarFile.h"

#include "../domain/exceptions.h"
#include <fstream>
#include <sstream>
#include <utility>

RepoLocatariFile::RepoLocatariFile(std::string filename) : filename(std::move(filename))
{
    loadFromFile();
}

void RepoLocatariFile::loadFromFile()
{
    std::ifstream in(filename);
    if (!in.is_open())
    {
        return;
    }

    std::string line;
    while (std::getline(in, line))
    {
        if (line.empty())
        {
            continue;
        }

        std::stringstream linieStream(line);
        std::string apartamentText;
        std::string numeProprietar;
        std::string suprafataText;
        std::string tipApartament;

        if (!std::getline(linieStream, apartamentText, ',') || !std::getline(linieStream, numeProprietar, ',') ||
            !std::getline(linieStream, suprafataText, ',') || !std::getline(linieStream, tipApartament))
        {
            throw RepoException("Linie invalida in fisierul de locatari.");
        }

        try
        {
            const int apartament = std::stoi(apartamentText);
            const float suprafata = std::stof(suprafataText);
            RepoLocatariVector::add(apartament, numeProprietar, suprafata, tipApartament);
        }
        catch (const std::exception&)
        {
            throw RepoException("Date invalide in fisierul de locatari.");
        }
    }
}

void RepoLocatariFile::saveToFile() const
{
    std::ofstream out(filename);
    if (!out.is_open())
    {
        throw RepoException("Nu s-a putut deschide fisierul de locatari pentru scriere.");
    }

    for (const auto& locatar : getAll())
    {
        out << locatar.getApartament() << ',' << locatar.getNumeProprietar() << ',' << locatar.getSuprafata() << ','
            << locatar.getTipApartament() << '\n';
    }

    if (!out.good())
    {
        throw RepoException("Eroare la salvarea fisierului de locatari.");
    }
}

void RepoLocatariFile::add(const int apartament, const string& nume_proprietar, const float suprafata,
                           const string& tip_apartament)
{
    RepoLocatariVector::add(apartament, nume_proprietar, suprafata, tip_apartament);
    saveToFile();
}

void RepoLocatariFile::remove(const int apartament)
{
    RepoLocatariVector::remove(apartament);
    saveToFile();
}

void RepoLocatariFile::update(const int apartament, const string& nume_nou)
{
    RepoLocatariVector::update(apartament, nume_nou);
    saveToFile();
}
