cmake_minimum_required(VERSION 3.28)
project(lab6_7_qt)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

set(CORE_SOURCES
        domain/exceptions.cpp
        domain/exceptions.h
        domain/locatar.cpp
        domain/locatar.h
        domain/validatorLocatar.cpp
        domain/validatorLocatar.h

        repository/repoLocatar.cpp
        repository/repoLocatar.h
        repository/repoLocatarFile.cpp
        repository/repoLocatarFile.h

        service/listaNotificare.cpp
        service/listaNotificare.h
        service/raportTipApartamentDTO.h
        service/serviceLocatar.cpp
        service/serviceLocatar.h
        service/undo.cpp
        service/undo.h

        utils/Observer.h
        utils/Observable.h
)

set(GUI_SOURCES
        gui/LocatariGUI.cpp
        gui/LocatariGUI.h
        gui/RaportGUI.cpp
        gui/RaportGUI.h
        gui/ListaNotificareListModel.cpp
        gui/ListaNotificareListModel.h
        gui/ListaNotificareCRUDGUI.cpp
        gui/ListaNotificareCRUDGUI.h
        gui/ListaNotificareReadOnlyGUI.cpp
        gui/ListaNotificareReadOnlyGUI.h
        gui/style.h
)

add_executable(lab6_7_qt
        main.cpp
        ${CORE_SOURCES}
        ${GUI_SOURCES}
)

target_include_directories(lab6_7_qt PRIVATE
        domain
        repository
        service
        gui
        utils
)

target_link_libraries(lab6_7_qt PRIVATE Qt6::Widgets)


add_executable(run_tests
        tests/test_lab6_7.cpp
        ${CORE_SOURCES}
)

target_include_directories(run_tests PRIVATE
        domain
        repository
        service
        utils
)

target_compile_options(run_tests PRIVATE --coverage -O0 -g)
target_link_options(run_tests PRIVATE --coverage)
