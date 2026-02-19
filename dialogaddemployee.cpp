#include "dialogaddemployee.h"
#include "ui_dialogaddemployee.h"

DialogAddEmployee::DialogAddEmployee(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogAddEmployee)
{
    ui->setupUi(this);
    QRegularExpression re("^[A-Za-zА-Яа-яЁё ]+$");
    QRegularExpressionValidator *reg_valid = new QRegularExpressionValidator(re, this);
    ui->le_name->setValidator(reg_valid);
    ui->le_last_name->setValidator(reg_valid);
    ui->le_tel->setValidator(new QIntValidator(1000000,9999999));
}

DialogAddEmployee::~DialogAddEmployee()
{
    delete ui;
}

void DialogAddEmployee::on_buttonBox_accepted()
{
    emit GetEmployeeInfo(ui->le_name->text(), ui->le_last_name->text(), ui->le_tel->text());
}

