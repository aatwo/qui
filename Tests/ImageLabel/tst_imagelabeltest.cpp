#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include <QImage>
#include "../../Widgets/ImageLabel.h"

//------------------------------------------------------------------------------

class ImageLabelTest : public QObject
{
    Q_OBJECT
    public:

                            ImageLabelTest();


    private Q_SLOTS:

        void                testLifeCycleNoImage();
        void                testLifeCycleWithImage();
        void                testLifeCycleWithPostCreationImage();


    private:

        qui::ImageLabel*    label = nullptr;
        QImage              image;


}; // ImageLabelTest

//------------------------------------------------------------------------------

ImageLabelTest::ImageLabelTest()
    : image( "://test.png" )
{
}

void ImageLabelTest::testLifeCycleNoImage()
{
    label = new qui::ImageLabel();
    label->show();

    QVERIFY2( label->image.isNull(), "Image is not null!" );
    QVERIFY2( label->cachedImage.isNull(), "Cached image is not null!" );

    delete label;
    label = nullptr;
    QVERIFY( true );
}

void ImageLabelTest::testLifeCycleWithImage()
{
    label = new qui::ImageLabel( image );
    label->show();

    QVERIFY2( !label->image.isNull(), "Image is null!" );
    QVERIFY2( !label->cachedImage.isNull(), "Cached image is null!" );

    delete label;
    label = nullptr;
    QVERIFY( true );
}

void ImageLabelTest::testLifeCycleWithPostCreationImage()
{
    label = new qui::ImageLabel();
    label->setImage( image );
    label->show();

    QVERIFY2( !label->image.isNull(), "Image is null!" );
    QVERIFY2( !label->cachedImage.isNull(), "Cached image is null!" );

    delete label;
    label = nullptr;
    QVERIFY( true );
}

QTEST_MAIN(ImageLabelTest)

#include "tst_imagelabeltest.moc"
