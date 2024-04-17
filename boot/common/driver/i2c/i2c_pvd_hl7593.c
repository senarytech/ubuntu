/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright � 2013-2020 Synaptics Incorporated. All rights reserved.
 *
 * This file contains information that is proprietary to Synaptics
 * Incorporated ("Synaptics"). The holder of this file shall treat all
 * information contained herein as confidential, shall use the
 * information only for its intended purpose, and shall not duplicate,
 * disclose, or disseminate any of this information in any manner
 * unless Synaptics has otherwise provided express, written
 * permission.
 *
 * Use of the materials may require a license of intellectual property
 * from a third party or from Synaptics. This file conveys no express
 * or implied licenses to any intellectual property rights belonging
 * to Synaptics.
 *
 * INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS," AND
 * SYNAPTICS EXPRESSLY DISCLAIMS ALL EXPRESS AND IMPLIED WARRANTIES,
 * INCLUDING ANY IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, AND ANY WARRANTIES OF NON-INFRINGEMENT OF ANY
 * INTELLECTUAL PROPERTY RIGHTS. IN NO EVENT SHALL SYNAPTICS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, PUNITIVE, OR
 * CONSEQUENTIAL DAMAGES ARISING OUT OF OR IN CONNECTION WITH THE USE
 * OF THE INFORMATION CONTAINED IN THIS DOCUMENT, HOWEVER CAUSED AND
 * BASED ON ANY THEORY OF LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, AND EVEN IF SYNAPTICS WAS
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. IF A TRIBUNAL OF
 * COMPETENT JURISDICTION DOES NOT PERMIT THE DISCLAIMER OF DIRECT
 * DAMAGES OR ANY OTHER DAMAGES, SYNAPTICS' TOTAL CUMULATIVE LIABILITY
 * TO ANY PARTY SHALL NOT EXCEED ONE HUNDRED U.S. DOLLARS.
 */
#include "i2c_driver.h"
#include "io.h"
#include "debug.h"
#include "chip_voltage_info.h"
#include "timer.h"
#include "pv_comp.h"

// all defined under uv
#define HL7593_STEPuV 6250
#define MIN_VOLTAGE 600000
#define MAX_VOLTAGE 1394000

#define SLAVE_ADDR 	0x61
#define RSVD		0xFFFFFFFF
#define REG_VSEL1	0x1
#define REG_CTRL2	0x2

extern int i2c_master_init(int id, int speed, int b_10bit_addr);
extern int i2c_master_write_and_read(int id, int target_addr, unsigned char* send_buff, int send_len, unsigned char* recv_buff, int recv_len);

static int hl7593_pmic_volt2data(int volt, int *p_data)
{
	unsigned int data;

	if ((volt >= MIN_VOLTAGE) && (volt <= MAX_VOLTAGE))
		data = (volt - MIN_VOLTAGE) / HL7593_STEPuV;
	else
		data = RSVD;

	if (data == RSVD)
		return -1;
	else {
		*p_data = data;
		return 0;
	}
}

static int hl7593_pmic_data2volt(int *p_volt, int vdata)
{
	unsigned int volt;
	unsigned int data = vdata;

	data = data & 0x3F;
	if (data <= 0x3f)
		volt = MIN_VOLTAGE + (data * HL7593_STEPuV);
	else
		volt = RSVD;

	if (volt == RSVD)
		return -1;
	else {
		*p_volt = volt;
		return 0;
	}
}

static int hl7593_set_vol(int master_id, int slaveAddr, int volt)
{
	int target = volt;
	unsigned char buff[2];
	unsigned char data;
	int vdata = 0;
	int ret = 1;

	ret = hl7593_pmic_volt2data(target, &vdata);
	if (ret == 0) {
		i2c_master_init(master_id, 100, 0);  //int id, int speed, int b_10bit_addr

		buff[0] = (unsigned char)REG_VSEL1;
		buff[1] = (unsigned char)0x80 + vdata;
		ret = i2c_master_write_and_read(master_id, slaveAddr, buff, 2, (unsigned char*)0, 0);
		if (ret) {
			dbg_printf(PRN_ERR,"PMIC i2c write fail !\n");
			return ret;
		}

		buff[0] = (unsigned char)REG_VSEL1;
		ret = i2c_master_write_and_read(master_id, slaveAddr, buff, 1, &data, 1);
		if (ret) {
			dbg_printf(PRN_ERR,"PMIC i2c read fail !\n");
			return ret;
		}

		if (buff[1] != data) {
			dbg_printf(PRN_ERR, "PMIC hl7593: read:0x%02x != write:0x%02x\n", data, buff[1]);
			return -1;
		}
		mdelay(1);
	}
	else {
		dbg_printf(PRN_ERR, "Illegal target voltage for PMIC sy8824b !\n");
		return -1;
	}
	return ret;
}

static int hl7593_get_vol(int master_id, int slaveAddr)
{
	unsigned char read = 0xff;
	unsigned char buff[2];
	int volt;
	unsigned int bulk_reg = REG_VSEL1;
	unsigned int bulk_ctrl = REG_CTRL2;
	int ret = 1;

	i2c_master_init(master_id, 100, 0);  //int id, int speed, int b_10bit_addr

	ret = i2c_master_write_and_read(master_id, slaveAddr, (unsigned char*)&bulk_ctrl, 1, (unsigned char*)&read, 1);
	if (ret) {
		dbg_printf(PRN_ERR,"PMIC i2c read fail !\n");
		return ret;
	}

	buff[0] = (unsigned char)REG_CTRL2;
	buff[1] = (unsigned char)(read & 0x7F); //disable fast discharge
	ret = i2c_master_write_and_read(master_id, slaveAddr, buff, 2, (unsigned char*)0, 0);
	if (ret) {
		dbg_printf(PRN_ERR,"PMIC i2c write fail !\n");
		return ret;
	}

	ret = i2c_master_write_and_read(master_id, slaveAddr, (unsigned char*)&bulk_reg, 1, (unsigned char*)&read, 1);

	ret |= hl7593_pmic_data2volt(&volt, (int)(read-0x80));
	if(ret) {
		dbg_printf(PRN_ERR," i2c read fail\n");
		return -1;
	}
	return volt;
}

static int i2c_get_cpu_volt(void)
{
	int volt = hl7593_get_vol(VOUT_CPU_ID, SLAVE_ADDR);

	if(volt < 0)
		return 0;

	return volt;
}

static int i2c_get_core_volt(void)
{
	int volt = hl7593_get_vol(VOUT_CORE_ID, SLAVE_ADDR);

	if(volt < 0)
		return 0;

	return volt;
}

static int i2c_set_volt(int master_id, int from, int to)
{
	int volt = from;

	while (volt != to){
		if (volt > to){
			volt -= HL7593_STEPuV;
			if (volt < to)
				volt = to;
		}else{
			volt += HL7593_STEPuV;
			if (volt > to)
				volt = to;
		}

		if(0 != hl7593_set_vol(master_id, SLAVE_ADDR, volt))
			return 1;
	}

	return 0;
}

static int i2c_set_vcpu_volt(int from, int to)
{
	return i2c_set_volt(VOUT_CPU_ID, from ,to);
}

static int i2c_set_vcore_volt(int from, int to)
{
	return i2c_set_volt(VOUT_CORE_ID, from ,to);
}

const dvfs_ops_t hl7593_ops = {
	.get_vcpu_volt = i2c_get_cpu_volt,
	.get_vcore_volt = i2c_get_core_volt,
	.set_vcpu_volt = i2c_set_vcpu_volt,
	.set_vcore_volt =i2c_set_vcore_volt
};
