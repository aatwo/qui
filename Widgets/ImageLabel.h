#ifndef QUI_IMAGELABEL_H
#define QUI_IMAGELABEL_H


#include <QWidget>
#include <QImage>


class ImageLabelTest;


namespace qui
{


    // This class is designed to replace QLabel for displaying images, allowing you to lock the
    // image aspect ratio, use both QPixmaps and QImages, and change the render speed between
    // best-quality and fastest-to-render.
    class ImageLabel : public QWidget
    {
        Q_OBJECT
        friend class ImageLabelTest;
        public:

            enum class RenderSpeed
            {
                fast,
                best_quality
            };

            explicit            ImageLabel( QWidget* parent = nullptr );
            explicit            ImageLabel( const QImage& image, QWidget* parent = nullptr );
            explicit            ImageLabel( const QPixmap& image, QWidget* parent = nullptr );

            void                setImage( const QImage& image );
            void                setImage( const QPixmap& image );
            void                setRenderSpeed( RenderSpeed speed );

            QImage              getImage() const;
            QImage              getCachedImage() const;
            RenderSpeed         getRenderSpeed() const;
            void                setLockAspectRatio( bool lock );


        private:

            virtual void        paintEvent( QPaintEvent* ) override;
            virtual void        resizeEvent( QResizeEvent* ) override;
            virtual QSize       sizeHint() const override;
            void                recomputeCachedImage();

            QImage              image;
            QImage              cachedImage;
            RenderSpeed         renderSpeed = RenderSpeed::best_quality;
            bool                lockAspectRatio = true;


    }; // ImageLabel


} // namespace qui


#endif // QUI_IMAGELABEL_H
