#include <Adafruit_NeoPixel.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Adafruit_GFX.h>

#define PIN D2            
#define NUMPIXELS 86
int valeurLuminosite, pourcentageLuminosite, heures, minutes, chiffre1, chiffre2, chiffre3, chiffre4, lastMinute;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


const char *ssid = "Livebox-E8B2";
const char *password = "QoDXChmz4zXSiuKwkU";


WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

void setup() {
  Serial.begin(9600);

  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connexion au WiFi...");
  }
  Serial.println("Connecté au WiFi");

  
  strip.begin();
  strip.show();

  
  timeClient.begin();
  timeClient.setTimeOffset(3600);
}

void loop() {
  
  timeClient.update();

  heures = timeClient.getHours();
  minutes = timeClient.getMinutes();
  lastMinute = chiffre4;
  chiffre1 = heures / 10;
  chiffre2 = heures % 10;

  chiffre3 = minutes / 10;
  chiffre4 = minutes % 10;

  pourcentageLuminosite = 60;
  valeurLuminosite = map(pourcentageLuminosite, 0, 100, 0, 255);
  strip.setBrightness(valeurLuminosite);

  if(lastMinute != chiffre4)
  {
    strip.clear();
  }

  
  for (int i = 42; i < 44; i++) {
    strip.setPixelColor(i, strip.Color(255, 255, 255));
  }

  choix(1, chiffre1);
  choix(2, chiffre2);
  choix(3, chiffre3);
  choix(4, chiffre4);

  strip.show();

  delay(1000);
}

void choix(int section, int number) {
  int x;
  
  switch (section) {
    case 1:
      x = 0;
    break;

    case 2:
      x = 21;
    break;

    case 3:
      x = 44;
    break;

    case 4:
      x = 65;
    break;
  }

  switch (number) {
    case 0:
      zero(x);
    break;

    case 1:
      un(x);
    break;

    case 2:
      deux(x);
    break;

    case 3:
      trois(x);
    break;

    case 4:
      quatre(x);
    break;

    case 5:
      cinque(x);
    break;

    case 6:
      six(x);
    break;

    case 7:
      sept(x);
    break;

    case 8:
      huit(x);
    break;

    case 9:
      neuf(x);
    break;
  }
}

void zero(int x)
{
  for (int i = x+3; i < x+21; i++)
  {
    strip.setPixelColor(i, strip.Color(0, 0, 255));  
  }
  strip.show();
}



void un(int x)
{
  for (int i = x; i < x+21; i++)
  {
    if( ((i >= x+3) && (i < x+6)) || ((i >= x+18) && (i < x+21)) )
    {
      strip.setPixelColor(i, strip.Color(0, 0, 255));  
    }
  }
  strip.show();
}



void deux(int x)
{
  for (int i = x; i < x+21; i++)
  {
    if( ((i >= x+0) && (i < x+9)) || ((i >= x+12) && (i < x+18)) )
    {
      strip.setPixelColor(i, strip.Color(0, 0, 255));  
    }
  }
  strip.show();
}



void trois(int x)
{
  for (int i = x; i < x+21; i++)
  {
    if( ((i >= x+0) && (i < x+9)) || ((i >= x+15) && (i < x+21)) )
    {
      strip.setPixelColor(i, strip.Color(0, 0, 255));  
    }
  }
  strip.show();
}



void quatre(int x)
{
  for (int i = x; i < x+21; i++)
  {
    if( ((i >= x+0) && (i < x+6)) || ((i >= x+9) && (i < x+12)) || ( (i >= x+18) && (i < x+21) ))
    {
      strip.setPixelColor(i, strip.Color(0, 0, 255));  
    }
  }
  strip.show();
}



void cinque(int x)
{
  for (int i = x; i < x+21; i++)
  {
    if( ((i >= x+0) && (i < x+3)) || ((i >= x+6) && (i < x+12)) || ( (i >= x+15) && (i < x+21) ))
    {
      strip.setPixelColor(i, strip.Color(0, 0, 255));  
    }
  }
  strip.show();
}



void six(int x)
{
  for (int i = x; i < x+21; i++)
  {
    if( ((i >= x+0) && (i < x+3)) || ((i >= x+6) && (i < x+12)) || ( (i >= x+12) && (i < x+21) ))
    {
      strip.setPixelColor(i, strip.Color(0, 0, 255));  
    }
  }
  strip.show();
}



void sept(int x)
{
  for (int i = x; i < x+21; i++)
  {
    if( ((i >= x+3) && (i < x+9)) || ((i >= x+18) && (i < x+21)) )
    {
      strip.setPixelColor(i, strip.Color(0, 0, 255));  
    }
  }
  strip.show();
}



void huit(int x)
{
  for (int i = x; i < x+21; i++)
  {
    strip.setPixelColor(i, strip.Color(0, 0, 255));  
  }
  strip.show();
}



void neuf(int x)
{
  for (int i = x; i < x+21; i++)
  {
    if( ((i >= x) && (i < x+12)) || ((i >= x+15) && (i < x+21)) )
    {
      strip.setPixelColor(i, strip.Color(0, 0, 255));  
    }
  }
  strip.show();
}







