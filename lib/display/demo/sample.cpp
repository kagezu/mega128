#include "display/display.h"

// Display lcd;

void Lcd_Writ_Bus(unsigned char d)
{
  SPDR = d;

  SPI_WAIT;
}

void Lcd_Write_Com(unsigned char VH)
{
  PORTC &= _BV(A3);
  Lcd_Writ_Bus(VH);
}

void Lcd_Write_Data(unsigned char VH)
{
  PORTC |= _BV(A3);
  Lcd_Writ_Bus(VH);
}

void Lcd_Write_Com_Data(unsigned char com, unsigned char dat)
{
  Lcd_Write_Com(com);
  Lcd_Write_Data(dat);
}

void Address_set(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
  Lcd_Write_Com(0x2a);
  Lcd_Write_Data(x1 >> 8);
  Lcd_Write_Data(x1);
  Lcd_Write_Data(x2 >> 8);
  Lcd_Write_Data(x2);
  Lcd_Write_Com(0x2b);
  Lcd_Write_Data(y1 >> 8);
  Lcd_Write_Data(y1);
  Lcd_Write_Data(y2 >> 8);
  Lcd_Write_Data(y2);
  Lcd_Write_Com(0x2c);
}


void Lcd_Init(void)
{
  digitalWrite(A4, HIGH);
  delayMicroseconds(15000); // Ждать стабилизации напряжений
  digitalWrite(A4, LOW);
  delayMicroseconds(15000); // Ждать стабилизации напряжений
  digitalWrite(A4, HIGH);
  delayMicroseconds(15000); // Ждать стабилизации напряжений

  digitalWrite(A5, LOW);  //CS

  Lcd_Write_Com(0x11);
  delayMicroseconds(15000); // Ждать стабилизации напряжений
  Lcd_Write_Com(0xB1);
  Lcd_Write_Data(0x05);
  Lcd_Write_Data(0x3C);
  Lcd_Write_Data(0x3C);
  Lcd_Write_Com(0xB2);
  Lcd_Write_Data(0x05);
  Lcd_Write_Data(0X3C);
  Lcd_Write_Data(0X3C);
  Lcd_Write_Com(0xB3);
  Lcd_Write_Data(0x05);
  Lcd_Write_Data(0x3C);
  Lcd_Write_Data(0x3C);
  Lcd_Write_Data(0x05);
  Lcd_Write_Data(0x3C);
  Lcd_Write_Data(0x3C);
  Lcd_Write_Com(0xB4);
  Lcd_Write_Data(0x03);
  Lcd_Write_Com(0xC0);
  Lcd_Write_Data(0x28);
  Lcd_Write_Data(0x08);
  Lcd_Write_Data(0x04);
  Lcd_Write_Com(0xC1);
  Lcd_Write_Data(0xC0);
  Lcd_Write_Com(0xC2);
  Lcd_Write_Data(0x0D);
  Lcd_Write_Data(0X00);
  Lcd_Write_Com(0xC3);
  Lcd_Write_Data(0x8D);
  Lcd_Write_Data(0x2A);
  Lcd_Write_Com(0xC4);
  Lcd_Write_Data(0x8D);
  Lcd_Write_Data(0xEE);
  Lcd_Write_Com(0xC5);
  Lcd_Write_Data(0x1A);
  Lcd_Write_Com(0x17);
  Lcd_Write_Data(0x05);
  Lcd_Write_Com(0x36);
  Lcd_Write_Data(0xD8);
  Lcd_Write_Com(0xE0);
  Lcd_Write_Data(0x03);
  Lcd_Write_Data(0x22);
  Lcd_Write_Data(0x07);
  Lcd_Write_Data(0x0A);
  Lcd_Write_Data(0x2E);
  Lcd_Write_Data(0x30);
  Lcd_Write_Data(0x25);
  Lcd_Write_Data(0x2A);
  Lcd_Write_Data(0x28);
  Lcd_Write_Data(0x26);
  Lcd_Write_Data(0x2E);
  Lcd_Write_Data(0x3A);
  Lcd_Write_Data(0x00);
  Lcd_Write_Data(0x01);
  Lcd_Write_Data(0x03);
  Lcd_Write_Data(0x13);
  Lcd_Write_Com(0xE1);
  Lcd_Write_Data(0x04);
  Lcd_Write_Data(0x16);
  Lcd_Write_Data(0x06);
  Lcd_Write_Data(0x0D);
  Lcd_Write_Data(0x2D);
  Lcd_Write_Data(0x26);
  Lcd_Write_Data(0x23);
  Lcd_Write_Data(0x27);
  Lcd_Write_Data(0x27);
  Lcd_Write_Data(0x25);
  Lcd_Write_Data(0x2D);
  Lcd_Write_Data(0x3B);
  Lcd_Write_Data(0x00);
  Lcd_Write_Data(0x01);
  Lcd_Write_Data(0x04);
  Lcd_Write_Data(0x13);
  Lcd_Write_Com(0x3A);
  Lcd_Write_Data(0x05);
  Lcd_Write_Com(0x29);
  digitalWrite(A5, HIGH);
}

void LCD_Clear(unsigned int j)
{
  unsigned int l = 128 * 160 - 1;
  unsigned char i = j >> 8;
  digitalWrite(A5, LOW);
  Address_set(0, 0, 127, 159);
  Lcd_Write_Data(j >> 8);
  //      Lcd_Write_Data(j);
  SPDR = j;
  while (l--) {
    while (!(SPSR & _BV(SPIF))); // wait
    SPDR = i;
    asm volatile("nop");
    while (!(SPSR & _BV(SPIF))); // wait
    SPDR = j;
    //    asm volatile("nop");
    //    while (!(SPSR & _BV(SPIF))) ; // wait
  }
  //    while (!(SPSR & _BV(SPIF))) ; // wait
  digitalWrite(A5, HIGH);
}

int main(void)
{
  DDRC |= LCD_RS | LCD_CS | LCD_RESET;
  PORTC |= LCD_RS | LCD_CS | LCD_RESET;
  DDRB |= _BV(PB2) | _BV(PB4) | _BV(PB5);
  PORTB = _BV(PB2) | (PORTB & ~(_BV(PB4) | _BV(PB5)));
  SPCR = _BV(SPE) | _BV(MSTR) | _BV(2);// | _BV(3);
  SPSR = _BV(SPI2X);
  SPDR = 0;


  Lcd_Init();
  for (;;) {
    LCD_Clear(0);
    LCD_Clear(500);


    // SPDR = 0;

    // SPI_WAIT;

    // while (!(SPSR & _BV(SPIF)));

    // lcd.clear(0xf800);
    // lcd.clear(0x07E0);
    // lcd.clear(0x001F);
    // lcd.clear(0x0);

    // for (int i = 0; i < 300; i++) {
    //   lcd.color(random(65535));
    //   lcd.rectLine(random(127), random(159), random(127), random(159)); // rectangle at x, y, with, hight, color
    // }

  }
}
