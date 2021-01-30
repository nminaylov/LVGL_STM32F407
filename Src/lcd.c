/* Функции для работы с дисплеем 240x320 (ILI9341) */

#include "LCD.h"
#include <math.h>
#include "printf.h"
#include <stdarg.h>

uint16_t BGColor = WHITE;
uint16_t TextColor = BLACK;

uint16_t Text_X = 0;
uint16_t Text_Y = 0;
uint16_t Text_X_start = 0;

tFont * Font;

void LCD_init(void)
{
    LCD_HWinit();
    LL_mDelay(5);
    LCD_RES_UP;
    LL_mDelay(5);
    LCD_SendCMD(ILI9341_RESET);
    LL_mDelay(10);

    LCD_SendCMD(ILI9341_POWERA);
    LCD_SendData(0x39);
    LCD_SendData(0x2C);
    LCD_SendData(0x00);
    LCD_SendData(0x34);
    LCD_SendData(0x02);

    LCD_SendCMD(ILI9341_POWERB);
    LCD_SendData(0x00);
    LCD_SendData(0xC1);
    LCD_SendData(0x30);

    LCD_SendCMD(ILI9341_DTCA);
    LCD_SendData(0x85);
    LCD_SendData(0x00);
    LCD_SendData(0x78);

    LCD_SendCMD(ILI9341_DTCB);
    LCD_SendData(0x00);
    LCD_SendData(0x00);

    LCD_SendCMD(ILI9341_POWER_SEQ);
    LCD_SendData(0x64);
    LCD_SendData(0x03);
    LCD_SendData(0x12);
    LCD_SendData(0x81);

    LCD_SendCMD(ILI9341_PRC);
    LCD_SendData(0x20);

    LCD_SendCMD(ILI9341_POWER1);
    LCD_SendData(0x23);

    LCD_SendCMD(ILI9341_POWER2);
    LCD_SendData(0x10);

    LCD_SendCMD(ILI9341_VCOM1);
    LCD_SendData(0x3E);
    LCD_SendData(0x28);

    LCD_SendCMD(ILI9341_VCOM2);
    LCD_SendData(0x86);

    LCD_SendCMD(ILI9341_MAC); // <--
    LCD_SendData(0x28); //28

    LCD_SendCMD(ILI9341_PIXEL_FORMAT);
    LCD_SendData(0x55);

    LCD_SendCMD(ILI9341_FRC);
    LCD_SendData(0x00);
    LCD_SendData(0x13);

    LCD_SendCMD(ILI9341_DFC);
    LCD_SendData(0x08);
    LCD_SendData(0x82);
    LCD_SendData(0x27);

    LCD_SendCMD(ILI9341_3GAMMA_EN);
    LCD_SendData(0x00);

//	LCD_SendCMD(ILI9341_COLUMN_ADDR);
//	LCD_SendData(0x00);
//	LCD_SendData(0x00);
//	LCD_SendData(0x00);
//	LCD_SendData(0xEF);
//
//	LCD_SendCMD(ILI9341_PAGE_ADDR);
//	LCD_SendData(0x00);
//	LCD_SendData(0x00);
//	LCD_SendData(0x01);
//	LCD_SendData(0x3F);

    LCD_SendCMD(ILI9341_GAMMA);
    LCD_SendData(0x01);

    LCD_SendCMD(ILI9341_PGAMMA);
    LCD_SendData(0x0F);
    LCD_SendData(0x31);
    LCD_SendData(0x2B);
    LCD_SendData(0x0C);
    LCD_SendData(0x0E);
    LCD_SendData(0x08);
    LCD_SendData(0x4E);
    LCD_SendData(0xF1);
    LCD_SendData(0x37);
    LCD_SendData(0x07);
    LCD_SendData(0x10);
    LCD_SendData(0x03);
    LCD_SendData(0x0E);
    LCD_SendData(0x09);
    LCD_SendData(0x00);

    LCD_SendCMD(ILI9341_NGAMMA);
    LCD_SendData(0x00);
    LCD_SendData(0x0E);
    LCD_SendData(0x14);
    LCD_SendData(0x03);
    LCD_SendData(0x11);
    LCD_SendData(0x07);
    LCD_SendData(0x31);
    LCD_SendData(0xC1);
    LCD_SendData(0x48);
    LCD_SendData(0x08);
    LCD_SendData(0x0F);
    LCD_SendData(0x0C);
    LCD_SendData(0x31);
    LCD_SendData(0x36);
    LCD_SendData(0x0F);

    LCD_SendCMD(ILI9341_SLEEP_OUT);

    LL_mDelay(10);
    LCD_SendCMD(ILI9341_DISPLAY_ON);

    LCD_Fill(0, 0, LCD_W, LCD_H, BLACK);
    LL_mDelay(100);

    LCD_BL_ON;

    LCD_SetFont(&t_12x24_full);
    LCD_SetBGColor(BLACK);
    LCD_SetTextColor(WHITE);
}

