/**
 * @file server.c
 * @brief UDP Sunucusu ile Dosya Alma
 *
 * Bu program, bir UDP sunucusu oluşturur ve belirtilen port üzerinden gelen dosya verilerini alır.
 * Dosya BUFFER_SIZE baytlık parçalar halinde alınır ve belirtilen dosyaya yazılır.
 *
 * @date 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

// Dosya parçalarını okumak ve yazmak için buffer boyutu
#define BUFFER_SIZE 1024

// Sunucu port numarası
#define PORT 8080

// Alınacak dosyanın adı
#define RECEIVED_FILE_NAME "received_file.txt"

/**
 * @brief Hata mesajı basar ve programdan çıkar.
 *
 * @param s Hata mesajı.
 */
void die(char *s) {
    perror(s);
    exit(1);
}

/**
 * @brief UDP socket oluşturur ve sunucu adresine bağlar.
 *
 * Bu fonksiyon, IPv4 ve UDP kullanarak bir socket oluşturur ve belirtilen port numarasına bağlar.
 *
 * @return int Oluşturulan ve bağlanan socket tanımlayıcısı.
 */
int create_socket() {
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        die("socket");

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));  // server_addr yapısını sıfırlarla doldur
    server_addr.sin_family = AF_INET;              // Adres ailesi IPv4
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // Tüm yerel IP adreslerini kabul et
    server_addr.sin_port = htons(PORT);            // Port numarasını ayarla (host to network short)

    // Socket'i bağlama
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        die("bind");

    return sockfd;
}

/**
 * @brief UDP üzerinden gelen veriyi alır ve dosyaya yazar.
 *
 * Bu fonksiyon, UDP üzerinden gelen veriyi BUFFER_SIZE baytlık parçalar halinde alır ve belirtilen dosyaya yazar.
 * Eğer alınan veri BUFFER_SIZE'dan küçükse, veri alımı biter.
 *
 * @param sockfd UDP socket tanımlayıcısı.
 * @param filename Alınacak verinin yazılacağı dosyanın adı.
 */
void receive_file(int sockfd, char *filename) {
    struct sockaddr_in client_addr;
    char buffer[BUFFER_SIZE];
    int len, n;
    FILE *fp;

    len = sizeof(client_addr);

    // Dosya açma
    if ((fp = fopen(filename, "wb")) == NULL)
        die("Dosya açma");

    // Veriyi alır ve dosyaya yazar
    while (1) {
        // UDP üzerinden veri al
        n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &len);
        if (n == -1)
            die("recvfrom");
        buffer[n] = '\0';
        fwrite(buffer, 1, n, fp);  // Alınan veriyi dosyaya yaz
        if (n < BUFFER_SIZE)  // Alınan veri boyutu BUFFER_SIZE'dan küçükse veri bitmiştir
            break;
    }

    fclose(fp);
}

/**
 * @brief Ana fonksiyon, UDP sunucusunu başlatır ve dosyayı alır.
 *
 * Bu fonksiyon, UDP socket oluşturur, belirtilen port numarasına bağlar ve dosyayı alır.
 * Alım tamamlandıktan sonra socket'i kapatır.
 *
 * @return int Programın çıkış durumu.
 */
int main() {
    int sockfd;  // UDP socket tanımlayıcısı

    // Socket oluşturma ve bağlama
    sockfd = create_socket();

    // Dosyayı alma
    receive_file(sockfd, RECEIVED_FILE_NAME);

    printf("Dosya alımı tamamlandı.\n");

    // Socket'i kapatma
    close(sockfd);

    return 0;
}
