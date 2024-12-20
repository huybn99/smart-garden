#define BLYNK_TEMPLATE_ID "TMPL6BmNU04gZ"
#define BLYNK_DEVICE_NAME "VUONTHONGMINH"
#define BLYNK_FIRMWARE_VERSION        "0.1.0"
#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG
#define APP_DEBUG
#include "BlynkEdgent.h"
#include <BH1750.h>
#include <DHT.h>
#include <Wire.h>
#include <sServo.h>
#include <LiquidCrystal_I2C.h>
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 20, 4);
DHT dht(26, DHT11);// Cấu hình chân DHT11
static const int servoPin = 13;
Servo servo1;
BH1750 lightMeter;
#define sensorPin  35
#define sensorPin1 34
unsigned long timeDelay = millis();
boolean updateState=0;
unsigned long timeBlink=millis();
WidgetLED ledConnect(V0);
const int out2 = 18;
const int out3 = 27;
const int out1 = 14;
boolean bt1State = LOW;
boolean bt2State = LOW;
boolean bt3State = LOW;
boolean bt4State = LOW;
BLYNK_WRITE(V2){
  int p = param.asInt();
  digitalWrite(out2,p);
}
BLYNK_WRITE(V6){
  int p = param.asInt();
  digitalWrite(out3,p);
}
BLYNK_WRITE(V1){
  int p = param.asInt();
  digitalWrite(out1,p);
}
int readSensor1() {
    int val = analogRead(sensorPin1);    // Read the analog value form sensor
    return val;                // Return analog moisture value
}
int morem()
{
  for(int posDegrees = 0; posDegrees < 126; posDegrees++) {
    servo1.write(posDegrees);
    delay(20);
  }

}
int dongrem()
{
  for(int posDegrees = 126; posDegrees >= 0; posDegrees--) {
    servo1.write(posDegrees);
    delay(20);
  }
}
void setup() {
  Serial.begin(9600);
  Wire.begin();
  servo1.attach(servoPin);
  lightMeter.begin();
  pinMode(out1,OUTPUT);
  pinMode(out2,OUTPUT);
  pinMode(out3,OUTPUT);
  digitalWrite(out2, LOW);
  digitalWrite(out3, LOW);
  digitalWrite(out1, LOW);
  dht.begin();
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("DO AN TOT NGHIEP");
  lcd.setCursor(0, 1);
  lcd.print("NGUYEN QUANG DUC");
  lcd.setCursor(0, 2);
  lcd.print("NGUYEN QUANG HUY");
  lcd.setCursor(0, 3);
  lcd.print("PHAM THANH TU");
  Serial.println(F("BH1750 Test begin"));
  BlynkEdgent.begin();
  delay(3000);
  lcd.clear();
}
void loop() {
  BlynkEdgent.run();
  int Humidity = dht.readHumidity();// đọc độ ẩm    
  int Temperature = dht.readTemperature();// đọc nhiệt độ
  int rainAnalogVal = analogRead(sensorPin);
 int luongmua= 20-(rainAnalogVal*20)/4095;
  if(luongmua>10)
  {
  dongrem();
  lcd.setCursor(15, 2);
  lcd.print("MUA");
  }else
  {
  morem();
  lcd.setCursor(15, 1);
  lcd.print("BT "); 
  }
    if(ledConnect.getValue())
    {
      ledConnect.off();
    }else{
      ledConnect.on();
    }
  int lux1 = lightMeter.readLightLevel();
  Serial.print("Light1: ");
  Serial.print(lux1);
  Serial.println(" lx");
  Blynk.virtualWrite(V4,lux1);
  Serial.print("Analog output1: ");
  Serial.println(readSensor1());
  int dat1=110- (readSensor1()*100/4095);
  Blynk.virtualWrite(V3,dat1);
  Blynk.virtualWrite(V5,Temperature);
  Blynk.virtualWrite(V6,Humidity);
  Blynk.virtualWrite(V7,luongmua);
  Blynk.virtualWrite(V1,digitalRead(out1));
  Blynk.virtualWrite(V2,digitalRead(out2));
  lcd.setCursor(0, 0);
  lcd.print(Temperature);
  lcd.print("ND:");
  lcd.write(0xDF);
  lcd.print("C");
  lcd.setCursor(8, 0);
  lcd.print("DA:");
  lcd.print(Humidity);
  lcd.print("% ");
  lcd.setCursor(0, 1);
  lcd.print("CUONG DO SANG:");
  lcd.print(lux1); 
  lcd.print("lux        "); 
  lcd.setCursor(0, 3);
  lcd.print("LUONG MUA:");
  lcd.setCursor(13, 3);
  lcd.print(luongmua);
  lcd.print("mm   ");
  if(dat1<60)
  {
    if(bt1State==LOW)
    {
      digitalWrite(out1,1);
      Blynk.virtualWrite(V1,digitalRead(out1));
      delay(200);
      bt1State=HIGH;
    }
  }else{
    if(bt1State==HIGH)
    {
      digitalWrite(out1,0);
      Blynk.virtualWrite(V1,digitalRead(out1));
      delay(200);
      bt1State=LOW;
    }
  }
  if(lux1<10000){
    if(bt2State==LOW){
      digitalWrite(out2,1);
      Blynk.virtualWrite(V2,digitalRead(out2));
      delay(200);
      bt2State=HIGH;
    }
  }else{
  if(bt2State==HIGH){
      digitalWrite(out2,0);
      Blynk.virtualWrite(V2,digitalRead(out2));
      delay(200);
      bt2State=LOW;
    }
  }
  
  delay(200);
}
