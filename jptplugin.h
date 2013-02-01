#ifndef JPTPLUGIN_H
#define JPTPLUGIN_H

#include <QtGui/QImageIOPlugin>


class JPTPlugin : public QImageIOPlugin
{
    //Q_OBJECT
    //Q_PLUGIN_METADATA(IID "com.ivolgamus.Pipeline.JPTPlugin" FILE "jpt.json")
public:
    QStringList keys() const;
    Capabilities capabilities(QIODevice *device, const QByteArray &format) const;
    QImageIOHandler * create(QIODevice *device, const QByteArray &format) const;
};

#endif // JPTPLUGIN_H
