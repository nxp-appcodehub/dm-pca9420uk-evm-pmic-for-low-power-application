/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file pca9420uk_drv.c
 * @brief The pca9420uk_drv.c file implements the PCA9420UK PMIC driver interfaces.
 */

//-----------------------------------------------------------------------
// ISSDK Includes
//-----------------------------------------------------------------------
#include "pca9420uk_drv.h"
#include "pca9420uk.h"

#include "systick_utils.h"
#include "stdio.h"
#include "fsl_debug_console.h"

bool repeatedStart = 1;

//APIs

int32_t PCA9420_I2C_Initialize(pca9420_i2c_sensorhandle_t *pSensorHandle, ARM_DRIVER_I2C *pBus, uint8_t index, uint16_t sAddress)
{
	int32_t status;
	uint8_t reg = 0;

	/*! Check the input parameters. */
	if ((pSensorHandle == NULL) || (pBus == NULL))
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	pSensorHandle->deviceInfo.deviceInstance = index;
	pSensorHandle->deviceInfo.functionParam = NULL;
	pSensorHandle->deviceInfo.idleFunction = NULL;

	/*! Initialize the sensor handle. */
	pSensorHandle->pCommDrv = pBus;
	pSensorHandle->slaveAddress = sAddress;
	pSensorHandle->isInitialized = true;

	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_DRV_Read(pca9420_i2c_sensorhandle_t *pSensorHandle, uint8_t RegAddress, uint16_t* Data)
{
	int32_t status;
	uint8_t reg;

	status = Register_I2C_Read(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, RegAddress, PCA9420UK_REG_SIZE_BYTES, Data);

	if (ARM_DRIVER_OK != status)
	{
		pSensorHandle->isInitialized = false;
		return SENSOR_ERROR_INIT;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_DRV_Write(pca9420_i2c_sensorhandle_t *pSensorHandle, uint8_t RegAddress, uint16_t Data)
{
	int32_t status;

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, RegAddress,
			Data, 0, repeatedStart);
	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_wtchdg_timer_reset(pca9420_i2c_sensorhandle_t *pSensorHandle)
{
	int32_t status;

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_TOP_CNTL4,
			PCA9420_WTCHDG_TIMER_RESET, 0, repeatedStart);
	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_Get_long_glitch_timer(pca9420_i2c_sensorhandle_t *pSensorHandle, enum LongGlitchTimer *pBuffer)
{
	uint8_t reg;
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Read(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_TOP_CNTL3, PCA9420UK_REG_SIZE_BYTES, &reg);

	if (ARM_DRIVER_OK != status)
	{
		pSensorHandle->isInitialized = false;
		return SENSOR_ERROR_INIT;
	}
	reg = (reg & PCA9420_TOP_CNTL3_ON_GLT_LONG_MASK) >> PCA9420_TOP_CNTL3_ON_GLT_LONG_SHIFT;

	switch(reg)
	{
	case PCA9420_GLTH_Timer_4:
		*pBuffer = PCA9420_GLTH_Timer_4;
		break;
	case PCA9420_GLTH_Timer_8:
		*pBuffer = PCA9420_GLTH_Timer_8;
		break;
	case PCA9420_GLTH_Timer_12:
		*pBuffer = PCA9420_GLTH_Timer_12;
		break;
	case PCA9420_GLTH_Timer_16:
		*pBuffer = PCA9420_GLTH_Timer_16;
		break;
	default:
		*pBuffer = PCA9420_GLTH_Timer_8;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_Set_long_glitch_timer(pca9420_i2c_sensorhandle_t *pSensorHandle, enum LongGlitchTimer eLngGthTmr)
{
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_TOP_CNTL3,
			(uint8_t)(eLngGthTmr << PCA9420_TOP_CNTL3_ON_GLT_LONG_SHIFT), PCA9420_TOP_CNTL3_ON_GLT_LONG_MASK, repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_SW_reset(pca9420_i2c_sensorhandle_t *pSensorHandle)
{
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_TOP_CNTL3,
			PCA9420_TOP_CNTL3_SW_RESET_MASK, 0, repeatedStart);
	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_Set_mode_control(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_mode epca9420_mode)
{
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_TOP_CNTL3,
			(uint8_t)(epca9420_mode << PCA9420_MODE_CNTL_SEL_SHIFT), PCA9420_MODE_CNTL_SEL_MASK, repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_Get_mode_control(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_mode *pBuffer)
{
	uint8_t reg;
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Read(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_TOP_CNTL3, PCA9420UK_REG_SIZE_BYTES, &reg);

	if (ARM_DRIVER_OK != status)
	{
		pSensorHandle->isInitialized = false;
		return SENSOR_ERROR_INIT;
	}
	reg = (reg & PCA9420_MODE_CNTL_SEL_MASK) >> PCA9420_MODE_CNTL_SEL_SHIFT;

	switch(reg)
	{
	case kPCA9420_Mode0:
		*pBuffer = kPCA9420_Mode0;
		break;
	case kPCA9420_Mode1:
		*pBuffer = kPCA9420_Mode1;
		break;
	case kPCA9420_Mode2:
		*pBuffer = kPCA9420_Mode2;
		break;
	case kPCA9420_Mode3:
		*pBuffer = kPCA9420_Mode3;
		break;
	default:
		*pBuffer = kPCA9420_Mode0;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_Set_die_temp_tshld(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_die_temp_warning epca9420_die_temp)
{
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_TOP_CNTL2,
			(uint8_t)(epca9420_die_temp << PCA9420_MODE_DIE_TEMP_SHIFT), PCA9420_MODE_DIE_TEMP_MASK, repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_Get_die_temp_tshld(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_die_temp_warning *pBuffer)
{
	uint8_t reg;
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Read(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_TOP_CNTL2, PCA9420UK_REG_SIZE_BYTES, &reg);

	if (ARM_DRIVER_OK != status)
	{
		pSensorHandle->isInitialized = false;
		return SENSOR_ERROR_INIT;
	}
	reg = (reg & PCA9420_MODE_DIE_TEMP_MASK) >> PCA9420_MODE_DIE_TEMP_SHIFT;

	switch(reg)
	{
	case kPCA9420_DieTempWarn75C:
		*pBuffer = kPCA9420_DieTempWarn75C;
		break;
	case kPCA9420_DieTempWarn80C:
		*pBuffer = kPCA9420_DieTempWarn80C;
		break;
	case kPCA9420_DieTempWarn85C:
		*pBuffer = kPCA9420_DieTempWarn85C;
		break;
	case kPCA9420_DieTempWarn90C:
		*pBuffer = kPCA9420_DieTempWarn90C;
		break;
	default:
		*pBuffer = kPCA9420_DieTempWarn85C;
	}
	return SENSOR_ERROR_NONE;
}


int32_t PCA9420_Set_thrml_shtdn_tshld(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_them_shdn epca9420_them_shdn)
{
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_TOP_CNTL2,
			(uint8_t)(epca9420_them_shdn << PCA9420_MODE_THML_STDN_SHIFT), PCA9420_MODE_THML_STDN_MASK, repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_Get_thrml_shtdn_tshld(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_them_shdn *pBuffer)
{
	uint8_t reg;
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Read(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_TOP_CNTL2, PCA9420UK_REG_SIZE_BYTES, &reg);

	if (ARM_DRIVER_OK != status)
	{
		pSensorHandle->isInitialized = false;
		return SENSOR_ERROR_INIT;
	}
	reg = (reg & PCA9420_MODE_THML_STDN_MASK) >> PCA9420_MODE_THML_STDN_SHIFT;

	switch(reg)
	{
	case kPCA9420_ThemShdn95C:
		*pBuffer = kPCA9420_ThemShdn95C;
		break;
	case kPCA9420_ThemShdn100C:
		*pBuffer = kPCA9420_ThemShdn100C;
		break;
	case kPCA9420_ThemShdn105C:
		*pBuffer = kPCA9420_ThemShdn105C;
		break;
	case kPCA9420_ThemShdn110C:
		*pBuffer = kPCA9420_ThemShdn110C;
		break;
	case kPCA9420_ThemShdn115C:
		*pBuffer = kPCA9420_ThemShdn115C;
		break;
	case kPCA9420_ThemShdn120C:
		*pBuffer = kPCA9420_ThemShdn120C;
		break;
	case kPCA9420_ThemShdn125C:
		*pBuffer = kPCA9420_ThemShdn125C;
		break;
	default:
		*pBuffer = kPCA9420_ThemShdn110C;
	}
	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_Set_asys_uvlo_tshld(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_asys_uvlo_sel epca9420_asys_uvlo)
{
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_TOP_CNTL2,
			(uint8_t)(epca9420_asys_uvlo << PCA9420_MODE_ASYS_UVLO_SHIFT), PCA9420_MODE_ASYS_UVLO_MASK, repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_Get_asys_uvlo_tshld(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_asys_uvlo_sel *pBuffer)
{
	uint8_t reg;
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Read(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_TOP_CNTL2, PCA9420UK_REG_SIZE_BYTES, &reg);

	if (ARM_DRIVER_OK != status)
	{
		pSensorHandle->isInitialized = false;
		return SENSOR_ERROR_INIT;
	}
	reg = (reg & PCA9420_MODE_ASYS_UVLO_MASK) >> PCA9420_MODE_ASYS_UVLO_SHIFT;

	switch(reg)
	{
	case kPCA9420_AsysUvloSel2V4:
		*pBuffer = kPCA9420_AsysUvloSel2V4;
		break;
	case kPCA9420_AsysUvloSel2V5:
		*pBuffer = kPCA9420_AsysUvloSel2V5;
		break;
	case kPCA9420_AsysUvloSel2V6:
		*pBuffer = kPCA9420_AsysUvloSel2V6;
		break;
	case kPCA9420_AsysUvloSel2V7:
		*pBuffer = kPCA9420_AsysUvloSel2V7;
		break;
	default:
		*pBuffer = kPCA9420_AsysUvloSel2V7;
	}
	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_Set_vin_uvlo_tshld(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_vin_uvlo_sel epca9420_vin_uvlo)
{
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_TOP_CNTL1,
			(uint8_t)(epca9420_vin_uvlo << PCA9420_MODE_VIN_UVLO_SHIFT), PCA9420_MODE_VIN_UVLO_MASK, repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_Get_vin_uvlo_tshld(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_vin_uvlo_sel *pBuffer)
{
	uint8_t reg;
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Read(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_TOP_CNTL1, PCA9420UK_REG_SIZE_BYTES, &reg);

	if (ARM_DRIVER_OK != status)
	{
		pSensorHandle->isInitialized = false;
		return SENSOR_ERROR_INIT;
	}
	reg = (reg & PCA9420_MODE_VIN_UVLO_MASK) >> PCA9420_MODE_VIN_UVLO_SHIFT;

	switch(reg)
	{
	case kPCA9420_VinUvloSel2V9:
		*pBuffer = kPCA9420_VinUvloSel2V9;
		break;
	case kPCA9420_VinUvloSel3V1:
		*pBuffer = kPCA9420_VinUvloSel3V1;
		break;
	case kPCA9420_VinUvloSel3V3:
		*pBuffer = kPCA9420_VinUvloSel3V3;
		break;
	case kPCA9420_VinUvloSel3V5:
		*pBuffer = kPCA9420_VinUvloSel3V5;
		break;
	default:
		*pBuffer = kPCA9420_VinUvloSel3V1;
	}
	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_Set_vin_overvoltage_tshld(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_vin_ovp_sel epca9420_vin_ovp)
{
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_TOP_CNTL1,
			(uint8_t)(epca9420_vin_ovp << PCA9420_MODE_VIN_OVP_SHIFT), PCA9420_MODE_VIN_OVP_MASK, repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_Get_vin_overvoltage_tshld(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_vin_ovp_sel *pBuffer)
{
	uint8_t reg;
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Read(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_TOP_CNTL1, PCA9420UK_REG_SIZE_BYTES, &reg);

	if (ARM_DRIVER_OK != status)
	{
		pSensorHandle->isInitialized = false;
		return SENSOR_ERROR_INIT;
	}
	reg = (reg & PCA9420_MODE_VIN_OVP_MASK) >> PCA9420_MODE_VIN_OVP_SHIFT;

	switch(reg)
	{
	case kPCA9420_VinOvpSel5V5:
		*pBuffer = kPCA9420_VinOvpSel5V5;
		break;
	case kPCA9420_VinOvpSel6V0:
		*pBuffer = kPCA9420_VinOvpSel6V0;
		break;
	default:
		*pBuffer = kPCA9420_VinOvpSel5V5;
	}
	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_asys_input_src_sel(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_asys_input_sel epca9420_asys_input_sel)
{
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_TOP_CNTL1,
			(uint8_t)(epca9420_asys_input_sel << PCA9420_MODE_ASYS_INP_SEL_SHIFT), PCA9420_MODE_ASYS_INP_SEL_MASK, repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

//ASYS PRE-WARNING VOLTAGE THRESHOLD
int32_t PCA9420_Set_asys_prewarn_vol_tshld(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_asys_prewarning epca9420_asys_prewarning)
{
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_TOP_CNTL1,
			(uint8_t)(epca9420_asys_prewarning << PCA9420_MODE_ASYS_PRE_VOL_SHIFT), PCA9420_MODE_ASYS_PRE_VOL_MASK, repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_Get_asys_prewarn_vol_tshld(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_asys_prewarning *pBuffer)
{
	uint8_t reg;
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Read(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_TOP_CNTL1, PCA9420UK_REG_SIZE_BYTES, &reg);

	if (ARM_DRIVER_OK != status)
	{
		pSensorHandle->isInitialized = false;
		return SENSOR_ERROR_INIT;
	}
	reg = (reg & PCA9420_MODE_ASYS_PRE_VOL_MASK) >> PCA9420_MODE_ASYS_PRE_VOL_SHIFT;

	switch(reg)
	{
	case kPCA9420_AsysPreWarn3V3:
		*pBuffer = kPCA9420_AsysPreWarn3V3;
		break;
	case kPCA9420_AsysPreWarn3V4:
		*pBuffer = kPCA9420_AsysPreWarn3V4;
		break;
	case kPCA9420_AsysPreWarn3V5:
		*pBuffer = kPCA9420_AsysPreWarn3V5;
		break;
	case kPCA9420_AsysPreWarn3V6:
		*pBuffer = kPCA9420_AsysPreWarn3V6;
		break;
	default:
		*pBuffer = kPCA9420_AsysPreWarn3V5;
	}

	return SENSOR_ERROR_NONE;
}

//VIN input current limit selection
int32_t PCA9420_Set_vin_inp_cur(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_vin_ilim epca9420_vin_ilim)
{
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_TOP_CNTL0,
			(uint8_t)(epca9420_vin_ilim << PCA9420_MODE_VIN_ILIM_SEL_SHIFT), PCA9420_MODE_VIN_ILIM_SEL_MASK, repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_Get_vin_inp_cur(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_vin_ilim *pBuffer)
{
	uint8_t reg;
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Read(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_TOP_CNTL0, PCA9420UK_REG_SIZE_BYTES, &reg);

	if (ARM_DRIVER_OK != status)
	{
		pSensorHandle->isInitialized = false;
		return SENSOR_ERROR_INIT;
	}
	reg = (reg & PCA9420_MODE_VIN_ILIM_SEL_MASK) >> PCA9420_MODE_VIN_ILIM_SEL_SHIFT;

	switch(reg)
	{
	case kPCA9420_VinIlim_74_85_98:
		*pBuffer = kPCA9420_VinIlim_74_85_98;
		break;
	case kPCA9420_VinIlim_370_425_489:
		*pBuffer = kPCA9420_VinIlim_370_425_489;
		break;
	default:
		*pBuffer = kPCA9420_VinIlim_370_425_489;
	}

	return SENSOR_ERROR_NONE;
}


//regulator control APIs
int32_t PCA9420_enable_disable_bleed_resistor(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_vol_reg_source epca9420_vol_reg_source, uint8_t operation)
{
	int32_t status;
	uint8_t shift, mask;

	if(epca9420_vol_reg_source == kPCA9420_SW1)
	{
		shift = PCA9420_SW1_BLEED_SHIFT;
		mask =  PCA9420_SW1_BLEED_MASK;
	}
	else if(epca9420_vol_reg_source == kPCA9420_SW2)
	{
		shift = PCA9420_SW2_BLEED_SHIFT;
		mask =  PCA9420_SW2_BLEED_MASK;
	}
	else if(epca9420_vol_reg_source == kPCA9420_LDO1)
	{
		shift = PCA9420_LDO1_BLEED_SHIFT;
		mask =  PCA9420_LDO1_BLEED_MASK;
	}
	else if(epca9420_vol_reg_source == kPCA9420_LDO2)
	{
		shift = PCA9420_LDO2_BLEED_SHIFT;
		mask =  PCA9420_LDO2_BLEED_MASK;
	}

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_ACT_DIS_CNTL_1,
			(uint8_t)(operation << shift), mask, repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_ship_mode_enable_disable(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_mode epca9420_mode, uint8_t operation)
{
	int32_t status;
	uint8_t offset;

	if(epca9420_mode == kPCA9420_Mode0)
	{
		offset = PCA9420UK_MODECFG_0_0;
	}
	else if(epca9420_mode == kPCA9420_Mode1)
	{
		offset = PCA9420UK_MODECFG_1_0;
	}
	else if(epca9420_mode == kPCA9420_Mode2)
	{
		offset = PCA9420UK_MODECFG_2_0;
	}
	else if(epca9420_mode == kPCA9420_Mode3)
	{
		offset = PCA9420UK_MODECFG_3_0;
	}

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, offset,
			(uint8_t)(operation << PCA9420_SHIP_MODE_SHIFT), PCA9420_SHIP_MODE_MASK, repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_mode_control(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_mode epca9420_mode, uint8_t operation)
{
	int32_t status;
	uint8_t offset;

	if(epca9420_mode == kPCA9420_Mode0)
	{
		offset = PCA9420UK_MODECFG_0_0;
	}
	else if(epca9420_mode == kPCA9420_Mode1)
	{
		offset = PCA9420UK_MODECFG_1_0;
	}
	else if(epca9420_mode == kPCA9420_Mode2)
	{
		offset = PCA9420UK_MODECFG_2_0;
	}
	else if(epca9420_mode == kPCA9420_Mode3)
	{
		offset = PCA9420UK_MODECFG_3_0;
	}

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, offset,
			(uint8_t)(operation << PCA9420_MODE_CTRL_SHIFT), PCA9420_MODE_CTRL_MASK, repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_Set_sw1_out_vol(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_mode epca9420_mode, enum _pca9420_sw1_out epca9420_sw1_out)
{
	int32_t status;
	uint8_t offset, Data;

	if(epca9420_mode == kPCA9420_Mode0)
	{
		offset = PCA9420UK_MODECFG_0_0;
	}
	else if(epca9420_mode == kPCA9420_Mode1)
	{
		offset = PCA9420UK_MODECFG_1_0;
	}
	else if(epca9420_mode == kPCA9420_Mode2)
	{
		offset = PCA9420UK_MODECFG_2_0;
	}
	else if(epca9420_mode == kPCA9420_Mode3)
	{
		offset = PCA9420UK_MODECFG_3_0;
	}

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, offset,
			(uint8_t)(epca9420_sw1_out << PCA9420_SW1_VOL_SHIFT), PCA9420_SW1_VOL_MASK, repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_Set_sw2_out_vol(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_mode epca9420_mode, enum _pca9420_sw2_out epca9420_sw2_out)
{
	int32_t status;
	uint8_t offset;

	if(epca9420_mode == kPCA9420_Mode0)
	{
		offset = PCA9420UK_MODECFG_0_1;
	}
	else if(epca9420_mode == kPCA9420_Mode1)
	{
		offset = PCA9420UK_MODECFG_1_1;
	}
	else if(epca9420_mode == kPCA9420_Mode2)
	{
		offset = PCA9420UK_MODECFG_2_1;
	}
	else if(epca9420_mode == kPCA9420_Mode3)
	{
		offset = PCA9420UK_MODECFG_3_1;
	}

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, offset,
			(uint8_t)(epca9420_sw2_out << PCA9420_SW2_VOL_SHIFT), PCA9420_SW2_VOL_OFF_MASK, repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_Set_ldo1_out_vol(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_mode epca9420_mode, enum _pca9420_ldo1_out epca9420_ldo1_out)
{
	int32_t status;
	uint8_t offset;
	uint16_t Data;

	if(epca9420_mode == kPCA9420_Mode0)
	{
		offset = PCA9420UK_MODECFG_0_2;
	}
	else if(epca9420_mode == kPCA9420_Mode1)
	{
		offset = PCA9420UK_MODECFG_1_2;
	}
	else if(epca9420_mode == kPCA9420_Mode2)
	{
		offset = PCA9420UK_MODECFG_2_2;
	}
	else if(epca9420_mode == kPCA9420_Mode3)
	{
		offset = PCA9420UK_MODECFG_3_2;
	}

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, offset,
			(uint8_t)(epca9420_ldo1_out << PCA9420_LDO1_VOL_SHIFT), PCA9420_LDO1_VOL_MASK, repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_Set_ldo2_out_vol(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_mode epca9420_mode, enum _pca9420_ldo2_out epca9420_ldo2_out)
{
	int32_t status;
	uint8_t offset;

	if(epca9420_mode == kPCA9420_Mode0)
	{
		offset = PCA9420UK_MODECFG_0_3;
	}
	else if(epca9420_mode == kPCA9420_Mode1)
	{
		offset = PCA9420UK_MODECFG_1_3;
	}
	else if(epca9420_mode == kPCA9420_Mode2)
	{
		offset = PCA9420UK_MODECFG_2_3;
	}
	else if(epca9420_mode == kPCA9420_Mode3)
	{
		offset = PCA9420UK_MODECFG_3_3;
	}

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, offset,
			(uint8_t)(epca9420_ldo2_out << PCA9420_LDO2_VOL_SHIFT), PCA9420_LDO2_VOL_OFF_MASK, repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}


int32_t PCA9420_Set_wtchdg_timer(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_mode epca9420_mode, enum _pca9420_wd_timer epca9420_wd_timer)
{
	int32_t status;
	uint8_t offset;

	if(epca9420_mode == kPCA9420_Mode0)
	{
		offset = PCA9420UK_MODECFG_0_3;
	}
	else if(epca9420_mode == kPCA9420_Mode1)
	{
		offset = PCA9420UK_MODECFG_1_3;
	}
	else if(epca9420_mode == kPCA9420_Mode2)
	{
		offset = PCA9420UK_MODECFG_2_3;
	}
	else if(epca9420_mode == kPCA9420_Mode3)
	{
		offset = PCA9420UK_MODECFG_3_3;
	}

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, offset,
			(uint8_t)(epca9420_wd_timer << PCA9420_MODE_WD_TIMER_SHIFT), PCA9420_MODE_WD_TIMER_MASK, repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_vol_reg_enable_disable(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_mode epca9420_mode,
		enum _pca9420_vol_reg_source epca9420_vol_reg_source, uint8_t operation)
{
	int32_t status;
	uint8_t offset, shift, mask;
	uint8_t Data;

	if(epca9420_mode == kPCA9420_Mode0)
	{
		offset = PCA9420UK_MODECFG_0_2;
	}
	else if(epca9420_mode == kPCA9420_Mode1)
	{
		offset = PCA9420UK_MODECFG_1_2;
	}
	else if(epca9420_mode == kPCA9420_Mode2)
	{
		offset = PCA9420UK_MODECFG_2_2;
	}
	else if(epca9420_mode == kPCA9420_Mode3)
	{
		offset = PCA9420UK_MODECFG_3_2;
	}

	if(epca9420_vol_reg_source == kPCA9420_SW1)
	{
		shift = PCA9420_SW1_EN_SHIFT;
		mask =  PCA9420_SW1_EN_MASK;
	}
	else if(epca9420_vol_reg_source == kPCA9420_SW2)
	{
		shift = PCA9420_SW2_EN_SHIFT;
		mask =  PCA9420_SW2_EN_MASK;
	}
	else if(epca9420_vol_reg_source == kPCA9420_LDO1)
	{
		shift = PCA9420_LDO1_EN_SHIFT;
		mask =  PCA9420_LDO1_EN_MASK;
	}
	else if(epca9420_vol_reg_source == kPCA9420_LDO2)
	{
		shift = PCA9420_LDO2_EN_SHIFT;
		mask =  PCA9420_LDO2_EN_MASK;
	}

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, offset,
			(uint8_t)(operation << shift), mask, repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

//Mode setting configuration upon falling edge on ON pin
int32_t PCA9420_on_pin_mode_set(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_mode epca9420_mode, uint8_t operation)
{
	int32_t status;
	uint8_t offset;

	if(epca9420_mode == kPCA9420_Mode0)
	{
		offset = PCA9420UK_MODECFG_0_1;
	}
	else if(epca9420_mode == kPCA9420_Mode1)
	{
		offset = PCA9420UK_MODECFG_1_1;
	}
	else if(epca9420_mode == kPCA9420_Mode2)
	{
		offset = PCA9420UK_MODECFG_2_1;
	}
	else if(epca9420_mode == kPCA9420_Mode3)
	{
		offset = PCA9420UK_MODECFG_3_1;
	}

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, offset,
			(uint8_t)(operation << PCA9420_MODE_ON_CFG_SHIFT), PCA9420_MODE_ON_CFG_MASK, repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

//Clear interrupt
int32_t PCA9420_clear_interrupt(pca9420_i2c_sensorhandle_t *pSensorHandle)
{
	int32_t status;

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_SUB_INT0,
			PCA9420_CLR_INT_MASK, 0, repeatedStart);
	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_SUB_INT1,
			PCA9420_CLR_INT_MASK, 0, repeatedStart);
	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_SUB_INT2,
			PCA9420_CLR_INT_MASK, 0, repeatedStart);
	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

//Enable/disable system level interrupts
int32_t PCA9420_enable_disable_System_int(pca9420_i2c_sensorhandle_t *pSensorHandle, uint8_t data)
{
	int32_t status;

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_SUB_INT0_MASK,
			data, 0, repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

//Enable/disable linear battery charger block interrupts
int32_t PCA9420_enable_disable_bat_int(pca9420_i2c_sensorhandle_t *pSensorHandle, uint8_t data)
{
	int32_t status;

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_SUB_INT1_MASK,
			data, 0, repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}
	return SENSOR_ERROR_NONE;
}

//Enable/disable voltage level block interrupts
int32_t PCA9420_enable_disable_vol_reg_int(pca9420_i2c_sensorhandle_t *pSensorHandle, uint8_t data)
{
	int32_t status;

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_SUB_INT2_MASK,
			data, 0, repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

//Enable/disable charging in case of watchdog expire
int32_t PCA9420_chrg_conf_wtchdg(pca9420_i2c_sensorhandle_t *pSensorHandle, uint8_t operation)
{
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_TOP_CNTL0,
			(uint8_t)(operation << PCA9420UK_EN_CHG_IN_WTCH_SHIFT), PCA9420UK_EN_CHG_IN_WTCH_MASK, repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

//battery control APIs
int32_t PCA9420_enable_chg_lock(pca9420_i2c_sensorhandle_t *pSensorHandle)
{
	uint8_t reg;
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_CHG_CNTL0,
			0xAB, 0, repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_ntc_cntrl_enable_disable(pca9420_i2c_sensorhandle_t *pSensorHandle, uint8_t operation)
{
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_CHG_CNTL0,
			(uint8_t)(operation << PCA9420_NTC_EN_SHIFT), PCA9420_NTC_EN_MASK, repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_fast_preq_timer_enable_disable(pca9420_i2c_sensorhandle_t *pSensorHandle, uint8_t operation)
{
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_CHG_CNTL0,
			(uint8_t)(operation << PCA9420_CHG_TIMER_EN_SHIFT), PCA9420_CHG_TIMER_EN_MASK, repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_charger_enable_disable(pca9420_i2c_sensorhandle_t *pSensorHandle, uint8_t operation)
{
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_CHG_CNTL0,
			(uint8_t)(operation << PCA9420_CHG_EN_SHIFT), PCA9420_CHG_EN_MASK, repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

// battery charge control APIs
int32_t PCA9420_Set_battery_charge_cur(pca9420_i2c_sensorhandle_t *pSensorHandle, uint8_t epca9420_bat_chrg_cur)
{
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_CHG_CNTL1,
			(uint8_t)(epca9420_bat_chrg_cur << PCA9420_MODE_ICHG_CC_SHIFT), PCA9420_MODE_ICHG_CC_MASK, repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

//Program top off current
int32_t PCA9420_Set_battery_topoff_cur(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_bat_topoff_cur epca9420_bat_topoff_cur)
{
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_CHG_CNTL2,
			(uint8_t)(epca9420_bat_topoff_cur << PCA9420_MODE_ICHG_TOPOFF_SHIFT), PCA9420_MODE_ICHG_TOPOFF_MASK, repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

//low battery charge current
int32_t PCA9420_Set_low_battery_chrg_cur(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_low_bat_chrg_cur epca9420_low_bat_chrg_cur)
{
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_CHG_CNTL3,
			(uint8_t)(epca9420_low_bat_chrg_cur << PCA9420_MODE_ICHG_LOW_SHIFT), PCA9420_MODE_ICHG_LOW_MASK, repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_Get_low_battery_chrg_cur(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_low_bat_chrg_cur *pBuffer)
{
	uint8_t reg;
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Read(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_CHG_CNTL3, PCA9420UK_REG_SIZE_BYTES, &reg);

	if (ARM_DRIVER_OK != status)
	{
		pSensorHandle->isInitialized = false;
		return SENSOR_ERROR_INIT;
	}
	reg = (reg & PCA9420_MODE_ICHG_LOW_MASK) >> PCA9420_MODE_ICHG_LOW_SHIFT;

	switch(reg)
	{
	case kPCA9420_ICHG_LOW_0:
		*pBuffer = kPCA9420_ICHG_LOW_0;
		break;
	case kPCA9420_ICHG_LOW_8:
		*pBuffer = kPCA9420_ICHG_LOW_8;
		break;
	case kPCA9420_ICHG_LOW_16:
		*pBuffer = kPCA9420_ICHG_LOW_16;
		break;
	case kPCA9420_ICHG_LOW_24:
		*pBuffer = kPCA9420_ICHG_LOW_24;
		break;
	default:
		*pBuffer = kPCA9420_ICHG_LOW_8;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_Set_dead_chrg_timer(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_dead_chrg_timer epca9420_dead_chrg_timer)
{
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_CHG_CNTL4,
			(uint8_t)(epca9420_dead_chrg_timer << PCA9420_MODE_ICHG_DEAD_TIMER_SHIFT), PCA9420_MODE_ICHG_DAED_TIMER_MASK, repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_Set_dead_chrg_cur(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_dead_bat_chrg_cur epca9420_dead_bat_chrg_cur)
{
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_CHG_CNTL4,
			(uint8_t)(epca9420_dead_bat_chrg_cur << PCA9420_MODE_ICHG_DEAD_SHIFT), PCA9420_MODE_ICHG_DAED_MASK, repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_Get_dead_chrg_cur(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_dead_bat_chrg_cur *pBuffer)
{
	uint8_t reg;
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Read(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_CHG_CNTL4, PCA9420UK_REG_SIZE_BYTES, &reg);

	if (ARM_DRIVER_OK != status)
	{
		pSensorHandle->isInitialized = false;
		return SENSOR_ERROR_INIT;
	}
	reg = (reg & PCA9420_MODE_ICHG_DAED_MASK) >> PCA9420_MODE_ICHG_DEAD_SHIFT;

	switch(reg)
	{
	case kPCA9420_ICHG_DEAD_0:
		*pBuffer = kPCA9420_ICHG_DEAD_0;
		break;
	case kPCA9420_ICHG_DEAD_4:
		*pBuffer = kPCA9420_ICHG_DEAD_4;
		break;
	case kPCA9420_ICHG_DEAD_16:
		*pBuffer = kPCA9420_ICHG_DEAD_16;
		break;
	case kPCA9420_ICHG_DEAD_20:
		*pBuffer = kPCA9420_ICHG_DEAD_20;
		break;
	default:
		*pBuffer = kPCA9420_ICHG_DEAD_4;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_Set_thrshld_rechrg(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_threshld_rechrg epca9420_threshld_rechrg)
{
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_CHG_CNTL5,
			(uint8_t)(epca9420_threshld_rechrg << PCA9420_VBAT_RESTART_SHIFT), PCA9420_VBAT_RESTART_MASK, repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_Set_bat_regulation_vol(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_bat_reg_vol epca9420_bat_reg_vol)
{
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_CHG_CNTL5,
			(uint8_t)(epca9420_bat_reg_vol << PCA9420_VBAT_REG_SHIFT), PCA9420_VBAT_REG_MASK, repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_Set_thmstr_resistance(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_ntc_res_sel epca9420_ntc_res_sel)
{
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_CHG_CNTL6,
			(uint8_t)(epca9420_ntc_res_sel << PCA9420_NTC_RES_SEL_SHIFT), PCA9420_NTC_RES_SEL_MASK,  repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_Get_thmstr_resistance(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_ntc_res_sel *pBuffer)
{
	uint8_t reg;
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Read(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_CHG_CNTL6, PCA9420UK_REG_SIZE_BYTES, &reg);

	if (ARM_DRIVER_OK != status)
	{
		pSensorHandle->isInitialized = false;
		return SENSOR_ERROR_INIT;
	}
	reg = (reg & PCA9420_NTC_RES_SEL_MASK) >> PCA9420_NTC_RES_SEL_SHIFT;

	switch(reg)
	{
	case kPCA9420_NTC_RES_SEL_100:
		*pBuffer = kPCA9420_NTC_RES_SEL_100;
		break;
	case kPCA9420_NTC_RES_SEL_10:
		*pBuffer = kPCA9420_NTC_RES_SEL_10;
		break;
	default:
		*pBuffer = kPCA9420_NTC_RES_SEL_10;
	}

	return SENSOR_ERROR_NONE;
}


int32_t PCA9420_Set_fast_chrg_timer(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_fast_chrg_timer epca9420_fast_chrg_timer)
{
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_CHG_CNTL6,
			(uint8_t)(epca9420_fast_chrg_timer << PCA9420_ICHG_FAST_TIMER_SHIFT), PCA9420_ICHG_FAST_TIMER_MASK, repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_Get_fast_chrg_timer(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_fast_chrg_timer *pBuffer)
{
	uint8_t reg;
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Read(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_CHG_CNTL6, PCA9420UK_REG_SIZE_BYTES, &reg);

	if (ARM_DRIVER_OK != status)
	{
		pSensorHandle->isInitialized = false;
		return SENSOR_ERROR_INIT;
	}
	reg = (reg & PCA9420_ICHG_FAST_TIMER_MASK) >> PCA9420_ICHG_FAST_TIMER_SHIFT;

	switch(reg)
	{
	case kPCA9420_ICHG_FAST_TIMER_3:
		*pBuffer = kPCA9420_ICHG_FAST_TIMER_3;
		break;
	case kPCA9420_ICHG_FAST_TIMER_5:
		*pBuffer = kPCA9420_ICHG_FAST_TIMER_5;
		break;
	case kPCA9420_ICHG_FAST_TIMER_7:
		*pBuffer = kPCA9420_ICHG_FAST_TIMER_7;
		break;
	case kPCA9420_ICHG_FAST_TIMER_9:
		*pBuffer = kPCA9420_ICHG_FAST_TIMER_9;
		break;
	default:
		*pBuffer = kPCA9420_ICHG_FAST_TIMER_5;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_Set_preq_timer(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_preq_chrg_timer epca9420_preq_chrg_timer)
{
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_CHG_CNTL6,
			(uint8_t)(epca9420_preq_chrg_timer << PCA9420_ICHG_PREQ_TIMER_SHIFT), PCA9420_ICHG_PREQ_TIMER_MASK, repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_Get_preq_timer(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_preq_chrg_timer *pBuffer)
{
	uint8_t reg;
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Read(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_CHG_CNTL6, PCA9420UK_REG_SIZE_BYTES, &reg);

	if (ARM_DRIVER_OK != status)
	{
		pSensorHandle->isInitialized = false;
		return SENSOR_ERROR_INIT;
	}
	reg = (reg & PCA9420_ICHG_PREQ_TIMER_MASK) >> PCA9420_ICHG_PREQ_TIMER_SHIFT;

	switch(reg)
	{
	case kPCA9420_ICHG_PREQ_TIMER_15:
		*pBuffer = kPCA9420_ICHG_PREQ_TIMER_15;
		break;
	case kPCA9420_ICHG_PREQ_TIMER_30:
		*pBuffer = kPCA9420_ICHG_PREQ_TIMER_30;
		break;
	case kPCA9420_ICHG_PREQ_TIMER_45:
		*pBuffer = kPCA9420_ICHG_PREQ_TIMER_45;
		break;
	case kPCA9420_ICHG_PREQ_TIMER_60:
		*pBuffer = kPCA9420_ICHG_PREQ_TIMER_60;
		break;
	default:
		*pBuffer = kPCA9420_ICHG_PREQ_TIMER_30;
	}
	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_Set_topoff_timer(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_topoff_timer epca9420_topoff_timer)
{
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_CHG_CNTL6,
			(uint8_t)(epca9420_topoff_timer << PCA9420_T_TOPOFF_TIMER_SHIFT), PCA9420_T_TOPOFF_TIMER_MASK, repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_Get_topoff_timer(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_topoff_timer *pBuffer)
{
	uint8_t reg;
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Read(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_CHG_CNTL6, PCA9420UK_REG_SIZE_BYTES, &reg);

	if (ARM_DRIVER_OK != status)
	{
		pSensorHandle->isInitialized = false;
		return SENSOR_ERROR_INIT;
	}
	reg = (reg & PCA9420_T_TOPOFF_TIMER_MASK) >> PCA9420_T_TOPOFF_TIMER_SHIFT;

	switch(reg)
	{
	case kPCA9420_TOPOFF_TIMER_0:
		*pBuffer = kPCA9420_TOPOFF_TIMER_0;
		break;
	case kPCA9420_TOPOFF_TIMER_6_4:
		*pBuffer = kPCA9420_TOPOFF_TIMER_6_4;
		break;
	case kPCA9420_TOPOFF_TIMER_12_8:
		*pBuffer = kPCA9420_TOPOFF_TIMER_12_8;
		break;
	case kPCA9420_TOPOFF_TIMER_19_2:
		*pBuffer = kPCA9420_TOPOFF_TIMER_19_2;
		break;
	default:
		*pBuffer = kPCA9420_TOPOFF_TIMER_6_4;
	}
	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_Set_ntc_beta_val(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_ntc_beta_val epca9420_ntc_beta_val)
{
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_CHG_CNTL7,
			(uint8_t)(epca9420_ntc_beta_val << PCA9420_NTC_BETA_SHIFT), PCA9420_NTC_BETA_MASK, repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_Get_ntc_beta_val(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_ntc_beta_val *pBuffer)
{
	uint8_t reg;
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Read(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_CHG_CNTL7, PCA9420UK_REG_SIZE_BYTES, &reg);

	if (ARM_DRIVER_OK != status)
	{
		pSensorHandle->isInitialized = false;
		return SENSOR_ERROR_INIT;
	}
	reg = (reg & PCA9420_NTC_BETA_MASK) >> PCA9420_NTC_BETA_SHIFT;

	switch(reg)
	{
	case kPCA9420_NTC_BETA_SEL_3434:
		*pBuffer = kPCA9420_NTC_BETA_SEL_3434;
		break;
	case kPCA9420_NTC_BETA_SEL_3610:
		*pBuffer = kPCA9420_NTC_BETA_SEL_3610;
		break;
	case kPCA9420_NTC_BETA_SEL_3934:
		*pBuffer = kPCA9420_NTC_BETA_SEL_3934;
		break;
	case kPCA9420_NTC_BETA_SEL_3950:
		*pBuffer = kPCA9420_NTC_BETA_SEL_3950;
		break;
	case kPCA9420_NTC_BETA_SEL_4100:
		*pBuffer = kPCA9420_NTC_BETA_SEL_4100;
		break;
	case kPCA9420_NTC_BETA_SEL_4311:
		*pBuffer = kPCA9420_NTC_BETA_SEL_4311;
		break;
	case kPCA9420_NTC_BETA_SEL_4543:
		*pBuffer = kPCA9420_NTC_BETA_SEL_4543;
		break;
	case kPCA9420_NTC_BETA_SEL_4750:
		*pBuffer = kPCA9420_NTC_BETA_SEL_4750;
		break;
	default:
		*pBuffer = kPCA9420_NTC_BETA_SEL_3610;
	}
	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_Set_thrml_reg_thshld(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_thrml_reg_thshld epca9420_thrml_reg_thshld)
{
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_CHG_CNTL7,
			(uint8_t)(epca9420_thrml_reg_thshld << PCA9420_THM_REG_SHIFT), PCA9420_THM_REG_MASK, repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}
	return SENSOR_ERROR_NONE;
}

int32_t PCA9420_Get_thrml_reg_thshld(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_thrml_reg_thshld *pBuffer)
{
	uint8_t reg;
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Read(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, PCA9420UK_CHG_CNTL7, PCA9420UK_REG_SIZE_BYTES, &reg);

	if (ARM_DRIVER_OK != status)
	{
		pSensorHandle->isInitialized = false;
		return SENSOR_ERROR_INIT;
	}
	reg = (reg & PCA9420_THM_REG_MASK) >> PCA9420_THM_REG_SHIFT;

	switch(reg)
	{
	case kPCA9420_THM_REG_80:
		*pBuffer = kPCA9420_THM_REG_80;
		break;
	case kPCA9420_THM_REG_85:
		*pBuffer = kPCA9420_THM_REG_85;
		break;
	case kPCA9420_THM_REG_90:
		*pBuffer = kPCA9420_THM_REG_90;
		break;
	case kPCA9420_THM_REG_95:
		*pBuffer = kPCA9420_THM_REG_95;
		break;
	case kPCA9420_THM_REG_100:
		*pBuffer = kPCA9420_THM_REG_100;
		break;
	case kPCA9420_THM_REG_105:
		*pBuffer = kPCA9420_THM_REG_105;
		break;
	case kPCA9420_THM_REG_110:
		*pBuffer = kPCA9420_THM_REG_110;
		break;
	case kPCA9420_THM_REG_115:
		*pBuffer = kPCA9420_THM_REG_115;
		break;
	default:
		*pBuffer = kPCA9420_THM_REG_110;
	}
	return SENSOR_ERROR_NONE;
}
