// IR Remote Controlled Mouse-ish thing (kinda)

                decodeState = 0;
            } else {
#INT_TIMER3
                case 0x40BF08F7: // left

}
#include <xc.h>
// External interrupt for IR
                setup_timer_1(T1_DISABLED);
                if (t > 1000)
void t3_isr(void) {
                    mousePacket[1] = -1;
    TRISB0 = 1; // IR receiver (INT0)

#INT_EXT
            } else {
                    bit_set(irData, 31 - bitCounter);
    irData = 0;
                    break;
    INTCON2bits.INTEDG0 = 0; // falling edge
// CONFIG BITS
void ext_isr(void) {
                if (t < 3000) irRepeat = 1;
                else
    setup_timer_3(T3_DISABLED);
                case 0x40BF8877: // down

#pragma config OSC = INTIO67
    int16 t;
                decodeState = 3;
                    bit_clear(irData, 31 - bitCounter);
    clear_interrupt(INT_TIMER3);
                    mousePacket[2] = 1;
    enable_interrupts(GLOBAL);
#pragma config STVREN = OFF
    if (decodeState != 0)
                ext_int_edge(L_TO_H);

}
                    break;
    enable_interrupts(PERIPH);
#pragma config PWRT = ON
        t = get_timer1();
            }
                bitCounter++;

                case 0x40BF48B7: // right
    enable_interrupts(INT_EXT);
#pragma config BOREN = OFF

            break;

void main(void) {
                    mousePacket[1] = 1;
    enable_interrupts(INT_TIMER1);
#pragma config WDT = OFF
    switch(decodeState) {

                if (bitCounter > 31) {
    SysInit();
                    break;
    enable_interrupts(INT_TIMER3);
#pragma config PBADEN = OFF
        case 0:
        case 3:
                    irReady = 1;

            }
    clear_interrupt(INT_TIMER1);
#pragma config CCP2MX = PORTC
            setup_timer_1(T1_INTERNAL | T1_DIV_BY_2);
            if (t < 400 || t > 700) {
                }
    while (1) {

    clear_interrupt(INT_TIMER3);
#pragma config MCLRE = OFF
            set_timer1(0);
                decodeState = 0;
                decodeState = 3;
        if (irReady) {
            if (irData != 0)
}
#pragma config LVP = OFF
            decodeState = 1;
                setup_timer_1(T1_DISABLED);
                ext_int_edge(L_TO_H);
            irReady = 0;
                fakeMouseSend();


            bitCounter = 0;
            } else {
            }
            decodeState = 0;

void pause(int msec) {
// Serial output (for debugging)
            ext_int_edge(L_TO_H);
                if (irRepeat) {
            break;
            setup_timer_1(T1_DISABLED);
            mousePacket[0] = 0;
    while (msec > 0) {
#use delay(clock = 8000000)
            break;
                    set_timer3(25000);
    }

            mousePacket[1] = 0;
        for (unsigned char count = 0; count < 194; count++) {
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7, bits=8)

                    irRepeat = 0;
}
            set_timer3(25000);
            mousePacket[2] = 0;
            nop();

        case 1:
                    irReady = 1;

            setup_timer_3(T3_INTERNAL | T3_DIV_BY_8);
            irData = 0;
        }
// Function Prototypes
            if (t < 8500 || t > 9500) {
                } else {
// Timer1 = timeout for decoding

        }
        msec--;
void SysInit(void);
                decodeState = 0;
                    decodeState = 4;
#INT_TIMER1
            switch(irData) {
    }
    }
void pause(int);
                setup_timer_1(T1_DISABLED);
                    ext_int_edge(H_TO_L);
void t1_isr(void) {
                case 0x40BF30CF: // btn 1
}
}
void fakeMouseSend(void);
            } else {
                }
    irReady = 0;
                    mousePacket[0] = 1;


void nop(void);
                decodeState = 2;
            }
    decodeState = 0;
                    break;
void SysInit(void) {
void nop(void) {

                ext_int_edge(H_TO_L);
            break;
    ext_int_edge(H_TO_L);
                case 0x40BFB04F: // up
    OSCCON |= 0x70; // 8 MHz
    // literally does nothing