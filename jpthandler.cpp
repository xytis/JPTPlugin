#include "jpthandler.h"

#include <QImage>

JPTHandler::JPTHandler(/*QObject *parent) :
    QImageIOHandler(parent)*/)
{
    state = BeforeHeader;
}

bool JPTHandler::canRead() const
{
    if (state == BeforeHeader) {
        return device()->peek(4) == "JPT ";
    } else {
        return false;
    }
}

bool JPTHandler::read(QImage *image)
{
    //Create a dummy image.
    * image = QImage(16,16, QImage::Format_ARGB32);
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 16; ++j) {
            QRgb color;
            color = qRgb(128,128,40);
            image->setPixel(j,i,color);
        }
    }
    return true;
}
