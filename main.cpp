/**
    License: 2013 Copyright to Shane B. Betz
    Programmer: Shane Betz
    Date: 5/6/2013

    Purpose: To provide a printer scheduler, allow the operator to type in a url
             and have them printed at a specific time.
**/


#include "printershaman_gui_main.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

        /* Notes: Everything is taken over by the printershaman_gui_main class
         * Begin the program
         */

            //Initizalize the main printer shaman class
            QWidget * main_window_handle = new QWidget();

            printershaman_gui_main * main_window_widget = new printershaman_gui_main();
            main_window_widget->setMainWindow_handle(main_window_handle);
            main_window_widget->beginPrinterShaman();

            //Start scheduler
            main_window_widget->checkPrintSchedule_loop();


    return a.exec();
}
