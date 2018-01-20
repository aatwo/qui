#include "AnimatedStackedWidget.h"
#include <QDebug>
#include <QtMath>


qui::AnimatedStackedWidget::AnimatedStackedWidget( QWidget* parent )
    : QWidget( parent )
{
    setObjectName( "AnimatedStackedWidget" );
    mWidgetAnimator = new QPropertyAnimation( this, "mCurrentIndexAnimated" );
    mWidgetAnimator->setDuration( mAnimationDurationMs );
    mWidgetAnimator->setEasingCurve( mAnimationEasingCurve );

    connect( mWidgetAnimator, &QPropertyAnimation::finished, this, &AnimatedStackedWidget::onAnimationFinished );
}

qui::AnimatedStackedWidget::~AnimatedStackedWidget()
{
    if( mAnimationChangeOpacity )
    {
        for( QWidget* w : mWidgetList )
            w->setGraphicsEffect( nullptr );
    }
}

int qui::AnimatedStackedWidget::addWidget( QWidget* widget )
{
    // Add the animation opacity effect
    if( mAnimationChangeOpacity )
    {
        auto opacityEffect = new QGraphicsOpacityEffect( widget );
        mOpacityEffectList.append( opacityEffect );
        widget->setGraphicsEffect( opacityEffect );
        opacityEffect->setOpacity( 1.0 );
    }

    if( !mWidgetList.size() )
    {
        mCurrentIndex = 0;
        mCurrentIndexAnimated = 0.0f;
        mWidgetList.append( widget );
        widget->setParent( this );
        widget->setGeometry( contentsRect() );
        widget->show();
        return 0;
    }

    mWidgetList.append( widget );
    widget->setParent( this );
    widget->hide();
    return mWidgetList.size() - 1;
}

int qui::AnimatedStackedWidget::count() const
{
    return mWidgetList.size();
}

int qui::AnimatedStackedWidget::currentIndex() const
{
    return mCurrentIndex;
}

QWidget* qui::AnimatedStackedWidget::currentWidget() const
{
    if( mCurrentIndex == -1 )
        return nullptr;

    return mWidgetList[ mCurrentIndex ];
}

int qui::AnimatedStackedWidget::indexOf( QWidget* widget ) const
{
    return mWidgetList.indexOf( widget );
}

int qui::AnimatedStackedWidget::insertWidget( int index, QWidget* widget )
{
    if( index < 0 || index > mWidgetList.size() )
        return -1;

    // Add the animation opacity effect
    if( mAnimationChangeOpacity )
    {
        auto opacityEffect = new QGraphicsOpacityEffect( widget );
        mOpacityEffectList.insert( index, opacityEffect );
        widget->setGraphicsEffect( opacityEffect );
        opacityEffect->setOpacity( 1.0 );
    }

    if( index <= mCurrentIndex )
    {
        // Insert the widget and increment the current index
        mWidgetList.insert( index, widget );
        mCurrentIndex++;
        emit currentChanged( mCurrentIndex );
    }

    else
    {
        // Just insert the widget
        mWidgetList.insert( index, widget );
    }

    // Halt any animations and snap to the new index immediately
    mWidgetAnimator->stop();
    setCurrentIndexAnimated( static_cast<float>( mCurrentIndex ) );
    for( int i = 0; i < mWidgetList.size(); i++ )
    {
        QWidget*    w       = mWidgetList[ i ];
        bool        visible = ( i == mCurrentIndex );

        w->setVisible( visible );
        if( visible )
        {
            w->setGeometry( contentsRect() );
            if( mAnimationChangeOpacity )
                mOpacityEffectList[ i ]->setOpacity( 1.0 );
        }
    }

    return index;
}

