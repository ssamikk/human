#ifndef VIEWHUMANDIALOG_H
#define VIEWHUMANDIALOG_H

#include <QDialog>
#include <QJsonObject>

namespace Ui {
class ViewHumanDialog;
}

class ViewHumanDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ViewHumanDialog(QWidget *parent = 0, const QJsonObject &human = QJsonObject());
    ~ViewHumanDialog();
    QJsonObject getHuman() const;

private slots:
    void on_pushButtonEdit_clicked();
private:
    void load();
    Ui::ViewHumanDialog *ui;
    QJsonObject human;
};

#endif // VIEWHUMANDIALOG_H
