#include "validatorLocatar.h"
#include "exceptions.h"

namespace
{
bool esteVidSauSpatii(const string& text)
{
    return text.find_first_not_of(" \t\r\n") == string::npos;
}
}

void ValidatorLocatar::valideazaDate(const int apartament, const string& nume_proprietar, const float suprafata,
                                     const string& tip_apartament) const
{
    string erori;

    if (apartament <= 0)
    {
        erori += "Apartamentul trebuie sa fie > 0. ";
    }
    if (esteVidSauSpatii(nume_proprietar))
    {
        erori += "Numele proprietarului nu poate fi vid. ";
    }
    if (suprafata <= 0)
    {
        erori += "Suprafata trebuie sa fie > 0. ";
    }
    if (esteVidSauSpatii(tip_apartament))
    {
        erori += "Tipul apartamentului nu poate fi vid. ";
    }

    if (!erori.empty())
    {
        throw ValidationException(erori);
    }
}

void ValidatorLocatar::valideazaNume(const string& nume_proprietar) const
{
    if (esteVidSauSpatii(nume_proprietar))
    {
        throw ValidationException("Numele proprietarului nu poate fi vid.");
    }
}
