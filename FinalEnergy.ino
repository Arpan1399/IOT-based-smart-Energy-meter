#include<ESP8266WiFi.h>
#include<WiFiClient.h>
#include<ESP8266WebServer.h>
#include <PZEM004Tv30.h>
#include <BlynkSimpleEsp8266.h>
PZEM004Tv30 pzem(D1,D2);  //RX/TX
const char* ssid = "arpan";//Replace with your network SSID
const char* password = "123456789";//Replace with your network password

ESP8266WebServer server(80);
float voltage,current,pf,frequency,power,Unit,Price;
float voltage_blynk=0;
float current_blynk=0;
float power_blynk=0;
float energy_blynk=0;
float frequency_blynk=0;
float Unit_blynk=0;
float Price_blynk=0;

char auth[] = "31OvyZP2MTHzFc6DaGMuqeZhmknymkIO";
unsigned long lastMillis = 0;
unsigned int energyTariff = 8.0;

String page = "";
void setup() 
{
Serial.begin(115200);
WiFi.begin(ssid, password);
Blynk.begin(auth, ssid, password);
while (WiFi.status() != WL_CONNECTED)
  {
  delay(500);
  Serial.print(".");
  }
  Serial.println(WiFi.localIP());
  server.on("/", []()
  {
    page = "<html><head><title>Smart Energy Meter using IoT</title></head><style type=\"text/css\">";
    page += "table{border-collapse: collapse;}th {background-color:  green ;color: white;}table,td {border: 4px solid black;font-size: x-large;";
    page += "text-align:center;border-style: groove;border-color: rgb(255,0,0);}</style><body><center>";
    page += "<h1>Smart Energy Meter using IoT</h1><br><br><table style=\"width: 1200px;height: 450px;\"><tr>";
    page += "<th>Parameters</th><th>Value</th><th>Units</th></tr>";
    page += "<tr><td>Voltage</td><td>"+String(voltage)+"</td><td>Volts</td></tr>";
    page += "<tr><td>Current</td><td>"+String(current)+"</td><td>Amperes</td></tr>";
    page += "<tr><td>Power Factor</td><td>"+String(pf)+"</td><td>XXXX</td></tr>";
    page += "<tr><td>Power</td><td>"+String(power)+"</td><td>Watts</td></tr>";
    page += "<tr><td>Frequency</td><td>"+String(frequency,1)+"</td><td>Hz</td></tr>";
    page += "<tr><td>Unit</td><td>"+String(Unit,5)+"</td><td>Unit</td></tr>";
    page += "<tr><td>Price</td><td>"+String(Price,5)+"</td><td>INR</td></tr>";
    page += "<meta http-equiv=\"refresh\" content=\"3\">";
    server.send(200, "text/html", page);
  });
  server.begin();
}
void loop() 
{
Blynk.run();
voltage = pzem.voltage();
if(voltage >= 0.0){   voltage_blynk =voltage; }
current = pzem.current();
if(current >= 0.0){ current_blynk=current;    }  //A 
power = pzem.power();
 if(power >= 0.0){power_blynk=power;       } //kW
frequency = pzem.frequency();
 if(frequency >= 0.0){frequency_blynk=frequency;}
pf = pzem.pf();
 if(pf >= 0.0){  energy_blynk =pf;  } ///kWh
Unit = Unit+((pf*lastMillis)/60000000);
 if(Unit >= 0){Unit_blynk=Unit;} //inr
Price = Unit*energyTariff;
 if(Price >= 0){Price_blynk=Price;} //inr 
delay(1000);
      //Publish data every 10 seconds (10000 milliseconds). Change this value to publish at a different interval.
          if (millis() - lastMillis > 10000) {
            lastMillis = millis();
            Blynk.virtualWrite(V1, voltage_blynk);
            Blynk.virtualWrite(V2, frequency_blynk);
            Blynk.virtualWrite(V3, current_blynk);            
            Blynk.virtualWrite(V4, power_blynk);
            Blynk.virtualWrite(V5, lastMillis);
            Blynk.virtualWrite(V6, energy_blynk);
            Blynk.virtualWrite(V7, Unit_blynk);
            Blynk.virtualWrite(V8, Price_blynk);
          }
server.handleClient();
}
