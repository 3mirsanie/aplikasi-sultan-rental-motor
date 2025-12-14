#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    setWindowTitle("Login Admin");
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_loginBtn_clicked()
{
    QString u = ui->username->text();
    QString p = ui->password->text();

    if (u == adminUser && p == adminPass) {
        accept();
    } else {
        QMessageBox::warning(this, "Login Gagal", "Username atau password salah!");
    }
}

void LoginDialog::on_cancelBtn_clicked()
{
    reject();
}
