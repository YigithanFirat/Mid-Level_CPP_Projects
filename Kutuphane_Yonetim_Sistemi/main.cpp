#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct Kitap
{
    int id;
    string baslik, yazar;
    bool mevcut;
};

vector<Kitap> kutuphane;
int kitapID = 1;

void kitapEkle()
{
    Kitap k;
    k.id = kitapID++;
    cout << "Kitap basligi giriniz: ";
    cin.ignore();
    getline(cin, k.baslik);
    cout << "Yazar adini giriniz: ";
    getline(cin, k.yazar);
    k.mevcut = true;
    kutuphane.push_back(k);
    cout << "Kitap eklendi!\n";
}

void kitaplariListele()
{
    if(kutuphane.empty())
    {
        cout << "Kutuphanede hic kitap yok.\n";
        return;
    }
    cout << "=== KUTUPHANEDEKI KITAPLAR === \n";
    for(auto &k : kutuphane)
    {
        cout << "ID: " << k.id
             << " | Baslik: " << k.baslik
             << " | Yazar: " << k.yazar
             << " | Durum: " << (k.mevcut ? "Mevcut" : "Odunc Verildi") << endl;
    }
}

void kitapAra()
{
    string aranan;
    cout << "Aranacak basligi giriniz: ";
    cin.ignore();
    getline(cin, aranan);
    bool bulundu = false;
    for(auto &k : kutuphane)
    {
        if(k.baslik.find(aranan) != string::npos)
        {
            cout << "Bulundu => ID: " << k.id
                 << " | Baslik: " << k.baslik
                 << " | Yazar: " << k.yazar << endl;
            bulundu = true;
        }
        if(!bulundu) cout << "Kitap bulunamadi!";
    }
}

void kitapSil()
{
    int silinecekID;
    cout << "Silinecek kitap ID'sini giriniz: ";
    cin >> silinecekID;
    for(auto it = kutuphane.begin(); it != kutuphane.end(); ++it)
    {
        if(it->id == silinecekID)
        {
            kutuphane.erase(it);
            cout << "Kitap silindi!\n";
            return;
        }
    }
}


int main() 
{
    int secim;
    do
    {
        cout << "\n===== KUTUPHANE YONETIM SISTEMI =====\n";
        cout << "1. Kitap Ekle\n";
        cout << "2. Kitaplari Listele\n";
        cout << "3. Kitap Ara\n";
        cout << "4. Kitap Sil\n";
        cout << "5. Cikis\n";
        cout << "Seciminizi yapin: ";
        cin >> secim;

        switch(secim)
        {
            case 1: kitapEkle(); break;
            case 2: kitaplariListele(); break;
            case 3: kitapAra(); break;
            case 4: kitapSil(); break;
            case 5: cout << "Program sonlandiriliyor...\n"; break;
            default: cout << "Gecersiz secim!\n"; break;
        }
    }while (secim != 5);
    return 0;
}