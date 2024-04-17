// SPDX-License-Identifier: GPL-2.0
/*
 * Synaptics VS680 pinctrl driver
 *
 * Copyright (C) 2019 Synaptics Incorporated
 *
 * Author: Jisheng Zhang <jszhang@kernel.org>
 */

#include <dm/device.h>
#include <dm/pinctrl.h>

#include "pinctrl-synaptics.h"

static const struct synaptics_desc_group vs680_soc_pinctrl_groups[] = {
	SYNAPTICS_PINCTRL_GROUP("SDIO_CDn", 0x0, 0x3, 0x00,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "sdio"), /* CDn */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "gpio"), /* GPIO45 */
			SYNAPTICS_PINCTRL_FUNCTION(0x2, "tw1")), /* SCL */
	SYNAPTICS_PINCTRL_GROUP("SDIO_WP", 0x0, 0x3, 0x03,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "sdio"), /* WP */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "gpio"), /* GPIO44 */
			SYNAPTICS_PINCTRL_FUNCTION(0x2, "tw1")), /* SDA */
	SYNAPTICS_PINCTRL_GROUP("SPI1_SS0n", 0x0, 0x3, 0x06,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "spi1"), /* SS0n */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "gpio")), /* GPIO54 */
	SYNAPTICS_PINCTRL_GROUP("SPI1_SS1n", 0x0, 0x3, 0x09,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO53 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "spi1"), /* SS1n */
			SYNAPTICS_PINCTRL_FUNCTION(0x4, "pwm"), /* PWM1 */
			SYNAPTICS_PINCTRL_FUNCTION(0x7, "phy")), /* DBG14 */
	SYNAPTICS_PINCTRL_GROUP("SPI1_SS2n", 0x0, 0x3, 0x0c,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO52 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "spi1"), /* SS2n */
			SYNAPTICS_PINCTRL_FUNCTION(0x2, "sts7"), /* CLK */
			SYNAPTICS_PINCTRL_FUNCTION(0x4, "pwm"), /* PWM0 */
			SYNAPTICS_PINCTRL_FUNCTION(0x7, "phy")), /* DBG12 */
	SYNAPTICS_PINCTRL_GROUP("SPI1_SS3n", 0x0, 0x3, 0x0f,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO51 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "spi1"), /* SS3n */
			SYNAPTICS_PINCTRL_FUNCTION(0x2, "sts7"), /* SD */
			SYNAPTICS_PINCTRL_FUNCTION(0x7, "phy")), /* DBG13 */
	SYNAPTICS_PINCTRL_GROUP("SPI1_SDO", 0x0, 0x3, 0x12,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "spi1"), /* SDO */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "gpio")), /* GPIO50 */
	SYNAPTICS_PINCTRL_GROUP("SPI1_SCLK", 0x0, 0x3, 0x15,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "spi1"), /* SCLK */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "gpio")), /* GPIO49 */
	SYNAPTICS_PINCTRL_GROUP("SPI1_SDI", 0x0, 0x3, 0x18,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "spi1"), /* SDI */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "gpio")), /* GPIO48 */
	SYNAPTICS_PINCTRL_GROUP("TW0_SCL", 0x0, 0x3, 0x1b,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO47 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "tw0"), /* SCL */
			SYNAPTICS_PINCTRL_FUNCTION(0x7, "phy")), /* DBG10 */
	SYNAPTICS_PINCTRL_GROUP("TW0_SDA", 0x4, 0x3, 0x00,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO46 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "tw0"), /* SDA */
			SYNAPTICS_PINCTRL_FUNCTION(0x7, "phy")), /* DBG11 */
	SYNAPTICS_PINCTRL_GROUP("STS0_CLK", 0x4, 0x3, 0x03,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO43 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "sts0"), /* CLK */
			SYNAPTICS_PINCTRL_FUNCTION(0x2, "cpupll"), /* CLKO */
			SYNAPTICS_PINCTRL_FUNCTION(0x4, "uart3"), /* RXD */
			SYNAPTICS_PINCTRL_FUNCTION(0x7, "phy")), /* DBG0 */
	SYNAPTICS_PINCTRL_GROUP("STS0_SOP", 0x4, 0x3, 0x06,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO42 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "sts0"), /* SOP */
			SYNAPTICS_PINCTRL_FUNCTION(0x2, "syspll"), /* CLKO */
			SYNAPTICS_PINCTRL_FUNCTION(0x3, "sts5"), /* CLK */
			SYNAPTICS_PINCTRL_FUNCTION(0x4, "uart3"), /* TXD */
			SYNAPTICS_PINCTRL_FUNCTION(0x7, "phy")), /* DBG1 */
	SYNAPTICS_PINCTRL_GROUP("STS0_SD", 0x4, 0x3, 0x09,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO41 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "sts0"), /* SD */
			SYNAPTICS_PINCTRL_FUNCTION(0x2, "mempll"), /* CLKO */
			SYNAPTICS_PINCTRL_FUNCTION(0x4, "uart3"), /* CTSn */
			SYNAPTICS_PINCTRL_FUNCTION(0x7, "phy")), /* DBG2 */
	SYNAPTICS_PINCTRL_GROUP("STS0_VALD", 0x4, 0x3, 0x0c,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO40 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "sts0"), /* VALD */
			SYNAPTICS_PINCTRL_FUNCTION(0x3, "sts5"), /* SD */
			SYNAPTICS_PINCTRL_FUNCTION(0x4, "uart3"), /* RTSn */
			SYNAPTICS_PINCTRL_FUNCTION(0x7, "phy")), /* DBG3 */
	SYNAPTICS_PINCTRL_GROUP("STS1_CLK", 0x4, 0x3, 0x0f,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO39 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "sts1"), /* CLK */
			SYNAPTICS_PINCTRL_FUNCTION(0x2, "pwm"), /* pwm0 */
			SYNAPTICS_PINCTRL_FUNCTION(0x7, "phy")), /* DBG4 */
	SYNAPTICS_PINCTRL_GROUP("STS1_SOP", 0x4, 0x3, 0x12,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO38 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "sts1"), /* SOP */
			SYNAPTICS_PINCTRL_FUNCTION(0x2, "pwm"), /* PWM1 */
			SYNAPTICS_PINCTRL_FUNCTION(0x3, "sts6"), /* CLK */
			SYNAPTICS_PINCTRL_FUNCTION(0x7, "phy")), /* DBG5 */
	SYNAPTICS_PINCTRL_GROUP("STS1_SD", 0x4, 0x3, 0x15,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO37 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "sts1"), /* SD */
			SYNAPTICS_PINCTRL_FUNCTION(0x2, "pwm"), /* PWM2 */
			SYNAPTICS_PINCTRL_FUNCTION(0x7, "phy")), /* DBG6 */
	SYNAPTICS_PINCTRL_GROUP("STS1_VALD", 0x4, 0x3, 0x18,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO36 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "sts1"), /* VALD */
			SYNAPTICS_PINCTRL_FUNCTION(0x2, "pwm"), /* PWM3 */
			SYNAPTICS_PINCTRL_FUNCTION(0x3, "sts6"), /* SD */
			SYNAPTICS_PINCTRL_FUNCTION(0x7, "phy")), /* DBG7 */
	SYNAPTICS_PINCTRL_GROUP("USB2_DRV_VBUS", 0x4, 0x3, 0x1b,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "usb2"), /* VBUS */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "gpio")), /* GPIO55 */
	SYNAPTICS_PINCTRL_GROUP("RGMII_MDC", 0x8, 0x3, 0x00,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "rgmii"), /* MDC */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "gpio"), /* GPIO29 */
			SYNAPTICS_PINCTRL_FUNCTION(0x7, "phy")), /* DBG8 */
	SYNAPTICS_PINCTRL_GROUP("RGMII_MDIO", 0x8, 0x3, 0x03,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "rgmii"), /* MDIO */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "gpio"), /* GPIO28 */
			SYNAPTICS_PINCTRL_FUNCTION(0x7, "phy")), /* DBG9 */
	SYNAPTICS_PINCTRL_GROUP("RGMII_TXC", 0x8, 0x3, 0x06,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "rgmii"), /* TXC */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "gpio")), /* GPIO23 */
	SYNAPTICS_PINCTRL_GROUP("RGMII_TXD0", 0x8, 0x3, 0x09,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "rgmii"), /* TXD0 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "gpio")), /* GPIO27 */
	SYNAPTICS_PINCTRL_GROUP("RGMII_TXD1", 0x8, 0x3, 0x0c,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "rgmii"), /* TXD1 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "gpio"), /* GPIO26 */
			SYNAPTICS_PINCTRL_FUNCTION(0x7, "phy")), /* DBG15 */
	SYNAPTICS_PINCTRL_GROUP("RGMII_TXD2", 0x8, 0x3, 0x0f,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "rgmii"), /* TXD2 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "gpio")), /* GPIO25 */
	SYNAPTICS_PINCTRL_GROUP("RGMII_TXD3", 0x8, 0x3, 0x12,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "rgmii"), /* TXD3 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "gpio")), /* GPIO24 */
	SYNAPTICS_PINCTRL_GROUP("RGMII_TXCTL", 0x8, 0x3, 0x15,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "rgmii"), /* TXCTL */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "gpio")), /* GPIO22 */
	SYNAPTICS_PINCTRL_GROUP("RGMII_RXC", 0x8, 0x3, 0x18,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "rgmii"), /* RXC */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "gpio")), /* GPIO31 */
	SYNAPTICS_PINCTRL_GROUP("RGMII_RXD0", 0x8, 0x3, 0x1b,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "rgmii"), /* RXD0 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "gpio")), /* GPIO35 */
	SYNAPTICS_PINCTRL_GROUP("RGMII_RXD1", 0xc, 0x3, 0x00,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "rgmii"), /* RXD1 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "gpio")), /* GPIO34 */
	SYNAPTICS_PINCTRL_GROUP("RGMII_RXD2", 0xc, 0x3, 0x03,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "rgmii"), /* RXD2 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "gpio")), /* GPIO33 */
	SYNAPTICS_PINCTRL_GROUP("RGMII_RXD3", 0xc, 0x3, 0x06,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "rgmii"), /* RXD3 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "gpio")), /* GPIO32 */
	SYNAPTICS_PINCTRL_GROUP("RGMII_RXCTL", 0xc, 0x3, 0x09,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "rgmii"), /* RXCTL */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "gpio")), /* GPIO30 */
};

