#include <cassert>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "../domain/exceptions.h"
#include "../domain/locatar.h"
#include "../domain/validatorLocatar.h"
#include "../repository/repoLocatar.h"
#include "../repository/repoLocatarFile.h"
#include "../service/listaNotificare.h"
#include "../service/serviceLocatar.h"
#include "../service/undo.h"

using std::string;
namespace fs = std::filesystem;

#define REQUIRE(cond) assert(cond)

namespace
{
fs::path testRoot()
{
    const fs::path root = fs::temp_directory_path() / "lab6_7_qt_tests";
    fs::create_directories(root);
    return root;
}

fs::path tempFile(const string& name)
{
    const fs::path file = testRoot() / name;
    fs::remove(file);
    return file;
}

string readAll(const fs::path& file)
{
    std::ifstream in(file);
    std::stringstream buffer;
    buffer << in.rdbuf();
    return buffer.str();
}

template <typename TException, typename TFunc>
string mustThrow(TFunc&& func)
{
    try
    {
        func();
    }
    catch (const TException& ex)
    {
        return ex.what();
    }
    assert(false);
    return {};
}

void populate(ServiceLocatar& service)
{
    service.add(1, "Mihai", 55.0F, "studio");
    service.add(2, "Ana", 42.0F, "decomandat");
    service.add(3, "Bogdan", 31.0F, "semidecomandat");
    service.add(4, "Carmen", 60.0F, "decomandat");
}
}

void testDomainAndExceptions()
{
    Locatar locatar(10, "Ion Pop", 45.5F, "garsoniera");
    REQUIRE(locatar.getApartament() == 10);
    REQUIRE(locatar.getNumeProprietar() == "Ion Pop");
    REQUIRE(std::fabs(locatar.getSuprafata() - 45.5F) < 0.001F);
    REQUIRE(locatar.getTipApartament() == "garsoniera");

    locatar.setApartament(20);
    locatar.setNumeProprietar("Maria Ionescu");
    locatar.setSuprafata(60.0F);
    locatar.setTipApartament("decomandat");
    REQUIRE(locatar.getApartament() == 20);
    REQUIRE(locatar.getNumeProprietar() == "Maria Ionescu");
    REQUIRE(locatar.getSuprafata() == 60.0F);
    REQUIRE(locatar.getTipApartament() == "decomandat");

    Locatar copie(locatar);
    locatar.setNumeProprietar("Schimbat");
    REQUIRE(copie.getNumeProprietar() == "Maria Ionescu");

    REQUIRE(string(ValidationException("validare").what()) == "validare");
    REQUIRE(string(RepoException("repo").what()) == "repo");
    REQUIRE(string(ServiceException("service").what()) == "service");
}

void testValidator()
{
    ValidatorLocatar validator;
    validator.valideazaDate(1, "Ion", 1.0F, "studio");
    validator.valideazaNume("Ion");

    const string allErrors = mustThrow<ValidationException>([&]() {
        validator.valideazaDate(0, " \t\n", -2.0F, "   ");
    });
    REQUIRE(allErrors.find("Apartamentul trebuie sa fie > 0.") != string::npos);
    REQUIRE(allErrors.find("Numele proprietarului nu poate fi vid.") != string::npos);
    REQUIRE(allErrors.find("Suprafata trebuie sa fie > 0.") != string::npos);
    REQUIRE(allErrors.find("Tipul apartamentului nu poate fi vid.") != string::npos);

    REQUIRE(mustThrow<ValidationException>([&]() { validator.valideazaNume("\t\n "); }) ==
            "Numele proprietarului nu poate fi vid.");
}

