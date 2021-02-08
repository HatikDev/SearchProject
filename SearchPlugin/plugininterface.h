#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <QString>
#include <QtPlugin>
#include <QFuture>

class PluginInterface
{
public:
    virtual ~PluginInterface(){};
    virtual void testPlugin(QString testString) = 0;

//    virtual void setPath(QString path) = 0;
//    virtual void setTime(int daysCount) = 0;
//    virtual void setSize(int size) = 0;

    virtual void startSearch(QString path, QStringList masks,
                             bool isNotOlder, bool isFileSize,
                             short type, int size, int time) = 0;
    virtual void cancel() = 0;

    virtual QObject* instance() = 0;
};

Q_DECLARE_INTERFACE(PluginInterface, "ru.mephi.FileSearcher.SearchPluginInterface/1.0")

#endif // PLUGININTERFACE_H
