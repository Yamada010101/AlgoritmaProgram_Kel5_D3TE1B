
#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <cctype>
#include <cstdlib>

using namespace std;

// ==========================================
// DEKLARASI KONSTANTA
// ==========================================

const int MAX_PASIEN = 100;
const int MAX_KUNJUNGAN = 500;
const int MAX_PERIKSA = 500;

const string PASSWORD_DOKTER = "123456789";

// ==========================================
// STRUKTUR DATA
// ==========================================

// Menyimpan identitas unik setiap pasien
struct Pasien {
    string nama;
    string nik;
    int usia;
    float tinggi;
    float berat;
};

// Menyimpan setiap kunjungan pasien
struct Kunjungan {
    string nik;
    string noPendaftaran;
    string noAntrean;
    string tanggalKunjungan;
    string keluhan;
    bool sudahDiperiksa;
};

// Menyimpan riwayat pemeriksaan
struct Pemeriksaan {
    string nik;
    string noPendaftaran;
    string namaDokter;
    string tanggalPeriksa;
    string catatan;
};

// ==========================================
// DEKLARASI ARRAY DAN VARIABEL
// ==========================================

// Data identitas seluruh pasien
Pasien dataPasien[MAX_PASIEN];

// Data seluruh kunjungan pasien
Kunjungan dataKunjungan[MAX_KUNJUNGAN];

// Data riwayat pemeriksaan
Pemeriksaan dataPeriksa[MAX_PERIKSA];

// Antrean menyimpan indeks kunjungan yang masih aktif
int antrean[MAX_KUNJUNGAN];

int jumlahPasien = 0;
int jumlahKunjungan = 0;
int jumlahAntrean = 0;
int jumlahPeriksa = 0;

int totalPendaftaran = 0;

// ==========================================
// FUNGSI MEMBERSIHKAN LAYAR
// ==========================================

void hapusLayar() {
    system("cls");
}

// ==========================================
// FUNGSI MENUNGGU ENTER
// ==========================================

void tungguEnter() {
    cout << "\nTekan ENTER untuk melanjutkan...";
    cin.get();
}

// ==========================================
// FUNGSI VALIDASI NIK
// ==========================================

bool validasiNIK(string nik) {

    if (nik.length() != 16) {
        return false;
    }

    for (int i = 0; i < nik.length(); i++) {

        if (!isdigit((unsigned char)nik[i])) {
            return false;
        }
    }

    return true;
}

// ==========================================
// FUNGSI INPUT ANGKA BULAT
// ==========================================

int inputInteger(string pesan, bool bolehNol = false) {

    string input;
    int nilai;
    char sisa;

    while (true) {

        cout << pesan;
        getline(cin, input);

        stringstream ss(input);

        if (ss >> nilai && !(ss >> sisa)) {

            if (nilai > 0 || (bolehNol && nilai == 0)) {
                return nilai;
            }
        }

        cout << "Masukkan angka yang valid!\n";
    }
}

// ==========================================
// FUNGSI INPUT ANGKA DESIMAL
// ==========================================

float inputFloat(string pesan) {

    string input;
    float nilai;
    char sisa;

    while (true) {

        cout << pesan;
        getline(cin, input);

        stringstream ss(input);

        if (ss >> nilai && !(ss >> sisa) && nilai > 0) {
            return nilai;
        }

        cout << "Masukkan angka yang valid!\n";
    }
}

// ==========================================
// MENCARI IDENTITAS PASIEN BERDASARKAN NIK
// ==========================================

int cariPasien(string nik) {

    for (int i = 0; i < jumlahPasien; i++) {

        if (dataPasien[i].nik == nik) {
            return i;
        }
    }

    return -1;
}

// ==========================================
// MENCARI PASIEN DI ANTREAN AKTIF
// ==========================================

int cariPasienDalamAntrean(string nik) {

    for (int i = 0; i < jumlahAntrean; i++) {

        int indeks = antrean[i];

        if (dataKunjungan[indeks].nik == nik) {
            return i;
        }
    }

    return -1;
}

// ==========================================
// MENCARI PEMERIKSAAN BERDASARKAN PENDAFTARAN
// ==========================================

