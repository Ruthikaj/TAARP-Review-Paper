#include <SPI.h>
#include <LoRa.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(3,4,5,6,7,8);
#define alarm A0
void Lcd4_Decimal2(int, int, int);
void Lcd4_Decimal3(int, int, int);
void Lora_Event();
void Lora_Received();
int Receive=1;//lora
char data[52];
int ind=0;
int t,h;
char latitude[10],longitude[10];
void setup()
{
pinMode(alarm, OUTPUT);
digitalWrite(alarm, LOW);
Serial.begin(9600);
while (!Serial);
Serial.println("LORA RX");
lcd.begin(16, 2);
lcd.setCursor(0,0);
lcd.print(" SOLDIER HEALTH ");
lcd.setCursor(0,1);
lcd.print(" LORA SYSTEM RX ");
delay(2000);
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
Lora_Event();
lcd.setCursor(0,0); lcd.print("T:"); Lcd4_Decimal3(2,0,t);
lcd.setCursor(0,1); lcd.print("H:");Lcd4_Decimal3(2,1,h);
lcd.setCursor(6,0); lcd.print(latitude);
lcd.setCursor(6,1); lcd.print(longitude);
if(t>40){digitalWrite(alarm, HIGH);}
else if(h>100){digitalWrite(alarm, HIGH);}
else{digitalWrite(alarm, LOW);}
}
void Lora_Event()
{
int packetSize = LoRa.parsePacket();
if (packetSize)
{
// if (!LoRa.available()) {Lora_send();}
while (LoRa.available())
{
if(Receive==1)
{
//123:123 //temp:rate
data[ind]=(char)LoRa.read();
if(data[ind]!='\n'){ind++;}
if(data[ind]=='\n'){Receive=0; Lora_Received(); Receive=1;}
if(ind>50){ind=0;}
}
}
}
}
/////////////////////////////////////////////////////////////////////////////////////
/////
void Lora_Received()
{
//123:123 //temp:rate
t = (data[0 ]-'0')*100 + (data[1 ] -'0')*10 + (data[2 ]-'0')*1;
h = (data[4 ]-'0')*100 + (data[5 ] -'0')*10 + (data[6 ]-'0')*1;
for(int i=0;i<9;i++){latitude[i]=data[8+i];}
for(int j=0;j<9;j++){longitude[j]=data[18+j];}
data[ind]=' ';
ind=0;
Serial.println(data);
}
//////////////////////////////////////////////
void Lcd4_Decimal2(int clm, int row, int num)
{
unsigned int ans1,ans2;
ans1=num/10;
ans2=num%10;
lcd.setCursor(clm+0, row+0); lcd.print(ans1);
lcd.setCursor(clm+1, row+0); lcd.print(ans2);
}
//////////////////////////////////////////////
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
