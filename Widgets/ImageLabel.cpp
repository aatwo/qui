#include "imagelabel.h"
#include <QtGui>


qui::ImageLabel::ImageLabel( QWidget* parent )
    : QWidget( parent )
{
    setContentsMargins( 0, 0, 0, 0 );
    recomputeCachedImage();
}

qui::ImageLabel::ImageLabel( const QImage& image, QWidget* parent )
    : QWidget( parent ),
      image( image )
{
}

void qui::ImageLabel::setImage( const QImage& image )
{
    this->image = image;
    recomputeCachedImage();
}

void qui::ImageLabel::setRenderSpeed( qui::ImageLabel::RenderSpeed speed )
{
    this->renderSpeed = speed;
}

void qui::ImageLabel::paintEvent( QPaintEvent* )
{
    QPainter p( this );
    p.save();

    int cachedImageWidth    = 0.5 * width() - 0.5 * cachedImage.width();
    int cachedImageHeight   = 0.5 * height() - 0.5 * cachedImage.height();

    p.drawImage( cachedImageWidth, cachedImageHeight, cachedImage );

    p.restore();
}

void qui::ImageLabel::resizeEvent( QResizeEvent* )
{
    recomputeCachedImage();
}

void qui::ImageLabel::recomputeCachedImage()
{
    if( image.isNull() )
        return;

    int availableWidth = width() - contentsMargins().left() - contentsMargins().right();
    int availableHeight = height() - contentsMargins().top() - contentsMargins().bottom();

    Qt::TransformationMode transformationMode = Qt::SmoothTransformation;
    if( renderSpeed == RenderSpeed::fast )
        transformationMode = Qt::FastTransformation;

    cachedImage = image.scaled( availableWidth, availableHeight, Qt::KeepAspectRatio, transformationMode );
    update();
}
