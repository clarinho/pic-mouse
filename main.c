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