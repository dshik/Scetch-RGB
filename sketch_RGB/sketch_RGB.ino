#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

//Создаем экземпляр класса для DNS
MDNSResponder mdns;

//Задаём параметры сети
const char* ssid = "iPhone Дмитрий";
const char* password = "cbmk5e67va0t7";

ESP8266WebServer server(80);

String webPage = "";

//объявляем переменные с номерами пинов
int pinR = 1;
int pinG = 3;
int pinB = 15;

void setup() {
  webPage += "<h1>G1Tech: ESP8266 Web Server</h1><p>LED = 16 <a href=\"LEDOn\"><button>ON</button></a>&nbsp;<a href=\"LEDOff\"><button>OFF</button></a></p>";
  
  //объявляем используемые порты
  pinMode(pinR, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(pinB, OUTPUT);
  digitalWrite(pinR, LOW);
  digitalWrite(pinG, LOW);
  digitalWrite(pinB, LOW);

  delay(1000);
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.println("GO!");

  //Устанавливаем соединение
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("YES!");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP adres: ");
  Serial.println(WiFi.localIP());

  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }

  server.on("/", [] () {
    server.send(200, "text/html", webPage);
  });

  server.on("/LEDOn", [] () {
    server.send(200, "text/html", webPage);
    digitalWrite(pinR, HIGH);
    digitalWrite(pinG, HIGH);
    digitalWrite(pinB, HIGH);
    delay(1000);
  });

  server.on("/LEDOff", [] () {
    server.send(200, "text/html", webPage);
    digitalWrite(pinR, LOW);
    digitalWrite(pinG, LOW);
    digitalWrite(pinB, LOW);
    delay(1000);
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

  digitalWrite(pinR, HIGH); //включаем красный
  delay(500); //ждем 500 Мс
  digitalWrite(pinR, LOW); //выключаем красный
  digitalWrite(pinG, HIGH); //включаем зеленый
  delay(500); //ждем 500 Мс
  digitalWrite(pinG, LOW); //выключаем зеленый
  digitalWrite(pinB, HIGH); //включаем синий
  delay(500); //ждем 500 Мс
  digitalWrite(pinB, LOW); //выключаем синий

}
