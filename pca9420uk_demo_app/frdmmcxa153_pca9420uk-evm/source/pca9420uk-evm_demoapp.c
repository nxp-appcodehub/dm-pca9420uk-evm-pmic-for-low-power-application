/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file pca9420uk-evm_demoapp.c
 * @brief The pca9420uk-evm_demoapp.c file implements the ISSDK PCA9420 PMIC
 *        driver example demonstration with interrupt mode.
 */

//-----------------------------------------------------------------------
// SDK Includes
//-----------------------------------------------------------------------
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "RTE_Device.h"
#include "stdio.h"
#include "frdmmcxa153.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_LPI2C_DMA_BASEADDR DMA0
#define EXAMPLE_LPI2C_DMA_CLOCK    kCLOCK_GateDMA

//-----------------------------------------------------------------------
// ISSDK Includes
//-----------------------------------------------------------------------
#include "issdk_hal.h"
#include "gpio_driver.h"
#include "../pmic/pca9420uk_drv.h"
#include "../pmic/pca9420uk.h"
#include "systick_utils.h"

//-----------------------------------------------------------------------
// CMSIS Includes
//-----------------------------------------------------------------------
#include "Driver_I2C.h"
#include "Driver_GPIO.h"

//-----------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------
enum _pca9420_thrml_reg_thshld epca9420_thrml_reg_thshld;
enum _pca9420_ntc_beta_val epca9420_ntc_beta_val;
enum _pca9420_ntc_res_sel epca9420_ntc_res_sel;
enum _pca9420_topoff_timer epca9420_topoff_timer;
enum _pca9420_preq_chrg_timer epca9420_preq_chrg_timer;
enum _pca9420_fast_chrg_timer epca9420_fast_chrg_timer;
enum _pca9420_dead_bat_chrg_cur epca9420_dead_bat_chrg_cur;
enum _pca9420_low_bat_chrg_cur  epca9420_low_bat_chrg_cur;
enum _pca9420_bat_chrg_cur epca9420_bat_chrg_cur;

//device setting
enum _pca9420_vin_ilim epca9420_vin_ilim;
enum _pca9420_asys_prewarning epca9420_asys_prewarning;
enum _pca9420_asys_input_sel epca9420_asys_input_sel;
enum _pca9420_vin_ovp_sel epca9420_vin_ovp;
enum _pca9420_vin_uvlo_sel epca9420_vin_uvlo;
enum _pca9420_asys_uvlo_sel epca9420_asys_uvlo;
enum _pca9420_them_shdn epca9420_them_shdn;
enum _pca9420_die_temp_warning epca9420_die_temp;
enum _pca9420_mode epca9420_mode;
enum LongGlitchTimer eLngGthTmr;

//regulator group setting
enum _pca9420_sw1_out epca9420_sw1_out;
enum _pca9420_sw2_out epca9420_sw2_out;
enum _pca9420_ldo1_out epca9420_ldo1_out;
enum _pca9420_ldo2_out epca9420_ldo2_out;

//-----------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------
ARM_DRIVER_I2C *I2Cdrv = &I2C_S_DRIVER;
GENERIC_DRIVER_GPIO *pGpioDriver = &Driver_GPIO_KSDK;

pca9420_i2c_sensorhandle_t pca9420Driver;

//-----------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------
void PCA9420_INT1_ISR(void)
{
	/* Clear external interrupt flag. */
	GPIO_GpioClearInterruptFlags(PCA9420_INT.base, 1U << PCA9420_INT.pinNumber);
	PRINTF("\r\n\033[35m Interrupt Occurred!!! Check Interrupt Status. \033[37m\r\n");
	SDK_ISR_EXIT_BARRIER;
}

/*! -----------------------------------------------------------------------
 *  @brief       Initialize PCA9420UK Interrupt Pin and Enable IRQ
 *  @details     This function initializes PCA9420UK interrupt pin
 *  @return      void  There is no return value.
 *  -----------------------------------------------------------------------*/
void init_pca9420_wakeup_int(void)
{
	pGpioDriver->pin_init(&PCA9420_INT, GPIO_DIRECTION_IN, NULL, NULL, NULL);
	EnableIRQ(PCA9420_INT1_IRQ);
}

//PCA9420_Functions
//-----------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------

bool isMultipleof5 (int n)
{
	while ( n > 0 )
		n = n - 5;

	if ( n == 0 )
		return true;

	return false;
}

static void pmic_status()
{
	uint16_t character, data, vol_offset, offset;
	float sw1_vol, sw2_vol, ldo1_vol, ldo2_vol;

	PRINTF("\r\n**********\033[35m PMIC STATUS \033[37m**********\r\n");

	//Mode status
	PRINTF("\r\n\033[93m MODE:  \033[37m");
	PCA9420_DRV_Read(&pca9420Driver, PCA9420UK_TOP_CNTL3, &data );
	data = (data & PCA9420_MODE_CNTL_SEL_MASK) >> PCA9420_MODE_CNTL_SEL_SHIFT;

	switch (data)
	{
	case kPCA9420_Mode0:
		offset = PCA9420UK_MODECFG_0_0;
		PRINTF("Mode 0\r\n");
		break;
	case kPCA9420_Mode1:
		offset = PCA9420UK_MODECFG_1_0;
		PRINTF("Mode 1\r\n");
		break;
	case kPCA9420_Mode2:
		offset = PCA9420UK_MODECFG_2_0;
		PRINTF("Mode 2\r\n");
		break;
	case kPCA9420_Mode3:
		offset = PCA9420UK_MODECFG_3_0;
		PRINTF("Mode 3\r\n");
		break;
	default:
		PRINTF("\r\nInvalid option selected\r\n");
		break;
	}

	PCA9420_DRV_Read(&pca9420Driver, offset, &data );
	if(data >=0 && data <= 40)
		PRINTF("\r\n\033[32m SW1: \033[37m  %f V \r\n",(float)((0.500)+(data * (0.025))));
	else if(data >= 41 && data <= 62)
	{
		sw1_vol = 1.5;
		PRINTF("\r\n\033[32m SW1: \033[37m  %f V \r\n",sw1_vol);
	}
	else
	{
		sw1_vol = 1.8;
		PRINTF("\r\n\033[32m SW1: \033[37m  %f V \r\n",sw1_vol);
	}

	//SW2 Regulator Status
	PCA9420_DRV_Read(&pca9420Driver, offset+1, &data );
	PCA9420_DRV_Read(&pca9420Driver, offset+1, &vol_offset );

	vol_offset = (vol_offset & PCA9420_SW2_VOL_OFFSET_MASK) >> PCA9420_SW2_VOL_OFFSET_SHIFT;
	data = (data & PCA9420_SW2_VOL_MASK);

	if(vol_offset)
	{
		if(data >=0 && data <= 24)
			PRINTF("\r\n\033[32m SW2: \033[37m  %f V \r\n",(float)(((1.500) + (data * .025)) + 1.2));
		else
			PRINTF("\r\n\033[32m SW2: \033[37m  %f V \r\n",(float)(2.1 + 1.2));
	}
	else
	{
		if(data >=0 && data <= 24)
			PRINTF("\r\n\033[32m SW2: \033[37m  %f V \r\n",(float)((1.500) + (data * .025)));

		else
			PRINTF("\r\n\033[32m SW2: \033[37m  %f V \r\n", (float)(2.1));
		sw2_vol = 2.1;
	}

	//LDO1 Status
	PCA9420_DRV_Read(&pca9420Driver, offset+2, &data );
	data = (data & PCA9420_LDO1_VOL_MASK) >> PCA9420_LDO1_VOL_SHIFT ;

	if(data >=0 && data <= 8)
		PRINTF("\r\n\033[32m LDO1: \033[37m %f V \r\n",(float)((1.700) + (data * .025)));
	else
		PRINTF("\r\n\033[32m LDO1: \033[37m %f V \r\n",(float)(1.9));

	//LDO2 Regulator Status
	PCA9420_DRV_Read(&pca9420Driver, offset+3, &data );
	PCA9420_DRV_Read(&pca9420Driver, offset+3, &vol_offset );

	vol_offset = (vol_offset & PCA9420_LDO2_VOL_OFFSET_MASK) >> PCA9420_LDO2_VOL_OFFSET_SHIFT;
	data = (data & PCA9420_LDO2_VOL_MASK);

	if(vol_offset)
	{
		if(data >=0 && data <= 24)
			PRINTF("\r\n\033[32m LDO2: \033[37m %f V \r\n",(float)(((1.500) + (data * .025)) + 1.2));
		else
			PRINTF("\r\n\033[32m LDO2: \033[37m %f V \r\n",(float)(2.1 + 1.2));
	}
	else
	{
		if(data >=0 && data <= 24)
			PRINTF("\r\n\033[32m LDO2: \033[37m %f V \r\n",(float)((1.500) + (data * .025)));
		else
			PRINTF("\r\n\033[32m LDO2: \033[37m %f V \r\n",(float)(2.1));
	}

	PRINTF("\r\n********************************\r\n");
}

static void get_device_id()
{
	uint16_t data, id, rev;
	int32_t status;

	PRINTF("\r\n********************************\r\n");
	status = PCA9420_DRV_Read(&pca9420Driver, PCA9420UK_DEV_INFO, &data );

	if (SENSOR_ERROR_NONE != status)
	{
		PRINTF("\r\nError in getting PCA9420 Device Information.\r\n");
	}
	else
	{
		id = (data & PCA9420UK_DEVICE_ID_MASK) >> PCA9420UK_DEVICE_ID_SHIFT;
		PRINTF("\r\n\033[93m Device ID = %x \033[37m \r\n", id);

		rev = (data & PCA9420UK_DEVICE_REV_MASK) >> PCA9420UK_DEVICE_REV_SHIFT;
		PRINTF("\r\n\033[35m Device Revision = %x \033[37m \r\n", rev);
	}
	PRINTF("\r\n********************************\r\n");
}

static void get_regulator_status()
{
	uint16_t character, data;

	PCA9420_DRV_Read(&pca9420Driver, PCA9420UK_REG_STATUS, &data );
	PRINTF("\r\n************\033[35m REG POWER-GOOD STATUS \033[37m************\r\n");

	if(data & PCA9420_SW1_VOUT_MASK)
		PRINTF("\r\n\033[93m VOUT_SW1:  \033[37m VOUT_SW1 is OK. \r\n");
	else
		PRINTF("\r\n\033[93m VOUT_SW1:  \033[37m VOUT_SW1 is not OK. \r\n");

	if(data & PCA9420_SW2_VOUT_MASK)
		PRINTF("\r\n\033[93m VOUT_SW2:  \033[37m VOUT_SW2 is OK. \r\n");
	else
		PRINTF("\r\n\033[93m VOUT_SW2:  \033[37m VOUT_SW2 is not OK. \r\n");

	if(data & PCA9420_LDO1_VOUT_MASK)
		PRINTF("\r\n\033[93m VOUT_LDO1: \033[37m VOUT_LDO1 is OK. \r\n");
	else
		PRINTF("\r\n\033[93m VOUT_LDO1: \033[37m VOUT_LDO1 is not OK. \r\n");

	if(data & PCA9420_LDO2_VOUT_MASK)
		PRINTF("\r\n\033[93m VOUT_LDO2: \033[37m VOUT_LDO2 is OK. \r\n");
	else
		PRINTF("\r\n\033[93m VOUT_LDO2: \033[37m VOUT_LDO2 is not OK. \r\n");

	PRINTF("\r\n***********************************************\r\n");

}

static void top_level_interrupt_status()
{
	uint16_t character, data, int_status=1;
	char dummy;

	PCA9420_clear_interrupt(&pca9420Driver);
	PCA9420_DRV_Read(&pca9420Driver, PCA9420UK_TOP_INT, &data );

	if((data & PCA9420_SYS_INT_MASK) >> PCA9420_SYS_INT_SHIFT || (data & PCA9420_BAT_INT_MASK) >> PCA9420_BAT_INT_SHIFT
			|| ((data & PCA9420_BUCK_INT_MASK) >> PCA9420_BUCK_INT_SHIFT) || ((data & PCA9420_LDO_INT_MASK) >> PCA9420_LDO_INT_SHIFT))
	{
		if((data & PCA9420_SYS_INT_MASK) >> PCA9420_SYS_INT_SHIFT)
			PRINTF("\r\n\033[31m System level interrupt occurred!!! \033[37m\r\n");

#if (!PCA9421UK_EVM_EN) //PCA9420UK-EVM
		if((data & PCA9420_BAT_INT_MASK) >> PCA9420_BAT_INT_SHIFT)
			PRINTF("\r\n\033[31m Linear battery charger interrupt occurred!!! \033[37m\r\n");
#else //PCA9421UK-EVM
		if((data & PCA9421_VIN_INT_MASK) >> PCA9421_VIN_INT_SHIFT)
			PRINTF("\r\n\033[31m VIN interrupt occurred!!! \033[37m\r\n");
#endif

		if(((data & PCA9420_BUCK_INT_MASK) >> PCA9420_BUCK_INT_SHIFT) || ((data & PCA9420_LDO_INT_MASK) >> PCA9420_LDO_INT_SHIFT))
			PRINTF("\r\n\033[31m Voltage regulator interrupt occurred!!! \033[37m");

		do{
			PRINTF("\r\n\r\n\033[93m *****Select any one to check particular Interrupt status***** \033[37m\r\n");
			PRINTF("\r\n1. System Level Interrupt \r\n");

#if (!PCA9421UK_EVM_EN) //PCA9420UK-EVM
			PRINTF("\r\n2. Linear Battery Charger Interrupt \r\n");
#else //PCA9421UK-EVM
			PRINTF("\r\n2. VIN Interrupt \r\n");
#endif

			PRINTF("\r\n3. Voltage Regulator Interrupt\r\n");
			PRINTF("\r\n4. Exit\r\n");
			PRINTF("\r\n********************************\r\n");

			do
			{
				PRINTF("\r\nEnter your choice :- ");
				character = GETCHAR();
				character -= 48;
				PRINTF("%d\r\n",character);
				GETCHAR();
				if(character < 1 | character > 4)
					PRINTF("\r\nInvalid Value, Please enter correct value\r\n");

			}while(character < 1 | character > 4);

			switch (character)
			{
			case 1: //System level interrupt
				if((data & PCA9420_SYS_INT_MASK) >> PCA9420_SYS_INT_SHIFT)
				{
					PCA9420_DRV_Read(&pca9420Driver, PCA9420UK_SUB_INT0, &data );
					if((data & PCA9420_TEMP_PREWARN_MASK) >> PCA9420_TEMP_PREWARN_SHIFT)
						PRINTF("\r\n\033[31m Die Temperature is greater than the pre-warning temperature. \033[37m\r\n");
					if((data & PCA9420_THEM_SHDN_MASK) >> PCA9420_THEM_SHDN_SHIFT)
						PRINTF("\r\n\033[31m Die Temperature is greater than the thermal shutdown threshold. \033[37m\r\n");
					if((data & PCA9420_ASYS_PREWARN_MASK) >> PCA9420_ASYS_PREWARN_SHIFT)
						PRINTF("\r\n\033[31m ASYS voltage falls below the threshold set in ASYS pre-warning voltage threshold. \033[37m\r\n");
					if((data & PCA9420_WD_TMR_MASK) >> PCA9420_WD_TMR_SHIFT)
						PRINTF("\r\n\033[31m Watchdog timer has expired \033[37m\r\n");
					if((data & PCA9420_IN_PWR_MASK) >> PCA9420_IN_PWR_SHIFT)
						PRINTF("\r\n\033[31m Invalid input power voltage. \033[37m\r\n");
				}
				else
				{
					PRINTF("\r\n\033[32m No system level interrupt occurred \033[37m\r\n");
				}
				break;
			case 2: //Charger block interrupt
#if (!PCA9421UK_EVM_EN)
				if((data & PCA9420_BAT_INT_MASK) >> PCA9420_BAT_INT_SHIFT)
				{
					PCA9420_DRV_Read(&pca9420Driver, PCA9420UK_SUB_INT1, &data );
					if((data & PCA9420_DEAD_TMR_MASK) >> PCA9420_DEAD_TMR_SHIFT)
						PRINTF("\r\n\033[31m Dead charge timer has expired. \033[37m\r\n");
					if((data & PCA9420_VIN_ILIM_MASK) >> PCA9420_VIN_ILIM_SHIFT)
						PRINTF("\r\n\033[31m Input current limit interrupt occurred. \033[37m\r\n");
					if((data & PCA9420_FAST_TMR_MASK) >> PCA9420_FAST_TMR_SHIFT)
						PRINTF("\r\n\033[31m Fast charging timer has expired. \033[37m\r\n");
					if((data & PCA9420_PREQ_TMR_MASK) >> PCA9420_PREQ_TMR_SHIFT)
						PRINTF("\r\n\033[31m Pre-qualification charging timer has expired. \033[37m\r\n");
					if((data & PCA9420_VBAT_DET_MASK) >> PCA9420_VBAT_DET_SHIFT)
						PRINTF("\r\n\033[31m Battery presence status is changed. \033[37m\r\n");
					if((data & PCA9420_VBAT_OK_MASK) >> PCA9420_VBAT_OK_SHIFT)
						PRINTF("\r\n\033[31m Battery status is changed. \033[37m\r\n");
					if((data & PCA9420_CHG_OK_MASK) >> PCA9420_CHG_OK_SHIFT)
						PRINTF("\r\n\033[31m Charger status has changed. \033[37m\r\n");
				}
				else
				{
					PRINTF("\r\n\033[32m No charger block interrupt occurred \033[37m\r\n");
				}
#else //PCA9421UK-EVM
				if((data & PCA9421_VIN_INT_MASK) >> PCA9421_VIN_INT_SHIFT)
				{
					PCA9420_DRV_Read(&pca9420Driver, PCA9420UK_SUB_INT1, &data );
					if((data & PCA9420_VIN_ILIM_MASK) >> PCA9420_VIN_ILIM_SHIFT)
						PRINTF("\r\n\033[31m Input current limit interrupt occurred. \033[37m\r\n");
				}
				else
				{
					PRINTF("\r\n\033[32m No charger block interrupt occurred \033[37m\r\n");
				}
#endif
				break;
			case 3: //Voltage regulator interrupt
				if(((data & PCA9420_BUCK_INT_MASK) >> PCA9420_BUCK_INT_SHIFT) || ((data & PCA9420_LDO_INT_MASK) >> PCA9420_LDO_INT_SHIFT))
				{
					if((data & PCA9420_VOUTSW1_MASK) >> PCA9420_VOUTSW1_SHIFT)
						PRINTF("\r\n\033[31m SW1 output voltage status has changed. \033[37m\r\n");
					if((data & PCA9420_VOUTSW2_MASK) >> PCA9420_VOUTSW2_SHIFT)
						PRINTF("\r\n\033[31m SW2 output voltage status has changed. \033[37m\r\n");
					if((data & PCA9420_VOUTLDO1_MASK) >> PCA9420_VOUTLDO1_SHIFT)
						PRINTF("\r\n\033[31m LDO1 output voltage status has changed. \033[37m\r\n");
					if((data & PCA9420_VOUTLDO2_MASK) >> PCA9420_VOUTLDO2_SHIFT)
						PRINTF("\r\n\033[31m LDO2 output voltage status has changed. \033[37m\r\n");
				}
				else
				{
					PRINTF("\r\n\033[32m No voltage regulator interrupt occurred \033[37m\r\n");
				}
				break;
			case 4:
				int_status = 0;
				//Clearing the interrupt
				PCA9420_clear_interrupt(&pca9420Driver);
				break;
			default:
				PRINTF("\r\nInvalid option selected\r\n");
				break;
			}
			do
			{
				dummy = GETCHAR();
			} while(dummy != 13);
		}while(int_status);
	}
	else
	{
		PRINTF("\r\n\033[32m No interrupt occurred \033[37m\r\n");
	}
	PRINTF("\r\n*******************************\r\n");
}