void LCD_HWinit(void) // Настраиваем пины, SPI
{
    LL_SPI_InitTypeDef SPI_InitStruct;
    LL_GPIO_InitTypeDef GPIO_InitStruct;

    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);

    LCD_DC_DN;
    LCD_BL_OFF;
    LCD_CS_UP;
    LCD_RES_DN;
    GPIO_InitStruct.Pin = LL_GPIO_PIN_4 | LL_GPIO_PIN_6 | LL_GPIO_PIN_7; // CS, DC, BL
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LL_GPIO_PIN_7; // RES
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    LL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LL_GPIO_PIN_3 | LL_GPIO_PIN_5; // SCL, SDA
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    SPI_InitStruct.TransferDirection = LL_SPI_HALF_DUPLEX_TX;
    SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
    SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
    SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW;
    SPI_InitStruct.ClockPhase = LL_SPI_PHASE_1EDGE;
    SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
    SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV2;
    SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
    SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
    SPI_InitStruct.CRCPoly = 10;
    LL_SPI_Init(SPI1, &SPI_InitStruct);

    LL_SPI_Enable(SPI1);
}

void LCD_SendCMD(uint8_t val)
{
    while (!LL_SPI_IsActiveFlag_TXE(SPI1))
        ; // Ждем пока освободится буфер
    LCD_DC_DN; // A0 = 0 - CMD
    LCD_CS_DN;
    LL_SPI_TransmitData8(SPI1, val);
    for (uint8_t i = 0; i < 100; i++)
    {
        if (LL_SPI_IsActiveFlag_BSY(SPI1)) // Ждем начала передачи
            break;
    }
    while (LL_SPI_IsActiveFlag_BSY(SPI1))
        ; // Ждем окончание передачи
    LCD_CS_UP;
}

void LCD_SendData(uint8_t val)
{
    while (!LL_SPI_IsActiveFlag_TXE(SPI1))
        ; // Ждем пока освободится буфер
    LCD_DC_UP; // A0 = 1 - DATA
    LCD_CS_DN;
    LL_SPI_TransmitData8(SPI1, val);
    for (uint8_t i = 0; i < 100; i++)
    {
        if (LL_SPI_IsActiveFlag_BSY(SPI1)) // Ждем начала передачи
            break;
    }
    while (LL_SPI_IsActiveFlag_BSY(SPI1))
        ; // Ждем окончание передачи
    LCD_CS_UP;
}

void LCD_SendPixel(uint16_t data) // Шлём 2 байта (данные)
{
    while (!LL_SPI_IsActiveFlag_TXE(SPI1))
        ;
    LL_SPI_TransmitData8(SPI1, (data >> 8));
    while (!LL_SPI_IsActiveFlag_TXE(SPI1))
        ;
    LL_SPI_TransmitData8(SPI1, (data & 0xFF));
}

// Установка окна x, y - координаты начала; w, h - ширина, высота окна
void LCD_SetWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    LCD_SendCMD(ILI9341_COLUMN_ADDR); // CASET
    LCD_SendData(x >> 8);
    LCD_SendData(x & 0xFF);
    LCD_SendData((x + w - 1) >> 8);
    LCD_SendData((x + w - 1) & 0xFF);
    LCD_SendCMD(ILI9341_PAGE_ADDR); // RASET
    LCD_SendData(y >> 8);
    LCD_SendData(y & 0xFF);
    LCD_SendData((y + h - 1) >> 8);
    LCD_SendData((y + h - 1) & 0xFF);
    LCD_SendCMD(ILI9341_GRAM); // RAMWR
}

// Заполняем область цветом
void LCD_Fill(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) // todo заливка через дма
{
    uint32_t count = w * h;
    LCD_SetWindow(x, y, w, h);
    LCD_DC_UP;
    LCD_CS_DN;
    while (count--)
        LCD_SendPixel(color);
    while (LL_SPI_IsActiveFlag_BSY(SPI1))
        ; // Ждем окончание передачи
    LCD_CS_UP;
}

