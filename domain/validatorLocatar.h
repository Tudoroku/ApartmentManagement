#ifndef LAB6_7_VALIDATORLOCATAR_H
#define LAB6_7_VALIDATORLOCATAR_H

#include <string>

using std::string;

class ValidatorLocatar
{
public:
    /*
     * Valideaza toate campurile unui locatar.
     *
     * arunca:
     *  ValidationException daca exista date invalide
     */
    void valideazaDate(int apartament, const string& nume_proprietar, float suprafata, const string& tip_apartament) const;

    /*
     * Valideaza numele proprietarului.
     *
     * arunca:
     *  ValidationException daca numele este vid sau contine doar spatii
     */
    void valideazaNume(const string& nume_proprietar) const;
};

#endif //LAB6_7_VALIDATORLOCATAR_H
