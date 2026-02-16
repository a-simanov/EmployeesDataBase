#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
namespace Ui {
class LoginWindow;
}

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();
signals:
    void LoginAccept (const QString& name_db, const QString& login, const QString& pass);
private slots:
    void on_buttonBox_rejected();

    void on_buttonBox_accepted();

private:
    Ui::LoginWindow *ui;
};

#endif // LOGINWINDOW_H
