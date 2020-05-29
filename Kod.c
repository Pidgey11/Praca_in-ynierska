#include <stdio.h>
#include <math.h>
#include <wiringPiSPI.h>
#include <wiringPi.h>
#include <stdlib.h>
#define pi 3.141592653689
#define command 3
#define kanal_1 0
#define kanal_2 1
#define kanal_3 2
#define kanal_4 3
#define liczba_serii 32
#define offset 16000
#define N 8192 //liczba probek
int main()
{
wiringPiSetup();
pinMode(0,OUTPUT);
pullUpDnControl(0,PUD_UP);
digitalWrite(0,HIGH);
float f = 8192; // czestotliowsc probkowanie
double ssp[N];
double ssm[N];
double scp[N];
double scm[N];
double sm[N];
//pierwszy syngał
printf("Pierwszy sygnał \n");
float f1; // czestotliwosc sygnalu
float fi1; // faza poczatkowa
float a1; // apmplituda sygnalu
float tetad1; // kat padania fali
printf("czestotliwosc s1 [Hz](wyzsza)= ");
scanf("%f", &f1);
printf("faza poczatkowa s1[rad]= ");
scanf("%f", &fi1);
printf("amplituda s1= ");
scanf("%f", &a1);
printf("kat padania fali s1[deg] = ");
scanf("%f", &tetad1);
double teta1=tetad1*pi/180;
double figs1=2*pi*f1*0.0001*sin(teta1);
double figc1=2*pi*f1*0.0001*cos(teta1);
// drugi sygnał
printf("Drugi sygnał \n");
float f2; // czestotliwosc sygnalu
float fi2; // faza poczatkowa
float a2=; // apmplituda sygnalu
float tetad2; // kat padania fali
printf("czestotliwosc s2 [Hz]= ");
scanf("%f", &f2);
printf("faza poczatkowa s2[rad]= ");
scanf("%f", &fi2);
printf("amplituda s2= ");
scanf("%f", &a2);
printf("kat padania fali s2[deg] = ");
scanf("%f", &tetad2);
double teta2=tetad2*pi/180;
double figs2=2*pi*f2*0.0001*sin(teta2);
double figc2=2*pi*f2*0.0001*cos(teta2);
int fd;
fd = wiringPiSPISetup(1,1000000);
short int i = 0;
float sinus[N];
unsigned char spi_buff[liczba_serii][768];
unsigned char spi_buff1[liczba_serii][768];
unsigned char spi_buff2[liczba_serii][768];
unsigned char spi_buff3[liczba_serii][768];
// operacje na sygnałach
for( t = 0 ; t <N ; t++)
{
// sinus[t] = (rand() % 6000) + 16000.0 + 10000.0*(float)sin(2*pi*f1*(float)((float)t/(float)6100));
int h = rand() % 10000;
ssp[t] =h+ offset + a1*sin(2*pi*(f1/f)*t+figs1+fi1) +a2*sin(2*pi*(f2/f)*t+figs2+fi2);
ssm[t] =h+ offset + a1*sin(2*pi*(f1/f)*t-figs1+fi1) +a2*sin(2*pi*(f2/f)*t-figs2+fi2);
scp[t] =h+ offset + a1*sin(2*pi*(f1/f)*t+figc1+fi1) +a2*sin(2*pi*(f2/f)*t+figc2+fi2);
scm[t] =h+ offset + a1*sin(2*pi*(f1/f)*t-figc1+fi1) +a2*sin(2*pi*(f2/f)*t-figc2+fi2);
sm[t] = ssp[t] +ssm[t]+scp[t]+scm[t];
}
//przypisywanie do buforow nadawczych
i =0;
for(int n = 0 ; n < liczba_serii; n++){
for(int m =0; m <768; m+=3){
i = (m/3)+n*256;
spi_buff[n][m+2]=(short int)ssp[i]&0x00FF;
spi_buff[n][m+1]=((short int)ssp[i]&0xFF00)>>8;
spi_buff[n][m] = (unsigned char)((command << 4) + (kanal_1));
spi_buff1[n][m+2]=(short int)ssm[i]&0x00FF;
spi_buff1[n][m+1]=((short int)ssm[i]&0xFF00)>>8;
spi_buff1[n][m] = (unsigned char)((command << 4) + (kanal_2));
spi_buff2[n][m+2]=(short int)scp[i]&0x00FF;
spi_buff2[n][m+1]=((short int)scp[i]&0xFF00)>>8;
spi_buff2[n][m] = (unsigned char)((command << 4) + (kanal_3));
spi_buff3[n][m+2]=(short int)scm[i]&0x00FF;
spi_buff3[n][m+1]=((short int)scm[i]&0xFF00)>>8;
spi_buff3[n][m] = (unsigned char)((command << 4) + (kanal_4));
}
}
//wysylanie do DAC
while(1){
for(int l = 0; l < liczba_serii; l++){
for(int j = 0; j <3*256 ; j+=3){
digitalWrite(0,LOW);
wiringPiSPIDataRW(1, (unsigned char*)&spi_buff[l][j], 3);
digitalWrite(0,HIGH);
digitalWrite(0,LOW);
wiringPiSPIDataRW(1, (unsigned char*)&spi_buff1[l][j], 3);
digitalWrite(0,HIGH);
digitalWrite(0,LOW);
wiringPiSPIDataRW(1, (unsigned char*)&spi_buff2[l][j], 3);
digitalWrite(0,HIGH);
digitalWrite(0,LOW);
wiringPiSPIDataRW(1, (unsigned char*)&spi_buff3[l][j], 3);
digitalWrite(0,HIGH);
}
}
delay(1000);
}
return 0;
}