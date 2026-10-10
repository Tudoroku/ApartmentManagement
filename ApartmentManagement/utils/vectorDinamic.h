#ifndef LAB6_7_VECTORDINAMIC_H
#define LAB6_7_VECTORDINAMIC_H

#include <cstddef>
#include <new>
#include <stdexcept>
#include <utility>

template<typename T>
class VectorDinamic
{
private:
    T* elemente;
    std::size_t lungime;
    std::size_t capacitate;

    /*
     * Realoca buffer-ul intern la o capacitate mai mare.
     * Copiaza elementele existente si elibereaza memoria veche.
     */
    void reserve(const std::size_t capacitate_noua)
    {
        if (capacitate_noua <= capacitate)
        {
            return;
        }

        T* elemente_noi = static_cast<T*>(::operator new(sizeof(T) * capacitate_noua));
        std::size_t i = 0;
        try
        {
            for (; i < lungime; i++)
            {
                new (elemente_noi + i) T(elemente[i]);
            }
        }
        catch (...)
        {
            for (std::size_t j = 0; j < i; j++)
            {
                elemente_noi[j].~T();
            }
            ::operator delete(elemente_noi);
            throw;
        }

        for (std::size_t j = 0; j < lungime; j++)
        {
            elemente[j].~T();
        }
        ::operator delete(elemente);

        elemente = elemente_noi;
        capacitate = capacitate_noua;
    }

public:
    /*
     * Creeaza un vector dinamic gol.
     */
    VectorDinamic() : elemente(nullptr), lungime(0), capacitate(0) {}

    /*
     * Elibereaza elementele si memoria alocata intern.
     */
    ~VectorDinamic()
    {
        for (std::size_t i = 0; i < lungime; i++)
        {
            elemente[i].~T();
        }
        ::operator delete(elemente);
    }

    /*
     * Creeaza o copie profunda a vectorului primit.
     */
    VectorDinamic(const VectorDinamic& other) : elemente(nullptr), lungime(0), capacitate(0)
    {
        if (other.capacitate == 0)
        {
            return;
        }

        elemente = static_cast<T*>(::operator new(sizeof(T) * other.capacitate));
        capacitate = other.capacitate;
        try
        {
            for (; lungime < other.lungime; lungime++)
            {
                new (elemente + lungime) T(other.elemente[lungime]);
            }
        }
        catch (...)
        {
            for (std::size_t i = 0; i < lungime; i++)
            {
                elemente[i].~T();
            }
            ::operator delete(elemente);
            elemente = nullptr;
            lungime = 0;
            capacitate = 0;
            throw;
        }
    }

    /*
     * Atribuie vectorul primit prin copy assignment.
     *
     * return:
     *  referinta la obiectul curent
     */
    VectorDinamic& operator=(const VectorDinamic& other)
    {
        if (this == &other)
        {
            return *this;
        }

        VectorDinamic copie(other);
        swap(copie);
        return *this;
    }

    /*
     * Inter-schimba continutul cu alt vector.
     */
    void swap(VectorDinamic& other) noexcept
    {
        std::swap(elemente, other.elemente);
        std::swap(lungime, other.lungime);
        std::swap(capacitate, other.capacitate);
    }

    /*
     * Returneaza numarul curent de elemente.
     */
    [[nodiscard]] std::size_t size() const noexcept
    {
        return lungime;
    }

    /*
     * Verifica daca vectorul este gol.
     */
    [[nodiscard]] bool empty() const noexcept
    {
        return lungime == 0;
    }

    /*
     * Returneaza elementul de pe pozitia data (modificabil).
     *
     * arunca:
     *  std::out_of_range daca pozitia este invalida
     */
    T& operator[](const std::size_t pozitie)
    {
        if (pozitie >= lungime)
        {
            throw std::out_of_range("Pozitie invalida in VectorDinamic");
        }
        return elemente[pozitie];
    }

    /*
     * Returneaza elementul de pe pozitia data (constant).
     *
     * arunca:
     *  std::out_of_range daca pozitia este invalida
     */
    const T& operator[](const std::size_t pozitie) const
    {
        if (pozitie >= lungime)
        {
            throw std::out_of_range("Pozitie invalida in VectorDinamic");
        }
        return elemente[pozitie];
    }

    /*
     * Adauga un element la finalul colectiei.
     * Daca nu mai exista spatiu, colectia se redimensioneaza automat.
     */
    void push_back(const T& valoare)
    {
        if (lungime == capacitate)
        {
            const std::size_t capacitate_noua = capacitate == 0 ? 2 : capacitate * 2;
            reserve(capacitate_noua);
        }
        new (elemente + lungime) T(valoare);
        lungime++;
    }

    /*
     * Sterge elementul de pe pozitia data si deplaseaza elementele la stanga.
     *
     * arunca:
     *  std::out_of_range daca pozitia este invalida
     */
    void erase(const std::size_t pozitie)
    {
        if (pozitie >= lungime)
        {
            throw std::out_of_range("Pozitie invalida in VectorDinamic");
        }

        for (std::size_t i = pozitie; i + 1 < lungime; i++)
        {
            elemente[i] = elemente[i + 1];
        }

        elemente[lungime - 1].~T();
        --lungime;
    }

    /*
     * Returneaza iterator la primul element.
     */
    T* begin() noexcept
    {
        return elemente;
    }

    /*
     * Returneaza iterator constant la primul element.
     */
    const T* begin() const noexcept
    {
        return elemente;
    }

    /*
     * Returneaza iterator la pozitia de dupa ultimul element.
     */
    T* end() noexcept
    {
        return elemente + lungime;
    }

    /*
     * Returneaza iterator constant la pozitia de dupa ultimul element.
     */
    const T* end() const noexcept
    {
        return elemente + lungime;
    }
};

#endif //LAB6_7_VECTORDINAMIC_H
