/**
    License: 2013 Copyright to Shane B. Betz
    Programmer: Shane Betz
    Date: 5/6/2013

    Purpose: To provide a printer scheduler, allow the operator to type in a url
             and have them printed at a specific time.
**/

#include "printershaman_gui_main.h"

printershaman_gui_main::printershaman_gui_main(QWidget *parent)
    : QWidget(parent)
{

    //Set build variables
    version_number = "1.0.0";

    //Define the main windows title
    window_title_static = "Printer Shaman (v"+version_number+") by Xenland";

}

/**
 * @brief printershaman_gui_main::~printershaman_gui_main
 * @info
 *      This is where the class is started and ran when it is fully iniitalized.
 */
printershaman_gui_main::~printershaman_gui_main()
{
    //Do nothing
}


void printershaman_gui_main::setMainWindow_handle(QWidget * main_window_widget){
    main_window = main_window_widget;
}


void printershaman_gui_main::beginPrinterShaman(){
    //Define keep checking schedule
    keep_checking_schedule = 1;

    //Open SQLite file for querying.
    print_schedule_db = QSqlDatabase::addDatabase("QSQLITE", "schedule");
    print_schedule_db.setDatabaseName("./db/print_schedule");
    print_schedule_db.open();


    //Start webview for printing canvas
    fileview_web = new QWebView();
    QObject::connect(fileview_web, SIGNAL(loadFinished(bool)), SLOT(fileviewLoaded_now_print()));

    printer = new QPrinter;

    //Show main window
        //Create box layout
        main_window_layout = new QBoxLayout(QBoxLayout::TopToBottom);

            //Attach screens (widgets) to the main layout
                //Main Boot screen
                main_window_layout_bootscreen = new QWidget(0);
                main_window_layout->addWidget(main_window_layout_bootscreen);
                    //Init main boot screen
                    initMainBootScreen();


                //Add a new scheduled print
                main_window_layout_addNewScheduledPrintScreen = new QWidget(0);
                main_window_layout->addWidget(main_window_layout_addNewScheduledPrintScreen);
                    //Init "add a new scheduled print"
                    init_addNewScheduledPrint_screen();

                //Add "Printing Schedule" to show the printing schedule from the db file and edit it.
                main_window_layout_printingScheduleScreen = new QWidget(0);
                main_window_layout->addWidget(main_window_layout_printingScheduleScreen);
                    //Init "show printing schedule"
                    init_printingSchedule_screen();


        //Attach layout to main window
        main_window->setLayout(main_window_layout);

        //Show main window now that a layout is attached.
        main_window->show();


    //Output Initailziation of class/app
    qDebug() << "---------------------------";
    qDebug() << "Printer Shaman v"+version_number;
    qDebug() << "Helping you print on schedule!";
    qDebug() << "---------------------------";

    //Define the main windows title
    main_window->setWindowTitle("Booting Up | "+window_title_static);

    //Set window icon options
    QString main_window_icon_dir_text       = ":/images/icon_printer_shaman_logo_app_size";
    QImage main_window_icon_images          = QImage(main_window_icon_dir_text);
    QIcon main_window_icon                  = QIcon(QPixmap::fromImage(main_window_icon_images));
    main_window->setWindowIcon(main_window_icon);

    //Now call the "Show main boot screen" function to show the main boot screen.
    showMainBootScreen();
}


