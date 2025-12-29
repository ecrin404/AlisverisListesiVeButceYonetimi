#include <stdio.h>
#define MAX_URUN 800   // Maksimum eklenebilecek urun sayisi
#define MAX_ISIM 50   // Urun adi icin karakter siniri
#define MAX_BIRIM 10  // Birim adi icin sinir (kg, litre, adet, paket vs.)

// FONKSIYONLAR 
void urunEkle(char urunler[][MAX_ISIM], char birimler[][MAX_BIRIM], float fiyatlar[], float miktarlar[], int tamamlandi[], int *urun_sayisi);
void listeyiGoster(char urunler[][MAX_ISIM], char birimler[][MAX_BIRIM], float fiyatlar[], float miktarlar[], int tamamlandi[], int urun_sayisi);
void urunAlindiYap(char urunler[][MAX_ISIM], int tamamlandi[], int urun_sayisi);
void urunSil(char urunler[][MAX_ISIM], char birimler[][MAX_BIRIM], float fiyatlar[], float miktarlar[], int tamamlandi[], int *urun_sayisi);
void urunDuzenle(char urunler[][MAX_ISIM], char birimler[][MAX_BIRIM], float fiyatlar[], float miktarlar[], int tamamlandi[], int urun_sayisi);
void toplamHesapla(float fiyatlar[], float miktarlar[], int tamamlandi[], int urun_sayisi);
void butceKontrolu(float fiyatlar[], float miktarlar[], int tamamlandi[], int urun_sayisi, float butce);
void listeyiTemizle(int *urun_sayisi);
void menuGoster();
void bufferTemizle(); // Tampon bellegini (buffer) temizleyerek hatali okumayi onluyor.

// DOSYA ISLEMLERI
void dosyayaKaydet(char urunler[][MAX_ISIM], char birimler[][MAX_BIRIM], float fiyatlar[], float miktarlar[], int tamamlandi[], int urun_sayisi, float butce);
void dosyadanYukle(char urunler[][MAX_ISIM], char birimler[][MAX_BIRIM], float fiyatlar[], float miktarlar[], int tamamlandi[], int *urun_sayisi, float *butce);

// ANA FONKSIYON 
int main() {
    char urunler[MAX_URUN][MAX_ISIM];
    char birimler[MAX_URUN][MAX_BIRIM]; // "kg" , "L" , "adet" , "paket" gibi birimler icin
    float fiyatlar[MAX_URUN];
    float miktarlar[MAX_URUN];
    int tamamlandi[MAX_URUN]; // 0 = Alinmadi , 1 = Alindi icin kullanilacak

    int urun_sayisi = 0;
    float butce = 0;
    int secim;

    // Baslangicta kaydedilmis dosyadaki (varsa) verileri yukluyor.
    dosyadanYukle(urunler, birimler, fiyatlar, miktarlar, tamamlandi, &urun_sayisi, &butce);

    printf("\n=== ALISVERIS LISTESI ===\n");
    
    // Eger butce henuz sisteme girilmemis ise istiyor.
    if(butce == 0) {
        printf("Lutfen butcenizi girin (TL): ");
        scanf("%f", &butce);
        bufferTemizle(); // Enter tusunu temizlemek icin
    }

    do {
        menuGoster();
        printf("Seciminiz: ");
        scanf("%d", &secim);
        bufferTemizle(); // Secim sonrasi enter temizligi icin

        switch(secim) {
            case 1: 
                urunEkle(urunler, birimler, fiyatlar, miktarlar, tamamlandi, &urun_sayisi); 
                break;
            case 2: 
                listeyiGoster(urunler, birimler, fiyatlar, miktarlar, tamamlandi, urun_sayisi); 
                break;
            case 3: 
                urunAlindiYap(urunler, tamamlandi, urun_sayisi); 
                break;
            case 4: 
                urunSil(urunler, birimler, fiyatlar, miktarlar, tamamlandi, &urun_sayisi); 
                break;
            case 5: 
                urunDuzenle(urunler, birimler, fiyatlar, miktarlar, tamamlandi, urun_sayisi); 
                break;
            case 6: 
                toplamHesapla(fiyatlar, miktarlar, tamamlandi, urun_sayisi); 
                break;
            case 7: 
                butceKontrolu(fiyatlar, miktarlar, tamamlandi, urun_sayisi, butce); 
                break;
            case 8: 
                listeyiTemizle(&urun_sayisi); 
                break;
            case 9: 
                dosyayaKaydet(urunler, birimler, fiyatlar, miktarlar, tamamlandi, urun_sayisi, butce);
                printf("\nVeriler kaydedildi. Cikis yapiliyor...\n"); 
                break;
            default: 
                printf("Gecersiz secim! Lutfen tekrar deneyin.\n");
        }
    } while(secim != 9);

    return 0;
}

