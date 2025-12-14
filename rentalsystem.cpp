#include "rentalsystem.h"
#include <fstream>
#include <algorithm>

RentalSystem::RentalSystem(){}

bool RentalSystem::motorIdExists(int id) const {
    for(const auto &m : listMotor) if(m.id == id) return true;
    return false;
}
bool RentalSystem::penyewaIdExists(int id) const {
    for(const auto &p : listPenyewa) if(p.id == id) return true;
    return false;
}

bool RentalSystem::addMotor(int id, const std::string &merk, const std::string &plat, int harga){
    if(motorIdExists(id)) return false;
    Motor m; m.id = id; m.merk = merk; m.plat = plat; m.harga = harga; m.tersedia = true; m.id_penyewa = 0; m.lama_sewa = 0;
    listMotor.push_back(m);
    return true;
}

bool RentalSystem::deleteMotor(int id){
    for(auto it = listMotor.begin(); it != listMotor.end(); ++it){
        if(it->id == id){
            if(!it->tersedia) return false;
            listMotor.erase(it);
            return true;
        }
    }
    return false;
}

void RentalSystem::editMotorBasic(int id, const std::string &merk, int harga){
    for(auto &m : listMotor){
        if(m.id == id){
            if(!merk.empty()) m.merk = merk;
            m.harga = harga;
            return;
        }
    }
}

std::vector<Motor> RentalSystem::getAllMotor() const { return listMotor; }
bool RentalSystem::isMotorAvailable(int id) const {
    for(const auto &m : listMotor) if(m.id == id) return m.tersedia;
    return false;
}

bool RentalSystem::addPenyewa(int id, const std::string &nama, const std::string &kontak){
    if(penyewaIdExists(id)) return false;
    Penyewa p; p.id = id; p.nama = nama; p.kontak = kontak;
    listPenyewa.push_back(p);
    return true;
}

bool RentalSystem::deletePenyewa(int id){
    for(const auto &m : listMotor) if(m.id_penyewa == id) return false;
    for(auto it = listPenyewa.begin(); it != listPenyewa.end(); ++it){
        if(it->id == id){ listPenyewa.erase(it); return true; }
    }
    return false;
}

void RentalSystem::editPenyewa(int id, const std::string &nama, const std::string &kontak){
    for(auto &p : listPenyewa) if(p.id == id){ if(!nama.empty()) p.nama = nama; if(!kontak.empty()) p.kontak = kontak; return; }
}

int RentalSystem::getNextIdTransaksi() const {
    int maxId = 0;
    for(const auto &t : listTransaksi) if(t.idTransaksi > maxId) maxId = t.idTransaksi;
    return maxId + 1;
}

bool RentalSystem::sewaMotor(int idMotor, int idPenyewa, const std::string &tgl, int hari){
    for(auto &m : listMotor){
        if(m.id == idMotor){
            if(!m.tersedia) return false;
            // ensure penyewa exists
            if(!penyewaIdExists(idPenyewa)) return false;
            m.tersedia = false;
            m.id_penyewa = idPenyewa;
            m.lama_sewa = hari;
            long long biaya = (long long)m.harga * hari;
            Transaksi trx;
            trx.idTransaksi = getNextIdTransaksi();
            trx.idMotor = idMotor;
            trx.idPenyewa = idPenyewa;
            trx.tanggal = tgl;
            trx.lamaSewa = hari;
            trx.totalBiaya = biaya;
            trx.isSelesai = false;
            listTransaksi.push_back(trx);
            return true;
        }
    }
    return false;
}

bool RentalSystem::kembalikanMotor(int idMotor){
    for(auto &m : listMotor){
        if(m.id == idMotor){
            if(m.tersedia) return false;
            // close transaction
            for(auto &t : listTransaksi){
                if(t.idMotor == idMotor && !t.isSelesai){
                    t.isSelesai = true;
                    break;
                }
            }
            m.tersedia = true;
            m.id_penyewa = 0;
            m.lama_sewa = 0;
            return true;
        }
    }
    return false;
}

std::vector<Transaksi> RentalSystem::getAllTransaksi() const { return listTransaksi; }
std::vector<Penyewa> RentalSystem::getAllPenyewa() const { return listPenyewa; }

// Persistence (simple text files)
void RentalSystem::saveFile() const {
    std::ofstream fMotor("motor_data.txt");
    if(fMotor.is_open()){
        for(const auto &m : listMotor){
            fMotor << m.id << std::endl << m.merk << std::endl << m.plat << std::endl << m.harga << std::endl << m.tersedia << std::endl << m.id_penyewa << std::endl << m.lama_sewa << std::endl;
        }
        fMotor.close();
    }
    std::ofstream fPenyewa("penyewa_data.txt");
    if(fPenyewa.is_open()){
        for(const auto &p : listPenyewa){
            fPenyewa << p.id << std::endl << p.nama << std::endl << p.kontak << std::endl;
        }
        fPenyewa.close();
    }
    std::ofstream fTrx("transaksi_data.txt");
    if(fTrx.is_open()){
        for(const auto &t : listTransaksi){
            fTrx << t.idTransaksi << std::endl << t.idMotor << std::endl << t.idPenyewa << std::endl << t.tanggal << std::endl << t.lamaSewa << std::endl << t.totalBiaya << std::endl << t.isSelesai << std::endl;
        }
        fTrx.close();
    }
}

void RentalSystem::loadFile(){
    // motor
    listMotor.clear(); listPenyewa.clear(); listTransaksi.clear();
    std::ifstream fMotor("motor_data.txt");
    if(fMotor.is_open()){
        int id, harga, idp, ls; bool av; std::string mk, pl;
        while(fMotor >> id){
            fMotor.ignore(); std::getline(fMotor, mk); std::getline(fMotor, pl);
            fMotor >> harga >> av >> idp >> ls;
            Motor m; m.id = id; m.merk = mk; m.plat = pl; m.harga = harga; m.tersedia = av; m.id_penyewa = idp; m.lama_sewa = ls;
            listMotor.push_back(m);
        }
        fMotor.close();
    }
    std::ifstream fPenyewa("penyewa_data.txt");
    if(fPenyewa.is_open()){
        int id; std::string nm, kt;
        while(fPenyewa >> id){
            fPenyewa.ignore(); std::getline(fPenyewa, nm); std::getline(fPenyewa, kt);
            Penyewa p; p.id = id; p.nama = nm; p.kontak = kt;
            listPenyewa.push_back(p);
        }
        fPenyewa.close();
    }
    std::ifstream fTrx("transaksi_data.txt");
    if(fTrx.is_open()){
        int idT, idM, idP, lm; long long tot; bool sel; std::string tgl;
        while(fTrx >> idT){
            fTrx >> idM >> idP;
            fTrx.ignore(); std::getline(fTrx, tgl);
            fTrx >> lm >> tot >> sel;
            Transaksi t; t.idTransaksi = idT; t.idMotor = idM; t.idPenyewa = idP; t.tanggal = tgl; t.lamaSewa = lm; t.totalBiaya = tot; t.isSelesai = sel;
            listTransaksi.push_back(t);
        }
        fTrx.close();
    }
}
