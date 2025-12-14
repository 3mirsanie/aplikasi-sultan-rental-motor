#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    rental.loadFile();
    refreshMotorList();
    refreshPenyewaList();
    refreshTransaksiList();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showError(const QString &msg){
    QMessageBox::warning(this, "Error", msg);
}

// --- Motor slots ---
void MainWindow::on_addMotorBtn_clicked()
{
    bool ok;
    int id = QInputDialog::getInt(this, "Tambah Motor", "ID Motor:", 1, 1, 1000000, 1, &ok);
    if(!ok) return;
    QString merk = QInputDialog::getText(this, "Tambah Motor", "Merk:", QLineEdit::Normal, "", &ok);
    if(!ok || merk.isEmpty()){ showError("Merk kosong"); return; }
    QString plat = QInputDialog::getText(this, "Tambah Motor", "Plat:", QLineEdit::Normal, "", &ok);
    if(!ok || plat.isEmpty()){ showError("Plat kosong"); return; }
    int harga = QInputDialog::getInt(this, "Tambah Motor", "Harga/hari:", 10000, 0, 100000000, 1000, &ok);
    if(!ok) return;

    bool res = rental.addMotor(id, merk.toStdString(), plat.toStdString(), harga);
    if(!res) showError("Gagal tambah: ID sudah ada");
    refreshMotorList();
}

void MainWindow::on_editMotorBtn_clicked()
{
    auto item = ui->motorList->currentItem();
    if(!item){ showError("Pilih motor terlebih dahulu"); return; }
    int id = item->data(Qt::UserRole).toInt();
    QString merk = QInputDialog::getText(this, "Edit Motor", "Merk:", QLineEdit::Normal, item->text(), nullptr);
    // For simplicity just allow changing merk and harga
    bool ok;
    int harga = QInputDialog::getInt(this, "Edit Motor", "Harga/hari:", 10000, 0, 100000000, 1000, &ok);
    if(!ok) return;
    rental.editMotorBasic(id, merk.toStdString(), harga);
    refreshMotorList();
}

void MainWindow::on_deleteMotorBtn_clicked()
{
    auto item = ui->motorList->currentItem();
    if(!item){ showError("Pilih motor terlebih dahulu"); return; }
    int id = item->data(Qt::UserRole).toInt();
    QString confirm = QInputDialog::getText(this, "Hapus Motor", "Ketik 'YA' untuk konfirmasi:");
    if(confirm != "YA") return;
    bool res = rental.deleteMotor(id);
    if(!res) showError("Gagal hapus: motor mungkin sedang disewa atau tidak ada");
    refreshMotorList();
}

// --- Penyewa slots ---
void MainWindow::on_addPenyewaBtn_clicked()
{
    bool ok;
    int id = QInputDialog::getInt(this, "Tambah Penyewa", "ID Penyewa:", 1, 1, 1000000, 1, &ok);
    if(!ok) return;
    QString nama = QInputDialog::getText(this, "Tambah Penyewa", "Nama:", QLineEdit::Normal, "", &ok);
    if(!ok || nama.isEmpty()){ showError("Nama kosong"); return; }
    QString kontak = QInputDialog::getText(this, "Tambah Penyewa", "Kontak:", QLineEdit::Normal, "", &ok);
    if(!ok || kontak.isEmpty()){ showError("Kontak kosong"); return; }

    bool res = rental.addPenyewa(id, nama.toStdString(), kontak.toStdString());
    if(!res) showError("Gagal tambah: ID sudah ada");
    refreshPenyewaList();
}

void MainWindow::on_editPenyewaBtn_clicked()
{
    auto item = ui->penyewaList->currentItem();
    if(!item){ showError("Pilih penyewa terlebih dahulu"); return; }
    int id = item->data(Qt::UserRole).toInt();
    bool ok;
    QString nama = QInputDialog::getText(this, "Edit Penyewa", "Nama:", QLineEdit::Normal, item->text(), &ok);
    if(!ok) return;
    QString kontak = QInputDialog::getText(this, "Edit Penyewa", "Kontak:", QLineEdit::Normal, "", &ok);
    if(!ok) return;
    rental.editPenyewa(id, nama.toStdString(), kontak.toStdString());
    refreshPenyewaList();
}

