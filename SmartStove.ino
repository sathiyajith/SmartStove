#include <Q2HX711.h>
#include <SimpleTimer.h>
#define BLYNK_PRINT SwSerial


#include <SoftwareSerial.h>
SoftwareSerial SwSerial(10, 11); // RX, TX
    
#include <BlynkSimpleStream.h>
char auth[] = "ee2232746bc94de3abcdec4527dcf5b0";

const int ledpin = 9;

const int buzzerpin = 11;

const int heatpin = A0;
const int gaspin=A1;

const byte datapin = A2;
const byte clockpin = A3;

int mq2thres= 70; //heat sensor threshold
int mq5thres=200; //gas sensor threshold


int loadthres=1;
Q2HX711 hx711(datapin,clockpin);

SimpleTimer timer;
BLYNK_WRITE(V4)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
  digitalWrite(ledpin,pinValue);
  }

BLYNK_WRITE(V5)
{
  int timervalue=param.asInt();
  digitalWrite(ledpin,timervalue);
 }

void setup() {
  SwSerial.begin(9600);
  Serial.begin(9600);
  Blynk.begin(Serial, auth);
  pinMode(ledpin, OUTPUT);
  pinMode(buzzerpin, OUTPUT);  
  pinMode(heatpin, INPUT);
  pinMode(gaspin,INPUT);
  Blynk.email("sanjailal4@gmail.com","SmartStoveAlert","SMART STOVE RUNNING GOOD!!");  
  digitalWrite(ledpin, HIGH);  
  timer.setTimeout(60000, repeatMe);
}
void repeatMe() {
      Serial.println(millis()/1000);
      Serial.println("nnnnnnnnnnnnnnnnn");
     double weight=((hx711.read()/100000.0)-83.89);
     Serial.println("yyyyyyyyyyyyyyyy");
     if(weight<loadthres)
     digitalWrite(ledpin, LOW); 
}
void loop() {
  digitalWrite(buzzerpin,LOW);
   Blynk.run();
  int heatvalue=analogRead(heatpin);
  int gasvalue=analogRead(gaspin);
  double weight=((hx711.read()/100000.0)-82.00);
  
Blynk.virtualWrite(V1,gasvalue);
  if (gasvalue > mq5thres)
  {
    digitalWrite(ledpin, LOW);  
    digitalWrite(buzzerpin,HIGH);
    Serial.println("Gas leak detected ");
    
  } 
   else if ((weight < loadthres)&&(heatvalue > mq2thres))
  {
    timer.run();
    Serial.println("heatloss detected ");
  }
Blynk.virtualWrite(V2,heatvalue);  
Blynk.virtualWrite(V3,weight);

  digitalWrite(buzzerpin,LOW);
  
  delay(1000);
}