/* ** ** ** ** **
 *  Major Logic Functions
 * ** ** ** ** **/
        /** ** ** ** ** ** ** **
         ** Helper Functions  **/
        void printershaman_gui_main::hideAllMajorLayouts(){
                //Hide main menu screen
                main_window_layout_bootscreen->hide();

                //Hide "Add new scheduled print"
                main_window_layout_addNewScheduledPrintScreen->hide();

                //Hide "Printing Schedule"
                main_window_layout_printingScheduleScreen->hide();
        }

        /** ** ** ** ** ** ** **
         ** Main Boot Screen  **/

            //This will initizalise the main boot screens "skeleton"
            void printershaman_gui_main::initMainBootScreen(){
                QGridLayout * bootscreen_layout = new QGridLayout();
                main_window_layout_bootscreen->setLayout(bootscreen_layout);
                main_window_layout_bootscreen->hide();


                //Add stuff to the main boot menu screen
                    /** ** ** ** ** ** ** ** ** ** **
                     ** "Successfull Add Message"  **/
                        successfull_add_label = new QLabel("");
                        bootscreen_layout->addWidget(successfull_add_label, 0, 0, 1, 1);
                        successfull_add_label->hide();
                        successfull_add_label->setStyleSheet("background-color:#dff0d8;color:#468847;font-weight:bold;text-align:center;");

                    /** ** ** ** ** ** ** ** ** **
                     **  "Printer Shaman" Logo  **/
                        QLabel * logo_imagelabel    = new QLabel();
                        QString logo_dir_text       = ":/images/printer_shaman_logo_app_size";
                        QImage logo_image(logo_dir_text);

                        if(logo_image.isNull() == true){
                            qWarning() << "Logo image couldn't be found";
                        }else{
                            logo_imagelabel->setPixmap(QPixmap::fromImage(logo_image));
                        }

                        //Add logo to the main window (layout)
                        bootscreen_layout->addWidget(logo_imagelabel, 1, 0, 3, 1);


                    /** ** ** ** ** ** ** ** **
                     * Button "Add new scheduled print"
                     ** ** ** ** ** ** ** ** **/
                        QPushButton * new_scheduled_print_button = new QPushButton("Add a new scheduled print");

                            //Set icon
                            /*QString show_printer_controls_icon_text = current_working_directory+"/images/icon_show_printer_controls.png";
                            QImage show_printer_controls_icon_image(show_printer_controls_icon_text);
                            show_printer_controls->setIcon(QPixmap::fromImage(show_printer_controls_icon_image));
                            */

                            //Set sizing
                            new_scheduled_print_button->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

                            //Add button to the main window (layout)
                            bootscreen_layout->addWidget(new_scheduled_print_button, 0, 1, 2, 1);

                            //Make connections to this button
                            QObject::connect(new_scheduled_print_button, SIGNAL(clicked()), this, SLOT(show_addNewScheduledPrint_screen()));


                    /** ** ** ** ** ** ** ** **
                     * Button "Schedule Printing Options"
                     ** ** ** ** ** ** ** ** **/
                        QPushButton * show_printer_controls_button = new QPushButton("Printing Schedule");

                            //Set sizing
                            show_printer_controls_button->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

                            //Add button to the main window (layout)
                            bootscreen_layout->addWidget(show_printer_controls_button, 2, 1);

                            //Make connections to this button
                            QObject::connect(show_printer_controls_button, SIGNAL(clicked()), this, SLOT(showEditPrintingSchedule()));




                    /** ** ** ** ** ** ** ** **
                     * Button "Application Details"
                     ** ** ** ** ** ** ** ** **/
                        QPushButton * show_application_details = new QPushButton("Show Author and Application Details");
                                       // show_application_details->
                            //Set icon
                            /*QString show_printer_controls_icon_text = current_working_directory+"/images/icon_show_printer_controls.png";
                            QImage show_printer_controls_icon_image(show_printer_controls_icon_text);
                            show_printer_controls->setIcon(QPixmap::fromImage(show_printer_controls_icon_image));
                            */

                            //Set sizing
                            show_application_details->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

                            //Add button to the main window (layout)
                            bootscreen_layout->addWidget(show_application_details, 3, 1);

                            //Make connections to this button
                            //QObject::connect(show_printer_controls, SIGNAL(clicked()), printer_scheduling_controls_handle, SLOT(showSchedulingControls()));



            }


            //This will reset the main boot screen to the init phase (with out initalizing again)
            void printershaman_gui_main::showMainBootScreen(){
                //Hide all screens here
                hideAllMajorLayouts();

                qDebug() << "MAIN BOOT SCREEN";


                //Show main boot screen layout.
                main_window_layout_bootscreen->show();

                //Set window title
                main_window->setWindowTitle("Main Menu | "+window_title_static);

                //Reposition
                mainWindow_autoreposition();
            }



         void printershaman_gui_main::slot_showMainBootScreen(){
             showMainBootScreen();
         }


         void printershaman_gui_main::mainWindow_autoreposition(){
             //Set Size & Position options
                 //Get screen size.
                 QDesktopWidget * desktop_information     = QApplication::desktop();
                 QRect client_screen_geometry_info       = desktop_information->availableGeometry();

                 int screen_width    = client_screen_geometry_info.width();
                 int screen_height   = client_screen_geometry_info.height();

                 int app_width   = 600;
                 int app_height  = 200;

                 //Move window into middle of screen (proportionatly as possible)
                             //(screen width / 2) - (app width / 2)
                 int app_new_x   = (screen_width/2) - (app_width/2);
                 int app_new_y   = (screen_height/2) - (app_height/2);

                     //If the new y(+app height) is greater than window height make it snap to the window bottom
                     if((app_new_y+app_height) >= screen_height){
                         app_new_y = 0;
                     }

             main_window->resize(app_width, app_height);
             main_window->move(app_new_x, app_new_y);
         }

         void printershaman_gui_main::printingSchedule_autoreposition(){
             //Set Size & Position options
                 //Get screen size.
                 QDesktopWidget * desktop_information     = QApplication::desktop();
                 QRect client_screen_geometry_info       = desktop_information->availableGeometry();

                 int screen_width    = client_screen_geometry_info.width();
                 int screen_height   = client_screen_geometry_info.height();

                 int app_width   = 800;
                 int app_height  = 300;

                 //Move window into middle of screen (proportionatly as possible)
                 //(screen width / 2) - (app width / 2)
                 int app_new_x   = (screen_width/2) - (app_width/2);
                 int app_new_y   = (screen_height/2) - (app_height/2);

                     //If the new y(+app height) is greater than window height make it snap to the window bottom
                     if((app_new_y+app_height) >= screen_height){
                         app_new_y = 0;
                     }

             main_window->resize(app_width, app_height);
             main_window->move(app_new_x, app_new_y);
         }

         void printershaman_gui_main::addNewSchedule_autoReposition(){
             //Set Size & Position options
                 //Get screen size.
                 QDesktopWidget * desktop_information     = QApplication::desktop();
                 QRect client_screen_geometry_info       = desktop_information->availableGeometry();

                 int screen_width    = client_screen_geometry_info.width();
                 int screen_height   = client_screen_geometry_info.height();

                 int app_width   = 800;
                 int app_height  = 500;

                 //Move window into middle of screen (proportionatly as possible)
                 //(screen width / 2) - (app width / 2)
                 int app_new_x   = (screen_width/2) - (app_width/2);
                 int app_new_y   = (screen_height/2) - (app_height/2);

                     //If the new y(+app height) is greater than window height make it snap to the window bottom
                     if((app_new_y+app_height) >= screen_height){
                         app_new_y = 0;
                     }

             main_window->resize(app_width, app_height);
             main_window->move(app_new_x, app_new_y);
         }



        /** ** ** ** ** ** ** ** ** ** **
         **   Add New Scheduled Print  **/
        void printershaman_gui_main::init_addNewScheduledPrint_screen(){
            QGridLayout * addNewScheduledPrint_layout = new QGridLayout();
            main_window_layout_addNewScheduledPrintScreen->setLayout(addNewScheduledPrint_layout);
            main_window_layout_addNewScheduledPrintScreen->hide();

                /** ** ** ** ** ** ** ** ** ** ** ** ** **
                 **  "Printer Shaman" Add New Schedule  **/
                    QLabel * logo_imagelabel    = new QLabel();
                    QString logo_dir_text       = ":/images/add_new_schedule";
                    QImage logo_image(logo_dir_text);

                    if(logo_image.isNull() == true){
                        qWarning() << "Logo image couldn't be found";
                    }else{
                        logo_imagelabel->setPixmap(QPixmap::fromImage(logo_image));
                    }

                    //Add logo to the main window (layout)
                    addNewScheduledPrint_layout->addWidget(logo_imagelabel, 0, 0, 6, 1);



                /** ** ** ** ** ** ** ** **
                 * Label "select a printer you want to print from"
                 ** ** ** ** ** ** ** ** **/
                QLabel * add_new_scheduled_print_header_label = new QLabel("Which printer will be desired at the scheduled time(s)?");
                QFont add_new_scheduled_print_header_label_font_style("Arial", 12);
                add_new_scheduled_print_header_label->setFont(add_new_scheduled_print_header_label_font_style);
                addNewScheduledPrint_layout->addWidget(add_new_scheduled_print_header_label, 0, 1);


                /** ** ** ** ** ** ** ** **
                 * List available printers, to choose from.
                 ** ** ** ** ** ** ** ** **/
                    //We only query for available printers "per" show "add schedule" screen, so we just have the skeleton for displaying the printer selection.

                        /** ** ** ** ** ** ** ** **
                         * Combobox place holder for available printers.
                         ** ** ** ** ** ** ** ** **/
                         available_printers_combobox = new QComboBox();
                         addNewScheduledPrint_layout->addWidget(available_printers_combobox, 1, 1);

                            //Style available printers combo box gui
                            QFont available_printers_combobox_font_style("Arial", 12);
                            available_printers_combobox->setFont(available_printers_combobox_font_style);
                            available_printers_combobox->setMinimumHeight(30);

                /** ** ** ** ** ** ** ** **
                 * Label Ask how many copies
                 ** ** ** ** ** ** ** ** **/
                QLabel * how_many_to_print = new QLabel("How many copies to print?");
                QFont how_many_to_print_font("Arial", 12);
                how_many_to_print->setFont(how_many_to_print_font);
                how_many_to_print->setStyleSheet("margin-top:1em;");
                addNewScheduledPrint_layout->addWidget(how_many_to_print, 2, 1);

                /** ** ** ** ** ** ** ** **
                 * Input Ask user what url they'd like printed out.
                 ** ** ** ** ** ** ** ** **/
                how_many_to_print_input = new QLineEdit();
                addNewScheduledPrint_layout->addWidget(how_many_to_print_input, 3, 1);

                /** ** ** ** ** ** ** ** **
                 * Label Ask user what url they'd like printed out.
                 ** ** ** ** ** ** ** ** **/
                QLabel * url_to_print_label = new QLabel("What url (or file location) did you want to print?");
                QFont url_to_print_label_font_style("Arial", 12);
                url_to_print_label->setFont(url_to_print_label_font_style);
                url_to_print_label->setStyleSheet("margin-top:1em;");
                addNewScheduledPrint_layout->addWidget(url_to_print_label, 4, 1);



                /** ** ** ** ** ** ** ** **
                 * Input Ask user what url they'd like printed out.
                 ** ** ** ** ** ** ** ** **/
                url_to_print_input = new QLineEdit();
                addNewScheduledPrint_layout->addWidget(url_to_print_input, 5, 1);


                /** ** ** ** ** ** ** ** **
                 * Label "Which days do you want this to print at?"
                 ** ** ** ** ** ** ** ** **/
                QLabel * days_of_the_week_to_print_label = new QLabel("Days of the week to print.");
                QFont days_of_the_week_to_print_label_font_style("Arial", 12);
                days_of_the_week_to_print_label->setFont(days_of_the_week_to_print_label_font_style);
                days_of_the_week_to_print_label->setStyleSheet("margin-top:1em;");
                addNewScheduledPrint_layout->addWidget(days_of_the_week_to_print_label, 6, 1);

                /** ** ** ** ** ** ** ** **
                 * Input Checkboxes (which days do you want the printer to print this url?)
                 ** ** ** ** ** ** ** ** **/
                QWidget * days_of_week_checkbox_widget = new QWidget();
                QGridLayout * days_of_week_checkbox_grid = new QGridLayout();
                days_of_week_checkbox_widget->setLayout(days_of_week_checkbox_grid);
                addNewScheduledPrint_layout->addWidget(days_of_week_checkbox_widget, 7, 1);
                    //Add checkboxes to the layout/grid
                    days_of_week_checkbox_monday = new QCheckBox("Monday");
                    days_of_week_checkbox_grid->addWidget(days_of_week_checkbox_monday, 0, 0);

                    days_of_week_checkbox_tuesday = new QCheckBox("Tuesday");
                    days_of_week_checkbox_grid->addWidget(days_of_week_checkbox_tuesday, 0, 1);

                    days_of_week_checkbox_wednesday = new QCheckBox("Wednesday");
                    days_of_week_checkbox_grid->addWidget(days_of_week_checkbox_wednesday, 0, 2);

                    days_of_week_checkbox_thursday = new QCheckBox("Thursday");
                    days_of_week_checkbox_grid->addWidget(days_of_week_checkbox_thursday, 0, 3);

                    days_of_week_checkbox_friday = new QCheckBox("Friday");
                    days_of_week_checkbox_grid->addWidget(days_of_week_checkbox_friday, 0, 4);

                    days_of_week_checkbox_saturday = new QCheckBox("Saturday");
                    days_of_week_checkbox_grid->addWidget(days_of_week_checkbox_saturday, 1, 0);

                    days_of_week_checkbox_sunday = new QCheckBox("Sunday");
                    days_of_week_checkbox_grid->addWidget(days_of_week_checkbox_sunday, 1, 1);



                /** ** ** ** ** ** ** ** **
                 * Label which months do you want the printer to print this url
                 ** ** ** ** ** ** ** ** **/
                QLabel * months_of_the_year_print_label = new QLabel("Months of the year to print.");
                QFont months_of_the_year_print_label_font_style("Arial", 12);
                months_of_the_year_print_label->setFont(months_of_the_year_print_label_font_style);
                months_of_the_year_print_label->setStyleSheet("margin-top:1em;");
                addNewScheduledPrint_layout->addWidget(months_of_the_year_print_label, 6, 0);


                /** ** ** ** ** ** ** ** **
                 * Input Checkboxes (which months do you want the printer to print this url?)
                 ** ** ** ** ** ** ** ** **/
                QWidget * months_of_the_year_checkbox_widget = new QWidget();
                QGridLayout * months_of_the_year_checkbox_grid = new QGridLayout();
                months_of_the_year_checkbox_widget->setLayout(months_of_the_year_checkbox_grid);
                addNewScheduledPrint_layout->addWidget(months_of_the_year_checkbox_widget, 7, 0);

                    //Add checkboxes to the layout/grid
                    month_of_the_year_checkbox_january = new QCheckBox("January");
                    months_of_the_year_checkbox_grid->addWidget(month_of_the_year_checkbox_january, 0, 0);

                    month_of_the_year_checkbox_february = new QCheckBox("February");
                    months_of_the_year_checkbox_grid->addWidget(month_of_the_year_checkbox_february, 0, 1);

                    month_of_the_year_checkbox_march = new QCheckBox("March");
                    months_of_the_year_checkbox_grid->addWidget(month_of_the_year_checkbox_march, 0, 2);

                    month_of_the_year_checkbox_april = new QCheckBox("April");
                    months_of_the_year_checkbox_grid->addWidget(month_of_the_year_checkbox_april, 0, 3);

                    month_of_the_year_checkbox_may = new QCheckBox("May");
                    months_of_the_year_checkbox_grid->addWidget(month_of_the_year_checkbox_may, 0, 4);

                    month_of_the_year_checkbox_june = new QCheckBox("June");
                    months_of_the_year_checkbox_grid->addWidget(month_of_the_year_checkbox_june, 1, 0);

                    month_of_the_year_checkbox_july = new QCheckBox("July");
                    months_of_the_year_checkbox_grid->addWidget(month_of_the_year_checkbox_july, 1, 1);

                    month_of_the_year_checkbox_august = new QCheckBox("August");
                    months_of_the_year_checkbox_grid->addWidget(month_of_the_year_checkbox_august, 1, 2);

                    month_of_the_year_checkbox_september = new QCheckBox("September");
                    months_of_the_year_checkbox_grid->addWidget(month_of_the_year_checkbox_september, 1, 3);

                    month_of_the_year_checkbox_october = new QCheckBox("October");
                    months_of_the_year_checkbox_grid->addWidget(month_of_the_year_checkbox_october, 1, 4);

                    month_of_the_year_checkbox_november = new QCheckBox("November");
                    months_of_the_year_checkbox_grid->addWidget(month_of_the_year_checkbox_november, 2, 0);

                    month_of_the_year_checkbox_december = new QCheckBox("December");
                    months_of_the_year_checkbox_grid->addWidget(month_of_the_year_checkbox_december, 2, 1);


                /** ** ** ** ** ** ** ** **
                 * Label (what time did you want to print?)
                 ** ** ** ** ** ** ** ** **/
                QLabel * time_of_day_to_print_label = new QLabel("What time should the printing begin?");
                QFont time_of_day_to_print_label_font_style("Arial", 12);
                time_of_day_to_print_label->setFont(time_of_day_to_print_label_font_style);
                time_of_day_to_print_label->setStyleSheet("margin-top:1em;");
                addNewScheduledPrint_layout->addWidget(time_of_day_to_print_label, 8, 1);


                /** ** ** ** ** ** ** ** **
                 * Input (Ask user what time they want to print.)
                 ** ** ** ** ** ** ** ** **/
                QWidget * time_of_day_to_print_widget       = new QWidget();
                QGridLayout * time_of_day_to_print_layout   = new QGridLayout();
                time_of_day_to_print_widget->setLayout(time_of_day_to_print_layout);
                addNewScheduledPrint_layout->addWidget(time_of_day_to_print_widget, 9, 1);

                    //Add hours input to grid
                    time_of_day_to_print_hours_input = new QLineEdit("Hours");
                    time_of_day_to_print_layout->addWidget(time_of_day_to_print_hours_input, 0, 0);

                    //Add minutes input to grid
                    time_of_day_to_print_minutes_input = new QLineEdit("Minutes");
                    time_of_day_to_print_layout->addWidget(time_of_day_to_print_minutes_input, 0, 1);



                /** ** ** ** ** ** ** ** **
                 * Button(s) "Add schedule" & "Cancel Add"
                 ** ** ** ** ** ** ** ** **/
                    QWidget * add_cancel_button_group_widget = new QWidget();
                    QGridLayout * add_cancel_button_group_grid = new QGridLayout();
                    add_cancel_button_group_widget->setLayout(add_cancel_button_group_grid);
                    addNewScheduledPrint_layout->addWidget(add_cancel_button_group_widget, 8, 0, 2, 1);

                        QPushButton * add_schedule_button = new QPushButton("Add Schedule");
                        add_schedule_button->setStyleSheet("QPushButton{padding:1em;}");
                        add_schedule_button->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

                            //Add button to the add_cancel_button_group_grid (layout)
                            add_cancel_button_group_grid->addWidget(add_schedule_button, 0, 0);

                            //Connect "clicked" to function
                            connect(add_schedule_button, SIGNAL(clicked()), this, SLOT(sync_schedule_to_db()));

                        QPushButton * cancel_add_schedule_button = new QPushButton("Cancel");
                        cancel_add_schedule_button->setStyleSheet("QPushButton{padding:1em;}");
                        cancel_add_schedule_button->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

                            //Add button to the add_cancel_button_group_grid (layout)
                            add_cancel_button_group_grid->addWidget(cancel_add_schedule_button, 1, 0);

                            //Connect "clicked" to function
                            connect(cancel_add_schedule_button, SIGNAL(clicked()), this, SLOT(slot_showMainBootScreen()));
        }




        /**
         * @brief printershaman_gui_main::show_addNewScheduledPrint_screen
         */
        void printershaman_gui_main::show_addNewScheduledPrint_screen(){
            //Hide all layouts
            hideAllMajorLayouts();

            //Set title
            main_window->setWindowTitle("Add new scheduled print | "+window_title_static);

            //Show the "add new scheduled print screen" layout.
            main_window_layout_addNewScheduledPrintScreen->show();

                //refresh/prerender nessecary things
                    /** ** ** ** ** ** ** ** **
                     * List available printers, to choose from.
                     ** ** ** ** ** ** ** ** **/
                    QPrinterInfo * get_available_printers = new QPrinterInfo();
                    QList<QPrinterInfo> available_printers_list;
                    available_printers_list.append(QPrinterInfo::availablePrinters());

                    //Go through available printer list and add to the combobox.
                        //Clear combobox
                        for(int a = available_printers_combobox->count(); a > 0 ; a--){
                            available_printers_combobox->removeItem(a);
                        }

                        //Populate combobox
                        for (int i = 0; i < available_printers_list.size(); ++i) {
                            int is_default_printer = 0;

                            QString printer_title_string = available_printers_list.at(i).printerName();


                                //If this is the default printer append "(DEFAULT)" to it
                                if(printer_title_string == get_available_printers->defaultPrinter().printerName()){
                                    is_default_printer = 1;

                                    printer_title_string = printer_title_string+" (Default) ";
                                }

                            //Add string data row to the combobox
                            available_printers_combobox->addItem(printer_title_string);
                                //Set selected if default
                                if(is_default_printer){
                                    available_printers_combobox->setCurrentIndex(i);
                                }
                        }

            //Reposition
            addNewSchedule_autoReposition();
        }


        /** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **
         **   SLOT, "Sync" new schedule to the database  **/
        void printershaman_gui_main::sync_schedule_to_db(){

            /** Make sure all the required fields are at minimum filled out, then confirm or alert the user of success or failure **/

            if(print_schedule_db.isOpen()){
                //Create query string
                QString query_string = "";
                query_string += "INSERT INTO `schedule_index`";
                query_string += " (`id`, `timestamp_created`, `printer_name`, `url_location`, `day_mon`, `day_tue`, `day_wed`, `day_thur`, `day_fri`, `day_sat`, `day_sun`, `month_jan`, `month_feb`, `month_march`, `month_april`, `month_may`, `month_june`, `month_july`, `month_aug`, `month_sep`, `month_oct`, `month_nov`, `month_dec`, `hour`, `minutes`, `num_of_copies`)";
                query_string += " values(NULL, '%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9', '%10', '%11', '%12', '%13', '%14', '%15', '%16', '%17', '%18', '%19', '%20', '%21', '%22', '%23','%24','%25')";


                    //Get variables and convert them for SQL insert.
                        //Current timestamp
                        QDateTime query_dateTimestamp = QDateTime::currentDateTime();
                        query_dateTimestamp.currentDateTimeUtc();
                        int query_miliTimestamp = query_dateTimestamp.toTime_t();
                        int query_secondsTimestamp = query_miliTimestamp;

                        //Printer selected title.
                        QString printer_selected_title = available_printers_combobox->currentText();
                            //Remove the "(Default)" if nessecary.
                            printer_selected_title.replace("(Default)", "");

                        //Url/Filelocation
                        QString url_location_string = url_to_print_input->displayText();

                        //Days of the week
                            int days_of_week_valid = 0;
                            int day_monday      = days_of_week_checkbox_monday->isChecked();
                            int day_tuesday     = days_of_week_checkbox_tuesday->isChecked();
                            int day_wednesday   = days_of_week_checkbox_wednesday->isChecked();
                            int day_thursday    = days_of_week_checkbox_thursday->isChecked();
                            int day_friday      = days_of_week_checkbox_friday->isChecked();
                            int day_saturday    = days_of_week_checkbox_saturday->isChecked();
                            int day_sunday      = days_of_week_checkbox_sunday->isChecked();

                                //Check if day of the week is valid.
                                if(day_monday == 1 || day_tuesday == 1 || day_wednesday == 1 || day_thursday == 1 || day_friday == 1 || day_saturday == 1 || day_sunday == 1){
                                     days_of_week_valid = 1;
                                }

                       //Months of the year
                            int months_of_year_valid = 0;
                            int month_january   = month_of_the_year_checkbox_january->isChecked();
                            int month_feburary  = month_of_the_year_checkbox_february->isChecked();
                            int month_march     = month_of_the_year_checkbox_march->isChecked();
                            int month_april     = month_of_the_year_checkbox_april->isChecked();
                            int month_may       = month_of_the_year_checkbox_may->isChecked();
                            int month_june      = month_of_the_year_checkbox_june->isChecked();
                            int month_july      = month_of_the_year_checkbox_july->isChecked();
                            int month_august    = month_of_the_year_checkbox_august->isChecked();
                            int month_sep       = month_of_the_year_checkbox_september->isChecked();
                            int month_oct       = month_of_the_year_checkbox_october->isChecked();
                            int month_nov       = month_of_the_year_checkbox_november->isChecked();
                            int month_dec       = month_of_the_year_checkbox_december->isChecked();

                                //Check if months of the year is valid.
                                if(month_january == 1 || month_feburary == 1 || month_march == 1 || month_april == 1 || month_may == 1 || month_june == 1 || month_july == 1 || month_august == 1 || month_sep == 1 || month_oct == 1 || month_nov == 1 || month_dec == 1){
                                    months_of_year_valid = 1;
                                }

                       //Hour
                            int hour_military   =  time_of_day_to_print_hours_input->displayText().toInt();

                       //Minute
                            int minutes         = time_of_day_to_print_minutes_input->displayText().toInt();

                       //Num of copies
                            int num_of_copies   = how_many_to_print_input->displayText().toInt();

                //Check if everything is valid before adding to the schedule
                if(months_of_year_valid == 1 && days_of_week_valid == 1 && num_of_copies >= 1 && minutes >= 0 && hour_military >= 0){
                    QSqlQuery insert_query(print_schedule_db);
                    bool insert_success = false;
                    insert_success = insert_query.exec(
                                    query_string
                                    .arg(query_secondsTimestamp)
                                    .arg(printer_selected_title)
                                    .arg(url_location_string)
                                    .arg(day_monday)
                                    .arg(day_tuesday)
                                    .arg(day_wednesday)
                                    .arg(day_thursday)
                                    .arg(day_friday)
                                    .arg(day_saturday)
                                    .arg(day_sunday)
                                    .arg(month_january)
                                    .arg(month_feburary)
                                    .arg(month_march)
                                    .arg(month_april)
                                    .arg(month_may)
                                    .arg(month_june)
                                    .arg(month_july)
                                    .arg(month_august)
                                    .arg(month_sep)
                                    .arg(month_oct)
                                    .arg(month_nov)
                                    .arg(month_dec)
                                    .arg(hour_military)
                                    .arg(minutes)
                                    .arg(num_of_copies)
                                );
                    if(insert_success == true){
                        //Set newly scheduled print
                        successfull_add_label->setText("Your scheduled print has been added!");
                        successfull_add_label->setFixedHeight(35);
                            //Create new font style
                            QFont successfull_add_label_font_style("Arial", 15);

                        successfull_add_label->setFont(successfull_add_label_font_style);
                        successfull_add_label->show();

                        //Show main boot screen
                        showMainBootScreen();
                    }
                }else{
                    //Input error happened, alert the user that they need some more information to fill out.
                    QDialog * input_error = new QDialog(0);
                    QVBoxLayout * input_error_grid = new QVBoxLayout(0);
                    input_error->setLayout(input_error_grid);

                    //Add labels per error message
                    if(days_of_week_valid == 0){
                        //Add days of week error
                        QLabel * days_of_week_error = new QLabel("Please check atleast one day of the week.");
                        input_error_grid->addWidget(days_of_week_error);
                    }
                    if(months_of_year_valid == 0){
                        //Add months of the year error
                        QLabel * months_of_the_year_error = new QLabel("Please check at least one month of the year.");
                        input_error_grid->addWidget(months_of_the_year_error);
                    }
                    if(num_of_copies <= 0){
                        //Add num of copies error
                        QLabel * num_of_copies_error = new QLabel("Please type in the number of copies you wish to have printed.");
                        input_error_grid->addWidget(num_of_copies_error);
                    }
                    if(hour_military <= 0 || hour_military >= 24){
                        //Add hour error
                        QLabel * hour_error = new QLabel("Please type in the hour in military time (add 12 for PM)");
                        input_error_grid->addWidget(hour_error);
                    }
                    if(minutes <= 0 || minutes >= 60){
                        //Add minutes error
                        QLabel * minutes_error = new QLabel("Please type in a minute that is less then 60 or greater or equal to zero");
                        input_error_grid->addWidget(minutes_error);
                    }

                    //Show error dialog box
                    input_error->show();
                }
            }else{
                qDebug() << "DB NOT OPEN";
            }
        }

        /** ** ** ** ** ** ** ** ** ** ** ** **
         ** SLOT, "Init Printing Schedule"   **/
        void printershaman_gui_main::init_printingSchedule_screen(){
            printingSchedule_layout = new QGridLayout();
            main_window_layout_printingScheduleScreen->setLayout(printingSchedule_layout);

            //Attach table view.
            printingSchedule_tableview = new QTableView();
                printingSchedule_tableview->setEditTriggers(QAbstractItemView::NoEditTriggers);

            printingSchedule_standarditemmodel = new QStandardItemModel(0, 6);
                printingSchedule_standarditemmodel->setHorizontalHeaderItem(0, new QStandardItem(QString("#")));
                printingSchedule_standarditemmodel->setHorizontalHeaderItem(1, new QStandardItem(QString("Printer Name")));
                printingSchedule_standarditemmodel->setHorizontalHeaderItem(2, new QStandardItem(QString("Copies")));
                printingSchedule_standarditemmodel->setHorizontalHeaderItem(3, new QStandardItem(QString("URL/File Location")));
                printingSchedule_standarditemmodel->setHorizontalHeaderItem(4, new QStandardItem(QString("Daily Time")));
                printingSchedule_standarditemmodel->setHorizontalHeaderItem(5, new QStandardItem(QString("Months of the year")));
                printingSchedule_standarditemmodel->setHorizontalHeaderItem(5, new QStandardItem(QString("Days of the week")));


                printingSchedule_tableview->setModel(printingSchedule_standarditemmodel);

                //Tableview properties
                printingSchedule_tableview->resizeColumnsToContents();

            printingSchedule_layout->addWidget(printingSchedule_tableview, 0,0, 1,1, Qt::AlignTop);

            //Group "remove" and "Back to main menu" button in a VBox
            group_remove_mainmenu_btn_holder = new QWidget(0);
            group_remove_mainmenu_btn_layout = new QVBoxLayout(0);
            group_remove_mainmenu_btn_holder->setLayout(group_remove_mainmenu_btn_layout);
            printingSchedule_layout->addWidget(group_remove_mainmenu_btn_holder, 0,1, 1,1, Qt::AlignTop);

                //Add "Remove" button
                remove_row = new QPushButton("Remove Selected Row");
                remove_row->setStyleSheet("QPushButton{padding:1em;}");
                group_remove_mainmenu_btn_layout->addWidget(remove_row);

                QObject::connect(remove_row, SIGNAL(clicked()), this, SLOT(slot_removeSelectedPrintingRow()));


                //Add "Main Menu" button
                back_to_main_menu = new QPushButton("Back to Main Menu");
                back_to_main_menu->setStyleSheet("QPushButton{padding:1em;}");
                group_remove_mainmenu_btn_layout->addWidget(back_to_main_menu);

            QObject::connect(back_to_main_menu, SIGNAL(clicked()), this, SLOT(slot_showMainBootScreen()));

            //Hide
            main_window_layout_printingScheduleScreen->hide();
        }

        /** ** ** ** ** ** ** ** ** ** ** ** **
         **   SLOT, Show "Printing Schedule" **/
        void printershaman_gui_main::showEditPrintingSchedule(){
            qDebug() << "SHOWING PRINT SCHEDULE";

            //Hide all major layouts.
            hideAllMajorLayouts();

            //Show printing schedule screen
            main_window_layout_printingScheduleScreen->show();

            //Set title
            main_window->setWindowTitle("Printing Schedule | "+window_title_static);

            /** Pre render printing schedule **/
                //Query printing schedule from the sqlite database.

                    if(print_schedule_db.isOpen()){
                        qDebug() << "DB IS OPENED";
                        //Clear table view
                        //printingSchedule_standarditemmodel->removeRows(0, printingSchedule_standarditemmodel->rowCount(), QModelIndex());

                        QString query_string = "SELECT `id`, `timestamp_created`, `num_of_copies`, `printer_name`, `url_location`, `day_mon`, `day_tue`, `day_wed`, `day_thur`, `day_fri`, `day_sat`, `day_sun`, `month_jan`, `month_feb`, `month_march`, `month_april`, `month_may`, `month_june`, `month_july`, `month_aug`, `month_sep`, `month_oct`, `month_nov`, `month_dec`, `hour`, `minutes` FROM `schedule_index` ORDER BY `id` ASC";
                        QSqlQuery select_query(print_schedule_db);
                        bool select_success = false;
                        select_success = select_query.exec(query_string);
                        qDebug() << "SELECT ERROR (IF ANY): " << select_query.lastError();
                        qDebug() << select_success;

                        if(select_success == true){
                            while(select_query.next()){
                                qDebug() << "---------------------------";
                                qDebug() << select_query.value(0).toInt();
                                qDebug() << select_query.value(1).toString();

                                QList<QStandardItem *> list_of_schedule_details;
                                    //Item id
                                    QStandardItem * item_id = new QStandardItem(select_query.value("id").toString());
                                    list_of_schedule_details.append(item_id);

                                    //Printername
                                    QStandardItem * printername = new QStandardItem(select_query.value("printer_name").toString());
                                    list_of_schedule_details.append(printername);

                                    //Num of Copies
                                    QStandardItem * num_of_copies = new QStandardItem(select_query.value("num_of_copies").toString());
                                    list_of_schedule_details.append(num_of_copies);

                                    //URL Location
                                    QStandardItem * url_location = new QStandardItem(select_query.value("url_location").toString());
                                    list_of_schedule_details.append(url_location);

                                    //Daily Time
                                        //Get hour and minutes and concat like (hh:mm)
                                        QString dailyTime_string(select_query.value("hour").toString()+":"+select_query.value("minutes").toString());

                                    QStandardItem * dailytime = new QStandardItem(dailyTime_string);
                                    list_of_schedule_details.append(dailytime);

                                    //Days of the week

                                printingSchedule_standarditemmodel->appendRow(list_of_schedule_details);
                            }
                         }else{
                            qDebug() << "FAILED TO QUERY FOR SCHEDULE!!!";
                        }
                    }else{
                        qDebug() << "DB IS NOT OPENED";
                        //Display a popup message notifying that the database can't be found at this time.
                        QDialog * database_not_found_dialog = new QDialog(0);
                        database_not_found_dialog->setWindowTitle("Database Load Error | Printer Shaman");
                        database_not_found_dialog->setModal(1);
                        QGridLayout * database_not_found_layout = new QGridLayout(0);
                        database_not_found_dialog->setLayout(database_not_found_layout);

                            //Add label displaying the error message
                            QLabel * error_mesage = new QLabel("<span style='color:red;'><h3>Database Error</h3></span>We are sorry, The database could not be loaded at this time.<br/>Please, notify the author.");
                            database_not_found_layout->addWidget(error_mesage, 0,0, 1,1);

                       //Show error message
                            database_not_found_dialog->show();
                    }

            //Reposition
            printingSchedule_autoreposition();
        }

        void printershaman_gui_main::checkPrintSchedule_loop(){
                loop_check_schedule_timer = new QTimer();
                connect(loop_check_schedule_timer, SIGNAL(timeout()), this, SLOT(checkPrintSchedule()));
                loop_check_schedule_timer->start(1000);
        }

        void printershaman_gui_main::checkPrintSchedule(){
            //Query print schedule
            if(print_schedule_db.isOpen()){
                //Current timestamp as QDateTime("Mon May 13 19:58:28 2013")
                QDateTime current_dateTimestamp   = QDateTime::currentDateTime();

                    //Month as 2 digit int (with out leading zeros as most human inputs don't do leading zeros for dates)
                int current_month             = current_dateTimestamp.toString("M").toInt();

                    //Day as 2 digit int (with out leading zeros as most human inputs don't do leading zeros for dates)
                    QString current_day               = current_dateTimestamp.toString("dddd");
                    //Hour as 2 digit int (24 hour time) (with out leading zeros as most human inputs don't do leading zeros for dates)
                    int current_hour              = current_dateTimestamp.toString("h").toInt();

                    //Minute as 2 digit int (with out leading zeros as most human inputs don't do leading zeros for dates)
                    int current_minute            = current_dateTimestamp.toString("m").toInt();

                    qDebug() << "current_hour" << current_hour;
                    qDebug() << "current_minute" << current_minute;

                QString query_string = "SELECT `id`, `timestamp_created`, `printer_name`, `url_location`, `day_mon`, `day_tue`, `day_wed`, `day_thur`, `day_fri`, `day_sat`, `day_sun`, `month_jan`, `month_feb`, `month_march`, `month_april`, `month_may`, `month_june`, `month_july`, `month_aug`, `month_sep`, `month_oct`, `month_nov`, `month_dec`, `hour`, `minutes`,`last_print_timestamp` ";
                query_string += "FROM `schedule_index`";
                query_string += "WHERE `hour` = '%1' AND `minutes` = '%2'";
                query_string += "ORDER BY `id` ASC";


                int lastPrint_dateTimestamp = current_dateTimestamp.toTime_t();
                lastPrint_dateTimestamp -= 60;

                QSqlQuery select_query(print_schedule_db);

                bool select_success = false;
                select_success = select_query.exec(query_string.arg(current_hour).arg(current_minute));
                if(select_success == true){
                    while(select_query.next()){

                        //Check if this has already been printed within the last minute (prevents double printing the same data row)
                        QDateTime current_dateTimestamp   = QDateTime::currentDateTime();
                        int lastPrint_dateTimestamp = current_dateTimestamp.toTime_t();
                        lastPrint_dateTimestamp -= 60;

                        if((select_query.value("last_print_timestamp").toInt() <= lastPrint_dateTimestamp) || (select_query.value("last_print_timestamp").toInt() == 0)){
                            //Check if this row matches the current date.
                                //Check month
                                int current_month_checked = 1;

                                if(current_month == 1 && select_query.value("month_jan").toInt() == 1){
                                        qDebug() << "JAN CHECKED";
                                }else if(current_month == 2 && select_query.value("month_feb").toInt() == 1){
                                        qDebug() << "feb CHECKED";
                                }else if(current_month == 3 && select_query.value("month_march").toInt() == 1){
                                        qDebug() << "MARCH CHECKED";
                                }else if(current_month == 4 && select_query.value("month_april").toInt() == 1){
                                        qDebug() << "april CHECKED";
                                }else if(current_month == 5 && select_query.value("month_may").toInt() == 1){
                                        qDebug() << "may CHECKED";
                                }else if(current_month == 6 && select_query.value("month_june").toInt() == 1){
                                        qDebug() << "june CHECKED";
                                }else if(current_month == 7 && select_query.value("month_july").toInt() == 1){
                                        qDebug() << "july CHECKED";
                                }else if(current_month == 8 && select_query.value("month_aug").toInt() == 1){
                                        qDebug() << "aug CHECKED";
                                }else if(current_month == 9 && select_query.value("month_sep").toInt() == 1){
                                        qDebug() << "sep CHECKED";
                                }else if(current_month == 10 && select_query.value("month_oct").toInt() == 1){
                                        qDebug() << "oct CHECKED";
                                }else if(current_month == 11 && select_query.value("month_nov").toInt() == 1){
                                        qDebug() << "nov CHECKED";
                                }else if(current_month == 12 && select_query.value("month_dec").toInt() == 1){
                                        qDebug() << "dec CHECKED";
                                }else{
                                    //Current month dosen't match any scheduled prints for this target url/file
                                    current_month_checked = 0;
                                }

                                //If a month matched the criteria, check if today matches
                                if(current_month_checked == 1){

                                    int current_day_checked = 1;

                                    if(current_day == "Monday" && select_query.value("day_mon").toInt() == 1){
                                        qDebug() << "monday checked";
                                    }else if(current_day == "Tuesday" && select_query.value("day_tue").toInt() == 1){
                                        qDebug() << "tuesday checked";
                                    }else if(current_day == "Wednesday" && select_query.value("day_wed").toInt() == 1){
                                        qDebug() << "Wednesday checked";
                                    }else if(current_day == "Thursday" && select_query.value("day_thur").toInt() == 1){
                                        qDebug() << "Thursday checked";
                                    }else if(current_day == "Friday" && select_query.value("day_fri").toInt() == 1){
                                        qDebug() << "Friday checked";
                                    }else if(current_day == "Saturday" && select_query.value("day_sat").toInt() == 1){
                                        qDebug() << "Saturday checked";
                                    }else if(current_day == "Sunday" && select_query.value("day_sun").toInt() == 1){
                                        qDebug() << "Sunday checked";
                                    }else{
                                        current_day_checked = 0;
                                    }



                                    if(current_day_checked == 1){
                                        qDebug() << "PRINTING";
                                        //PRINT!
                                        fileview_web->load(QUrl(select_query.value("url_location").toString()));

                                        //Setup printer
                                        printer->setPrintRange(QPrinter::AllPages);
                                        printer->setOrientation(QPrinter::Portrait);
                                        printer->setPaperSize(QPrinter::A4);
                                        printer->setResolution(QPrinter::HighResolution);
                                        printer->setFullPage(false);

                                        int num_of_copies = select_query.value("day_sun").toInt();
                                        if(num_of_copies <= 0){
                                            num_of_copies = 1;
                                        }

                                        printer->setNumCopies(num_of_copies);

                                        //Update last timestamp print
                                        QString query_string_update;
                                        query_string_update = QString("UPDATE `schedule_index` SET `last_print_timestamp` = '%1' WHERE `id` = '%2'").arg(current_dateTimestamp.toTime_t()).arg(select_query.value("id").toInt());

                                        qDebug() << query_string_update;

                                        QSqlQuery update_query(print_schedule_db);

                                        bool update_success = false;
                                        update_success      = update_query.exec(query_string_update);
                                        qDebug() << "UPDATE" << update_success;
                                        loop_check_schedule_timer->stop();
                                    }
                                }
                        }
                    }
                }
            }

            if(keep_checking_schedule != 1){
                loop_check_schedule_timer->stop();
            }
        }
        void printershaman_gui_main::fileviewLoaded_now_print(){
            qDebug() <<" PRINTING";

            //Print
                //Resume timer
                loop_check_schedule_timer->start();

                fileview_web->print(printer);
        }

        void printershaman_gui_main::slot_removeSelectedPrintingRow(){
            QItemSelection selection(printingSchedule_tableview->selectionModel()->selection());

            foreach(const QModelIndex & index, selection.indexes()) {
                //index.row()
                QModelIndex id = index.sibling(index.row(),0);
                QVariant id_data(id.data());


                //Delete the print schedule
                QString query_string_delete;
                query_string_delete = QString("DELETE FROM `schedule_index` WHERE `id` = '%1'").arg(id_data.toInt());



                QSqlQuery delete_query(print_schedule_db);

                bool update_success = false;
                update_success      = delete_query.exec(query_string_delete);
                qDebug() << "UPDATE" << update_success;

                //Remove visually.
                printingSchedule_standarditemmodel->removeRows(index.row(), 1);
            }

        }
