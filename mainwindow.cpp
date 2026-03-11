#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&login_window, &LoginWindow::LoginAccept, this, &MainWindow::ProcessLogin);
    connect(&dial_create_table, &DialogCreateTable::TableNameAccept, this, &MainWindow::CreateTable);
    connect(&dial_add_emp, &DialogAddEmployee::GetEmployeeInfo, this, &MainWindow::AddEmployee);
    this->hide();
    login_window.show();
    dial_create_table.hide();
    dial_add_emp.hide();
    ui->tableView->hide();
    ui->gb_table_btns->hide();
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
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", connectionName);
    db.setHostName("localhost");
    db.setDatabaseName(name_db);
    db.setUserName(login);
    db.setPassword(pass);
    db.setPort(3306);

    if (!db.open()) {
        qDebug() << "Connection error:" << db.lastError().text();
        QMessageBox::warning(this, "Error","Incorrect DB name, login or pass!");
        login_window.show();
        this->hide();
    } else {
        qDebug() << "Connection completed!";
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
        qDebug() << "Error creating table:" << query.lastError().text();
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
        qDebug() << "BD don't open!";
        return;
    }

    QSqlQuery query(db);
    if (query.exec(QString("TRUNCATE TABLE %1").arg(curr_table))) {
        qDebug() << QString("Table %1 clear!").arg(curr_table);
        if (model) {
            model->select();
        }
    } else {
        qDebug() << "Clear table error:" << query.lastError().text();
    }
}

void MainWindow::ShowTables() {
    QSqlDatabase db = QSqlDatabase::database("MainMySqlConn");
    if (!db.isOpen()) {
        qDebug() << "BD don't open!";
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
        qDebug() << "BD don't open!!";
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
    ui->gb_table_btns->show();
}

void MainWindow::AddEmployee (const QString& name, const QString& last_name, const QString& tel){
    QSqlDatabase db = QSqlDatabase::database("MainMySqlConn");

    if (!db.isOpen()) {
        qDebug() << "BD don't open!";
        return;
    }
    if (!db.transaction()) {
        qDebug() << "Can't start transaction: " << db.lastError().text();
        return;
    }
    QSqlQuery query(db);
    query.prepare(QString("INSERT INTO %1(Firstname, Lastname, Tel) VALUES (:first, :last, :tel)").arg(curr_table));
    query.bindValue(":first", name);
    query.bindValue(":last", last_name);
    query.bindValue(":tel", tel);
    if(query.exec()) {
        if (db.commit()) {
            qDebug() << "Employee added and committed";
            if (model) {
                model->select();
            }
        } else {
            qDebug() << "Commit error:" << db.lastError().text();
            db.rollback();
        }
    } else {
        qDebug() << "Error add employee";
        db.rollback();
    }
}

void MainWindow::ExportTofile(const QString& file_name) {
    QSqlDatabase db = QSqlDatabase::database("MainMySqlConn");
    if (!db.isOpen()) return;

    QFile file(file_name);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Can' open file!";
        return;
    }

    QTextStream out(&file);
    QSqlQuery query(QString("SELECT * FROM %1").arg(curr_table), db);

    QSqlRecord rec = query.record();
    int cols = rec.count();

    for (int i = 0; i < cols; ++i) {
        out << rec.fieldName(i) << (i == cols - 1 ? "" : ",");
    }
    out << "\n";

    while (query.next()) {
        for (int i = 0; i < cols; ++i) {
            QString value = query.value(i).toString();
            if (value.contains(',')) value = "\"" + value + "\"";
            out << value << (i == cols - 1 ? "" : ",");
        }
        out << "\n";
    }

    file.close();
    qDebug() << "Export to file completed successfully!";

}

void MainWindow::DeleteRow () {
    QSqlDatabase db = QSqlDatabase::database("MainMySqlConn");

    if (!db.isOpen()) {
        qDebug() << "База не открыта!";
        return;
    }
    if (!db.transaction()) {
        qDebug() << "Can't start transaction: " << db.lastError().text();
        return;
    }
    QSqlQuery query(db);
    query.prepare(QString("DELETE FROM %1 WHERE id = :id").arg(curr_table));
    query.bindValue(":id", curr_row);

    if(!query.exec()) {
        qDebug() << "Deleting row error" << query.lastError().text();
        db.rollback();
        return;
    }
    query.prepare(QString("UPDATE %1 SET id = id - 1 WHERE id > :id").arg(curr_table));
    query.bindValue(":id", curr_row);
    if(!query.exec()) {
        qDebug() << "Update ID error" << query.lastError().text();
        db.rollback();
        return;
    }

    if(!db.commit()) {
        qDebug() << "Commit error!" << db.lastError().text();
        db.rollback();
        return;
    }

    query.prepare(QString("ALTER TABLE %1 AUTO_INCREMENT = 1").arg(curr_table));
    if(query.exec()) {
        qDebug() << "Employee added and committed";
            if (model) {
                model->select();
            }
    } else {
        qDebug() << "Commit error:" << db.lastError().text();
    }

}

void MainWindow::on_btn_add_into_table_clicked()
{
    dial_add_emp.show();
}


void MainWindow::on_btn_create_tbl_clicked()
{
    dial_create_table.show();
}


void MainWindow::on_pb_expor_to_file_clicked()
{
    QString file_name = QFileDialog::getSaveFileName(this, QString("Save file"), QDir::currentPath(), tr("*.txt"));
    ExportTofile(file_name);
}


void MainWindow::on_btn_delete_row_clicked()
{
    DeleteRow();
}


void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    curr_row = index.row() + 1;
}