template <typename TRepo>
void testRepoConcret()
{
    TRepo repo;
    REQUIRE(repo.getAll().empty());

    repo.add(20, "Bogdan", 50.0F, "decomandat");
    repo.add(10, "Ana", 40.0F, "studio");
    REQUIRE(repo.getAll().size() == 2);
    REQUIRE(repo.find(10).getNumeProprietar() == "Ana");

    repo.update(20, "Bogdan Nou");
    REQUIRE(repo.find(20).getNumeProprietar() == "Bogdan Nou");

    REQUIRE(mustThrow<RepoException>([&]() { repo.add(20, "Duplicat", 1.0F, "x"); }) ==
            "Exista deja un locatar cu acest apartament.");
    REQUIRE(mustThrow<RepoException>([&]() { [[maybe_unused]] const auto& ignored = repo.find(999); }) == "Locatarul nu a fost gasit.");
    REQUIRE(mustThrow<RepoException>([&]() { repo.update(999, "Nume"); }) == "Locatarul nu a fost gasit.");
    REQUIRE(mustThrow<RepoException>([&]() { repo.remove(999); }) == "Locatarul nu a fost gasit.");

    repo.remove(10);
    REQUIRE(repo.getAll().size() == 1);
    REQUIRE(repo.find(20).getNumeProprietar() == "Bogdan Nou");
}

void testRepoVectorAndMap()
{
    testRepoConcret<RepoLocatariVector>();
    testRepoConcret<RepoLocatariMap>();

    RepoLocatariMap repoMap;
    repoMap.add(2, "B", 20.0F, "b");
    repoMap.add(1, "A", 10.0F, "a");
    const auto all = repoMap.getAll();
    REQUIRE(all[0].getApartament() == 1);
    REQUIRE(all[1].getApartament() == 2);
}

void testListaNotificareDirect()
{
    ListaNotificare lista;
    REQUIRE(lista.getAll().empty());
    REQUIRE(lista.size() == 0);

    lista.add(Locatar(1, "Ana", 30.0F, "studio"));
    REQUIRE(lista.size() == 1);
    REQUIRE(lista.getAll()[0].getApartament() == 1);

    lista.setContent({Locatar(2, "B", 40.0F, "x"), Locatar(3, "C", 50.0F, "y")});
    REQUIRE(lista.size() == 2);
    REQUIRE(lista.getAll()[1].getApartament() == 3);

    const fs::path file = tempFile("notificari_direct.csv");
    lista.exportCSV(file.string());
    REQUIRE(readAll(file) == "2,B,40,x\n3,C,50,y\n");

    const fs::path badFile = testRoot() / "director_inexistent" / "notificari.csv";
    fs::remove_all(badFile.parent_path());
    mustThrow<std::runtime_error>([&]() { lista.exportCSV(badFile.string()); });

    lista.clear();
    REQUIRE(lista.getAll().empty());
    fs::remove(file);
}

void testServiceCRUDValidationAndRepoErrors()
{
    RepoLocatariVector repo;
    ServiceLocatar service(repo, 0.0);

    REQUIRE(mustThrow<ValidationException>([&]() { service.add(0, "", 0.0F, ""); }).find("Apartamentul") != string::npos);

    service.add(10, "Dan", 48.0F, "studio");
    service.add(20, "Elena", 65.0F, "decomandat");
    REQUIRE(service.getAll().size() == 2);
    REQUIRE(service.find(10).getNumeProprietar() == "Dan");

    REQUIRE(mustThrow<RepoException>([&]() { service.add(10, "Duplicat", 1.0F, "x"); }) ==
            "Exista deja un locatar cu acest apartament.");
    REQUIRE(mustThrow<ValidationException>([&]() { service.update(10, "   "); }) ==
            "Numele proprietarului nu poate fi vid.");
    REQUIRE(mustThrow<RepoException>([&]() { [[maybe_unused]] const auto& ignored = service.find(404); }) == "Locatarul nu a fost gasit.");
    REQUIRE(mustThrow<RepoException>([&]() { service.remove(404); }) == "Locatarul nu a fost gasit.");

    service.update(10, "Dan Nou");
    REQUIRE(service.find(10).getNumeProprietar() == "Dan Nou");
    service.remove(20);
    REQUIRE(service.getAll().size() == 1);
}

