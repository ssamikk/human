#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class TableModel;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool load(QString fileName = QString());
    bool save();
public slots:
    void addHuman();
    void dellHuman();
    void open();
private slots:
    void on_tableView_doubleClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    TableModel* tableModel;
    QString fileName;
};



#endif // MAINWINDOW_H
