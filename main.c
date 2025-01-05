// IR Remote Controlled Mouse-ish thing (kinda)
#include <xc.h>

// CONFIG BITS
#pragma config OSC = INTIO67
#pragma config STVREN = OFF
#pragma config PWRT = ON
#pragma config BOREN = OFF
#pragma config WDT = OFF
#pragma config PBADEN = OFF
#pragma config CCP2MX = PORTC
#pragma config MCLRE = OFF
#pragma config LVP = OFF

// Serial output (for debugging)
#use delay(clock = 8000000)
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7, bits=8)

// Function Prototypes
void SysInit(void);
void pause(int);
void fakeMouseSend(void);
void nop(void);

// Variables
signed int8 mousePacket[4] = {0, 0, 0, 0};
short irReady = 0, irRepeat = 0;
int8 bitCounter = 0, decodeState = 0;
int32 irData = 0;

// External interrupt for IR
#INT_EXT
void ext_isr(void) {
    int16 t;
    if (decodeState != 0)
        t = get_timer1();

    switch(decodeState) {
        case 0:
            setup_timer_1(T1_INTERNAL | T1_DIV_BY_2);
            set_timer1(0);
            decodeState = 1;
            bitCounter = 0;
            ext_int_edge(L_TO_H);
            break;

        case 1:
            if (t < 8500 || t > 9500) {
                decodeState = 0;
                setup_timer_1(T1_DISABLED);
            } else {
                decodeState = 2;
                ext_int_edge(H_TO_L);
            }
            break;

        case 2:
            if (t < 1500 || t > 5000) {
                decodeState = 0;
                setup_timer_1(T1_DISABLED);
            } else {
                if (t < 3000) irRepeat = 1;
                decodeState = 3;
                ext_int_edge(L_TO_H);
            }
            break;

        case 3:
            if (t < 400 || t > 700) {
                decodeState = 0;
                setup_timer_1(T1_DISABLED);
            } else {
                if (irRepeat) {
                    set_timer3(25000);
                    irRepeat = 0;
                    irReady = 1;
                } else {
                    decodeState = 4;
                    ext_int_edge(H_TO_L);
                }
            }
            break;

        case 4:
            if (t < 400 || t > 1800) {
                decodeState = 0;
                setup_timer_1(T1_DISABLED);
            } else {
                if (t > 1000)
                    bit_set(irData, 31 - bitCounter);
                else
                    bit_clear(irData, 31 - bitCounter);

                bitCounter++;

                if (bitCounter > 31) {
                    irReady = 1;
                }
                decodeState = 3;
                ext_int_edge(L_TO_H);
            }
            break;
    }
}

// Timer1 = timeout for decoding
#INT_TIMER1
void t1_isr(void) {
    irReady = 0;
    decodeState = 0;
    ext_int_edge(H_TO_L);
    setup_timer_1(T1_DISABLED);
    clear_interrupt(INT_TIMER1);
}

// Timer3 = delay to reset IR code
#INT_TIMER3
void t3_isr(void) {
    irData = 0;
    setup_timer_3(T3_DISABLED);
    clear_interrupt(INT_TIMER3);
}

void main(void) {
    SysInit();

    while (1) {
        if (irReady) {
            irReady = 0;
            decodeState = 0;
            setup_timer_1(T1_DISABLED);

            set_timer3(25000);
            setup_timer_3(T3_INTERNAL | T3_DIV_BY_8);

            switch(irData) {
                case 0x40BF30CF: // btn 1
                    mousePacket[0] = 1;
                    break;
                case 0x40BFB04F: // up
                    mousePacket[2] = -1;
                    break;
                case 0x40BF708F: // btn 3
                    mousePacket[0] = 3;
                    break;
                case 0x40BF08F7: // left
                    mousePacket[1] = -1;
                    break;
                case 0x40BF8877: // down
                    mousePacket[2] = 1;
                    break;
                case 0x40BF48B7: // right
                    mousePacket[1] = 1;
                    break;
            }

            if (irData != 0)
                fakeMouseSend();

            mousePacket[0] = 0;
            mousePacket[1] = 0;
            mousePacket[2] = 0;
            irData = 0;
        }
    }
}

void SysInit(void) {
    OSCCON |= 0x70; // 8 MHz
    OSCTUNEbits.PLLEN = 1;
    ADCON1 = 0x0F;

    TRISC6 = 0; // TX
    TRISC7 = 1; // RX

    TRISB0 = 1; // IR receiver (INT0)
    INTCON2bits.INTEDG0 = 0; // falling edge

    enable_interrupts(GLOBAL);
    enable_interrupts(PERIPH);
    enable_interrupts(INT_EXT);
    enable_interrupts(INT_TIMER1);
    enable_interrupts(INT_TIMER3);
    clear_interrupt(INT_TIMER1);
    clear_interrupt(INT_TIMER3);
}

void pause(int msec) {
    while (msec > 0) {
        for (unsigned char count = 0; count < 194; count++) {
            nop();
        }
        msec--;
    }
}

void nop(void) {
    // literally does nothing
}

void fakeMouseSend(void) {
    printf("Mouse: B=%d, X=%d, Y=%d\n", mousePacket[0], mousePacket[1], mousePacket[2]);
    delay_ms(10);
}
