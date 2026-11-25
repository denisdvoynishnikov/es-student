#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/clocks.h"

#include "log.h"

void clk_info(void)
{
    // шапка таблицы: сигнал, настроенная частота, измеренная частота
    printf("%-8s %9s %12s\n", "signal", "set_khz", "measured_khz");

    // clk_ref, clk_sys, clk_peri, clk_usb, clk_adc — clock_get_hz() в кГц и frequency_count_khz()
    uint32_t set_khz = clock_get_hz(clk_ref) / 1000;
    uint32_t measured_khz = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_REF);
    printf("%-8s %9u %12u\n", "clk_ref", (unsigned)set_khz, (unsigned)measured_khz);

    set_khz = clock_get_hz(clk_sys) / 1000;
    measured_khz = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS);
    printf("%-8s %9u %12u\n", "clk_sys", (unsigned)set_khz, (unsigned)measured_khz);

    set_khz = clock_get_hz(clk_peri) / 1000;
    measured_khz = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_PERI);
    printf("%-8s %9u %12u\n", "clk_peri", (unsigned)set_khz, (unsigned)measured_khz);

    set_khz = clock_get_hz(clk_usb) / 1000;
    measured_khz = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_USB);
    printf("%-8s %9u %12u\n", "clk_usb", (unsigned)set_khz, (unsigned)measured_khz);

    set_khz = clock_get_hz(clk_adc) / 1000;
    measured_khz = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_ADC);
    printf("%-8s %9u %12u\n", "clk_adc", (unsigned)set_khz, (unsigned)measured_khz);

    // rosc — прочерк вместо настроенной частоты и измеренная частота
    measured_khz = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_ROSC_CLKSRC);
    printf("%-8s %9s %12u\n", "rosc", "-", (unsigned)measured_khz);
}

void uptime(void)
{
    printf("uptime: %llu ms\n", time_us_64() / 1000);
}

const uint32_t CLK_SYS_LOW_KHZ = 62500;

// set_sys_clock_khz() вызывает set_sys_clock_pll(), а та переводит clk_peri
// на PLL для USB, 48 МГц. Так настроено по умолчанию:
// PICO_CLOCK_ADJUST_PERI_CLOCK_WITH_SYS_CLOCK равен 0.
static void clk_sys_set(uint32_t khz)
{
    if (set_sys_clock_khz(khz, false))
    {
        LOG_INF("clk_sys %u kHz\n", (unsigned)khz);
    }
    else
    {
        LOG_ERR("clk_sys %u kHz is not set\n", (unsigned)khz);
    }
}

void clk_sys_low(void)
{
    clk_sys_set(CLK_SYS_LOW_KHZ);
}

void clk_sys_default(void)
{
    clk_sys_set(SYS_CLK_KHZ);
}