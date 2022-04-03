#include "ds3231.h"
#include "ds3231.c"
#include <stdio.h>
#include <cstdlib>
#include "pico/stdlib.h"
#include "hardware/rtc.h"
#include "Fonts/font12.c"
#include "pico/sleep.h"
#include "utils/sleep.cpp"
#include "solar.cpp"
#include "draw.cpp"
#include "widgets.cpp"


//TODO: Remove all date related code
extern uint8_t buf[];
extern char *week[];

static const char *DATETIME_MONTHS[12] = {
        "January",
        "February",
        "March",
        "April",
        "May",
        "June",
        "July",
        "August",
        "September",
        "October",
        "November",
        "December"
};

static const char *DATETIME_DOWS[7] = {
        "Sunday",
        "Monday",
        "Tuesday",
        "Wednesday",
        "Thursday",
        "Friday",
        "Saturday",
};

int8_t convert(uint8_t input) {
    char buffi[2];
    sprintf(buffi, "%02x", input);
    return atoi(buffi);
}

int main() {
    ////// Test
    const uint LED_PIN = 25;

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 1);
    /////// Test

    // For remote debug
    sleep_ms(5000);

    stdio_init_all();
    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Kyiv coordinates
    SolarThings solar(30.524937, 50.464631);
    Drawer drawer;

    ds3231ReadTime();
    datetime_t t = {(int16_t) (2000 + convert(buf[6])),
                    convert(buf[5] & 0x1F),
                    convert(buf[4] & 0x3F),
                    (int8_t) (convert(buf[3] & 0x07) - 1),
                    convert(buf[2] & 0x3F),
                    convert(buf[1] & 0x7F),
                    convert(buf[0] & 0x7F)
    };

    rtc_init();
    rtc_set_datetime(&t);
    sleep_ms(100);
    while (1) {
        rtc_get_datetime(&t);
        time_t rawtime = mkgmtime(t.year, t.month, t.day, t.hour, t.min, t.sec);
        Paint_Clear(WHITE);
        sun_paths(solar, rawtime, drawer,2);
        moon_earth(solar, rawtime, drawer, 2);

        char time_str[5];
        sprintf(time_str, "%02d:%02d", (t.hour + 2) % 24, t.min);
        Paint_DrawString_EN(20, 230, time_str, &Font48, WHITE, GRAY4);
        drawer.commit();


        gpio_put(LED_PIN, 0);
        int till_next_minute = 60 - (rawtime % 60);
        deep_sleep(till_next_minute);
        gpio_put(LED_PIN, 1);
    }
    return 0;
}




