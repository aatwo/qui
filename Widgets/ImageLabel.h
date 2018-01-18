#ifndef IMAGELABEL_H
#define IMAGELABEL_H


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

            void                setImage( const QImage& image );
            void                setRenderSpeed( RenderSpeed speed );


        private:

            virtual void        paintEvent( QPaintEvent* ) override;
            virtual void        resizeEvent( QResizeEvent* ) override;
            void                recomputeCachedImage();

            QImage              image;
            QImage              cachedImage;
            RenderSpeed         renderSpeed = RenderSpeed::best_quality;


    }; // ImageLabel


} // namespace qui


#endif // IMAGELABEL_H
