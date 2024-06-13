
#include <string.h>
#include <pico/stdlib.h>
#include <hardware/gpio.h>
#include <hardware/pio.h>

#define EL_VS_PIN    16
#define EL_HS_PIN    17
#define EL_VCLK_PIN  18
#define EL_VID_PIN   19

#define SCR_WIDTH   320
#define SCR_HEIGHT  (256 + 1)
#define SCR_STRIDE  (SCR_WIDTH / 8)

#include "el.pio.h"

#include "lenna.xbm"
#include "gears.xbm"
#include "aperture.xbm"

typedef struct
{
    uint16_t width;
    uint16_t height;
    uint8_t data[SCR_STRIDE * SCR_HEIGHT];

} frame_t;


static frame_t frame0 = 
{
    .width = SCR_WIDTH - 1,
    .height = SCR_HEIGHT - 1,
    .data = {0},
};


static const uint8_t* const pics[] = 
{
    lenna_bits,
    gears_bits,
    aperture_bits,
};


int main(void)
{
    stdio_init_all();

    gpio_init(EL_VS_PIN);
    gpio_init(EL_HS_PIN);
    gpio_init(EL_VCLK_PIN);
    gpio_init(EL_VID_PIN);

    el_program_init(pio0);

    uint8_t pic_idx = 0;

    for (;;)
    {
        memcpy(frame0.data, pics[pic_idx], SCR_HEIGHT * SCR_STRIDE);

        for (uint i = 0; i < 1; i++)
        {
            for (size_t j = 0; j < sizeof(frame0) / sizeof(uint32_t); j++)
            {
                uint32_t data = *(((uint32_t*)&frame0) + j);
                pio_sm_put_blocking(pio0, EL_PIO_SM, data);
                
                while (!pio_sm_is_tx_fifo_empty(pio0, EL_PIO_SM))
                {
                    asm("nop;");
                }
            }
        }

        sleep_ms(1000 * 5);

        pic_idx++;
        if (pic_idx >= (sizeof(pics) / sizeof(pics[0])))
        {
            pic_idx = 0;
        }
    }

    return 0;
}