int cariPemeriksaan(string noPendaftaran) {

    for (int i = 0; i < jumlahPeriksa; i++) {

        if (dataPeriksa[i].noPendaftaran == noPendaftaran) {
            return i;
        }
    }

    return -1;
}

// ==========================================
// FUNGSI MENGHAPUS PASIEN DARI ANTREAN
// ==========================================

void hapusPasienAntrian(int posisi) {

    for (int i = posisi; i < jumlahAntrean - 1; i++) {

        antrean[i] = antrean[i + 1];
    }

    jumlahAntrean--;
}

// ==========================================
// FUNGSI MEMBUAT TANGGAL
// ==========================================

string buatTanggal(bool besok) {

    time_t sekarang = time(0);

    tm *waktuPtr = localtime(&sekarang);

    tm waktu = *waktuPtr;

    if (besok) {
        waktu.tm_mday += 1;
    }

    mktime(&waktu);

    stringstream tanggal;

    tanggal << setfill('0')
            << setw(2) << waktu.tm_mday << "-"
            << setw(2) << waktu.tm_mon + 1 << "-"
            << waktu.tm_year + 1900;

    return tanggal.str();
}

// ==========================================
// DASHBOARD UTAMA
// ==========================================

void dashboardUtama() {

    cout << "========================================\n";
    cout << "             POLIKLINIK SEHAT\n";
    cout << "          SISTEM INFORMASI PASIEN\n";
    cout << "========================================\n\n";

    cout << "  1. USER\n";
    cout << "  2. DOKTER\n";
    cout << "  3. KELUAR\n";

    cout << "\n========================================\n";
}

// ==========================================
// DEKLARASI MENU
// ==========================================

void menuUser();
void menuDokter();

// ==========================================
// MENU PENDAFTARAN PASIEN
// ==========================================

