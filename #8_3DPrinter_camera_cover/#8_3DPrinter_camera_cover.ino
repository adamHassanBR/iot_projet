

#include <Arduino.h>
#include <arduino_homekit_server.h>
#include "wifi_info.h"
#include <Servo.h>
Servo monServo;
bool etat;
#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);

void setup() 
{
  Serial.begin(115200);
  wifi_connect(); // in wifi_info.h
  //homekit_storage_reset(); // to remove the previous HomeKit pairing storage when you first run this new HomeKit example
  my_homekit_setup();

  monServo.attach(D4);
}

void loop() 
{
  my_homekit_loop();
  delay(10);
}

//==============================
// HomeKit setup and loop
//==============================

extern "C" homekit_server_config_t config;
extern "C" homekit_characteristic_t cha_switch_on;

static uint32_t next_heap_millis = 0;




void cha_switch_on_setter(const homekit_value_t value) 
{

bool  on = value.bool_value;
  
  cha_switch_on.value.bool_value = on;  //sync de la valeur
  if(on == true)
  {
    monServo.write(180);
  }
  if(on == false)
  {
    monServo.write(0);
  }
  Serial.print(on);
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
  if (t > next_heap_millis) 
  {
    // afficher les informations sur le tas toutes les 5 secondes
    next_heap_millis = t + 5 * 1000;
    LOG_D("Free heap: %d, HomeKit clients: %d",
        ESP.getFreeHeap(), arduino_homekit_connected_clients_count());

  }
}
