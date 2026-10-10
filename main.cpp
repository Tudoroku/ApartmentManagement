#include <QApplication>
#include "repoLocatarFile.h"
#include "gui/LocatariGUI.h"
#include "repository/repoLocatar.h"
#include "service/serviceLocatar.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    RepoLocatariFile repo("memory.txt");
    ServiceLocatar srv{repo, 0.0};

    LocatariGUI gui{srv};
    gui.show();

    return app.exec();
}