void daftarPasien() {

    hapusLayar();

    string nik;
    string keluhan;

    cout << "========================================\n";
    cout << "            PENDAFTARAN PASIEN\n";
    cout << "========================================\n\n";

    // Memeriksa kapasitas kunjungan dan antrean

    if (jumlahKunjungan >= MAX_KUNJUNGAN ||
        jumlahAntrean >= MAX_KUNJUNGAN) {

        cout << "Kapasitas pendaftaran penuh!\n";
        return;
    }

    // INPUT NIK

    cout << "Masukkan NIK : ";
    getline(cin, nik);

    if (!validasiNIK(nik)) {

        cout << "\nNIK tidak valid!\n";
        cout << "NIK harus terdiri dari 16 digit angka.\n";
        return;
    }

    // MENCARI IDENTITAS PASIEN

    int posisiPasien = cariPasien(nik);

    // MENCEGAH PENDAFTARAN GANDA SAAT MASIH MENGANTRE

    if (cariPasienDalamAntrean(nik) != -1) {

        cout << "\nPasien masih memiliki antrean aktif!\n";
        cout << "Silakan menunggu pemeriksaan sebelumnya.\n";
        return;
    }

    // JIKA PASIEN SUDAH TERDAFTAR

    if (posisiPasien != -1) {

        cout << "\n========================================\n";
        cout << "       DATA PASIEN DITEMUKAN\n";
        cout << "========================================\n";

        cout << "Nama         : "
             << dataPasien[posisiPasien].nama << endl;

        cout << "NIK          : "
             << dataPasien[posisiPasien].nik << endl;

        cout << "Usia         : "
             << dataPasien[posisiPasien].usia
             << " tahun\n";

        cout << "Tinggi Badan : "
             << dataPasien[posisiPasien].tinggi
             << " cm\n";

        cout << "Berat Badan  : "
             << dataPasien[posisiPasien].berat
             << " kg\n";

        cout << "\nData identitas ditemukan.\n";
        cout << "Silakan masukkan keluhan terbaru.\n";
    }

    // JIKA PASIEN BARU

    else {

        if (jumlahPasien >= MAX_PASIEN) {

            cout << "\nKapasitas data pasien penuh!\n";
            return;
        }

        Pasien pasienBaru;

        cout << "\n---------- DATA DIRI PASIEN ----------\n";

        pasienBaru.nik = nik;

        cout << "Nama              : ";
        getline(cin, pasienBaru.nama);

        if (pasienBaru.nama.empty()) {

            cout << "\nNama tidak boleh kosong!\n";
            return;
        }

        pasienBaru.usia =
            inputInteger("Usia              : ");

        pasienBaru.tinggi =
            inputFloat("Tinggi Badan (cm) : ");

        pasienBaru.berat =
            inputFloat("Berat Badan (kg)  : ");

        // Menyimpan identitas pasien baru

        dataPasien[jumlahPasien] = pasienBaru;

        posisiPasien = jumlahPasien;

        jumlahPasien++;
    }

    // INPUT KELUHAN TERBARU

    cout << "\nKeluhan Terbaru : ";
    getline(cin, keluhan);

    if (keluhan.empty()) {

        cout << "\nKeluhan tidak boleh kosong!\n";
        return;
    }

    // MEMBUAT DATA KUNJUNGAN BARU

    Kunjungan kunjunganBaru;

    kunjunganBaru.nik = nik;
    kunjunganBaru.keluhan = keluhan;
    kunjunganBaru.sudahDiperiksa = false;

    // MEMBUAT NOMOR PENDAFTARAN

    stringstream reg;

    reg << "REG"
        << setfill('0')
        << setw(4) << totalPendaftaran + 1;

    kunjunganBaru.noPendaftaran = reg.str();

    // MEMBUAT NOMOR ANTREAN

    stringstream nomorAntrean;

    nomorAntrean << "A"
                 << setfill('0')
                 << setw(3) << totalPendaftaran + 1;

    kunjunganBaru.noAntrean = nomorAntrean.str();

    // TANGGAL KUNJUNGAN

    kunjunganBaru.tanggalKunjungan = buatTanggal(true);

    // MENYIMPAN KUNJUNGAN

    dataKunjungan[jumlahKunjungan] = kunjunganBaru;

    // MEMASUKKAN INDEKS KUNJUNGAN KE ANTREAN

    antrean[jumlahAntrean] = jumlahKunjungan;

    jumlahAntrean++;

    jumlahKunjungan++;

    totalPendaftaran++;

    // MENAMPILKAN HASIL PENDAFTARAN

    hapusLayar();

    cout << "========================================\n";
    cout << "         PENDAFTARAN BERHASIL!\n";
    cout << "========================================\n\n";

    cout << "Nama              : "
         << dataPasien[posisiPasien].nama << endl;

    cout << "NIK               : " << nik << endl;

    cout << "Usia              : "
         << dataPasien[posisiPasien].usia
         << " tahun\n";

    cout << "Tinggi Badan      : "
         << dataPasien[posisiPasien].tinggi
         << " cm\n";

    cout << "Berat Badan       : "
         << dataPasien[posisiPasien].berat
         << " kg\n";

    cout << "Keluhan           : " << keluhan << endl;

    cout << "\n---------- INFORMASI ANTREAN ----------\n";

    cout << "No Pendaftaran    : "
         << kunjunganBaru.noPendaftaran << endl;

    cout << "No Antrean        : "
         << kunjunganBaru.noAntrean << endl;

    cout << "Tanggal Kunjungan : "
         << kunjunganBaru.tanggalKunjungan << endl;

    cout << "\nStatus            : Menunggu pemeriksaan\n";

    cout << "\nData berhasil disimpan!\n";
}

// ==========================================
// MENU CEK PENDAFTARAN
// ==========================================

