#include "memmap.h"
#include "mem_ctrl.h"
#include "system_manager.h"
#include "ra_gbl.h"
/*
 *#include "sm_const.h" DTS for build warning
*/

#define DDR_REG_BASE            MEMMAP_DDR_REG_BASE
#define DDR_CONFIGURATION_REG       (RA_MctrlDual_Configuration + DDR_REG_BASE)
#define DDR_TRIGGER_REG         (RA_MctrlDual_Trigger + DDR_REG_BASE)
#define DDR_STATUS_REG          (RA_MctrlDual_Status + DDR_REG_BASE)
#define DDR_TIMING_I_REG        (RA_MctrlDual_Timing1 + DDR_REG_BASE)
#define DDR_TIMING_II_REG       (RA_MctrlDual_Timing2 + DDR_REG_BASE)
#define DDR_TIMING_III_REG      (RA_MctrlDual_Timing3 + DDR_REG_BASE)
#define DDR_CONTROL_I_REG       (RA_MctrlDual_Control1 + DDR_REG_BASE)
#define DDR_CONTROL_II_REG      (RA_MctrlDual_Control2 + DDR_REG_BASE)
#define DDR_DEBUG_TRIGGER_REG       (RA_MctrlDual_DebugTrigger + DDR_REG_BASE)
#define DDR_DEBUG_ST_ADDR_REG       (RA_MctrlDual_DebugStAddr + DDR_REG_BASE)
#define DDR_DEBUG_ED_ADDR_REG       (RA_MctrlDual_DebugEdAddr + DDR_REG_BASE)
#define DDR_DEBUG_STATUS_REG        (RA_MctrlDual_DebugStatus + DDR_REG_BASE)
#define DDR_DEBUG_RADDR_REG     (RA_MctrlDual_DebugRaddr + DDR_REG_BASE)
#define DDR_DEBUG_RDATA_REG     (RA_MctrlDual_DebugRaddr + DDR_REG_BASE)
#define DDR_DEBUG_RDATA0_REG        (RA_MctrlDual_DebugRdata0 + DDR_REG_BASE)
#define DDR_DEBUG_RDATA1_REG        (RA_MctrlDual_DebugRdata1 + DDR_REG_BASE)
#define DDR_PHY_REG         (RA_MctrlDual_PHY + DDR_REG_BASE)

#define	DUAL_CHANNEL_1_ENABLE		(0x01 << LSb32MctrlDual_Configuration_dual_channel)
#define	DUAL_CHANNEL_1_DISABLE		(0x00 << LSb32MctrlDual_Configuration_dual_channel)
#define	AL_SEL_LATENCY_DISABLE		(0x00 << LSb32MctrlDual_Configuration_al_sel)
#define	AL_SEL_LATENCY_2_CYCLES		(0x02 << LSb32MctrlDual_Configuration_al_sel)
#define	AL_SEL_LATENCY_3_CYCLES		(0x03 << LSb32MctrlDual_Configuration_al_sel)
#define	AL_SEL_LATENCY_4_CYCLES		(0x04 << LSb32MctrlDual_Configuration_al_sel)
#define	AL_SEL_LATENCY_5_CYCLES		(0x05 << LSb32MctrlDual_Configuration_al_sel)
#define	CL_SEL_LATENCY_3_CYCLES		(0x02 << LSb32MctrlDual_Configuration_cl_sel)
#define	CL_SEL_LATENCY_4_CYCLES		(0x03 << LSb32MctrlDual_Configuration_cl_sel)
#define	CL_SEL_LATENCY_5_CYCLES		(0x04 << LSb32MctrlDual_Configuration_cl_sel)
#define	CL_SEL_LATENCY_6_CYCLES		(0x05 << LSb32MctrlDual_Configuration_cl_sel)
#define	NO_CS_ADDR_ONE_CHIP		(0x00 << LSb32MctrlDual_Configuration_no_cs_addr)
#define	NO_CS_ADDR_TWO_CHIP		(0x01 << LSb32MctrlDual_Configuration_no_cs_addr)
#define	NO_CS_ADDR_FOUR_CHIP		(0x02 << LSb32MctrlDual_Configuration_no_cs_addr)
#define	NO_ROW_ADDR			(0x0D << LSb32MctrlDual_Configuration_no_row_addr)
#define	NO_COL_ADDR			(0x0A << LSb32MctrlDual_Configuration_no_col_addr)
#define	BANK_MODE_ROW_ADDR		(0x01 << LSb32MctrlDual_Configuration_bank_mode)
#define	BANK_MODE_COLUMN_ADDR		(0x00 << LSb32MctrlDual_Configuration_bank_mode)
#define	BANK_SEL_4_BANK			(0x00 << LSb32MctrlDual_Configuration_bank_sel)
#define	BANK_SEL_8_BANK			(0x01 << LSb32MctrlDual_Configuration_bank_sel)
#define	MEM_TYPE_DDR_I			(0x00 << LSb32MctrlDual_Configuration_mem_type)
#define	MEM_TYPE_SDR			(0x01 << LSb32MctrlDual_Configuration_mem_type)
#define	MEM_TYPE_MOBILE_DDR		(0x02 << LSb32MctrlDual_Configuration_mem_type)
#define	MEM_TYPE_MOBILE_SDR		(0x03 << LSb32MctrlDual_Configuration_mem_type)
#define	MEM_TYPE_DDR_II			(0x04 << LSb32MctrlDual_Configuration_mem_type)
#define	DATA_WIDTH_16_BIT		(0x00 << LSb32MctrlDual_Configuration_data_width)
#define	DATA_WIDTH_32_BIT		(0x01 << LSb32MctrlDual_Configuration_data_width)

