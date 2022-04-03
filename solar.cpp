#include <math.h>

#define RADEG       (180.0/M_PI)
#define SEC_IN_DAY   (60 * 60 * 24)
#define JULIAN_DAY_1970 (2451544.5 - 10957)

class SolarThings {
private:
    double longitude;
    double latitude;

    void from_sun(double M_moon, double e_moon, double a_moon, double N_moon, double w_moon, double i_moon, double (&moon_xy)[2]) {
        double E0 = M_moon + (180 / M_PI) * e_moon * sin(M_moon / RADEG) * (1 + e_moon * cos(M_moon / RADEG));
        E0 = normalize(E0);
        double E1 = E0 - (E0 - (180 / M_PI) * e_moon * sin(E0 / RADEG) - M_moon) / (1 - e_moon * cos(E0 / RADEG));
        E1 = normalize(E1) / RADEG;
        double x = a_moon * (cos(E1) - e_moon);
        double y = a_moon * (sqrt(1 - e_moon * e_moon)) * sin(E1);

        double r = sqrt(x * x + y * y);
        double v = atan2(y, x);
        v = normalize(v * RADEG);

        moon_xy[0] = r * (cos(N_moon / RADEG) * cos((v + w_moon) / RADEG) - sin(N_moon / RADEG) * sin(
                (v + w_moon) / RADEG) * cos((i_moon) / RADEG));
        moon_xy[1] = r * (sin(N_moon / RADEG) * cos((v + w_moon) / RADEG) + cos(N_moon / RADEG) * sin(
                (v + w_moon) / RADEG) * cos(i_moon / RADEG));
    }

    static double normalize(double degrees) {
        return fmod(degrees, 360);
    }

    double refraction_correction(double zenith) {
        double exoatm_elevation = 90 - zenith;
        if (exoatm_elevation > 85) {
            return 0;
        }
        double refractionCorrection;
        double te = tan(exoatm_elevation / RADEG);
        if (exoatm_elevation > 5.0) {
            refractionCorrection =
                    58.1 / te - 0.07 / (te * te * te) + 0.000086 / (te * te * te * te * te);
        } else {
            if (exoatm_elevation > -0.575) {
                refractionCorrection = get_refraction_corrected_elevation(exoatm_elevation);
            } else {
                refractionCorrection = -20.774 / te;
            }
        }
        return refractionCorrection / 3600;
    }

    double get_refraction_corrected_elevation(double e) {
        return 1735.0 + e * (-518.2 + e * (103.4 + e * (-12.79 + e * 0.711)));
    }

    double solar_noon_time(double lon, double eqTime) {
        return 720.0 + (lon * 4.0) - eqTime;
    }

    double sunrise_time(double lon, double eqTime, double ha, short timezone) {
        return 720.0 + ((lon - ha) * 4.0) - eqTime + 60 * timezone;
    }

    double sunset_time(double lon, double eqTime, double ha, short timezone) {
        return 720.0 + ((lon + ha) * 4.0) - eqTime + 60 * timezone;
    }

    double julian(double time) {
        return (time / SEC_IN_DAY) + JULIAN_DAY_1970;
    }

    double sun_declination(double t) {
        double e = obliquity_corrected(t) / RADEG;
        double b = sun_apparent_longitude(t) / RADEG;
        double sint = sin(e) * sin(b);
        double theta = asin(sint);
        return theta * RADEG;
    }

    double eccentricity_earth_orbit(double t) {
        return 0.016708634 - t * (0.000042037 + 0.0000001267 * t);
    }

    double equation_of_time(double t) {
        double eps = obliquity_corrected(t) / RADEG;
        double l0 = sun_geometric_mean_longitude(t) / RADEG;
        double m = sun_geometric_mean_anomaly(t) / RADEG;
        double e = eccentricity_earth_orbit(t);
        double y = tan(eps / 2);
        y *= y;

        double sin2l0 = sin(2 * l0);
        double cos2l0 = cos(2 * l0);
        double sin4l0 = sin(4 * l0);
        double sin1m = sin(m);
        double sin2m = sin(2 * m);

        double etime =
                y * sin2l0
                - 2 * e * sin1m
                + 4 * e * y * sin1m * cos2l0
                - 0.5 * y * y * sin4l0
                - 1.25 * e * e * sin2m;

        return (etime * RADEG) * 4.0;
    }

    double sun_geometric_mean_anomaly(double t) {
        return 357.52911 + t * (35999.05029 - 0.0001537 * t);
    }

    double sun_equation_of_center(double t) {
        double m = sun_geometric_mean_anomaly(t) / RADEG;
        return sin(1 * m) * (1.914602 - t * (0.004817 + 0.000014 * t))
               + sin(2 * m) * (0.019993 - t * (0.000101))
               + sin(3 * m) * (0.000289);
    }

    double sun_geometric_mean_longitude(double t) {
        double l0 = 280.46646 + t * (36000.76983 + 0.0003032 * t);
        l0 = l0 - 360 * floor(l0 / 360);
        return l0;
    }

    double sun_true_longitude(double t) {
        return sun_geometric_mean_longitude(t) + sun_equation_of_center(t);
    }

