#ifndef JPTHANDLER_H
#define JPTHANDLER_H

#include <QImageIOHandler>

class JPTHandler : public QImageIOHandler
{
    //Q_OBJECT
public:
    explicit JPTHandler(/*QObject *parent = 0*/);
    bool canRead() const;
    bool read(QImage *image);
private:
    enum State { BeforeHeader, BeforeColors, BeforeAlpha, AtEnd, Error };
    mutable State state;
};

#endif // JPTHANDLER_H
