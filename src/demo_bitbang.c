
#include <pico/stdlib.h>
#include <hardware/gpio.h>

#define EL_VS_PIN    16
#define EL_HS_PIN    17
#define EL_VCLK_PIN  18
#define EL_VID_PIN   19

#define SCR_WIDTH   320
#define SCR_HEIGHT  (256 + 1)
#define SCR_STRIDE  (SCR_WIDTH / 8)

#include "lenna.xbm"
#include "gears.xbm"
#include "aperture.xbm"


static const uint8_t* const pics[] = 
{
    lenna_bits,
    gears_bits,
    aperture_bits,
};


static void display_frame(const uint8_t *bitmap)
{
    gpio_put(EL_VCLK_PIN, 1);
    gpio_put(EL_VS_PIN, 1);
    sleep_us(1);
    gpio_put(EL_VS_PIN, 0);

    for (uint y = 0; y <= SCR_HEIGHT; y++)
    {    
        gpio_put(EL_HS_PIN, 0);
        for (uint x = 0; x < 2; x++)
        {
            gpio_put(EL_VCLK_PIN, 0);
            gpio_put(EL_VCLK_PIN, 1);
        }
        gpio_put(EL_HS_PIN, 1);

        for (int x = 0; x < SCR_STRIDE; x++)
        {        
            uint8_t byte = bitmap[y * SCR_STRIDE + x];
            
            for (uint bn = 0; bn < 8; bn++)
            {
                gpio_put(EL_VID_PIN, (~byte >> bn) & 1);
                gpio_put(EL_VCLK_PIN, 0);
                gpio_put(EL_VCLK_PIN, 1);
            }
        }
    }

    gpio_put(EL_VS_PIN, 0);
    gpio_put(EL_HS_PIN, 0);
    gpio_put(EL_VCLK_PIN, 0);
}


int main(void)
{
    stdio_init_all();

    gpio_init(EL_VS_PIN);
    gpio_init(EL_HS_PIN);
    gpio_init(EL_VCLK_PIN);
    gpio_init(EL_VID_PIN);

    gpio_set_dir(EL_VS_PIN, true);
    gpio_set_dir(EL_HS_PIN, true);
    gpio_set_dir(EL_VCLK_PIN, true);
    gpio_set_dir(EL_VID_PIN, true);


    uint8_t pic_idx = 0;

    for (;;)
    {
        display_frame(pics[pic_idx]);
        sleep_ms(1000 * 5);

        pic_idx++;
        if (pic_idx >= (sizeof(pics) / sizeof(pics[0])))
        {
            pic_idx = 0;
        }
    }
    
    return 0;
}