#define	TRIGGER_AUTO_PWRSAV_CH1		(0x01 << LSb32MctrlDual_Trigger_auto_pwrsav_ch1)
#define	TRIGGER_AUTO_PWRSAV_CH0		(0x01 << LSb32MctrlDual_Trigger_auto_pwrsav_ch0)
#define	TRIGGER_PWRSAV_ACT_CH1		(0x01 << LSb32MctrlDual_Trigger_pwrsav_actReq_ch1)
#define	TRIGGER_PWRSAV_ACT_CH0		(0x01 << LSb32MctrlDual_Trigger_pwrsav_actReq_ch0)
#define	TRIGGER_PWRSAV_EXIT_CH1		(0x01 << LSb32MctrlDual_Trigger_pwrsav_exitReq_ch1)
#define	TRIGGER_PWRSAV_EXIT_CH0		(0x01 << LSb32MctrlDual_Trigger_pwrsav_exitReq_ch0)
#define	TRIGGER_SDRAM1_INIT_REQ		(0x01 << LSb32MctrlDual_Trigger_sdram1_init_req)
#define	TRIGGER_SDRAM0_INIT_REQ		(0x01 << LSb32MctrlDual_Trigger_sdram0_init_req)
#define	TRIGGER_SDRAM_WBUFFERFLASH_CH1	(0x01 << LSb32MctrlDual_Trigger_wBufferFlushCh1)
#define	TRIGGER_SDRAM_WBUFFERFLASH_CH0	(0x01 << LSb32MctrlDual_Trigger_wBufferFlushCh0)

#define	STATUS_TTB_EMPTY				(0x01 << LSb32MctrlDual_Status_ttbEmpty)
#define	STATUS_INQ1_FULL		(0x01 << LSb32MctrlDual_Status_inq1_full)
#define	STATUS_INQ1_EMPTY		(0x01 << LSb32MctrlDual_Status_inq1_empty)
#define	STATUS_INQ0_FULL		(0x01 << LSb32MctrlDual_Status_inq0_full)
#define	STATUS_INQ0_EMPTY		(0x01 << LSb32MctrlDual_Status_inq0_empty)
#define	STATUS_APS_ON_CH1		(0x01 << LSb32MctrlDual_Status_aps_on_ch1)
#define	STATUS_APS_ON_CH0		(0x01 << LSb32MctrlDual_Status_aps_on_ch0)
#define	STATUS_PWRSAV_ACTDONE_CH1	(0x01 << LSb32MctrlDual_Status_pwrsav_actDone_ch1)
#define	STATUS_PWRSAV_ACTDONE_CH0	(0x01 << LSb32MctrlDual_Status_pwrsav_actDone_ch0)
#define	STATUS_PWRSAV_EXITDONE_CH1	(0x01 << LSb32MctrlDual_Status_pwrsav_exitDone_ch1)
#define	STATUS_PWRSAV_EXITDONE_CH0	(0x01 << LSb32MctrlDual_Status_pwrsav_exitDone_ch0)
#define	STATUS_SDRAM1_INIT_DONE		(0x01 << LSb32MctrlDual_Status_sdram1_init_done)
#define	STATUS_SDRAM0_INIT_DONE		(0x01 << LSb32MctrlDual_Status_sdram0_init_done)
#define	STATUS_SDRAM_WBUFFEREMPTY_CH1	(0x01 << LSb32MctrlDual_Status_wBufferEmptyCh1)
#define	STATUS_SDRAM_WBUFFEREMPTY_CH0	(0x01 << LSb32MctrlDual_Status_wBufferEmptyCh0)

