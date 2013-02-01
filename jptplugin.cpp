#include "jptplugin.h"
#include "jpthandler.h"


QStringList JPTPlugin::keys() const
{
    return QStringList() << "jpt";
}

QImageIOPlugin::Capabilities JPTPlugin::capabilities(QIODevice *device, const QByteArray &format) const
{
    if (format == "jpt")
        return CanRead;
    if (format.isEmpty()) {
        //See the first 4 bytes for symbols "JPT "
        JPTHandler handler;
        handler.setDevice(device);
        if (handler.canRead()) {
            return CanRead;
        }
    }
    return 0;
}

QImageIOHandler * JPTPlugin::create(QIODevice *device, const QByteArray &format) const
{
    JPTHandler * handler = new JPTHandler;
    handler->setDevice(device);
    handler->setFormat(format);
    return handler;
}

Q_EXPORT_PLUGIN2(jptplugin, JPTPlugin)
