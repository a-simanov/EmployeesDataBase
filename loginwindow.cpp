#include "loginwindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_buttonBox_rejected()
{
    QApplication::quit();
}


void LoginWindow::on_buttonBox_accepted()
{
    emit LoginAccept(ui->le_db_name->text(), ui->le_login->text(), ui->le_pass->text());
}


