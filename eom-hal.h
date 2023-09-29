#ifndef __eom_hal_hpp
#define __eom_hal_hpp

#include <stddef.h>
#include <stdint.h>

#include "u8g2.h"

#define EOM_HAL_RETURN_ERRCHK(expr)                                                                \
    {                                                                                              \
        eom_hal_err_t _errchk = (expr);                                                            \
        if (EOM_HAL_OK != _errchk) {                                                               \
            return _errchk;                                                                        \
        }                                                                                          \
    }

#define EOM_HAL_ERR_CHK(val)                                                                       \
    {                                                                                              \
        eom_hal_err_t err = val;                                                                   \
        if (EOM_HAL_OK != err) {                                                                   \
            ESP_LOGE(TAG, "ERROR: %s returned %s", #val, eom_hal_err_str[err]);                    \
        }                                                                                          \
    }

#define EOM_DISPLAY_WIDTH 128
#define EOM_DISPLAY_HEIGHT 64

#define EOM_HAL_PRESSURE_MAX 0x0FFF // 12 bits ADC reading

//=== Housekeeping

#ifdef __cplusplus
extern "C" {
#endif

enum eom_hal_err {
    EOM_HAL_OK,
    EOM_HAL_FAIL,
    EOM_HAL_ERR_HARDWARE_LOCKED,
    EOM_HAL_ERR_NO_STORAGE,
    EOM_HAL_DEVICE_BUSY,
    EOM_HAL_TIMEOUT,
};

typedef enum eom_hal_err eom_hal_err_t;

enum eom_hal_hardware_type {
    EOM_HAL_HARDWARE_TYPE_NG_PLUS,
    EOM_HAL_HARDWARE_TYPE_EOM_3000,
    EOM_HAL_HARDWARE_TYPE_EOM_AIO,
};

typedef enum eom_hal_hardware_type eom_hal_hardware_type_t;

eom_hal_hardware_type_t eom_hal_get_hardware_type(void);

const char* eom_hal_get_version(void);

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

enum eom_hal_button_event { EOM_HAL_BUTTON_PRESS, EOM_HAL_BUTTON_HOLD, EOM_HAL_BUTTON_DOWN };

typedef enum eom_hal_button_event eom_hal_button_event_t;

typedef void (*eom_hal_button_callback_t)(eom_hal_button_t, eom_hal_button_event_t);

/** @deprecated use eom_hal_register_button_handler */
eom_hal_err_t eom_hal_register_button_press(eom_hal_button_t button, eom_hal_button_callback_t cb);
/** @deprecated use eom_hal_register_button_handler */
eom_hal_err_t eom_hal_register_button_hold(eom_hal_button_t button, eom_hal_button_callback_t cb);

eom_hal_err_t eom_hal_register_button_handler(eom_hal_button_callback_t cb);

//=== Rotary Encoder

struct eom_hal_color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

typedef struct eom_hal_color eom_hal_color_t;
typedef void (*eom_hal_encoder_callback_t)(int delta);

void eom_hal_set_encoder_color(eom_hal_color_t color);
void eom_hal_set_encoder_rgb(uint8_t r, uint8_t g, uint8_t b);
void eom_hal_set_encoder_brightness(uint8_t bright);
/** @deprecated use eom_hal_register_encoder_handler*/
void eom_hal_register_encoder_change(eom_hal_encoder_callback_t cb);
void eom_hal_register_encoder_handler(eom_hal_encoder_callback_t cb);

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
    EOM_HAL_ACCESSORY_MASTER,

    /**
     * Accept commands using the default data stream protocol.
     */
    EOM_HAL_ACCESSORY_SLAVE,
};

typedef enum eom_hal_accessory_mode eom_hal_accessory_mode_t;

#define EOM_HAL_ACCESSORY_BUS_DISPLAY_NAME_LEN 40

struct eom_hal_accessory_bus_device {
    char display_name[EOM_HAL_ACCESSORY_BUS_DISPLAY_NAME_LEN];
    uint8_t address;
};

typedef struct eom_hal_accessory_bus_device eom_hal_accessory_bus_device_t;

typedef void (*eom_hal_accessory_scan_callback_t)(eom_hal_accessory_bus_device_t*, void*);

eom_hal_accessory_mode_t eom_hal_get_accessory_mode(void);
void eom_hal_set_accessory_mode(eom_hal_accessory_mode_t mode);
void eom_hal_accessory_master_write(uint8_t address, uint8_t* bytes, size_t length);
void eom_hal_accessory_master_read(uint8_t address, uint8_t* bytes, size_t length);
void eom_hal_accessory_master_read_register(uint8_t address, uint8_t reg, uint8_t* bytes,
                                            size_t length);
void eom_hal_accessory_master_write_register(uint8_t address, uint8_t reg, uint8_t* bytes,
                                             size_t length);
void eom_hal_accessory_master_write_str(uint8_t address, const char* str);
void eom_hal_accessory_master_read_str(uint8_t address, char* buffer, size_t max_length);
eom_hal_err_t eom_hal_accessory_master_probe(uint8_t address);
void eom_hal_accessory_scan_bus(void);
void eom_hal_accessory_scan_bus_p(eom_hal_accessory_scan_callback_t cb, void* param);

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
eom_hal_err_t eom_hal_get_device_serial(char* dst, size_t len);
eom_hal_err_t eom_hal_set_device_serial(char* serial);
const char* eom_hal_get_device_serial_const(void);

//=== SD Card
const char* eom_hal_get_sd_mount_point(void);
long long int eom_hal_get_sd_size_bytes(void);

//=== Display
u8g2_t* eom_hal_get_display_ptr(void);
int eom_hal_get_display_width(void);
int eom_hal_get_display_height(void);
void eom_hal_send_display_screenshot(const char* label);

#ifdef __cplusplus
}
#endif

#endif
