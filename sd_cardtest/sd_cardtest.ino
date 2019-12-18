#include <SPI.h>
#include <SD.h>
File myFile;
const int chipSelect = 16;
String namee[1000];
void setup()
{
  Serial.begin(115200);
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
  myFile = SD.open("test.txt", FILE_WRITE);
  if (myFile) {
    Serial.print("Writing to test.txt...");
    //myFile.println("ABCDEFG");
    myFile.close();
    Serial.println("done.");
  } else {
    Serial.println("error opening test.txt");
  }

  myFile = SD.open("test.txt");
  if (myFile) {
    int i,w = 0;
    Serial.println("test.txt:");
    while (myFile.available()) {
      int string = myFile.read();
      if (string != 95) {
        namee[w] = namee[w] + char(string);
      }
      if (string == 95) {
        w++;
      }
      //Serial.print(char(myFile.read()));
      //Serial.write(myFile.read());
    }
    for(i=0;i<=w;i++){
    Serial.println(namee[i]);
    }
    myFile.close(); // เมื่ออ่านเสร็จ ปิดไฟล์
  } else {
    // ถ้าอ่านไม่สำเร็จ ให้แสดง error
    Serial.println("error opening test.txt");
  }
}
void loop()
{
}
