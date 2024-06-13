
#include <string.h>
#include <pico/stdlib.h>
#include <hardware/gpio.h>
#include <hardware/pio.h>
#include <hardware/dma.h>

#define EL_VS_PIN    16
#define EL_HS_PIN    17
#define EL_VCLK_PIN  18
#define EL_VID_PIN   19

#define SCR_WIDTH   320
#define SCR_HEIGHT  (256 + 1)
#define SCR_STRIDE  (SCR_WIDTH / 8)
#define WH_HDR      (((SCR_HEIGHT - 1) << 16) | (SCR_WIDTH - 1))

#include "el.pio.h"

#include "lenna.xbm"
#include "gears.xbm"
#include "aperture.xbm"

static const uint8_t* const pics[] = 
{
    lenna_bits,
    gears_bits,
    aperture_bits,
};

static int32_t dma_chan = -1; 

int main(void)
{
    stdio_init_all();

    gpio_init(EL_VS_PIN);
    gpio_init(EL_HS_PIN);
    gpio_init(EL_VCLK_PIN);
    gpio_init(EL_VID_PIN);

    el_program_init(pio0);

    dma_chan = dma_claim_unused_channel(true);
    dma_channel_config cfg = dma_channel_get_default_config(dma_chan);
    
    channel_config_set_transfer_data_size(&cfg, DMA_SIZE_32);
    channel_config_set_read_increment(&cfg, true);
    channel_config_set_dreq(&cfg, DREQ_PIO0_TX0);

    dma_channel_configure(
        dma_chan,
        &cfg,
        &pio0_hw->txf[EL_PIO_SM],                   /* Write address */
        NULL,                                       /* Read address */
        SCR_HEIGHT * SCR_STRIDE / sizeof(uint32_t), /* Number of transfers */
        false                                       /* Don't start yet */
    );


    uint8_t pic_idx = 0;

    for (;;)
    {
        /* put the screen width and height constants to the SM */
        pio_sm_put_blocking(pio0, EL_PIO_SM, WH_HDR);

        /* tell DMA to transfer bitmap data to the SM */
        dma_channel_set_read_addr(dma_chan, pics[pic_idx], true);

        sleep_ms(1000 * 5);

        pic_idx++;
        if (pic_idx >= (sizeof(pics) / sizeof(pics[0])))
        {
            pic_idx = 0;
        }
    }

    return 0;
}