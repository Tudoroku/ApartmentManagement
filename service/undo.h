#ifndef LAB6_7_UNDO_H
#define LAB6_7_UNDO_H

#include "../domain/locatar.h"
#include "../repository/repoLocatar.h"
#include <string>

class ActiuneUndo
{
public:
    virtual void doUndo() = 0;
    virtual ~ActiuneUndo() = default;
};

class UndoAdauga final : public ActiuneUndo
{
private:
    RepoLocatari& repo;
    int apartament;

public:
    UndoAdauga(RepoLocatari& repo, int apartament);
    void doUndo() override;
};

class UndoSterge final : public ActiuneUndo
{
private:
    RepoLocatari& repo;
    Locatar locatarSters;

public:
    UndoSterge(RepoLocatari& repo, const Locatar& locatarSters);
    void doUndo() override;
};

class UndoModifica final : public ActiuneUndo
{
private:
    RepoLocatari& repo;
    int apartament;
    std::string numeVechi;

public:
    UndoModifica(RepoLocatari& repo, int apartament, const std::string& numeVechi);
    void doUndo() override;
};

#endif // LAB6_7_UNDO_H
