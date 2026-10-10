#ifndef LAB6_7_REPOLOCATARFILE_H
#define LAB6_7_REPOLOCATARFILE_H

#include "repoLocatar.h"
#include <string>

class RepoLocatariFile final : public RepoLocatariVector
{
private:
    std::string filename;

    void loadFromFile();
    void saveToFile() const;

public:
    explicit RepoLocatariFile(std::string filename);

    void add(int apartament, const string& nume_proprietar, float suprafata, const string& tip_apartament) override;
    void remove(int apartament) override;
    void update(int apartament, const string& nume_nou) override;
};

#endif // LAB6_7_REPOLOCATARFILE_H
