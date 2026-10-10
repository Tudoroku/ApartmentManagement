//
// Created by tudor on 4/1/2026.
//

#include "serviceLocatar.h"

#include "../domain/exceptions.h"
#include <algorithm>
#include <map>
#include <random>
#include <vector>

ServiceLocatar::ServiceLocatar(RepoLocatari& repo, const double probabilitateEsec)
    : repo(repo), probabilitateEsec(probabilitateEsec), generator(std::random_device{}()), distributie(0.0, 1.0)
{
    if (probabilitateEsec < 0.0 || probabilitateEsec > 1.0)
    {
        throw ServiceException("Probabilitatea de esec trebuie sa fie in intervalul [0, 1].");
    }
}

void ServiceLocatar::verificaEsecProbabilistic()
{
    if (probabilitateEsec <= 0.0)
    {
        return;
    }
    if (probabilitateEsec >= 1.0)
    {
        throw ServiceException("Operatia a esuat probabilistic.");
    }

    const double valoare = distributie(generator);
    if (valoare < probabilitateEsec)
    {
        throw ServiceException("Operatia a esuat probabilistic.");
    }
}

void ServiceLocatar::add(int apartament, const string& nume_proprietar, float suprafata, const string& tip_apartament)
{
    verificaEsecProbabilistic();
    validator.valideazaDate(apartament, nume_proprietar, suprafata, tip_apartament);
    repo.add(apartament, nume_proprietar, suprafata, tip_apartament);
    undoActions.push_back(std::make_unique<UndoAdauga>(repo, apartament));
}

void ServiceLocatar::remove(int apartament)
{
    verificaEsecProbabilistic();
    const Locatar locatarDeSters = repo.find(apartament);
    repo.remove(apartament);
    undoActions.push_back(std::make_unique<UndoSterge>(repo, locatarDeSters));
}

void ServiceLocatar::update(int apartament, const string& nume_nou)
{
    verificaEsecProbabilistic();
    validator.valideazaNume(nume_nou);
    const string numeVechi = repo.find(apartament).getNumeProprietar();
    repo.update(apartament, nume_nou);
    undoActions.push_back(std::make_unique<UndoModifica>(repo, apartament, numeVechi));
}

void ServiceLocatar::undo()
{
    if (undoActions.empty())
    {
        throw ServiceException("Nu mai exista operatii pentru undo.");
    }

    undoActions.back()->doUndo();
    undoActions.pop_back();
}

std::vector<Locatar> ServiceLocatar::sortByApartament() const
{
    auto lista = repo.getAll();
    std::sort(lista.begin(), lista.end(),
              [](const Locatar& l1, const Locatar& l2)
              {
                  return l1.getApartament() < l2.getApartament();
              });
    return lista;
}

const Locatar& ServiceLocatar::find(int apartament) const
{
    return repo.find(apartament);
}

std::vector<Locatar> ServiceLocatar::getAll() const
{
    return repo.getAll();
}

std::vector<Locatar> ServiceLocatar::filterByTip(const string& tip_apartament) const
{
    const auto toateLocatarii = repo.getAll();
    std::vector<Locatar> rezultat;
    std::copy_if(toateLocatarii.begin(), toateLocatarii.end(), std::back_inserter(rezultat),
                 [&tip_apartament](const Locatar& locatar)
                 {
                     return locatar.getTipApartament() == tip_apartament;
                 });
    return rezultat;
}

std::vector<Locatar> ServiceLocatar::filterBySuprafata(float suprafata_maxima) const
{
    const auto toateLocatarii = repo.getAll();
    std::vector<Locatar> rezultat;
    std::copy_if(toateLocatarii.begin(), toateLocatarii.end(), std::back_inserter(rezultat),
                 [suprafata_maxima](const Locatar& locatar)
                 {
                     return locatar.getSuprafata() < suprafata_maxima;
                 });
    return rezultat;
}

std::vector<Locatar> ServiceLocatar::sortByNumeProprietar() const
{
    std::vector<Locatar> rezultat = repo.getAll();
    std::sort(rezultat.begin(), rezultat.end(),
              [](const Locatar& l1, const Locatar& l2)
              {
                  return l1.getNumeProprietar() < l2.getNumeProprietar();
              });
    return rezultat;
}

std::vector<Locatar> ServiceLocatar::sortBySuprafata() const
{
    std::vector<Locatar> rezultat = repo.getAll();
    std::sort(rezultat.begin(), rezultat.end(),
              [](const Locatar& l1, const Locatar& l2)
              {
                  return l1.getSuprafata() < l2.getSuprafata();
              });
    return rezultat;
}

std::vector<Locatar> ServiceLocatar::sortByTipSiSuprafata() const
{
    std::vector<Locatar> rezultat = repo.getAll();
    std::sort(rezultat.begin(), rezultat.end(),
              [](const Locatar& l1, const Locatar& l2)
              {
                  if (l1.getTipApartament() == l2.getTipApartament())
                  {
                      return l1.getSuprafata() < l2.getSuprafata();
                  }
                  return l1.getTipApartament() < l2.getTipApartament();
              });
    return rezultat;
}

void ServiceLocatar::clearListaNotificare()
{
    listaNotificare.clear();
}

void ServiceLocatar::addApartamentInListaNotificare(int apartament)
{
    try
    {
        const Locatar& locatar = repo.find(apartament);
        listaNotificare.add(locatar);
    }
    catch (const RepoException&)
    {
        throw ServiceException("Apartamentul nu exista in lista principala.");
    }
}

void ServiceLocatar::genereazaListaNotificare(int n)
{
    if (n <= 0)
    {
        throw ServiceException("Numarul de apartamente trebuie sa fie > 0.");
    }

    const auto toateLocatarii = repo.getAll();
    if (toateLocatarii.empty())
    {
        throw ServiceException("Nu exista locatari in lista principala.");
    }
    if (static_cast<std::size_t>(n) > toateLocatarii.size())
    {
        throw ServiceException("Nu exista suficienti locatari pentru generare.");
    }

    std::vector<Locatar> selectie = toateLocatarii;
    std::random_device rd;
    std::mt19937 generator(rd());
    std::shuffle(selectie.begin(), selectie.end(), generator);

    std::vector<Locatar> rezultat(selectie.begin(), selectie.begin() + n);
    listaNotificare.setContent(rezultat);
}

const std::vector<Locatar>& ServiceLocatar::getListaNotificare() const
{
    return listaNotificare.getAll();
}

std::size_t ServiceLocatar::getListaNotificareSize() const
{
    return listaNotificare.size();
}

void ServiceLocatar::exportListaNotificareCSV(const string& filename) const
{
    listaNotificare.exportCSV(filename);
}

void ServiceLocatar::addListaNotificareObserver(Observer* observer)
{
    listaNotificare.addObserver(observer);
}

void ServiceLocatar::removeListaNotificareObserver(Observer* observer)
{
    listaNotificare.removeObserver(observer);
}

std::vector<RaportTipApartamentDTO> ServiceLocatar::raportTipApartament() const
{
    std::map<string, int> frecventeTip;
    for (const auto& locatar : repo.getAll())
    {
        ++frecventeTip[locatar.getTipApartament()];
    }

    std::vector<RaportTipApartamentDTO> raport;
    raport.reserve(frecventeTip.size());
    for (const auto& [tip, numar] : frecventeTip)
    {
        raport.push_back({tip, numar});
    }
    return raport;
}
