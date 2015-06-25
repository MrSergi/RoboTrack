#pragma once

#define SELECT		0x01
#define DESELECT 	0x00

bool spiInit(void);
void spiSelect(bool select);
uint8_t spiTransferByte(uint8_t in);
