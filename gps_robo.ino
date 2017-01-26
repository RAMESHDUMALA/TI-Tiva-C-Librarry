#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>


SoftwareSerial mySerial(D6, D7); // 10 RX / 11 TX

char ch;
int i = 0;
char buf[65];
char temp[50];
char temp_lat[25];
char temp_lon[25];
float latitude, longitude;
int pirPin = D1;    //the digital pin connected to the PIR sensor's output

const char* ssid     = "WiSe"; //to change
const char* password = "ramesh2020";///to change
const char* host     = "api.thingspeak.com"; // Your domain

void setup()
{
  Serial.begin(115200);
  mySerial.begin(9600);
  pinMode(pirPin, INPUT);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: " + WiFi.localIP());

}

void loop()
{

  if (mySerial.available())
  {
    ch = mySerial.read();
    //Serial.write(ch);
    buf[i] = ch;
    i++;
    if (ch == '\n')
    {
      if (buf[0] == '$' && buf[1] == 'G' && buf[2] == 'P' && buf[3] == 'G' && buf[4] == 'G' && buf[5] == 'A' && buf[6] == ',')
      {
        //Serial.println(buf);
        //Serial.println("matched");
        for (int k = 0; k <= 23; k++)

        {
          temp[k] = buf[k + 18];
        }
        int k = 0;
        while (temp[k] != ',')
        {
          temp_lat[k] = temp[k];
          k++;
        }
        k = k + 3;
        int m = 0;
        while (temp[k] != ',')
        {
          temp_lon[m] = temp[k];
          k++;
          m++;
        }
      latitude = (float)atof(temp_lat);
      longitude = (float)atof(temp_lon);        
      Serial.println(latitude);
        Serial.println(longitude);
      
        WiFiClient client;
        const int httpPort = 80;
        if (!client.connect(host, httpPort)) {
          Serial.println("connection failed");
          return;
        }

        if (digitalRead(pirPin) == HIGH) {
       
          client.print(String("GET ") + "https://api.thingspeak.com/update?api_key=R70KOH4UDOUVSLX9&field1=" +latitude+"&field2="+longitude+" HTTP/1.1\r\n" +
                       "Host: " + host + "\r\n" +
                       "Connection: keep-alive\r\n\r\n");
          delay(1500);

        }



        //Serial.println(temp_lat);
        //Serial.println(temp_lon);
        memset(buf, 0, 65);
        delay(200);
        i = 0;

      }
      else
      {
        //Serial.println("not matched");
        memset(buf, 0, 65);
        delay(200);
        i = 0;
      }
    }
  }


}
