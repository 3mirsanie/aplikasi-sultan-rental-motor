#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private slots:
    void on_loginBtn_clicked();
    void on_cancelBtn_clicked();

private:
    Ui::LoginDialog *ui;
    const QString adminUser = "admin";
    const QString adminPass = "123";
};

#endif // LOGINDIALOG_H
