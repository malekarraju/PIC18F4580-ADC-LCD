#include<p18f4580.h>

#define RS PORTCbits.RC0
#define E PORTCbits.RC1

void Delay(int);
void Lcd_Cmd(char);
void Lcd_Data(char);
void Lcd_String(const rom char*);
void Lcd_Inti(void);

void main()
{  

	int ADC_Val, i;
	int arr[4];

	// one time execution
	TRISD=0x00;      //Port directio
	
	TRISCbits.RC0 = 0;   // RS
    TRISCbits.RC1 = 0;   // E
	
	
	ADCON0 = 0x03; // ADON=1, GO=1, channel selct AN0
	ADCON1 = 0X0E; // Make analog i/p pins for ANO 0000 1110
	ADCON2 = 0x88; // right justify, clk 
	
	Lcd_Inti();
	Lcd_String("ADC Value:");	

    // use because program stuck here
	while(1)
	{
		ADCON0bits.GO=1;
		while(ADCON0bits.GO);
		
		ADC_Val = ADRESH; // copying 8 bit data
		ADC_Val = (ADC_Val<<8) | ADRESL;

		for(i=0; i<4; i++)
		{
			arr[i] = (ADC_Val%10) + '0';
			ADC_Val /= 10;
		}

		Lcd_Cmd(0xC0);   // 2nd line, 1st position

		for(i=3; i>=0; i--) Lcd_Data(arr[i]);		// reverse storing
	}
}

// Function Defination

void Lcd_Inti()
{	
	/* LCD initi. */
	Lcd_Cmd(0x0e); // Lcd on, Cursor on 0x0f for Blink
	Lcd_Cmd(0x01); // Clear Screen
	Lcd_Cmd(0x06); // Inc Cursor
	Lcd_Cmd(0x38); // 8 bit mode, 2 lines, 5x7 Matrix
	Lcd_Cmd(0x80); // 1st Row, 1st Pos/Col
	
}

void Lcd_String(const rom char* str)
{
	while(*str != '\0')
	{
		Lcd_Data(*str);
		str++;
	}
}

void Delay(int a)
{
	int i,j;
	for(i=0; i<100; i++)
		for(j=0; j<100; j++);
}

void Lcd_Cmd(char ch)
{
	PORTD = ch; 
	RS = 0;  // Command Mode
	// RW = 0;  // Write to LCD, connecting to ground
	E = 1;
	Delay(100);
	E = 0;
}

void Lcd_Data(char dt)
{
	PORTD = dt; 
	RS = 1;  // Data Mode
	// RW = 0;  // Write to LCD, connecting to ground
	E = 1;
	Delay(100);
	E = 0;
}


