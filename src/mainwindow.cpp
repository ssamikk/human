#include "humandialog.h"
#include "mainwindow.h"
#include "tablemodel.h"
#include "ui_mainwindow.h"
#include "viewhumandialog.h"

#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include "constfunction.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), fileName(qApp->applicationDirPath()+QDir::separator() + "save.json")
{
    ui->setupUi(this);
    tableModel = new TableModel(this);
    ui->tableView->setModel(tableModel);
    ui->tableView->setColumnWidth(0, 200);
    connect(ui->actionAdd, &QAction::triggered, this, &MainWindow::addHuman);
    connect(ui->actionDell, &QAction::triggered, this, &MainWindow::dellHuman);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::open);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::load(QString fileName)
{
    if (!fileName.isEmpty()) {
        this->fileName = fileName;
    }
    QFile loadFile(this->fileName);

    if (!loadFile.exists()) {
        qWarning()<<utr("Файл не существует.\n")<<this->fileName;
        return false;
    }

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning()<<utr("Запрещено чтение файла.\n")<<this->fileName;
        return false;
    }
    QByteArray saveData = loadFile.readAll();
    QJsonParseError error;
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData, &error));
    if (error.error != QJsonParseError::NoError) {
        qWarning()<<utr("Ошибка в JSON файле = ") + error.errorString();
        return false;
    }
    if (loadDoc.array().isEmpty()){
        qWarning()<<utr("JSON пуст");
        return false;
    }
    tableModel->setJsonArray(loadDoc.array());
    return true;
}


bool MainWindow::save()
{
    QFile saveFile(fileName);

    if (!saveFile.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, utr("Запись файла"), utr("Ошибка при открытии файла на запись.\n")+fileName, QMessageBox::Cancel);
        return false;
    }
    QJsonDocument saveDoc(tableModel->getJsonArray());
    saveFile.write(saveDoc.toJson());
    return true;
}

void MainWindow::addHuman()
{
    HumanDialog humanDialog(this);
    if (humanDialog.exec()==QDialog::Accepted){
        ViewHumanDialog view(this, humanDialog.save());
        if (view.exec()==QDialog::Accepted){
            tableModel->addObject(view.getHuman());
            ui->tableView->selectRow(tableModel->rowCount()-1);
            save();
        }
    }
}

void MainWindow::dellHuman()
{
    QModelIndexList list = ui->tableView->selectionModel()->selectedRows();
    if (list.isEmpty()) {
        return;
    }
    std::sort(list.begin(), list.end());
    for (int i = list.count()-1; i >= 0; i--){
        tableModel->removeRow(list.at(i).row());
    }
    save();
}

void MainWindow::open()
{
    QString fName = QFileDialog::getOpenFileName(this, utr("Выберите файл"),
                                                      qApp->applicationDirPath(),
                                                      tr("JSON (*.json)"));
    if (!fName.isEmpty()) {
        load(fName);
    }
}

void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    ViewHumanDialog dialog(this, tableModel->getJsonArray().at(index.row()).toObject());
    if (dialog.exec() == QDialog::Accepted){
        tableModel->replaceHuman(index.row(), dialog.getHuman());
        ui->tableView->selectRow(index.row());
        save();
    }
}
