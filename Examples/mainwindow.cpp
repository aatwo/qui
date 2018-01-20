#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>
#include <ImageLabel.h>
#include <AnimatedStackedWidget.h>


MainWindow::MainWindow( QWidget* parent ) :
    QMainWindow( parent ),
    ui( new Ui::MainWindow )
{
    ui->setupUi( this );

    { // ImageLabel

        QImage image( ":/test.png" );
        auto imageLabel = new qui::ImageLabel( image, ui->stackedWidget );
        ui->stackedWidget->addWidget( imageLabel );
        ui->comboBox->addItem( imageLabel->objectName() );
    }

    { // Animated stacked widget

        QWidget*        container       = new QWidget( ui->stackedWidget );
        QVBoxLayout*    containerLayout = new QVBoxLayout( container );
        QHBoxLayout*    buttonLayout    = new QHBoxLayout();
        QPushButton*    goLeftButton    = new QPushButton( "<-", container );
        QPushButton*    goRightButton   = new QPushButton( "->", container );
        QComboBox*      indexCombo      = new QComboBox( container );

        goLeftButton->setSizePolicy( QSizePolicy::Maximum, goLeftButton->sizePolicy().verticalPolicy() );
        goRightButton->setSizePolicy( QSizePolicy::Maximum, goRightButton->sizePolicy().verticalPolicy() );

        auto getRandomInt = []( int min, int max )
        {
            return qrand() % ( max - min + 1 ) + min;
        };

        auto animatedStackedWidget = new qui::AnimatedStackedWidget( container );
        for( int i = 0; i < 20; i++ )
        {
            QWidget* w = nullptr;
            switch( getRandomInt( 0, 2 ) )
            {
                case 0:
                {
                    w = new QLabel( QString( "Widget %1 " ).arg( i + 1 ) );

                    int r = getRandomInt( 0, 255 );
                    int g = getRandomInt( 0, 255 );
                    int b = getRandomInt( 0, 255 );
                    QColor colour = QColor( r, g, b );
                    w->setStyleSheet( QString( "QLabel { background: %1; }" ).arg( colour.name() ) );
                    break;
                }

                case 1:
                {
                    w = new QWidget();
                    QGridLayout* l = new QGridLayout( w );

                    QLabel* l1 = new QLabel( "Label 1", w );
                    QLabel* l2 = new QLabel( "Label 2", w );
                    QLabel* l3 = new QLabel( "Label 3", w );

                    QLineEdit* e1 = new QLineEdit( "Line edit 1", w );
                    QLineEdit* e2 = new QLineEdit( "Line edit 2", w );
                    QLineEdit* e3 = new QLineEdit( "Line edit 3", w );

                    l->addWidget( l1, 0, 0, 1, 1 );
                    l->addWidget( e1, 0, 1, 1, 1 );

                    l->addWidget( l2, 1, 0, 1, 1 );
                    l->addWidget( e2, 1, 1, 1, 1 );

                    l->addWidget( l3, 2, 0, 1, 1 );
                    l->addWidget( e3, 2, 1, 1, 1 );
                    break;
                }

                case 2:
                {
                    w = new QDial();
                    break;
                }
            }

            animatedStackedWidget->addWidget( w );
            indexCombo->addItem( QString::number( i + 1 ) );
        }

        containerLayout->addWidget( animatedStackedWidget );
        containerLayout->addLayout( buttonLayout );
        buttonLayout->addWidget( goLeftButton );
        buttonLayout->addWidget( indexCombo );
        buttonLayout->addWidget( goRightButton );

        connect( goLeftButton, &QPushButton::clicked, [=](){ animatedStackedWidget->setCurrentIndex( animatedStackedWidget->currentIndex() - 1 ); } );
        connect( goRightButton, &QPushButton::clicked, [=](){ animatedStackedWidget->setCurrentIndex( animatedStackedWidget->currentIndex() + 1 ); } );
        connect( indexCombo, QOverload<int>::of( &QComboBox::currentIndexChanged ), [=]( int index ){ animatedStackedWidget->setCurrentIndex( index ); } );

        ui->stackedWidget->addWidget( container );
        ui->comboBox->addItem( animatedStackedWidget->objectName() );
    }

    connect( ui->comboBox, QOverload<int>::of( &QComboBox::currentIndexChanged ), this, &MainWindow::onComboBoxIndexChanged );
    ui->comboBox->setCurrentIndex( 1 );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onComboBoxIndexChanged( int index )
{
    ui->stackedWidget->setCurrentIndex( index );
}
