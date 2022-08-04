#include<stdio.h>

void zenerfonk(float girisGerilim, float direnc,float yukDirenci,float zenerMaxguc,float zenerGerilim){
float akim,yukgerilim;
akim = girisGerilim/ (direnc+yukDirenci);
yukgerilim = akim*yukDirenci;
printf("\n akim değeri: %f", akim);
printf("\n yuk gerilimi: %f", yukgerilim);
    if(yukgerilim>zenerGerilim){ //zenerin aktif olduğu durum
printf("\n zener aktif");
yukgerilim = zenerGerilim;
printf("\n yukgerilimi zener gerilimine esittir");
float direncgerilimi;
direncgerilimi = girisGerilim-yukgerilim;
printf("\ndirenc gerilimi", direncgerilimi);
float akim1,yukakimi,zenerakimi;
akim1= direncgerilimi/direnc;
yukakimi= yukgerilim/yukDirenci;
zenerakimi= akim1-yukakimi;
printf("\nzenerakimi: %f", zenerakimi);
                                     }
    else { //zenerin pasif olduğu durum
float direncgerilimi;
printf("\nzener pasif durumda");
direncgerilimi = girisGerilim-yukgerilim;
printf("\ndirenc gerilimi", direncgerilimi);
float zenerakimi;
zenerakimi= 0;
printf("\nzenerakimi: %f", zenerakimi);
    }
        }

int main(){
float girisGerilim,direnc,yukDirenci,zenerMaxguc,zenerGerilim;
printf("siraile lutfen giris gerilimi,direnc,yukdirenci,zener max gucunu ve zener gerilimini giriniz");
scanf("%f", & girisGerilim);
scanf("%f", & direnc);
scanf("%f", & yukDirenci);
scanf("%f", & zenerMaxguc);
scanf("%f", & zenerGerilim);
//Durukan Oktay 1810205045

zenerfonk(girisGerilim,direnc,yukDirenci,zenerMaxguc,zenerGerilim);


return 0; }