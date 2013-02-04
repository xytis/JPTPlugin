#include "jpthandler.h"

#include <QImage>
#include <QFile>

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
    //    //Create a dummy image.
    //    * image = QImage(16,16, QImage::Format_ARGB32);
    //    for (int i = 0; i < 16; ++i) {
    //        for (int j = 0; j < 16; ++j) {
    //            QRgb color;
    //            color = qRgb(128,128,40);
    //            image->setPixel(j,i,color);
    //        }
    //    }
    //    return true;

    QDataStream input(device());

//    qWarning("%u", input.byteOrder());
    input.setByteOrder(QDataStream::LittleEndian);

    quint32 cursor = 0;
    quint32 magic;
    quint32 version;
    input >> magic >> version;
//    qWarning("Magic: %x Version: %x", magic, version);
    cursor += 8;
    if (input.status() != QDataStream::Ok || magic != 0x2054504A || version != 0x10001) {
        qWarning("JPTHandler::read() encountered unsuported file.");
        if (input.status() != QDataStream::Ok) {
            qWarning("JPTHandler::read() bad datastream.");
        }
        if (magic != 0x2054504A) {
            qWarning("JPTHandler::read() bad magic number: %u", magic);
        }
        if (version != 0x10001) {
            qWarning("JPTHandler::read() bad version: %x", version);
        }
        return false;
    }

    quint32 colorOffset, colorSize, alphaOffset, alphaSize, flags;
    input >> colorOffset >> colorSize >> alphaOffset >> alphaSize >> flags; //another 20 bytes read
    cursor += 20;
//    qWarning("JPTHandler::read() Read file header.");
//    qWarning("JPTHandler::read() Color offset: %x\nColor size: %x\nAlpha offset: %x\nAlpha size: %x", colorOffset, colorSize, alphaOffset, alphaSize);

    //Offsets work as markers. When current cursor reaches one of them -- it reads the aprorpiate data.

    //    quint8 *color = new quint8[colorSize];
    //    quint8 *alpha = new quint8[alphaSize];

    QByteArray color(colorSize, (char) 0xcd);
//    qWarning("JPTHandler::read() colorSize, %u", color.size());

    QByteArray alpha(alphaSize, 0xad);
//    qWarning("JPTHandler::read() alphaSize, %u", alpha.size());


    input.setByteOrder(QDataStream::BigEndian);
    //Current position is 4 * 7 = 28
    while (!input.atEnd()) {
        //Try to read any relevant data
        if (cursor == colorOffset) {
            //Read color
            for (quint32 i = 0; i < colorSize; ++i) {
                quint8 byte;
                input >> byte;
                color[i] = byte;
            }
            cursor += colorSize;
//            qWarning("JPTHandler::read() Read color data, %u", color.size());
        }

        if (cursor == alphaOffset) {
            //Read alpha
            for (quint32 i = 0; i < alphaSize; ++i) {
                quint8 byte;
                input >> byte;
                alpha[i] = byte;
            }
            cursor += alphaSize;
//            qWarning("JPTHandler::read() Read alpha data, %u", alpha.size());
        }

        //Skip a byte
        input.skipRawData(1);
        ++cursor;
    }
//    {
//        QFile file("/Users/vvalentinavicius/Documents/color.jpg");
//        file.open(QIODevice::WriteOnly);
//        file.write(color);
//        file.close();
//    }
//    {
//        QFile file("/Users/vvalentinavicius/Documents/alpha.jpg");
//        file.open(QIODevice::WriteOnly);
//        file.write(alpha);
//        file.close();
//    }

//    qWarning("JPTHandler::read() Read both files.");

    QImage colorImage;
    if (!colorImage.loadFromData(color,"JPG")) {
        qWarning("JPTHandler::read() Failed to load color image!");
        return false;
    }
    QImage alphaImage;
    if (!alphaImage.loadFromData(alpha,"JPG")) {
        qWarning("JPTHandler::read() Failed to load aplha image!");
        return false;
    }

    int width = colorImage.width();
    int height = colorImage.height();

    if (width != alphaImage.width() || height != alphaImage.height()) {
        qWarning("JPTHandler::read() color and alpha chanel size missmatch.");
        return false;
    }

    *image = QImage(width, height, QImage::Format_ARGB32);

    //Glue alpha with color.
    for (int y = 0; y < height; ++y) {
        QRgb *outputLine = (QRgb *)image->scanLine(y);
        QRgb *colorLine = (QRgb *)colorImage.scanLine(y);
        QRgb *alphaLine = (QRgb *)alphaImage.scanLine(y);
        for (int x = 0; x < width; ++x) {
            outputLine[x] = qRgba(qRed(colorLine[x]),
                                  qGreen(colorLine[x]),
                                  qBlue(colorLine[x]),
                                  qAlpha(alphaLine[x]));
        }
    }

    return true;
}