static const struct synaptics_desc_group vs680_avio_pinctrl_groups[] = {
	SYNAPTICS_PINCTRL_GROUP("I2S1_DO0", 0x0, 0x3, 0x00,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO19 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "i2s1"), /* DO0 */
			SYNAPTICS_PINCTRL_FUNCTION(0x7, "avio")), /* DBG4 */
	SYNAPTICS_PINCTRL_GROUP("I2S1_DO1", 0x0, 0x3, 0x03,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO17 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "i2s1"), /* DO1 */
			SYNAPTICS_PINCTRL_FUNCTION(0x3, "sts2"), /* CLK */
			SYNAPTICS_PINCTRL_FUNCTION(0x7, "avio")), /* DBG5 */
	SYNAPTICS_PINCTRL_GROUP("I2S1_DO2", 0x0, 0x3, 0x06,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO16 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "i2s1"), /* DO2 */
			SYNAPTICS_PINCTRL_FUNCTION(0x2, "pwm"), /* PWM2 */
			SYNAPTICS_PINCTRL_FUNCTION(0x3, "sts2"), /* SD */
			SYNAPTICS_PINCTRL_FUNCTION(0x4, "pdm"), /* DI2 */
			SYNAPTICS_PINCTRL_FUNCTION(0x7, "avio")), /* DBG6 */
	SYNAPTICS_PINCTRL_GROUP("I2S1_DO3", 0x0, 0x3, 0x09,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO15 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "i2s1"), /* DO3 */
			SYNAPTICS_PINCTRL_FUNCTION(0x2, "pwm"), /* PWM3 */
			SYNAPTICS_PINCTRL_FUNCTION(0x3, "sts3"), /* CLK */
			SYNAPTICS_PINCTRL_FUNCTION(0x4, "pdm"), /* DI3 */
			SYNAPTICS_PINCTRL_FUNCTION(0x7, "avio")), /* DBG7 */
	SYNAPTICS_PINCTRL_GROUP("I2S1_LRCKIO", 0x0, 0x3, 0x0c,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO21 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "i2s1"), /* LRCKIO */
			SYNAPTICS_PINCTRL_FUNCTION(0x2, "pwm"), /* PWM0 */
			SYNAPTICS_PINCTRL_FUNCTION(0x3, "arc_test"), /* OUT */
			SYNAPTICS_PINCTRL_FUNCTION(0x7, "avio")), /* DBG0 */
	SYNAPTICS_PINCTRL_GROUP("I2S1_BCLKIO", 0x0, 0x3, 0x0f,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO20 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "i2s1"), /* BCLKIO */
			SYNAPTICS_PINCTRL_FUNCTION(0x2, "pwm"), /* PWM1 */
			SYNAPTICS_PINCTRL_FUNCTION(0x7, "avio")), /* DBG1 */
	SYNAPTICS_PINCTRL_GROUP("SPDIFO", 0x0, 0x3, 0x12,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO14 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "spdifo"),
			SYNAPTICS_PINCTRL_FUNCTION(0x4, "avpll")), /* CLKO */
	SYNAPTICS_PINCTRL_GROUP("SPDIFI", 0x0, 0x3, 0x15,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO4 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "spdifi"),
			SYNAPTICS_PINCTRL_FUNCTION(0x2, "pdm")), /* DI */
	SYNAPTICS_PINCTRL_GROUP("I2S2_LRCKIO", 0x0, 0x3, 0x18,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO13 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "i2s2")), /* LRCKIO */
	SYNAPTICS_PINCTRL_GROUP("I2S2_BCLKIO", 0x0, 0x3, 0x1b,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO12 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "i2s2"), /* BCLKIO */
			SYNAPTICS_PINCTRL_FUNCTION(0x2, "pdm")), /* CLKIO */
	SYNAPTICS_PINCTRL_GROUP("I2S2_DI0", 0x4, 0x3, 0x00,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO11 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "i2s2"), /* DI0 */
			SYNAPTICS_PINCTRL_FUNCTION(0x2, "pdm")), /* DI3 */
	SYNAPTICS_PINCTRL_GROUP("I2S2_DI1", 0x4, 0x3, 0x03,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO10 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "i2s2"), /* DI1 */
			SYNAPTICS_PINCTRL_FUNCTION(0x2, "pdm"), /* DI2 */
			SYNAPTICS_PINCTRL_FUNCTION(0x3, "sts3")), /* SD */
	SYNAPTICS_PINCTRL_GROUP("I2S2_DI2", 0x4, 0x3, 0x06,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO9 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "i2s2"), /* DI2 */
			SYNAPTICS_PINCTRL_FUNCTION(0x2, "pdm"), /* DI1 */
			SYNAPTICS_PINCTRL_FUNCTION(0x3, "sts4")), /* CLK */
	SYNAPTICS_PINCTRL_GROUP("I2S2_DI3", 0x4, 0x3, 0x09,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO8 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "i2s2"), /* DI3 */
			SYNAPTICS_PINCTRL_FUNCTION(0x2, "pdm"), /* DI0 */
			SYNAPTICS_PINCTRL_FUNCTION(0x3, "sts4")), /* SD */
	SYNAPTICS_PINCTRL_GROUP("I2S1_MCLK", 0x4, 0x3, 0x0c,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO18 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "i2s1"), /* MCLK */
			SYNAPTICS_PINCTRL_FUNCTION(0x7, "avio")), /* DBG3 */
	SYNAPTICS_PINCTRL_GROUP("I2S2_MCLK", 0x4, 0x3, 0x0f,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO7 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "i2s2"), /* MCLK */
			SYNAPTICS_PINCTRL_FUNCTION(0x2, "pdm"), /* CLKIO */
			SYNAPTICS_PINCTRL_FUNCTION(0x4, "hdmi")), /* FBCLK */
	SYNAPTICS_PINCTRL_GROUP("TX_EDDC_SCL", 0x4, 0x3, 0x12,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO6 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "tx_eddc")), /* SCL */
	SYNAPTICS_PINCTRL_GROUP("TX_EDDC_SDA", 0x4, 0x3, 0x15,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO5 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "tx_eddc")), /* SDA */
	SYNAPTICS_PINCTRL_GROUP("I2S3_DO", 0x4, 0x3, 0x18,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO1 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "i2s3"), /* DO */
			SYNAPTICS_PINCTRL_FUNCTION(0x7, "avio")), /* DBG2 */
	SYNAPTICS_PINCTRL_GROUP("I2S3_LRCKIO", 0x4, 0x3, 0x1b,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO3 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "i2s3")), /* LRCKIO */
	SYNAPTICS_PINCTRL_GROUP("I2S3_BCLKIO", 0x8, 0x3, 0x00,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO2 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "i2s3")), /* BCLKIO */
	SYNAPTICS_PINCTRL_GROUP("I2S3_DI", 0x8, 0x3, 0x03,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO0 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "i2s3")), /* DI */
};