// DOSYA ISLEMLERI 

void dosyayaKaydet(char urunler[][MAX_ISIM], char birimler[][MAX_BIRIM], float fiyatlar[], float miktarlar[], int tamamlandi[], int urun_sayisi, float butce) {
    FILE *cfPtr;

    if ((cfPtr = fopen("alisveris.txt", "w")) == NULL) {
        printf("HATA: Dosya kaydedilemedi!\n");
    } else {
        fprintf(cfPtr, "%.2f\n", butce);
        fprintf(cfPtr, "%d\n", urun_sayisi);

        for (int i = 0; i < urun_sayisi; i++) {
            fprintf(cfPtr, "%s %.2f %.2f %s %d\n", urunler[i], fiyatlar[i], miktarlar[i], birimler[i], tamamlandi[i]);
        }
        fclose(cfPtr);
        printf(">> Liste ve butce basariyla dosyaya kaydedildi.\n");
    }
}

void dosyadanYukle(char urunler[][MAX_ISIM], char birimler[][MAX_BIRIM], float fiyatlar[], float miktarlar[], int tamamlandi[], int *urun_sayisi, float *butce) {
    FILE *cfPtr;

    if ((cfPtr = fopen("alisveris.txt", "r")) == NULL) {
        printf(">> Kayitli dosya bulunamadi. Yeni liste baslatiliyor...\n");
        *urun_sayisi = 0; // Dosya yoksa urun sayisini sifirliyor.
        *butce = 0; // Dosya yoksa butceyi sifirliyor.
    } else {
        if(fscanf(cfPtr, "%f", butce) != 1) {
            *butce = 0;
        }

        if(fscanf(cfPtr, "%d", urun_sayisi) != 1) {
            *urun_sayisi = 0;
            fclose(cfPtr);
            return;
        }

        int i = 0;
        // fscanf 5 tane veriyi (isim, fiyat, miktar, birim, durum) başariyla okudugu surece dongu calısıyor. Okuyamazsa dongu hemen duruyor.
        while (i < *urun_sayisi && fscanf(cfPtr, "%s %f %f %s %d", urunler[i], &fiyatlar[i], &miktarlar[i], birimler[i], &tamamlandi[i]) == 5) {
            i++;
        }

        // Donguden sonra okunan sayiyi guncelliyor.
        *urun_sayisi = i;

        fclose(cfPtr);
        printf(">> %d adet urun ve %.2f TL butce dosyadan yuklendi.\n", *urun_sayisi, *butce);
    }
}

// YARDIMCI FONKSIYONLAR

// Klavye tampon bellegini (buffer) satir sonuna (\n) kadar okuyup temizliyor. Hatalari engelliyor ve yazdigim kodun duzgun calismasini sagliyor.
void bufferTemizle() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void menuGoster() {
    printf("\n-------- MENU --------\n");
    printf("1. Urun Ekle\n");
    printf("2. Listeyi Goster\n");
    printf("3. Urunu Alindi Isaretle\n");
    printf("4. Urun Sil\n");
    printf("5. Urun Duzenle\n");
    printf("6. Toplam Hesapla\n");
    printf("7. Butce Kontrolu\n");
    printf("8. Listeyi Temizle\n");
    printf("9. Kaydet ve Cikis Yap\n");
    printf("--------------------------\n");
}

