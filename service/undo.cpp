#include "undo.h"

UndoAdauga::UndoAdauga(RepoLocatari& repo, const int apartament) : repo(repo), apartament(apartament) {}

void UndoAdauga::doUndo()
{
    repo.remove(apartament);
}

UndoSterge::UndoSterge(RepoLocatari& repo, const Locatar& locatarSters) : repo(repo), locatarSters(locatarSters) {}

void UndoSterge::doUndo()
{
    repo.add(locatarSters.getApartament(), locatarSters.getNumeProprietar(), locatarSters.getSuprafata(),
             locatarSters.getTipApartament());
}

UndoModifica::UndoModifica(RepoLocatari& repo, const int apartament, const std::string& numeVechi)
    : repo(repo), apartament(apartament), numeVechi(numeVechi)
{
}

void UndoModifica::doUndo()
{
    repo.update(apartament, numeVechi);
}
