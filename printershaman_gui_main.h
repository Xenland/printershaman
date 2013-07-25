#ifndef PRINTERSHAMAN_GUI_MAIN_H
#define PRINTERSHAMAN_GUI_MAIN_H

#include <QApplication>
#include <QWidget>
#include <QDebug>
#include <QDir>
#include <QPixmap>
#include <QIcon>
#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QList>
#include <QtPrintSupport/QPrinter>
#include <QComboBox>
#include <QPrinterInfo>
#include <QLineEdit>
#include <QCheckBox>
#include <QDesktopWidget>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDateTime>
#include <QFont>
#include <QTableView>
#include <QStandardItemModel>
#include <QAbstractItemView>
#include <QTimer>
#include <QWebView>
#include <QPrintDialog>
#include <QPrinter>
#include <QSsl>
#include <iostream>     // std::cout
#include <cmath>        // std::abs

class printershaman_gui_main : public QWidget
{
    Q_OBJECT
    
public:
    printershaman_gui_main(QWidget *parent = 0);
    ~printershaman_gui_main();

    //Public General Variables
    QString version_number;
    QString current_working_directory;
    QString window_title_static;

    QTimer * loop_check_schedule_timer;

    QSqlDatabase print_schedule_db;
    QWebView * fileview_web;

    QPrinter * printer;

    //Public App Variables
    QWidget * main_window;
    QBoxLayout * main_window_layout;

        //Sub-layouts to the main_window_layout
        QWidget * main_window_layout_bootscreen;
            //Main boot screen object(s)
            QLabel * successfull_add_label;


        QWidget * main_window_layout_addNewScheduledPrintScreen;
            //Add new scheduled print screen objects
            QComboBox * available_printers_combobox;
            QLineEdit * url_to_print_input;
            QLineEdit * how_many_to_print_input;

                //Days of week
                QCheckBox * days_of_week_checkbox_monday;
                QCheckBox * days_of_week_checkbox_tuesday;
                QCheckBox * days_of_week_checkbox_wednesday;
                QCheckBox * days_of_week_checkbox_thursday;
                QCheckBox * days_of_week_checkbox_friday;
                QCheckBox * days_of_week_checkbox_saturday;
                QCheckBox * days_of_week_checkbox_sunday;

                //Months of the year
                QCheckBox * month_of_the_year_checkbox_january;
                QCheckBox * month_of_the_year_checkbox_february;
                QCheckBox * month_of_the_year_checkbox_march;
                QCheckBox * month_of_the_year_checkbox_april;
                QCheckBox * month_of_the_year_checkbox_may;
                QCheckBox * month_of_the_year_checkbox_june;
                QCheckBox * month_of_the_year_checkbox_july;
                QCheckBox * month_of_the_year_checkbox_august;
                QCheckBox * month_of_the_year_checkbox_september;
                QCheckBox * month_of_the_year_checkbox_october;
                QCheckBox * month_of_the_year_checkbox_november;
                QCheckBox * month_of_the_year_checkbox_december;

                //Hour
                QLineEdit * time_of_day_to_print_hours_input;

                //Mins
                QLineEdit * time_of_day_to_print_minutes_input;


            QWidget * main_window_layout_printingScheduleScreen;
            QGridLayout * printingSchedule_layout;
                //Add "printing schedule" screen objects.
                QTableView * printingSchedule_tableview;
                QStandardItemModel  * printingSchedule_standarditemmodel;

                //Group two buttons vertically (Remove datarow and Back to main menu)
                QWidget * group_remove_mainmenu_btn_holder;
                QVBoxLayout * group_remove_mainmenu_btn_layout;

                    //Remove row (button)
                    QPushButton * remove_row;

                    //Main menu (button)
                    QPushButton * back_to_main_menu;


    //Public Functions
    void setMainWindow_handle(QWidget * main_window_widget);
    void beginPrinterShaman();

        //(Convience) Public Functions
        void mainWindow_autoreposition();
        void printingSchedule_autoreposition();
        void addNewSchedule_autoReposition();

        //Public Functions (Major Logic)
            //Helper functions
            void hideAllMajorLayouts();

            //Main Boot Screen Functions
            void initMainBootScreen();
            void showMainBootScreen();

            //Add New Scheduled Print Screen Functions
            void init_addNewScheduledPrint_screen();
private:
        int keep_checking_schedule;

public slots:
    //Check print schedule.
            void checkPrintSchedule_loop();
            void checkPrintSchedule();
            void fileviewLoaded_now_print();

    //Main boot screen slots
            void slot_showMainBootScreen();
            void showAuthorAppDetails();

    //Add New Scheduled Print Screen signals
        //Show "add new scheduled print" screen.
            void show_addNewScheduledPrint_screen();

        //Sync newly created schedule to the database
            void sync_schedule_to_db();

    //Show "Printing Schedule" menu
        void showEditPrintingSchedule();
        void init_printingSchedule_screen();
        void slot_removeSelectedPrintingRow();
};

#endif // PRINTERSHAMAN_GUI_MAIN_H
