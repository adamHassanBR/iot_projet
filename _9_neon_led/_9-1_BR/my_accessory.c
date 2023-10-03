
#include <homekit/homekit.h>
#include <homekit/characteristics.h>

void my_accessory_identify(homekit_value_t _value) {
  printf("accessory identify\n");
}



homekit_characteristic_t cha_switch_white_static = HOMEKIT_CHARACTERISTIC_(ON, false);
homekit_characteristic_t cha_switch_white_motion = HOMEKIT_CHARACTERISTIC_(ON, false);
homekit_characteristic_t cha_switch_color_static = HOMEKIT_CHARACTERISTIC_(ON, false);
homekit_characteristic_t cha_switch_color_motion = HOMEKIT_CHARACTERISTIC_(ON, false);


homekit_accessory_t *accessories[] = {
    HOMEKIT_ACCESSORY(.id=1, .category=homekit_accessory_category_switch, .services=(homekit_service_t*[]) {
        HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {
            HOMEKIT_CHARACTERISTIC(NAME, "logo_br"),
            HOMEKIT_CHARACTERISTIC(MANUFACTURER, "HASSAN Adam"),
            HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "0123456"),
            HOMEKIT_CHARACTERISTIC(MODEL, "ESP8266/ESP32"),
            HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "1.0"),
            HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
            NULL
        }),
    HOMEKIT_SERVICE(SWITCH, .primary=true, .characteristics=(homekit_characteristic_t*[]){
      &cha_switch_white_static,
      HOMEKIT_CHARACTERISTIC(NAME, "Blanc"),
      NULL
    }),
    
    HOMEKIT_SERVICE(SWITCH, .primary=true, .characteristics=(homekit_characteristic_t*[]){
      &cha_switch_white_motion,
      HOMEKIT_CHARACTERISTIC(NAME, "Blanc2"),
      NULL
    }),


    
    HOMEKIT_SERVICE(SWITCH, .primary=true, .characteristics=(homekit_characteristic_t*[]){
      &cha_switch_color_static,
       HOMEKIT_CHARACTERISTIC(NAME, "Couleurs"),
      NULL
    }),
    HOMEKIT_SERVICE(SWITCH, .primary=true, .characteristics=(homekit_characteristic_t*[]){
      &cha_switch_color_motion,
       HOMEKIT_CHARACTERISTIC(NAME, "Couleurs2"),
      NULL
    }),
        NULL
    }),
    NULL
};

homekit_server_config_t config = {
    .accessories = accessories,
    .password = "111-11-122"
};
