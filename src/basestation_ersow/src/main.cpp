/**
 * @file /src/main.cpp
 *
 * @brief Qt based gui.
 *
 * @date November 2010
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include <QtGui>
#include <QApplication>
#include "../include/basestation_ersow/main_window.hpp"

/*****************************************************************************
** Main
*****************************************************************************/

int main(int argc, char **argv) {

    /*********************
    ** Qt
    **********************/
    QApplication app(argc, argv);
    QFontDatabase::addApplicationFont("://images/ds-crystal_[id.allfont.net].ttf");
    basestation_ersow::MainWindow w(argc,argv);
    w.showFullScreen();

    return app.exec();
}
