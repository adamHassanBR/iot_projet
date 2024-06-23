#include <homekit/homekit.h>
#include <homekit/characteristics.h>



void my_accessory_identify(homekit_value_t _value) 
{
	printf("accessory identify\n");
}


homekit_characteristic_t cha_current_temperature = HOMEKIT_CHARACTERISTIC_(CURRENT_TEMPERATURE, 0);
homekit_characteristic_t cha_name_temperature = HOMEKIT_CHARACTERISTIC_(NAME, "Temperature Sensor");

homekit_characteristic_t cha_humidity  = HOMEKIT_CHARACTERISTIC_(CURRENT_RELATIVE_HUMIDITY, 0);
homekit_characteristic_t cha_name_humidity = HOMEKIT_CHARACTERISTIC_(NAME, "humidity Sensor");

homekit_characteristic_t cha_switch_on = HOMEKIT_CHARACTERISTIC_(ON, false);
homekit_characteristic_t cha_name = HOMEKIT_CHARACTERISTIC_(NAME, "Switch");

homekit_accessory_t *accessories[] = 
{
    HOMEKIT_ACCESSORY(.id=1, .category=homekit_accessory_category_sensor, .services=(homekit_service_t*[]) 
    {
        HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) 
        {
            HOMEKIT_CHARACTERISTIC(NAME, "Capteur d'environnement"),
            HOMEKIT_CHARACTERISTIC(MANUFACTURER, "Hassan Adam"),
            HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "0123456"),
            HOMEKIT_CHARACTERISTIC(MODEL, "ESP8266/ESP32"),
            HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "2.0"),
            HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
            NULL
        }),
        HOMEKIT_SERVICE(TEMPERATURE_SENSOR, .primary=true, .characteristics=(homekit_characteristic_t*[]) 
        {
            &cha_current_temperature,
			      &cha_name_temperature,
            NULL
        }),		
        HOMEKIT_SERVICE(HUMIDITY_SENSOR, .characteristics=(homekit_characteristic_t*[]) 
        {
            HOMEKIT_CHARACTERISTIC(NAME, "Humidity Sensor"),
            &cha_humidity,
            &cha_name_humidity,
            NULL
        }),
        HOMEKIT_SERVICE(SWITCH, .characteristics=(homekit_characteristic_t*[]) 
        {
            HOMEKIT_CHARACTERISTIC(NAME, "Humidity Sensor"),
            &cha_switch_on,
			      &cha_name,
            NULL
        }),
        NULL
    }),
    NULL
};

homekit_server_config_t config = 
{
		.accessories = accessories,
		.password = "111-11-112"
};


