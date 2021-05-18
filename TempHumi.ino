#include <Wire.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#define DHT_DataPin 9
#define DHTTYPE DHT11
DHT dht(DHT_DataPin,DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
SoftwareSerial BT(10,11);// 接收腳, 傳送腳,RX11,TX10

const int sensorPin=2;
int HC = 9;
int LEDRed = 13;
int LEDRed1 = 8;
int val;
int hc;
void setup()
{
  dht.begin();
  pinMode(HC, OUTPUT);
  pinMode(LEDRed,OUTPUT); 
  pinMode(LEDRed1,OUTPUT); 
  pinMode(sensorPin,INPUT);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0); // 設定游標位置在第一行行首
  lcd.print("");
  Serial.begin(9600);
  BT.begin(9600);

  Serial.println("BT is ready!");
  BT.begin(38400);
}
void loop()
{
  delay(36000);
  int c = dht.readTemperature();
  
  Serial.print("T:");
  Serial.print(c);
  Serial.println("C");

  char value0[21];
  char value1[21];

  sprintf(value0, "T:%dC ", c);
  sprintf(value1, "%-20s", value0);
  lcd.setCursor(0,1);
  lcd.print(value1);
  float moist;
  // 若收到「序列埠監控視窗」的資料，則送到藍牙模組
  if (Serial.available())
  {
    val = Serial.read();
     BT.print(val);
  }
  if(Serial.available())
  {
   hc = BT.read();
   BT.println(hc); 
}

   // 若收到藍牙模組的資料，則送到「序列埠監控視窗」
  if (BT.available())
  {
    val = BT.read();
    hc = BT.read();
    Serial.print(val);
    BT.print(hc);
  }
  lcd.setCursor(9, 0);
  lcd.print("SH:");
  lcd.print(val);
  lcd.print("%");
  lcd.setCursor(9,1);
  lcd.print("SC:");
  lcd.print(hc);
  lcd.print("C");

  moist = analogRead(sensorPin);
  lcd.setCursor(0, 0);
  lcd.print("H:");
  lcd.print(moist/710*100); //土壤溼度數值轉換成百分比 並顯示在LCD上
  lcd.print("%");
  delay(500);
  lcd.setCursor(0, 0);
  lcd.print("         ");
  

   
  
  
  
  if(moist/710*100>val) //土壤溼度低於40% LED燈會亮起 代表灑水
  digitalWrite(LEDRed, HIGH);
  else
  digitalWrite(LEDRed, LOW);
  if(c>hc)
  digitalWrite(LEDRed1, LOW);
  else
  digitalWrite(LEDRed1, HIGH);
}
