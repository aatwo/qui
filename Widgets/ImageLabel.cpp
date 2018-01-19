#include "imagelabel.h"
#include <QtGui>


qui::ImageLabel::ImageLabel( QWidget* parent )
    : QWidget( parent )
{
    setObjectName( "ImageLabel" );
    setContentsMargins( 0, 0, 0, 0 );
    recomputeCachedImage();
}

qui::ImageLabel::ImageLabel( const QImage& image, QWidget* parent )
    : ImageLabel( parent )
{
    this->image = image;
}

qui::ImageLabel::ImageLabel( const QPixmap& image, QWidget* parent )
    : ImageLabel( parent )
{
    this->image = image.toImage();
}

void qui::ImageLabel::setImage( const QImage& image )
{
    this->image = image;
    recomputeCachedImage();
}

void qui::ImageLabel::setImage( const QPixmap& image )
{
    this->image = image.toImage();
    recomputeCachedImage();
}

void qui::ImageLabel::setRenderSpeed( qui::ImageLabel::RenderSpeed speed )
{
    this->renderSpeed = speed;
}

QImage qui::ImageLabel::getImage() const
{
    return image;
}

QImage qui::ImageLabel::getCachedImage() const
{
    return cachedImage;
}

qui::ImageLabel::RenderSpeed qui::ImageLabel::getRenderSpeed() const
{
    return renderSpeed;
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

QSize qui::ImageLabel::sizeHint() const
{
    if( image.isNull() )
        return QSize( 10, 10 );

    return image.size();
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
