/**
* author: monoit
* date: 31-May-2018 12:08:23 AM
* blinky: toggles PORTB pins on and off every 150ms
*/

#define F_CPU 16000000UL
#include <avr/uart.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/myInputOutput.h>

void initUART(void)
{
	// Not necessary; initialize anyway
	DDRD |= _BV(PD1);
	DDRD &= ~_BV(PD0);

	// Set baud rate; lower byte and top nibble
	UBRR0H = ((_UBRR) & 0xF00);
	UBRR0L = (uint8_t) ((_UBRR) & 0xFF);

	TX_START();
	RX_START();

	// Set frame format = 8-N-1
	UCSR0C = (_DATA << UCSZ00);

}

/*! \brief Returns a byte from the serial buffer
 * 	Use this function if the RX interrupt is not enabled.
 * 	Returns 0 on empty buffer
 */
uint8_t getByte(void)
{
	// Check to see if something was received
	while (!(UCSR0A & _BV(RXC0)));
	return (uint8_t) UDR0;
}


/*! \brief Transmits a byte
 * 	Use this function if the TX interrupt is not enabled.
 * 	Blocks the serial port while TX completes
 */
void putByte(unsigned char data)
{
	// Stay here until data buffer is empty
	while (!(UCSR0A & _BV(UDRE0)));
	UDR0 = (unsigned char) data;

}

/*! \brief Writes an ASCII string to the TX buffer */
void writeString(char *str)
{
	while (*str != '\0')
	{
		putByte(*str);
		++str;
	}
}

const char* readString(void)
{
	char rxstr[RX_BUFF];
	char* temp;
	temp = rxstr;

	while((*temp = getByte()) != '\n')
	{
		++temp;
	}

	return rxstr;
}



int main()	{
        pinMode('C',5,1);
		pinMode('C',4,1);
        initUART();
        int data = 0;
		while(1)
		{
            while(!(UCSR0A & (1<<RXC0))); { 
                data = UDR0;
                /*if(UDR0 == 0b00000010) {
                    digitalWrite('C',5,1);
                    //_delay_ms(220);
                }
                if(UDR0 == 0b00000011) {
                    digitalWrite('C',5,0);
                    //_delay_ms(220);
                }
                if(UDR0 == 0b00000100) {
                    digitalWrite('C',4,1);
                    //_delay_ms(220);
                }
                if(UDR0 == 0b00000101) {
                    digitalWrite('C',4,0);
                    //_delay_ms(220);
                }*/
            }
            if(data == 2) {
                int toogle = 0;
                if(digitalRead('C',5) == 0) {
                    toogle = 1;
                }
                digitalWrite('C',5,toogle);
                data = 0;
            }
            /*if(data == 3) {
                digitalWrite('C',5,0);
                data = 0;
            }*/
            if(data == 4) {
                int toogle = 0;
                if(digitalRead('C',4) == 0) {
                    toogle = 1;
                }
                digitalWrite('C',4,toogle);
                data = 0;
            }
            if(data == 5) {
                char state = "1";
                
                if(digitalRead('C',5) == 1) {
                    //state = 1;
                }
                data = 0;
                writeString(&state);
            }
		}
		return 0;
	}