void qui::AnimatedStackedWidget::removeWidget( QWidget* widget )
{
    int index = indexOf( widget );
    if( index < 0 || index >= mWidgetList.size() )
        return;

    int startCurrentIndex = mCurrentIndex;

    widget->setVisible( false );

    // Remove the opacity effect
    if( mAnimationChangeOpacity )
    {
        widget->setGraphicsEffect( nullptr );
        mOpacityEffectList.removeAt( index );
    }

    // Remove the widget and decrement the index if we need to
    mWidgetList.removeAt( index );
    if( index < mCurrentIndex )
        mCurrentIndex--;

    // Ensure the index is still valid
    if( !mWidgetList.size() )
        mCurrentIndex = -1;

    else if( mCurrentIndex < 0 )
        mCurrentIndex = 0;

    else if( mCurrentIndex >= mWidgetList.size() )
        mCurrentIndex = mWidgetList.size() - 1;

    // Notify of the index change
    if( startCurrentIndex != mCurrentIndex )
        emit currentChanged( mCurrentIndex );

    // Halt any animations and snap to the new index immediately
    mWidgetAnimator->stop();
    setCurrentIndexAnimated( static_cast<float>( mCurrentIndex ) );
    for( int i = 0; i < mWidgetList.size(); i++ )
    {
        QWidget*    w       = mWidgetList[ i ];
        bool        visible = ( i == mCurrentIndex );

        w->setVisible( visible );
        if( visible )
        {
            w->setGeometry( contentsRect() );

            if( mAnimationChangeOpacity )
                mOpacityEffectList[ i ]->setOpacity( 1.0 );
        }
    }
}

QWidget* qui::AnimatedStackedWidget::widget( int index ) const
{
    if( index < 0 || index >= mWidgetList.size() )
        return nullptr;

    return mWidgetList[ index ];
}

void qui::AnimatedStackedWidget::setAnimationEasingCurve( const QEasingCurve& easingCurve )
{
    mAnimationEasingCurve = easingCurve;
    mWidgetAnimator->setEasingCurve( easingCurve );
}

void qui::AnimatedStackedWidget::setAnimationDurationMs( int ms )
{
    mAnimationDurationMs = ms;
    mWidgetAnimator->setDuration( ms );
}

void qui::AnimatedStackedWidget::setAnimationHeightChangePixels( int pixels )
{
    mAnimationHeightChange = pixels;
}

void qui::AnimatedStackedWidget::setAnimationOffScreenDistance( int pixels )
{
    mAnimationOffScreenDistance = pixels;
}

void qui::AnimatedStackedWidget::setAnimateOpacity( bool animateOpacity )
{
    if( animateOpacity == mAnimationChangeOpacity )
        return;

    mAnimationChangeOpacity = animateOpacity;

    if( animateOpacity )
        createGraphicsEffects();
    else
        cleanupGraphicsEffects();
}

void qui::AnimatedStackedWidget::setAnimationOrientation( AnimationOrientation orientation )
{
    mAnimationOrientation = orientation;
}

void qui::AnimatedStackedWidget::setAnimateWidgetSize( bool animate )
{
    mAnimateWidgetSize = animate;
}

void qui::AnimatedStackedWidget::setAnimationWidgetScaleFactor( float scaleFactor )
{
    mAnimationWidgetScaleFactor = scaleFactor;
}

void qui::AnimatedStackedWidget::setCurrentIndex( int index )
{
    if( index < 0 || index >= mWidgetList.size() || index == mCurrentIndex )
        return;

    mWidgetAnimator->stop();
    mWidgetAnimator->setStartValue( mCurrentIndexAnimated );
    mWidgetAnimator->setEndValue( static_cast<float>( index ) );
    mWidgetAnimator->start();

    mCurrentIndex = index;
    emit currentChanged( index );
}

void qui::AnimatedStackedWidget::setCurrentWidget( QWidget* widget )
{
    int index = indexOf( widget );
    setCurrentIndex( index );
}

void qui::AnimatedStackedWidget::resizeEvent( QResizeEvent* )
{
    recomputeWidgetGeometries();
}

float qui::AnimatedStackedWidget::getCurrentIndexAnimated() const
{
    return mCurrentIndexAnimated;
}

void qui::AnimatedStackedWidget::setCurrentIndexAnimated( float value )
{
    mCurrentIndexAnimated = value;
    emit currentIndexAnimatedChanged();

    recomputeWidgetGeometries();
}

