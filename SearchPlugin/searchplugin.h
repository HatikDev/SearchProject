#ifndef SEARCHPLUGIN_H
#define SEARCHPLUGIN_H

#include "searchplugin_global.h"
#include "plugininterface.h"

#include <QDir>
#include <QFileInfoList>
#include <QFuture>
#include <QString>
#include <QLinkedList>
#include <QtConcurrent>

class SEARCHPLUGIN_EXPORT Searchplugin : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ru.mephi.FileSearcher.SearchPluginInterface/1.0")
    Q_INTERFACES(PluginInterface)

signals:
    void fileFind(QString filename);

    void finished();

public:
    Searchplugin();
    void testPlugin(QString testString) override;

    void startSearch(QString path, QStringList mask,
                     bool isNotOlder, bool isFileSize,
                     short type, int size, int time) override;
    void cancel() override;

    virtual QObject* instance() override;

private:
    QString m_path;
    static bool m_isNotOlder;
    static bool m_isFileSize;
    static short m_type;
    static int m_daysCount;
    static int m_size;
    static QStringList m_mask;

    static Searchplugin* m_instance;

//    static QFuture<QString> result;
//    static QLinkedList<QString> allObjects;
//    static QList<QString> finalList;

//    static QMutex mutex;
//    static QMutex isFinished;
    static QList<QString> allFiles;
    static QList<QString> folders;
    static QList<QString> acceptableFolders;

    static bool isFileSatisfy(const QString& filename);
    static QString search(const QString &name);
    static void restartSearch();


    static QMutex folderMutex;

    static void addFiles(const QString& directory);
    static void addFolders();
};

#endif // SEARCHPLUGIN_H
