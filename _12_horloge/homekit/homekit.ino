

#include <Arduino.h>
#include <arduino_homekit_server.h>
#include <Adafruit_NeoPixel.h>
#include "wifi_info.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Adafruit_GFX.h>

#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);

#define NEOPIN          D2
#define NUMPIXELS       86

int heures, minutes, chiffre1, chiffre2, chiffre3, chiffre4, lastMinute;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIN, NEO_GRB + NEO_KHZ800);

bool received_sat = false;
bool received_hue = false;

bool is_on = false;
float current_brightness =  100.0;
float current_sat = 0.0;
float current_hue = 0.0;
int rgb_colors[3];


WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
void setup() 
{
  
  timeClient.begin();
  timeClient.setTimeOffset(3600);
	Serial.begin(115200);
	wifi_connect(); 

  pixels.begin(); 
  pixels.show();
  delay(1000);
	rgb_colors[0] = 255;
  rgb_colors[1] = 255;
  rgb_colors[2] = 255;
	my_homekit_setup();
}

void loop() {
	my_homekit_loop();
  

  
	delay(10);
}







extern "C" homekit_server_config_t accessory_config;
extern "C" homekit_characteristic_t cha_on;
extern "C" homekit_characteristic_t cha_bright;
extern "C" homekit_characteristic_t cha_sat;
extern "C" homekit_characteristic_t cha_hue;

static uint32_t next_heap_millis = 0;

void my_homekit_setup() {

  cha_on.setter = set_on;
  cha_bright.setter = set_bright;
  cha_sat.setter = set_sat;
  cha_hue.setter = set_hue;
  
	arduino_homekit_setup(&accessory_config);

}

void my_homekit_loop() {
	arduino_homekit_loop();
	const uint32_t t = millis();
    timeClient.update();

  heures = timeClient.getHours();
  minutes = timeClient.getMinutes();
  lastMinute = chiffre4;
  chiffre1 = heures / 10;
  chiffre2 = heures % 10;

  chiffre3 = minutes / 10;
  chiffre4 = minutes % 10;



  if(lastMinute != chiffre4)
  {
    pixels.clear();
  }

	if (t > next_heap_millis) {
		
		next_heap_millis = t + 5 * 1000;
		LOG_D("Free heap: %d, HomeKit clients: %d",
				ESP.getFreeHeap(), arduino_homekit_connected_clients_count());
        updateColor();

	}
}

void set_on(const homekit_value_t v) {
    bool on = v.bool_value;
    cha_on.value.bool_value = on; 

    if(on) {
        is_on = true;
        Serial.println("On");
    } else  {
        is_on = false;
        Serial.println("Off");
    }

    updateColor();
}

void set_hue(const homekit_value_t v) {
    Serial.println("set_hue");
    float hue = v.float_value;
    cha_hue.value.float_value = hue; 

    current_hue = hue;
    received_hue = true;
    
    updateColor();
}

void set_sat(const homekit_value_t v) {
    Serial.println("set_sat");
    float sat = v.float_value;
    cha_sat.value.float_value = sat; 

    current_sat = sat;
    received_sat = true;
    
    updateColor();

}

void set_bright(const homekit_value_t v) {
    Serial.println("set_bright");
    int bright = v.int_value;
    cha_bright.value.int_value = bright; 

    current_brightness = bright;

    updateColor();
}

void updateColor()
{
  if(is_on)
  {
    
    

      pixels.show();
      if(received_hue && received_sat)
      {
        HSV2RGB(current_hue, current_sat, current_brightness);
        received_hue = false;
        received_sat = false;
      }
      
      int b = map(current_brightness,0, 100,75, 255);
      Serial.println(b);
  
      pixels.setBrightness(b);
      choix(1, chiffre1);
      choix(2, chiffre2);
      choix(3, chiffre3);
      choix(4, chiffre4);
      pixels.show();

    }
  else if(!is_on) 
  {
      Serial.println("is_on == false");
      pixels.setBrightness(0);
      for(int i = 0; i < NUMPIXELS; i++)
      {
        pixels.setPixelColor(i, pixels.Color(0,0,0));
      }
      pixels.show();
  }
}

