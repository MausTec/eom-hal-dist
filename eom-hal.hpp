#ifndef __eom_hal_hpp
#define __eom_hal_hpp

#include <stdint.h>
#include <stddef.h>

//=== Housekeeping

enum eom_hal_err {
    EOM_HAL_OK,
    EOM_HAL_FAIL,
    EOM_HAL_ERR_HARDWARE_LOCKED,
    EOM_HAL_ERR_NO_STORAGE,
};

typedef enum eom_hal_err eom_hal_err_t;

enum eom_hal_hardware_type {
    EOM_HAL_HARDWARE_TYPE_NG_PLUS,
    EOM_HAL_HARDWARE_TYPE_EOM_3000,
    EOM_HAL_HARDWARE_TYPE_EOM_AIO,
};

typedef enum eom_hal_hardware_type eom_hal_hardware_type_t;

eom_hal_hardware_type_t eom_hal_get_hardware_type(void);

const char *eom_hal_get_version(void);

void eom_hal_init(void);
void eom_hal_tick(void);
void eom_hal_deinit(void);

//=== Button Registration

enum eom_hal_button {
    EOM_HAL_BUTTON_BACK,
    EOM_HAL_BUTTON_MID,
    EOM_HAL_BUTTON_OK,
    EOM_HAL_BUTTON_MENU,
};

typedef enum eom_hal_button eom_hal_button_t;
typedef void (*eom_hal_button_callback_t)(void);

eom_hal_err_t eom_hal_register_button_press(eom_hal_button_t button, eom_hal_button_callback_t cb);
eom_hal_err_t eom_hal_register_button_hold(eom_hal_button_t button, eom_hal_button_callback_t cb);

//=== Rotary Encoder

struct eom_hal_color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

typedef struct eom_hal_color eom_hal_color_t;
typedef void (*eom_hal_encoder_callback_t)(uint8_t delta);

void eom_hal_set_encoder_color(eom_hal_color_t color);
void eom_hal_register_encoder_change(eom_hal_encoder_callback_t cb);

//=== Pressure
uint16_t eom_hal_get_pressure_reading(void);
uint8_t eom_hal_get_sensor_sensitivity(void);
void eom_hal_set_sensor_sensitivity(uint8_t sensitivity);

//=== Vibration
uint8_t eom_hal_get_motor_speed(void);
void eom_hal_set_motor_speed(uint8_t speed);

//=== RJ Abstraction
enum eom_hal_accessory_mode {
    /**
     * Disable the accessory port and disconnect the bus.
     */
    EOM_HAL_ACCESSORY_DISABLED,

    /**
     * Originate commands using the default data stream protocol.
     */
    EOM_HAL_ACCESSORY_SOURCE,

    /**
     * Accept commands using the default data stream protocol.
     */
    EOM_HAL_ACCESSORY_TARGET,
};

typedef enum eom_hal_accessory_mode eom_hal_accessory_mode_t;

eom_hal_accessory_mode_t eom_hal_get_accessory_mode(void);
void eom_hal_set_accessory_mode(eom_hal_accessory_mode_t mode);
void eom_hal_accessory_write(uint8_t *bytes, size_t length);

//=== Display

struct eom_hal_oled_gpio_config {
    int reset_gpio;
    int cs_gpio;
    int dc_gpio;
};

typedef struct eom_hal_oled_gpio_config eom_hal_oled_gpio_config_t;

eom_hal_oled_gpio_config_t eom_hal_get_oled_gpio_config(void);


//=== RGB LED Ring {EoM-AiO, NG+}

uint8_t eom_hal_get_rgb_led_count(void);
void eom_hal_set_rgb_led(uint8_t i, eom_hal_color_t color);

//=== EEPROM Config Abstraction
eom_hal_err_t eom_hal_get_device_serial(char *dst, size_t len);
eom_hal_err_t eom_hal_set_device_serial(char *serial);

#endif