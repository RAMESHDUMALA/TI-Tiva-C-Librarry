#include <EEPROM.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;       // hard-wired for UNO shields anyway.
#include <TouchScreen.h>
#include <GSM.h>

#define YP A1  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 7   // can be a digital pin
#define XP 6   // can be a digital pin

// Calibrate values
#define TS_MINX 125
#define TS_MINY 85
#define TS_MAXX 965
#define TS_MAXY 905

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
GSM gsmAccess;
GSM_SMS sms;

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define MINPRESSURE 10
#define MAXPRESSURE 1000
#define PINNUMBER ""


String Key[3][4] = {
  { "7", "8", "9", "/" },
  { "4", "5", "6", "C" },
  { "1", "2", "3", "K" }
};

String N1, N2, ShowSC, opt;
bool updata = false;
float answers = -1;
int num = 0;
int money = 0;
int paymoney = 0;
char input[12];
byte value;
int count = 0;


TSPoint waitTouch() {
  TSPoint p;
  do {
    p = ts.getPoint();
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
  } while ((p.z < MINPRESSURE ) || (p.z > MAXPRESSURE));
  p.x = map(p.x, TS_MINX, TS_MAXX, 0, 320);
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, 240);;
  return p;
}


void numberkey()
{
  tft.setRotation(2);
  tft.fillScreen(BLACK);
  tft.fillRect(0, 80, 240, 180, WHITE);
  tft.drawFastHLine(0, 80, 240, BLACK);
  tft.drawFastHLine(0, 140, 240, BLACK);
  tft.drawFastHLine(0, 200, 240, BLACK);
  tft.drawFastHLine(0, 260, 240, BLACK);


  tft.drawFastVLine(0, 80, 240, BLACK);
  tft.drawFastVLine(60, 80, 240, BLACK);
  tft.drawFastVLine(120, 80, 240, BLACK);
  tft.drawFastVLine(180, 80, 240, BLACK);


  tft.setCursor(22 + (60 * 0), 100 + (60 * 0));
  tft.setTextSize(3);
  tft.setTextColor(BLACK);
  tft.println(1);
  tft.setCursor(22 + (60 * 1), 100 + (60 * 0));
  tft.setTextSize(3);
  tft.setTextColor(BLACK);
  tft.println(2);
  tft.setCursor(22 + (60 * 2), 100 + (60 * 0));
  tft.setTextSize(3);
  tft.setTextColor(BLACK);
  tft.println(3);
  tft.setCursor(22 + (60 * 3), 100 + (60 * 0));
  tft.setTextSize(3);
  tft.setTextColor(BLACK);
  tft.println("K");


  tft.setCursor(22 + (60 * 0), 100 + (60 * 1));
  tft.setTextSize(3);
  tft.setTextColor(BLACK);
  tft.println(4);
  tft.setCursor(22 + (60 * 1), 100 + (60 * 1));
  tft.setTextSize(3);
  tft.setTextColor(BLACK);
  tft.println(5);
  tft.setCursor(22 + (60 * 2), 100 + (60 * 1));
  tft.setTextSize(3);
  tft.setTextColor(BLACK);
  tft.println(6);
  tft.setCursor(22 + (60 * 3), 100 + (60 * 1));
  tft.setTextSize(3);
  tft.setTextColor(BLACK);
  tft.println("C");

  tft.setCursor(22 + (60 * 0), 100 + (60 * 2));
  tft.setTextSize(3);
  tft.setTextColor(BLACK);
  tft.println(7);
  tft.setCursor(22 + (60 * 1), 100 + (60 * 2));
  tft.setTextSize(3);
  tft.setTextColor(BLACK);
  tft.println(8);
  tft.setCursor(22 + (60 * 2), 100 + (60 * 2));
  tft.setTextSize(3);
  tft.setTextColor(BLACK);
  tft.println(9);

  tft.setCursor(22 + (60 * 3), 100 + (60 * 2));
  tft.setTextSize(3);
  tft.setTextColor(BLACK);
  tft.println("0");
  while (1) {
    TSPoint p = waitTouch();
    //Serial.println(p.x);
    updata = false;
    for (int i1 = 0; i1 < 3; i1++) {
      for (int i2 = 0; i2 < 4; i2++) {
        if ((p.x >= 240 - ((i1 + 1) * 60) + 1 && p.x <= 240 - (i1 * 60) - 1) && (p.y >= (i2 * 60) + 1 && p.y <= ((i2 + 1) * 60) - 1)) {
          if ((i1 <= 2 && i2 <= 2) || (i1 == 3 && i2 == 1)) {
            if (opt == 0) {
              if (answers != -1) answers = -1;
              N1 = N1 + Key[i1][i2];
              ShowSC = N1;
              money = ShowSC.toInt();

            } else {
              N2 = N2 + Key[i1][i2];
              ShowSC = opt + N2;
            }
          }
          else {
            if (Key[i1][i2] == "C") {
              N1 = N2 = "";
              opt = "";
              answers = 0;
              ShowSC = N1;
              money = 0;
            }
            else if (Key[i1][i2] == "/") {
              N1 = N1 + 0;
              ShowSC = N1;
            }
            else if (Key[i1][i2] == "K")
            {
              N1 = N2 = "";
              opt = "";
              answers = 0;
              ShowSC = N1;
              num = 1;
              break;
            }
          }
          updata = true;
        }
      }
    }

    if (updata) {
      tft.fillRect(0, 0, 240, 80, BLACK);
      tft.setCursor(10, 10);
      tft.setTextSize(3);
      tft.setTextColor(WHITE);
      tft.println(ShowSC);
    }
    delay(300);
    if (num)
    {
      num = 0;
      break;
    }
  }



}
void menu()
{
  tft.setRotation(2);
  tft.fillScreen(BLACK);
  tft.setTextSize(2);
  tft.setCursor(15, 10);
  tft.setTextColor(WHITE);
  tft.println("AUTOMATED CANTEEN");
  tft.fillRect(50, 50, 80, 40, RED);
  tft.setCursor(65, 60);
  tft.setTextColor(WHITE);
  tft.println("MENU");

  tft.setCursor(0, 100);
  tft.setTextColor(WHITE);
  tft.println("1. IDLY");
  //  tft.fillRect(150, 90, 80, 30, GREEN);
  //  tft.setCursor(150, 100);
  //  tft.println("SELECT");

  tft.setCursor(0, 150);
  tft.setTextColor(WHITE);
  tft.println("2. WATER");
  //  tft.fillRect(150, 140, 80, 30, GREEN);
  //  tft.setCursor(150, 150);
  //  tft.println("SELECT");
  tft.fillRect(150, 200, 80, 30, GREEN);
  tft.setCursor(170, 210);
  tft.println("PAY");

}


