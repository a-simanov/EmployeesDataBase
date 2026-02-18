#ifndef DIALOGCREATETABLE_H
#define DIALOGCREATETABLE_H

#include <QDialog>

namespace Ui {
class DialogCreateTable;
}

class DialogCreateTable : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCreateTable(QWidget *parent = nullptr);
    ~DialogCreateTable();
signals:
    void TableNameAccept (const QString& name_table);
private slots:
    void on_buttonBox_accepted();

private:
    Ui::DialogCreateTable *ui;
};

#endif // DIALOGCREATETABLE_H
