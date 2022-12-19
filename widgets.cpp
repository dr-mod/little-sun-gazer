#include "../Pico_ePaper_Code/c/lib/Fonts/font24.c"
//#include "Fonts/font48.c"
#include "../Pico_ePaper_Code/c/lib/Fonts/font8.c"


void moon_earth(SolarThings &solar, time_t utc_time, Drawer &draw, int time_zone) {
    short WIDTH = 280;
    short BIG_Y = 132;
    short EARTH_ORBIT = 80;
    short MOON_ORBIT = 40;
    double coordinates[4];
    solar.celestial_bodies((long) utc_time, coordinates);

    double feta = atan2(coordinates[0], coordinates[1]);

    short sun_center_x = 280 / 2;
    short sun_center_y = BIG_Y;

    short earth_x = EARTH_ORBIT * sin(feta) + sun_center_x;
    short earth_y = BIG_Y - EARTH_ORBIT * cos(feta);

    draw.pen_color(0);
    draw.circle(sun_center_x, sun_center_y, 20);
    Paint_DrawString_EN(sun_center_x - 3*8/2, sun_center_y  + 20, "sun", &Font12, WHITE, GRAY4);

    draw.pen_color(2);
    draw.circle_empty(sun_center_x, sun_center_y, EARTH_ORBIT);
    draw.pen_color(0);
    draw.circle(earth_x, earth_y, 14);

    draw.pen_color(2);
    draw.circle_empty(earth_x, earth_y, MOON_ORBIT);

    feta = atan2(coordinates[2], coordinates[3]);
    draw.pen_color(0);
    short moon_x = earth_x + MOON_ORBIT * sin(feta);
    short moon_y = earth_y - MOON_ORBIT * cos(feta);
    draw.circle(moon_x, moon_y, 8);
    if (moon_y > earth_y) {
        Paint_DrawString_EN(earth_x - 5*8/2, earth_y - 14, "earth", &Font12, WHITE, GRAY4);
    } else {
        Paint_DrawString_EN(earth_x - 5*8/2, earth_y + 14, "earth", &Font12, WHITE, GRAY4);
    }
    Paint_DrawString_EN(moon_x - 4*8/2, moon_y + 8, "moon", &Font12, WHITE, GRAY4);
}

float sun_paths(SolarThings &solar, time_t utc_time, Drawer &draw, short time_zone) {
    short WIDTH = 280;
    short HEIGHT = 120;
    short X = 0;
    short Y = 480 - 127;
    short N = 60;

    draw.pen_width(1);
    draw.pen_style(1);
    long utc_time_day_start = (utc_time - (utc_time % (60 * 60 * 24)));
    long non_utc_time_day_start = utc_time_day_start - (60 * 60 * time_zone);

    float angles[N + 1];
    float min, max;
    min = max = 0;
    for (int i = 0; i <= N; i++) {
        angles[i] = solar.elevation((non_utc_time_day_start + i * (SEC_IN_DAY / N)));
        if (min > angles[i]) min = angles[i];
        if (max < angles[i]) max = angles[i];
    }
    float amplitude = max - min;
    for (int i = 0; i <= N; i++) {
        angles[i] += abs(min);
        angles[i] /= amplitude;
    }

    short zero_deg = (1 - (abs(min) / amplitude)) * HEIGHT + Y;
    draw.pen_color(1);
    draw.line(0, zero_deg, WIDTH, zero_deg);

    draw.pen_color(0);
    for (int i = 1; i <= N; i++) {
        short x1 = ((i - 1) * (SEC_IN_DAY / N)) / ((double) SEC_IN_DAY) * WIDTH + X;
        short x2 = (i * (SEC_IN_DAY / N)) / ((double) SEC_IN_DAY) * WIDTH + X;
        short y1 = (1 - angles[i - 1]) * HEIGHT + Y;
        short y2 = (1 - angles[i]) * HEIGHT + Y;

        draw.line(x1, y1, x2, y2);
    }

    float current_angle = solar.elevation(utc_time);
    current_angle += abs(min);
    current_angle /= amplitude;
    short sun_x = (((utc_time % SEC_IN_DAY) + (60 * 60 * time_zone)) % SEC_IN_DAY) / ((double) SEC_IN_DAY) * WIDTH + X;
    draw.circle(sun_x, (1 - current_angle) * HEIGHT + Y, 10);

    draw.pen_color(2);
    draw.pen_style(0);
    long sunrise_time = solar.sunrise(utc_time_day_start + SEC_IN_DAY / 2, 2);
    short line_x = (sunrise_time % SEC_IN_DAY) / ((double) SEC_IN_DAY) * WIDTH + X;
    draw.line(line_x, zero_deg, line_x, Y - 3);

    long sunset_time = solar.sunset((utc_time_day_start + SEC_IN_DAY / 2), 2);
    short line_x_2 = (sunset_time % SEC_IN_DAY) / ((double) SEC_IN_DAY) * WIDTH + X;
    draw.line(line_x_2, zero_deg, line_x_2, Y - 3);

    char time_str[6];
    sprintf(time_str, "%02d:%02d", (sunrise_time / (60 * 60)) % 24, sunrise_time / 60 % 60);
    Paint_DrawString_EN(line_x - 7*7/2, Y - 15 - 12, "sunrise", &Font12, WHITE, GRAY4);
    Paint_DrawString_EN(line_x - 7*5/2, Y - 15, time_str, &Font12, WHITE, GRAY4);

    sprintf(time_str, "%02d:%02d", (sunset_time / (60 * 60)) % 24, sunset_time / 60 % 60);
    Paint_DrawString_EN(line_x_2 - 7*6/2, Y - 15 - 12, "sunset", &Font12, WHITE, GRAY4);
    Paint_DrawString_EN(line_x_2 - 7*5/2, Y - 15, time_str, &Font12, WHITE, GRAY4);

    return current_angle;
}