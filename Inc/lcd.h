#ifndef __LCD_H
#define __LCD_H

#include "stm_lib.h"
#include "lcd_image.h"
#include "lcd_font.h"
#include "printf.h"

#define LCD_W 320
#define LCD_H 240

#define SAVE_X_OFFSET

#define LCD_RES_DN 	LL_GPIO_ResetOutputPin(GPIOD,LL_GPIO_PIN_7)
#define LCD_RES_UP 	LL_GPIO_SetOutputPin(GPIOD,LL_GPIO_PIN_7)
#define LCD_BL_OFF 	LL_GPIO_ResetOutputPin(GPIOB,LL_GPIO_PIN_7)
#define LCD_BL_ON	LL_GPIO_SetOutputPin(GPIOB,LL_GPIO_PIN_7)
#define LCD_DC_DN 	LL_GPIO_ResetOutputPin(GPIOB,LL_GPIO_PIN_6)
#define LCD_DC_UP 	LL_GPIO_SetOutputPin(GPIOB,LL_GPIO_PIN_6)
#define LCD_CS_DN 	LL_GPIO_ResetOutputPin(GPIOB,LL_GPIO_PIN_4)
#define LCD_CS_UP 	LL_GPIO_SetOutputPin(GPIOB,LL_GPIO_PIN_4)

#define BLACK           0x0000      /*   0,   0,   0 */
#define NAVY            0x000F      /*   0,   0, 128 */
#define DGREEN          0x03E0      /*   0, 128,   0 */
#define DCYAN           0x03EF      /*   0, 128, 128 */
#define MAROON          0x7800      /* 128,   0,   0 */
#define PURPLE          0x780F      /* 128,   0, 128 */
#define OLIVE           0x7BE0      /* 128, 128,   0 */
#define LGRAY           0xC618      /* 192, 192, 192 */
#define DGRAY           0x7BEF      /* 128, 128, 128 */
#define BLUE            0x001F      /*   0,   0, 255 */
#define GREEN           0x07E0      /*   0, 255,   0 */
#define CYAN            0x07FF      /*   0, 255, 255 */
#define RED             0xF800      /* 255,   0,   0 */
#define MAGENTA         0xF81F      /* 255,   0, 255 */
#define YELLOW          0xFFE0      /* 255, 255,   0 */
#define WHITE           0xFFFF      /* 255, 255, 255 */
#define ORANGE          0xFD20      /* 255, 165,   0 */
#define GREENYELLOW     0xAFE5      /* 173, 255,  47 */
#define BROWN 			0XBC40
#define BRRED           0XFC07

#define ILI9341_RESET               0x01
#define ILI9341_SLEEP_OUT           0x11
#define ILI9341_GAMMA               0x26
#define ILI9341_DISPLAY_OFF         0x28
#define ILI9341_DISPLAY_ON          0x29
#define ILI9341_COLUMN_ADDR         0x2A
#define ILI9341_PAGE_ADDR           0x2B
#define ILI9341_GRAM                0x2C
#define ILI9341_MAC                 0x36
#define ILI9341_PIXEL_FORMAT        0x3A
#define ILI9341_WDB                 0x51
#define ILI9341_WCD                 0x53
#define ILI9341_RGB_INTERFACE       0xB0
#define ILI9341_FRC                 0xB1
#define ILI9341_BPC                 0xB5
#define ILI9341_DFC                 0xB6
#define ILI9341_POWER1              0xC0
#define ILI9341_POWER2              0xC1
#define ILI9341_VCOM1               0xC5
#define ILI9341_VCOM2               0xC7
#define ILI9341_POWERA              0xCB
#define ILI9341_POWERB              0xCF
#define ILI9341_PGAMMA              0xE0
#define ILI9341_NGAMMA              0xE1
#define ILI9341_DTCA                0xE8
#define ILI9341_DTCB                0xEA
#define ILI9341_POWER_SEQ           0xED
#define ILI9341_3GAMMA_EN           0xF2
#define ILI9341_INTERFACE           0xF6
#define ILI9341_PRC                 0xF7

#define LCD_printf(...) fctprintf(&LCD_out, NULL, __VA_ARGS__)

void LCD_init(void);
void LCD_HWinit(void);
void LCD_SendCMD(uint8_t val);
void LCD_SendData(uint8_t val);
void LCD_SendPixel(uint16_t data);
void LCD_Sleep(void);
void LCD_SetWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void LCD_Fill(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void LCD_putchar(char chr);
void LCD_SendChar(uint8_t * start);
void LCD_Image_mono(tImage img, uint16_t x, uint16_t y);
void LCD_print(char * str);
void LCD_Out(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t * data); // todo dma
//void LCD_printf(const char *pFormat, ...);
void LCD_SetTextColor(uint16_t color);
void LCD_SetBGColor(uint16_t color);
void LCD_SetTextPos(uint16_t x, uint16_t y);
void LCD_SetFont(const tFont * fnt);

#endif