#define	TIMING_I_INIT_TWTR		(0x02 << LSb32MctrlDual_Timing1_init_twtr)

#define	DEBUG_TRIGGER_MT_LOOP		(0x01 << LSb32MctrlDual_DebugTrigger_mt_loop)
#define	DEBUG_TRIGGER_MT_ACTIVE		(0x01 << LSb32MctrlDual_DebugTrigger_mt_active)

#define	DEBUG_STATUS_MT_DONE		(0x01 << LSb32MctrlDual_DebugStatus_mt_done)
#define	DEBUG_STATUS_MT_ERROR_MASK	MSK32MctrlDual_DebugStatus_mt_error

#define	DDR_PHY_REG_BASE		(DDR_REG_BASE + RA_MctrlDual_PHY)
#define	DDR_PHY_TIMING_REG		(DDR_PHY_REG_BASE + RA_PHY_Timing)
#define	DDR_PHY_DLL_REG			(DDR_PHY_REG_BASE + RA_PHY_DLL)

#define DDR_PHY_TIMING_RLAT		(0x01 << LSb32PHY_Timing_rlat)
#define DDR_PHY_TIMING_RLAT_MASK	(MSK32PHY_Timing_rlat)
#define	DDR_PHY_TIMING_RPOP		(0x01 << LSb32PHY_Timing_en2vld)
#define	DDR_PHY_TIMING_RPOP_MASK	(MSK32PHY_Timing_en2vld)
#define	DDR_PHY_DLL_PHSEL		(0x01 << LSb32PHY_DLL_PHSEL)
#define	DDR_PHY_DLL_PHSEL_MASK		(MSK32PHY_DLL_PHSEL)
#define	DDR_PHY_DLL_PHSEL_EN		(MSK32PHY_DLL_UPDATE_EN)

#define	APBPERIF_REG_BASE		(0x00 + MEMMAP_APBPERIF_REG_BASE)
#define	GPIO_INST_OFFSET		0x00000C00
#define	GPIO_INST_REG_BASE		(APBPERIF_REG_BASE + GPIO_INST_OFFSET)
#define	GPIO_SWPORTA_DR			(0x00 + GPIO_INST_REG_BASE)
#define	GPIO_SWPORTA_DDR		(0x04 + GPIO_INST_REG_BASE)
#define	GPIO_PORTA_CTL			(0x08 + GPIO_INST_REG_BASE)
#define	GPIO_SWPORTB_DR			(0x0C + GPIO_INST_REG_BASE)
#define	GPIO_SWPORTB_DDR		(0x10 + GPIO_INST_REG_BASE)
#define	GPIO_PORTB_CTL			(0x14 + GPIO_INST_REG_BASE)
#define	GPIO_SWPORTC_DR			(0x18 + GPIO_INST_REG_BASE)
#define	GPIO_SWPORTC_DDR		(0x1C + GPIO_INST_REG_BASE)
#define	GPIO_PORTC_CTL			(0x20 + GPIO_INST_REG_BASE)
#define	GPIO_SWPORTD_DR			(0x24 + GPIO_INST_REG_BASE)
#define	GPIO_SWPORTD_DDR		(0x28 + GPIO_INST_REG_BASE)
#define	GPIO_PORTD_CTL			(0x2C + GPIO_INST_REG_BASE)
#define	GPIO_INTEN			(0x30 + GPIO_INST_REG_BASE)
#define	GPIO_INTMASK			(0x34 + GPIO_INST_REG_BASE)
#define	GPIO_INTTYPE_LEVEL		(0x38 + GPIO_INST_REG_BASE)
#define	GPIO_INT_POLARITY		(0x3C + GPIO_INST_REG_BASE)
#define	GPIO_INTSTATUS			(0x40 + GPIO_INST_REG_BASE)
#define	GPIO_RAWINTSTATUS		(0x44 + GPIO_INST_REG_BASE)
#define	GPIO_DEBOUNCE			(0x48 + GPIO_INST_REG_BASE)