void MainWindow::on_deletePenyewaBtn_clicked()
{
    auto item = ui->penyewaList->currentItem();
    if(!item){ showError("Pilih penyewa terlebih dahulu"); return; }
    int id = item->data(Qt::UserRole).toInt();
    QString confirm = QInputDialog::getText(this, "Hapus Penyewa", "Ketik 'YA' untuk konfirmasi:");
    if(confirm != "YA") return;
    bool res = rental.deletePenyewa(id);
    if(!res) showError("Gagal hapus: Penyewa sedang menyewa atau tidak ada");
    refreshPenyewaList();
}

void MainWindow::on_sewaBtn_clicked()
{
    // choose motor (available), choose penyewa
    QListWidgetItem *mItem = ui->motorList->currentItem();
    if(!mItem){ showError("Pilih motor yang tersedia pada list Motor"); return; }
    int idM = mItem->data(Qt::UserRole).toInt();
    if(!rental.isMotorAvailable(idM)){ showError("Motor tidak tersedia"); return; }

    QListWidgetItem *pItem = ui->penyewaList->currentItem();
    int idP = -1;
    if(pItem) idP = pItem->data(Qt::UserRole).toInt();
    else {
        bool ok;
        idP = QInputDialog::getInt(this, "Penyewa Baru?", "Masukkan ID Penyewa baru:", 1, 1, 1000000, 1, &ok);
        if(!ok) return;
        QString nama = QInputDialog::getText(this, "Nama:", "Nama:");
        QString kontak = QInputDialog::getText(this, "Kontak:", "Kontak:");
        rental.addPenyewa(idP, nama.toStdString(), kontak.toStdString());
        refreshPenyewaList();
    }

    bool ok;
    int hari = QInputDialog::getInt(this, "Lama Sewa", "Hari:", 1, 1, 365, 1, &ok);
    if(!ok) return;
    QString tgl = QInputDialog::getText(this, "Tanggal Sewa", "DD-MM-YYYY:", QLineEdit::Normal, "01-01-2025", &ok);
    if(!ok) return;

    bool res = rental.sewaMotor(idM, idP, tgl.toStdString(), hari);
    if(!res) showError("Gagal sewa: Periksa kembali data");
    refreshMotorList();
    refreshTransaksiList();
}

void MainWindow::on_kembaliBtn_clicked()
{
    auto item = ui->motorList->currentItem();
    if(!item){ showError("Pilih motor yang ingin dikembalikan"); return; }
    int id = item->data(Qt::UserRole).toInt();
    bool res = rental.kembalikanMotor(id);
    if(!res) showError("Gagal mengembalikan motor (mungkin sudah tersedia atau tidak ada)");
    refreshMotorList();
    refreshTransaksiList();
}

void MainWindow::on_saveBtn_clicked()
{
    rental.saveFile();
    QMessageBox::information(this, "Simpan", "Data berhasil disimpan.");
}

// --- Refresh helpers ---
void MainWindow::refreshMotorList()
{
    ui->motorList->clear();
    for(const auto &m : rental.getAllMotor()){
        QString txt = QString::fromStdString(m.merk + " | " + m.plat + " | Rp " + std::to_string(m.harga));
        QListWidgetItem *it = new QListWidgetItem(txt);
        it->setData(Qt::UserRole, m.id);
        if(!m.tersedia) it->setForeground(Qt::red);
        ui->motorList->addItem(it);
    }
}

void MainWindow::refreshPenyewaList()
{
    ui->penyewaList->clear();
    for(const auto &p : rental.getAllPenyewa()){
        QString txt = QString::fromStdString(p.nama + " | " + p.kontak);
        QListWidgetItem *it = new QListWidgetItem(txt);
        it->setData(Qt::UserRole, p.id);
        ui->penyewaList->addItem(it);
    }
}

void MainWindow::refreshTransaksiList()
{
    ui->transaksiList->clear();
    for(const auto &t : rental.getAllTransaksi()){
        QString txt = QString::fromStdString("TRX-" + std::to_string(t.idTransaksi) + " | M:" + std::to_string(t.idMotor) + " | P:" + std::to_string(t.idPenyewa) + " | " + t.tanggal + " | Rp " + std::to_string(t.totalBiaya) + " | " + (t.isSelesai?"SELESAI":"AKTIF"));
        QListWidgetItem *it = new QListWidgetItem(txt);
        it->setData(Qt::UserRole, t.idTransaksi);
        if(!t.isSelesai) it->setForeground(Qt::blue);
        ui->transaksiList->addItem(it);
    }
}
