#ifndef LAB6_7_LISTANOTIFICARE_H
#define LAB6_7_LISTANOTIFICARE_H

#include "../domain/locatar.h"
#include "../utils/Observable.h"
#include <cstddef>
#include <string>
#include <vector>

class ListaNotificare : public Observable
{
private:
    std::vector<Locatar> notificari;

public:
    void clear();
    void add(const Locatar& locatar);
    void setContent(const std::vector<Locatar>& locatari);
    void exportCSV(const std::string& filename) const;

    [[nodiscard]] const std::vector<Locatar>& getAll() const noexcept;
    [[nodiscard]] std::size_t size() const noexcept;
};

#endif // LAB6_7_LISTANOTIFICARE_H
