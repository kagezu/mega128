#include "xpage/xpage.h"
#include "display/display.h"
#include "text/text.h"
#include "font/micro_5x6.h"

XPage page0(0x000, XMEM_60K, 0x1100);
XPage page1(0x100, XMEM_60K, 0x1100);
XPage page2(0x000, XMEM_4K, 0x1100);
XPage page3(0x100, XMEM_4K, 0x1100);
XPage page4(0x010, XMEM_0K, 0x1100);
XPage page5(0x110, XMEM_0K, 0x1100);

Display lcd;
Text text(&lcd);
uint16_t error = 0;

void volatile fill(uint16_t length, uint8_t data)
{
  uint8_t *ptr = (uint8_t *)0x1100;
  while (length--) *ptr++ = data;
}

uint16_t volatile test(uint16_t length, uint8_t data)
{
  uint8_t *ptr = (uint8_t *)0x1100;
  uint16_t e = 0;
  while (length--) if (*ptr++ != data) e++;
  error += e;
  return e;
}

void volatile testPage(XPage *page, const char *msg1, const char *msg2)
{
  page->use();

  fill(page->getSizeHeap(), 0xff);
  uint16_t e = test(page->getSizeHeap(), 0xff);
  text.print(msg1);
  text.print(e);
  text.printR(PSTR(" "));

  fill(page->getSizeHeap(), 0x00);
  e = test(page->getSizeHeap(), 0x00);
  text.print(msg2);
  text.print(e);
  text.printR(PSTR(" "));
}

int main()
{
  lcd.clear(RGB(0x40u));
  lcd.background(RGB(0x40u));
  lcd.color(RGB(0xffff00u));
  text.font(micro_5x6);

  for (;;) {
    text.at(0, 0);
    text.printR(PSTR("Тестирование памяти"));
    text.printR(PSTR(" "));

    testPage(&page0, "PAGE 0 - FF - errors: ", "PAGE 0 - 00 - errors: ");
    testPage(&page1, "PAGE 1 - FF - errors: ", "PAGE 1 - 00 - errors: ");
    testPage(&page2, "PAGE 2 - FF - errors: ", "PAGE 2 - 00 - errors: ");
    testPage(&page3, "PAGE 3 - FF - errors: ", "PAGE 3 - 00 - errors: ");
    testPage(&page4, "PAGE 4 - FF - errors: ", "PAGE 4 - 00 - errors: ");
    testPage(&page5, "PAGE 5 - FF - errors: ", "PAGE 5 - 00 - errors: ");
    text.printR(PSTR(" "));
    text.print("Total errors: ");
    text.print(error);

  }
}

