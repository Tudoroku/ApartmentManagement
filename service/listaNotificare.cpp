#include "listaNotificare.h"

#include <fstream>
#include <stdexcept>

void ListaNotificare::clear()
{
    notificari.clear();
    notifyObservers();
}

void ListaNotificare::add(const Locatar& locatar)
{
    notificari.push_back(locatar);
    notifyObservers();
}

void ListaNotificare::setContent(const std::vector<Locatar>& locatari)
{
    notificari = locatari;
    notifyObservers();
}

void ListaNotificare::exportCSV(const std::string& filename) const
{
    std::ofstream out(filename);
    if (!out.is_open())
    {
        throw std::runtime_error("Nu s-a putut deschide fisierul pentru export.");
    }

    for (const auto& locatar : notificari)
    {
        out << locatar.getApartament() << ','
            << locatar.getNumeProprietar() << ','
            << locatar.getSuprafata() << ','
            << locatar.getTipApartament() << '\n';
    }
}

const std::vector<Locatar>& ListaNotificare::getAll() const noexcept
{
    return notificari;
}

std::size_t ListaNotificare::size() const noexcept
{
    return notificari.size();
}
