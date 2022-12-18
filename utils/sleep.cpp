#include "hardware/clocks.h"
#include "../../pico-extras/src/rp2_common/hardware_rosc/include/hardware/rosc.h"
#include "hardware/structs/scb.h"
#include "time_utils.cpp"

void sleep_callback(void) {
    printf("RTC woke us up\n");
}

void rtc_sleep(int seconds) {
    datetime_t t;
    rtc_get_datetime(&t);
    time_t rawtime = mkgmtime(t.year, t.month, t.day, t.hour, t.min, t.sec);
    rawtime += seconds;
    timestamp_to_datetime(rawtime, &t);
    uart_default_tx_wait_blocking();
    sleep_goto_sleep_until(&t, &sleep_callback);
}

void recover_from_sleep(uint scb_orig, uint clock0_orig, uint clock1_orig){
    //Re-enable ring Oscillator control
    rosc_write(&rosc_hw->ctrl, ROSC_CTRL_ENABLE_BITS);

    //reset procs back to default
    scb_hw->scr = scb_orig;
    clocks_hw->sleep_en0 = clock0_orig;
    clocks_hw->sleep_en1 = clock1_orig;

    //reset clocks
    clocks_init();
    stdio_init_all();
}

static void deep_sleep(int seconds) {
    uint scb_orig = scb_hw->scr;
    uint clock0_orig = clocks_hw->sleep_en0;
    uint clock1_orig = clocks_hw->sleep_en1;

    sleep_run_from_xosc();

    rtc_sleep(seconds);

    recover_from_sleep(scb_orig, clock0_orig, clock1_orig);
}