    double sun_apparent_longitude(double t) {
        double omega = (125.04 - 1934.136 * t) / RADEG;
        return sun_true_longitude(t) - 0.00569 - 0.00478 * sin(omega);
    }

    double mean_obliquity_of_ecliptic(double t) {
        double seconds = 21.448 - t * (46.8150 + t * (0.00059 - t * (0.001813)));
        return 23.0 + (26.0 + (seconds / 60.0)) / 60.0;
    }

    double obliquity_corrected(double t) {
        double e0 = mean_obliquity_of_ecliptic(t);
        double omega = (125.04 - 1934.136 * t) / RADEG;
        return e0 + 0.00256 * cos(omega);
    }

public:
    SolarThings(double longitude, double latitude) {
        this->longitude = -longitude;
        this->latitude = (latitude > 89.8) ? 89.8 : latitude;
        this->latitude = (latitude < -89.8) ? -89.8 : this->latitude;
    }

    double elevation(long time) {
        double julian_day = julian(time);
        double mtime = (julian_day - 2451545) / 36525;

        double solar_declin = sun_declination(mtime);
        double eqTime = equation_of_time(mtime);

        long noonTime =
                round(solar_noon_time(longitude, eqTime) * 60)
                + (time / SEC_IN_DAY) * SEC_IN_DAY;

        double trueSolarTime = ((julian_day + 0.5) - floor(julian_day + 0.5)) * 1440;
        trueSolarTime += (eqTime - 4.0 * longitude);
        trueSolarTime -= 1440 * floor(trueSolarTime / 1440);


        double latitude_rad = this->latitude / RADEG;

        double csz =
                sin(latitude_rad) * sin(solar_declin / RADEG)
                + cos(latitude_rad)
                  * cos(solar_declin / RADEG)
                  * cos((trueSolarTime / 4 - 180) / RADEG);
        if (csz > +1) csz = +1;
        if (csz < -1) csz = -1;

        double zenith = acos(csz);

        double refractionCorrection = this->refraction_correction(zenith * RADEG);
        double solarZen = (zenith * RADEG) - refractionCorrection;

        double elevation = 90 - solarZen;

        return elevation;
    }

    long sunrise(long time, short timezone) {
        double julian_day = julian(time);
        double mtime = (julian_day - 2451545) / 36525;
        double solar_declin = sun_declination(mtime);
        double eq_time = equation_of_time(mtime);

        double latitude_rad = this->latitude / RADEG;

        double ha = acos((cos(90.883 / RADEG)/(cos(latitude_rad) * cos(solar_declin / RADEG)))
                - tan(latitude_rad) * tan(solar_declin / RADEG)) * RADEG;
        long sunrise_timev = round(sunrise_time(longitude, eq_time, ha, timezone) * (60))
                             + (time / SEC_IN_DAY) * SEC_IN_DAY;

        return sunrise_timev;
    }

    long sunset(long time, short timezone) {
        double julian_day = julian(time);
        double mtime = (julian_day - 2451545) / 36525;
        double solar_declin = sun_declination(mtime);
        double eq_time = equation_of_time(mtime);

        double latitude_rad = this->latitude / RADEG;

        double ha = acos((cos(90.883 / RADEG)/(cos(latitude_rad) * cos(solar_declin / RADEG)))
                         - tan(latitude_rad) * tan(solar_declin / RADEG)) * RADEG;
        long sunrise_timev = round(sunset_time(longitude, eq_time, ha, timezone) * (60))
                             + (time / SEC_IN_DAY) * SEC_IN_DAY;

        return sunrise_timev;
    }

    void celestial_bodies(long time, double (&result)[4]) {
        double julian_day = julian(time);
        double d = (julian_day - 2451545);

        double w = 282.9404 + 4.70935E-5 * d;
        double e = (0.016709 - (1.151E-9 * d));
        double M = normalize(356.047 + 0.9856002585 * d);

        double E = M + (180 / M_PI) * e * sin(M / RADEG) * (1 + e * cos(M / RADEG));
        E = E / RADEG;
        double xv = cos(E) - e;
        double yv = sin(E) * sqrt(1 - e * e);

        double r = sqrt(xv * xv + yv * yv);

        double v = atan2(yv, xv) * RADEG;
        double lonsun = normalize(v + w) / RADEG;
        double xs = r * cos(lonsun);
        double ys = r * sin(lonsun);

        double earthX = -1 * xs;
        double earthY = -1 * ys;

        double N_moon = normalize(125.1228 - 0.0529538083 * d);
        double i_moon = 5.1454;
        double w_moon = 318.0634 + 0.1643573223 * d;
        double a_moon = 60.2666 * 4.26354E-5;
        double e_moon = 0.054900;
        double M_moon = normalize(115.3654 + 13.0649929509 * d);

        double moon_xy[2];
        from_sun(M_moon, e_moon, a_moon, N_moon, w_moon, i_moon, moon_xy);

        result[0] = earthX;
        result[1] = earthY;
        result[2] = moon_xy[0];
        result[3] = moon_xy[1];
    }
};

