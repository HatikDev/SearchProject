#include "searchplugin.h"

#include <QDebug>

//QLinkedList<QString> Searchplugin::allObjects = QLinkedList<QString>();
//QList<QString> Searchplugin::finalList = QList<QString>();
//QMutex Searchplugin::mutex;
//QMutex Searchplugin::isFinished;
Searchplugin* Searchplugin::m_instance;
//QFuture<QString> Searchplugin::result;

QList<QString> Searchplugin::allFiles = QList<QString>();
QList<QString> Searchplugin::folders = QList<QString>();
QList<QString> Searchplugin::acceptableFolders = QList<QString>();

bool Searchplugin::m_isNotOlder = false;
bool Searchplugin::m_isFileSize = false;
short Searchplugin::m_type = 0;
int Searchplugin::m_daysCount = 0;
int Searchplugin::m_size = 0;
QStringList Searchplugin::m_mask = QStringList();

QMutex Searchplugin::folderMutex;

Searchplugin::Searchplugin()
{
    if (!m_instance) m_instance = this;
}

void Searchplugin::testPlugin(QString testString)
{
    qDebug() << testString;
}

QString Searchplugin::search(const QString& name)
{

}

bool Searchplugin::isFileSatisfy(const QString& filename)
{
    QFileInfo fileInfo(filename);

    if (m_isFileSize)
    {
        switch(m_type) {
        case 0: if(fileInfo.size() >= m_size * 1024) return false;
        case 1: if(fileInfo.size() <= m_size * 1024) return false;
        default: if(fileInfo.size() / 1024 != m_size) return false;
        }
    }

    if (m_isNotOlder)
    {
        QDateTime current = QDateTime::currentDateTime();

        if (fileInfo.birthTime().daysTo(current) <= m_daysCount) return false;
    }

    emit m_instance->fileFind(filename);
    return true;
}

void Searchplugin::restartSearch()
{

}

void Searchplugin::addFiles(const QString& directory)
{
    //qDebug() << directory << directory.size();

    if (directory[directory.size()-1] == ".")
    {
        folderMutex.lock();

        folders.removeOne(directory);

        folderMutex.unlock();
    } else
    {
        QFileInfo info(directory);
        QDir dir(directory);

        QStringList dirs = dir.entryList(QDir::Dirs);
        QStringList files = dir.entryList(QDir::Files);

        qDebug() << dirs << files;

        folderMutex.lock();

        for (auto object : dirs)
            folders.append(dir.path() + "/" + object);
        for (auto object : files)
            allFiles.append(dir.path() + "/" + object);

        folders.removeOne(directory);

        folderMutex.unlock();
    }
}

void Searchplugin::startSearch(QString path, QStringList mask,
                               bool isNotOlder, bool isFileSize,
                               short type, int size, int time)
{
    m_path = path;
    m_mask = mask;
    m_isNotOlder = isNotOlder;
    m_isFileSize = isFileSize;
    m_type = type;
    m_size = size;
    m_daysCount = time;

    folders.clear();
    allFiles.clear();

    folders.append(m_path);

    while(folders.length() > 0)
    {
        QFuture<void> future = QtConcurrent::map(folders, addFiles);

        future.waitForFinished();
    }

    QFuture<QString> results = QtConcurrent::filtered(allFiles, isFileSatisfy);

    results.waitForFinished();
    emit m_instance->finished();
}

void Searchplugin::cancel()
{
    //result.cancel();
}

QObject* Searchplugin::instance()
{
    return m_instance;
}
