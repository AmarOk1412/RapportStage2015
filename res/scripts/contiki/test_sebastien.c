#include "contiki.h"
#include "stm32f10x.h"
#include <stdio.h>
#include "status_led.h"

static struct etimer timerLed;

static int it = 0;
static bool ledRX = TRUE;
static bool ledTX = TRUE;

PROCESS(test_leds_and_spi, "Test de l'allumage des leds via SPI");
AUTOSTART_PROCESSES(&test_leds_and_spi);

PROCESS_THREAD(test_leds_and_spi, ev, data)
{
  //Begin Process
  PROCESS_BEGIN();

  //Init Leds
  status_led_init();
  status_led_rx_on(ledRX);
  status_led_tx_on(ledTX);

  //Set timers
  etimer_set(&timerLed, CLOCK_CONF_SECOND);

  while(1)
  {
    PROCESS_WAIT_EVENT();
    if(ev == PROCESS_EVENT_TIMER)
    {
      ledRX = !ledRX;
      it++;
      if(it%3==0)
        ledTX = !ledTX;
      status_led_rx_on(ledRX);
      status_led_tx_on(ledTX);
      etimer_reset(&timerLed);
    }
  }

  PROCESS_END();
}
