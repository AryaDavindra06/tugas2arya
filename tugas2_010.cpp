#include <iostream>
#include <queue>
#include <stack>
using namespace std;

// ================= STRUCT =================

struct Karyawan {
    int nip;
    string nama;
    string divisi;
    string status;
};

struct Node {
    Karyawan data;
    Node *left, *right;
};

struct Riwayat {
    string aksi;
    int nip;
};

// ================= GLOBAL =================

Node* root = NULL;

queue<int> antreanCuti;
stack<Riwayat> undoStack;

// ================= INSERT BST =================

Node* insert(Node* root, Karyawan k) {

    if (root == NULL) {
        Node* baru = new Node;

        baru->data = k;
        baru->left = NULL;
        baru->right = NULL;

        return baru;
    }

    if (k.nip < root->data.nip) {
        root->left = insert(root->left, k);
    }
    else if (k.nip > root->data.nip) {
        root->right = insert(root->right, k);
    }

    return root;
}

// ================= SEARCH BST =================

Node* cari(Node* root, int nip) {

    if (root == NULL || root->data.nip == nip) {
        return root;
    }

    if (nip < root->data.nip) {
        return cari(root->left, nip);
    }

    return cari(root->right, nip);
}

// ================= TAMPIL DATA =================

void tampil(Node* root) {

    if (root != NULL) {

        tampil(root->left);

        cout << "NIP    : " << root->data.nip << endl;
        cout << "Nama   : " << root->data.nama << endl;
        cout << "Divisi : " << root->data.divisi << endl;
        cout << "Status : " << root->data.status << endl;
        cout << "--------------------------" << endl;

        tampil(root->right);
    }
}

// ================= CARI NODE TERKECIL =================

Node* minValue(Node* node) {

    Node* current = node;

    while (current && current->left != NULL) {
        current = current->left;
    }

    return current;
}

// ================= HAPUS BST =================

Node* hapus(Node* root, int nip) {

    if (root == NULL) {
        return root;
    }

    if (nip < root->data.nip) {
        root->left = hapus(root->left, nip);
    }

    else if (nip > root->data.nip) {
        root->right = hapus(root->right, nip);
    }

    else {

        // 1 anak / tanpa anak
        if (root->left == NULL) {

            Node* temp = root->right;
            delete root;

            return temp;
        }

        else if (root->right == NULL) {

            Node* temp = root->left;
            delete root;

            return temp;
        }

        // 2 anak
        Node* temp = minValue(root->right);

        root->data = temp->data;

        root->right = hapus(root->right, temp->data.nip);
    }

    return root;
}

// ================= TAMBAH KARYAWAN =================

void tambahKaryawan() {

    Karyawan k;

    cout << "Masukkan NIP : ";
    cin >> k.nip;
    cin.ignore();

    cout << "Masukkan Nama : ";
    getline(cin, k.nama);

    cout << "Masukkan Divisi : ";
    getline(cin, k.divisi);

    k.status = "Tidak Cuti";

    root = insert(root, k);

    cout << "Data karyawan berhasil ditambahkan\n";
}

// ================= AJUKAN CUTI =================

void ajukanCuti() {

    int nip;

    cout << "Masukkan NIP : ";
    cin >> nip;

    Node* k = cari(root, nip);

    if (k == NULL) {
        cout << "Karyawan tidak ditemukan\n";
        return;
    }

    if (k->data.status == "Tidak Cuti") {

        k->data.status = "Cuti";

        undoStack.push({"ajukan", nip});

        cout << "Pengajuan cuti berhasil\n";
    }

    else {

        antreanCuti.push(nip);

        cout << "Karyawan sedang cuti, pengajuan masuk ke antrean\n";
    }
}

// ================= SELESAI CUTI =================

void selesaiCuti() {

    int nip;

    cout << "Masukkan NIP : ";
    cin >> nip;

    Node* k = cari(root, nip);

    if (k == NULL) {
        cout << "Karyawan tidak ditemukan\n";
        return;
    }

    k->data.status = "Tidak Cuti";

    undoStack.push({"selesai", nip});

    cout << "Cuti berhasil diselesaikan\n";

    if (!antreanCuti.empty()) {

        int nextNip = antreanCuti.front();
        antreanCuti.pop();

        Node* next = cari(root, nextNip);

        if (next != NULL) {
            next->data.status = "Cuti";
        }

        cout << "Pengajuan cuti berikutnya diproses dari antrean\n";
    }
}

// ================= HAPUS KARYAWAN =================

void hapusKaryawan() {

    int nip;

    cout << "Masukkan NIP yang dihapus : ";
    cin >> nip;

    if (cari(root, nip) == NULL) {

        cout << "Karyawan tidak ditemukan\n";
        return;
    }

    root = hapus(root, nip);

    cout << "Data karyawan berhasil dihapus\n";
}

// ================= UNDO =================

void undoAksi() {

    if (undoStack.empty()) {

        cout << "Tidak ada aksi untuk di-undo\n";
        return;
    }

    Riwayat last = undoStack.top();
    undoStack.pop();

    Node* k = cari(root, last.nip);

    if (k == NULL) {
        return;
    }

    if (last.aksi == "ajukan") {

        k->data.status = "Tidak Cuti";

        cout << "Undo: Status cuti dibatalkan\n";
    }

    else if (last.aksi == "selesai") {

        k->data.status = "Cuti";

        cout << "Undo: Status cuti dikembalikan menjadi cuti\n";
    }
}

// ================= TAMPIL ANTREAN =================

void tampilAntrean() {

    if (antreanCuti.empty()) {

        cout << "Antrean kosong\n";
        return;
    }

    queue<int> temp = antreanCuti;

    while (!temp.empty()) {

        int nip = temp.front();
        temp.pop();

        Node* k = cari(root, nip);

        if (k != NULL) {

            cout << k->data.nama
                 << " menunggu giliran cuti\n";
        }
    }
}

// ================= MAIN =================

int main() {

    int pilih;

    do {

        cout << "\n=== SISTEM CUTI KARYAWAN ===\n";
        cout << "1. Tambah Karyawan\n";
        cout << "2. Tampil Karyawan\n";
        cout << "3. Ajukan Cuti\n";
        cout << "4. Selesai Cuti\n";
        cout << "5. Hapus Karyawan\n";
        cout << "6. Undo\n";
        cout << "7. Tampil Antrean\n";
        cout << "0. Keluar\n";

        cout << "Pilih Menu : ";
        cin >> pilih;

        switch(pilih) {

            case 1:
                tambahKaryawan();
                break;

            case 2:
                tampil(root);
                break;

            case 3:
                ajukanCuti();
                break;

            case 4:
                selesaiCuti();
                break;

            case 5:
                hapusKaryawan();
                break;

            case 6:
                undoAksi();
                break;

            case 7:
                tampilAntrean();
                break;

            case 0:
                cout << "Program selesai\n";
                break;

            default:
                cout << "Menu tidak tersedia\n";
        }

    } while (pilih != 0);

    return 0;
}