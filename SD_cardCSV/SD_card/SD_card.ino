#include <LiquidCrystal.h>
LiquidCrystal lcd(9,8,5,4,3,2);
#include <SD.h>
File myFile;
const int chipSelect=10;

int dat1=A0;
int dat2=A1;
int data1=0;
int data2=0;
int i=0;
boolean present=0;
void setup()
{
  pinMode(dat1,INPUT);
  pinMode(dat2,INPUT);
  lcd.begin(20,4);
  lcd.clear();
  lcd.home();
  checkSD();
  // Write label Excel
  myFile = SD.open("datalog.csv", FILE_WRITE);
  if(myFile)
  {
    myFile.print("line");
    myFile.print(",");
    myFile.print("data1");
    myFile.print(",");
    myFile.println("data2");
    myFile.close();
    
  }
}
void loop()
{
  if(present==1)
  {
    //do it
    lcd.clear();
    i++;
    data1=analogRead(dat1);
    data2=analogRead(dat2);
    lcd.home();
    lcd.print("Val1 : ");
    lcd.print(data1);
    lcd.setCursor(0,1);
    lcd.print("Val2 : ");
    lcd.print(data2);
    writeSD();
  }
  delay(200);
}

void checkSD()
{
  lcd.print("check SD card");
  if (!SD.begin(chipSelect)) {
    lcd.setCursor(0,2);
    lcd.print("Card failed");
    // don't do anything more:
    return;
  }
  lcd.setCursor(0,2);
  lcd.print("SD card OK");
  present=1;
  delay(2000);
}
void writeSD()
{
  myFile = SD.open("datalog.csv", FILE_WRITE);
  if(myFile)
  {
    myFile.print(i);
    myFile.print(",");
    myFile.print(data1);
    myFile.print(",");
    myFile.println(data2);
    myFile.close();
    
  }
}

