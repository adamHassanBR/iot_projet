/***** CO2 *****
* VCC = 5V
* GND = GND
* A0 = A0
/***** CO2 *****/

/***** Chumidité °C *****
* GND = GND
* VCC = 5V
* DATA = D5
/***** Chumidité °C *****/










#include <Arduino.h>
#include <arduino_homekit_server.h>
#include "wifi_info.h"





/***** Chumidité °C *****/

#include "DHT.h"  
#define DHTPIN D5   
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);
#define MQ2pin (A0)

/***** Chumidité °C *****/

float h;
float t;
float co2;


#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);









//==================================================//
//web app//
//==================================================//

/********************Page html********************/
/*
const char index_html2[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <title>Qualité de l'air</title>
</head>
<body>
  <h1>Température : %etatT%</h1>
  <h1>Humidité : %etatH%</h1>
</body>
</html>
)=====";*/

/**************************************/

/*
//Gestion des évenements
void onConnected(const WiFiEventStationModeConnected& event);
void onGotIP(const WiFiEventStationModeGotIP& event);
 



//API
// Exemples de commandes de l'api :
// http://192.168.1.17/camera/api.html?cmd=getCAM
// http://192.168.1.17/camera/api.html?cmd=setCAM&value=ouvert
// http://192.168.1.17/camera/api.html?cmd=setCAM&value=fermer
void handleAPI_html ()
{
  String laTemperature = String(t);
  String lHumidite = String(h);
  
  String temp(reinterpret_cast<const __FlashStringHelper *>(index_html2));
  temp.replace("%etatT%", laTemperature);
  temp.replace("%etatH%", lHumidite);

  serverWeb.send(200, "text/html", temp);
}
void handleJson() {
  float humidity = h;
  float temperature = t;


  StaticJsonDocument<200> doc;
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;

  String out;
  serializeJson(doc, out);
  serverWeb.send(200, "application/json", out);
}

void handleData()
{

  float humiditydata = h;
  float temperaturedata = t;
  
  Serial.println("Serving /data");
  serverWeb.sendHeader("Access-Control-Allow-Origin", "*");
  
  StaticJsonDocument<200> doc;
  doc["temperature"] = temperaturedata;
  doc["humidity"] = humiditydata;

  String out2;
  serializeJson(doc, out2);
  serverWeb.send(200, "application/json",out2);
}


void onConnected(const WiFiEventStationModeConnected& event)
{
  Serial.println("connexion établie");
}

void onGotIP(const WiFiEventStationModeGotIP& event)
{
  Serial.println("Adress IP : " + WiFi.localIP().toString());         // Adress IP : 192.168.1.17
  Serial.println("Paserelle IP : " + WiFi.gatewayIP().toString());    // Paserelle IP : 192.168.1.1
  Serial.println("DNS IP : " + WiFi.dnsIP().toString());              // DNS IP : 192.168.1.1
  Serial.print("Puissance de réception du signal : ");                 // -75 db
  Serial.print(WiFi.RSSI());
  Serial.println(" dB");
}

*/








//==================================================//
//apple homekit//
//==================================================//


extern "C" homekit_server_config_t config;
extern "C" homekit_characteristic_t cha_temperature;
extern "C" homekit_characteristic_t cha_humidity;
extern "C" homekit_characteristic_t cha_co2;
extern "C" homekit_characteristic_t cha_co2Detect;





// Called when the value is read by iOS Home APP
homekit_value_t cha_programmable_switch_event_getter() {
  // Should always return "null" for reading, see HAP section 9.75
  return HOMEKIT_NULL_CPP();
}



static uint32_t next_heap_millis = 0;
static uint32_t next_report_millis = 0;

void my_homekit_report() {


    /***** Chumidité °C *****/
  
    h = dht.readHumidity();
    
    // Lecture de la température
    t = dht.readTemperature();

    co2 = analogRead(MQ2pin);

  
    /***** Chumidité °C *****/
  // FIXME, read your real sensors here.

  float humidityH = h;
  float temperatureH = t;
  float co2H = co2;
  bool co2DetectH = false;
  
  cha_temperature.value.float_value = temperatureH;
  homekit_characteristic_notify(&cha_temperature, cha_temperature.value);

  cha_humidity.value.float_value = humidityH;
  homekit_characteristic_notify(&cha_humidity, cha_humidity.value);

  cha_co2.value.float_value = co2H;
  homekit_characteristic_notify(&cha_co2, cha_co2.value);

  cha_co2Detect.value.bool_value = co2DetectH;
  homekit_characteristic_notify(&cha_co2Detect, cha_co2Detect.value);


  LOG_D("temperatureH %.1f, humidityH %.1f, co2H %.1f", temperatureH, humidityH, co2H);
}

int random_value(int min, int max) {
  return min + random(max - min);
}

void my_homekit_setup() {
  arduino_homekit_setup(&config);
}

void my_homekit_loop() {
  arduino_homekit_loop();
  const uint32_t t = millis();
  if (t > next_report_millis) {
    // report sensor values every 10 seconds
    next_report_millis = t + 10 * 1000;
    my_homekit_report();
  }
  if (t > next_heap_millis) {
    // Show heap info every 5 seconds
    next_heap_millis = t + 5 * 1000;
    LOG_D("Free heap: %d, HomeKit clients: %d",
        ESP.getFreeHeap(), arduino_homekit_connected_clients_count());

  }
}



void setup() 
{
  
	Serial.begin(115200);
  Serial.println("");

  // dans wifi_info.h permet à l'esp8266 à internet
	wifi_connect(); // in wifi_info.h
 
	// pour supprimer le stockage de couplage HomeKit précédent lorsque vous exécutez ce nouvel exemple HomeKit pour la première fois
  //homekit_storage_reset(); 

  



  
  /***** Chumidité °C *****/
   
  dht.begin();
   
  /***** Chumidité °C *****/
/*
  static WiFiEventHandler onConnectedHandler = WiFi.onStationModeConnected(onConnected);
  static WiFiEventHandler onGotIPHandler = WiFi.onStationModeGotIP(onGotIP);


  //Mise en place du server Web
  serverWeb.on("/capteur_qualite_air/index.html",handleAPI_html); 
  serverWeb.on("/capteur_qualite_air/json", handleJson);
  serverWeb.on("/capteur_qualite_air/data", handleData);
  serverWeb.begin();

*/
  
	my_homekit_setup();
}

void loop() {
	if(WiFi.isConnected())
  {
    serverWeb.handleClient();
  }
 
  my_homekit_loop();
  delay(10);
}

//==============================
// HomeKit setup and loop
//==============================
