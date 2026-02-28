#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "loginwindow.h"
#include "dialogcreatetable.h"
#include "dialogaddemployee.h"

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>
#include <QDebug>
#include <QMessageBox>
#include <QListWidget>
#include <QFileDialog>
#include <QSqlRecord>

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

    void on_pb_expor_to_file_clicked();

    void on_btn_delete_row_clicked();

    void on_tableView_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    LoginWindow login_window;
    DialogCreateTable dial_create_table;
    DialogAddEmployee dial_add_emp;
    QSqlTableModel * model = nullptr;
    QString curr_table;
    int curr_row;

    void ProcessLogin (const QString& name_db, const QString& login, const QString& pass);
    void ConnetDB(const QString& name_db, const QString& login, const QString& pass);
    void CreateTable (const QString& table_name);
    void ShowTables();
    void AddEmployee (const QString& name, const QString& last_name, const QString& tel);
    void ExportTofile(const QString& file_name);
    void DeleteRow ();
};
#endif // MAINWINDOW_H
