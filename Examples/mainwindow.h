#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>


namespace Ui {
    class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT
    public:

        explicit            MainWindow( QWidget* parent = 0 );
                            ~MainWindow();


    private:

        void                onComboBoxIndexChanged( int index );

        Ui::MainWindow*     ui;


}; // MainWindow


#endif // MAINWINDOW_H
