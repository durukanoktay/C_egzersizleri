#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#define SIZE 1024
/*
 * @brief dosyanın gönderilme işlemi
 * @param *fp dosyayı, sockfd ise bağlanmak için kullandığımız soketi temsil eder
 * fgets fonksiyonu burada temel görevi yerine getiren fonksiyondur. data diye belirttiğmiz buffera, fp dosysından Size kadar veri yazar, yazamadığında Null döndürür ve döngü biter.
 * send fonksiyonu sockfd'ye data'nın içindeki değeri yazar, burdaki 0 bir flag'dir özel bir durum yoksa 0 atanır, eğer send başarısız olursa -1 döner
 * bzero ise buffer'ı temizler
*/
void send_file(FILE *fp, int sockfd){
  int n;
  char data[SIZE] = {0};

  while(fgets(data, SIZE, fp) != NULL) {
    if (send(sockfd, data, sizeof(data), 0) == -1) 
    {
      // hata kodu yollar
      perror("Dosya yollarken bir hata meyda geldi");
      // programdan çıkış yap
      exit(1);
    }
    bzero(data, SIZE);
  }
}

int main(){
  char *ip = "127.0.0.1";
  int port = 8080;
  int e;

  int sockfd;
  // bağlanacağımız sunucuyu belirtmek için sockaddr_in kullanırız
  struct sockaddr_in server_addr;
  FILE *fp;
  char *filename = "send.svg";
  //socket(IPV4 ataması, TCP türünde olduğunu belirtme, 0 flag durumu)
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
    perror("soket hatası");
    exit(1);
  }
  printf("Server socket created successfully.\n");

  server_addr.sin_family = AF_INET; // bağlanacağımız ip inn IPV4 türünde olduğun belirtiyoruz
  server_addr.sin_port = port; // port numarısnı belirtiyoruz
  server_addr.sin_addr.s_addr = inet_addr(ip); // ip numarasını veriyoruz

  // socket'i server'a bağlıyoruz, eğer bağlantı hatası olursa -1 dönecek ve hata mesajı basacağız
  // @param bağlanan soket, bağlantı sağlanan server ip adresi, server adresinin boyutu
  e = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if(e == -1) {
    perror("bağlantı hatası");
    exit(1);
  }
	printf("Server'a bağlanıldı.\n");

  // dosyayı read modunda açıyoruz
  fp = fopen(filename, "r");
  if (fp == NULL) {
    perror("Dosya okunurken bir hata meydana geldi");
    exit(1);
  }

  // dosya yollamak için yukarıda belirttiğimiz fonksiyonu kullanıyoruz
  send_file(fp, sockfd);
  printf("Dosya yollandı.\n");

	printf("bağlantı kapatılıyor.\n");
  // soketi kapatıyoruz
  close(sockfd);

  return 0;
}
