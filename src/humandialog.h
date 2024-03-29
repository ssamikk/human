#ifndef HUMANDIALOG_H
#define HUMANDIALOG_H

#include <QDialog>
#include <QJsonObject>

namespace Ui {
class HumanDialog;
}

class HumanDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HumanDialog(QWidget *parent = 0, const QJsonObject &human = QJsonObject() );
    ~HumanDialog();
    QJsonObject save() const;
private slots:
    void on_pushButtonSave_clicked();

    void on_spinBoxAge_valueChanged(int age);

    void on_dateEditBirthday_dateChanged(const QDate &date);

    void on_dateEditDeath_dateChanged(const QDate &date);

private:
    Ui::HumanDialog *ui;
};

#endif // HUMANDIALOG_H