#define SM_MSG_SIZE                     32
#define SM_MSGQ_TOTAL_SIZE      512
#define SM_MSGQ_HEADER_SIZE     SM_MSG_SIZE
#define SM_MSGQ_SIZE            (512-32)	//;( SM_MSGQ_TOTAL_SIZE - SM_MSGQ_HEADER_SIZE )
#define SM_MSGQ_MSG_COUNT       ( SM_MSGQ_SIZE / SM_MSG_SIZE )

#define CPU0_INPUT_QUEUE_ADDR   ( SM_DTCM_END - SM_MSGQ_TOTAL_SIZE * 4 )
#define CPU1_INPUT_QUEUE_ADDR   ( SM_DTCM_END - SM_MSGQ_TOTAL_SIZE * 3 )
#define CPU0_OUTPUT_QUEUE_ADDR  ( SM_DTCM_END - SM_MSGQ_TOTAL_SIZE * 2 )
#define CPU1_OUTPUT_QUEUE_ADDR  ( SM_DTCM_END - SM_MSGQ_TOTAL_SIZE * 1 )

#define SM_Recv_MsgQ (MV_SM_MsgQ*)SOC_DTCM( CPU1_OUTPUT_QUEUE_ADDR )

#define SM_Recv_Q_Cnt 1

#define	SM_Send_MsgQ	(0x04004000-512*3 - 0x04000000 + 0xf7fa0000)




FIGO_SRAM_ENTRY:
	
		/*
                 * force to be supervisor mode and disable IRQ/FIQ
                 */
                MRS             r0, cpsr                // load program status register
                BIC             r0, r0, #0x1F           // r0 = r0 & !0x1F
                ORR             r0, r0, #0xD3           // r0 = r0 | 0xD3
                MSR             cpsr_c, r0              // Supervisor with disabled IRQ/FIQ

#               ifdef           ENABLE_REG_LOG
                LDR             r0, =CHIP_CTRL_SW_GENERIC3_REG
                LDR             r1, =0x4e124624         // write "OFF$" as log
                STR             r1, [r0]
#               endif

                /*
                 * disable MMU/PU and turn off caches
                 */
                MRC             p15, 0, r0, c1, c0, 0
                BIC             r0, r0, #0x00001000     // clear bits 12 (---I)
                BIC             r0, r0, #0x00000085     // clear bits 7, 2:0 (B--- -D-M)
                ORR             r0, r0, #0x00002000     // set bit 13 (V) High exception vectors
                MCR             p15, 0, r0, c1, c0, 0


#               ifdef           ENABLE_REG_LOG
                LDR             r0, =CHIP_CTRL_SW_GENERIC3_REG
                LDR             r1, =0x214D4D55         // write "!MMU" as log
                STR             r1, [r0]
#               endif

		/*
		 * put DDR into self-refresh mode
		 */
		//! ***** Block all data requests.:
		//SDRAM_Control_Register_14.Block_all_data_req: 1
		mov	r0, #1
		ldr	r1, =0xF7CB0068
		str	r0, [r1]

		//! ***** flush write buffer:
		//User_Initiated_Command_Register_0.user_wcb_drain_req: write 1
		mov	r0, #2
		ldr	r1, =0xF7CB0160
		str	r0, [r1]

		//! ***** Check Memory_Controller_Status_Register.wcb_empty