void cekPendaftaran() {

    hapusLayar();

    string nik;

    cout << "========================================\n";
    cout << "             CEK PENDAFTARAN\n";
    cout << "========================================\n\n";

    cout << "Masukkan NIK : ";
    getline(cin, nik);

    if (!validasiNIK(nik)) {

        cout << "\nNIK tidak valid!\n";
        return;
    }

    int posisi = cariPasien(nik);

    if (posisi == -1) {

        cout << "\nData pasien tidak ditemukan.\n";
        cout << "Silakan daftar terlebih dahulu.\n";
        return;
    }

    // MENAMPILKAN IDENTITAS PASIEN

    cout << "\n========================================\n";
    cout << "             DATA PASIEN\n";
    cout << "========================================\n";

    cout << "Nama         : "
         << dataPasien[posisi].nama << endl;

    cout << "NIK          : "
         << dataPasien[posisi].nik << endl;

    cout << "Usia         : "
         << dataPasien[posisi].usia << " tahun\n";

    cout << "Tinggi Badan : "
         << dataPasien[posisi].tinggi << " cm\n";

    cout << "Berat Badan  : "
         << dataPasien[posisi].berat << " kg\n";

    // MENAMPILKAN SELURUH KUNJUNGAN

    cout << "\n========================================\n";
    cout << "           RIWAYAT PENDAFTARAN\n";
    cout << "========================================\n";

    int ditemukan = 0;

    for (int i = 0; i < jumlahKunjungan; i++) {

        if (dataKunjungan[i].nik == nik) {

            cout << "\nKunjungan ke-" << ++ditemukan << endl;

            cout << "No Pendaftaran : "
                 << dataKunjungan[i].noPendaftaran << endl;

            cout << "No Antrean     : "
                 << dataKunjungan[i].noAntrean << endl;

            cout << "Tanggal        : "
                 << dataKunjungan[i].tanggalKunjungan << endl;

            cout << "Keluhan        : "
                 << dataKunjungan[i].keluhan << endl;

            if (dataKunjungan[i].sudahDiperiksa) {

                cout << "Status         : Sudah diperiksa\n";

            } else {

                cout << "Status         : Menunggu pemeriksaan\n";
            }

            cout << "----------------------------------------\n";
        }
    }

    cout << "\nTotal kunjungan : " << ditemukan << endl;
}

// ==========================================
// MENU RIWAYAT PEMERIKSAAN PASIEN
// ==========================================

void riwayatPemeriksaan() {

    hapusLayar();

    string nik;

    cout << "========================================\n";
    cout << "          RIWAYAT PEMERIKSAAN\n";
    cout << "========================================\n\n";

    cout << "Masukkan NIK : ";
    getline(cin, nik);

    if (!validasiNIK(nik)) {

        cout << "\nNIK tidak valid!\n";
        return;
    }

    int posisi = cariPasien(nik);

    if (posisi == -1) {

        cout << "\nData pasien tidak ditemukan.\n";
        return;
    }

    // MENAMPILKAN IDENTITAS PASIEN

    cout << "\n========================================\n";
    cout << "               DATA PASIEN\n";
    cout << "========================================\n";

    cout << "Nama         : "
         << dataPasien[posisi].nama << endl;

    cout << "NIK          : "
         << dataPasien[posisi].nik << endl;

    cout << "Usia         : "
         << dataPasien[posisi].usia << " tahun\n";

    cout << "Tinggi Badan : "
         << dataPasien[posisi].tinggi << " cm\n";

    cout << "Berat Badan  : "
         << dataPasien[posisi].berat << " kg\n";

    // MENAMPILKAN RIWAYAT SETIAP KUNJUNGAN

    cout << "\n========================================\n";
    cout << "          RIWAYAT KUNJUNGAN\n";
    cout << "========================================\n";

    int nomor = 0;

    for (int i = 0; i < jumlahKunjungan; i++) {

        if (dataKunjungan[i].nik == nik) {

            nomor++;

            cout << "\nKunjungan ke-" << nomor << endl;

            cout << "No Pendaftaran : "
                 << dataKunjungan[i].noPendaftaran << endl;

            cout << "No Antrean     : "
                 << dataKunjungan[i].noAntrean << endl;

            cout << "Tanggal        : "
                 << dataKunjungan[i].tanggalKunjungan << endl;

            cout << "Keluhan        : "
                 << dataKunjungan[i].keluhan << endl;

            // MENCARI PEMERIKSAAN SESUAI KUNJUNGAN

            int indeksPeriksa =
                cariPemeriksaan(dataKunjungan[i].noPendaftaran);

            if (indeksPeriksa != -1) {

                cout << "\n--- HASIL PEMERIKSAAN ---\n";

                cout << "Nama Dokter : "
                     << dataPeriksa[indeksPeriksa].namaDokter
                     << endl;

                cout << "Tanggal     : "
                     << dataPeriksa[indeksPeriksa].tanggalPeriksa
                     << endl;

                cout << "Catatan     : "
                     << dataPeriksa[indeksPeriksa].catatan
                     << endl;

            } else {

                cout << "\nStatus : Belum diperiksa\n";
            }

            cout << "----------------------------------------\n";
        }
    }

    if (nomor == 0) {
        cout << "\nBelum ada riwayat kunjungan.\n";
    }

    cout << "\nTotal kunjungan : " << nomor << endl;
}

