#ifndef __PINMUX_SETTING_H__
#define __PINMUX_SETTING_H__

#include "pinmux.h"

typedef struct {
	unsigned int index;
	unsigned int value;
}pinmux_table_t;

//uart3  ---  {STS0_CLK,      4},

pinmux_table_t pinmux_table[] = {
	{SDIO_CDn,      0},
	{SDIO_WP,       1},
	{SPI1_SS0n,     0},
	{SPI1_SS1n,     4},
	{SPI1_SS2n,     3},
	{SPI1_SS3n,     3},
	{SPI1_SDO,      0},
	{SPI1_SCLK,     0},
	{SPI1_SDI,      0},
	{TW0_SCL,       1},
	{TW0_SDA,       1},
	{STS0_CLK,      4},
	{STS0_SOP,      4},
	{STS0_SD,       4},
	{STS0_VALD,     4},
	{STS1_CLK,      0},
	{STS1_SOP,      0},
	{STS1_SD,       0},
	{STS1_VALD,     0},
	{USB2_DRV_VBUS, 1},

	{RGMII_MDC,     1}, // 0
	{RGMII_MDIO,    1}, // 0, lhl
	{RGMII_TXC,     0},
	{RGMII_TXD0,    0},
	{RGMII_TXD1,    0},
	{RGMII_TXD2,    0},
	{RGMII_TXD3,    0},
	{RGMII_TXCTL,   0},
	{RGMII_RXC,     0},
	{RGMII_RXD0,    0},
	{RGMII_RXD1,    0},
	{RGMII_RXD2,    0},
	{RGMII_RXD3,    0},
	{RGMII_RXCTL,   0},

	{I2S1_DO0,      1},
	{I2S1_DO1,      1},
	{I2S1_DO2,      1},
	{I2S1_DO3,      1},
	{I2S1_LRCKIO,   1},
	{I2S1_BCLKIO,   1},
	{SPDIFO,        1},
	{SPDIFI,        2},
	{I2S2_LRCKIO,   0},
	{I2S2_BCLKIO,   1},
	{I2S2_DI0,      0},
	{I2S2_DI1,      0},
	{I2S2_DI2,      0},
	{I2S2_DI3,      0},
	{I2S1_MCLK,     1},
	{I2S2_MCLK,     2},
	{TX_EDDC_SCL,   0},
	{TX_EDDC_SDA,   0},
	{I2S3_DO,       0},
	{I2S3_LRCKIO,   1},
	{I2S3_BCLKIO,   1},
	{I2S3_DI,       1},

	{SM_TW2_SCL,       0},
	{SM_TW2_SDA,       0},
	{SM_URT1_TXD,      6},
	{SM_URT1_RXD,      6},
	{SM_HDMI_HPD,      0},
	{SM_HDMI_CEC,      1},
	{SM_TMS,           1},
	{SM_TDI,           1},
	{SM_TDO,           1},
	{SM_TW3_SCL,       1},
	{SM_TW3_SDA,       1},
	{SM_SPI2_SS0n,     1},
	{SM_SPI2_SS1n,     0},
	{SM_SPI2_SS2n,     2},
	{SM_SPI2_SS3n,     2},
	{SM_SPI2_SDO,      1},
	{SM_SPI2_SDI,      1},
	{SM_SPI2_SCLK,     1},
	{SM_URT0_TXD,      0},
	{SM_URT0_RXD,      0},
	{SM_HDMIRX_HPD,    1},
	{SM_HDMIRX_PWR5V,  1},
};
#endif
