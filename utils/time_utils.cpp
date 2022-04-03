const int SecondsPerMinute = 60;
const int SecondsPerHour = 3600;
const int SecondsPerDay = 86400;
const int daysOfMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

bool IsLeapYear(short year) {
    if (year % 4 != 0) return false;
    if (year % 100 != 0) return true;
    return (year % 400) == 0;
}

time_t mkgmtime(short year, short month, short day, short hour, short minute, short second) {
    time_t secs = 0;
    for (short y = 1970; y < year; ++y)
        secs += (IsLeapYear(y) ? 366 : 365) * SecondsPerDay;
    for (short m = 1; m < month; ++m) {
        secs += daysOfMonth[m - 1] * SecondsPerDay;
        if (m == 2 && IsLeapYear(year)) secs += SecondsPerDay;
    }
    secs += (day - 1) * SecondsPerDay;
    secs += hour * SecondsPerHour;
    secs += minute * SecondsPerMinute;
    secs += second;
    return secs;
}

//TODO: This has to be substituted with some standard function
void timestamp_to_datetime(int seconds, datetime_t *t) {

    long int current_year, days_untill_now, extra_time,
            extraDays, index, date, month, flag = 0;

    days_untill_now = seconds / (24 * 60 * 60);
    int day_of_week = (days_untill_now >= -4 ? (days_untill_now + 4) % 7 : (days_untill_now + 5) % 7 + 6) - 1;
    extra_time = seconds % (24 * 60 * 60);
    current_year = 1970;

    while (days_untill_now >= 365) {
        if (current_year % 400 == 0
            || (current_year % 4 == 0
                && current_year % 100 != 0)) {
            days_untill_now -= 366;
        } else {
            days_untill_now -= 365;
        }
        current_year += 1;
    }

    extraDays = days_untill_now + 1;

    if (current_year % 400 == 0
        || (current_year % 4 == 0
            && current_year % 100 != 0))
        flag = 1;

    month = 0, index = 0;
    if (flag == 1) {
        while (true) {
            if (index == 1) {
                if (extraDays - 29 < 0)
                    break;
                month += 1;
                extraDays -= 29;
            } else {
                if (extraDays - daysOfMonth[index] < 0) {
                    break;
                }
                month += 1;
                extraDays -= daysOfMonth[index];
            }
            index += 1;
        }
    } else {
        while (true) {
            if (extraDays - daysOfMonth[index] < 0) {
                break;
            }
            month += 1;
            extraDays -= daysOfMonth[index];
            index += 1;
        }
    }

    if (extraDays > 0) {
        month += 1;
        date = extraDays;
    } else {
        if (month == 2 && flag == 1)
            date = 29;
        else {
            date = daysOfMonth[month - 1];
        }
    }

    t->year = current_year;
    t->month = month;

    t->day = date;
    t->dotw = day_of_week;

    t->sec = (extra_time % 3600) % 60;
    t->min = (extra_time % 3600) / 60;
    t->hour = extra_time / 3600;
}