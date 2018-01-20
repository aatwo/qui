#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include "../../Widgets/AnimatedStackedWidget.h"

//------------------------------------------------------------------------------

class AnimatedStackedWidgetTest : public QObject
{
    Q_OBJECT
    public:

                            AnimatedStackedWidgetTest();


    private Q_SLOTS:

        void                testLifeCycleNoItems();
        void                testLifeCycleWithItems();
        void                testAppendingItems();
        void                testInsertingItems();
        void                testRemovingItems();
        void                testClearingItems();

        void                testFirstItemIndexChangeSignal();
        void                testIndexChangeSignal();
        void                testIndexRemovedSignal();


    private:




}; // AnimatedStackedWidgetTest

//------------------------------------------------------------------------------

AnimatedStackedWidgetTest::AnimatedStackedWidgetTest()
{
}

void AnimatedStackedWidgetTest::testLifeCycleNoItems()
{
    auto w = new qui::AnimatedStackedWidget();
    w->show();
    delete w;

    QVERIFY( true );
}

void AnimatedStackedWidgetTest::testLifeCycleWithItems()
{
    auto w = new qui::AnimatedStackedWidget();
    w->show();

    QPointer<QWidget> childWidget = new QWidget( w );
    w->addWidget( childWidget );

    delete w;
    QVERIFY2( !childWidget, "Child widget was not deleted..." );
}

void AnimatedStackedWidgetTest::testAppendingItems()
{
    auto w = new qui::AnimatedStackedWidget();
    w->show();

    for( int i = 0; i < 3; i++ )
    {
        auto childWidget = new QWidget( w );
        w->addWidget( childWidget );

        QVERIFY2( w->mWidgetList[ i ] == childWidget, "Child widget was not appended correctly..." );
    }
}

void AnimatedStackedWidgetTest::testInsertingItems()
{
    auto w = new qui::AnimatedStackedWidget();
    w->show();

    for( int i = 0; i < 3; i++ )
    {
        auto childWidget = new QWidget( w );
        w->addWidget( childWidget );
    }

    QSignalSpy currentIndexSpy( w, &qui::AnimatedStackedWidget::currentChanged );

    auto w1 = new QWidget( w );
    w->insertWidget( 0, w1 );
    QVERIFY2( w->mWidgetList[ 0 ] == w1, "Inserted widget at position 0 but it was not present in the internal list..." );
    auto argList = currentIndexSpy.takeFirst();
    int emittedCurrentIndex = argList[ 0 ].toInt();
    QVERIFY2( emittedCurrentIndex == 1, "Current index should have been bumped to 1 (it was 0 before inserting an item at 0)..." );

    auto w2 = new QWidget( w );
    int insertionIndex = w->mWidgetList.size();
    w->insertWidget( insertionIndex, w2 );
    QVERIFY2( w2 == w->mWidgetList[ insertionIndex ], "Inserted widget at the end of the list but it was not present in the internal list..." );

    auto w3 = new QWidget( w );
    insertionIndex = 2;
    w->insertWidget( insertionIndex, w3 );
    QVERIFY2( w3 == w->mWidgetList[ insertionIndex ], "Inserted widget at the end of the list but it was not present in the internal list..." );
}

