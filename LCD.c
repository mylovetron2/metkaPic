
// LCD module connections
sbit LCD_RS at LATE0_bit;
sbit LCD_EN at LATE2_bit;
sbit LCD_D4 at LATB4_bit;
sbit LCD_D5 at LATB5_bit;
sbit LCD_D6 at LATB6_bit;
sbit LCD_D7 at LATB7_bit;

sbit LCD_RS_Direction at TRISE0_bit;
sbit LCD_EN_Direction at TRISE2_bit;
sbit LCD_D4_Direction at TRISB4_bit;
sbit LCD_D5_Direction at TRISB5_bit;
sbit LCD_D6_Direction at TRISB6_bit;
sbit LCD_D7_Direction at TRISB7_bit;
// End LCD module connections

char txt1[] = "Welcome to";
char txt2[] = "BIGdsPIC6";
char txt3[] = "Lcd4bit";
char txt4[] = "example";

char i;                              // Loop variable
char uart_rd;
void Move_Delay() {                  // Function used for text moving
  Delay_ms(500);                     // You can change the moving speed here
}

unsigned int adc_rd;
char temp[6];

void main(){
  ADPCFG = 0xFFFF;                   // Configure AN pins as digital I/O
  //TRISB10_bit = 1;

  TRISB=0;
  TRISE.F5=0;
  TRISE.F1=0;
  LATE.F5=0;
  LATE.F1=0;
  LATB.F0=0;
  LATB.F1=0;
  LATB.F2=0;
  LATB.F3=0;
  
  TRISB0_bit=1;
  
  Lcd_Init();                        // Initialize LCD
  UART1_Init(9600);               // Initialize UART module at 9600 bps
  Delay_ms(100);                  // Wait for UART module to stabilize
  ADC1_Init();
  UART1_Write_Text("Start");
  UART1_Write(13);
  UART1_Write(10);
  

  Lcd_Cmd(_LCD_CLEAR);               // Clear display
  Lcd_Cmd(_LCD_CURSOR_OFF);          // Cursor off
  Lcd_Out(1,6,txt3);                 // Write text in first row

  Lcd_Out(2,6,txt4);                 // Write text in second row
  Delay_ms(2000);
  Lcd_Cmd(_LCD_CLEAR);               // Clear display

  Lcd_Out(1,1,txt1);                 // Write text in first row
  Lcd_Out(2,5,txt2);                 // Write text in second row

  Delay_ms(1000); 

  while(1){
           //LATD = ADC1_Get_Sample(10);
           adc_rd=ADC1_Read(0);
           IntToStr(adc_rd,temp);  // txt is "  437" (two blanks here)
           UART1_Write_Text(temp);
           //UART1_Write_Text("\n");
            Lcd_Out(1,1,temp);
           Delay_ms(1000);

  }
}             

/*
    if (UART1_Data_Ready()==1) {     // If data is received
        uart_rd = UART1_Read();     // read the received data
        UART1_Write(uart_rd);       // and send data via UART
        //Lcd_Cmd(_LCD_CLEAR);               // Clear display
        //Lcd_Cmd(_LCD_CURSOR_OFF);          // Cursor off
        //Lcd_Out(1,1,uart_rd);
    }
    */
    /*
TRISD = 0;                  // Set PORTD as output
  TRISB10_bit = 1;             // Set PORTB.B0 as input

  ADC1_Init();                // Initialize ADC module
  Delay_ms(100);

  while(1) {
    LATD = ADC1_Get_Sample(10);   // Get ADC value from corresponding channel
    Delay_ms(50);
  }
  */