#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#define SIZE 1024
/*
* @brief dosya yazma işlemleri için kullandığımız fonksiyon
* @param sockfd'den veri gelecektir
*/
void write_file(int sockfd){
  int n;
  FILE *fp;
  char *filename = "recv.svg";
  char buffer[SIZE];

  fp = fopen(filename, "w");
  while (1) {
    // soketten aldığımız veriyi buffer içine yazıyourz, socket -1 döndürdüğünde döngü bitecek
    n = recv(sockfd, buffer, SIZE, 0);
    if (n <= 0){
      break;
      return;
    }
    // dosyaya buffer'ın içeriğini yazdırıyourz
    fprintf(fp, "%s", buffer);
    bzero(buffer, SIZE);
  }
  return;
}

int main(){
  char *ip = "127.0.0.1";
  int port = 8080;
  int e;

  int sockfd, new_sock;
  struct sockaddr_in server_addr, new_addr;
  socklen_t addr_size;
  char buffer[SIZE];
  // socket tanımlaması: IPV4, TCP, flag
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) 
  {
    perror("soket hatası");
    exit(1);
  }
  printf("Server socket oluşturuldu.\n");
  // IPV4 olma durumu
  server_addr.sin_family = AF_INET;
  // port numarası
  server_addr.sin_port = port;
  //  ip numarası
  server_addr.sin_addr.s_addr = inet_addr(ip);
  // soket, server ip ve port bilgileri
  e = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if(e < 0) {
    perror("Binding hatası");
    exit(1);
  }
  printf("Binding başarılı\n");
  // portu dinliyoruz, 5 adet bağlantıya izin verilmiştir
  if(listen(sockfd, 5) == 0){
		printf("dinleniyor\n");
	}else{
		perror("dinleme hatası");
    exit(1);
	}

  addr_size = sizeof(new_addr);
  // dinlenen soket kabul edildiğinde bu fonksiyonu kullanırız
  new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size);
  // dosyaya yazma işlemini burda gerçekleştiririz
  write_file(new_sock);
  printf("yazma işlemi başarılı.\n");

  return 0;
}
