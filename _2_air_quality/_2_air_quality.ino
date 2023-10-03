
#include <Arduino.h>
#include <arduino_homekit_server.h>
#include "wifi_info.h"
#include "DHT.h"
#define pinDHT 2    
#define typeDeDHT DHT22  

DHT dht(pinDHT, typeDeDHT);

float t;
float h;

#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);

void setup() 
{
	Serial.begin(115200);
  //homekit_storage_reset(); 
	wifi_connect(); // in wifi_info.h
  dht.begin();
	my_homekit_setup();
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

static uint32_t next_heap_millis = 0;
static uint32_t next_report_millis = 0;

void my_homekit_setup() 
{
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

	float temperature_value = t;
	cha_current_temperature.value.float_value = temperature_value;
  LOG_D("Current temperature: %.1f", temperature_value);


  float hymidity_value = h;
	cha_humidity.value.float_value = hymidity_value;
  LOG_D("Current humidity: %.1f", hymidity_value);

	homekit_characteristic_notify(&cha_current_temperature, cha_current_temperature.value);
  homekit_characteristic_notify(&cha_humidity, cha_humidity.value);
}