/* Nokia 5110 LCD AVR Library example
 *
 * Copyright (C) 2015 Sergey Denisov.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 *
 * Original library written by SkewPL, http://skew.tk
 * Custom char code by Marcelo Cohen - 2021
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "nokia5110.h"

uint8_t glyph[] =
    {
        0b00001000,
        0b00010010,
        0b01100000,
        0b00010010,
        0b00001000};

char str[10];
char tempo[10];
float total = 0.0;

// interrupção timer;
ISR(TIMER2_OVF_vect)
{
    if (total < 1.5)
    {
        PORTC |= (1 << PC3);
    }
    while (total < 1.5)
    {
        if (PINB & (1 << PB0))
        {
            if ((total + 1.0) > 1.5)
            {
                total = 1.5;
            }
            else
            {
                total += 1.0;
            }
            _delay_ms(500);
            nokia_lcd_clear();
            nokia_lcd_write_string("Saldo: R$", 1);
            dtostrf(total, 2, 2, str);
            nokia_lcd_write_string(str, 1);
            nokia_lcd_render();
        }
        else if (PIND & (1 << PD7))
        {
            if ((total + 0.25) > 1.5)
            {
                total = 1.5;
            }
            else
            {
                total += 0.25;
            }
            _delay_ms(500);
            nokia_lcd_clear();
            nokia_lcd_write_string("Saldo: R$", 1);
            dtostrf(total, 2, 2, str);
            nokia_lcd_write_string(str, 1);
            nokia_lcd_render();
        }
        else if (PIND & (1 << PD6))
        {
            if ((total + 0.5) > 1.5)
            {
                total = 1.5;
            }
            else
            {
                total += 0.5;
            }
            _delay_ms(500);
            nokia_lcd_clear();
            nokia_lcd_write_string("Saldo: R$", 1);
            dtostrf(total, 2, 2, str);
            nokia_lcd_write_string(str, 1);
            nokia_lcd_render();
        }
    }
    if (total >= 1.5)
    {
        _delay_ms(500);
        PORTC &= ~(1 << PC3);

        nokia_lcd_clear();
        nokia_lcd_write_string("Saldo: R$", 1);
        dtostrf(total, 2, 2, str);
        nokia_lcd_write_string(str, 1);
        nokia_lcd_set_cursor(0, 12);
        nokia_lcd_write_string("Ligue!", 1);
        nokia_lcd_render();
    }
    while (total >= 1.5)
    {
        float segundos = 10.0;
        if (PIND & (1 << PD5))
        {
            for (int cont = 0; cont <= 9; cont++)
            {
                PORTC |= (1 << PC2);
                nokia_lcd_clear();
                nokia_lcd_write_string("Saldo: R$", 1);
                nokia_lcd_write_string(str, 1);
                nokia_lcd_set_cursor(0, 12);
                nokia_lcd_write_string("Tempo: ", 1);
                dtostrf(segundos, 2, 0, tempo);
                nokia_lcd_write_string(tempo, 1);
                nokia_lcd_write_string(" s", 1);
                nokia_lcd_render();
                _delay_ms(1000);
                segundos -= 1.0;
            }

            PORTC &= ~(1 << PC2);
            _delay_ms(500);
            PORTC |= (1 << PC1);
            _delay_ms(500);
            PORTC &= ~(1 << PC1);
            total = 0.0;
        }
    }
    nokia_lcd_clear();
    nokia_lcd_write_string("Saldo: R$", 1);
    dtostrf(total, 2, 2, str);
    nokia_lcd_write_string(str, 1);
    nokia_lcd_render();
    _delay_ms(500);
}

int main(void)
{

    // desativa interrupção;
    cli();

    TCCR2A = 0x00;
    TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20);

    TIMSK2 = (1 << TOIE2);

    DDRC |= (1 << PC1) | (1 << PC2) | (1 << PC3);

    DDRB &= ~(1 << PB0);
    DDRD &= ~(1 << PD5);
    DDRD &= ~(1 << PD6);
    DDRD &= ~(1 << PD7);

    nokia_lcd_init();
    nokia_lcd_clear();
    nokia_lcd_write_string("Saldo: R$", 1);
    dtostrf(total, 2, 2, str);
    nokia_lcd_write_string(str, 1);
    nokia_lcd_render();

    _delay_ms(500);

    // habilitar interrupção;
    sei();
    for (;;)
    {
    }
}