void testServiceProbability()
{
    RepoLocatariVector repo;
    REQUIRE(mustThrow<ServiceException>([&]() { ServiceLocatar invalid(repo, -0.1); }) ==
            "Probabilitatea de esec trebuie sa fie in intervalul [0, 1].");
    REQUIRE(mustThrow<ServiceException>([&]() { ServiceLocatar invalid(repo, 1.1); }) ==
            "Probabilitatea de esec trebuie sa fie in intervalul [0, 1].");

    ServiceLocatar serviceZero(repo, 0.0);
    serviceZero.add(1, "Ana", 35.0F, "studio");
    serviceZero.update(1, "Ana Noua");
    serviceZero.remove(1);
    REQUIRE(serviceZero.getAll().empty());

    repo.add(1, "Ana", 35.0F, "studio");
    ServiceLocatar serviceFail(repo, 1.0);
    REQUIRE(mustThrow<ServiceException>([&]() { serviceFail.add(2, "B", 40.0F, "x"); }) ==
            "Operatia a esuat probabilistic.");
    REQUIRE(mustThrow<ServiceException>([&]() { serviceFail.update(1, "Ana X"); }) ==
            "Operatia a esuat probabilistic.");
    REQUIRE(mustThrow<ServiceException>([&]() { serviceFail.remove(1); }) ==
            "Operatia a esuat probabilistic.");
    REQUIRE(repo.find(1).getNumeProprietar() == "Ana");

    RepoLocatariVector repoRandom;
    ServiceLocatar serviceRandom(repoRandom, 0.5);
    bool success = false;
    bool fail = false;
    for (int i = 1; i <= 1000 && (!success || !fail); ++i)
    {
        try
        {
            serviceRandom.add(10000 + i, "Nume", 10.0F, "tip");
            success = true;
        }
        catch (const ServiceException& ex)
        {
            REQUIRE(string(ex.what()) == "Operatia a esuat probabilistic.");
            fail = true;
        }
    }
    REQUIRE(success);
    REQUIRE(fail);
}

void testServiceFilterSortReport()
{
    RepoLocatariVector repo;
    ServiceLocatar service(repo);
    REQUIRE(service.sortByNumeProprietar().empty());
    REQUIRE(service.sortBySuprafata().empty());
    REQUIRE(service.sortByTipSiSuprafata().empty());
    REQUIRE(service.raportTipApartament().empty());

    populate(service);

    const auto tip = service.filterByTip("decomandat");
    REQUIRE(tip.size() == 2);
    REQUIRE(tip[0].getApartament() == 2);
    REQUIRE(tip[1].getApartament() == 4);
    REQUIRE(service.filterByTip("nu exista").empty());

    const auto sup = service.filterBySuprafata(50.0F);
    REQUIRE(sup.size() == 2);
    REQUIRE(sup[0].getApartament() == 2);
    REQUIRE(sup[1].getApartament() == 3);
    REQUIRE(service.filterBySuprafata(1.0F).empty());

    const auto byName = service.sortByNumeProprietar();
    REQUIRE(byName[0].getNumeProprietar() == "Ana");
    REQUIRE(byName[1].getNumeProprietar() == "Bogdan");
    REQUIRE(byName[2].getNumeProprietar() == "Carmen");
    REQUIRE(byName[3].getNumeProprietar() == "Mihai");

    const auto bySurface = service.sortBySuprafata();
    REQUIRE(bySurface[0].getApartament() == 3);
    REQUIRE(bySurface[3].getApartament() == 4);

    const auto byTypeSurface = service.sortByTipSiSuprafata();
    REQUIRE(byTypeSurface[0].getTipApartament() == "decomandat");
    REQUIRE(byTypeSurface[0].getSuprafata() == 42.0F);
    REQUIRE(byTypeSurface[1].getTipApartament() == "decomandat");
    REQUIRE(byTypeSurface[1].getSuprafata() == 60.0F);
    REQUIRE(byTypeSurface[2].getTipApartament() == "semidecomandat");
    REQUIRE(byTypeSurface[3].getTipApartament() == "studio");

    std::map<string, int> report;
    for (const auto& dto : service.raportTipApartament())
    {
        report[dto.tipApartament] = dto.numarAparitii;
    }
    REQUIRE(report["decomandat"] == 2);
    REQUIRE(report["semidecomandat"] == 1);
    REQUIRE(report["studio"] == 1);
}

