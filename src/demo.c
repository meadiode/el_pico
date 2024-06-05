
#include <pico/stdlib.h>
#include <hardware/gpio.h>
#include <hardware/pwm.h>
#include <stdio.h>
#include <string.h>


#define EL_VS    16
#define EL_HS    17
#define EL_VCLK  18
#define EL_VID   19

#define SCR_WIDTH   320
#define SCR_HEIGHT  256
#define SCR_STRIDE  (SCR_WIDTH / 8)


void frame(uint32_t t)
{
    uint32_t cnt = 0;

    gpio_put(EL_VCLK, 1);
    gpio_put(EL_VS, 1);


    for (int y = 0; y < SCR_HEIGHT + 1; y++)
    {
        gpio_put(EL_HS, 0);
    
        for (int x = 0; x < 2; x++)
        {
            gpio_put(EL_VCLK, 0);
            sleep_us(1);
            gpio_put(EL_VCLK, 1);
        }
        
        gpio_put(EL_HS, 1);

    
        for (int x = 0; x < SCR_STRIDE; x++)
        {
            gpio_put(EL_VS, y == 0 && x == 0);
        
            for (int b = 0; b < 8; b++)
            {
                uint8_t c = (((x * 8 + b + t) / 32) + ((y + t) / 32)) % 2;
                
                gpio_put(EL_VID,  c);
                gpio_put(EL_VCLK, 0);
                gpio_put(EL_VCLK, 1);
                cnt += 1;
            }
        }
        cnt += 1;
    }

    gpio_put(EL_VS, 0);
    gpio_put(EL_HS, 0);
    gpio_put(EL_VCLK, 0);
}


int main(void)
{
    stdio_init_all();

    gpio_init(EL_VS);
    gpio_init(EL_HS);
    gpio_init(EL_VCLK);
    gpio_init(EL_VID);

    gpio_set_dir(EL_VS, true);
    gpio_set_dir(EL_HS, true);
    gpio_set_dir(EL_VCLK, true);
    gpio_set_dir(EL_VID, true);

    uint32_t t = 0;

    for (;;)
    {
        frame(t);
        t++;
    }
    
    return 0;
}
