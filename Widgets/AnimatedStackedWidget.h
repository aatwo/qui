#ifndef QUI_ANIMATED_STACKED_WIDGET
#define QUI_ANIMATED_STACKED_WIDGET


#include <QWidget>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>


class AnimatedStackedWidgetTest;


namespace qui
{

    // This class can be used to manage a collection of widgets, displaying one at a time and
    // animating the transition between them. It's API mirrors the QStackedWidget API to make it
    // easier to use and familiar for Qt developers and the animations are highly customisable to
    // provide a huge range of visual possibilities. No controls for changing the current index are
    // provided by the class (same as QStackedWidget) so to change the current widget you must do
    // it programmatically.
    class AnimatedStackedWidget : public QWidget
    {
        Q_OBJECT
        friend class AnimatedStackedWidgetTest;
        public:

            enum class AnimationOrientation
            {
                horizontal,
                vertical
            };

                                AnimatedStackedWidget( QWidget* parent = nullptr );
                                ~AnimatedStackedWidget();

            int                 addWidget( QWidget* widget );
            int                 count() const;
            int                 currentIndex() const;
            QWidget*            currentWidget() const;
            int                 indexOf( QWidget* widget ) const;
            int                 insertWidget( int index, QWidget* widget );
            void                removeWidget( QWidget* widget );
            QWidget*            widget( int index ) const;

            void                setAnimationEasingCurve( const QEasingCurve& easingCurve );
            void                setAnimationDurationMs( int ms );
            void                setAnimationHeightChangePixels( int pixels );
            void                setAnimationOffScreenDistance( int pixels );
            void                setAnimateOpacity( bool animateOpacity );
            void                setAnimationOrientation( AnimationOrientation orientation );
            void                setAnimateWidgetSize( bool animate );
            void                setAnimationWidgetScaleFactor( float scaleFactor );


        public slots:

            void                setCurrentIndex( int index );
            void                setCurrentWidget( QWidget* widget );


        signals:

            void                currentChanged( int index );
            void                widgetRemoved( int index );
            void                currentIndexAnimatedChanged();


        private:

            virtual void        resizeEvent( QResizeEvent* ) override;
            float               getCurrentIndexAnimated() const;
            void                setCurrentIndexAnimated( float value );
            void                recomputeWidgetGeometries();
            void                onAnimationFinished();
            void                createGraphicsEffects();
            void                cleanupGraphicsEffects();


            Q_PROPERTY( float mCurrentIndexAnimated READ getCurrentIndexAnimated WRITE setCurrentIndexAnimated NOTIFY currentIndexAnimatedChanged )

            float               mCurrentIndexAnimated   = -1.0;
            int                 mCurrentIndex           = -1;
            QPropertyAnimation* mWidgetAnimator         = nullptr;
            QWidgetList         mWidgetList;
            QList<QGraphicsOpacityEffect*> mOpacityEffectList;

            // AnimationProperties
            int                 mAnimationDurationMs        = 400;
            int                 mAnimationHeightChange      = 0;
            int                 mAnimationOffScreenDistance = 40;
            bool                mAnimationChangeOpacity     = true;
            AnimationOrientation mAnimationOrientation      = AnimationOrientation::horizontal;
            QEasingCurve        mAnimationEasingCurve       = QEasingCurve( QEasingCurve::OutCubic );
            bool                mAnimateWidgetSize          = true;
            float               mAnimationWidgetScaleFactor = 0.2f;


    }; // AnimatedStackedWidget


} // namespace qui


#endif // QUI_ANIMATED_STACKED_WIDGET