static const struct synaptics_desc_group vs680_sysmgr_pinctrl_groups[] = {
	SYNAPTICS_PINCTRL_GROUP("SM_TW2_SCL", 0x0, 0x3, 0x00,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "rx_edid"), /* SCL */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "tw2"), /* SCL */
			SYNAPTICS_PINCTRL_FUNCTION(0x2, "gpio")), /* SM GPIO0 */
	SYNAPTICS_PINCTRL_GROUP("SM_TW2_SDA", 0x0, 0x3, 0x03,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "rx_edid"), /* SDA */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "tw2"), /* SDA */
			SYNAPTICS_PINCTRL_FUNCTION(0x2, "gpio")), /* SM GPIO1 */
	SYNAPTICS_PINCTRL_GROUP("SM_URT1_TXD", 0x0, 0x3, 0x06,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "porb"), /* VOUT 1p05 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "uart1"), /* TXD */
			SYNAPTICS_PINCTRL_FUNCTION(0x2, "gpio"), /* SM GPIO4 */
			SYNAPTICS_PINCTRL_FUNCTION(0x3, "pwm"), /* PWM2 */
			SYNAPTICS_PINCTRL_FUNCTION(0x4, "timer"), /* TIMER0 */
			SYNAPTICS_PINCTRL_FUNCTION(0x5, "porb")), /* AVDD LV */
	SYNAPTICS_PINCTRL_GROUP("SM_URT1_RXD", 0x0, 0x3, 0x09,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "gpio"), /* SM GPIO5 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "uart1"), /* RXD */
			SYNAPTICS_PINCTRL_FUNCTION(0x2, "clk_25m"),
			SYNAPTICS_PINCTRL_FUNCTION(0x3, "pwm"), /* PWM3 */
			SYNAPTICS_PINCTRL_FUNCTION(0x4, "timer"), /* TIMER1 */
			SYNAPTICS_PINCTRL_FUNCTION(0x5, "por")), /* VDDSOC_RSTB */
	SYNAPTICS_PINCTRL_GROUP("SM_HDMI_HPD", 0x0, 0x3, 0x0c,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "gpio"), /* SM GPIO2 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "hdmi")), /* HPD */
	SYNAPTICS_PINCTRL_GROUP("SM_HDMI_CEC", 0x0, 0x3, 0x0f,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "gpio"), /* SM GPIO3 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "hdmi")), /* CEC */
	SYNAPTICS_PINCTRL_GROUP("SM_TMS", 0x0, 0x3, 0x12,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "jtag"), /* TMS */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "gpio"), /* SM GPIO6 */
			SYNAPTICS_PINCTRL_FUNCTION(0x2, "pwm")), /* PWM0 */
	SYNAPTICS_PINCTRL_GROUP("SM_TDI", 0x0, 0x3, 0x15,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "jtag"), /* TDI */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "gpio"), /* SM GPIO7 */
			SYNAPTICS_PINCTRL_FUNCTION(0x2, "pwm")), /* PWM1 */
	SYNAPTICS_PINCTRL_GROUP("SM_TDO", 0x0, 0x3, 0x18,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "jtag"), /* TDO */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "gpio")), /* SM GPIO8 */
	SYNAPTICS_PINCTRL_GROUP("SM_TW3_SCL", 0x0, 0x3, 0x1b,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "gpio"), /* SM GPIO9 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "tw3"), /* SCL */
			SYNAPTICS_PINCTRL_FUNCTION(0x2, "pwm")), /* PWM2 */
	SYNAPTICS_PINCTRL_GROUP("SM_TW3_SDA", 0x4, 0x3, 0x00,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "gpio"), /* SM GPIO10 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "tw3"), /* SDA */
			SYNAPTICS_PINCTRL_FUNCTION(0x2, "pwm")), /* PWM3 */
	SYNAPTICS_PINCTRL_GROUP("SM_SPI2_SS0n", 0x4, 0x3, 0x03,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "spi2"), /* SS0 n*/
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "gpio"), /* SM GPIO17 */
			SYNAPTICS_PINCTRL_FUNCTION(0x7, "porb")), /* AVDD33_LV */
	SYNAPTICS_PINCTRL_GROUP("SM_SPI2_SS1n", 0x4, 0x3, 0x06,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "gpio"), /* SM GPIO16 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "spi2"), /* SS1n */
			SYNAPTICS_PINCTRL_FUNCTION(0x6, "uart1"), /* RTSn */
			SYNAPTICS_PINCTRL_FUNCTION(0x7, "vdd")), /* CPU PORTB */
	SYNAPTICS_PINCTRL_GROUP("SM_SPI2_SS2n", 0x4, 0x3, 0x09,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "mon"), /* VDD 1P8 */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "spi2"), /* SS2n */
			SYNAPTICS_PINCTRL_FUNCTION(0x2, "gpio"), /* SM GPIO15 */
			SYNAPTICS_PINCTRL_FUNCTION(0x3, "pwm"), /* PWM0 */
			SYNAPTICS_PINCTRL_FUNCTION(0x4, "timer"), /* TIMER0 */
			SYNAPTICS_PINCTRL_FUNCTION(0x5, "uart2"), /* TXD */
			SYNAPTICS_PINCTRL_FUNCTION(0x7, "clk_25m")),
	SYNAPTICS_PINCTRL_GROUP("SM_SPI2_SS3n", 0x4, 0x3, 0x0c,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "pwr_ok"),
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "spi2"), /* SS3n */
			SYNAPTICS_PINCTRL_FUNCTION(0x2, "gpio"), /* SM GPIO14 */
			SYNAPTICS_PINCTRL_FUNCTION(0x3, "pwm"), /* PWM1 */
			SYNAPTICS_PINCTRL_FUNCTION(0x4, "timer"), /* TIMER1 */
			SYNAPTICS_PINCTRL_FUNCTION(0x5, "uart2"), /* RXD */
			SYNAPTICS_PINCTRL_FUNCTION(0x7, "uart1")), /* CTSn */
	SYNAPTICS_PINCTRL_GROUP("SM_SPI2_SDO", 0x4, 0x3, 0x0f,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "spi2"), /* SDO */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "gpio"), /* SM GPIO13 */
			SYNAPTICS_PINCTRL_FUNCTION(0x2, "uart2")), /* RTSn */
	SYNAPTICS_PINCTRL_GROUP("SM_SPI2_SDI", 0x4, 0x3, 0x12,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "spi2"), /* SDI */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "gpio"), /* SM GPIO12 */
			SYNAPTICS_PINCTRL_FUNCTION(0x2, "uart2")), /* CTSn */
	SYNAPTICS_PINCTRL_GROUP("SM_SPI2_SCLK", 0x4, 0x3, 0x15,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "spi2"), /* SCLK */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "gpio")), /* SM GPIO11 */
	SYNAPTICS_PINCTRL_GROUP("SM_URT0_TXD", 0x4, 0x3, 0x18,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "uart0"), /* TXD */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "gpio")), /* SM GPIO19 */
	SYNAPTICS_PINCTRL_GROUP("SM_URT0_RXD", 0x4, 0x3, 0x1b,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "uart0"), /* RXD */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "gpio")), /* SM GPIO19 */
	SYNAPTICS_PINCTRL_GROUP("SM_HDMIRX_HPD", 0x8, 0x3, 0x00,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "hdmirx"), /* HPD */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "gpio")), /* SM GPIO20 */
	SYNAPTICS_PINCTRL_GROUP("SM_HDMIRX_PWR5V", 0x8, 0x3, 0x03,
			SYNAPTICS_PINCTRL_FUNCTION(0x0, "hdmirx"), /* PWR5V */
			SYNAPTICS_PINCTRL_FUNCTION(0x1, "gpio")), /* SM GPIO21 */
};

