# **🛒 *ALIŞVERİŞ LİSTESİ VE BÜTÇE YÖNETİMİ***



### ***Algoritma ve Programlamaya Giriş : Güz Dönemi Bitirme Projesi***



##### **🔗 Bu proje, kullanıcıların market alışverişlerini planlamalarını, bütçelerini yönetmelerini ve harcamalarını kontrol altına almalarını sağlayan C tabanlı bir terminal uygulamasıdır. Standart bir liste uygulamasından farklı olarak, verileri kalıcı olarak saklar ve dinamik bütçe hesaplamaları yapar.**



### **🚀 Öne Çıkan Özellikler**



##### **Veri Kalıcılığı:** Program kapatıldığında bütçe, ürün listesi ve ürünlerin "alındı" durumları otomatik olarak `alisveris.txt` dosyasına kaydedilir. Yeniden başlatıldığında veriler kaldığı yerden yüklenir.

##### 

##### **Akıllı Metin Girişi:** C dilindeki `scanf` kaynaklı boşluk karakteri problemlerini çözer. Kullanıcı "Elma Sirkesi" yazdığında, sistem bunu otomatik olarak "Elma\_Sirkesi" formatına dönüştürerek veri bütünlüğünü korur.

##### 

##### **Dinamik Bütçe Yönetimi:** Toplam maliyeti, sepete eklenen ürünlerin tutarını ve kalan bütçeyi anlık olarak hesaplar; bütçe aşımı durumunda kullanıcıyı uyarır.



### **🛠 Teknik Detaylar**



##### 🔗 Proje, algoritma ve bellek yönetimi prensiplerini göstermek amacıyla belirli teknik kısıtlar ve özel çözümlerle geliştirilmiştir:

##### 

##### **Kütüphaneler:** Proje, taşınabilirliği artırmak adına sadece standart giriş/çıkış kütüphanesi olan `<stdio.h>` kullanılarak geliştirilmiştir.

##### 

##### **Veri Yapısı:** Veriler; ürün adları, fiyatlar, miktarlar ve durumlar için ayrı ayrı tanımlanmış `Paralel Diziler` üzerinden yönetilir ve indeks numaraları ile senkronize edilir.

##### 

##### **Tampon Bellek (Buffer) Yönetimi:** Terminal girişlerindeki atlama sorunlarını engellemek için özelleştirilmiş bir `bufferTemizle()` fonksiyonu kullanılmıştır.

##### 

##### **Pointer Aritmetiği:** <string.h> kütüphanesi kullanılmadan; metin kopyalama, karakter değiştirme ve kaydırma işlemleri tamamen `Pointer Aritmetiği` ve özel döngülerle gerçekleştirilmiştir.

##### 

##### **Pass-by-Reference:** Fonksiyonlar arası veri aktarımı, bellek kullanımını optimize etmek ve veriyi doğrudan kaynağında değiştirmek için pointerlar üzerinden yapılmıştır.

###### 

### **📁 Dosya Yapısı ve Veri Formatı**

**Program, verilerini `alisveris.txt` dosyasında şu format ile saklar:**

```text
1250.00                                       <-- Mevcut Bütçe
8                                             <-- Toplam Ürün Sayısı
-----------------------------------------------------------------------------------------------
Salkim_Domates      84.90   2.50   kg      1  <-- [ İsim | Fiyat | Miktar | Birim | Durum ]
Tam_Yagli_Sut       44.50   2.00   L       1
Cikolata            13.25   5.00   adet    0
Kelebek_Makarna     22.50   3.00   paket   1
Un                  25.00   5.00   kg      1
Biber_Salcasi      105.00   1.65   kg      0
Dana_Kiyma         800.00   0.75   kg      1
Sucuk             1760.00   0.25   kg      0

