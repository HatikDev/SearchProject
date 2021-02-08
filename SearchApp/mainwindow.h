#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../SearchPlugin/plugininterface.h"
//#include "../SearchPlugin/searchplugin.h"

#include <QPluginLoader>

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QFutureWatcher>
#include <QLinkedList>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_browseButton_clicked();

    void on_fileSizeCheckBox_stateChanged(int state);

    void on_notOlderCheckBox_stateChanged(int state);

    void on_startSearchButton_clicked();

    void on_cancelButton_clicked();

    void fileWasFound(QString filename);

    void searchFinished();

private:
    Ui::MainWindow* ui;
    PluginInterface* plugin;
    int filesCount;

    void loadPlugin();

    void block();
    void unblock();
};
#endif // MAINWINDOW_H
