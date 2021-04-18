#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

// Установить свой SSID и пароль 
const char* ssid = "NodeMCU";
const char* password = "12345678";

// Настройка IP-адрес
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);

uint8_t LED1pin = D7;
bool LED1status = LOW;

uint8_t LED2pin = D6;
bool LED2status = LOW;

uint8_t LED3pin = D5;
bool LED3status = LOW;

void setup() {
  Serial.begin(115200);
  pinMode(LED1pin, OUTPUT);
  pinMode(LED2pin, OUTPUT);
  pinMode(LED3pin, OUTPUT);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  server.on("/", handle_OnConnect);
  
  server.on("/led1on", handle_led1on);
  server.on("/led1off", handle_led1off);
  
  server.on("/led2on", handle_led2on);
  server.on("/led2off", handle_led2off);
  
  server.on("/led3on", handle_led3on);
  server.on("/led3off", handle_led3off);
 
  server.onNotFound(handle_NotFound);
  
  server.begin();
  Serial.println("HTTP SERVER STARTED");

  ledReady();
}
void loop() {
  server.handleClient();
  if(LED1status)
    {digitalWrite(LED1pin, HIGH);}
  else
    {digitalWrite(LED1pin, LOW);}
  
  if(LED2status)
    {digitalWrite(LED2pin, HIGH);}
  else
    {digitalWrite(LED2pin, LOW);}

  if(LED3status)
    {digitalWrite(LED3pin, HIGH);}
  else
    {digitalWrite(LED3pin, LOW);}

}

void handle_OnConnect() {
  LED1status = LOW;
  LED2status = LOW;
  LED3status = LOW;
  Serial.println("GPIO7 Status: OFF | GPIO6 Status: OFF | GPIO5 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,LED2status,LED3status)); 
}

void ledReady() {
  digitalWrite(LED1pin, HIGH);
  delay(500);
  digitalWrite(LED1pin, LOW);
  delay(500);
  digitalWrite(LED2pin, HIGH);
  delay(500);
  digitalWrite(LED2pin, LOW);
  delay(500);
  digitalWrite(LED3pin, HIGH);
  delay(500);
  digitalWrite(LED3pin, LOW);
}

void handle_led1on() {
  LED1status = HIGH;
  Serial.println("GPIO7 STATUS: ON");
  server.send(200, "text/html", SendHTML(true,LED2status,LED3status));
}

void handle_led1off() {
  LED1status = LOW;
  Serial.println("GPIO7 STATUS: OFF");
  server.send(200, "text/html", SendHTML(false,LED2status,LED3status)); 
}

void handle_led2on() {
  LED2status = HIGH;
  Serial.println("GPIO6 STATUS: ON");
  server.send(200, "text/html", SendHTML(LED1status,true,LED3status)); 
}

void handle_led2off() {
  LED2status = LOW;
  Serial.println("GPIO6 STATUS: OFF");
  server.send(200, "text/html", SendHTML(LED1status,false,LED3status)); 
}

void handle_led3on() {
  LED3status = HIGH;
  Serial.println("GPIO5 STATUS: ON");
  server.send(200, "text/html", SendHTML(LED1status,LED2status,true)); 
}

void handle_led3off() {
  LED3status = LOW;
  Serial.println("GPIO5 STATUS: OFF");
  server.send(200, "text/html", SendHTML(LED1status,LED2status,false)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "NOT FOUND");
}

String SendHTML(uint8_t led1stat,uint8_t led2stat,uint8_t led3stat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #1abc9c;}\n";
  ptr +=".button-on:active {background-color: #16a085;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>ESP8266 Web Server</h1>\n";
  ptr +="<h3>Using Access Point(AP) Mode</h3>\n";
  
  if(led1stat)
    {ptr +="<p>LED1 Status: ON</p><a class=\"button button-off\" href=\"/led1off\">OFF</a>\n";}
  else
    {ptr +="<p>LED1 Status: OFF</p><a class=\"button button-on\" href=\"/led1on\">ON</a>\n";}

  if(led2stat)
    {ptr +="<p>LED2 Status: ON</p><a class=\"button button-off\" href=\"/led2off\">OFF</a>\n";}
  else
    {ptr +="<p>LED2 Status: OFF</p><a class=\"button button-on\" href=\"/led2on\">ON</a>\n";}

  if(led3stat)
    {ptr +="<p>LED3 Status: ON</p><a class=\"button button-off\" href=\"/led3off\">OFF</a>\n";}
  else
    {ptr +="<p>LED3 Status: OFF</p><a class=\"button button-on\" href=\"/led3on\">ON</a>\n";}

  ptr +="</body>\n";
  ptr +="</html>\n";

  StaticJsonDocument<100> jsonDocument;

  if(led1stat)
    {jsonDocument["LED1"] = "On";}
  else
    {jsonDocument["LED1"] = "Off";}

  if(led2stat)
    {jsonDocument["LED2"] = "On";}
  else
    {jsonDocument["LED2"] = "Off";}

   if(led3stat)
     {jsonDocument["LED3"] = "On";}
   else
     {jsonDocument["LED3"] = "Off";}
    
  char buffer[100];

  serializeJsonPretty(jsonDocument, buffer);

  Serial.println(buffer);
  return buffer;
}
