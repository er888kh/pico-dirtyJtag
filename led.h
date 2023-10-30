#ifndef LED_H
#define LED_H

#include <stdbool.h>
#include "pico/stdlib.h"

void led_init( bool inverted, int ledTxPin, int ledRxPin, int errorLedPin );

void led_tx( bool state );
void led_rx( bool state );
void led_error( bool state );

void led_tx_on_until( uint64_t delta_ms );
void led_rx_on_until( uint64_t delta_ms );
void led_err_on_until( uint64_t delta_ms );
/*
int64_t led_err_alarm_off( alarm_id_t aid, void* _unused );
int64_t led_tx_alarm_off( alarm_id_t aid, void* _unused );
int64_t led_rx_alarm_off( alarm_id_t aid, void* _unused );
*/

#endif