void qui::AnimatedStackedWidget::recomputeWidgetGeometries()
{
    if( !mWidgetList.size() )
        return;

    // Set the position and visibility of each widget
    for( int i = 0; i < mWidgetList.size(); i++ )
    {
        QWidget* w = mWidgetList[ i ];

        float minIndex = mCurrentIndexAnimated - 1.0f;
        float maxIndex = mCurrentIndexAnimated + 1.0f;

        // Update visibility
        bool isVisible = ( i >= minIndex && i <= maxIndex );
        w->setVisible( isVisible );

        if( !isVisible )
            continue;

        w->resize( contentsRect().size() );

        int     widgetX             = contentsRect().left();
        int     widgetY             = contentsRect().top();
        int     widgetWidth         = contentsRect().width();
        int     widgetHeight        = contentsRect().height();
        float   distanceFromCenter  = qAbs( mCurrentIndexAnimated - static_cast<float>( i ) ); // Value between 0.0 and 1.0
        float   progress            = ( static_cast<float>( i ) - minIndex ) / ( maxIndex - minIndex );

        // animations runs from minIndex all the way to maxIndex
        // so the progress is equal to i as a percentage through
        // the min to max index range

        // Animate the widget size
        if( mAnimateWidgetSize )
        {
            float sizeScaleFactorRange  = 1.0f - mAnimationWidgetScaleFactor;
            float sizeMultiplier        = mAnimationWidgetScaleFactor + sizeScaleFactorRange * ( 1.0 - distanceFromCenter );

            widgetWidth     = sizeMultiplier * static_cast<float>( contentsRect().width() );
            widgetHeight    = sizeMultiplier * static_cast<float>( contentsRect().height() );
        }

        // Animate the widgets horizontal position
        if( mAnimationOrientation == AnimationOrientation::horizontal )
        {
            float minWidgetX = -widgetWidth - mAnimationOffScreenDistance;
            float maxWidgetX = width() + mAnimationOffScreenDistance;

            widgetX = minWidgetX + ( maxWidgetX - minWidgetX ) * progress;
            widgetY = ( 0.5f * height() ) - ( 0.5f * widgetHeight ) + ( distanceFromCenter * mAnimationHeightChange );
        }

        // Animate the widgets vertical position
        else
        {
            float minWidgetX = -widgetHeight - mAnimationOffScreenDistance;
            float maxWidgetY = height() + mAnimationOffScreenDistance;

            widgetX = ( 0.5f * width() ) - ( 0.5f * widgetWidth ) + ( distanceFromCenter * mAnimationHeightChange );
            widgetY = minWidgetX + ( maxWidgetY - minWidgetX ) * progress;
        }

        if( mAnimationChangeOpacity )
        {
            auto opacityEffect = mOpacityEffectList[ i ];
            opacityEffect->setOpacity( 1.0 - distanceFromCenter );
        }

        w->move( widgetX, widgetY );
        w->resize( widgetWidth, widgetHeight );
    }
}

void qui::AnimatedStackedWidget::onAnimationFinished()
{
    if( !mWidgetList.size() )
        return;

    for( int i = 0; i < mWidgetList.size(); i++ )
    {
        QWidget* w = mWidgetList[ i ];

        if( i != mCurrentIndex )
        {
            w->setVisible( false );
            continue;
        }

        if( mAnimationChangeOpacity )
            mOpacityEffectList[ i ]->setOpacity( 1.0 );

        w->setGeometry( contentsRect() );
        w->move( contentsRect().topLeft() );
        w->setVisible( true );
    }
}

void qui::AnimatedStackedWidget::createGraphicsEffects()
{
    for( QWidget* w : mWidgetList )
    {
        auto opacityEffect = new QGraphicsOpacityEffect( w );
        mOpacityEffectList.append( opacityEffect );
        w->setGraphicsEffect( opacityEffect );
        opacityEffect->setOpacity( 1.0 );
    }
}

void qui::AnimatedStackedWidget::cleanupGraphicsEffects()
{
    for( QWidget* w : mWidgetList )
        w->setGraphicsEffect( nullptr );

    mOpacityEffectList.clear();
}
