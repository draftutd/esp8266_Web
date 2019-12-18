#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <SPI.h>
#include "RTClib.h"
#include <SD.h>
File myFile;
#define Buzzer 0
const int chipSelect = 16;
String namee[1000] = "";
String buzzeron = "";
String totalon = "";
int LoG = 1;
int Minon;
int Ninon;
String Mi = "", Ni = "";
char date[20];
char time1[20];
RTC_DS1307 RTC;

const char *ssid = "SaveData";
const char *password = "password";

ESP8266WebServer server(80);

void handle_msg() {
  String date = server.arg("date");
  String time1 = server.arg("time");
  String in = server.arg("in");
  String out = server.arg("out");
  String time2 = server.arg("time1");
  String today = server.arg("today");
  String drink = server.arg("drink");
  String total = server.arg("total");
  buzzeron = time2;
  totalon = total;
  myFile = SD.open("test.txt", FILE_WRITE);
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.print(date); myFile.print("_");
    myFile.print(time1); myFile.print("_");
    myFile.print(in); myFile.print("_");
    myFile.print(out); myFile.print("_");
    myFile.print(in.toInt() - out.toInt()); myFile.print("_");
    myFile.print(today); myFile.print("_");
    myFile.print(drink); myFile.print("_");
    myFile.print(total); myFile.print("_");
    myFile.close();
    Serial.println("done.");
  }
  myFile = SD.open("test.txt");
  int i, w = 0;
  if (myFile) {
    while (myFile.available()) {
      int string = myFile.read();
      if (string != 95) {
        namee[w] = namee[w] + char(string);
      }
      if (string == 95) {
        w++;
      }
    }
    myFile.close();
  }
  const String A  = "<head>";
  const String B  = "<style>table, th, td {border: 1px solid black;border-collapse: collapse;}th, td {padding: 5px;text-align: central;}</style>";
  const String C  = "<form action='back'><p><input type='submit' value='BACK'></form>";
  const String D  = "<table style='width:100%'>";
  const String E  = "<caption><h1>Record Table</h1></caption>";
  const String F  = "<tr><th>DATE</th><th>TIME</th><th>WATER IN</th><th>WATER OUT</th><th>WATER TOTAL</th><th>DRINKING TO DAY</th><th>DRINKING WATER</th><th>DRINKING TOTAL</th></tr>";
  //const String G  = "<tr><td>January</td><td>$100</td><td>January</td><td>$100</td><td>January</td><td>$100</td><td>January</td><td>$100</td></tr>";
  String G  = "";
  for (i = w; i > 0; i -= 8) {
    String ea  = "<tr><td>" + namee[i - 8] + "</td><td>" + namee[i - 7] + "</td><td>" + namee[i - 6] + "</td><td>" + namee[i - 5] + "</td><td>" + namee[i - 4] + "</td><td>" + namee[i - 3] + "</td><td>" + namee[i - 2] + "</td><td>" + namee[i - 1] + "</td></tr>";
    G += ea;
    ea = "";
  }
  for (i = 0; i <= w; i++) {
    namee[i] = "";
  }
  const String H  = "</table></head>";
  String htmlRes = A + B + C + D + E + F + G + H;
  server.send(200, "text/html", htmlRes);

}

void setup() {
  Wire.begin(); // Start the I2C
  Serial.begin(115200);
  RTC.begin();  // Init RTC
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  delay(10);
  while (!Serial) {
    ;
  }
  Serial.print("Initializing SD card...");
  pinMode(SS, OUTPUT);
  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  WiFi.mode(WIFI_AP); // ใช้งาน WiFi ในโหมด AP
  WiFi.softAP(ssid);  //or WiFi.softAP(ssid, password)
  Serial.print("Connect to : ");
  Serial.println(ssid);
  IPAddress apip = WiFi.softAPIP();
  Serial.print("visit: ");
  Serial.println(apip);
  server.on("/", handleRoot);
  server.on("/msg", handle_msg);
  server.on("/back", handleRoot);
  server.begin();
  pinMode(Buzzer, OUTPUT);
}

void handleRoot() {
  const String A  = "<head>";
  const String B  = "<meta http-equiv='Content - Type' content='text / html; charset = UTF - 8' />";
  const String C  = "<form action='msg'>";
  const String D  = "<p>DATE <input type='text' name='date' value='";
  const String DA = "' size=20 autofocus>";
  const String E  = "<p>TIME <input type='text' name='time' value='";
  const String EA = "' size=20 autofocus>";
  const String F  = "<p>WATER IN <input type='number' name='in' size=20 autofocus>";
  const String G  = "<p>WATER OUT <input type='number' name='out' size=20 autofocus>";
  const String GA  = "<p>TIME <input type='number' name='time1' size=20 autofocus> Min";
  const String H  = "<p>DRINKING TO DAY <input type='number' name='today' size=20 autofocus>";
  const String I  = "<p>DRINKING WATER <input type='number' name='drink' size=20 autofocus>";
  const String J  = "<p>TOTAL <input type='number' name='total' size=20 autofocus>";
  const String K  = "<p><input type='submit' value='SAVE'></form>";
  const String L  = "</head>";
  String WEB = A + B + C + D + date + DA + E + time1 + EA + F + G + GA + H + I + J + K + L;
  server.send (200, "text/html", WEB);
}

void loop() {

  char time2[20];
  server.handleClient();
  DateTime now = RTC.now();
  sprintf(date, "%02d/%02d/%02d",  now.day(), now.month(), now.year());
  sprintf(time1, "%02d:%02d:%02d",  now.hour(), now.minute(), now.second());
  Mi = now.minute();
  Ni = now.second();

  if (buzzeron != "" && LoG == 1) {
    Minon = (Mi.toInt() + buzzeron.toInt()) % 60;
    Ninon = Ni.toInt();
    Serial.println(String(now.minute()));
    Serial.println(String(now.second()));
    Serial.println(Minon);
    Serial.println(Ninon);
    buzzeron = "";
    LoG = 0;
  }
  if ((Minon  == Mi.toInt()) && (Ninon  == Ni.toInt()) && (LoG == 0)) {
    Serial.println("Buzzeron");
    Buzzeronn();
    LoG = 1;
  }
  if (totalon.toInt() == 0) {
    Buzzeronn();
    totalon = 100;
  }
}

void Buzzeronn() {
  digitalWrite(Buzzer, HIGH);
  delay(1000);
  digitalWrite(Buzzer, LOW);
  delay(1000);
  digitalWrite(Buzzer, HIGH);
  delay(1000);
  digitalWrite(Buzzer, LOW);
  delay(1000);
  digitalWrite(Buzzer, HIGH);
  delay(1000);
  digitalWrite(Buzzer, LOW);
  delay(1000);
}
