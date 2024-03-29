#include "humandialog.h"
#include "ui_humandialog.h"

#include "constfunction.h"

#include <QMessageBox>
#include <QDebug>
#include <QFile>

HumanDialog::HumanDialog(QWidget *parent, const QJsonObject &human) :
    QDialog(parent),
    ui(new Ui::HumanDialog)
{
    ui->setupUi(this);
    ui->dateEditBirthday->setMaximumDate(QDate::currentDate());
    QFile file(":/national.txt");
    if (file.open(QFile::ReadOnly)) {
        ui->comboBoxNational->clear();
        ui->comboBoxNational->addItems(QString(file.readAll()).split("\n", Qt::SkipEmptyParts));
    }
    if (!human.isEmpty()) {
        ui->spinBoxAge->blockSignals(true);
        ui->dateEditBirthday->blockSignals(true);
        ui->dateEditDeath->blockSignals(true);
        ui->lineEditName->setText(human.value(columnsName[CName]).toString());
        if (human.value(columnsName[CSex]).toString().startsWith(utr("ж"))){
            ui->radioButtonG->setChecked(true);
        }
        ui->spinBoxAge->setValue(human.value(columnsName[CAge]).toInt());
        ui->spinBoxHeight->setValue(human.value(columnsName[CHeight]).toInt());
        ui->doubleSpinBoxWeight->setValue(human.value(columnsName[CWeight]).toDouble());
        int i = ui->comboBoxNational->findText(human.value(columnsName[CNationality]).toString());
        if ( i > -1 ) {
            ui->comboBoxNational->setCurrentIndex(i);
        }
        ui->dateEditBirthday->setDate(human.value(columnsName[CBirthday]).toVariant().toDate());
        ui->dateEditDeath->setMinimumDate(ui->dateEditBirthday->date());
        QString death = human.value(columnsName[CDeath]).toString();
        if (!death.startsWith(utr("Н"))) {
            ui->dateEditDeath->setDate(human.value(columnsName[CDeath]).toVariant().toDate());
        } else {
            ui->dateEditDeath->setDate(ui->dateEditDeath->minimumDate());
        }
        ui->spinBoxAge->blockSignals(false);
        ui->dateEditBirthday->blockSignals(false);
        ui->dateEditDeath->blockSignals(false);
    }
}

HumanDialog::~HumanDialog()
{
    delete ui;
}

QJsonObject HumanDialog::save() const
{
    QJsonObject human;
    human.insert(columnsName[CName], QJsonValue::fromVariant(ui->lineEditName->text()));
    human.insert(columnsName[CSex], QJsonValue::fromVariant(ui->radioButtonM->isChecked()?utr("мужской"):utr("женский")));
    human.insert(columnsName[CAge], QJsonValue::fromVariant(ui->spinBoxAge->value()));
    human.insert(columnsName[CHeight], QJsonValue::fromVariant(ui->spinBoxHeight->value()));
    human.insert(columnsName[CWeight], QJsonValue::fromVariant(ui->doubleSpinBoxWeight->value()));
    human.insert(columnsName[CNationality], QJsonValue::fromVariant(ui->comboBoxNational->currentText()));
    human.insert(columnsName[CBirthday], QJsonValue::fromVariant(ui->dateEditBirthday->date()));
    if (ui->dateEditDeath->date() == ui->dateEditDeath->minimumDate()){
        human.insert(columnsName[CDeath], QJsonValue::fromVariant(ui->dateEditDeath->specialValueText()));
    } else {
        human.insert(columnsName[CDeath], QJsonValue::fromVariant(ui->dateEditDeath->date()));
    }
    return human;
}

void HumanDialog::on_pushButtonSave_clicked()
{
    QDate currentDate = QDate::currentDate();
    QDate date = ui->dateEditBirthday->date();
    if ( ui->lineEditName->text().isEmpty() ) {
        QMessageBox::critical(this, utr("Ввод данных о человеке"), utr("Не введено имя"), QMessageBox::Cancel);
        return;
    } else if ( date.addYears(ui->spinBoxAge->value()) > currentDate ) {
        QMessageBox::critical(this, utr("Ввод данных о человеке"), utr("Возраст человека больше текущей даты"), QMessageBox::Cancel);
        return;
    } else if (currentDate.daysTo(date.addYears(ui->spinBoxAge->value())) > 365 && ui->dateEditDeath->date() == ui->dateEditDeath->minimumDate() ) {
        QMessageBox::critical(this, utr("Ввод данных о человеке"), utr("Не указана дата смерти"), QMessageBox::Cancel);
        return;
    } else {
        if ( ui->dateEditDeath->date() != ui->dateEditDeath->minimumDate()) {
            int year = date.addYears(ui->spinBoxAge->value()).year() - ui->dateEditDeath->date().year();
            qDebug()<<year;
            if ( year > 1 ) {
                QMessageBox::critical(this, utr("Ввод данных о человеке"), utr("Не верно указанна дата смерти"), QMessageBox::Cancel);
                return;
            }
        }
    }
    accept();
}

void HumanDialog::on_spinBoxAge_valueChanged(int age)
{
    QDate date = ui->dateEditBirthday->date();
    QDate currentDate = QDate::currentDate();
    if (ui->dateEditDeath->date() == ui->dateEditDeath->minimumDate() ) {
        date = date.addYears(currentDate.year() - date.year() - age);
        ui->dateEditDeath->blockSignals(true);
        ui->dateEditDeath->setMinimumDate(date);
        ui->dateEditDeath->setDate(date);
        ui->dateEditDeath->blockSignals(false);
    } else {
        date = ui->dateEditDeath->date().addYears(-age);
        ui->dateEditDeath->setMinimumDate(date);
    }
    ui->dateEditBirthday->blockSignals(true);
    ui->dateEditBirthday->setDate(date);
    ui->dateEditBirthday->blockSignals(false);
}

void HumanDialog::on_dateEditBirthday_dateChanged(const QDate &date)
{
    ui->dateEditDeath->setMinimumDate(date);
    ui->spinBoxAge->blockSignals(true);
    if (ui->dateEditDeath->date() > date) {
        ui->spinBoxAge->setValue(date.daysTo(ui->dateEditDeath->date()) / 365 );
    } else {
        ui->spinBoxAge->setValue(date.daysTo(QDate::currentDate()) / 365 );
    }
    ui->spinBoxAge->blockSignals(false);
}

void HumanDialog::on_dateEditDeath_dateChanged(const QDate &date)
{
    ui->spinBoxAge->blockSignals(true);
    if ( date == ui->dateEditDeath->minimumDate() ) {
        ui->spinBoxAge->setValue(ui->dateEditBirthday->date().daysTo(QDate::currentDate())/365);
    } else {
        ui->spinBoxAge->setValue(ui->dateEditBirthday->date().daysTo(date)/365);
    }
    ui->spinBoxAge->blockSignals(false);
}
