
#include <Arduino.h>
#include <arduino_homekit_server.h>
#include "wifi_info.h"
#include "DHT.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT_U.h>

#define SCREEN_WIDTH 128 // OLED display width
#define SCREEN_HEIGHT 64 // OLED display height

#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define pinDHT 0         // Pin where the DHT22 is connected
#define DHTTYPE DHT22    // DHT 22 (AM2302)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT dht(pinDHT, DHTTYPE);

float t;
float h;

#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);

void setup() 
{
	Serial.begin(115200);
  //homekit_storage_reset(); 

  dht.begin();

  // Initialize the OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32 display
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.display();
  delay(2000); // Pause for 2 seconds
  display.clearDisplay();

	wifi_connect(); // in wifi_info.h
  
	my_homekit_setup();
}

void drawTemperature(int t) {
  display.setTextSize(3); // Taille du texte pour la valeur
  display.setTextColor(SSD1306_WHITE);

  // Convertir la température en chaîne et la dessiner
  String tempString = String(t);
  int16_t x, y;
  uint16_t w, h;
  display.getTextBounds(tempString, 0, 0, &x, &y, &w, &h);
  x = (SCREEN_WIDTH / 4 - w / 2); // Centrez la valeur sur la moitié gauche
  y = (SCREEN_HEIGHT / 2 - h / 2);
  display.setCursor(x, y);
  display.print(tempString);

  display.setTextSize(1.25); // Taille du texte plus petite pour l'unité
  display.setCursor(x + w, y + h / 4); // Positionnez l'unité à côté de la valeur
  display.print((char)247); // Symbole degré
  display.println("C");
}

void drawHumidity(int h) {
  display.setTextSize(3); // Taille du texte pour la valeur
  display.setTextColor(SSD1306_WHITE);

  // Convertir l'humidité en chaîne et la dessiner
  String humString = String(h);
  int16_t x2, y2;
  uint16_t w2, h2;
  display.getTextBounds(humString, 0, 0, &x2, &y2, &w2, &h2);
  x2 = (SCREEN_WIDTH * 3 / 4 - w2 / 2); // Centrez la valeur sur la moitié droite
  y2 = (SCREEN_HEIGHT / 2 - h2 / 2);
  display.setCursor(x2, y2);
  display.print(humString);

  display.setTextSize(1.25); // Taille du texte plus petite pour l'unité
  display.setCursor(x2 + w2, y2 + h2 / 4); // Positionnez l'unité à côté de la valeur
  display.println("%");
}

void loop()   
{
	my_homekit_loop();
	delay(10);
}

//==============================
// Homekit setup and loop
//==============================

// access your homekit characteristics defined in my_accessory.c
extern "C" homekit_server_config_t config;
extern "C" homekit_characteristic_t cha_current_temperature;
extern "C" homekit_characteristic_t cha_humidity;
extern "C" homekit_characteristic_t cha_switch_on;

static uint32_t next_heap_millis = 0;
static uint32_t next_report_millis = 0;

void cha_switch_on_setter(const homekit_value_t value) {
	bool on = value.bool_value;
	cha_switch_on.value.bool_value = on;	//sync the value
	LOG_D("Switch: %s", on ? "ON" : "OFF");
	
  if(on == true)
  {
    display.ssd1306_command(SSD1306_DISPLAYON);
  }
  else
  {
    display.ssd1306_command(SSD1306_DISPLAYOFF);
  }


}

void my_homekit_setup() 
{
  cha_switch_on.setter = cha_switch_on_setter;
	arduino_homekit_setup(&config);
}

void my_homekit_loop() 
{
	arduino_homekit_loop();
	const uint32_t t = millis();
	if (t > next_report_millis) 
  {
		next_report_millis = t + 10 * 1000;
		my_homekit_report();
	}
	if (t > next_heap_millis) 
  {
		next_heap_millis = t + 5 * 1000;
		LOG_D("Free heap: %d, HomeKit clients: %d",
		ESP.getFreeHeap(), arduino_homekit_connected_clients_count());

	}
}

void my_homekit_report() 
{
  h = dht.readHumidity();
  t = dht.readTemperature();
   // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  display.clearDisplay();

  // Display temperature on the left
  drawTemperature((int)t); // Convert to int

  // Display humidity on the right
  drawHumidity((int)h); // Convert to int

  display.display(); 

	float temperature_value = t;
	cha_current_temperature.value.float_value = temperature_value;
  LOG_D("Current temperature: %.1f", temperature_value);


  float hymidity_value = h;
	cha_humidity.value.float_value = hymidity_value;
  LOG_D("Current humidity: %.1f", hymidity_value);

	homekit_characteristic_notify(&cha_current_temperature, cha_current_temperature.value);
  homekit_characteristic_notify(&cha_humidity, cha_humidity.value);
}