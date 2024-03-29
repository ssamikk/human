#include "viewhumandialog.h"
#include "ui_viewhumandialog.h"
#include "constfunction.h"
#include "humandialog.h"

ViewHumanDialog::ViewHumanDialog(QWidget *parent, const QJsonObject &human) :
    QDialog(parent),
    ui(new Ui::ViewHumanDialog), human(human)
{
    ui->setupUi(this);
    load();
}

ViewHumanDialog::~ViewHumanDialog()
{
    delete ui;
}

void ViewHumanDialog::on_pushButtonEdit_clicked()
{
    HumanDialog dialog(this, human);
    if (dialog.exec() == QDialog::Accepted){
        human = dialog.save();
        load();
    }
}

void ViewHumanDialog::load()
{
    ui->label->setText(utr("<b>Имя:</b> %1 <b>Пол: </b> %2 <br/>"
                           "<b>Возраст:</b> %3 <b>Рост:</b> %4 <b>Вес:</b> %5 <br/>"
                           "<b>Национальность:</b> %6 <br/>"
                           "<b>Дата рождения:</b> %7 %8").arg(human.value(columnsName[CName]).toString(),
                                                              human.value(columnsName[CSex]).toString())
                       .arg(human.value(columnsName[CAge]).toInt())
                       .arg(human.value(columnsName[CHeight]).toInt())
                       .arg(human.value(columnsName[CWeight]).toInt())
                       .arg(human.value(columnsName[CNationality]).toString())
                       .arg(human.value(columnsName[CBirthday]).toString())
                       .arg(human.value(columnsName[CDeath]).toString().startsWith(utr("Н"))? "<b>" + human.value(columnsName[CDeath]).toString() + "<b>" :
                                                                                              utr("<b>Дата смерти:</b> ")+human.value(columnsName[CDeath]).toString()));
}

QJsonObject ViewHumanDialog::getHuman() const
{
    return human;
}
