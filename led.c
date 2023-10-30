#include "led.h"
#include "hardware/gpio.h"

uint64_t TxOnUntil = 0;
uint64_t RxOnUntil = 0;
uint64_t ErrOnUntil = 0;

bool LedInverted = false;
int LedRxPin = 0;
int LedTxPin = 0;
int LedErrorPin = 0;

int64_t led_err_alarm_off( alarm_id_t aid, void* _unused ) {
    absolute_time_t curr = get_absolute_time();
    if (LedErrorPin != -1 && ErrOnUntil && curr > ErrOnUntil)
        gpio_put(LedErrorPin, LedInverted);
    
    return ErrOnUntil > curr ? ErrOnUntil - curr : 1000; //us
}

int64_t led_tx_alarm_off( alarm_id_t aid, void* _unused ) {
    absolute_time_t curr = get_absolute_time();
    if (LedTxPin != -1 && TxOnUntil && curr > TxOnUntil)
        gpio_put(LedTxPin, LedInverted);
    
    return TxOnUntil > curr ? TxOnUntil - curr : 1000; //us
}

int64_t led_rx_alarm_off( alarm_id_t aid, void* _unused ) {
    absolute_time_t curr = get_absolute_time();
    if (LedRxPin != -1 && RxOnUntil && curr > RxOnUntil)
        gpio_put(LedRxPin, LedInverted);
    
    return RxOnUntil > curr ? RxOnUntil - curr : 1000; //us
}

void led_init( bool inverted, int ledRxPin, int ledTXPin, int ledErrorPin_ ) {
    LedTxPin = ledTXPin;
    LedRxPin = ledRxPin;
    LedErrorPin = ledErrorPin_;
    LedInverted = inverted;

    if ( LedTxPin != -1 ) {
        gpio_init( LedTxPin);
        gpio_set_dir(LedTxPin, GPIO_OUT);
        gpio_put(LedTxPin, (LedInverted) ? 1 : 0);
    }

    if ( LedTxPin != LedRxPin ) {
        if ( LedRxPin != -1 ) {
            gpio_init( LedRxPin);
            gpio_set_dir(LedRxPin, GPIO_OUT);
            gpio_put(LedRxPin, (LedInverted) ? 1 : 0);
        }
    }

    if ( ( LedErrorPin != LedRxPin ) && ( LedErrorPin != LedTxPin ) ) {
        if ( LedErrorPin != -1 ) {
            gpio_init( LedErrorPin);
            gpio_set_dir(LedErrorPin, GPIO_OUT);
            gpio_put(LedErrorPin, (LedInverted) ? 1 : 0 );
        }
    }
    led_tx_on_until(300);
    led_rx_on_until(300);
    led_err_on_until(300);

    add_alarm_in_us(1, led_tx_alarm_off, NULL, true);
    add_alarm_in_us(2, led_rx_alarm_off, NULL, true);
    add_alarm_in_us(3, led_err_alarm_off, NULL, true);
}

void led_tx_on_until( uint64_t delta_ms ) {
    TxOnUntil = MAX(TxOnUntil, get_absolute_time() + (delta_ms * 1000));
    if (LedTxPin != -1)
        gpio_put(LedTxPin, !LedInverted);
}

void led_rx_on_until( uint64_t delta_ms ) {
    RxOnUntil = MAX(RxOnUntil, get_absolute_time() + (delta_ms * 1000));
    if (LedRxPin != -1)
        gpio_put(LedRxPin, !LedInverted);
}

void led_err_on_until( uint64_t delta_ms ) {
    ErrOnUntil = MAX(ErrOnUntil, get_absolute_time() + (delta_ms * 1000));
    if (LedErrorPin != -1)
        gpio_put(LedErrorPin, !LedInverted);
}

void led_tx( bool state ) {
    TxOnUntil = 0;
    if ( LedTxPin != -1 )  
        gpio_put(LedRxPin, LedInverted ^ state);
}

void led_rx( bool state ) {
    RxOnUntil = 0;
    if ( LedRxPin != -1 )  
        gpio_put(LedRxPin, LedInverted ^ state);
}

void led_error( bool state ) {
    ErrOnUntil = 0;
    if ( LedErrorPin != -1 )  
        gpio_put(LedErrorPin, LedInverted ^ state);
}
