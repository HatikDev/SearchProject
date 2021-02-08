#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    loadPlugin();
    filesCount = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_browseButton_clicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    QString path = QFileDialog::getExistingDirectory(this, "Выбор папки",
                                                     QDir::currentPath());

    if (!path.isEmpty()) ui->pathLineEdit->setText(path);
}

void MainWindow::on_fileSizeCheckBox_stateChanged(int state)
{
    if (state == 0)
    {
        ui->fileSizeComboBox->setEnabled(false);
        ui->fileSizeSpinBox->setEnabled(false);
        ui->fileSizeType->setEnabled(false);
    } else
    {
        ui->fileSizeComboBox->setEnabled(true);
        ui->fileSizeSpinBox->setEnabled(true);
        ui->fileSizeType->setEnabled(true);
    }
}

void MainWindow::on_notOlderCheckBox_stateChanged(int state)
{
    if (state == 0)
    {
        ui->timeSpinBox->setEnabled(false);
        ui->timeComboBox->setEnabled(false);
    } else
    {
        ui->timeSpinBox->setEnabled(true);
        ui->timeComboBox->setEnabled(true);
    }
}

void MainWindow::on_startSearchButton_clicked()
{
    ui->resultsWidget->clear();
    block();
    filesCount = 0;

    if (ui->pathLineEdit->text().isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Ошибка");
        msgBox.setText("Не выбрана папка для поиска");
        msgBox.exec();

        unblock();

        return;
    }

    Qt::CheckState notOlderState = ui->notOlderCheckBox->checkState();
    Qt::CheckState fileSizeState = ui->fileSizeCheckBox->checkState();

    short type = ui->fileSizeComboBox->currentIndex();

    plugin->startSearch(ui->pathLineEdit->text(),
                        QStringList(ui->fileExtensionLineEdit->text()),
                        notOlderState, fileSizeState, type, ui->fileSizeSpinBox->value(),
                        ui->timeSpinBox->value());
}

void MainWindow::loadPlugin()
{
    QDir dir = QDir::currentPath();
    dir.cd("../searchplugin/debug");

    QString path = dir.path() + "/searchplugin.dll";

    QPluginLoader loader(path);

    if(!loader.load()){
        qDebug() << "Error:" << loader.fileName() << loader.errorString();
    }

    plugin = qobject_cast<PluginInterface*>(loader.instance());
    if(plugin)
    {
        plugin->testPlugin("Hello, plugin");

        connect(plugin->instance(), SIGNAL(fileFind(QString)),
                this, SLOT(fileWasFound(QString)));

        connect(plugin->instance(), SIGNAL(finished()),
                this, SLOT(searchFinished()));
    } else{
        qDebug() << "Plugin " << loader.fileName() <<
                    "doesn't support Plugin interface" ;
    }
}

void MainWindow::block()
{
    ui->startSearchButton->setEnabled(false);
    ui->cancelButton->setEnabled(true);
    ui->pathLineEdit->setEnabled(false);
    ui->browseButton->setEnabled(false);
    ui->notOlderCheckBox->setEnabled(false);
    ui->fileSizeCheckBox->setEnabled(false);
}

void MainWindow::unblock()
{
    ui->startSearchButton->setEnabled(true);
    ui->cancelButton->setEnabled(false);
    ui->pathLineEdit->setEnabled(true);
    ui->browseButton->setEnabled(true);
    ui->notOlderCheckBox->setEnabled(true);
    ui->fileSizeCheckBox->setEnabled(true);
}

void MainWindow::on_cancelButton_clicked()
{
    unblock();
}

void MainWindow::searchFinished()
{
    ui->resultsWidget->addItem("Найдено файлов: " + QString::number(filesCount));

    unblock();
}

void MainWindow::fileWasFound(QString filename)
{
    filesCount++;
    ui->resultsWidget->addItem(filename);
}
