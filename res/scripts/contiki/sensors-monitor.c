#include <stdio.h>
#include "contiki.h"
#include "dev/button-sensor.h"
#include "dev/light-sensor.h"
#include "dev/sht11-sensor.h"
#include "dev/leds.h"
#include <stdio.h>
#include <math.h>

/*---------------------------------------------------------------------------*/
PROCESS(test_button_process, "Test button");
AUTOSTART_PROCESSES(&test_button_process);
/*---------------------------------------------------------------------------*/
static uint8_t active;
PROCESS_THREAD(test_button_process, ev, data)
{
  static struct etimer timer;
  static int light = 0;
  static int temperature = 0;
  static int humidity = 0;
  static int dec;
  PROCESS_BEGIN();

  //Activate sensors
  SENSORS_ACTIVATE(light_sensor);
  SENSORS_ACTIVATE(sht11_sensor);
  while(1) {
    //Get sensors value each seconds
    etimer_set(&timer, CLOCK_CONF_SECOND);
    PROCESS_WAIT_EVENT();

    light = light_sensor.value(0);
    printf("Light: %d\n", light);

    //Convert temperature in °C
    temperature = ((sht11_sensor.value(SHT11_SENSOR_TEMP)/10) - 396)/10;
    printf("Temperature: %d\n", temperature);

    //Concert humidity in %a
    humidity = sht11_sensor.value(SHT11_SENSOR_HUMIDITY);
    int rh = -4 + 0.0405*humidity - 2.8e-6*(humidity*humidity);
    printf("Humidity:%d\n", rh);

    //Turn on the leds
    if(light < 100)
      leds_on(LEDS_YELLOW);
    else
      leds_off(LEDS_YELLOW);

    if(temperature > 25)
      leds_on(LEDS_RED);
    else
      leds_off(LEDS_RED);

    if(rh > 40)
      leds_on(LEDS_BLUE);
    else
      leds_off(LEDS_BLUE);

  }
  //Deactivate sensors
  SENSORS_DEACTIVATE(light_sensor);
  SENSORS_DEACTIVATE(sht11_sensor);
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
