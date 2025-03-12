/**
 * @file server.c
 * @brief Bir Dosya Almak için TCP Sunucusu
 *
 * Bu program, bir TCP sunucusu oluşturur ve istemciden dosya alır.
 * Alınan dosya, BUFFER_SIZE baytlık parçalar halinde yazılır.
 * 
 * @date 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

// Sunucu için port numarası
#define PORT 8080

// Dosya parçalarını okumak ve yazmak için buffer boyutu
#define BUFFER_SIZE 1024

// Alınacak dosyanın yolu
#define RECEIVED_FILE_PATH "received_file.txt"

// Fonksiyon deklarasyonları
void handle_client(int new_socket, FILE *file);
int create_server_socket(struct sockaddr_in *address);

/**
 * @brief Sunucuyu kuran, istemciden bağlantı kabul eden ve dosyayı alan ana fonksiyon.
 *
 * Bu fonksiyon, alınan dosyayı yazmak için bir dosya açar, sunucu soketini oluşturur,
 * istemciden bağlantı kabul eder, dosyayı alır ve ardından dosya ve soketi kapatır.
 *
 * @return int Programın çıkış durumu.
 */
int main() {
    int server_fd, new_socket;          // Sunucu ve yeni istemci soket tanımlayıcıları
    struct sockaddr_in address;         // Sunucu adres yapısı
    int addrlen = sizeof(address);
    FILE *file;                         // Dosya işaretçisi

    // Dosyayı ikili yazma modunda aç
    file = fopen(RECEIVED_FILE_PATH, "wb");
    if (file == NULL) {
        perror("Dosya açma hatası");  // Dosya açma hatası durumunda hata mesajı yazdır
        exit(EXIT_FAILURE);
    }

    // Sunucu soketini oluştur
    server_fd = create_server_socket(&address);

    // Sunucu soketini dinlemeye başla
    if (listen(server_fd, 3) < 0) {
        perror("Dinleme hatası");  // Dinleme hatası durumunda hata mesajı yazdır
        close(server_fd);
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // İstemciden bağlantı kabul et
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Kabul etme hatası");  // Kabul etme hatası durumunda hata mesajı yazdır
        close(server_fd);
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // İstemciden gelen dosyayı al
    handle_client(new_socket, file);

    // Dosya ve soketleri kapat
    fclose(file);
    close(new_socket);
    close(server_fd);

    printf("Dosya başarıyla alındı.\n");

    return 0;
}

/**
 * @brief İstemciden gelen dosyayı alan ve dosyaya yazan fonksiyon.
 *
 * Bu fonksiyon, istemciden gelen veriyi BUFFER_SIZE baytlık parçalar halinde okur
 * ve bu parçaları dosyaya yazar. Eğer veri alma sırasında herhangi bir hata oluşursa,
 * hata mesajı yazdırır.
 *
 * @param new_socket İstemciye bağlı soket tanımlayıcısı.
 * @param file Yazılacak dosyanın işaretçisi.
 */
void handle_client(int new_socket, FILE *file) {
    char buffer[BUFFER_SIZE] = {0};  // Dosya parçalarını okumak için buffer
    int bytes_read;

    // Veriyi parça parça oku ve dosyaya yaz
    while ((bytes_read = read(new_socket, buffer, BUFFER_SIZE)) > 0) {
        fwrite(buffer, 1, bytes_read, file);
    }

    // Veri alma hatası durumunda hata mesajı yazdır
    if (bytes_read < 0) {
        perror("Veri alma hatası");
    }
}

/**
 * @brief Bir TCP sunucu soketi oluşturur ve sunucu adres yapısını başlatır.
 *
 * Bu fonksiyon, IPv4 adres ailesi ve TCP protokolü kullanarak bir soket oluşturur.
 * Ayrıca verilen port ve IP adresi ile sunucu adres yapısını başlatır.
 *
 * @param address Sunucu adres yapısına işaretçi.
 * @return int Oluşturulan sunucu soket tanımlayıcısı.
 */
int create_server_socket(struct sockaddr_in *address) {
    int server_fd;

    // Bir soket oluştur
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Soket oluşturma hatası");  // Soket oluşturma hatası durumunda hata mesajı yazdır
        exit(EXIT_FAILURE);
    }

    // Sunucu adres ailesi, port ve IP adresini ayarla
    address->sin_family = AF_INET;
    address->sin_addr.s_addr = INADDR_ANY;
    address->sin_port = htons(PORT);

    // Soketi verilen adrese bağla
    if (bind(server_fd, (struct sockaddr *)address, sizeof(*address)) < 0) {
        perror("Bağlama hatası");  // Bağlama hatası durumunda hata mesajı yazdır
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    return server_fd;
}