loop1:
		ldr	r1, =0xF7CB0004
		ldr	r0, [r1]
		and	r0, r0, #0x40
		cmp	r0, #0x40
		bne	loop1

		//! ***** Stop scheduler scheduling regular requests from BQ:
		//SDRAM_Control_Register_14.halt_scheduler: 1
		mov	r0, #3
		ldr	r1, =0xF7CB0068
		str	r0, [r1]

		//! ***** turn off ODT:
		//SDRAM_Configuration_Register_Type_1.RTT#: 1->0
		ldr	r0, =0x00002530
		ldr	r1, =0xF7CB0020
		str	r0, [r1]

		ldr	r0, =0x0F000200
		ldr	r1, =0xF7CB0160
		str	r0, [r1]

		//! ***** Enter Self Refresh:
		//User_Initiated_Command_Register_0.user_sr_req: write b01
		ldr	r0, =0x100000
		ldr	r1, =0x1
1:		subs	r0, r0, r1
		bne	1b

		ldr	r0, =0xF000040
		ldr	r1, =0xF7CB0160
		str	r0, [r1]

		ldr	r0, =0x100000
		ldr	r1, =0x1
1:		subs	r0, r0, r1
		bne	1b

		//! ***** wait DDR enter self-refresh done
loop2:
		ldr	r1, =0xF7CB0008
		ldr	r0, [r1]
		and	r0, r0, #0x11
		cmp	r0, #0x11
		bne	loop2




			 	//;	MsgQ = SM_Send_MsgQ	
				LDR				r0,	=SM_Send_MsgQ
				LDR				r1,	[r0]	
		
				//;	r2 = m_Queue
				ADD				r2,	r0,	#SM_MSGQ_HEADER_SIZE
		
				//;	r2 = m_Quene[MsgQ->m_iWrite]
				ADD				r2,	r2,	r1
		
				//;	back up
				MOV				r5,	r1	

				MOV				r3,	#10	// len = 10
				LSL				r3,	#16
				//;	iModuleID = MV_SM_ID_POWER
				ADD				r3,	r3,	#MV_SM_ID_POWER

				//;	save iModuleID & len		
				STR				r3,	[r2],	#4
		
				//;	save message content
				//;	LDR		r4,	=0xFFFFFFFF

				LDR				r6,	=SM_BOOT_FLAG_REG
				LDR				r7,	[r6]
				
				LDR				r8,	=MV_SoC_STATE_WARMDOWN_1
				CMP				r7,	r8
				LDREQ			r4,	=MV_SM_POWER_WARMDOWN_1_RESP
				BEQ				WARM_DOWN_MSG
				
				LDR				r8,	=MV_SoC_STATE_WARMDOWN_2
				CMP				r7, r8
				LDREQ			r4, =MV_SM_POWER_WARMDOWN_2_RESP
				BEQ				WARM_DOWN_MSG


				LDR				r4, =MV_SoC_STATE_COLDBOOT
				STR				r4, [r2],   #4
				
				LDR				r4, =MV_SoC_STATE_COLDBOOT
				STR             r4, [r2],   #4
				B				SEND_MSG				

WARM_DOWN_MSG:
				STR				r4,	[r2],	#4
		
				STR				r4,	[r2],	#4
		
				/*
				MsgQ->m_iWrite += SM_MSG_SIZE;
				if( MsgQ->m_iWrite >= SM_MSGQ_SIZE )
				{
					MsgQ->m_iWrite -= SM_MSGQ_SIZE;
				}
				*/
SEND_MSG:
			
				//;LDR				r5,	[r5]
				ADD				r5,	r5,	#SM_MSG_SIZE
		
				CMP				r5,	#SM_MSGQ_SIZE
				SUBCS			r5,	r5,	#SM_MSGQ_SIZE		
	
				STR				r5,	[r0]

				/*
					MsgQ->m_iWriteTotal += SM_MSG_SIZE;
				*/
				LDR				r5,	[r0,	#8]
				ADD				r5,	r5,	#SM_MSG_SIZE
		
				STR				r5,	[r0,	#8]

		/* ensure data are written to SM DTCM before idle */
		DSB

		
#               ifdef           ENABLE_REG_LOG
                LDR             r0, =CHIP_CTRL_SW_GENERIC3_REG
                LDR             r1, =0x2344311         // write "!MMU" as log
                STR             r1, [r0]
#               endif

				//;	goto a dead loop
DEAD_LOOP:
				B				DEAD_LOOP