void testUndoViaServiceAndDirectClasses()
{
    RepoLocatariVector repo;
    ServiceLocatar service(repo);
    REQUIRE(mustThrow<ServiceException>([&]() { service.undo(); }) == "Nu mai exista operatii pentru undo.");

    service.add(1, "Ana", 30.0F, "studio");
    service.update(1, "Ana Noua");
    service.remove(1);

    service.undo();
    REQUIRE(service.find(1).getNumeProprietar() == "Ana Noua");
    service.undo();
    REQUIRE(service.find(1).getNumeProprietar() == "Ana");
    service.undo();
    REQUIRE(service.getAll().empty());
    REQUIRE(mustThrow<ServiceException>([&]() { service.undo(); }) == "Nu mai exista operatii pentru undo.");

    RepoLocatariMap repoMap;
    ServiceLocatar serviceMap(repoMap);
    serviceMap.add(1, "A", 10.0F, "x");
    serviceMap.add(2, "B", 20.0F, "y");
    serviceMap.update(1, "A2");
    serviceMap.remove(2);
    serviceMap.undo();
    serviceMap.undo();
    serviceMap.undo();
    serviceMap.undo();
    REQUIRE(serviceMap.getAll().empty());

    RepoLocatariVector repoDirect;
    repoDirect.add(5, "Vechi", 44.0F, "tip");
    UndoModifica undoModifica(repoDirect, 5, "Inapoi");
    undoModifica.doUndo();
    REQUIRE(repoDirect.find(5).getNumeProprietar() == "Inapoi");
    UndoSterge undoSterge(repoDirect, Locatar(6, "Sters", 66.0F, "tip"));
    undoSterge.doUndo();
    REQUIRE(repoDirect.find(6).getNumeProprietar() == "Sters");
    UndoAdauga undoAdauga(repoDirect, 6);
    undoAdauga.doUndo();
    REQUIRE(mustThrow<RepoException>([&]() { repoDirect.find(6); }) == "Locatarul nu a fost gasit.");
}

void testServiceListaNotificare()
{
    RepoLocatariVector repo;
    ServiceLocatar service(repo);
    REQUIRE(mustThrow<ServiceException>([&]() { service.addApartamentInListaNotificare(404); }) ==
            "Apartamentul nu exista in lista principala.");
    REQUIRE(mustThrow<ServiceException>([&]() { service.genereazaListaNotificare(0); }) ==
            "Numarul de apartamente trebuie sa fie > 0.");
    REQUIRE(mustThrow<ServiceException>([&]() { service.genereazaListaNotificare(1); }) ==
            "Nu exista locatari in lista principala.");

    service.add(1, "Ana", 30.0F, "studio");
    service.add(2, "Bogdan", 50.0F, "decomandat");
    service.add(3, "Carmen", 40.0F, "studio");

    REQUIRE(mustThrow<ServiceException>([&]() { service.genereazaListaNotificare(4); }) ==
            "Nu exista suficienti locatari pentru generare.");

    service.clearListaNotificare();
    REQUIRE(service.getListaNotificareSize() == 0);
    REQUIRE(service.getListaNotificare().empty());

    service.addApartamentInListaNotificare(2);
    REQUIRE(service.getListaNotificareSize() == 1);
    REQUIRE(service.getListaNotificare()[0].getApartament() == 2);

    service.genereazaListaNotificare(2);
    REQUIRE(service.getListaNotificareSize() == 2);
    std::set<int> ids;
    for (const auto& locatar : service.getListaNotificare())
    {
        ids.insert(locatar.getApartament());
        REQUIRE(locatar.getApartament() >= 1 && locatar.getApartament() <= 3);
    }
    REQUIRE(ids.size() == 2);

    const fs::path file = tempFile("service_notificari.csv");
    service.exportListaNotificareCSV(file.string());
    REQUIRE(fs::exists(file));
    fs::remove(file);
}