static int vs680_pinctrl_probe(struct udevice *dev)
{
	const struct synaptics_pinctrl_desc *desc = (struct synaptics_pinctrl_desc *)dev_get_driver_data(dev);
	return synaptics_pinctrl_probe(dev, desc);
}

static const struct synaptics_pinctrl_desc vs680_soc_pinctrl_data = {
	.groups = vs680_soc_pinctrl_groups,
	.ngroups = ARRAY_SIZE(vs680_soc_pinctrl_groups),
};

static const struct synaptics_pinctrl_desc vs680_avio_pinctrl_data = {
	.groups = vs680_avio_pinctrl_groups,
	.ngroups = ARRAY_SIZE(vs680_avio_pinctrl_groups),
};

static const struct synaptics_pinctrl_desc vs680_sysmgr_pinctrl_data = {
	.groups = vs680_sysmgr_pinctrl_groups,
	.ngroups = ARRAY_SIZE(vs680_sysmgr_pinctrl_groups),
};

static const struct udevice_id vs680_pinctrl_match[] = {
	{
		.compatible = "syna,vs680-soc-pinctrl",
		.data = (ulong)&vs680_soc_pinctrl_data,
	},
	{
		.compatible = "syna,vs680-avio-pinctrl",
		.data = (ulong)&vs680_avio_pinctrl_data,
	},
	{
		.compatible = "syna,vs680-sysmgr-pinctrl",
		.data = (ulong)&vs680_sysmgr_pinctrl_data,
	},
	{}
};

U_BOOT_DRIVER(vs680_pinctrl) = {
	.name		= "vs680-pinctrl",
	.id		= UCLASS_PINCTRL,
	.of_match	= vs680_pinctrl_match,
	.ops		= &synaptics_pinctrl_ops,
	.probe		= vs680_pinctrl_probe,
	.priv_auto_alloc_size = sizeof(struct synaptics_pinctrl),
};