static void device_settings()
{
	uint16_t character, data, edrv_setting = 1;

	while(edrv_setting)
	{
		PRINTF("\r\n**********\033[35m DEVICE CONFIGURATION \033[37m**********\r\n");
		PRINTF("\r\n1. VIN input Current Limit Selection\r\n");
		PRINTF("\r\n2. Pre-warning Voltage Threshold on ASYS \r\n");
		PRINTF("\r\n3. ASYS Input Source Selection\r\n");
		PRINTF("\r\n4. VIN Over-Voltage Protection Threshold\r\n");
		PRINTF("\r\n5. VIN Under-Voltage Lockout Threshold\r\n");
		PRINTF("\r\n6. ASYS Under-Voltage Lockout Threshold\r\n");
		PRINTF("\r\n7. Thermal Shutdown Threshold\r\n");
		PRINTF("\r\n8. Die Temperature Warning Threshold\r\n");
		PRINTF("\r\n9. Mode Selection\r\n");
		PRINTF("\r\n10. Long Glitch Timer on ON Key\r\n");
		PRINTF("\r\n11. Reset Watchdog Timer\r\n");
#if (!PCA9421UK_EVM_EN)
		PRINTF("\r\n12. Reset PCA9420UK-EVM\r\n");
#else
		PRINTF("\r\n12. Reset PCA9421UK-EVM\r\n");
#endif
		PRINTF("\r\n13. Exit from Device Configuration Setting\r\n");
		PRINTF("\r\n********************************\r\n");

		do
		{
			PRINTF("\r\nEnter your choice :- ");
			SCANF("%d",&character);
			PRINTF("%d\r\n",character);
			GETCHAR();
			if(character < 1 | character > 13)
				PRINTF("\r\nInvalid Value, Please enter correct value\r\n");
		}while(character < 1 | character > 13);

		switch (character)
		{
		case 1: //VIN input current limit configuration
		{
			enum _pca9420_vin_ilim pBuffer;
			PCA9420_Get_vin_inp_cur(&pca9420Driver, &pBuffer);

			switch(pBuffer)
			{
			case kPCA9420_VinIlim_74_85_98:
				PRINTF("\r\n\033[93m Currently VIN input current limit is 74 mA/85 mA/98 mA. \033[37m\r\n");
				break;
			case kPCA9420_VinIlim_370_425_489:
				PRINTF("\r\n\033[93m Currently VIN input current limit is 370 mA/425 mA/489 mA. \033[37m\r\n");
				break;
			default:
				PRINTF("Error in getting VIN input current limit!!!\r\n");
				break;
			}

			PRINTF("\r\nSelect VIN input current limit: (min/typ/max)------\r\n");
			PRINTF("\r\n1. 74 mA/85 mA/98 mA\r\n");
			PRINTF("\r\n2. 370 mA/425 mA/489 mA\r\n");
			do
			{
				PRINTF("\r\nEnter your choice :- ");
				character = GETCHAR();
				character -= 48;
				PRINTF("%d\r\n",character);
				GETCHAR();
				if(character < 1 | character > 2)
					PRINTF("\r\nInvalid Value, Please enter correct value\r\n");
			}while(character < 1 | character > 2);

			if(character == 1)
				epca9420_vin_ilim = kPCA9420_VinIlim_74_85_98;
			else if(character == 2)
				epca9420_vin_ilim = kPCA9420_VinIlim_370_425_489;

			PCA9420_Set_vin_inp_cur(&pca9420Driver, epca9420_vin_ilim);
			PCA9420_Get_vin_inp_cur(&pca9420Driver, &pBuffer);
			switch(pBuffer)
			{
			case kPCA9420_VinIlim_74_85_98:
				PRINTF("\r\nVIN input current limit is set to 74 mA/85 mA/98 mA. \r\n");
				break;
			case kPCA9420_VinIlim_370_425_489:
				PRINTF("\r\nVIN input current limit is set to 370 mA/425 mA/489 mA. \r\n");
				break;
			default:
				PRINTF("Error in getting VIN input current limit!!!\r\n");
				break;
			}
			break;
		}
		case 2: //ASYS pre-warning voltage threshold
		{
			enum _pca9420_asys_prewarning pBuffer;
			PCA9420_Get_asys_prewarn_vol_tshld(&pca9420Driver, &pBuffer);

			switch(pBuffer)
			{
			case kPCA9420_AsysPreWarn3V3:
				PRINTF("\r\n\033[93m Currently ASYS pre-warning voltage threshold is 3.3 V. \033[37m\r\n");
				break;
			case kPCA9420_AsysPreWarn3V4:
				PRINTF("\r\n\033[93m Currently ASYS pre-warning voltage threshold is 3.4 V. \033[37m\r\n");
				break;
			case kPCA9420_AsysPreWarn3V5:
				PRINTF("\r\n\033[93m Currently ASYS pre-warning voltage threshold is 3.5 V. \033[37m\r\n");
				break;
			case kPCA9420_AsysPreWarn3V6:
				PRINTF("\r\n\033[93m Currently ASYS pre-warning voltage threshold is 3.6 V. \033[37m\r\n");
				break;
			default:
				PRINTF("Error in getting ASYS pre-warning voltage threshold!!!\r\n");
			}

			PRINTF("\r\nSelect pre-warning voltage threshold------\r\n");
			PRINTF("\r\n1. 3.3 V\r\n");
			PRINTF("\r\n2. 3.4 V\r\n");
			PRINTF("\r\n3. 3.5 V\r\n");
			PRINTF("\r\n4. 3.6 V\r\n");

			do
			{
				PRINTF("\r\nEnter your choice :- ");
				character = GETCHAR();
				character -= 48;
				PRINTF("%d\r\n",character);
				GETCHAR();
				if(character < 1 | character > 4)
					PRINTF("\r\nInvalid Value, Please enter correct value\r\n");
			}while(character < 1 | character > 4);

			switch (character)
			{
			case 1:
				epca9420_asys_prewarning = kPCA9420_AsysPreWarn3V3;
				break;
			case 2:
				epca9420_asys_prewarning = kPCA9420_AsysPreWarn3V4;
				break;
			case 3:
				epca9420_asys_prewarning = kPCA9420_AsysPreWarn3V5;
				break;
			case 4:
				epca9420_asys_prewarning = kPCA9420_AsysPreWarn3V6;
				break;
			default:
				PRINTF("\r\nInvalid option selected\r\n");
				break;
			}
			PCA9420_Set_asys_prewarn_vol_tshld(&pca9420Driver, epca9420_asys_prewarning);
			PCA9420_Get_asys_prewarn_vol_tshld(&pca9420Driver, &pBuffer);

			switch(pBuffer)
			{
			case kPCA9420_AsysPreWarn3V3:
				PRINTF("\r\n\033[93m ASYS pre-warning voltage threshold is set to 3.3 V. \033[37m\r\n");
				break;
			case kPCA9420_AsysPreWarn3V4:
				PRINTF("\r\n\033[93m ASYS pre-warning voltage threshold is set to 3.4 V. \033[37m\r\n");
				break;
			case kPCA9420_AsysPreWarn3V5:
				PRINTF("\r\n\033[93m ASYS pre-warning voltage threshold is set to 3.5 V. \033[37m\r\n");
				break;
			case kPCA9420_AsysPreWarn3V6:
				PRINTF("\r\n\033[93m ASYS pre-warning voltage threshold is set to 3.6 V. \033[37m\r\n");
				break;
			default:
				PRINTF("Error in getting ASYS pre-warning voltage threshold!!!\r\n");
			}
			break;
		}
		case 3: //ASYS input source selection
		{
			PRINTF("\r\nSelect ASYS input source selection------\r\n");

#if (!PCA9421UK_EVM_EN) //PCA9420UK-EVM
			{
				PRINTF("\r\n1. Powered by either VBAT or VIN (whichever is greater)\r\n");
				PRINTF("\r\n2. Powered by VBAT \r\n");
				PRINTF("\r\n3. Powered by VIN\r\n");
			}
#else //PCA9421UK-EVM
			{
				PRINTF("\r\n1. Powered by either VIN or VIN_AUX (whichever is greater)\r\n");
				PRINTF("\r\n2. Powered by VIN_AUX \r\n");
				PRINTF("\r\n3. Powered by VIN\r\n");
			}
#endif

			do
			{
				PRINTF("\r\nEnter your choice :- ");
				character = GETCHAR();
				character -= 48;
				PRINTF("%d\r\n",character);
				GETCHAR();
				if(character < 1 | character > 3)
					PRINTF("\r\nInvalid Value, Please enter correct value\r\n");
			}while(character < 1 | character > 3);

			switch (character)
			{
			case 1:
				epca9420_asys_input_sel = kPCA9420_AsysInputSelVbatVin;
				break;
			case 2:
				epca9420_asys_input_sel = kPCA9420_AsysInputSelVbat;
				break;
			case 3:
				epca9420_asys_input_sel = kPCA9420_AsysInputSelVin;
				break;
			default:
				PRINTF("\r\nInvalid option selected\r\n");
				break;
			}
			PCA9420_asys_input_src_sel(&pca9420Driver, epca9420_asys_input_sel);
			break;
		}
		case 4: //VIN over-voltage protection threshold
		{
			enum _pca9420_vin_ovp_sel pBuffer;
			PCA9420_Get_vin_overvoltage_tshld(&pca9420Driver, &pBuffer);

			switch(pBuffer)
			{
			case kPCA9420_VinOvpSel5V5:
				PRINTF("\r\n\033[93m Currently VIN over-voltage protection threshold is 5.5 V. \033[37m\r\n");
				break;
			case kPCA9420_VinOvpSel6V0:
				PRINTF("\r\n\033[93m Currently VIN over-voltage protection threshold is 6.0 V. \033[37m\r\n");
				break;
			default:
				PRINTF("Error in getting VIN over-voltage protection threshold!!!\r\n");
			}

			PRINTF("\r\nSelect VIN over-voltage protection threshold------\r\n");
			PRINTF("\r\n1. 5.5 V\r\n");
			PRINTF("\r\n2. 6.0 V\r\n");

			do
			{
				PRINTF("\r\nEnter your choice :- ");
				character = GETCHAR();
				character -= 48;
				PRINTF("%d\r\n",character);
				GETCHAR();
				if(character < 1 | character > 2)
					PRINTF("\r\nInvalid Value, Please enter correct value\r\n");
			}while(character < 1 | character > 2);

			switch (character)
			{
			case 1:
				epca9420_vin_ovp = kPCA9420_VinOvpSel5V5;
				break;
			case 2:
				epca9420_vin_ovp = kPCA9420_VinOvpSel6V0;
				break;
			default:
				PRINTF("\r\nInvalid option selected\r\n");
				break;
			}
			PCA9420_Set_vin_overvoltage_tshld(&pca9420Driver, epca9420_vin_ovp);
			PCA9420_Get_vin_overvoltage_tshld(&pca9420Driver, &pBuffer);

			switch(pBuffer)
			{
			case kPCA9420_VinOvpSel5V5:
				PRINTF("\r\n\033[93m VIN over-voltage protection threshold is set to 5.5 V. \033[37m\r\n");
				break;
			case kPCA9420_VinOvpSel6V0:
				PRINTF("\r\n\033[93m VIN over-voltage protection threshold is set to 6.0 V. \033[37m\r\n");
				break;
			default:
				PRINTF("Error in getting VIN over-voltage protection threshold!!!\r\n");
			}
			break;
		}
		case 5: //VIN under-voltage lockout threshold
		{
			enum _pca9420_vin_uvlo_sel pBuffer;
			PCA9420_Get_vin_uvlo_tshld(&pca9420Driver, &pBuffer);

			switch(pBuffer)
			{
			case kPCA9420_VinUvloSel2V9:
				PRINTF("\r\n\033[93m Currently VIN under-voltage lockout threshold is 2.9 V. \033[37m\r\n");
				break;
			case kPCA9420_VinUvloSel3V1:
				PRINTF("\r\n\033[93m Currently VIN under-voltage lockout threshold is 3.1 V. \033[37m\r\n");
				break;
			case kPCA9420_VinUvloSel3V3:
				PRINTF("\r\n\033[93m Currently VIN under-voltage lockout threshold is 3.3 V. \033[37m\r\n");
				break;
			case kPCA9420_VinUvloSel3V5:
				PRINTF("\r\n\033[93m Currently VIN under-voltage lockout threshold is 3.5 V. \033[37m\r\n");
				break;
			default:
				PRINTF("Error in getting VIN under-voltage lockout threshold!!!\r\n");
			}

			PRINTF("\r\nSelect VIN under-voltage lockout threshold------\r\n");
			PRINTF("\r\n1. 2.9 V\r\n");
			PRINTF("\r\n2. 3.1 V\r\n");
			PRINTF("\r\n3. 3.3 V\r\n");
			PRINTF("\r\n4. 3.5 V\r\n");

			do
			{
				PRINTF("\r\nEnter your choice :- ");
				character = GETCHAR();
				character -= 48;
				PRINTF("%d\r\n",character);
				GETCHAR();
				if(character < 1 | character > 4)
					PRINTF("\r\nInvalid Value, Please enter correct value\r\n");
			}while(character < 1 | character > 4);

			switch (character)
			{
			case 1:
				epca9420_vin_uvlo = kPCA9420_VinUvloSel2V9;
				break;
			case 2:
				epca9420_vin_uvlo = kPCA9420_VinUvloSel3V1;
				break;
			case 3:
				epca9420_vin_uvlo = kPCA9420_VinUvloSel3V3;
				break;
			case 4:
				epca9420_vin_uvlo = kPCA9420_VinUvloSel3V5;
				break;
			default:
				PRINTF("\r\nInvalid option selected\r\n");
				break;
			}
			PCA9420_Set_vin_uvlo_tshld(&pca9420Driver, epca9420_vin_uvlo);
			PCA9420_Get_vin_uvlo_tshld(&pca9420Driver, &pBuffer);

			switch(pBuffer)
			{
			case kPCA9420_VinUvloSel2V9:
				PRINTF("\r\n\033[93m VIN under-voltage lockout threshold is set to 2.9 V. \033[37m\r\n");
				break;
			case kPCA9420_VinUvloSel3V1:
				PRINTF("\r\n\033[93m VIN under-voltage lockout threshold is set to 3.1 V. \033[37m\r\n");
				break;
			case kPCA9420_VinUvloSel3V3:
				PRINTF("\r\n\033[93m VIN under-voltage lockout threshold is set to 3.3 V. \033[37m\r\n");
				break;
			case kPCA9420_VinUvloSel3V5:
				PRINTF("\r\n\033[93m VIN under-voltage lockout threshold is set to 3.5 V. \033[37m\r\n");
				break;
			default:
				PRINTF("Error in getting VIN under-voltage lockout threshold!!!\r\n");
			}
			break;
		}
		case 6: //ASYS under-voltage lockout threshold
		{
			enum _pca9420_asys_uvlo_sel pBuffer;
			PCA9420_Get_asys_uvlo_tshld(&pca9420Driver, &pBuffer);

			switch(pBuffer)
			{
			case kPCA9420_AsysUvloSel2V4:
				PRINTF("\r\n\033[93m Currently ASYS under-voltage lockout threshold is 2.4 V. \033[37m\r\n");
				break;
			case kPCA9420_AsysUvloSel2V5:
				PRINTF("\r\n\033[93m Currently ASYS under-voltage lockout threshold is 2.5 V. \033[37m\r\n");
				break;
			case kPCA9420_AsysUvloSel2V6:
				PRINTF("\r\n\033[93m Currently ASYS under-voltage lockout threshold is 2.6 V. \033[37m\r\n");
				break;
			case kPCA9420_AsysUvloSel2V7:
				PRINTF("\r\n\033[93m Currently ASYS under-voltage lockout threshold is 2.7 V. \033[37m\r\n");
				break;
			default:
				PRINTF("Error in getting ASYS under-voltage lockout threshold!!!\r\n");
			}

			PRINTF("\r\nSelect ASYS under-voltage lockout threshold------\r\n");
			PRINTF("\r\n1. 2.4 V\r\n");
			PRINTF("\r\n2. 2.5 V\r\n");
			PRINTF("\r\n3. 2.6 V\r\n");
			PRINTF("\r\n4. 2.7 V\r\n");

			do
			{
				PRINTF("\r\nEnter your choice :- ");
				character = GETCHAR();
				character -= 48;
				PRINTF("%d\r\n",character);
				GETCHAR();
				if(character < 1 | character > 4)
					PRINTF("\r\nInvalid Value, Please enter correct value\r\n");
			}while(character < 1 | character > 4);

			switch (character)
			{
			case 1:
				epca9420_asys_uvlo = kPCA9420_AsysUvloSel2V4;
				break;
			case 2:
				epca9420_asys_uvlo = kPCA9420_AsysUvloSel2V5;
				break;
			case 3:
				epca9420_asys_uvlo = kPCA9420_AsysUvloSel2V6;
				break;
			case 4:
				epca9420_asys_uvlo = kPCA9420_AsysUvloSel2V7;
				break;
			default:
				PRINTF("\r\nInvalid option selected\r\n");
				break;
			}
			PCA9420_Set_asys_uvlo_tshld(&pca9420Driver, epca9420_asys_uvlo);
			PCA9420_Get_asys_uvlo_tshld(&pca9420Driver, &pBuffer);

			switch(pBuffer)
			{
			case kPCA9420_AsysUvloSel2V4:
				PRINTF("\r\n\033[93m ASYS under-voltage lockout threshold is set to 2.4 V. \033[37m\r\n");
				break;
			case kPCA9420_AsysUvloSel2V5:
				PRINTF("\r\n\033[93m ASYS under-voltage lockout threshold is set to 2.5 V. \033[37m\r\n");
				break;
			case kPCA9420_AsysUvloSel2V6:
				PRINTF("\r\n\033[93m ASYS under-voltage lockout threshold is set to 2.6 V. \033[37m\r\n");
				break;
			case kPCA9420_AsysUvloSel2V7:
				PRINTF("\r\n\033[93m ASYS under-voltage lockout threshold is set to 2.7 V. \033[37m\r\n");
				break;
			default:
				PRINTF("Error in getting ASYS under-voltage lockout threshold!!!\r\n");
			}
			break;
		}
		case 7: //Thermal shutdown threshold configuration
		{
			enum _pca9420_them_shdn pBuffer;
			PCA9420_Get_thrml_shtdn_tshld(&pca9420Driver, &pBuffer);

			switch(pBuffer)
			{
			case kPCA9420_ThemShdn95C:
				PRINTF("\r\n\033[93m Currently thermal shutdown threshold is 95 C. \033[37m\r\n");
				break;
			case kPCA9420_ThemShdn100C:
				PRINTF("\r\n\033[93m Currently thermal shutdown threshold is 100 C. \033[37m\r\n");
				break;
			case kPCA9420_ThemShdn105C:
				PRINTF("\r\n\033[93m Currently thermal shutdown threshold is 105 C. \033[37m\r\n");
				break;
			case kPCA9420_ThemShdn110C:
				PRINTF("\r\n\033[93m Currently thermal shutdown threshold is 110 C. \033[37m\r\n");
				break;
			case kPCA9420_ThemShdn115C:
				PRINTF("\r\n\033[93m Currently thermal shutdown threshold is 115 C. \033[37m\r\n");
				break;
			case kPCA9420_ThemShdn120C:
				PRINTF("\r\n\033[93m Currently thermal shutdown threshold is 120 C. \033[37m\r\n");
				break;
			case kPCA9420_ThemShdn125C:
				PRINTF("\r\n\033[93m Currently thermal shutdown threshold is 125 C. \033[37m\r\n");
				break;

			default:
				PRINTF("Error in thermal shutdown threshold!!!\r\n");
			}

			PRINTF("\r\nSelect thermal shutdown threshold (hysteresis with 20 C)------\r\n");
			PRINTF("\r\n1. 95 C\r\n");
			PRINTF("\r\n2. 100 C\r\n");
			PRINTF("\r\n3. 105 C\r\n");
			PRINTF("\r\n4. 110 C\r\n");
			PRINTF("\r\n5. 115 C\r\n");
			PRINTF("\r\n6. 120 C\r\n");
			PRINTF("\r\n7. 125 C\r\n");

			do
			{
				PRINTF("\r\nEnter your choice :- ");
				character = GETCHAR();
				character -= 48;
				PRINTF("%d\r\n",character);
				GETCHAR();
				if(character < 1 | character > 7)
					PRINTF("\r\nInvalid Value, Please enter correct value\r\n");
			}while(character < 1 | character > 7);

			switch (character)
			{
			case 1:
				epca9420_them_shdn = kPCA9420_ThemShdn95C;
				break;
			case 2:
				epca9420_them_shdn = kPCA9420_ThemShdn100C;
				break;
			case 3:
				epca9420_them_shdn = kPCA9420_ThemShdn105C;
				break;
			case 4:
				epca9420_them_shdn = kPCA9420_ThemShdn110C;
				break;
			case 5:
				epca9420_them_shdn = kPCA9420_ThemShdn115C;
				break;
			case 6:
				epca9420_them_shdn = kPCA9420_ThemShdn120C;
				break;
			case 7:
				epca9420_them_shdn = kPCA9420_ThemShdn125C;
				break;
			default:
				PRINTF("\r\nInvalid option selected\r\n");
				break;
			}

			PCA9420_Set_thrml_shtdn_tshld(&pca9420Driver, epca9420_them_shdn);
			PCA9420_Get_thrml_shtdn_tshld(&pca9420Driver, &pBuffer);

			switch(pBuffer)
			{
			case kPCA9420_ThemShdn95C:
				PRINTF("\r\n\033[93m Thermal shutdown threshold is set to 95 C. \033[37m\r\n");
				break;
			case kPCA9420_ThemShdn100C:
				PRINTF("\r\n\033[93m Thermal shutdown threshold is set to 100 C. \033[37m\r\n");
				break;
			case kPCA9420_ThemShdn105C:
				PRINTF("\r\n\033[93m Thermal shutdown threshold is set to 105 C. \033[37m\r\n");
				break;
			case kPCA9420_ThemShdn110C:
				PRINTF("\r\n\033[93m Thermal shutdown threshold is set to 110 C. \033[37m\r\n");
				break;
			case kPCA9420_ThemShdn115C:
				PRINTF("\r\n\033[93m Thermal shutdown threshold is set to 115 C. \033[37m\r\n");
				break;
			case kPCA9420_ThemShdn120C:
				PRINTF("\r\n\033[93m Thermal shutdown threshold is set to 120 C. \033[37m\r\n");
				break;
			case kPCA9420_ThemShdn125C:
				PRINTF("\r\n\033[93m Thermal shutdown threshold is set to 125 C. \033[37m\r\n");
				break;
			default:
				PRINTF("Error in thermal shutdown threshold!!!\r\n");
			}
			break;
		}
		case 8: //Die temperature warning threshold configuration
		{
			enum _pca9420_die_temp_warning pBuffer;
			PCA9420_Get_die_temp_tshld(&pca9420Driver, &pBuffer);

			switch(pBuffer)
			{
			case kPCA9420_DieTempWarn75C:
				PRINTF("\r\n\033[93m Currently Die temperature warning threshold is 75 C. \033[37m\r\n");
				break;
			case kPCA9420_DieTempWarn80C:
				PRINTF("\r\n\033[93m Currently Die temperature warning threshold is 80 C. \033[37m\r\n");
				break;
			case kPCA9420_DieTempWarn85C:
				PRINTF("\r\n\033[93m Currently Die temperature warning threshold is 85 C. \033[37m\r\n");
				break;
			case kPCA9420_DieTempWarn90C:
				PRINTF("\r\n\033[93m Currently Die temperature warning threshold is 90 C. \033[37m\r\n");
				break;
			default:
				PRINTF("Error in getting Die temperature warning threshold!!!\r\n");
			}

			PRINTF("\r\nSelect Die temperature warning threshold------\r\n");
			PRINTF("\r\n1. 75 C\r\n");
			PRINTF("\r\n2. 80 C\r\n");
			PRINTF("\r\n3. 85 C\r\n");
			PRINTF("\r\n4. 90 C\r\n");

			do
			{
				PRINTF("\r\nEnter your choice :- ");
				character = GETCHAR();
				character -= 48;
				PRINTF("%d\r\n",character);
				GETCHAR();
				if(character < 1 | character > 4)
					PRINTF("\r\nInvalid Value, Please enter correct value\r\n");
			}while(character < 1 | character > 4);

			switch (character)
			{
			case 1:
				epca9420_die_temp = kPCA9420_DieTempWarn75C;
				break;
			case 2:
				epca9420_die_temp = kPCA9420_DieTempWarn80C;
				break;
			case 3:
				epca9420_die_temp = kPCA9420_DieTempWarn85C;
				break;
			case 4:
				epca9420_die_temp = kPCA9420_DieTempWarn90C;
				break;
			default:
				PRINTF("\r\nInvalid option selected\r\n");
				break;
			}
			PCA9420_Set_die_temp_tshld(&pca9420Driver, epca9420_die_temp);
			PCA9420_Get_die_temp_tshld(&pca9420Driver, &pBuffer);

			switch(pBuffer)
			{
			case kPCA9420_DieTempWarn75C:
				PRINTF("\r\n\033[93m Die temperature warning threshold is set to 75 C. \033[37m\r\n");
				break;
			case kPCA9420_DieTempWarn80C:
				PRINTF("\r\n\033[93m Die temperature warning threshold is set to 80 C. \033[37m\r\n");
				break;
			case kPCA9420_DieTempWarn85C:
				PRINTF("\r\n\033[93m Die temperature warning threshold is set to 85 C. \033[37m\r\n");
				break;
			case kPCA9420_DieTempWarn90C:
				PRINTF("\r\n\033[93m Die temperature warning threshold is set to 90 C. \033[37m\r\n");
				break;
			default:
				PRINTF("Error in getting Die temperature warning threshold!!!\r\n");
			}
			break;
		}
		case 9: //Mode configuration
		{
			enum _pca9420_mode pBuffer;
			PCA9420_Get_mode_control(&pca9420Driver, &pBuffer);

			switch(pBuffer)
			{
			case kPCA9420_Mode0:
				PRINTF("\r\n\033[93m Currently Device is in MODE 0. \033[37m\r\n");
				break;
			case kPCA9420_Mode1:
				PRINTF("\r\n\033[93m Currently Device is in MODE 1. \033[37m\r\n");
				break;
			case kPCA9420_Mode2:
				PRINTF("\r\n\033[93m Currently Device is in MODE 2. \033[37m\r\n");
				break;
			case kPCA9420_Mode3:
				PRINTF("\r\n\033[93m Currently Device is in MODE 3. \033[37m\r\n");
				break;
			default:
				PRINTF("Error in getting mode setting!!!\r\n");
			}

			PRINTF("\r\nSelect MODE setting for device to operate------\r\n");
			PRINTF("\r\n1. Mode 0\r\n");
			PRINTF("\r\n2. Mode 1\r\n");
			PRINTF("\r\n3. Mode 2\r\n");
			PRINTF("\r\n4. Mode 3\r\n");

			do
			{
				PRINTF("\r\nEnter your choice :- ");
				character = GETCHAR();
				character -= 48;
				PRINTF("%d\r\n",character);
				GETCHAR();
				if(character < 1 | character > 4)
					PRINTF("\r\nInvalid Value, Please enter correct value\r\n");
			}while(character < 1 | character > 4);

			switch (character)
			{
			case 1:
				epca9420_mode = kPCA9420_Mode0;
				break;
			case 2:
				epca9420_mode = kPCA9420_Mode1;
				break;
			case 3:
				epca9420_mode = kPCA9420_Mode2;
				break;
			case 4:
				epca9420_mode = kPCA9420_Mode3;
				break;
			default:
				PRINTF("\r\nInvalid option selected\r\n");
				break;
			}
			PCA9420_Set_mode_control(&pca9420Driver, epca9420_mode);
			PCA9420_Get_mode_control(&pca9420Driver, &pBuffer);

			switch(pBuffer)
			{
			case kPCA9420_Mode0:
				PRINTF("\r\n\033[93m Device is in MODE 0. \033[37m\r\n");
				break;
			case kPCA9420_Mode1:
				PRINTF("\r\n\033[93m Device is in MODE 1. \033[37m\r\n");
				break;
			case kPCA9420_Mode2:
				PRINTF("\r\n\033[93m Device is in MODE 2. \033[37m\r\n");
				break;
			case kPCA9420_Mode3:
				PRINTF("\r\n\033[93m Device is in MODE 3. \033[37m\r\n");
				break;
			default:
				PRINTF("Error in getting mode setting!!!\r\n");
			}
			pmic_status();
			break;
		}
		case 10: //long glitch timer configuration
		{
			enum LongGlitchTimer pBuffer;
			PCA9420_Get_long_glitch_timer(&pca9420Driver, &pBuffer);

			switch(pBuffer)
			{
			case kPCA9420_Mode0:
				PRINTF("\r\n\033[93m Currently long glitch timer on ON key is 4 sec. \033[37m\r\n");
				break;
			case kPCA9420_Mode1:
				PRINTF("\r\n\033[93m Currently long glitch timer on ON key is 8 sec. \033[37m\r\n");
				break;
			case kPCA9420_Mode2:
				PRINTF("\r\n\033[93m Currently long glitch timer on ON key is 12 sec. \033[37m\r\n");
				break;
			case kPCA9420_Mode3:
				PRINTF("\r\n\033[93m Currently long glitch timer on ON key is 16 sec. \033[37m\r\n");
				break;
			default:
				PRINTF("Error in getting long glitch timer setting on ON key!!!\r\n");
			}

			PRINTF("\r\nSelect long glitch timer on ON Key------\r\n");
			PRINTF("\r\n1. 4 sec\r\n");
			PRINTF("\r\n2. 8 sec\r\n");
			PRINTF("\r\n3. 12 sec\r\n");
			PRINTF("\r\n4. 16 sec\r\n");

			do
			{
				PRINTF("\r\nEnter your choice :- ");
				character = GETCHAR();
				character -= 48;
				PRINTF("%d\r\n",character);
				GETCHAR();
				if(character < 1 | character > 4)
					PRINTF("\r\nInvalid Value, Please enter correct value\r\n");
			}while(character < 1 | character > 4);

			switch (character)
			{
			case 1:
				eLngGthTmr = PCA9420_GLTH_Timer_4;
				break;
			case 2:
				eLngGthTmr = PCA9420_GLTH_Timer_8;
				break;
			case 3:
				eLngGthTmr = PCA9420_GLTH_Timer_12;
				break;
			case 4:
				eLngGthTmr = PCA9420_GLTH_Timer_16;
				break;
			default:
				PRINTF("\r\nInvalid option selected\r\n");
				break;
			}

			PCA9420_Set_long_glitch_timer(&pca9420Driver, eLngGthTmr);
			PCA9420_Get_long_glitch_timer(&pca9420Driver, &pBuffer);

			switch(pBuffer)
			{
			case kPCA9420_Mode0:
				PRINTF("\r\n\033[93m Long glitch timer on ON key is set to 4 sec. \033[37m\r\n");
				break;
			case kPCA9420_Mode1:
				PRINTF("\r\n\033[93m Long glitch timer on ON key is set to 8 sec. \033[37m\r\n");
				break;
			case kPCA9420_Mode2:
				PRINTF("\r\n\033[93m Long glitch timer on ON key is set to 12 sec. \033[37m\r\n");
				break;
			case kPCA9420_Mode3:
				PRINTF("\r\n\033[93m Long glitch timer on ON key is set to 16 sec. \033[37m\r\n");
				break;
			default:
				PRINTF("Error in getting long glitch timer setting on ON key!!!\r\n");
			}
			break;
		}
		case 11: // Reset watchdog timer
			PCA9420_wtchdg_timer_reset(&pca9420Driver);
			PRINTF("\r\n********************************\r\n");
			PRINTF("\r\n\033[93m Reset performed on watchdog timer. \033[37m \r\n\r\n");
			PRINTF("\r\n********************************\r\n");
			break;
		case 12: //Reset PCA9420UK-EVB
			PCA9420_SW_reset(&pca9420Driver);
			PRINTF("\r\n********************************\r\n");
			PRINTF("\r\n\033[93m Device reset is done successfully. \033[37m \r\n\r\n");
#if (!PCA9421UK_EVM_EN)
			PRINTF("\r\n\033[32m PCA9420UK-EVM initialized with default configurations. \033[37m \r\n");
#else
			PRINTF("\r\n\033[32m PCA9421UK-EVM initialized with default configurations. \033[37m \r\n");
#endif

			PRINTF("\r\n********************************\r\n");
			break;
		case 13: //Exit from device configuration setting
			edrv_setting = 0;
			break;
		default:
			PRINTF("\r\nInvalid option selected\r\n");
			break;
		}
	}
}