// ==========================================
// LOGIN DOKTER
// ==========================================

bool loginDokter() {

    string password;

    hapusLayar();

    cout << "========================================\n";
    cout << "              LOGIN DOKTER\n";
    cout << "========================================\n\n";

    cout << "Masukkan Password : ";
    getline(cin, password);

    if (password == PASSWORD_DOKTER) {

        cout << "\nLogin berhasil!\n";
        return true;

    } else {

        cout << "\nPassword salah!\n";
        cout << "Akses ditolak.\n";
        return false;
    }
}

// ==========================================
// MENU ANTRIAN PASIEN UNTUK DOKTER
// ==========================================

void antrianPasien() {

    hapusLayar();

    cout << "========================================\n";
    cout << "             ANTRIAN PASIEN\n";
    cout << "========================================\n\n";

    if (jumlahAntrean == 0) {

        cout << "Belum ada pasien yang menunggu.\n";
        return;
    }

    // MENAMPILKAN ANTREAN AKTIF

    for (int i = 0; i < jumlahAntrean; i++) {

        int indeks = antrean[i];

        int posisiPasien = cariPasien(
            dataKunjungan[indeks].nik
        );

        cout << "\n----------------------------------------\n";

        cout << "Urutan Antrean : " << i + 1 << endl;

        cout << "Nomor Antrean  : "
             << dataKunjungan[indeks].noAntrean << endl;

        cout << "Nama           : "
             << dataPasien[posisiPasien].nama << endl;

        cout << "NIK            : "
             << dataPasien[posisiPasien].nik << endl;

        cout << "Usia           : "
             << dataPasien[posisiPasien].usia
             << " tahun\n";

        cout << "Keluhan        : "
             << dataKunjungan[indeks].keluhan << endl;

        cout << "Status         : Menunggu pemeriksaan\n";
    }

    cout << "\n========================================\n";

    // MEMILIH PASIEN

    cout << "\nMasukkan urutan antrean yang akan diperiksa.\n";
    cout << "Masukkan 0 untuk kembali.\n";

    int nomor = inputInteger("Nomor pilihan : ", true);

    if (nomor == 0) {
        return;
    }

    if (nomor > jumlahAntrean) {

        cout << "\nNomor antrean tidak ditemukan!\n";
        return;
    }

    int posisiAntrean = nomor - 1;

    // MENGAMBIL INDEKS KUNJUNGAN

    int indeksKunjungan = antrean[posisiAntrean];

    int posisiPasien = cariPasien(
        dataKunjungan[indeksKunjungan].nik
    );

    // MEMERIKSA KAPASITAS RIWAYAT

    if (jumlahPeriksa >= MAX_PERIKSA) {

        cout << "\nKapasitas riwayat pemeriksaan penuh!\n";
        return;
    }

    string namaDokter;
    string catatan;

    hapusLayar();

    cout << "========================================\n";
    cout << "           PEMERIKSAAN PASIEN\n";
    cout << "========================================\n\n";

    cout << "Nomor Antrean : "
         << dataKunjungan[indeksKunjungan].noAntrean
         << endl;

    cout << "Nama Pasien   : "
         << dataPasien[posisiPasien].nama << endl;

    cout << "NIK           : "
         << dataPasien[posisiPasien].nik << endl;

    cout << "Keluhan       : "
         << dataKunjungan[indeksKunjungan].keluhan << endl;

    // INPUT NAMA DOKTER

    cout << "\nNama Dokter Pemeriksa : ";
    getline(cin, namaDokter);

    if (namaDokter.empty()) {

        cout << "\nNama dokter tidak boleh kosong!\n";
        return;
    }

    // INPUT CATATAN PEMERIKSAAN

    cout << "\nMasukkan Catatan Hasil Pemeriksaan:\n";

    cout << "Catatan : ";
    getline(cin, catatan);

    if (catatan.empty()) {

        cout << "\nCatatan tidak boleh kosong!\n";
        return;
    }

    // MENYIMPAN RIWAYAT PEMERIKSAAN

    dataPeriksa[jumlahPeriksa].nik =
        dataKunjungan[indeksKunjungan].nik;

    dataPeriksa[jumlahPeriksa].noPendaftaran =
        dataKunjungan[indeksKunjungan].noPendaftaran;

    dataPeriksa[jumlahPeriksa].namaDokter =
        namaDokter;

    dataPeriksa[jumlahPeriksa].tanggalPeriksa =
        buatTanggal(false);

    dataPeriksa[jumlahPeriksa].catatan =
        catatan;

    jumlahPeriksa++;

    // MENGUBAH STATUS KUNJUNGAN

    dataKunjungan[indeksKunjungan].sudahDiperiksa = true;

    // MENYIMPAN DATA UNTUK KONFIRMASI

    string namaPasien = dataPasien[posisiPasien].nama;

    string noAntrean =
        dataKunjungan[indeksKunjungan].noAntrean;

    // MENGHAPUS PASIEN DARI ANTREAN AKTIF

    hapusPasienAntrian(posisiAntrean);

    // MENAMPILKAN KONFIRMASI

    cout << "\n========================================\n";
    cout << "       PEMERIKSAAN BERHASIL DISIMPAN\n";
    cout << "========================================\n";

    cout << "Nama Pasien : "
         << namaPasien << endl;

    cout << "No Antrean  : "
         << noAntrean << endl;

    cout << "Nama Dokter : "
         << namaDokter << endl;

    cout << "Catatan     : "
         << catatan << endl;

    cout << "\nStatus      : Sudah diperiksa";

    cout << "\nAntrean     : Berhasil dihapus";

    cout << "\nRiwayat pemeriksaan tetap tersimpan.\n";
}

