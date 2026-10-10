//
// Created by tudor on 4/1/2026.
//

#ifndef LAB6_7_LOCATAR_H
#define LAB6_7_LOCATAR_H
#include <string>
using std :: string;
class Locatar
{
private:
    int apartament;
    string nume_proprietar;
    float suprafata;
    string tip_apartament;
public:
    /*
     * Creeaza un locatar cu toate datele primite ca parametri.
     */
    Locatar(int apartament, const string &nume_proprietar, float suprafata, const string &tip_apartament);

    /*
     * Creeaza o copie a unui locatar existent.
     */
   // Locatar(const Locatar& other);

    /*
     * Seteaza numarul apartamentului.
     */
    void setApartament(int);

    /*
     * Seteaza numele proprietarului.
     */
    void setNumeProprietar(const string &);

    /*
     * Seteaza suprafata apartamentului.
     */
    void setSuprafata(float);

    /*
     * Seteaza tipul apartamentului.
     */
    void setTipApartament(const string &);

    /*
     * Returneaza numarul apartamentului.
     */
    [[nodiscard]] int getApartament() const;

    /*
     * Returneaza numele proprietarului.
     */
    [[nodiscard]] const string& getNumeProprietar() const;

    /*
     * Returneaza suprafata apartamentului.
     */
    [[nodiscard]] float getSuprafata() const;

    /*
     * Returneaza tipul apartamentului.
     */
    [[nodiscard]] const string& getTipApartament() const;
};
#endif //LAB6_7_LOCATAR_H