void AnimatedStackedWidgetTest::testRemovingItems()
{
    qui::AnimatedStackedWidget* w = new qui::AnimatedStackedWidget();
    w->show();

    QWidgetList childWidgets;
    for( int i = 0; i < 10; i++ )
    {
        auto childWidget = new QWidget( w );
        w->addWidget( childWidget );
        childWidgets.append( childWidget );
    }

    w->setCurrentIndex( 8 );
    QVERIFY2( w->mCurrentIndex == 8, "Unexpected index..." );

    { // Remove a widget in front of the current index

        QSignalSpy currentIndexSpy( w, &qui::AnimatedStackedWidget::currentChanged );
        w->removeWidget( childWidgets[ 0 ] );
        childWidgets.removeAt( 0 );

        QVERIFY2( currentIndexSpy.count() == 1, "No index change signal was emitted after a removal..." );
        auto argList = currentIndexSpy.takeFirst();
        int emittedCurrentIndex = argList[ 0 ].toInt();
        QVERIFY2( emittedCurrentIndex == 7, "Emitted index was incorrect after a removal..." );
        QVERIFY2( w->mCurrentIndex == 7, "Current index was incorrect after a removal..." );
    }

    w->setCurrentIndex( 1 );
    QVERIFY2( w->mCurrentIndex == 1, "Unexpected index..." );

    { // Remove a widget after the current index

        QSignalSpy currentIndexSpy( w, &qui::AnimatedStackedWidget::currentChanged );
        w->removeWidget( childWidgets[ 4 ] );
        childWidgets.removeAt( 4 );

        QVERIFY2( currentIndexSpy.count() == 0, "No index change signal should have been emitted after a removal..." );
        QVERIFY2( w->mCurrentIndex == 1, "Current index was incorrect after a removal..." );
    }

    { // Remove the widget at the current index

        QSignalSpy currentIndexSpy( w, &qui::AnimatedStackedWidget::currentChanged );
        w->removeWidget( childWidgets[ 1 ] );
        childWidgets.removeAt( 1 );

        QVERIFY2( currentIndexSpy.count() == 0, "No index change signal should have been emitted after the removal of the current index unless its the last item in the list..." );
        QVERIFY2( w->mCurrentIndex == 1, "Current index was incorrect after a removal..." );
    }
}

void AnimatedStackedWidgetTest::testClearingItems()
{
    qui::AnimatedStackedWidget* w = new qui::AnimatedStackedWidget();
    w->show();

    QWidgetList childWidgets;
    for( int i = 0; i < 10; i++ )
    {
        auto childWidget = new QWidget( w );
        w->addWidget( childWidget );
        childWidgets.append( childWidget );
    }

    while( w->currentIndex() != -1 && childWidgets.size() )
    {
        QWidget* childWidget = childWidgets[ 0 ];
        w->removeWidget( childWidget );
        childWidgets.removeAt( 0 );
    }

    QVERIFY2( w->mWidgetList.size() == 0, "Some widgets still linger..." );
}

void AnimatedStackedWidgetTest::testFirstItemIndexChangeSignal()
{
    qui::AnimatedStackedWidget* w = new qui::AnimatedStackedWidget();
    w->show();

    QCOMPARE( w->currentIndex(), -1 ); // Index should initially be -1

    QWidget* childWidget = new QWidget( w );

    QSignalSpy indexChangeSpy( w, &qui::AnimatedStackedWidget::currentChanged );

    // Check the signal is emitted after adding the first item
    w->addWidget( childWidget );
    QCOMPARE( w->currentWidget(), childWidget ); // The current widget should be the one we inserted
    QCOMPARE( w->currentIndex(), 0 ); // The index should now be 0
    QCOMPARE( indexChangeSpy.count(), 1 ); // There should be a single captured signal
    auto argList = indexChangeSpy.takeFirst();
    int currentChangedArg = argList.at( 0 ).toInt();
    QCOMPARE( currentChangedArg, 0 ); // The emitted index should have been 0
}

void AnimatedStackedWidgetTest::testIndexChangeSignal()
{
    qui::AnimatedStackedWidget* w = new qui::AnimatedStackedWidget();
    w->show();

    QCOMPARE( w->currentIndex(), -1 ); // Index should initially be -1

    for( int i = 0; i < 3; i++ )
    {
        auto childWidget = new QWidget( w );
        w->addWidget( childWidget );
    }

    // Check the signal is emitted after programatically setting a new index
    QSignalSpy indexChangeSpy( w, &qui::AnimatedStackedWidget::currentChanged );

    w->setCurrentIndex( 2 );
    QCOMPARE( w->currentIndex(), 2 );
    QCOMPARE( indexChangeSpy.count(), 1 );
    auto argList = indexChangeSpy.takeFirst();
    int currentChangedArg = argList.at( 0 ).toInt();
    QCOMPARE( currentChangedArg, 2 ); // The emitted index should have been 2
}

void AnimatedStackedWidgetTest::testIndexRemovedSignal()
{
    QVERIFY( true );
}

//------------------------------------------------------------------------------

QTEST_MAIN(AnimatedStackedWidgetTest)

#include "tst_animatedstackedwidgettest.moc"
