

#include <Arduino.h>
#include <arduino_homekit_server.h>
#include "wifi_info.h"
#include <Servo.h>
Servo monServo;
bool etat;
bool on;
#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);





//==================================================//
//web app//
//==================================================//

/********************Page html********************/

const char index_html2[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <title>Cache caméra</title>
    <meta http-equiv="Refresh" content="0; url=http://admirweb.com/hassa-adam/iot/cacheCam.php?getCAM=%getCAM%" />
</head>
<body>

</body>
</html>
)=====";

/**************************************/

//Gestion des évenements
void onConnected(const WiFiEventStationModeConnected& event);
void onGotIP(const WiFiEventStationModeGotIP& event); 



//API
// Exemples de commandes de l'api :
// http://192.168.1.17/camera/api.html?cmd=getCAM
// http://192.168.1.17/camera/api.html?cmd=setCAM&value=ouvert
// http://192.168.1.17/camera/api.html?cmd=setCAM&value=fermer
void handleAPI_html()
{
  String reponse;
  String temp2(reinterpret_cast<const __FlashStringHelper *>(index_html2));
  
  //serverWeb.args() = nombre d'agument
  if(serverWeb.args() > 0 ) // Dans le cas ou on a au moin un argument sur l'API
  {
    if(serverWeb.argName(0) == "cmd") // Dans le cas ou on a le premier argument sur l'API qui est "cmd"
    {
       if(serverWeb.args() == 1 ) // Dans le cas ou on a uniquement 1 argument sur l'API
       {
          if(serverWeb.arg(0) == "getCAM") // Dans le cas ou l'argument est "getCAM"
          {
            
              //reponse = etat; 
              if(etat == true)
              {
                reponse = "1";
                temp2.replace("%getCAM%", "1");
              }
              else
              {
                reponse = "0";
                temp2.replace("%getCAM%", "0");
              }
              serverWeb.send(200, "text/html", temp2);
          }
       }
       else if(serverWeb.args() == 2 ) // Dans le cas ou on a 2 argument sur l'API
       {
         if(serverWeb.arg(0) == "setCAM") // Dans le cas ou l'argument est "setCAM"
          {
            if(serverWeb.argName(1) == "value") // Dans le cas ou le deuxième argument est "value"
            {
              if(serverWeb.arg(1) == "ouvert") // Dans le cas le deuxième argument "value" a pour valeur "ouvert"
              {
                //Serial.println("OUVERT");
                etat = true;
                on = etat;
                monServo.write(180);
                temp2.replace("%getCAM%", "1");
              }
              else if (serverWeb.arg(1) == "fermer") // Dans le cas le deuxième argument "value" a pour valeur "fermer"
              {
                //Serial.println("FERMER");
                etat = false;
                on = etat;
                monServo.write(0);
                temp2.replace("%getCAM%", "0");
              }
            }
          }   
       }   
    } 
    serverWeb.send(200, "text/html", temp2);
  }
  else // dans le cas ou on a pas d'agument sur l'API
  {
    reponse = "Appele à l'API sans arguments ...";
    serverWeb.send(200, "text/html", temp2);
  }
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


//==================================================//
//web app//
//==================================================//









//==================================================//
//apple homekit//
//==================================================//


// accéder au caractéristiques HomeKit définies dans my_accessoire.c
extern "C" homekit_server_config_t config;
extern "C" homekit_characteristic_t cha_switch_on;

static uint32_t next_heap_millis = 0;



//Appelé lorsque la valeur du commutateur est modifiée par iOS Home APP
void cha_switch_on_setter(const homekit_value_t value) 
{
  on = value.bool_value;
  
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

  //Ajoutez la fonction .setter pour obtenir l'événement de commutation envoyé depuis l'application iOS Home.
  //Le .setter doit être ajouté avant arduino_homekit_setup.
  //Le serveur HomeKit utilise le .setter_ex en interne, voir la fonction homekit_accessories_init.
  //Il s'agit peut-être d'un problème de conception hérité de la bibliothèque esp-homekit d'origine,
  //et je n'ai aucune raison de modifier cette "fonctionnalité".
  cha_switch_on.setter = cha_switch_on_setter;
  arduino_homekit_setup(&config);

  //signaler la valeur de commutation à HomeKit si elle est modifiée (par exemple par un bouton physique)
  //bool switch_is_on = true/false;
  //cha_switch_on.value.bool_value = switch_is_on;
  //homekit_characteristic_notify(&cha_switch_on, cha_switch_on.value);
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

//==================================================//
//apple homekit//
//==================================================//

void setup() 
{
  Serial.begin(115200);
 Serial.println("");
  // dans wifi_info.h permet à l'esp8266 à internet
  wifi_connect();
 
  // pour supprimer le stockage de couplage HomeKit précédent lorsque vous exécutez ce nouvel exemple HomeKit pour la première fois
  //homekit_storage_reset(); 
 
  my_homekit_setup();
   //Declaration des sorties
  monServo.attach(D4);

  static WiFiEventHandler onConnectedHandler = WiFi.onStationModeConnected(onConnected);
  static WiFiEventHandler onGotIPHandler = WiFi.onStationModeGotIP(onGotIP);

  //Mise en place du server Web
  serverWeb.on("/camera/index.html",handleAPI_html);
  serverWeb.begin();
  


  
}

void loop() 
{
  if(WiFi.isConnected())
  {
    serverWeb.handleClient();
  }
  
  my_homekit_loop();
  delay(10);
}
