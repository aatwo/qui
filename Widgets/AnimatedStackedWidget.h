#ifndef QUI_ANIMATED_STACKED_WIDGET
#define QUI_ANIMATED_STACKED_WIDGET


#include <QWidget>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>


namespace qui
{


    class AnimatedStackedWidget : public QWidget
    {
        Q_OBJECT
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

            Q_PROPERTY( float mCurrentIndexAnimated READ getCurrentIndexAnimated WRITE setCurrentIndexAnimated NOTIFY currentIndexAnimatedChanged )

            float               mCurrentIndexAnimated   = -1.0;
            int                 mCurrentIndex           = -1;
            QPropertyAnimation* mWidgetAnimator         = nullptr;
            QWidgetList         mWidgetList;
            QList<QGraphicsOpacityEffect*> mOpacityEffectList;

            // AnimationProperties
            int                 mAnimationDurationMs        = 400;
            int                 mAnimationHeightChange      = 40; // Can be negative
            int                 mAnimationOffScreenDistance = 10;
            const bool          mAnimationChangeOpacity     = true;
            AnimationOrientation mAnimationOrientation      = AnimationOrientation::horizontal;


    }; // AnimatedStackedWidget


} // namespace qui


#endif // QUI_ANIMATED_STACKED_WIDGET
