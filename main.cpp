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

    //THIS WILL PREVENT MATT FROM RUNNING THE PROGRAM AFTER A CERTAIN DATE.
    //Current timestamp
    QDateTime timebomb_dateTimestamp = QDateTime::currentDateTime();
    timebomb_dateTimestamp.currentDateTimeUtc();
    int timebomb_timestamp = timebomb_dateTimestamp.toTime_t();

    //If current time is more than expire time/bomb then don't run application if less then expire time run app.
        //Current expire time is: (6/8/2013) (one month from written time)
    if(timebomb_timestamp < 1370759707){
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
    }else{
        QWidget * alert_expiretime  = new QWidget();
        QGridLayout * alert_layout   = new QGridLayout();
        alert_expiretime->setLayout(alert_layout);

        QLabel *  alert_label = new QLabel("We are sorry this app has expired, please email Midnightspecter@gmail.com to renew expiration time.");
        alert_layout->addWidget(alert_label);

        alert_expiretime->show();
    }

    return a.exec();
}
