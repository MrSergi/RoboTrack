#include "conf.h"

// SPI1 Driver
// PA7     23      SPI1_MOSI
// PA6     22      SPI1_MISO
// PA5     21      SPI1_SCK
// PA4     20      SPI1_NSS

static bool spiTest(void);

bool spiInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef spi;

    // Enable SPI1 clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, ENABLE);

    // MOSI + SCK as output
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // MISO as input
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // NSS as gpio slave select
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Init SPI2 hardware
    SPI_I2S_DeInit(SPI1);
    spi.SPI_Mode = SPI_Mode_Master;
    spi.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    spi.SPI_DataSize = SPI_DataSize_8b;
    spi.SPI_NSS = SPI_NSS_Soft;
    spi.SPI_FirstBit = SPI_FirstBit_MSB;
    spi.SPI_CRCPolynomial = 7;
    spi.SPI_CPOL = SPI_CPOL_High;
    spi.SPI_CPHA = SPI_CPHA_2Edge;
    spi.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
    SPI_Init(SPI1, &spi);
    SPI_Cmd(SPI1, ENABLE);

    return spiTest();
}

void spiSelect(bool select)
{
    if (select) {
        digitalLo(GPIOA, GPIO_Pin_4);
    } else {
        digitalHi(GPIOA, GPIO_Pin_4);
    }
}

uint8_t spiTransferByte(uint8_t in)
{
    uint8_t rx;
    SPI1->DR;
    SPI1->DR = in;
    while (!(SPI1->SR & SPI_I2S_FLAG_RXNE));
    rx = SPI1->DR;
    while (!(SPI1->SR & SPI_I2S_FLAG_TXE));
    while (SPI1->SR & SPI_I2S_FLAG_BSY);
    return rx;
}

bool spiTransfer(uint8_t *out, uint8_t *in, int len)
{
    uint8_t b;
    SPI1->DR;
    while (len--) {
        b = in ? *(in++) : 0xFF;
        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
        SPI_I2S_SendData(SPI1, b);
        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
        b = SPI_I2S_ReceiveData(SPI1);
        if (out)
            *(out++) = b;
    }

    return true;
}

static bool spiTest(void)
{
    uint8_t out[] = { 0x9F, 0, 0, 0 };
    uint8_t in[4];

    spiSelect(true);
    spiTransfer(in, out, sizeof(out));
    spiSelect(false);
    if (in[1] == 0xEF)
        return true;

    return false;
}