void testRepoFileLoadSaveAndErrors()
{
    const fs::path missing = tempFile("missing_repo.csv");
    RepoLocatariFile emptyRepo(missing.string());
    REQUIRE(emptyRepo.getAll().empty());

    const fs::path valid = tempFile("repo_valid.csv");
    {
        std::ofstream out(valid);
        out << "\n";
        out << "1,Popescu Ion,45.5,garsoniera\n";
        out << "\n";
        out << "2,Ionescu Maria,60,decomandat\n";
    }
    RepoLocatariFile repo(valid.string());
    REQUIRE(repo.getAll().size() == 2);
    REQUIRE(repo.find(1).getNumeProprietar() == "Popescu Ion");
    REQUIRE(repo.find(2).getSuprafata() == 60.0F);

    repo.add(3, "Ana", 30.0F, "studio");
    REQUIRE(readAll(valid).find("3,Ana,30,studio\n") != string::npos);
    repo.update(3, "Ana Noua");
    REQUIRE(readAll(valid).find("3,Ana Noua,30,studio\n") != string::npos);
    repo.remove(3);
    REQUIRE(readAll(valid).find("3,Ana") == string::npos);

    ServiceLocatar service(repo);
    service.add(4, "Undo", 44.0F, "tip");
    service.update(4, "Undo Nou");
    service.undo();
    REQUIRE(readAll(valid).find("4,Undo,44,tip\n") != string::npos);

    const fs::path badLine = tempFile("repo_bad_line.csv");
    {
        std::ofstream out(badLine);
        out << "1,Popescu Ion,45.5\n";
    }
    REQUIRE(mustThrow<RepoException>([&]() { RepoLocatariFile bad(badLine.string()); }) ==
            "Linie invalida in fisierul de locatari.");

    const fs::path badInt = tempFile("repo_bad_int.csv");
    {
        std::ofstream out(badInt);
        out << "abc,Popescu Ion,45.5,garsoniera\n";
    }
    REQUIRE(mustThrow<RepoException>([&]() { RepoLocatariFile bad(badInt.string()); }) ==
            "Date invalide in fisierul de locatari.");

    const fs::path badFloat = tempFile("repo_bad_float.csv");
    {
        std::ofstream out(badFloat);
        out << "1,Popescu Ion,abc,garsoniera\n";
    }
    REQUIRE(mustThrow<RepoException>([&]() { RepoLocatariFile bad(badFloat.string()); }) ==
            "Date invalide in fisierul de locatari.");

    const fs::path duplicate = tempFile("repo_duplicate.csv");
    {
        std::ofstream out(duplicate);
        out << "1,A,10,x\n1,B,20,y\n";
    }
    REQUIRE(mustThrow<RepoException>([&]() { RepoLocatariFile bad(duplicate.string()); }) ==
            "Date invalide in fisierul de locatari.");

    const fs::path missingDirFile = testRoot() / "dir_care_nu_exista" / "repo.csv";
    fs::remove_all(missingDirFile.parent_path());
    RepoLocatariFile repoFail(missingDirFile.string());
    REQUIRE(mustThrow<RepoException>([&]() { repoFail.add(1, "Ana", 40.0F, "studio"); }) ==
            "Nu s-a putut deschide fisierul de locatari pentru scriere.");

    fs::remove_all(testRoot());
}

void runAllTests()
{
    testDomainAndExceptions();
    testValidator();
    testRepoVectorAndMap();
    testListaNotificareDirect();
    testServiceCRUDValidationAndRepoErrors();
    testServiceProbability();
    testServiceFilterSortReport();
    testUndoViaServiceAndDirectClasses();
    testServiceListaNotificare();
    testRepoFileLoadSaveAndErrors();
}

int main()
{
    runAllTests();
    std::cout << "Toate testele au trecut.\n";
    return 0;
}