void HSV2RGB(float h,float s,float v) {

  int i;
  float m, n, f;

  s/=100;
  v/=100;

  if(s==0){
    rgb_colors[0]=rgb_colors[1]=rgb_colors[2]=round(v*255);
    return;
  }

  h/=60;
  i=floor(h);
  f=h-i;

  if(!(i&1)){
    f=1-f;
  }

  m=v*(1-s);
  n=v*(1-s*f);

  switch (i) {

    case 0: case 6:
      rgb_colors[0]=round(v*255);
      rgb_colors[1]=round(n*255);
      rgb_colors[2]=round(m*255);
    break;

    case 1:
      rgb_colors[0]=round(n*255);
      rgb_colors[1]=round(v*255);
      rgb_colors[2]=round(m*255);
    break;

    case 2:
      rgb_colors[0]=round(m*255);
      rgb_colors[1]=round(v*255);
      rgb_colors[2]=round(n*255);
    break;

    case 3:
      rgb_colors[0]=round(m*255);
      rgb_colors[1]=round(n*255);
      rgb_colors[2]=round(v*255);
    break;

    case 4:
      rgb_colors[0]=round(n*255);
      rgb_colors[1]=round(m*255);
      rgb_colors[2]=round(v*255);
    break;

    case 5:
      rgb_colors[0]=round(v*255);
      rgb_colors[1]=round(m*255);
      rgb_colors[2]=round(n*255);
    break;
  }
}

void choix(int section, int number) {
  int x;
  for (int i = 42; i < 44; i++) {
    pixels.setPixelColor(i, pixels.Color(rgb_colors[0],rgb_colors[1],rgb_colors[2]));
  }
  pixels.show();
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
    pixels.setPixelColor(i, pixels.Color(rgb_colors[0],rgb_colors[1],rgb_colors[2]));
  }
  pixels.show();
}



void un(int x)
{
  for (int i = x; i < x+21; i++)
  {
    if( ((i >= x+3) && (i < x+6)) || ((i >= x+18) && (i < x+21)) )
    {
      pixels.setPixelColor(i, pixels.Color(rgb_colors[0],rgb_colors[1],rgb_colors[2])); 
    }
  }
  pixels.show();
}



void deux(int x)
{
  for (int i = x; i < x+21; i++)
  {
    if( ((i >= x+0) && (i < x+9)) || ((i >= x+12) && (i < x+18)) )
    {
      pixels.setPixelColor(i, pixels.Color(rgb_colors[0],rgb_colors[1],rgb_colors[2]));
    }
  }
  pixels.show();
}



void trois(int x)
{
  for (int i = x; i < x+21; i++)
  {
    if( ((i >= x+0) && (i < x+9)) || ((i >= x+15) && (i < x+21)) )
    {
      pixels.setPixelColor(i, pixels.Color(rgb_colors[0],rgb_colors[1],rgb_colors[2]));
    }
  }
  pixels.show();
}



void quatre(int x)
{
  for (int i = x; i < x+21; i++)
  {
    if( ((i >= x+0) && (i < x+6)) || ((i >= x+9) && (i < x+12)) || ( (i >= x+18) && (i < x+21) ))
    {
      pixels.setPixelColor(i, pixels.Color(rgb_colors[0],rgb_colors[1],rgb_colors[2]));
    }
  }
  pixels.show();
}



void cinque(int x)
{
  for (int i = x; i < x+21; i++)
  {
    if( ((i >= x+0) && (i < x+3)) || ((i >= x+6) && (i < x+12)) || ( (i >= x+15) && (i < x+21) ))
    {
      pixels.setPixelColor(i, pixels.Color(rgb_colors[0],rgb_colors[1],rgb_colors[2]));
    }
  }
  pixels.show();
}



void six(int x)
{
  for (int i = x; i < x+21; i++)
  {
    if( ((i >= x+0) && (i < x+3)) || ((i >= x+6) && (i < x+12)) || ( (i >= x+12) && (i < x+21) ))
    {
      pixels.setPixelColor(i, pixels.Color(rgb_colors[0],rgb_colors[1],rgb_colors[2]));
    }
  }
  pixels.show();
}



void sept(int x)
{
  for (int i = x; i < x+21; i++)
  {
    if( ((i >= x+3) && (i < x+9)) || ((i >= x+18) && (i < x+21)) )
    {
      pixels.setPixelColor(i, pixels.Color(rgb_colors[0],rgb_colors[1],rgb_colors[2]));
    }
  }
  pixels.show();
}



void huit(int x)
{
  for (int i = x; i < x+21; i++)
  {
    pixels.setPixelColor(i, pixels.Color(rgb_colors[0],rgb_colors[1],rgb_colors[2]));
  }
  pixels.show();
}



void neuf(int x)
{
  for (int i = x; i < x+21; i++)
  {
    if( ((i >= x) && (i < x+12)) || ((i >= x+15) && (i < x+21)) )
    {
      pixels.setPixelColor(i, pixels.Color(rgb_colors[0],rgb_colors[1],rgb_colors[2]));
    }
  }
  pixels.show();
}
