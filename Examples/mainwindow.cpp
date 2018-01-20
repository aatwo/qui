#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <ImageLabel.h>
#include <AnimatedStackedWidget.h>
#include <QLabel>
#include <QPushButton>


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
            QLabel* w = new QLabel( QString( "Widget %1 " ).arg( i + 1 ) );

            int r = getRandomInt( 0, 255 );
            int g = getRandomInt( 0, 255 );
            int b = getRandomInt( 0, 255 );
            QColor colour = QColor( r, g, b );

            w->setStyleSheet( QString( "QLabel { background: %1; }" ).arg( colour.name() ) );
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