// ==========================================
// MENU DOKTER
// ==========================================

void menuDokter() {

    // LOGIN TERLEBIH DAHULU

    if (!loginDokter()) {
        tungguEnter();
        return;
    }

    string pilihan;

    do {

        hapusLayar();

        cout << "========================================\n";
        cout << "              MENU DOKTER\n";
        cout << "========================================\n\n";

        cout << "1. Antrian Pasien\n";
        cout << "2. Kembali\n";

        cout << "\n========================================\n";
        cout << "Masukkan Pilihan : ";

        getline(cin, pilihan);

        if (pilihan == "1") {

            antrianPasien();

        } else if (pilihan == "2") {

            break;

        } else {

            cout << "\nPilihan tidak tersedia!\n";
        }

        if (pilihan != "2") {
            tungguEnter();
        }

    } while (true);
}

// ==========================================
// MENU USER
// ==========================================

void menuUser() {

    string pilihan;

    do {

        hapusLayar();

        cout << "========================================\n";
        cout << "                MENU USER\n";
        cout << "========================================\n\n";

        cout << "1. Daftar Pasien\n";
        cout << "2. Cek Pendaftaran\n";
        cout << "3. Riwayat Pemeriksaan\n";
        cout << "4. Kembali\n";

        cout << "\n========================================\n";
        cout << "Masukkan Pilihan : ";

        getline(cin, pilihan);

        if (pilihan == "1") {

            daftarPasien();

        } else if (pilihan == "2") {

            cekPendaftaran();

        } else if (pilihan == "3") {

            riwayatPemeriksaan();

        } else if (pilihan == "4") {

            break;

        } else {

            cout << "\nPilihan tidak tersedia!\n";
        }

        if (pilihan != "4") {
            tungguEnter();
        }

    } while (true);
}

// ==========================================
// PROGRAM UTAMA
// ==========================================

int main() {

    string pilihan;

    do {

        hapusLayar();

        dashboardUtama();

        cout << "\nPilih Menu : ";
        getline(cin, pilihan);

        if (pilihan == "1") {

            menuUser();

        } else if (pilihan == "2") {

            menuDokter();

        } else if (pilihan == "3") {

            hapusLayar();

            cout << "========================================\n";
            cout << "       TERIMA KASIH TELAH MENGGUNAKAN\n";
            cout << "          LAYANAN POLIKLINIK\n";
            cout << "========================================\n";

            break;

        } else {

            cout << "\nPilihan tidak tersedia!\n";
            tungguEnter();
        }

    } while (true);

    return 0;
}