#if !(PCA9421UK_EVM_EN)
static void charging_control()
{
	uint16_t character, offset;
	uint16_t data, echrg_ctrl=1;
	float sw1_vol;

	PCA9420_enable_chg_lock(&pca9420Driver);

	while(echrg_ctrl){
		PRINTF("\r\n********************************\r\n");
		PRINTF("\r\n1. Fast Charge Current\r\n");
		PRINTF("\r\n2. Top-off Current\r\n");
		PRINTF("\r\n3. Pre-charge/Low Battery Charge Current\r\n");
		PRINTF("\r\n4. Dead Battery Charge Current\r\n");
		PRINTF("\r\n5. Fast Charge Timer\r\n");
		PRINTF("\r\n6. Pre-qualification Charge Timer\r\n");
		PRINTF("\r\n7. TOPOFF Timer\r\n");
		PRINTF("\r\n8. External Thermistor Current Selection\r\n");
		PRINTF("\r\n9. Thermistor Beta Value Selection\r\n");
		PRINTF("\r\n10. Thermal Regulation Threshold\r\n");
		PRINTF("\r\n11. Exit Charging control configuration\r\n");
		PRINTF("\r\n********************************\r\n");

		do
		{
			PRINTF("\r\nEnter your choice :- ");
			SCANF("%d",&character);
			PRINTF("%d\r\n",character);
			GETCHAR();
			if(character < 1 | character > 12)
				PRINTF("\r\nInvalid Value, Please enter correct value\r\n");
		}while(character < 1 | character > 12);

		switch (character)
		{
		case 1: //Fast charge current configuration
		{
			do
			{
				PCA9420_DRV_Read(&pca9420Driver,PCA9420UK_CHG_CNTL1, &data);
				data = (data & PCA9420_MODE_ICHG_CC_MASK) >> PCA9420_MODE_ICHG_CC_SHIFT;
				data = data * 5;
				PRINTF("\r\n\033[93m Currently fast charge current is %d mA. \033[37m\r\n", data);

				PRINTF("\r\nEnter fast charge current from 0 mA to 315 mA (in multiple of 5)------\r\n");
				SCANF("%d",&character);
				PRINTF("%d\r\n",character);
				GETCHAR();
				if(isMultipleof5(character) && character <= 315 && character >= 0)
					break;
				else
					PRINTF("\r\nInvalid Value, Please enter correct value\r\n");
			}while(1);

			data = character / 5;
			PCA9420_Set_battery_charge_cur(&pca9420Driver, data);
			PRINTF("\r\n Fast charge current is set to %d mA\r\n",character);
			break;
		}

		case 2: //Top-off current configuration
		{
			PCA9420_DRV_Read(&pca9420Driver,PCA9420UK_CHG_CNTL2, &data);
			data = (data & PCA9420_MODE_ICHG_TOPOFF_MASK) >> PCA9420_MODE_ICHG_TOPOFF_SHIFT;
			PRINTF("\r\n\033[93m Currently top-off current is %d mA. \033[37m\r\n", data);

			do
			{
				PRINTF("\r\nEnter top-off current from 0 mA to 63 mA------\r\n");
				SCANF("%d",&character);
				PRINTF("%d\r\n",character);
				GETCHAR();
				if(character <= 63 && character >= 0)
					break;
				else
					PRINTF("\r\nInvalid Value, Please enter correct value\r\n");
			}while(1);

			PCA9420_Set_battery_topoff_cur(&pca9420Driver, character);
			PRINTF("\r\n Top-off current is set to %d mA\r\n",character);
			break;
		}

		case 3: //pre-charge/low battery charge current configuration
		{
			enum _pca9420_low_bat_chrg_cur pBuffer;
			PCA9420_Get_low_battery_chrg_cur(&pca9420Driver, &pBuffer);

			switch (pBuffer)
			{
			case kPCA9420_ICHG_LOW_0:
				PRINTF("\r\n\033[93m Currently pre-charge current is 0mA. \033[37m\r\n");
				break;
			case kPCA9420_ICHG_LOW_8:
				PRINTF("\r\n\033[93m Currently pre-charge current is 8mA. \033[37m\r\n");
				break;
			case kPCA9420_ICHG_LOW_16:
				PRINTF("\r\n\033[93m Currently pre-charge current is 16mA. \033[37m\r\n");
				break;
			case kPCA9420_ICHG_LOW_24:
				PRINTF("\r\n\033[93m Currently pre-charge current is 24mA. \033[37m\r\n");
				break;
			default:
				PRINTF("\r\n\033[93m Error in pre-charge/low battery charge current!!! \033[37m\r\n");
			}

			PRINTF("\r\nSelect pre-charge(low battery charge) current------\r\n");
			PRINTF("\r\n1. 0 mA\r\n");
			PRINTF("\r\n2. 8 mA\r\n");
			PRINTF("\r\n3. 16 mA\r\n");
			PRINTF("\r\n4. 24 mA\r\n");

			do
			{
				PRINTF("\r\nEnter your choice :- ");
				character = GETCHAR();
				character -= 48;
				PRINTF("%d\r\n",character);
				GETCHAR();
				if(character < 1 | character > 4)
					PRINTF("\r\nInvalid Value, Please enter correct value\r\n");
			}while(character < 1 | character > 4);


			switch (character)
			{
			case 1:
				epca9420_low_bat_chrg_cur = kPCA9420_ICHG_LOW_0;
				break;
			case 2:
				epca9420_low_bat_chrg_cur = kPCA9420_ICHG_LOW_8;
				break;
			case 3:
				epca9420_low_bat_chrg_cur = kPCA9420_ICHG_LOW_16;
				break;
			case 4:
				epca9420_low_bat_chrg_cur = kPCA9420_ICHG_LOW_24;
				break;
			default:
				PRINTF("\r\nInvalid option selected\r\n");
				break;
			}
			PCA9420_Set_low_battery_chrg_cur(&pca9420Driver, epca9420_low_bat_chrg_cur);
			PCA9420_Get_low_battery_chrg_cur(&pca9420Driver, &pBuffer);

			switch (pBuffer)
			{
			case kPCA9420_ICHG_LOW_0:
				PRINTF("\r\nPre-charge current is set to 0mA.\r\n");
				break;
			case kPCA9420_ICHG_LOW_8:
				PRINTF("\r\nPre-charge current is set to 8mA.\r\n");
				break;
			case kPCA9420_ICHG_LOW_16:
				PRINTF("\r\nPre-charge current is set to 16mA.\r\n");
				break;
			case kPCA9420_ICHG_LOW_24:
				PRINTF("\r\nPre-charge current is set to 24mA.\r\n");
				break;
			default:
				PRINTF("\r\nError in pre-charge/low battery charge current!!!\r\n");
			}
			PRINTF("\r\n*************************************\r\n");
			break;
		}
		case 4: // Dead battery charge current configurations
		{
			enum _pca9420_dead_bat_chrg_cur pBuffer;
			PCA9420_Get_dead_chrg_cur(&pca9420Driver, &pBuffer);

			switch (pBuffer)
			{
			case kPCA9420_ICHG_DEAD_0:
				PRINTF("\r\n\033[93m Currently dead battery charge current is 0mA. \033[37m\r\n");
				break;
			case kPCA9420_ICHG_DEAD_4:
				PRINTF("\r\n\033[93m Currently dead battery charge current is 4mA. \033[37m\r\n");
				break;
			case kPCA9420_ICHG_DEAD_16:
				PRINTF("\r\n\033[93m Currently dead battery charge current is 16mA. \033[37m\r\n");
				break;
			case kPCA9420_ICHG_DEAD_20:
				PRINTF("\r\n\033[93m Currently dead battery charge current is 20mA. \033[37m\r\n");
				break;
			default:
				PRINTF("\r\nError in getting dead battery charge current!!!\r\n");
			}

			PRINTF("\r\nSelect Dead battery charge current------\r\n");
			PRINTF("\r\n1. 0 mA\r\n");
			PRINTF("\r\n2. 4 mA\r\n");
			PRINTF("\r\n3. 16 mA\r\n");
			PRINTF("\r\n4. 20 mA\r\n");

			do
			{
				PRINTF("\r\nEnter your choice :- ");
				character = GETCHAR();
				character -= 48;
				PRINTF("%d\r\n",character);
				GETCHAR();
				if(character < 1 | character > 4)
					PRINTF("\r\nInvalid Value, Please enter correct value\r\n");
			}while(character < 1 | character > 4);

			switch (character)
			{
			case 1:
				epca9420_dead_bat_chrg_cur = kPCA9420_ICHG_DEAD_0;
				break;
			case 2:
				epca9420_dead_bat_chrg_cur = kPCA9420_ICHG_DEAD_4;
				break;
			case 3:
				epca9420_dead_bat_chrg_cur = kPCA9420_ICHG_DEAD_16;
				break;
			case 4:
				epca9420_dead_bat_chrg_cur = kPCA9420_ICHG_DEAD_20;
				break;
			default:
				PRINTF("\r\nInvalid option selected\r\n");
				break;
			}
			PRINTF("\r\n********************************\r\n");

			PCA9420_Set_dead_chrg_cur(&pca9420Driver, epca9420_dead_bat_chrg_cur);
			PCA9420_Get_dead_chrg_cur(&pca9420Driver, &pBuffer);

			switch (pBuffer)
			{
			case kPCA9420_ICHG_DEAD_0:
				PRINTF("\r\nDead battery charge current is set to 0mA.\r\n");
				break;
			case kPCA9420_ICHG_DEAD_4:
				PRINTF("\r\nDead battery charge current is set to 4mA.\r\n");
				break;
			case kPCA9420_ICHG_DEAD_16:
				PRINTF("\r\nDead battery charge current is set to 16mA.\r\n");
				break;
			case kPCA9420_ICHG_DEAD_20:
				PRINTF("\r\nDead battery charge current is set to 20mA.\r\n");
				break;
			default:
				PRINTF("\r\nError in getting dead battery charge current!!!\r\n");
			}
			break;
		}
		case 5: // Fast charge timer configurations
		{
			enum _pca9420_fast_chrg_timer pBuffer;
			PCA9420_Get_fast_chrg_timer(&pca9420Driver, &pBuffer);

			switch (pBuffer)
			{
			case kPCA9420_ICHG_FAST_TIMER_3:
				PRINTF("\r\n\033[93m Currently fast charge timer is 3 hr. \033[37m \r\n");
				break;
			case kPCA9420_ICHG_FAST_TIMER_5:
				PRINTF("\r\n\033[93m Currently fast charge timer is 5 hr. \033[37m\r\n");
				break;
			case kPCA9420_ICHG_FAST_TIMER_7:
				PRINTF("\r\n\033[93m Currently fast charge timer is 7 hr. \033[37m\r\n");
				break;
			case kPCA9420_ICHG_FAST_TIMER_9:
				PRINTF("\r\n\033[93m  Currently fast charge timer is 9 hr. \033[37m\r\n");
				break;
			default:
				PRINTF("\r\nError in getting fast charge timer value!!!\r\n");
			}

			PRINTF("\r\nSelect Fast charge Timer------\r\n");
			PRINTF("\r\n1. 3 hour\r\n");
			PRINTF("\r\n2. 5 hour\r\n");
			PRINTF("\r\n3. 7 hour\r\n");
			PRINTF("\r\n4. 9 hour\r\n");

			do
			{
				PRINTF("\r\nEnter your choice :- ");
				character = GETCHAR();
				character -= 48;
				PRINTF("%d\r\n",character);
				GETCHAR();
				if(character < 1 | character > 4)
					PRINTF("\r\nInvalid Value, Please enter correct value\r\n");
			}while(character < 1 | character > 4);


			switch (character)
			{
			case 1:
				epca9420_fast_chrg_timer = kPCA9420_ICHG_FAST_TIMER_3;
				break;
			case 2:
				epca9420_fast_chrg_timer = kPCA9420_ICHG_FAST_TIMER_5;
				break;
			case 3:
				epca9420_fast_chrg_timer = kPCA9420_ICHG_FAST_TIMER_7;
				break;
			case 4:
				epca9420_fast_chrg_timer = kPCA9420_ICHG_FAST_TIMER_9;
				break;
			default:
				PRINTF("\r\nInvalid option selected\r\n");
				break;
			}
			PRINTF("\r\n********************************\r\n");

			PCA9420_Set_fast_chrg_timer(&pca9420Driver, epca9420_fast_chrg_timer);
			PCA9420_Get_fast_chrg_timer(&pca9420Driver, &pBuffer);

			switch (pBuffer)
			{
			case kPCA9420_ICHG_FAST_TIMER_3:
				PRINTF("\r\nFast charge timer is set to 3 hr.\r\n");
				break;
			case kPCA9420_ICHG_FAST_TIMER_5:
				PRINTF("\r\nFast charge timer is set to 5 hr.\r\n");
				break;
			case kPCA9420_ICHG_FAST_TIMER_7:
				PRINTF("\r\nFast charge timer is set to 7 hr.\r\n");
				break;
			case kPCA9420_ICHG_FAST_TIMER_9:
				PRINTF("\r\nFast charge timer is set to 9 hr.\r\n");
				break;
			default:
				PRINTF("\r\nError in getting fast charge timer value!!!\r\n");
			}
			break;
		}
		case 6: // Pre-qualification charge timer
		{
			enum _pca9420_preq_chrg_timer pBuffer;
			PCA9420_Get_preq_timer(&pca9420Driver, &pBuffer);

			switch (pBuffer)
			{
			case kPCA9420_ICHG_PREQ_TIMER_15:
				PRINTF("\r\n\033[93m Currently pre-qualification timer is 15 min. \033[37m\r\n");
				break;
			case kPCA9420_ICHG_PREQ_TIMER_30:
				PRINTF("\r\n\033[93m Currently pre-qualification timer is 30 min. \033[37m\r\n");
				break;
			case kPCA9420_ICHG_PREQ_TIMER_45:
				PRINTF("\r\n\033[93m Currently pre-qualification timer is 45 min. \033[37m\r\n");
				break;
			case kPCA9420_ICHG_PREQ_TIMER_60:
				PRINTF("\r\n\033[93m Currently pre-qualification timer is 60 min. \033[37m\r\n");
				break;
			default:
				PRINTF("\r\nError in getting pre-qualification timer value!!!\r\n");
			}

			PRINTF("\r\nSelect Pre-qualification charge Timer------\r\n");
			PRINTF("\r\n1. 15 min\r\n");
			PRINTF("\r\n2. 30 min\r\n");
			PRINTF("\r\n3. 45 min\r\n");
			PRINTF("\r\n4. 60 min\r\n");

			do
			{
				PRINTF("\r\nEnter your choice :- ");
				character = GETCHAR();
				character -= 48;
				PRINTF("%d\r\n",character);
				GETCHAR();
				if(character < 1 | character > 4)
					PRINTF("\r\nInvalid Value, Please enter correct value\r\n");
			}while(character < 1 | character > 4);

			switch (character)
			{
			case 1:
				epca9420_preq_chrg_timer = kPCA9420_ICHG_PREQ_TIMER_15;
				break;
			case 2:
				epca9420_preq_chrg_timer = kPCA9420_ICHG_PREQ_TIMER_30;
				break;
			case 3:
				epca9420_preq_chrg_timer = kPCA9420_ICHG_PREQ_TIMER_45;
				break;
			case 4:
				epca9420_preq_chrg_timer = kPCA9420_ICHG_PREQ_TIMER_60;
				break;
			default:
				PRINTF("\r\nInvalid option selected\r\n");
				break;
			}
			PRINTF("\r\n********************************\r\n");

			PCA9420_Set_preq_timer(&pca9420Driver, epca9420_preq_chrg_timer);
			PCA9420_Get_preq_timer(&pca9420Driver, &pBuffer);

			switch (pBuffer)
			{
			case kPCA9420_ICHG_PREQ_TIMER_15:
				PRINTF("\r\nPre-qualification timer is set to 15 min.\r\n");
				break;
			case kPCA9420_ICHG_PREQ_TIMER_30:
				PRINTF("\r\nPre-qualification timer is set to 30 min.\r\n");
				break;
			case kPCA9420_ICHG_PREQ_TIMER_45:
				PRINTF("\r\nPre-qualification timer is set to 45 min.\r\n");
				break;
			case kPCA9420_ICHG_PREQ_TIMER_60:
				PRINTF("\r\nPre-qualification timer is set to 60 min.\r\n");
				break;
			default:
				PRINTF("\r\nError in getting pre-qualification timer value!!!\r\n");
			}
			break;
		}
		case 7: //Topoff timer configurations
		{
			enum _pca9420_topoff_timer pBuffer;
			PCA9420_Get_topoff_timer(&pca9420Driver, &pBuffer);

			switch (pBuffer)
			{
			case kPCA9420_TOPOFF_TIMER_0:
				PRINTF("\r\n\033[93m Currently TOPOFF timer is 0 min. \033[37m\r\n");
				break;
			case kPCA9420_TOPOFF_TIMER_6_4:
				PRINTF("\r\n\033[93m Currently TOPOFF timer is 6.4 min. \033[37m\r\n");
				break;
			case kPCA9420_TOPOFF_TIMER_12_8:
				PRINTF("\r\n\033[93m Currently TOPOFF timer is 12.8 min. \033[37m\r\n");
				break;
			case kPCA9420_TOPOFF_TIMER_19_2:
				PRINTF("\r\n\033[93m Currently TOPOFF timer is 19.2 min. \033[37m\r\n");
				break;
			default:
				PRINTF("\r\nError in getting TOPOFF timer value!!!\r\n");
			}

			PRINTF("\r\nSelect TOPOFF Timer------\r\n");
			PRINTF("\r\n1. 0 min\r\n");
			PRINTF("\r\n2. 6.4 min\r\n");
			PRINTF("\r\n3. 12.8 min\r\n");
			PRINTF("\r\n4. 19.2 min\r\n");

			do
			{
				PRINTF("\r\nEnter your choice :- ");
				character = GETCHAR();
				character -= 48;
				PRINTF("%d\r\n",character);
				GETCHAR();
				if(character < 1 | character > 4)
					PRINTF("\r\nInvalid Value, Please enter correct value\r\n");
			}while(character < 1 | character > 4);


			switch (character)
			{
			case 1:
				epca9420_topoff_timer = kPCA9420_TOPOFF_TIMER_0;
				break;
			case 2:
				epca9420_topoff_timer = kPCA9420_TOPOFF_TIMER_6_4;
				break;
			case 3:
				epca9420_topoff_timer = kPCA9420_TOPOFF_TIMER_12_8;
				break;
			case 4:
				epca9420_topoff_timer = kPCA9420_TOPOFF_TIMER_19_2;
				break;
			default:
				PRINTF("\r\nInvalid option selected\r\n");
				break;
			}
			PRINTF("\r\n********************************\r\n");

			PCA9420_Set_topoff_timer(&pca9420Driver, epca9420_topoff_timer);
			PCA9420_Get_topoff_timer(&pca9420Driver, &pBuffer);

			switch (pBuffer)
			{
			case kPCA9420_TOPOFF_TIMER_0:
				PRINTF("\r\nTOPOFF timer is set to 0 min.\r\n");
				break;
			case kPCA9420_TOPOFF_TIMER_6_4:
				PRINTF("\r\nTOPOFF timer is set to 6.4 min.\r\n");
				break;
			case kPCA9420_TOPOFF_TIMER_12_8:
				PRINTF("\r\nTOPOFF timer is set to 12.8 min.\r\n");
				break;
			case kPCA9420_TOPOFF_TIMER_19_2:
				PRINTF("\r\nTOPOFF timer is set to 19.2 min.\r\n");
				break;
			default:
				PRINTF("\r\nError in getting TOPOFF timer value!!!\r\n");
			}
			break;
		}
		case 8: //NTC thermistor resistance configuration
		{
			enum _pca9420_ntc_res_sel pBuffer;
			PCA9420_Get_thmstr_resistance(&pca9420Driver, &pBuffer);

			switch (pBuffer)
			{
			case kPCA9420_NTC_RES_SEL_100:
				PRINTF("\r\nCurrently thermistor resistance(thermistor bias current) is 100 kO (Current = 5  A).\r\n");
				break;
			case kPCA9420_NTC_RES_SEL_10:
				PRINTF("\r\nCurrently thermistor resistance(thermistor bias current) is 10 kO (Current = 50  A).\r\n");
				break;
			default:
				PRINTF("\r\nInvalid option selected\r\n");
				break;
			}

			PRINTF("\r\nSelect Thermistor resistance (NTC thermistor bias current)------\r\n");
			PRINTF("\r\n1. 100 kO (Current = 5  A)\r\n");
			PRINTF("\r\n2. 10 kO (Current = 50  A)\r\n");

			do
			{
				PRINTF("\r\nEnter your choice :- ");
				character = GETCHAR();
				character -= 48;
				PRINTF("%d\r\n",character);
				GETCHAR();
				if(character < 1 | character > 2)
					PRINTF("\r\nInvalid Value, Please enter correct value\r\n");
			}while(character < 1 | character > 2);


			switch (character)
			{
			case 1:
				epca9420_ntc_res_sel = kPCA9420_NTC_RES_SEL_100;
				break;
			case 2:
				epca9420_ntc_beta_val = kPCA9420_NTC_RES_SEL_10;
				break;
			default:
				PRINTF("\r\nInvalid option selected\r\n");
				break;
			}
			PRINTF("\r\n********************************\r\n");

			PCA9420_Set_thmstr_resistance(&pca9420Driver, epca9420_ntc_beta_val);
			PCA9420_Get_thmstr_resistance(&pca9420Driver, &pBuffer);

			switch (pBuffer)
			{
			case kPCA9420_NTC_RES_SEL_100:
				PRINTF("\r\nThermistor resistance(thermistor bias current) is set to 100 kO (Current = 5  A).\r\n");
				break;
			case kPCA9420_NTC_RES_SEL_10:
				PRINTF("\r\nThermistor resistance(thermistor bias current) is set to 10 kO (Current = 50  A).\r\n");
				break;
			default:
				PRINTF("\r\nInvalid option selected\r\n");
				break;
			}
			break;
		}

		case 9: //Thermistor beta value selection
		{
			enum _pca9420_ntc_beta_val pBuffer;
			PCA9420_Get_ntc_beta_val(&pca9420Driver, &pBuffer);

			switch(pBuffer)
			{
			case kPCA9420_NTC_BETA_SEL_3434:
				PRINTF("\r\nCurrently thermistor beta value is 3434k.\r\n");
				break;
			case kPCA9420_NTC_BETA_SEL_3610:
				PRINTF("\r\nCurrently thermistor beta value is 3610k.\r\n");
				break;
			case kPCA9420_NTC_BETA_SEL_3934:
				PRINTF("\r\nCurrently thermistor beta value is 3934k.\r\n");
				break;
			case kPCA9420_NTC_BETA_SEL_3950:
				PRINTF("\r\nCurrently thermistor beta value is 3950k.\r\n");
				break;
			case kPCA9420_NTC_BETA_SEL_4100:
				PRINTF("\r\nCurrently thermistor beta value is 4100k.\r\n");
				break;
			case kPCA9420_NTC_BETA_SEL_4311:
				PRINTF("\r\nCurrently thermistor beta value is 4311k.\r\n");
				break;
			case kPCA9420_NTC_BETA_SEL_4543:
				PRINTF("\r\nCurrently thermistor beta value is 4543k.\r\n");
				break;
			case kPCA9420_NTC_BETA_SEL_4750:
				PRINTF("\r\nCurrently thermistor beta value is 4750k.\r\n");
				break;
			default:
				PRINTF("\r\nError in getting thermistor beta value!!!\r\n");
			}

			PRINTF("\r\nSelect thermistor beta value------\r\n");
			PRINTF("\r\n1. 3434k\r\n");
			PRINTF("\r\n2. 3610k\r\n");
			PRINTF("\r\n3. 3934k\r\n");
			PRINTF("\r\n4. 3950k\r\n");
			PRINTF("\r\n5. 4100k\r\n");
			PRINTF("\r\n6. 4311k\r\n");
			PRINTF("\r\n7. 4543k\r\n");
			PRINTF("\r\n8. 4750\r\n");

			do
			{
				PRINTF("\r\nEnter your choice :- ");
				character = GETCHAR();
				character -= 48;
				PRINTF("%d\r\n",character);
				GETCHAR();
				if(character < 1 | character > 8)
					PRINTF("\r\nInvalid Value, Please enter correct value\r\n");
			}while(character < 1 | character > 8);

			switch (character)
			{
			case 1:
				epca9420_ntc_beta_val = kPCA9420_NTC_BETA_SEL_3434;
				break;
			case 2:
				epca9420_ntc_beta_val = kPCA9420_NTC_BETA_SEL_3610;
				break;
			case 3:
				epca9420_ntc_beta_val = kPCA9420_NTC_BETA_SEL_3934;
				break;
			case 4:
				epca9420_ntc_beta_val = kPCA9420_NTC_BETA_SEL_3950;
				break;
			case 5:
				epca9420_ntc_beta_val = kPCA9420_NTC_BETA_SEL_4100;
				break;
			case 6:
				epca9420_ntc_beta_val = kPCA9420_NTC_BETA_SEL_4311;
				break;
			case 7:
				epca9420_ntc_beta_val = kPCA9420_NTC_BETA_SEL_4543;
				break;
			case 8:
				epca9420_ntc_beta_val = kPCA9420_NTC_BETA_SEL_4750;
				break;
			default:
				PRINTF("\r\nInvalid option selected\r\n");
				break;
			}
			PRINTF("\r\n********************************\r\n");

			PCA9420_Set_ntc_beta_val(&pca9420Driver, epca9420_ntc_beta_val);
			PCA9420_Get_ntc_beta_val(&pca9420Driver, &pBuffer);

			switch(pBuffer)
			{
			case kPCA9420_NTC_BETA_SEL_3434:
				PRINTF("\r\nThermistor beta value is set to 3434k.\r\n");
				break;
			case kPCA9420_NTC_BETA_SEL_3610:
				PRINTF("\r\nThermistor beta value is set to 3610k.\r\n");
				break;
			case kPCA9420_NTC_BETA_SEL_3934:
				PRINTF("\r\nThermistor beta value is set to 3934k.\r\n");
				break;
			case kPCA9420_NTC_BETA_SEL_3950:
				PRINTF("\r\nThermistor beta value is set to 3950k.\r\n");
				break;
			case kPCA9420_NTC_BETA_SEL_4100:
				PRINTF("\r\nThermistor beta value is set to 4100k.\r\n");
				break;
			case kPCA9420_NTC_BETA_SEL_4311:
				PRINTF("\r\nThermistor beta value is set to 4311k.\r\n");
				break;
			case kPCA9420_NTC_BETA_SEL_4543:
				PRINTF("\r\nThermistor beta value is set to 4543k.\r\n");
				break;
			case kPCA9420_NTC_BETA_SEL_4750:
				PRINTF("\r\nThermistor beta value is set to 4750k.\r\n");
				break;
			default:
				PRINTF("\r\nError in getting thermistor beta value!!!\r\n");
			}
			break;
		}
		case 10: //thermal regulation threshold configuration
		{
			enum _pca9420_thrml_reg_thshld pBuffer;
			PCA9420_Get_thrml_reg_thshld(&pca9420Driver, &pBuffer);

			switch(pBuffer)
			{
			case kPCA9420_THM_REG_80:
				PRINTF("\r\nCurrently thermal regulation threshold is 80  C.\r\n");
				break;
			case kPCA9420_THM_REG_85:
				PRINTF("\r\nCurrently thermal regulation threshold is 85 C.\r\n");
				break;
			case kPCA9420_THM_REG_90:
				PRINTF("\r\nCurrently thermal regulation threshold is 90 C.\r\n");
				break;
			case kPCA9420_THM_REG_95:
				PRINTF("\r\nCurrently thermal regulation threshold is 95 C.\r\n");
				break;
			case kPCA9420_THM_REG_100:
				PRINTF("\r\nCurrently thermal regulation threshold is 100 C.\r\n");
				break;
			case kPCA9420_THM_REG_105:
				PRINTF("\r\nCurrently thermal regulation threshold is 105 C.\r\n");
				break;
			case kPCA9420_THM_REG_110:
				PRINTF("\r\nCurrently thermal regulation threshold is 110 C.\r\n");
				break;
			case kPCA9420_THM_REG_115:
				PRINTF("\r\nCurrently thermal regulation threshold is 115 C.\r\n");
				break;
			default:
				PRINTF("\r\nError in getting thermal regulation threshold value!!!\r\n");
			}

			PRINTF("\r\nSelect thermal regulation threshold value------\r\n");
			PRINTF("\r\n1. 80 C\r\n");
			PRINTF("\r\n2. 85 C\r\n");
			PRINTF("\r\n3. 90 C\r\n");
			PRINTF("\r\n4. 95 C\r\n");
			PRINTF("\r\n5. 100 C\r\n");
			PRINTF("\r\n6. 105 C\r\n");
			PRINTF("\r\n7. 110 C\r\n");
			PRINTF("\r\n8. 115 C\r\n");

			do
			{
				PRINTF("\r\nEnter your choice :- ");
				character = GETCHAR();
				character -= 48;
				PRINTF("%d\r\n",character);
				GETCHAR();
				if(character < 1 | character > 8)
					PRINTF("\r\nInvalid Value, Please enter correct value\r\n");
			}while(character < 1 | character > 8);

			switch (character)
			{
			case 1:
				epca9420_thrml_reg_thshld = kPCA9420_THM_REG_80;
				break;
			case 2:
				epca9420_thrml_reg_thshld = kPCA9420_THM_REG_85;
				break;
			case 3:
				epca9420_thrml_reg_thshld = kPCA9420_THM_REG_90;
				break;
			case 4:
				epca9420_thrml_reg_thshld = kPCA9420_THM_REG_95;
				break;
			case 5:
				epca9420_thrml_reg_thshld = kPCA9420_THM_REG_100;
				break;
			case 6:
				epca9420_thrml_reg_thshld = kPCA9420_THM_REG_105;
				break;
			case 7:
				epca9420_thrml_reg_thshld = kPCA9420_THM_REG_110;
				break;
			case 8:
				epca9420_thrml_reg_thshld = kPCA9420_THM_REG_115;
				break;
			default:
				PRINTF("\r\nInvalid option selected\r\n");
				break;
			}
			PRINTF("\r\n********************************\r\n");

			PCA9420_Set_thrml_reg_thshld(&pca9420Driver, epca9420_thrml_reg_thshld);
			PCA9420_Get_thrml_reg_thshld(&pca9420Driver, &pBuffer);

			switch(pBuffer)
			{
			case kPCA9420_THM_REG_80:
				PRINTF("\r\nThermal regulation threshold is set to 80 C.\r\n");
				break;
			case kPCA9420_THM_REG_85:
				PRINTF("\r\nThermal regulation threshold is set to 85 C.\r\n");
				break;
			case kPCA9420_THM_REG_90:
				PRINTF("\r\nThermal regulation threshold is set to 90 C.\r\n");
				break;
			case kPCA9420_THM_REG_95:
				PRINTF("\r\nThermal regulation threshold is set to 95 C.\r\n");
				break;
			case kPCA9420_THM_REG_100:
				PRINTF("\r\nThermal regulation threshold is set to 100 C.\r\n");
				break;
			case kPCA9420_THM_REG_105:
				PRINTF("\r\nThermal regulation threshold is set to 105 C.\r\n");
				break;
			case kPCA9420_THM_REG_110:
				PRINTF("\r\nThermal regulation threshold is set to 110 C.\r\n");
				break;
			case kPCA9420_THM_REG_115:
				PRINTF("\r\nThermal regulation threshold is set to 115 C.\r\n");
				break;
			default:
				PRINTF("\r\nError in getting thermal regulation threshold value!!!\r\n");
			}
			break;
		}
		case 11: //Exit from charging control setting
			echrg_ctrl = 0;
			break;
		default: //default case
			PRINTF("\r\nInvalid option selected\r\n");
			break;
		}
	}
}
#endif

