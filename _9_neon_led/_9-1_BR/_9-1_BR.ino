
#include <Arduino.h>
#include <arduino_homekit_server.h>
#include "wifi_info.h"

#include <FastLED.h>

#define LED_PIN     D1
#define NUM_LEDS    300


bool statut_white_static;
bool statut_white_motion;
bool statut_color_static;
bool statut_color_motion;
bool initialisation = true;

CRGB leds[NUM_LEDS];

#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);

void setup() 
{
  
	Serial.begin(115200);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
	wifi_connect(); 
	//homekit_storage_reset(); // to remove the previous HomeKit pairing storage when you first run this new HomeKit example
	my_homekit_setup();
}
unsigned long previousMillis = 0;

// Interval at which to blink (milliseconds)
const long interval = 1000;
void loop() 
{
	my_homekit_loop();
	delay(10);
  if((statut_white_static == false) && (statut_white_motion == false) && (statut_color_static == false) && (statut_color_motion == false))
  {
    off();
  }
  
  unsigned long currentMillis = millis(); 
  if (currentMillis - previousMillis >= interval) 
  {
    previousMillis = currentMillis;
    if (statut_color_motion) 
    {
      colorMotion();
    }
    
    if (statut_white_motion) 
    {
      whiteMotion();
    }
  }
}

//==============================
// HomeKit setup and loop
//==============================

// access your HomeKit characteristics defined in my_accessory.c
extern "C" homekit_server_config_t config;
extern "C" homekit_characteristic_t cha_switch_white_static;
extern "C" homekit_characteristic_t cha_switch_white_motion;
extern "C" homekit_characteristic_t cha_switch_color_static;
extern "C" homekit_characteristic_t cha_switch_color_motion;

static uint32_t next_heap_millis = 0;



void cha_switch_white_static_setter(const homekit_value_t value) 
{
  statut_white_static = value.bool_value;
  cha_switch_white_static.value.bool_value = statut_white_static;  //sync de la valeur

  if(statut_white_static == true)
  {
    if(initialisation == true)
    {
       initalisation();
       initialisation = false;
    }
      whiteStatic();
  }
   if(statut_white_static == false)
  {
    initialisation = true;
  }
  
}

void cha_switch_white_motion_setter(const homekit_value_t value) 
{
  statut_white_motion = value.bool_value;
  cha_switch_white_motion.value.bool_value = statut_white_motion;  //sync de la valeur

  if(statut_white_motion == true)
  {
    if(initialisation == true)
    {
       initalisation();
       initialisation = false;
    }
  }
  if(statut_white_motion == false)
  {
    initialisation = true;
  }
  
}





void cha_switch_color_static_setter(const homekit_value_t value) 
{
  statut_color_static = value.bool_value;
  cha_switch_color_static.value.bool_value = statut_color_static;  //sync de la valeur

  if(statut_color_static == true)
  {
    if(initialisation == true)
    {
       initalisation();
       initialisation = false;
    }
      colorStatic();
  }
  if(statut_color_static == false)
  {
    initialisation = true;
  }
 
}

void cha_switch_color_motion_setter(const homekit_value_t value) 
{
  statut_color_motion = value.bool_value;
  cha_switch_color_motion.value.bool_value = statut_color_motion;  //sync de la valeur

  if(statut_color_motion == true)
  {
    if(initialisation == true)
    {
       initalisation();
       initialisation = false;
    }      
  }
  if(statut_color_motion == false)
  {
    initialisation = true;
  }
  
}






void my_homekit_setup() 
{


	cha_switch_white_static.setter = cha_switch_white_static_setter;
  cha_switch_white_motion.setter = cha_switch_white_motion_setter;
  cha_switch_color_static.setter = cha_switch_color_static_setter;
  cha_switch_color_motion.setter = cha_switch_color_motion_setter;
	arduino_homekit_setup(&config);

}

void my_homekit_loop() 
{
	arduino_homekit_loop();
	const uint32_t t = millis();
	if (t > next_heap_millis) 
	{
		// show heap info every 5 seconds
		next_heap_millis = t + 5 * 1000;
		LOG_D("Free heap: %d, HomeKit clients: %d", ESP.getFreeHeap(), arduino_homekit_connected_clients_count());

	}
}



void initalisation()
{
  for (int i = 0; i < NUM_LEDS; i++) 
   {
        leds[i] = CRGB::White;
        FastLED.show();
        delay(10);
    }
}


void whiteMotion()
{
  for (int i = 0; i < 256; i++) 
  {
    for (int j = 0; j < NUM_LEDS; j++) 
    {
      leds[j] = CHSV(0, 0, i + j);
    }
    FastLED.show();
    delay(10);
  }
}

void whiteStatic()
{
  for (int i = 0; i < NUM_LEDS; i++) 
  {

    leds[i] = CRGB::White;
  }
   FastLED.show();
}


void colorMotion()
{
   for (int i = 0; i < 256; i++) 
   {
        for (int j = 0; j < NUM_LEDS; j++) 
        {
            leds[j] = CHSV((i + j * 128 / NUM_LEDS) % 256 + 128, 255, 255);
        }
        FastLED.show();
        delay(10);
    }
}

void colorStatic()
{
  for (int i = 0; i < NUM_LEDS; i++) 
  {
        leds[i] = CHSV((i * 128 / NUM_LEDS) + 128, 255, 255);
   }
   FastLED.show();
}


void off()
{
  for (int i = 0; i < NUM_LEDS; i++) 
  {

    leds[i] = CRGB::Black;
  }
   FastLED.show();
}
