/**
 * @file client.c
 * @brief Bir Dosya Göndermek için TCP İstemcisi
 *
 * Bu program, bir TCP istemcisi oluşturur ve bir sunucuya bağlanarak dosya gönderir.
 * Dosya ikili modda okunur ve BUFFER_SIZE baytlık parçalar halinde gönderilir.
 * 
 * @author 
 * @date 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

// Sunucu için port numarası
#define PORT 8080

// Dosya parçalarını okumak ve göndermek için buffer boyutu
#define BUFFER_SIZE 1024

// Gönderilecek dosyanın yolu
#define SEND_FILE_PATH "send_file.txt"

// Fonksiyon deklarasyonları
int create_client_socket(struct sockaddr_in *serv_addr);
void send_file(int sock, FILE *file);

/**
 * @brief İstemciyi kuran, sunucuya bağlanan ve dosyayı gönderen ana fonksiyon.
 *
 * Bu fonksiyon gönderilecek dosyayı açar, istemci soketini oluşturur, sunucuya bağlanır,
 * dosyayı gönderir ve ardından dosya ve soketi kapatır.
 *
 * @return int Programın çıkış durumu.
 */
int main() {
    int sock;                       // Soket tanımlayıcısı
    struct sockaddr_in serv_addr;   // Sunucu adres yapısı
    FILE *file;                     // Dosya işaretçisi

    // Dosyayı ikili okuma modunda aç
    file = fopen(SEND_FILE_PATH, "rb");
    if (file == NULL) {
        perror("Dosya açma hatası");  // Dosya açma hatası durumunda hata mesajı yazdır
        exit(EXIT_FAILURE);
    }

    // İstemci soketini oluştur
    sock = create_client_socket(&serv_addr);

    // Sunucuya bağlan
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Bağlantı hatası");  // Bağlantı hatası durumunda hata mesajı yazdır
        close(sock);
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // Dosyayı sunucuya gönder
    send_file(sock, file);

    printf("Dosya başarıyla gönderildi.\n");  // Dosyanın başarıyla gönderildiğini belirt

    // Dosya ve soketi kapat
    fclose(file);
    close(sock);

    return 0;
}

/**
 * @brief Bir TCP istemci soketi oluşturur ve sunucu adres yapısını başlatır.
 *
 * Bu fonksiyon, IPv4 adres ailesi ve TCP protokolü kullanarak bir soket oluşturur.
 * Ayrıca verilen port ve IP adresi ile sunucu adres yapısını başlatır.
 *
 * @param serv_addr Sunucu adres yapısına işaretçi.
 * @return int Oluşturulan soket tanımlayıcısı.
 */
int create_client_socket(struct sockaddr_in *serv_addr) {
    int sock;

    // Bir soket oluştur
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Soket oluşturma hatası");  // Soket oluşturma hatası durumunda hata mesajı yazdır
        exit(EXIT_FAILURE);
    }

    // Sunucu adres ailesi, port ve IP adresini ayarla
    serv_addr->sin_family = AF_INET;
    serv_addr->sin_port = htons(PORT);

    // IP adresini metinden ikili forma çevir
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr->sin_addr) <= 0) {
        perror("Geçersiz adres veya adres desteklenmiyor");  // Adres çevirme hatası durumunda hata mesajı yazdır
        close(sock);
        exit(EXIT_FAILURE);
    }

    return sock;
}

/**
 * @brief Bağlı sunucuya bir dosya gönderir.
 *
 * Bu fonksiyon, dosyayı BUFFER_SIZE baytlık parçalar halinde okur ve her parçayı
 * sunucuya send() sistem çağrısı kullanarak gönderir. Gönderim sırasında herhangi bir hata
 * oluşursa, hata mesajı yazdırır ve programdan çıkar.
 *
 * @param sock Sunucuya bağlı soket tanımlayıcısı.
 * @param file Gönderilecek dosyanın işaretçisi.
 */
void send_file(int sock, FILE *file) {
    char buffer[BUFFER_SIZE] = {0};  // Dosya parçalarını okumak için buffer
    int bytes_read;

    // Dosyayı parça parça oku ve sunucuya gönder
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        if (send(sock, buffer, bytes_read, 0) == -1) {
            perror("Veri gönderme hatası");  // Veri gönderme hatası durumunda hata mesajı yazdır
            close(sock);
            fclose(file);
            exit(EXIT_FAILURE);
        }
    }
}