void LCD_Out(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t * data) // todo dma
{
    uint16_t count = w * h;
    uint32_t ptr = 0;
    LCD_SetWindow(x, y, w, h);
    LCD_DC_UP;
    LCD_CS_DN;
    while (count--)
        LCD_SendPixel(data[ptr++]);
    while (LL_SPI_IsActiveFlag_BSY(SPI1))
        ;
    LCD_CS_UP;
}

void LCD_putchar(char chr)
{
    LCD_SetWindow(Text_X, Text_Y, Font->char_w, Font->char_h);
    LCD_DC_UP;
    LCD_CS_DN;
    if (chr == '\n') // Если символ переноса
    {
#ifdef  SAVE_X_OFFSET
        Text_X = Text_X_start;
#else
        Text_X = 0;
#endif
        Text_Y += Font->char_h;
        chr = 0;
    }
    else
    {
        Text_X += Font->char_w;
        if (Text_X > (LCD_W - Font->char_w))
        {
#ifdef  SAVE_X_OFFSET
            Text_X = Text_X_start;
#else
            Text_X = 0;
#endif
            Text_Y += Font->char_h;
        }
    }
    if (Text_Y > (LCD_H - Font->char_h))
        Text_Y = 0;

    if (chr < Font->offset)
        chr = Font->offset;

    uint8_t * chardata = NULL;

    if (Font->type == 0)
    {
        if (chr < Font->offset)
            chr = Font->offset;
        chardata = (uint8_t *) &(Font->data[(Font->char_w * Font->char_h / 8) * (chr - Font->offset)]);
    }
    if (Font->type == 1)
    {
        if (chr < Font->offset)
            chr = Font->offset;
        chardata = (uint8_t *) &(Font->data[(Font->char_w - 1) * (chr - Font->offset)]);
    }
    LCD_SendChar(chardata);
    while (LL_SPI_IsActiveFlag_BSY(SPI1))
        ; // Ждем окончание передачи
    LCD_CS_UP;
}

void LCD_SendChar(uint8_t * start)
{
    if (Font->type == 0)
    {
        uint8_t len = Font->char_w * Font->char_h / 8;
        for (uint8_t byte_cnt = 0; byte_cnt < len; byte_cnt++)
        {
            for (uint8_t bit_cnt = 0; bit_cnt < 8; bit_cnt++)
            {
                if ((start[byte_cnt] << bit_cnt) & 0x80)
                    LCD_SendPixel(TextColor);
                else
                    LCD_SendPixel(BGColor);
            }
        }
    }
    if (Font->type == 1)
    {
        for (uint8_t bit_cnt = 0; bit_cnt < 8; bit_cnt++)
        {
            for (uint8_t byte_cnt = 0; byte_cnt < (Font->char_w - 1); byte_cnt++)
            {
                if ((start[byte_cnt] >> bit_cnt) & 0x01)
                    LCD_SendPixel(TextColor);
                else
                    LCD_SendPixel(BGColor);
            }
            LCD_SendPixel(BGColor);
        }
    }
}

// Вывод монохромного изображения
void LCD_Image_mono(tImage img, uint16_t x, uint16_t y)
{
    LCD_SetWindow(x, y, img.width, img.height);
    LCD_DC_UP;
    LCD_CS_DN;
    for (uint16_t byte_cnt = 0; byte_cnt < img.width * img.width / 8; byte_cnt++)
    {
        for (uint8_t bit_cnt = 0; bit_cnt < 8; bit_cnt++)
        {
            if ((img.data[byte_cnt] << bit_cnt) & 0x80)
                LCD_SendPixel(TextColor);
            else
                LCD_SendPixel(BGColor);
        }
    }
    while (LL_SPI_IsActiveFlag_BSY(SPI1))
        ; // Ждем окончание передачи
    LCD_CS_UP;
}

void LCD_print(char * str)
{
    //Text_X_start = Text_X;
    while (*str)
        LCD_putchar(*str++);
}

inline void LCD_out(char character, void* arg)
{
    LCD_putchar(character);
}

inline void LCD_SetTextColor(uint16_t color)
{
    TextColor = color;
}

inline void LCD_SetBGColor(uint16_t color)
{
    BGColor = color;
}

inline void LCD_SetTextPos(uint16_t x, uint16_t y)
{
    Text_X = x;
    Text_Y = y;
    Text_X_start = Text_X;
}

inline void LCD_SetFont(const tFont * fnt)
{
    Font = (tFont *) fnt;
}