void urunEkle(char urunler[][MAX_ISIM], char birimler[][MAX_BIRIM], float fiyatlar[], float miktarlar[], int tamamlandi[], int *urun_sayisi) {
    if(*urun_sayisi >= MAX_URUN) { 
        printf("UYARI: Liste kapasitesi dolu!\n"); 
        return; 
    }

    printf("\n--- URUN EKLE ---\n");
    
    printf("Urun Adi: ");
    
    // Kullanici bosluklu isim yazarsa diye eklendi. Bosluklu isimleri alip '_' ile birlestiriyor.
    scanf(" %[^\n]", urunler[*urun_sayisi]); 
    
    int k = 0;
    while(urunler[*urun_sayisi][k] != '\0') {
        if(urunler[*urun_sayisi][k] == ' ') {
            urunler[*urun_sayisi][k] = '_'; // Boslugu alt cizgi yapiyor.
        }
        k++;
    }

    printf("Birim Fiyati (TL): ");
    scanf("%f", &fiyatlar[*urun_sayisi]);

    printf("Miktari: ");
    scanf("%f", &miktarlar[*urun_sayisi]); 

    printf("Birimi (kg, L, adet, paket vs.): ");
    scanf("%s", birimler[*urun_sayisi]); // Birimi aliyor.

    tamamlandi[*urun_sayisi] = 0;
    (*urun_sayisi)++;             
    
    bufferTemizle();
    printf(">> Urun eklendi.\n");
}

void listeyiGoster(char urunler[][MAX_ISIM], char birimler[][MAX_BIRIM], float fiyatlar[], float miktarlar[], int tamamlandi[], int urun_sayisi) {
    if(urun_sayisi == 0) { 
        printf("\nListe bos.\n"); 
        return; 
    }

    printf("\n%-4s %-25s %-12s %-15s %-10s\n", "No", "Urun", "Fiyat", "Miktar", "Durum");
    printf("---- ------------------------- ------------ --------------- ----------\n");
    
    for(int i = 0; i < urun_sayisi; i++) {
        // Urunun Miktarini ve Birimini yan yana yazdiriyor.
        printf("%-4d %-25s %9.2f TL %6.2f %-7s ", i + 1, urunler[i], fiyatlar[i], miktarlar[i], birimler[i]);
        
        if(tamamlandi[i]) printf(" [ALINDI]\n"); 
        else printf(" [  ]\n");
    }
}

void urunSil(char urunler[][MAX_ISIM], char birimler[][MAX_BIRIM], float fiyatlar[], float miktarlar[], int tamamlandi[], int *urun_sayisi) {
    if(*urun_sayisi == 0) { 
        printf("Liste bos.\n"); 
        return; 
    }
    
    // Neyi silecegini bilmek icin once listeyi gosteriyor.
    listeyiGoster(urunler, birimler, fiyatlar, miktarlar, tamamlandi, *urun_sayisi);

    int silinecek;
    printf("\nSilinecek urun no: ");
    scanf("%d", &silinecek);
    bufferTemizle(); 

    if(silinecek < 1 || silinecek > *urun_sayisi) {
        return;
    }

    int index = silinecek - 1;

    // Kaydirma Islemi (SHIFTING)
    // Silinen urunun yerini doldurmak icin arkadaki urunleri sirayla one cekiyor.
    for(int i = index; i < *urun_sayisi - 1; i++) {
        
        // 1. Isimleri bir sonraki satirdan alip buraya tasi.
        int k = 0;
        while(urunler[i + 1][k] != '\0') { urunler[i][k] = urunler[i + 1][k]; k++; }
        urunler[i][k] = '\0';

        // 2. Birimleri de ayni sekilde tasi.
        int m = 0;
        while(birimler[i + 1][m] != '\0') { birimler[i][m] = birimler[i + 1][m]; m++; }
        birimler[i][m] = '\0';

        // 3. Sayisal verileri de tasi.
        fiyatlar[i] = fiyatlar[i + 1];
        miktarlar[i] = miktarlar[i + 1];
        tamamlandi[i] = tamamlandi[i + 1];
    }

    (*urun_sayisi)--; 
    printf(">> Urun silindi.\n");
}

