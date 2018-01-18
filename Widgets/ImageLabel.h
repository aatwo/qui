#ifndef QUI_IMAGELABEL_H
#define QUI_IMAGELABEL_H


#include <QWidget>
#include <QImage>


class ImageLabelTest;


namespace qui
{


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


        private:

            virtual void        paintEvent( QPaintEvent* ) override;
            virtual void        resizeEvent( QResizeEvent* ) override;
            virtual QSize       sizeHint() const override;
            void                recomputeCachedImage();

            QImage              image;
            QImage              cachedImage;
            RenderSpeed         renderSpeed = RenderSpeed::best_quality;


    }; // ImageLabel


} // namespace qui


#endif // QUI_IMAGELABEL_H
