#include "dialogcreatetable.h"
#include "ui_dialogcreatetable.h"

DialogCreateTable::DialogCreateTable(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogCreateTable)
{
    ui->setupUi(this);
}

DialogCreateTable::~DialogCreateTable()
{
    delete ui;
}

void DialogCreateTable::on_buttonBox_accepted()
{
    emit TableNameAccept(ui->le_table_name->text());
}

