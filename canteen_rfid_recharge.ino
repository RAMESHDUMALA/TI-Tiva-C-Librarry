
#include <EEPROM.h>
int accno = 1;
int balance = 200;//maximum 200 rubees

void setup()
{
  EEPROM.begin(512);
  EEPROM.write(accno, balance);
  EEPROM.commit();
  delay(100);

}

void loop()
{

}
