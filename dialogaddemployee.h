#ifndef DIALOGADDEMPLOYEE_H
#define DIALOGADDEMPLOYEE_H

#include <QDialog>
#include <QIntValidator>
#include <QRegularExpressionValidator>
#include <QRegularExpression>

namespace Ui {
class DialogAddEmployee;
}

class DialogAddEmployee : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddEmployee(QWidget *parent = nullptr);
    ~DialogAddEmployee();
signals:
    void GetEmployeeInfo(const QString& name, const QString& last_name, const QString& tel);
private slots:
    void on_buttonBox_accepted();

private:
    Ui::DialogAddEmployee *ui;    
};

#endif // DIALOGADDEMPLOYEE_H
