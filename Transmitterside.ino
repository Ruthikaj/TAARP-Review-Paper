#include <LiquidCrystal.h>
LiquidCrystal lcd(3,4,5,6,7,8);
#include <SPI.h>
#include <LoRa.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
TinyGPSPlus gps;
SoftwareSerial ss(A5,A4);
#define pulsesensor A1
int temperaturesensor=A0;
int temperature,hb=0,h=0,count,val;
void Lora_send();
void Lora_Decimal3(int);
void Lcd4_Decimal3(int, int, int);
void gps_loc();
void setup()
{
lcd.begin(16, 2);
lcd.setCursor(0,0); lcd.print(" SOLDIER HEALTH ");
lcd.setCursor(0,1); lcd.print(" LORA SYSYEM TX ");
delay(3000);
Serial.begin(9600);
ss.begin(9600);
lcd.clear();
lcd.setCursor(0,0);
if (!LoRa.begin(433E6))
{
lcd.print("LoRa failed! ");
while (1);
}
else{lcd.print("LoRa is Ready ");}
delay(100);
lcd.clear();
}
void loop()
{
gps_loc();
temperature = analogRead(temperaturesensor);
temperature = temperature/4;
temperature = temperature-115;
if(hb>2)
{
(millis() / 1000);
if(val!=(millis() / 1000)){ val=(millis() / 1000);count++; }
}
if(count>10){h=hb*6;}
if(digitalRead(pulsesensor)==LOW)
{
while(digitalRead(pulsesensor)==LOW);hb++;
}
lcd.setCursor(0,0); lcd.print("T:"); Lcd4_Decimal3(2,0,temperature);
lcd.setCursor(0,1); lcd.print("H:"); Lcd4_Decimal3(2,1,h);
lcd.setCursor(6,0); lcd.print(gps.location.lat(), 6);
lcd.setCursor(6,1); lcd.print(gps.location.lng(), 6);
LoRa.beginPacket();
Lora_Decimal3(temperature);
LoRa.print(':');
Lora_Decimal3(h);
LoRa.print(':');
LoRa.print(gps.location.lat(), 6);
LoRa.print(':');
LoRa.print(gps.location.lng(), 6);
LoRa.print(':');
LoRa.println();
LoRa.endPacket();
delay(500);
Serial.print(gps.location.lat(), 6);
Serial.print(":");
Serial.println(gps.location.lng(), 6);
if(count>10){hb=0;count=0;val=0;}
}
void gps_loc()
{
while (ss.available() > 0)
{
gps.encode(ss.read());
if (gps.location.isUpdated())
{
;
}
}
}
void Lcd4_Decimal2(int clm, int row, int num)
{
unsigned int ans1,ans2;
ans1=num/10;
ans2=num%10;
lcd.setCursor(clm+0, row+0); lcd.print(ans1);
lcd.setCursor(clm+1, row+0); lcd.print(ans2);
}
void Lcd4_Decimal3(int clm, int row, int num)
{
unsigned int ans1,ans2,ans3,a;
ans1=num/100;
a=num%100;
ans2=a/10;
ans3=a%10;
lcd.setCursor(clm+0, row+0); lcd.print(ans1);
lcd.setCursor(clm+1, row+0); lcd.print(ans2);
lcd.setCursor(clm+2, row+0); lcd.print(ans3);
}
void Lora_Decimal3(int val)
{
unsigned int ans1,ans2,ans3,a;
ans1=val/100;
a=val%100;
ans2=a/10;
ans3=a%10;
LoRa.print(ans1);
LoRa.print(ans2);
LoRa.print(
