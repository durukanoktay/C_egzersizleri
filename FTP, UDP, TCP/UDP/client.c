/**
 * @file client.c
 * @brief Dosya Göndermek için UDP İstemcisi
 *
 * Bu program, bir UDP istemcisi oluşturur ve belirtilen dosyayı sunucuya gönderir.
 * Dosya BUFFER_SIZE baytlık parçalar halinde okunur ve gönderilir.
 * 
 * @date 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

// Dosya parçalarını okumak ve göndermek için buffer boyutu
#define BUFFER_SIZE 1024

// Sunucu port numarası
#define PORT 8080

// Sunucu IP adresi
#define SERVER_IP "127.0.0.1"

// Gönderilecek dosyanın adı
#define FILE_NAME "send_file.txt"

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
 * @brief UDP socket oluşturur ve döndürür.
 *
 * Bu fonksiyon, IPv4 ve UDP kullanarak bir socket oluşturur ve socket tanımlayıcısını döner.
 *
 * @return int Oluşturulan socket tanımlayıcısı.
 */
int create_socket() {
    int sockfd;
    // AF_INET: IPv4, SOCK_DGRAM: UDP, 0: protokolü otomatik seç
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        die("socket");
    return sockfd;
}

/**
 * @brief Belirtilen dosyayı UDP üzerinden sunucuya gönderir.
 *
 * Bu fonksiyon, dosyayı BUFFER_SIZE baytlık parçalar halinde okur ve sunucuya gönderir.
 * Eğer gönderim sırasında herhangi bir hata oluşursa, hata mesajı basar ve programdan çıkar.
 *
 * @param sockfd UDP socket tanımlayıcısı.
 * @param server_addr Sunucunun adres yapısı.
 * @param filename Gönderilecek dosyanın adı.
 */
void send_file(int sockfd, struct sockaddr_in *server_addr, char *filename) {
    FILE *fp;
    // Dosyayı okuma modunda aç
    if ((fp = fopen(filename, "rb")) == NULL)
        die("Dosya açma");

    char buffer[BUFFER_SIZE];
    int n;

    // Dosyayı okur ve sunucuya gönderir
    while ((n = fread(buffer, 1, BUFFER_SIZE, fp)) > 0) {
        // UDP üzerinden sunucuya veri gönder
        if (sendto(sockfd, buffer, n, 0, (struct sockaddr *)server_addr, sizeof(*server_addr)) == -1)
            die("sendto");
        // Gönderim arası kısa bir bekleme süresi ekleyebilirsiniz
        usleep(1000);
    }

    fclose(fp);
}

/**
 * @brief Ana fonksiyon, UDP istemcisini başlatır ve dosyayı sunucuya gönderir.
 *
 * Bu fonksiyon, UDP socket oluşturur, sunucu adresini ayarlar ve belirtilen dosyayı sunucuya gönderir.
 * Gönderim tamamlandıktan sonra socket'i kapatır.
 *
 * @return int Programın çıkış durumu.
 */
int main() {
    int sockfd;  // UDP socket tanımlayıcısı
    struct sockaddr_in server_addr;  // Sunucu adres yapısı

    // Socket oluşturma
    sockfd = create_socket();

    // Sunucu adresi ayarlama
    memset(&server_addr, 0, sizeof(server_addr));  // server_addr yapısını sıfırlarla doldur
    server_addr.sin_family = AF_INET;              // Adres ailesi IPv4
    server_addr.sin_port = htons(PORT);            // Port numarasını ayarla (host to network short)
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0)  // IP adresini binary formata çevir
        die("inet_pton");

    // Dosyayı gönderme
    send_file(sockfd, &server_addr, FILE_NAME);

    printf("Dosya gönderimi tamamlandı.\n");

    // Socket'i kapatma
    close(sockfd);

    return 0;
}
