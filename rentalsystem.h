#ifndef RENTALSYSTEM_H
#define RENTALSYSTEM_H

#include <vector>
#include <string>

struct Motor {
    int id;
    std::string merk;
    std::string plat;
    int harga;
    bool tersedia;
    int id_penyewa;
    int lama_sewa;
};

struct Penyewa {
    int id;
    std::string nama;
    std::string kontak;
};

struct Transaksi {
    int idTransaksi;
    int idMotor;
    int idPenyewa;
    std::string tanggal;
    int lamaSewa;
    long long totalBiaya;
    bool isSelesai;
};

class RentalSystem {
public:
    RentalSystem();
    // motor
    bool addMotor(int id, const std::string &merk, const std::string &plat, int harga);
    bool deleteMotor(int id);
    void editMotorBasic(int id, const std::string &merk, int harga);
    std::vector<Motor> getAllMotor() const;
    bool isMotorAvailable(int id) const;

    // penyewa
    bool addPenyewa(int id, const std::string &nama, const std::string &kontak);
    bool deletePenyewa(int id);
    void editPenyewa(int id, const std::string &nama, const std::string &kontak);
    std::vector<Penyewa> getAllPenyewa() const;

    // transaksi
    bool sewaMotor(int idMotor, int idPenyewa, const std::string &tgl, int hari);
    bool kembalikanMotor(int idMotor);
    std::vector<Transaksi> getAllTransaksi() const;

    // persistence
    void saveFile() const;
    void loadFile();

private:
    std::vector<Motor> listMotor;
    std::vector<Penyewa> listPenyewa;
    std::vector<Transaksi> listTransaksi;

    int getNextIdTransaksi() const;
    bool motorIdExists(int id) const;
    bool penyewaIdExists(int id) const;
};

#endif // RENTALSYSTEM_H
