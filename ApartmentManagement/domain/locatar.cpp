//
// Created by tudor on 4/1/2026.
//

#include "locatar.h"

#include <iostream>
#include <ostream>
using std::cout;
using std::endl;
using std::ostream;
Locatar::Locatar(int apartament, const string &nume_proprietar, float suprafata, const string &tip_apartament)
    : apartament(apartament), nume_proprietar(nume_proprietar), suprafata(suprafata), tip_apartament(tip_apartament) {}

/*Locatar::Locatar(const Locatar &other) :
apartament(other.apartament), nume_proprietar(other.nume_proprietar),
suprafata(other.suprafata), tip_apartament(other.tip_apartament)
{
    cout << "copy constructor apelat pt " << other.apartament << endl;
}*/

void Locatar::setApartament(int apartament)
{
    this->apartament = apartament;
}

void Locatar::setNumeProprietar(const string &nume_proprietar)
{
    this->nume_proprietar = nume_proprietar;
}

void Locatar::setSuprafata(float suprafata)
{
    this->suprafata = suprafata;
}

void Locatar::setTipApartament(const string &tip_apartament)
{
    this->tip_apartament = tip_apartament;
}

int Locatar::getApartament() const
{
    return this->apartament;
}

const string& Locatar::getNumeProprietar() const
{
    return this->nume_proprietar;
}

float Locatar::getSuprafata() const
{
    return this->suprafata;
}

const string& Locatar::getTipApartament() const
{
    return this->tip_apartament;
}
