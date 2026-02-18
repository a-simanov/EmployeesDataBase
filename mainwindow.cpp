#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&login_window, &LoginWindow::LoginAccept, this, &MainWindow::ProcessLogin);
    connect(&dial_create_table, &DialogCreateTable::TableNameAccept, this, &MainWindow::CreateTable);
    this->hide();
    login_window.show();
    dial_create_table.hide();
    ui->tableView->hide();
    ui->btn_add_into_table->hide();
    ui->btn_clear_table->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ProcessLogin(const QString& name_db, const QString& login, const QString& pass) {
    ConnetDB(name_db, login, pass);
    login_window.hide();
    this->show();
}

void MainWindow::ConnetDB(const QString& name_db, const QString& login, const QString& pass) {
    const QString connectionName = "MainMySqlConn";
    if (QSqlDatabase::contains(connectionName)) {
        {
            QSqlDatabase db_old = QSqlDatabase::database(connectionName);
            db_old.close();
        }
        QSqlDatabase::removeDatabase(connectionName);
    }
    QSqlDatabase::removeDatabase(connectionName);
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", connectionName);
    db.setHostName("localhost");
    db.setDatabaseName(name_db);
    db.setUserName(login);
    db.setPassword(pass);
    db.setPort(3306);

    if (!db.open()) {
        qDebug() << "Ошибка подключения:" << db.lastError().text();
        QMessageBox::warning(this, "Error","Incorrect DB name, login or pass!");
        login_window.show();
        this->hide();
    } else {
        qDebug() << "Успешное подключение!";
        login_window.hide();
        this->show();
        ShowTables();
    }
}

void MainWindow::CreateTable (const QString& table_name) {
    QSqlDatabase db = QSqlDatabase::database("MainMySqlConn");

    if (!db.isOpen()) {
        qDebug() << "База не открыта!";
        return;
    }
    QSqlQuery query(db);
    QString str = QString("CREATE TABLE IF NOT EXISTS %1("
                "id INT AUTO_INCREMENT PRIMARY KEY, "
                "Firstname VARCHAR(20), "
                "Lastname VARCHAR(20), "
                "Tel INT"
                ");").arg(table_name);
    if (!query.exec(str)) {
        qDebug() << "Ошибка создания таблицы:" << query.lastError().text();
    }
    if (model) {
        ui->tableView->setModel(nullptr);
        model->deleteLater();
        model = nullptr;
    }
    model = new QSqlTableModel(this, db);
    model->setTable("TelBook");
    model->select();
    ui->tableView->setModel(model);
    ShowTables();

}

void MainWindow::on_btn_clear_table_clicked()
{
    QSqlDatabase db = QSqlDatabase::database("MainMySqlConn");

    if (!db.isOpen()) {
        qDebug() << "База не открыта!";
        return;
    }

    QSqlQuery query(db);
    if (query.exec("DELETE FROM TelBook")) {
        qDebug() << "Table clear!";
        if (model) {
            model->select();
        }
    } else {
        qDebug() << "Ошибка очистки:" << query.lastError().text();
    }
}

void MainWindow::ShowTables() {
    QSqlDatabase db = QSqlDatabase::database("MainMySqlConn");
    if (!db.isOpen()) {
        qDebug() << "База не открыта!";
        return;
    }
    ui->lw_tables->clear();
    QStringList tables = db.tables();
    ui->lw_tables->addItems(tables);    
}


void MainWindow::on_lw_tables_itemDoubleClicked(QListWidgetItem *item)
{
    QSqlDatabase db = QSqlDatabase::database("MainMySqlConn");

    if (!db.isOpen()) {
        qDebug() << "База не открыта!";
        return;
    }
    curr_table = item->text();
    ui->tableView->show();
    if (model) {
        ui->tableView->setModel(nullptr);
        model->deleteLater();
        model = nullptr;
    }
    model = new QSqlTableModel(this, db);
    model->setTable(curr_table);
    model->select();
    ui->tableView->setModel(model);
    ui->btn_add_into_table->show();
    ui->btn_clear_table->show();
}


void MainWindow::on_btn_add_into_table_clicked()
{
    QSqlDatabase db = QSqlDatabase::database("MainMySqlConn");

    if (!db.isOpen()) {
        qDebug() << "База не открыта!";
        return;
    }
    QSqlQuery query(db);
    query.exec(QString("INSERT INTO %1(Firstname, Lastname, Tel) VALUES ('Tom', 'Berenger', 41), ('Kot', 'Govnyuk', 2), ('Ya', 'Net', 35)").arg(curr_table));
    model->select();
}


void MainWindow::on_btn_create_tbl_clicked()
{
    dial_create_table.show();
}

