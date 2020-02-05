/*
 * File:   lab2.c
 * Author: Peter Yau
 *
 * Created on January 30, 2020, 7:54 PM
 */
// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h>
#include "lib_osccon.h"
#include "lib_adc.h"

int res = 0;
int res2 = 0;
int intB0 = 0;
int intB1 = 0;
int i = 0;
int j = 0;
int contador = 0;
int contador2 = 0;
int resultado;
void setup(void);
void set_adc(void);
void osccon(void);
void intEnable(void);
unsigned char lookup(unsigned char num);
void delay_ms(void);

void __interrupt() isr(void){
    if (PORTBbits.RB0 == 1){
        di();
        intB0 = 1;
        if (PORTBbits.RB0 == 0 && intB0 == 1){
            PORTA -= 1;
            intB0 = 0;
            INTCONbits.INTF = 0;
            ei();
        }
    }
    if (PIR1bits.ADIF == 1){
        di();
        res = ADRESH & 0b00001111;
        PORTD = lookup(res);
        PORTCbits.RC6 = 0;
        res2 = ADRESH >> 4;
        PORTD = lookup(res2);
        PORTCbits.RC6 = 1;
        PIR1bits.ADIF = 0;
        ei();
    }
}

void main(void) {
    setup();
    intEnable();
    oscInt(1);
    adcSetup();
    analogInSel(11);
    adcFoscSel(1);
    while(1){
        if (ADCON0bits.GO_DONE == 0){
            ADCON0bits.GO_DONE = 1;
        }
        if (PORTBbits.RB1 == 1){
            intB1 = 1;
            if (PORTBbits.RB1 == 0 && intB1 == 1){
                PORTA += 1;
                intB1 = 0;
            }
        }
    }
    return;
}

void delay_ms(void){
    for (i = 0; i <= 25; i ++){
        contador += contador;
        for (j = 0; j <= 25; j ++){
            contador2 += contador2;
        }
    }
}

void setup(void){
    TRISA = 0x00;
    TRISB = 0x13;
    TRISC = 0x00;
    TRISD = 0x00;
    ANSEL = 0x10;
    ANSELH = 0x00;
    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0xFF;
}

void intEnable(void){
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIE1bits.ADIE = 1;
    IOCB = 0x02;
}

unsigned char lookup(unsigned char num){
    switch(num){
        case 0b00000000: num = 0xC0; break;
        case 0b00000001: num = 0xF9; break;
        case 0b00000010: num = 0xA4; break;
        case 0b00000011: num = 0xB0; break;
        case 0b00000100: num = 0x99; break;
        case 0b00000101: num = 0x92; break;
        case 0b00000110: num = 0x82; break;
        case 0b00000111: num = 0xF8; break;
        case 0b00001000: num = 0x80; break;
        case 0b00001001: num = 0x90; break;
        case 0b00001010: num = 0x88; break;
        case 0b00001011: num = 0x83; break;
        case 0b00001100: num = 0xC6; break;
        case 0b00001101: num = 0xA1; break;
        case 0b00001110: num = 0x86; break;
        case 0b00001111: num = 0x8E; break;
    }
    return(num);
}

