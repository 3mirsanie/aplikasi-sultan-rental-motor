#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "rentalsystem.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Motor
    void on_addMotorBtn_clicked();
    void on_editMotorBtn_clicked();
    void on_deleteMotorBtn_clicked();
    void refreshMotorList();

    // Penyewa
    void on_addPenyewaBtn_clicked();
    void on_editPenyewaBtn_clicked();
    void on_deletePenyewaBtn_clicked();
    void refreshPenyewaList();

    // Transaksi
    void on_sewaBtn_clicked();
    void on_kembaliBtn_clicked();
    void refreshTransaksiList();

    void on_saveBtn_clicked();

private:
    Ui::MainWindow *ui;
    RentalSystem rental;
    void showError(const QString &msg);
};

#endif // MAINWINDOW_H
