#include <WiFi.h>
#include <HTTPClient.h>
const char* ssid = "ASUSX555Q";
const char* pass = "welcomestark";
const char* host = "mousetrap.promonissanbogor.com";
//const char* host = "192.168.137.1";
int getar = 15;
int outputsensor;
int period = 100;
unsigned long time_now = 0;
bool flag1 = false;
int led = 13;

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_OFF);
  delay(1000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  Serial.println("");
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  // --------------------------------- //
  pinMode(getar, INPUT);
  pinMode(led, OUTPUT);
}

void loop() {
  if (millis() > time_now + period) {
    time_now = millis();
    outputsensor = digitalRead(getar);
    if ((outputsensor == LOW) && (flag1 == false))
    {
      outputsensor = 0;
      Serial.println(outputsensor);
      KirimData();
      flag1 = true;
    }

    if ((outputsensor == HIGH) && (flag1 == true))
    {
      while (1) {
        outputsensor = 1;
        Serial.println(outputsensor);
        KirimData();
        goto lampu;
      }
lampu:
      while (1) {
        digitalWrite(led, HIGH);
        delay(1000);
        digitalWrite(led, LOW);
        delay(500);
      }
    }
  }
}

void KirimData() {
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("Connection Failed");
    return;
  }
  String Link;
  HTTPClient http;
  Link = "http://" + String(host) + "/tambah_proses.php?logsensor=" + String(outputsensor);
  http.begin(Link);
  http.GET();
  String respon = http.getString();
  Serial.println(respon);
  http.end();
}