static int find_operation()
{
	uint8_t character;

	PRINTF("\r\n1. Enable\r\n");
	PRINTF("\r\n2. Disable\r\n");
	PRINTF("\r\n3. Exit\r\n");

	do
	{
		PRINTF("\r\nEnter your choice :- ");
		character = GETCHAR();
		character -= 48;
		PRINTF("%d\r\n",character);
		GETCHAR();
		if(character < 1 | character > 3)
			PRINTF("\r\nInvalid Value, Please enter correct value\r\n");
	}while(character < 1 | character > 3);

	return character;

}

static void set_sw1_vol(enum _pca9420_mode epca9420_mode)
{
	float voltage;
	uint32_t data;

	do
	{
		PRINTF("\r\nEnter SW1 output voltage from (0.5 V to 1.5 V with 25 mV/step) or fixed 1.8V ---------\r\n");
		SCANF("%f",&voltage);
		PRINTF("%f\r\n",voltage);
		GETCHAR();

		data = voltage * 1000;

		if((voltage == 1.8f) || ((data % 25 == 0) && voltage <= 1.5f && voltage >= 0.500f))
			break;
		else
			PRINTF("\r\nInvalid Value, Please enter correct value\r\n");
	}while(!((voltage == 1.8f) || ((data % 25 == 0) && voltage <= 1.5f && voltage >= 0.500f)));

	if(voltage == 0.500f)
		epca9420_sw1_out = kPCA9420_Sw1OutVolt0V500;
	else if(voltage == 0.525f)
		epca9420_sw1_out = kPCA9420_Sw1OutVolt0V525;
	else if(voltage == 0.550f)
		epca9420_sw1_out = kPCA9420_Sw1OutVolt0V550;
	else if(voltage == 0.575f)
		epca9420_sw1_out = 	kPCA9420_Sw1OutVolt0V575;
	else if(voltage == 0.600f)
		epca9420_sw1_out = kPCA9420_Sw1OutVolt0V600;
	else if(voltage == 0.625f)
		epca9420_sw1_out = kPCA9420_Sw1OutVolt0V625;
	else if(voltage == 0.650f)
		epca9420_sw1_out = kPCA9420_Sw1OutVolt0V650;
	else if(voltage == 0.675f)
		epca9420_sw1_out = kPCA9420_Sw1OutVolt0V675;
	else if(voltage == 0.700f)
		epca9420_sw1_out = kPCA9420_Sw1OutVolt0V700;
	else if(voltage == 0.725f)
		epca9420_sw1_out = kPCA9420_Sw1OutVolt0V725;
	else if(voltage == 0.750f)
		epca9420_sw1_out = kPCA9420_Sw1OutVolt0V750;
	else if(voltage == 0.775f)
		epca9420_sw1_out = kPCA9420_Sw1OutVolt0V775;
	else if(voltage == 0.800f)
		epca9420_sw1_out = kPCA9420_Sw1OutVolt0V800;
	else if(voltage == 0.825f)
		epca9420_sw1_out = kPCA9420_Sw1OutVolt0V825;
	else if(voltage == 0.850f)
		epca9420_sw1_out = kPCA9420_Sw1OutVolt0V850;
	else if(voltage == 0.875f)
		epca9420_sw1_out = kPCA9420_Sw1OutVolt0V875;
	else if(voltage == 0.900f)
		epca9420_sw1_out = kPCA9420_Sw1OutVolt0V900;
	else if(voltage == 0.925f)
		epca9420_sw1_out = kPCA9420_Sw1OutVolt0V925;
	else if(voltage == 0.950f)
		epca9420_sw1_out = kPCA9420_Sw1OutVolt0V950;
	else if(voltage == 0.975f)
		epca9420_sw1_out = kPCA9420_Sw1OutVolt0V975;
	else if(voltage == 1.000f)
		epca9420_sw1_out = kPCA9420_Sw1OutVolt1V000;
	else if(voltage == 1.025f)
		epca9420_sw1_out = kPCA9420_Sw1OutVolt1V025;
	else if(voltage == 1.050f)
		epca9420_sw1_out = kPCA9420_Sw1OutVolt1V050;
	else if(voltage == 1.075f)
		epca9420_sw1_out = kPCA9420_Sw1OutVolt1V075;
	else if(voltage == 1.100f)
		epca9420_sw1_out = kPCA9420_Sw1OutVolt1V100;
	else if(voltage == 1.125f)
		epca9420_sw1_out = kPCA9420_Sw1OutVolt1V125;
	else if(voltage == 1.150f)
		epca9420_sw1_out = kPCA9420_Sw1OutVolt1V150;
	else if(voltage == 1.175f)
		epca9420_sw1_out = kPCA9420_Sw1OutVolt1V175;
	else if(voltage == 1.200f)
		epca9420_sw1_out = kPCA9420_Sw1OutVolt1V200;
	else if(voltage == 1.225f)
		epca9420_sw1_out = kPCA9420_Sw1OutVolt1V225;
	else if(voltage == 1.250f)
		epca9420_sw1_out = kPCA9420_Sw1OutVolt1V250;
	else if(voltage == 1.275f)
		epca9420_sw1_out = kPCA9420_Sw1OutVolt1V275;
	else if(voltage == 1.300f)
		epca9420_sw1_out = kPCA9420_Sw1OutVolt1V300;
	else if(voltage == 1.325f)
		epca9420_sw1_out = kPCA9420_Sw1OutVolt1V325;
	else if(voltage == 1.350f)
		epca9420_sw1_out = kPCA9420_Sw1OutVolt1V350;
	else if(voltage == 1.375f)
		epca9420_sw1_out = kPCA9420_Sw1OutVolt1V375;
	else if(voltage == 1.400f)
		epca9420_sw1_out = kPCA9420_Sw1OutVolt1V400;
	else if(voltage == 1.425f)
		epca9420_sw1_out = kPCA9420_Sw1OutVolt1V425;
	else if(voltage == 1.450f)
		epca9420_sw1_out = kPCA9420_Sw1OutVolt1V450;
	else if(voltage == 1.475f)
		epca9420_sw1_out = kPCA9420_Sw1OutVolt1V475;
	else if(voltage == 1.500f)
		epca9420_sw1_out = kPCA9420_Sw1OutVolt1V500;
	else if(voltage == 1.8f)
		epca9420_sw1_out = kPCA9420_Sw1OutVolt1V800;

	PCA9420_Set_sw1_out_vol(&pca9420Driver, epca9420_mode, epca9420_sw1_out);
}