void urunDuzenle(char urunler[][MAX_ISIM], char birimler[][MAX_BIRIM], float fiyatlar[], float miktarlar[], int tamamlandi[], int urun_sayisi) {
    if(urun_sayisi == 0) {
        return;
    }
    
    // Neyi duzenleyecegini bilmek icin once listeyi gosteriyor.
    listeyiGoster(urunler, birimler, fiyatlar, miktarlar, tamamlandi, urun_sayisi);
    
    int d;
    printf("\nDuzenlenecek no: ");
    scanf("%d", &d);
    
    if(d < 1 || d > urun_sayisi) { 
        bufferTemizle(); 
        return; 
    }

    int i = d - 1;
    printf("\n--- DUZENLEME ---\n");
    
    printf("Yeni Isim: ");
    scanf(" %[^\n]", urunler[i]); 

    // Bosluk varsa duzeltiyor.
    int k = 0;
    while(urunler[i][k] != '\0') { if(urunler[i][k] == ' ') urunler[i][k] = '_'; k++; }

    printf("Yeni Fiyat: ");
    scanf("%f", &fiyatlar[i]);
    
    printf("Yeni Miktar: ");
    scanf("%f", &miktarlar[i]);

    printf("Yeni Birim: ");
    scanf("%s", birimler[i]);
    
    bufferTemizle();
    printf(">> Urun duzenlendi.\n");
}

void urunAlindiYap(char urunler[][MAX_ISIM], int tamamlandi[], int urun_sayisi) {
    if(urun_sayisi == 0) {
        return;
    }

    int no;
    printf("\nAlinan urun no: ");
    scanf("%d", &no);
    bufferTemizle();
    
    if(no > 0 && no <= urun_sayisi) {
        tamamlandi[no - 1] = 1;
        printf(">> Urun ALINDI isaretlendi.\n");
    } else {
        printf("Gecersiz numara.\n");
    }
}

void toplamHesapla(float fiyatlar[], float miktarlar[], int tamamlandi[], int urun_sayisi) {
    float toplam = 0, harcanan = 0;
    for(int i = 0; i < urun_sayisi; i++) {
        float tutar = fiyatlar[i] * miktarlar[i];
        toplam += tutar;
        if(tamamlandi[i]) harcanan += tutar;
    }

    printf("\n--- HESAP OZETI ---\n");
    printf("Toplam Liste Tutari : %.2f TL\n", toplam);
    printf("Aldiklarinizin Tutari : %.2f TL\n", harcanan);
}

void butceKontrolu(float fiyatlar[], float miktarlar[], int tamamlandi[], int urun_sayisi, float butce) {
    float harcanan = 0;
    for(int i = 0; i < urun_sayisi; i++) {
        if(tamamlandi[i]) harcanan += fiyatlar[i] * miktarlar[i];
    }
    printf("\n--- BUTCE DURUMU ---\n");
    printf("Butce       : %.2f TL\n", butce);
    printf("Harcanan    : %.2f TL\n", harcanan);
    printf("Kalan Butce : %.2f TL\n", butce - harcanan);
    
    if(harcanan > butce) {
        printf("UYARI: Butceyi %.2f TL astiniz!\n", harcanan - butce);
    }
}

void listeyiTemizle(int *urun_sayisi) {
    // Eger liste zaten bos ise islem yapmiyor.
    if(*urun_sayisi == 0) {
    printf("Liste zaten bos.\n");
    return;
    }

    char onay;
    printf("DIKKAT !!! Tum liste kalici olarak silinecek. Emin misiniz? (E/H): ");
    scanf(" %c", &onay);
    bufferTemizle();

    if(onay == 'e' || onay == 'E') {
        *urun_sayisi = 0; // Tek satirda tum listeyi yok ediyor ve urun sayisini sifirliyor.
        printf(">> Tum liste temizlendi.\n");
    } else {
        printf(">> Islem iptal edildi.\n");
    }
}