void cardread()
{
  while (1)
  {
    if (Serial.available())
    {
      count = 0;
      while (Serial.available() && count < 12)         // Read 12 characters and store them in input array
      {
        input[count] = Serial.read();
        count++;
        delay(5);
      }
      //Serial.print(input);                             // Print RFID tag number
      //
      if ((input[0] ^ input[2] ^ input[4] ^ input[6] ^ input[8] == input[10]) &&
          (input[1] ^ input[3] ^ input[5] ^ input[7] ^ input[9] == input[11]))
      {
        break;
      }
    }

  }

}

void menurepeat()
{

  TSPoint p;
  menu();
  p = waitTouch();

  if (p.x >= 200 && p.x <= 230 && p.z > 175)
  {
    l1:numberkey();
    if(money!=0)
    {
    tft.fillScreen(BLACK);
    tft.setCursor(15, 10);
    tft.setTextColor(WHITE);
    tft.println("TOTAL AMOUNT");
    tft.setCursor(50, 100);
    paymoney = money;
    tft.println(paymoney);
    tft.setCursor(20, 150);
    tft.println("Card Plase");
    cardread();
    }
    else
    goto l1;
  }



}
void processing()
{
  menurepeat();
  tft.fillScreen(BLACK);
  tft.setCursor(100, 100);
  tft.setTextColor(WHITE);
  tft.println("PIN");
  delay(800);
  money = 0;
  numberkey();
  tft.fillScreen(BLACK);
  tft.setCursor(10, 100);
  tft.setTextColor(WHITE);
  tft.println("Processing");
  delay(800);
}
void setup() {

  Serial.begin(9600);
  tft.reset();
  tft.begin(0x154);
  boolean notConnected = true;
  tft.setRotation(2);
  tft.fillScreen(BLACK);
  tft.setTextSize(2);
  tft.setCursor(15, 50);
  tft.setTextColor(WHITE);
  tft.println("AUTOMATED CANTEEN");
  tft.setCursor(15, 80);
  tft.println("GSM CONNECTING");
  while (notConnected) {
    if (gsmAccess.begin(PINNUMBER) == GSM_READY) {
      notConnected = false;
      tft.setCursor(15, 110);
      tft.println("GSM CONNECTED");
      delay(1000);
    } else {
      Serial.println("Not connected");
      tft.setCursor(15, 110);
      tft.println("GSM CONNECT FAIL");
      delay(1000);
    }
  }
  processing();

}

void loop() {
  String stringOne =  "Automated Canteen Balance\r\n";

  if (strncmp(input, "14502712", 8) == 0 && money == 111)
  {
    value = EEPROM.read(1);
    if (value >= paymoney)
    {
      value = value - paymoney;
      //Serial.println(value);
      EEPROM.write(1, value);
      paymoney = 0;
      money = 0;
      stringOne = stringOne + value;
      sms.beginSMS("9010573750");
      sms.print(stringOne);
      sms.endSMS();
      tft.setCursor(30, 50);
      tft.fillScreen(BLACK);
      tft.println("Thank You");
      tft.setCursor(10, 80);
      tft.println("Balance");
      tft.setCursor(50, 120);
      tft.println(value);
      delay(1000);
      processing();
    }
    else
    {
      tft.fillScreen(BLACK);
      tft.setCursor(10, 100);
      tft.setTextColor(WHITE);
      tft.println("Sorry No Balance");
      paymoney = 0;
      money = 0;
      delay(1000);
      processing();
    }
  }

 if (strncmp(input, "14503595", 8) == 0 && money == 222)
  {
    value = EEPROM.read(2);
    if (value >= paymoney)
    {
      value = value - paymoney;
      //Serial.println(value);
      EEPROM.write(2, value);
      paymoney = 0;
      money = 0;
      stringOne = stringOne + value;
      sms.beginSMS("9966120252");
      sms.print(stringOne);
      sms.endSMS();
      tft.setCursor(30, 50);
      tft.fillScreen(BLACK);
      tft.println("Thank You");
      tft.setCursor(10, 80);
      tft.println("Balance");
      tft.setCursor(50, 120);
      tft.println(value);
      delay(1000);
      processing();
    }
    else
    {
      tft.fillScreen(BLACK);
      tft.setCursor(10, 100);
      tft.setTextColor(WHITE);
      tft.println("Sorry No Balance");
      paymoney = 0;
      money = 0;
      delay(1000);
      processing();
    }
  }
  
  else if (input != NULL) {
    tft.fillScreen(BLACK);
    tft.setCursor(10, 100);
    tft.setTextSize(2);
    tft.setTextColor(WHITE);
    money = 0;
    paymoney = 0;
    tft.println("Un Authorised Card");
    delay(1000);
    processing();
  }
}