static void set_sw2_vol(enum _pca9420_mode epca9420_mode)
{
	float voltage;
	uint32_t data;

	do
	{
		PRINTF("\r\nEnter SW2 output voltage from (1.5 V to 2.1 V or 2.7 V to 3.3 V) with 25 mV/step Adjustable resolution ---------\r\n");
		SCANF("%f",&voltage);
		PRINTF("%f\r\n",voltage);
		GETCHAR();

		data = voltage * 1000;

		if((data % 25 == 0) && ((voltage <= 2.1f && voltage >= 1.5f) || (voltage <= 3.3f && voltage >= 2.7f)))
			break;
		else
			PRINTF("\r\nInvalid Value, Please enter correct value\r\n");
	}while(!((data % 25 == 0) && ((voltage <= 2.1f && voltage >= 1.5f) || (voltage <= 3.3f && voltage >= 2.7f))));

	if(voltage == 1.500f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt1V500;
	else if(voltage == 1.525f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt1V525;
	else if(voltage == 1.550f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt1V550;
	else if(voltage == 1.575f)
		epca9420_sw2_out = 	kPCA9420_Sw2OutVolt1V575;
	else if(voltage == 1.600f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt1V600;
	else if(voltage == 1.625f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt1V625;
	else if(voltage == 1.650f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt1V650;
	else if(voltage == 1.675f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt1V675;
	else if(voltage == 1.700f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt1V700;
	else if(voltage == 1.725f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt1V725;
	else if(voltage == 1.750f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt1V750;
	else if(voltage == 1.775f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt1V775;
	else if(voltage == 1.800f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt1V800;
	else if(voltage == 1.825f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt1V825;
	else if(voltage == 1.850f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt1V850;
	else if(voltage == 1.875f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt1V875;
	else if(voltage == 1.900f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt1V900;
	else if(voltage == 1.925f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt1V925;
	else if(voltage == 1.950f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt1V950;
	else if(voltage == 1.975f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt1V975;
	else if(voltage == 2.000f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt2V000;
	else if(voltage == 2.025f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt2V025;
	else if(voltage == 2.050f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt2V050;
	else if(voltage == 2.075f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt2V075;
	else if(voltage == 2.100f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt2V100;
	else if(voltage == 2.700f) // Extended range input for SW2
		epca9420_sw2_out = kPCA9420_Sw2OutVolt2V700;
	else if(voltage == 2.725f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt2V725;
	else if(voltage == 2.750f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt2V750;
	else if(voltage == 2.775f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt2V775;
	else if(voltage == 2.800f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt2V800;
	else if(voltage == 2.825f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt2V825;
	else if(voltage == 2.850f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt2V850;
	else if(voltage == 2.875f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt2V875;
	else if(voltage == 2.900f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt2V900;
	else if(voltage == 2.925f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt2V925;
	else if(voltage == 2.950f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt2V950;
	else if(voltage == 2.975f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt2V975;
	else if(voltage == 3.000f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt3V000;
	else if(voltage == 3.025f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt3V025;
	else if(voltage == 3.050f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt3V050;
	else if(voltage == 3.075f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt3V075;
	else if(voltage == 3.100f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt3V100;
	else if(voltage == 3.125f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt3V125;
	else if(voltage == 3.150f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt3V150;
	else if(voltage == 3.175f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt3V175;
	else if(voltage == 3.200f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt3V200;
	else if(voltage == 3.225f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt3V225;
	else if(voltage == 3.250f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt3V250;
	else if(voltage == 3.275f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt3V275;
	else if(voltage == 3.300f)
		epca9420_sw2_out = kPCA9420_Sw2OutVolt3V300;

	PCA9420_Set_sw2_out_vol(&pca9420Driver, epca9420_mode, epca9420_sw2_out);
}

static void set_ldo1_vol(enum _pca9420_mode epca9420_mode)
{
	float voltage;
	uint32_t data;

	do
	{
		PRINTF("\r\nEnter LDO1 output voltage from (1.7 V to 1.9 V with 25 mV/step) ---------\r\n");
		SCANF("%f",&voltage);
		PRINTF("%f\r\n",voltage);
		GETCHAR();

		data = voltage * 1000;
		if(((data % 25 == 0) && voltage <= 1.9f && voltage >= 1.7f))
			break;
		else
			PRINTF("\r\nInvalid Value, Please enter correct value\r\n");
	}while(!(((data % 25 == 0) && voltage <= 1.9f && voltage >= 1.7f)));

	if(voltage == 1.700f)
		epca9420_ldo1_out = kPCA9420_Ldo1OutVolt1V700;
	else if(voltage == 1.725f)
		epca9420_ldo1_out = kPCA9420_Ldo1OutVolt1V725;
	else if(voltage == 1.750f)
		epca9420_ldo1_out = kPCA9420_Ldo1OutVolt1V750;
	else if(voltage == 1.775f)
		epca9420_ldo1_out = kPCA9420_Ldo1OutVolt1V775;
	else if(voltage == 1.800f)
		epca9420_ldo1_out = kPCA9420_Ldo1OutVolt1V800;
	else if(voltage == 1.825f)
		epca9420_ldo1_out = kPCA9420_Ldo1OutVolt1V825;
	else if(voltage == 1.850f)
		epca9420_ldo1_out = kPCA9420_Ldo1OutVolt1V850;
	else if(voltage == 1.875f)
		epca9420_ldo1_out = kPCA9420_Ldo1OutVolt1V875;
	else if(voltage == 1.900f)
		epca9420_ldo1_out = kPCA9420_Ldo1OutVolt1V900;

	PCA9420_Set_ldo1_out_vol(&pca9420Driver, epca9420_mode, epca9420_ldo1_out);
}

static void set_ldo2_vol(enum _pca9420_mode epca9420_mode)
{
	float voltage;
	uint32_t data;

	do
	{
		PRINTF("\r\nEnter LDO2 output voltage from (1.5 V to 2.1 V or 2.7 V to 3.3 V) with 25 mV/step Adjustable resolution ---------\r\n");
		SCANF("%f",&voltage);
		PRINTF("%f\r\n",voltage);
		GETCHAR();

		data = voltage * 1000;

		if((data % 25 == 0) && ((voltage <= 2.1f && voltage >= 1.5f) || (voltage <= 3.3f && voltage >= 2.7f)))
			break;
		else
			PRINTF("\r\nInvalid Value, Please enter correct value\r\n");
	}while(!((data % 25 == 0) && ((voltage <= 2.1f && voltage >= 1.5f) || (voltage <= 3.3f && voltage >= 2.7f))));

	if(voltage == 1.500f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt1V500;
	else if(voltage == 1.525f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt1V500;
	else if(voltage == 1.550f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt1V550;
	else if(voltage == 1.575f)
		epca9420_ldo2_out = 	kPCA9420_Ldo2OutVolt1V550;
	else if(voltage == 1.600f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt1V600;
	else if(voltage == 1.625f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt1V625;
	else if(voltage == 1.650f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt1V650;
	else if(voltage == 1.675f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt1V675;
	else if(voltage == 1.700f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt1V700;
	else if(voltage == 1.725f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt1V725;
	else if(voltage == 1.750f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt1V750;
	else if(voltage == 1.775f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt1V775;
	else if(voltage == 1.800f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt1V800;
	else if(voltage == 1.825f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt1V825;
	else if(voltage == 1.850f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt1V850;
	else if(voltage == 1.875f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt1V875;
	else if(voltage == 1.900f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt1V900;
	else if(voltage == 1.925f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt1V925;
	else if(voltage == 1.950f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt1V950;
	else if(voltage == 1.975f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt1V975;
	else if(voltage == 2.000f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt2V000;
	else if(voltage == 2.025f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt2V025;
	else if(voltage == 2.050f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt2V050;
	else if(voltage == 2.075f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt2V075;
	else if(voltage == 2.100f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt2V100;
	else if(voltage == 2.700f) // Extended range input for LDO2
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt2V700;
	else if(voltage == 2.725f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt2V725;
	else if(voltage == 2.750f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt2V750;
	else if(voltage == 2.775f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt2V775;
	else if(voltage == 2.800f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt2V800;
	else if(voltage == 2.825f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt2V825;
	else if(voltage == 2.850f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt2V850;
	else if(voltage == 2.875f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt2V875;
	else if(voltage == 2.900f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt2V900;
	else if(voltage == 2.925f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt2V925;
	else if(voltage == 2.950f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt2V950;
	else if(voltage == 2.975f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt2V975;
	else if(voltage == 3.000f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt3V000;
	else if(voltage == 3.025f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt3V025;
	else if(voltage == 3.050f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt3V050;
	else if(voltage == 3.075f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt3V075;
	else if(voltage == 3.100f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt3V100;
	else if(voltage == 3.125f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt3V125;
	else if(voltage == 3.150f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt3V150;
	else if(voltage == 3.175f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt3V175;
	else if(voltage == 3.200f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt3V200;
	else if(voltage == 3.225f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt3V225;
	else if(voltage == 3.250f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt3V250;
	else if(voltage == 3.275f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt3V275;
	else if(voltage == 3.300f)
		epca9420_ldo2_out = kPCA9420_Ldo2OutVolt3V300;

	PCA9420_Set_ldo2_out_vol(&pca9420Driver, epca9420_mode, epca9420_ldo2_out);
}

static void regulator_settings(enum _pca9420_mode epca9420_mode)
{
	uint32_t character, data, operation, grp_setting=1;
	double voltage;

	do{
		PRINTF("\r\n********************************\r\n");
		PRINTF("\r\n1. Enable/Disable Ship Mode(Only for PCA9420UK-EVM)\r\n");
		PRINTF("\r\n2. Enable/Disable Watchdog Timer\r\n");
		PRINTF("\r\n3. On Pin Mode Configuration\r\n");
		PRINTF("\r\n4. Mode Control Selection\r\n");
		PRINTF("\r\n5. SW1 Buck Regulator\r\n");
		PRINTF("\r\n6. SW2 Buck Regulator\r\n");
		PRINTF("\r\n7. LDO1\r\n");
		PRINTF("\r\n8. LDO2\r\n");
		PRINTF("\r\n9. Exit from Group Setting\r\n");
		PRINTF("\r\n********************************\r\n");

		do
		{
			PRINTF("\r\nEnter your choice :- ");
			character = GETCHAR();
			character -= 48;
			PRINTF("%d\r\n",character);
			GETCHAR();
			if(character < 1 | character > 9)
				PRINTF("\r\nInvalid Value, Please enter correct value\r\n");
		}while(character < 1 | character > 9);

		switch (character)
		{
		case 1: //Ship mode setting
		{
#if (!PCA9421UK_EVM_EN)
			operation = find_operation();
			if(operation == 1 || operation == 2)
			{
				PCA9420_ship_mode_enable_disable(&pca9420Driver, epca9420_mode, operation);
				if(operation == 1)
				{
					PRINTF("\r\n\033[33m Ship mode enabled!!! \033[37m");
				}
				else if(operation == 2)
					PRINTF("\r\n\033[33m Ship mode Disabled!!! \033[37m");
			}
#else
			PRINTF("\r\n\033[33m PCA9421UK-EVM doesn't support ship mode feature. \033[37m\r\n");
#endif
			break;
		}
		case 2: //Watchdog timer setting
		{
			operation = find_operation();
			if(operation == 1 || operation == 2)
			{
				PRINTF("\r\n********Watchdog Timer*********\r\n");
				PRINTF("\r\n1. Disable\r\n");
				PRINTF("\r\n2. 16 sec\r\n");
				PRINTF("\r\n3. 32 sec\r\n");
				PRINTF("\r\n4. 64 sec\r\n");
				PRINTF("\r\n********************************\r\n");

				do
				{
					PRINTF("\r\nEnter your choice :- ");
					character = GETCHAR();
					character -= 48;
					PRINTF("%d\r\n",character);
					GETCHAR();
					if(character < 1 | character > 4)
						PRINTF("\r\nInvalid Value, Please enter correct value\r\n");
				}while(character < 1 | character > 4);

				switch(character)
				{
				case 1:
					PCA9420_Set_wtchdg_timer(&pca9420Driver, epca9420_mode, kPCA9420_WdTimerDisabled);
					PRINTF("\r\n\033[33m Watchdog timer Disabled!!! \033[37m");
					break;
				case 2:
					PCA9420_Set_wtchdg_timer(&pca9420Driver, epca9420_mode, kPCA9420_WdTimer16s);
					PRINTF("\r\n\033[33m Watchdog timer is set to 16 sec. \033[37m");
					break;
				case 3:
					PCA9420_Set_wtchdg_timer(&pca9420Driver, epca9420_mode, kPCA9420_WdTimer32s);
					PRINTF("\r\n\033[33m Watchdog timer is set to 32 sec. \033[37m");
					break;
				case 4:
					PCA9420_Set_wtchdg_timer(&pca9420Driver, epca9420_mode, kPCA9420_WdTimer64s);
					PRINTF("\r\n\033[33m Watchdog timer is set to 64 sec. \033[37m");
					break;
				}
			}
			break;
		}
		case 3: //On pin configuration
		{
			PRINTF("\r\nUpon valid falling edge applied on 'ON' pin---------");
			PRINTF("\r\n1. Switch back to Mode 0 setting\r\n");
			PRINTF("\r\n2. Stay in Current Mode setting\r\n");

			do
			{
				PRINTF("\r\nEnter your choice :- ");
				character = GETCHAR();
				character -= 48;
				PRINTF("%d\r\n",character);
				GETCHAR();
				if(character < 1 | character > 2)
					PRINTF("\r\nInvalid Value, Please enter correct value\r\n");
			}while(character < 1 | character > 2);

			if(character == 1)
				PCA9420_on_pin_mode_set(&pca9420Driver, epca9420_mode, 0);
			else
				PCA9420_on_pin_mode_set(&pca9420Driver, epca9420_mode, 1);
			PRINTF("\r\n\033[33m ON pin configuration is applied. \033[37m");
			break;
		}
		case 4: //Mode Control Selection
		{
			PRINTF("\r\n1. Mode Control by internal I2C register\r\n");
			PRINTF("\r\n2. Mode Control by signal applied on external MODESEL0/MODESEL1 pins\r\n");

			do
			{
				PRINTF("\r\nEnter your choice :- ");
				character = GETCHAR();
				character -= 48;
				PRINTF("%d\r\n",character);
				GETCHAR();
				if(character < 1 | character > 2)
					PRINTF("\r\nInvalid Value, Please enter correct value\r\n");
			}while(character < 1 | character > 2);

			if(character == 1)
			{
				PCA9420_mode_control(&pca9420Driver, epca9420_mode, 0);
				PRINTF("\r\n\033[33m Now mode will be control by I2C register. \033[37m");
			}
			else
			{
				PCA9420_mode_control(&pca9420Driver, epca9420_mode, 1);
				PRINTF("\r\n\033[33m Now mode will be control by signal applied on external MODESEL0/MODESEL1 pins. \033[37m");
			}
			break;
		}
		case 5: //SW1
		{
			operation = find_operation();
			if(operation == 1)
			{
				PCA9420_vol_reg_enable_disable(&pca9420Driver, epca9420_mode, kPCA9420_SW1, 1);
				set_sw1_vol(epca9420_mode);
			}
			else if(operation == 2)
				PCA9420_vol_reg_enable_disable(&pca9420Driver, epca9420_mode, kPCA9420_SW1, 0);

			pmic_status();
			break;
		}
		case 6: //SW2
		{
			operation = find_operation();
			if(operation == 1)
			{
				PCA9420_vol_reg_enable_disable(&pca9420Driver, epca9420_mode, kPCA9420_SW2, 1);
				set_sw2_vol(epca9420_mode);
			}
			else if(operation == 2)
				PCA9420_vol_reg_enable_disable(&pca9420Driver, epca9420_mode, kPCA9420_SW2, 0);

			pmic_status();
			break;
		}
		case 7: //LDO1
		{
			operation = find_operation();
			if(operation == 1)
			{
				PCA9420_vol_reg_enable_disable(&pca9420Driver, epca9420_mode, kPCA9420_LDO1, 1);
				set_ldo1_vol(epca9420_mode);
			}
			else if(operation == 2)
				PCA9420_vol_reg_enable_disable(&pca9420Driver, epca9420_mode, kPCA9420_LDO1, 0);

			pmic_status();
			break;
		}
		case 8: //LDO2
			operation = find_operation();
			if(operation == 1)
			{
				PCA9420_vol_reg_enable_disable(&pca9420Driver, epca9420_mode, kPCA9420_LDO2, 1);
				set_ldo2_vol(epca9420_mode);
			}
			else if(operation == 2)
				PCA9420_vol_reg_enable_disable(&pca9420Driver, epca9420_mode, kPCA9420_LDO2, 0);

			pmic_status();
			break;
		case 9: //exit
			grp_setting = 0;
			break;
		default:
			break;
		}
	}while(grp_setting);
}

static void regulator_group_settings()
{
	uint32_t character, data, ereg_setting=1;

	do{
		PRINTF("\r\n\033[33m Select any one MODE to configure voltage regulator settings \033[37m\r\n");
		PRINTF("\r\n 1. MODE 0\r\n");
		PRINTF("\r\n 2. MODE 1\r\n");
		PRINTF("\r\n 3. MODE 2\r\n");
		PRINTF("\r\n 4. MODE 3\r\n");
		PRINTF("\r\n 5. Exit from Voltage Regulator Settings");
		PRINTF("\r\n********************************\r\n");

		do
		{
			PRINTF("\r\nEnter your choice :- ");
			character = GETCHAR();
			character -= 48;
			PRINTF("%d\r\n",character);
			GETCHAR();
			if(character < 1 | character > 5)
				PRINTF("\r\nInvalid Value, Please enter correct value\r\n");
		}while(character < 1 | character > 5);

		switch (character)
		{
		case 1:
			PRINTF("\r\n\033[33m Mode 0 is selected. \033[37m\r\n");
			regulator_settings(kPCA9420_Mode0);
			break;
		case 2:
			PRINTF("\r\n\033[33m Mode 1 is selected. \033[37m\r\n");
			regulator_settings(kPCA9420_Mode1);
			break;
		case 3:
			PRINTF("\r\n\033[33m Mode 2 is selected. \033[37m\r\n");
			regulator_settings(kPCA9420_Mode2);
			break;
		case 4:
			PRINTF("\r\n\033[33m Mode 3 is selected. \033[37m\r\n");
			regulator_settings(kPCA9420_Mode3);
			break;
		case 5:
			ereg_setting = 0;
			break;
		default:
			break;
		}
	}while(ereg_setting);

}

static void enable_disable_interrupt()
{
	uint16_t character, data, ereg_setting=1, operation, shift, mask;
	uint8_t e_sysint = 1, e_batint = 1, e_regint = 1, e_vinint = 1;

	do{
		PRINTF("\r\n 1. System level Interrupt\r\n");

#if (!PCA9421UK_EVM_EN)
		PRINTF("\r\n 2. Linear battery charger block Interrupt\r\n");
#else
		PRINTF("\r\n 2. VIN Interrupt\r\n");
#endif

		PRINTF("\r\n 3. Voltage regulator block Interrupt\r\n");
		PRINTF("\r\n 4. Exit\r\n");

		do
		{
			PRINTF("\r\nEnter your choice :- ");
			character = GETCHAR();
			character -= 48;
			PRINTF("%d\r\n",character);
			GETCHAR();
			if(character < 1 | character > 4)
				PRINTF("\r\nInvalid Value, Please enter correct value\r\n");
		}while(character < 1 | character > 4);
		PRINTF("\r\n*****************************\r\n");

		switch (character)
		{
		case 1: // System level interrupts
			while(e_sysint)
			{
				PCA9420_DRV_Read(&pca9420Driver, PCA9420UK_SUB_INT0_MASK, &data);

				PRINTF("\r\n************\033[35m SYSTEM LEVEL INTERRUPT STATE \033[37m************\r\n");
				if((data & PCA9420_TEMP_PREWARN_MASK) >> PCA9420_TEMP_PREWARN_SHIFT)
					PRINTF("\r\n\033[93m Die temperature pre-warning Interrupt: \033[37m Disabled. \r\n");
				else
					PRINTF("\r\n\033[93m Die temperature pre-warning Interrupt: \033[37m Enabled. \r\n");

				if((data & PCA9420_THEM_SHDN_MASK) >> PCA9420_THEM_SHDN_SHIFT)
					PRINTF("\r\n\033[93m Thermal shutdown Interrupt:            \033[37m Disabled. \r\n");
				else
					PRINTF("\r\n\033[93m Thermal shutdown Interrupt:            \033[37m Enabled. \r\n");

				if((data & PCA9420_ASYS_PREWARN_MASK) >> PCA9420_ASYS_PREWARN_SHIFT)
					PRINTF("\r\n\033[93m ASYS pre-warning  Interrupt:           \033[37m Disabled. \r\n");
				else
					PRINTF("\r\n\033[93m ASYS pre-warning  Interrupt:           \033[37m Enabled. \r\n");

				if((data & PCA9420_WD_TMR_MASK) >> PCA9420_WD_TMR_SHIFT)
					PRINTF("\r\n\033[93m Watchdog timer expiration Interrupt:   \033[37m Disabled. \r\n");
				else
					PRINTF("\r\n\033[93m Watchdog timer expiration Interrupt:   \033[37m Enabled. \r\n");

				if((data & PCA9420_IN_PWR_MASK) >> PCA9420_IN_PWR_SHIFT)
					PRINTF("\r\n\033[93m Input voltage Interrupt:               \033[37m Disabled. \r\n");
				else
					PRINTF("\r\n\033[93m Input voltage Interrupt:               \033[37m Enabled. \r\n");

				PRINTF("\r\n\r\n\033[93m *****Select any one to enable/disable particular Interrupt***** \033[37m\r\n");
				PRINTF("\r\n1. Die temperature pre-warning interrupt\r\n");
				PRINTF("\r\n2. Thermal shutdown interrupt \r\n");
				PRINTF("\r\n3. ASYS pre-warning interrupt\r\n");
				PRINTF("\r\n4. Watchdog timer expiration interrupt \r\n");
				PRINTF("\r\n5. Input voltage interrupt\r\n");
				PRINTF("\r\n6. Exit\r\n");

				do
				{
					PRINTF("\r\nEnter your choice :- ");
					character = GETCHAR();
					character -= 48;
					PRINTF("%d\r\n",character);
					GETCHAR();
					if(character < 1 | character > 6)
						PRINTF("\r\nInvalid Value, Please enter correct value\r\n");
				}while(character < 1 | character > 6);
				PRINTF("\r\n*****************************\r\n");

				switch(character)
				{
				case 1:
					shift = PCA9420_TEMP_PREWARN_SHIFT;
					break;
				case 2:
					shift = PCA9420_THEM_SHDN_SHIFT;
					break;
				case 3:
					shift = PCA9420_ASYS_PREWARN_SHIFT;
					break;
				case 4:
					shift = PCA9420_WD_TMR_SHIFT;
					break;
				case 5:
					shift = PCA9420_IN_PWR_SHIFT;
					break;
				case 6:
					e_sysint = 0;
					break;
				default:
					break;
				}

				if(e_sysint)
				{
					operation = find_operation();
					if(operation == 1)
					{
						data = data & ~(1 << shift);
						PCA9420_enable_disable_System_int(&pca9420Driver, data);
						PRINTF("\r\nInterrupt Enabled!!!\r\n");
					}
					else if(operation == 2)
					{
						data = data | (1 << shift);
						PCA9420_enable_disable_System_int(&pca9420Driver, data);
						PRINTF("\r\nInterrupt Disabled!!!\r\n");
					}
				}
			}
			break;

		case 2: // Linear battery charger block interrupt
#if (!PCA9421UK_EVM_EN)
			while(e_batint)
			{
				PCA9420_DRV_Read(&pca9420Driver, PCA9420UK_SUB_INT1_MASK, &data);

				PRINTF("\r\n************\033[35m LINEAR BATTERY CHARGER BLOCK INTERRUPT STATE \033[37m************\r\n");
				if((data & PCA9420_DEAD_TMR_MASK) >> PCA9420_DEAD_TMR_SHIFT)
					PRINTF("\r\n\033[93m Dead charge timer Interrupt:                   \033[37m Disabled. \r\n");
				else
					PRINTF("\r\n\033[93m Dead charge timer Interrupt:                   \033[37m Enabled. \r\n");

				if((data & PCA9420_VIN_ILIM_MASK) >> PCA9420_VIN_ILIM_SHIFT)
					PRINTF("\r\n\033[93m Input current limit Interrupt:                 \033[37m Disabled. \r\n");
				else
					PRINTF("\r\n\033[93m Input current limit Interrupt:                 \033[37m Enabled. \r\n");

				if((data & PCA9420_FAST_TMR_MASK) >> PCA9420_FAST_TMR_SHIFT)
					PRINTF("\r\n\033[93m Fast charging timer expiration Interrupt:      \033[37m Disabled. \r\n");
				else
					PRINTF("\r\n\033[93m Fast charging timer expiration Interrupt:      \033[37m Enabled. \r\n");

				if((data & PCA9420_PREQ_TMR_MASK) >> PCA9420_PREQ_TMR_SHIFT)
					PRINTF("\r\n\033[93m Pre-qual charging timer expiration Interrupt:  \033[37m Disabled. \r\n");
				else
					PRINTF("\r\n\033[93m Pre-qual charging timer expiration Interrupt:  \033[37m Enabled. \r\n");

				if((data & PCA9420_VBAT_DET_MASK) >> PCA9420_VBAT_DET_SHIFT)
					PRINTF("\r\n\033[93m Battery presence Interrupt:                    \033[37m Disabled. \r\n");
				else
					PRINTF("\r\n\033[93m Battery presence Interrupt:                    \033[37m Enabled. \r\n");

				if((data & PCA9420_VBAT_OK_MASK) >> PCA9420_VBAT_OK_SHIFT)
					PRINTF("\r\n\033[93m Battery Interrupt:                             \033[37m Disabled. \r\n");
				else
					PRINTF("\r\n\033[93m Battery Interrupt:                             \033[37m Enabled. \r\n");

				if((data & PCA9420_CHG_OK_MASK) >> PCA9420_CHG_OK_SHIFT)
					PRINTF("\r\n\033[93m Charger Interrupt:                             \033[37m Disabled. \r\n");
				else
					PRINTF("\r\n\033[93m Charger Interrupt:                             \033[37m Enabled. \r\n");

				PRINTF("\r\n\r\n\033[93m *****Select any one to enable/disable particular Interrupt***** \033[37m\r\n");
				PRINTF("\r\n1. Dead charge timer interrupt\r\n");
				PRINTF("\r\n2. Input current limit interrupt \r\n");
				PRINTF("\r\n3. Fast charging timer expiration interrupt\r\n");
				PRINTF("\r\n4. Pre-qual charging timer expiration interrupt \r\n");
				PRINTF("\r\n5. Battery presence interrupt\r\n");
				PRINTF("\r\n6. Battery interrupt \r\n");
				PRINTF("\r\n7. Charger interrupt\r\n");
				PRINTF("\r\n8. Exit\r\n");

				do
				{
					PRINTF("\r\nEnter your choice :- ");
					character = GETCHAR();
					character -= 48;
					PRINTF("%d\r\n",character);
					GETCHAR();
					if(character < 1 | character > 8)
						PRINTF("\r\nInvalid Value, Please enter correct value\r\n");
				}while(character < 1 | character > 8);
				PRINTF("\r\n*****************************\r\n");

				switch(character)
				{
				case 1:
					shift = PCA9420_DEAD_TMR_SHIFT;
					break;
				case 2:
					shift = PCA9420_VIN_ILIM_SHIFT;
					break;
				case 3:
					shift = PCA9420_FAST_TMR_SHIFT;
					break;
				case 4:
					shift = PCA9420_PREQ_TMR_SHIFT;
					break;
				case 5:
					shift = PCA9420_VBAT_DET_SHIFT;
					break;
				case 6:
					shift = PCA9420_VBAT_OK_SHIFT;
					break;
				case 7:
					shift = PCA9420_CHG_OK_SHIFT;
					break;
				case 8:
					e_batint = 0;
					break;
				default:
					break;
				}

				if(e_batint)
				{
					operation = find_operation();
					if(operation == 1)
					{
						data = data & ~(1 << shift);
						PCA9420_enable_disable_bat_int(&pca9420Driver, data);
						PRINTF("\r\nInterrupt Enabled!!!\r\n");
					}
					else if(operation == 2)
					{
						data = data | (1 << shift);
						PCA9420_enable_disable_bat_int(&pca9420Driver, data);
						PRINTF("\r\nInterrupt Disabled!!!\r\n");
					}
				}
			}
#else  //PCA9421UK-EVM
			while(e_vinint)
			{
				PCA9420_DRV_Read(&pca9420Driver, PCA9420UK_SUB_INT1_MASK, &data);
				PRINTF("\r\n3006: data = %x\r\n",data);

				PRINTF("\r\n************\033[35m VIN INTERRUPT STATE \033[37m************\r\n");

				if((data & PCA9420_VIN_ILIM_MASK) >> PCA9420_VIN_ILIM_SHIFT)
					PRINTF("\r\n\033[93m Input current limit Interrupt:                 \033[37m Disabled. \r\n");
				else
					PRINTF("\r\n\033[93m Input current limit Interrupt:                 \033[37m Enabled. \r\n");

				PRINTF("\r\n\r\n\033[93m *****Select any one to enable/disable particular Interrupt***** \033[37m\r\n");
				PRINTF("\r\n1. Input current limit interrupt \r\n");
				PRINTF("\r\n2. Exit\r\n");

				do
				{
					PRINTF("\r\nEnter your choice :- ");
					character = GETCHAR();
					character -= 48;
					PRINTF("%d\r\n",character);
					GETCHAR();
					if(character < 1 | character > 2)
						PRINTF("\r\nInvalid Value, Please enter correct value\r\n");
				}while(character < 1 | character > 2);
				PRINTF("\r\n*****************************\r\n");

				switch(character)
				{
				case 1:
					shift = PCA9420_VIN_ILIM_SHIFT;
					break;
				case 2:
					e_vinint = 0;
					break;
				default:
					break;
				}

				if(e_vinint)
				{
					operation = find_operation();
					if(operation == 1)
					{
						data = data & ~(1 << shift);
						PRINTF("\r\ndata = %x\r\n",data);
						PCA9420_enable_disable_bat_int(&pca9420Driver, data);
						PRINTF("\r\nInterrupt Enabled!!!\r\n");
					}
					else if(operation == 2)
					{
						data = data | (1 << shift);
						PCA9420_enable_disable_bat_int(&pca9420Driver, data);
						PRINTF("\r\nInterrupt Disabled!!!\r\n");
					}
				}
			}
#endif
			break;

		case 3: // Voltage regulator block interrupt
			while(e_regint)
			{
				PCA9420_DRV_Read(&pca9420Driver, PCA9420UK_SUB_INT2_MASK, &data);

				PRINTF("\r\n************\033[35m VOLTAGE REGULATOR BLOCK INTERRUPT STATE \033[37m************\r\n");
				if((data & PCA9420_VOUTSW1_MASK) >> PCA9420_VOUTSW1_SHIFT)
					PRINTF("\r\n\033[93m SW1 output voltage Interrupt:   \033[37m Disabled. \r\n");
				else
					PRINTF("\r\n\033[93m SW1 output voltage Interrupt:   \033[37m Enabled. \r\n");

				if((data & PCA9420_VOUTSW2_MASK) >> PCA9420_VOUTSW2_SHIFT)
					PRINTF("\r\n\033[93m SW2 output voltage Interrupt:   \033[37m Disabled. \r\n");
				else
					PRINTF("\r\n\033[93m SW2 output voltage Interrupt:   \033[37m Enabled. \r\n");

				if((data & PCA9420_VOUTLDO1_MASK) >> PCA9420_VOUTLDO1_SHIFT)
					PRINTF("\r\n\033[93m LDO1 output voltage Interrupt:  \033[37m Disabled. \r\n");
				else
					PRINTF("\r\n\033[93m LDO1 output voltage Interrupt:  \033[37m Enabled. \r\n");

				if((data & PCA9420_VOUTLDO2_MASK) >> PCA9420_VOUTLDO2_SHIFT)
					PRINTF("\r\n\033[93m LDO2 output voltage Interrupt:  \033[37m Disabled. \r\n");
				else
					PRINTF("\r\n\033[93m LDO2 output voltage Interrupt:  \033[37m Enabled. \r\n");


				PRINTF("\r\n\r\n\033[93m *****Select any one to enable/disable particular Interrupt***** \033[37m\r\n");
				PRINTF("\r\n1. SW1 output voltage interrupt\r\n");
				PRINTF("\r\n2. SW2 output voltage interrupt \r\n");
				PRINTF("\r\n3. LDO1 output voltage interrupt\r\n");
				PRINTF("\r\n4. LDO2 output voltage interrupt \r\n");
				PRINTF("\r\n5. Exit\r\n");

				do
				{
					PRINTF("\r\nEnter your choice :- ");
					character = GETCHAR();
					character -= 48;
					PRINTF("%d\r\n",character);
					GETCHAR();
					if(character < 1 | character > 5)
						PRINTF("\r\nInvalid Value, Please enter correct value\r\n");
				}while(character < 1 | character > 5);
				PRINTF("\r\n*****************************\r\n");

				switch(character)
				{
				case 1:
					shift = PCA9420_VOUTSW1_SHIFT;
					break;
				case 2:
					shift = PCA9420_VOUTSW2_SHIFT;
					break;
				case 3:
					shift = PCA9420_VOUTLDO1_SHIFT;
					break;
				case 4:
					shift = PCA9420_VOUTLDO2_SHIFT;
					break;
				case 5:
					e_regint = 0;
					break;
				}

				if(e_regint)
				{
					operation = find_operation();
					if(operation == 1)
					{
						data = data & ~(1 << shift);
						PCA9420_enable_disable_vol_reg_int(&pca9420Driver, data);
						PRINTF("\r\nInterrupt Enabled!!!\r\n");
					}
					else if(operation == 2)
					{
						data = data | (1 << shift);
						PCA9420_enable_disable_vol_reg_int(&pca9420Driver, data);
						PRINTF("\r\nInterrupt Disabled!!!\r\n");
					}
				}
			}
			break;

		case 4: // Exit
			ereg_setting = 0;
			break;
		default:
			break;
		}
	}while(ereg_setting);
}

#if (!PCA9421UK_EVM_EN)
static void charging_status()
{
	uint16_t character, data, offset;

	PRINTF("\r\n**************\033[35m Charge Detail Status \033[37m*****************\r\n");

	//VIN status
	PCA9420_DRV_Read(&pca9420Driver, PCA9420UK_CHG_STATUS1, &data );
	data = (data & PCA9420_IN_PWR_STATUS_MASK) >> PCA9420_IN_PWR_STATUS_SHIFT;

	PRINTF("\r\n\033[33m VIN STATUS:     \033[37m");
	if(data == 0x00)
		PRINTF("Input power is invalid. VIN<VIN_ULVO\r\n");
	else if(data == 0x01)
		PRINTF("Input power is invalid. (VIN<VIN_OVP and ASYS<VBAT+VINBAT_HEADROOM)\r\n");
	else if(data == 0x02)
		PRINTF("Input power is invalid. (VIN>VIN_OVP)\r\n");
	else if(data == 0x03)
		PRINTF("Input power is valid. (VIN>VIN_ULVO, ASYS>VBAT+VINBAT_HEADROOM and VIN<VIN_OVP)\r\n");

	//Battery detail status
	PCA9420_DRV_Read(&pca9420Driver, PCA9420UK_CHG_STATUS2, &data);
	data = (data & PCA9420_BAT_DETAIL_STATUS_MASK) >> PCA9420_BAT_DETAIL_STATUS_SHIFT;

	PRINTF("\r\n\033[33m BAT STATUS:     \033[37m");
	if(data == 0x00)
		PRINTF("Battery missing, not attached\r\n");
	else if(data == 0x01)
		PRINTF("Battery detection in-progress\r\n");
	else if(data == 0x02)
		PRINTF("Battery is in Dead battery mode.\r\n");
	else if(data == 0x03)
		PRINTF("Battery is in Pre-charge/ low battery mode.\r\n");
	else if(data == 0x04)
		PRINTF("Battery is in Fast charging mode.\r\n");
	else if(data == 0x05)
		PRINTF("Battery is in Top-off mode.\r\n");
	else if(data == 0x07)
		PRINTF("Battery Charger is in reset\r\n");

	//BAT Charger status
	PCA9420_DRV_Read(&pca9420Driver, PCA9420UK_CHG_STATUS2, &data );
	data = (data & PCA9420_BAT_CHG_STATUS_MASK) >> PCA9420_BAT_CHG_STATUS_SHIFT;

	PRINTF("\r\n\033[33m BAT CHG STATUS: \033[37m");
	if(data == 0x00)
		PRINTF("Charger in Idle State\r\n");
	else if(data == 0x01)
		PRINTF("Charger in Dead-Battery State\r\n");
	else if(data == 0x02)
		PRINTF("Charger in Low-Battery State\r\n");
	else if(data == 0x03 || data == 0x04)
		PRINTF("Charger in Fast Charging State in either Constant current or Constant voltage\r\n");
	else if(data == 0x05)
		PRINTF("Charger in Top-off State\r\n");
	else if(data == 0x06)
		PRINTF("Charger in Done State\r\n");

	//Battery Temperature stauts
	PCA9420_DRV_Read(&pca9420Driver, PCA9420UK_CHG_STATUS3, &data );
	data = (data & PCA9420_TEMP_STATUS_MASK) >> PCA9420_TEMP_STATUS_SHIFT;

	PRINTF("\r\n\033[33m TS STATUS:      \033[37m");
	if(data == 0x00)
		PRINTF("Battery Temperature Nominal\r\n");
	else if(data == 0x01)
		PRINTF("Battery Temperature is Cold\r\n");
	else if(data == 0x02)
		PRINTF("Battery Temperature is Cool\r\n");
	else if(data == 0x03)
		PRINTF("Battery Temperature is Warm\r\n");
	else if(data == 0x04)
		PRINTF("Battery Temperature is Hot\r\n");

	//Safety Timer status
	PCA9420_DRV_Read(&pca9420Driver, PCA9420UK_CHG_STATUS3, &data );
	data = (data & PCA9420_SFTY_TIMER_MASK) >> PCA9420_SFTY_TIMER_SHIFT;

	PRINTF("\r\n\033[33m SAFETY STATUS:  \033[37m");
	if(data == 0x00)
		PRINTF("Safety Timers having No Effect on Battery Charging\r\n");
	else if(data == 0x01)
		PRINTF("Pre-qual Timer expires, battery charging suspended\r\n");
	else if(data == 0x02)
		PRINTF("Fast Timer expires, battery Charging suspended\r\n");
	else if(data == 0x03)
		PRINTF("Battery short test fails, battery charging suspended\r\n");

	PRINTF("\r\n***************************************************\r\n");

}

static void battery_charging_settings()
{
	uint16_t character, data, offset;
	float sw1_vol;

	PRINTF("\r\n********************************\r\n");
	PRINTF("\r\n1. Charge Control\r\n");
	PRINTF("\r\n2. Charge Status\r\n");
	PRINTF("\r\n********************************\r\n");

	do
	{
		PRINTF("\r\nEnter your choice :- ");
		SCANF("%d",&character);
		PRINTF("%d\r\n",character);
		GETCHAR();
		if(character < 1 | character > 2)
			PRINTF("\r\nInvalid Value, Please enter correct value\r\n");
	}while(character < 1 | character > 2);

	switch (character)
	{
	case 1:
		charging_control();
		break;
	case 2:
		charging_status();
		break;
	default:
		PRINTF("\r\nInvalid option selected\r\n");
		break;
	}
}
#endif

/*! -----------------------------------------------------------------------
 *  @brief       This is the The main function implementation.
 *  @details     This function invokes board initializes routines, then then brings up the sensor and
 *               finally enters an endless loop to continuously read available samples.
 *  @param[in]   void This is no input parameter.
 *  @return      void  There is no return value.
 *  @constraints None
 *  @reeentrant  No
 *  -----------------------------------------------------------------------*/
int main(void)
{
	int32_t status;
	uint32_t input;
	char dummy;
	uint16_t Data;

#if RTE_I2C0_DMA_EN
	/*  Enable DMA clock. */
	CLOCK_EnableClock(EXAMPLE_LPI2C_DMA_CLOCK);
	edma_config_t edmaConfig = {0};
	EDMA_GetDefaultConfig(&edmaConfig);
	EDMA_Init(EXAMPLE_LPI2C_DMA_BASEADDR, &edmaConfig);
#endif

	RESET_PeripheralReset(kDMA_RST_SHIFT_RSTn);
	ARM_DRIVER_I2C *I2Cdrv = &I2C_S_DRIVER; // Now using the shield.h value!!!

	RESET_PeripheralReset(kLPI2C0_RST_SHIFT_RSTn);

	/*! Initialize the MCU hardware. */
	BOARD_InitPins();
	BOARD_InitBootClocks();
	BOARD_SystickEnable();
	BOARD_InitDebugConsole();
	init_pca9420_wakeup_int();

#if (!PCA9421UK_EVM_EN)
	PRINTF("\r\nISSDK PCA9420UK-EVM PMIC driver example demonstration.\r\n");
#else
	PRINTF("\r\nISSDK PCA9421UK-EVM PMIC driver example demonstration.\r\n");
#endif

	/*! Initialize the I2C driver. */
	status = I2Cdrv->Initialize(I2C_S_SIGNAL_EVENT);
	if (ARM_DRIVER_OK != status)
	{
		PRINTF("\r\n I2C Initialization Failed\r\n");
		return -1;
	}

	/*! Set the I2C Power mode. */
	status = I2Cdrv->PowerControl(ARM_POWER_FULL);
	if (ARM_DRIVER_OK != status)
	{
		PRINTF("\r\n I2C Power Mode setting Failed\r\n");
		return -1;
	}

	/*! Set the I2C bus speed. */
	status = I2Cdrv->Control(ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_FAST);
	if (ARM_DRIVER_OK != status)
	{
		PRINTF("\r\n I2C Control Mode setting Failed\r\n");
		return -1;
	}

	/*! Initialize  driver. */
	status = PCA9420_I2C_Initialize(&pca9420Driver, &I2C_S_DRIVER, I2C_S_DEVICE_INDEX, PCA9420UK_I2C_ADDR);
	if (SENSOR_ERROR_NONE != status)
	{
		PRINTF("\r\n Sensor Initialization Failed\r\n");
		return -1;
	}
	PCA9420_SW_reset(&pca9420Driver);

	while (1)/* Forever loop */
	{
		PRINTF("\r\n**********\033[35m MAIN MENU \033[37m**********\r\n");
		PRINTF("1. Device Information\r\n");
		PRINTF("2. PMIC Status\r\n");
		PRINTF("3. Regulator Status\r\n");
		PRINTF("4. Check Interrupt Status\r\n");
		PRINTF("5. Device Specific Settings\r\n");
		PRINTF("6. Battery Charging Settings\r\n");
		PRINTF("7. Voltage Regulator Group Settings\r\n");
		PRINTF("8. Enable/Disable Interrupt\r\n");
		PRINTF("******************************\r\n");
		SCANF("%d", &input);
		switch (input)
		{
		case 1:
			get_device_id();
			break;
		case 2:
			pmic_status();
			break;
		case 3:
			get_regulator_status();
			break;
		case 4:
			top_level_interrupt_status();
			break;
		case 5:
			device_settings();
			break;
		case 6:
#if (!PCA9421UK_EVM_EN)
			battery_charging_settings();
#else
			PRINTF("\r\n\033[33m PCA9421UK-EVM doesn't support linear battery charger feature.\033[37m\r\n");
#endif
			break;
		case 7:
			regulator_group_settings();
			break;
		case 8:
			enable_disable_interrupt();
			break;
		default:
			PRINTF("Invalid input, Continuing reading temperature\r\n");
		}
		PRINTF("\r\nPress Enter to goto Main Menu\r\n");
		do
		{
			dummy = GETCHAR();
		} while(dummy != 13);
	}

}
