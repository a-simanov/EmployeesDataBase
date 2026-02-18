#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "loginwindow.h"
#include "dialogcreatetable.h"

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>
#include <QDebug>
#include <QMessageBox>
#include <QListWidget>>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_clear_table_clicked();

    void on_lw_tables_itemDoubleClicked(QListWidgetItem *item);

    void on_btn_add_into_table_clicked();

    void on_btn_create_tbl_clicked();

private:
    Ui::MainWindow *ui;
    LoginWindow login_window;
    DialogCreateTable dial_create_table;
    QSqlTableModel * model = nullptr;
    QString curr_table;

    void ProcessLogin (const QString& name_db, const QString& login, const QString& pass);
    void ConnetDB(const QString& name_db, const QString& login, const QString& pass);
    void CreateTable (const QString& table_name);
    void ShowTables();
};
#endif // MAINWINDOW_H
