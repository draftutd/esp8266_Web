#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "Nodetest";
const char *password = "password";

ESP8266WebServer server(80); // Set web server port number to 80
// Assign output variables to GPIO pins
const int output1 = 1;
const int output2 = 2;
#define output1_OFF digitalWrite(output1, LOW)
#define output1_ON  digitalWrite(output1, HIGH)
#define output2_OFF digitalWrite(output2, LOW)
#define output2_ON  digitalWrite(output2, HIGH)

//HTML code
const String HtmlHtml = "<html><head>"
                        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" /></head>";
const String HtmlHtmlClose = "</html>";
const String HtmlTitle = "<h1>ESP8266 NodeMCU act as AP and web server to control GPIO</h1><br/>\n";
const String HtmlCenter = "<center>";
const String HtmlCloseCenter = "</center>";
const String HtmlLedState1 = "<h2>Light 1 </h2><br/>\n";
const String HtmlLedState2 = "<h2>Light 2 </h2><br/>\n";
const String HtmlButton1_ON = "<a href=\"LED1Off\"><button style=\"display: block; background-color: #FF0000;  height: 50px; width: 100px;\">ON</button></a><br/>";
const String HtmlButton1_OFF = "<a href=\"LED1On\"><button style=\"display: block; background-color: #00FF00; height: 50px; width: 100px;\">OFF</button></a><br/>";
const String HtmlButton2_ON = "<a href=\"LED2Off\"><button style=\"display: block; background-color: #FF0000;  height: 50px; width: 100px;\">ON</button></a><br/>";
const String HtmlButton2_OFF = "<a href=\"LED2On\"><button style=\"display: block; background-color: #00FF00; height: 50px; width: 100px;\">OFF</button></a><br/>";
String HtmlButtons1, HtmlButtons2;
void handleRoot() {
  output1_OFF;
  HtmlButtons1 =  HtmlButton1_OFF;
  output2_OFF;
  HtmlButtons2 =  HtmlButton2_OFF;
  response();
}
void handleLed1On() {
  output1_ON;
  HtmlButtons1 = HtmlButton1_ON;
  response();
}
void handleLed1Off() {
  output1_OFF;
  HtmlButtons1 =  HtmlButton1_OFF;
  response();
}
void handleLed2On() {
  output2_ON;
  HtmlButtons2 = HtmlButton2_ON;
  response();
}
void handleLed2Off() {
  output2_OFF;
  HtmlButtons2 =  HtmlButton2_OFF;
  response();
}
void response() {
  String htmlRes = HtmlHtml + HtmlCenter;
  htmlRes += HtmlTitle;
  htmlRes += HtmlLedState1;
  htmlRes += HtmlButtons1;
  htmlRes += HtmlLedState2;
  htmlRes += HtmlButtons2;
  htmlRes += HtmlCloseCenter;
  htmlRes += HtmlHtmlClose;
  server.send (200, "text/html", htmlRes);
}
void setup() {
  Serial.begin(115200);
  pinMode(output1, OUTPUT);
  pinMode(output2, OUTPUT);
  output1_OFF;
  output2_OFF;
  WiFi.mode(WIFI_AP); // ใช้งาน WiFi ในโหมด AP
  WiFi.softAP(ssid);  //or WiFi.softAP(ssid, password)
  Serial.print("Connect to : ");
  Serial.println(ssid);
  IPAddress apip = WiFi.softAPIP();
  Serial.print("visit: ");
  Serial.println(apip);
  server.on("/", handleRoot);
  server.on("/LED1On", handleLed1On);
  server.on("/LED1Off", handleLed1Off);
  server.on("/LED2On", handleLed2On);
  server.on("/LED2Off", handleLed2Off);
  server.begin();
}
void loop() {
  server.handleClient();
}
