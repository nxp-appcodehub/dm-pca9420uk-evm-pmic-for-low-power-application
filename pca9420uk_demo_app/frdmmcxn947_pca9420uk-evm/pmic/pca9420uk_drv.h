/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file pca9420uk_drv.h
 * @brief The pca9420uk_drv.h file describes the PCA9420UK PMIC driver interface and structures.
 */

#ifndef PCA9420_UK_H_
#define PCA9420_UK_H_

/* Standard C Includes */
#include <stdint.h>

/* ISSDK Includes */
#include "sensor_io_i2c.h"
#include "register_io_i2c.h"
#include "pca9420uk.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @brief This defines the sensor specific information for I2C.
 */
typedef struct
{
    registerDeviceInfo_t deviceInfo; /*!< I2C device context. */
    ARM_DRIVER_I2C *pCommDrv;        /*!< Pointer to the i2c driver. */
    bool isInitialized;              /*!< whether sensor is intialized or not.*/
    uint16_t slaveAddress;           /*!< slave address.*/
} pca9420_i2c_sensorhandle_t;

/*******************************************************************************
 * APIs
 ******************************************************************************/

/*! @brief       The interface function to initialize the PMIC.
 *  @details     This function initialize the PMIC and PMIC handle.
 *  @param[in]   pSensorHandle  handle to the PMIC.
 *  @param[in]   pBus           pointer to the CMSIS API compatible I2C bus object.
 *  @param[in]   index          the I2C device number.
 *  @param[in]   sAddress       slave address of the device on the bus.
 *  @constraints This should be the first API to be called.
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_I2C_Initialize() returns the status .
 */
int32_t PCA9420_I2C_Initialize(pca9420_i2c_sensorhandle_t *pSensorHandle, ARM_DRIVER_I2C *pBus, uint8_t index, uint16_t sAddress);

/*! @brief       The interface function to read the PMIC registers.
 *  @details     This function is to read the PMIC register value.
 *  @param[in]   pSensorHandle  handle to the PMIC.
 *  @param[in]   RegAddress     register address to read the value.
 *  @param[in]   Data           Pointer to memory location where retrieved data is stored.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_DRV_Read() returns the status .
 */
int32_t PCA9420_DRV_Read(pca9420_i2c_sensorhandle_t *pSensorHandle, uint8_t RegAddress, uint16_t* Data);

/*! @brief       The interface function to write the PMIC registers.
 *  @details     This function is to write the PMIC registers value.
 *  @param[in]   pSensorHandle  handle to the PMIC.
 *  @param[in]   RegAddress     register address to write data.
 *  @param[in]   Data           memory location where retrieved data is stored.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_DRV_Write() returns the status .
 */
int32_t PCA9420_DRV_Write(pca9420_i2c_sensorhandle_t *pSensorHandle, uint8_t RegAddress, uint16_t Data);

//System Control APIs

/*! @brief       The interface function to configure VIN input current limit.
 *  @details     This function is to configure VIN input current limit.
 *  @param[in]   pSensorHandle 		    handle to the PMIC.
 *  @param[in]   epca9420_vin_ilim          vin input current.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_Set_vin_inp_cur() returns the status .
 */
int32_t PCA9420_Set_vin_inp_cur(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_vin_ilim epca9420_vin_ilim);

/*! @brief       The interface function to configure VIN input current limit.
 *  @details     This function is to read VIN input current limit.
 *  @param[in]   pSensorHandle 		    handle to the PMIC.
 *  @param[in]   epca9420_vin_ilim          memory location where retrived vin input current will be stored.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_Get_vin_inp_cur() returns the status .
 */
int32_t PCA9420_Get_vin_inp_cur(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_vin_ilim * epca9420_vin_ilim);

/*! @brief       The interface function to configure ASYS pre-warning voltage threshold.
 *  @details     This function is to configure ASYS pre-warning voltage threshold.
 *  @param[in]   pSensorHandle 		           handle to the PMIC.
 *  @param[in]   _pca9420_asys_prewarning          ASYS pre-warning threshold.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_Set_asys_prewarn_vol_tshld() returns the status .
 */
int32_t PCA9420_Set_asys_prewarn_vol_tshld(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_asys_prewarning epca9420_asys_prewarning);

/*! @brief       The interface function to read ASYS pre-warning voltage threshold.
 *  @details     This function is to read ASYS pre-warning voltage threshold.
 *  @param[in]   pSensorHandle 		           handle to the PMIC.
 *  @param[in]   pBuffer                       handle to the output buffer.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_Get_asys_prewarn_vol_tshld() returns the status .
 */
int32_t PCA9420_Get_asys_prewarn_vol_tshld(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_asys_prewarning *pBuffer);

/*! @brief       The interface function to configure ASYS input source.
 *  @details     This function is to configure ASYS input source either VIN or VBAT.
 *  @param[in]   pSensorHandle 		           handle to the PMIC.
 *  @param[in]   epca9420_asys_input_sel       ASYS input source either VIN or VBAT.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_asys_input_src_sel() returns the status .
 */
int32_t PCA9420_asys_input_src_sel(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_asys_input_sel epca9420_asys_input_sel);

/*! @brief       The interface function to configure VIN over-voltage threshold.
 *  @details     This function is to configure ASYS VIN over-voltage threshold.
 *  @param[in]   pSensorHandle 		    handle to the PMIC.
 *  @param[in]   epca9420_vin_ovp       vin over-voltage thresholds.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_Set_vin_overvoltage_tshld() returns the status .
 */
int32_t PCA9420_Set_vin_overvoltage_tshld(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_vin_ovp_sel epca9420_vin_ovp);

/*! @brief       The interface function to get VIN over-voltage threshold.
 *  @details     This function is to read ASYS VIN over-voltage threshold.
 *  @param[in]   pSensorHandle 		    handle to the PMIC.
 *  @param[in]   pBuffer                handle to the output buffer.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_Get_vin_overvoltage_tshld() returns the status .
 */
int32_t PCA9420_Get_vin_overvoltage_tshld(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_vin_ovp_sel *pBuffer);

/*! @brief       The interface function to configure VIN under-voltage threshold.
 *  @details     This function is to configure ASYS VIN under-voltage threshold.
 *  @param[in]   pSensorHandle 		    handle to the PMIC.
 *  @param[in]   epca9420_vin_uvlo       vin under-voltage thresholds.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_Set_vin_uvlo_tshld() returns the status .
 */
int32_t PCA9420_Set_vin_uvlo_tshld(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_vin_uvlo_sel epca9420_vin_uvlo);

/*! @brief       The interface function to get VIN under-voltage threshold.
 *  @details     This function is to read ASYS VIN under-voltage threshold.
 *  @param[in]   pSensorHandle 		    handle to the PMIC.
 *  @param[in]   pBuffer                       handle to the output buffer.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_Get_vin_uvlo_tshld() returns the status .
 */
int32_t PCA9420_Get_vin_uvlo_tshld(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_vin_uvlo_sel *pBuffer);

/*! @brief       The interface function to configure ASYS under-voltage threshold.
 *  @details     This function is to configure ASYS under-voltage threshold.
 *  @param[in]   pSensorHandle 		     handle to the PMIC.
 *  @param[in]   epca9420_asys_uvlo      asys under-voltage thresholds.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_Set_asys_uvlo_tshld() returns the status .
 */
int32_t PCA9420_Set_asys_uvlo_tshld(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_asys_uvlo_sel epca9420_asys_uvlo);

/*! @brief       The interface function to configure ASYS under-voltage threshold.
 *  @details     This function is to configure ASYS under-voltage threshold.
 *  @param[in]   pSensorHandle 		     handle to the PMIC.
 *  @param[in]   pBuffer                 handle to the output buffer.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_Get_asys_uvlo_tshld() returns the status .
 */
int32_t PCA9420_Get_asys_uvlo_tshld(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_asys_uvlo_sel *pBuffer);

/*! @brief       The interface function to configure thermal shutdown threshold.
 *  @details     This function is to configure thermal shutdown threshold.
 *  @param[in]   pSensorHandle 		     handle to the PMIC.
 *  @param[in]   epca9420_them_shdn      thermal shutdown thresholds.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_Set_thrml_shtdn_tshld() returns the status .
 */
int32_t PCA9420_Set_thrml_shtdn_tshld(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_them_shdn epca9420_them_shdn);

/*! @brief       The interface function to get thermal shutdown threshold.
 *  @details     This function is to read thermal shutdown threshold.
 *  @param[in]   pSensorHandle 		     handle to the PMIC.
 *  @param[in]   pBuffer                 handle to the output buffer.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_Get_thrml_shtdn_tshld() returns the status .
 */
int32_t PCA9420_Get_thrml_shtdn_tshld(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_them_shdn *pBuffer);

/*! @brief       The interface function to configure die temperature threshold.
 *  @details     This function is to configure die temperature threshold.
 *  @param[in]   pSensorHandle 		    handle to the PMIC.
 *  @param[in]   epca9420_die_temp      die temperature thresholds.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_Set_die_temp_tshld() returns the status .
 */
int32_t PCA9420_Set_die_temp_tshld(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_die_temp_warning epca9420_die_temp);

/*! @brief       The interface function to get die temperature threshold.
 *  @details     This function is to read die temperature threshold.
 *  @param[in]   pSensorHandle 		    handle to the PMIC.
 *  @param[in]   pBuffer                handle to the output buffer.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_Get_die_temp_tshld() returns the status .
 */
int32_t PCA9420_Get_die_temp_tshld(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_die_temp_warning *pBuffer);

/*! @brief       The interface function to read mode control.
 *  @details     This function is to read mode control (Mode 0/1/2/3).
 *  @param[in]   pSensorHandle 		handle to the PMIC.
 *  @param[in]   pBuffer            handle to the output buffer.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_Get_mode_control() returns the status .
 */
int32_t PCA9420_Get_mode_control(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_mode *pBuffer);

/*! @brief       The interface function to configure mode control.
 *  @details     This function is to configure mode control (Mode 0/1/2/3).
 *  @param[in]   pSensorHandle 		handle to the PMIC.
 *  @param[in]   epca9420_mode      mode selected.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_Set_mode_control() returns the status .
 */
int32_t PCA9420_Set_mode_control(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_mode epca9420_mode);

/*! @brief       The interface function to reset the PMIC.
 *  @details     This function is to reset the PMIC board with default configurations.
 *  @param[in]   pSensorHandle 		handle to the PMIC.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_SW_reset() returns the status.
 */
int32_t PCA9420_SW_reset(pca9420_i2c_sensorhandle_t *pSensorHandle);

/*! @brief       The interface function to get long glitch timer.
 *  @details     This function is to read long glitch timer to reset the PMIC on long press.
 *  @param[in]   pSensorHandle 		handle to the PMIC.
 *  @param[in]   pBuffer            handle to the output buffer.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_Get_long_glitch_timer() returns the status.
 */
int32_t PCA9420_Get_long_glitch_timer(pca9420_i2c_sensorhandle_t *pSensorHandle, enum LongGlitchTimer *pBuffer);

/*! @brief       The interface function to configure long glitch timer.
 *  @details     This function is to configure long glitch timer to reset the PMIC on long press.
 *  @param[in]   pSensorHandle 		handle to the PMIC.
 *  @param[in]   eLngGthTmr         long glitch timer.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_Set_long_glitch_timer() returns the status.
 */
int32_t PCA9420_Set_long_glitch_timer(pca9420_i2c_sensorhandle_t *pSensorHandle, enum LongGlitchTimer eLngGthTmr);

/*! @brief       The interface function to reset the watchdog timer.
 *  @details     This function is to reset the watchdog timer.
 *  @param[in]   pSensorHandle 		handle to the PMIC.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_wtchdg_timer_reset() returns the status.
 */
int32_t PCA9420_wtchdg_timer_reset(pca9420_i2c_sensorhandle_t *pSensorHandle);


//Regulator control APIs

/*! @brief       The interface function to configure the bleed resistor.
 *  @details     This function is to enable/disable the bleed resistor.
 *  @param[in]   pSensorHandle 					handle to the PMIC.
 *  @param[in]   epca9420_vol_reg_source      	voltage regulation source.
 *  @param[in]   operation      	            enable/disable bleeding resistor.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_enable_disable_bleed_resistor() returns the status.
 */
int32_t PCA9420_enable_disable_bleed_resistor(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_vol_reg_source epca9420_vol_reg_source, uint8_t operation);

/*! @brief       The interface function to configure the ship mode.
 *  @details     This function is to enable/disable the ship mode.
 *  @param[in]   pSensorHandle 			handle to the PMIC.
 *  @param[in]   epca9420_mode      	mode selected.
 *  @param[in]   operation      	    enable/disable ship mode.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_ship_mode_enable_disable() returns the status.
 */
int32_t PCA9420_ship_mode_enable_disable(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_mode epca9420_mode, uint8_t operation);

/*! @brief       The interface function to configure the output voltage regulator source.
 *  @details     This function is to enable/disable the output voltage regulator source.
 *  @param[in]   pSensorHandle 		        	handle to the PMIC.
 *  @param[in]   epca9420_mode      	        mode selected.
 *  @param[in]   epca9420_vol_reg_source      	output voltage regulation source.
 *  @param[in]   operation      	    		to enable/disable voltage regulator source.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_vol_reg_enable_disable() returns the status.
 */
int32_t PCA9420_vol_reg_enable_disable(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_mode epca9420_mode,
		enum _pca9420_vol_reg_source epca9420_vol_reg_source, uint8_t operation);

/*! @brief       The interface function to configure SW1 buck regulator voltage.
 *  @details     This function is to configure SW1 buck regulator voltage.
 *  @param[in]   pSensorHandle 				handle to the PMIC.
 *  @param[in]   epca9420_mode      		mode selected.
 *  @param[in]   epca9420_sw1_out      	    voltage to set in sw1 buck regulator voltage.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_Set_sw1_out_vol() returns the status.
 */
int32_t PCA9420_Set_sw1_out_vol(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_mode epca9420_mode, enum _pca9420_sw1_out epca9420_sw1_out);

/*! @brief       The interface function to configure SW2 buck regulator voltage.
 *  @details     This function is to configure SW2 buck regulator voltage.
 *  @param[in]   pSensorHandle 				handle to the PMIC.
 *  @param[in]   epca9420_mode      		mode selected.
 *  @param[in]   epca9420_sw2_out      	    voltage to set in sw2 buck regulator.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_Set_sw2_out_vol() returns the status.
 */
int32_t PCA9420_Set_sw2_out_vol(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_mode epca9420_mode, enum _pca9420_sw2_out epca9420_sw2_out);

/*! @brief       The interface function to configure LDO1 regulator voltage.
 *  @details     This function is to configure LDO1 regulator voltage.
 *  @param[in]   pSensorHandle 				handle to the PMIC.
 *  @param[in]   epca9420_mode      		mode selected.
 *  @param[in]   epca9420_ldo1_out      	voltage to set in LDO1 buck.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_Set_ldo1_out_vol() returns the status.
 */
int32_t PCA9420_Set_ldo1_out_vol(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_mode epca9420_mode, enum _pca9420_ldo1_out epca9420_ldo1_out);

/*! @brief       The interface function to configure LDO2 regulator voltage.
 *  @details     This function is to configure LDO2 regulator voltage.
 *  @param[in]   pSensorHandle 				handle to the PMIC.
 *  @param[in]   epca9420_mode      		mode selected.
 *  @param[in]   epca9420_ldo2_out      	voltage to set in LDO2 buck regulator.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_Set_ldo2_out_vol() returns the status.
 */
int32_t PCA9420_Set_ldo2_out_vol(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_mode epca9420_mode, enum _pca9420_ldo2_out epca9420_ldo2_out);

/*! @brief       The interface function to configure on pin mode setting.
 *  @details     This function is to configure on pin mode setting.
 *  @param[in]   pSensorHandle 				handle to the PMIC.
 *  @param[in]   epca9420_mode      		mode selected.
 *  @param[in]   operation      	        on pin configuration.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_on_pin_mode_set() returns the status.
 */
int32_t PCA9420_on_pin_mode_set(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_mode epca9420_mode, uint8_t operation);

/*! @brief       The interface function to configure watchdog timer setting.
 *  @details     This function is to configure watchdog timer setting.
 *  @param[in]   pSensorHandle 				handle to the PMIC.
 *  @param[in]   epca9420_mode      		mode selected.
 *  @param[in]   epca9420_wd_timer      	watchdog timer settings.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_Set_wtchdg_timer() returns the status.
 */
int32_t PCA9420_Set_wtchdg_timer(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_mode epca9420_mode, enum _pca9420_wd_timer epca9420_wd_timer);

/*! @brief       The interface function to configure the mode setting.
 *  @details     This function is to configure the mode setting either via I2C register or mode selection line.
 *  @param[in]   pSensorHandle 		handle to the PMIC.
 *  @param[in]   epca9420_mode      mode selected.
 *  @param[in]   operation      	mode control configuration.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_mode_control() returns the status.
 */
int32_t PCA9420_mode_control(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_mode epca9420_mode, uint8_t operation);

/*! @brief       The interface function to clear the interrupts.
 *  @details     This function is to clear the top level and sub level interrupts.
 *  @param[in]   pSensorHandle 		handle to the PMIC.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_clear_interrupt() returns the status.
 */
int32_t PCA9420_clear_interrupt(pca9420_i2c_sensorhandle_t *pSensorHandle);

/*! @brief       The interface function to configure system level interrupt.
 *  @details     This function is to enable/disbale the system level interrupt.
 *  @param[in]   pSensorHandle 		handle to the PMIC.
 *  @param[in]   data 		        data to write.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_enable_disable_System_int() returns the status.
 */
int32_t PCA9420_enable_disable_System_int(pca9420_i2c_sensorhandle_t *pSensorHandle, uint8_t data);

/*! @brief       The interface function to configure battery charger block interrupt.
 *  @details     This function is to enable/disbale battery charger block interrupt.
 *  @param[in]   pSensorHandle 		handle to the PMIC.
 *  @param[in]   data 		        data to write.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_enable_disable_bat_int() returns the status.
 */
int32_t PCA9420_enable_disable_bat_int(pca9420_i2c_sensorhandle_t *pSensorHandle, uint8_t data);

/*! @brief       The interface function to configure voltage regulator block interrupt.
 *  @details     This function is to enable/disbale voltage regulator block interrupt.
 *  @param[in]   pSensorHandle 		handle to the PMIC.
 *  @param[in]   data 		        data to write.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_enable_disable_vol_reg_int() returns the status.
 */
int32_t PCA9420_enable_disable_vol_reg_int(pca9420_i2c_sensorhandle_t *pSensorHandle, uint8_t data);

#if (!PCA9421UK_EVM_EN)
//Battery charger control APIs

/*! @brief       The interface function to configure charging in case of watchdog timer expire.
 *  @details     This function is to enable/disbale charging in case of watchdog timer expire.
 *  @param[in]   pSensorHandle 		handle to the PMIC.
 *  @param[in]   operation 		    enable/disable charging.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_enable_disable_vol_reg_int() returns the status.
 */
int32_t PCA9420_chrg_conf_wtchdg(pca9420_i2c_sensorhandle_t *pSensorHandle, uint8_t operation);

/*! @brief       The interface function to enable the charging configuration lock.
 *  @details     This function is to enable the chraging configuration lock in order to write data.
 *  @param[in]   pSensorHandle 		handle to the PMIC.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_enable_chg_lock() returns the status.
 */
int32_t PCA9420_enable_chg_lock(pca9420_i2c_sensorhandle_t *pSensorHandle);

/*! @brief       The interface function to configure NTC control.
 *  @details     This function is to configure NTC control to enable/disable.
 *  @param[in]   pSensorHandle 		handle to the PMIC.
 *  @param[in]   operation          enable/disable to perform.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_ntc_cntrl_enable_disable() returns the status.
 */
int32_t PCA9420_ntc_cntrl_enable_disable(pca9420_i2c_sensorhandle_t *pSensorHandle, uint8_t operation);

/*! @brief       The interface function to configure pre-qualification timer.
 *  @details     This function is to configure pre-qualification timer to enable/disable.
 *  @param[in]   pSensorHandle 		handle to the PMIC.
 *  @param[in]   operation          operation to enable/disable.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_fast_preq_timer_enable_disable() returns the status.
 */
int32_t PCA9420_fast_preq_timer_enable_disable(pca9420_i2c_sensorhandle_t *pSensorHandle, uint8_t operation);

/*! @brief       The interface function to configure battery charging current.
 *  @details     This function is to configure battery charging current for battery charging.
 *  @param[in]   pSensorHandle 		       handle to the PMIC.
 *  @param[in]   epca9420_bat_chrg_cur     battery charging current.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_Set_battery_charge_cur() returns the status.
 */
int32_t PCA9420_Set_battery_charge_cur(pca9420_i2c_sensorhandle_t *pSensorHandle, uint8_t epca9420_bat_chrg_cur);

/*! @brief       The interface function to configure battery topoff charging current.
 *  @details     This function is to configure battery topoff charging current for battery charging.
 *  @param[in]   pSensorHandle 		       handle to the PMIC.
 *  @param[in]   epca9420_bat_topoff_cur   battery topoff current.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_Set_battery_topoff_cur() returns the status.
 */
int32_t PCA9420_Set_battery_topoff_cur(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_bat_topoff_cur epca9420_bat_topoff_cur);

/*! @brief       The interface function to configure low battery charging current.
 *  @details     This function is to configure low battery charging current for battery charging.
 *  @param[in]   pSensorHandle 		         handle to the PMIC.
 *  @param[in]   epca9420_low_bat_chrg_cur   low battery charging current.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_Set_low_battery_chrg_cur() returns the status.
 */
int32_t PCA9420_Set_low_battery_chrg_cur(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_low_bat_chrg_cur epca9420_low_bat_chrg_cur);

/*! @brief       The interface function to configure dead charging timer.
 *  @details     This function is to configure dead battery charging timer.
 *  @param[in]   pSensorHandle 		        handle to the PMIC.
 *  @param[in]   epca9420_dead_chrg_timer   dead charging timer.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_Set_dead_chrg_timer() returns the status.
 */
int32_t PCA9420_Set_dead_chrg_timer(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_dead_chrg_timer epca9420_dead_chrg_timer);

/*! @brief       The interface function to configure dead battery charging current.
 *  @details     This function is to configure dead battery charging current for battery charging.
 *  @param[in]   pSensorHandle 		          handle to the PMIC.
 *  @param[in]   epca9420_dead_bat_chrg_cur   dead battery charging current.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_Set_dead_chrg_cur() returns the status.
 */
int32_t PCA9420_Set_dead_chrg_cur(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_dead_bat_chrg_cur epca9420_dead_bat_chrg_cur);

/*! @brief       The interface function to configure VBAT recharge threshold.
 *  @details     This function is to configure VBAT recharge threshold.
 *  @param[in]   pSensorHandle 		        handle to the PMIC.
 *  @param[in]   epca9420_threshld_rechrg   threshold recharge.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_Set_thrshld_rechrg() returns the status.
 */
int32_t PCA9420_Set_thrshld_rechrg(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_threshld_rechrg epca9420_threshld_rechrg);

/*! @brief       The interface function to configure battery regulation voltage.
 *  @details     This function is to configure battery regulation voltage.
 *  @param[in]   pSensorHandle 		    handle to the PMIC.
 *  @param[in]   epca9420_bat_reg_vol   battery regulation voltage.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_Set_bat_regulation_vol() returns the status.
 */
int32_t PCA9420_Set_bat_regulation_vol(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_bat_reg_vol epca9420_bat_reg_vol);

/*! @brief       The interface function to configure external thermistor resistance.
 *  @details     This function is to configure external thermistor resistance.
 *  @param[in]   pSensorHandle 		    handle to the PMIC.
 *  @param[in]   epca9420_ntc_res_sel   thermistor resistance.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_Set_thmstr_resistance() returns the status.
 */
int32_t PCA9420_Set_thmstr_resistance(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_ntc_res_sel epca9420_ntc_res_sel);

/*! @brief       The interface function to configure fast charging timer.
 *  @details     This function is to configure fast battery charging timer.
 *  @param[in]   pSensorHandle 		        handle to the PMIC.
 *  @param[in]   epca9420_fast_chrg_timer   fast charging timer.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_Set_fast_chrg_timer() returns the status.
 */
int32_t PCA9420_Set_fast_chrg_timer(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_fast_chrg_timer epca9420_fast_chrg_timer);

/*! @brief       The interface function to configure pre-qualification charging timer.
 *  @details     This function is to configure pre-qualification battery charging timer.
 *  @param[in]   pSensorHandle 		        handle to the PMIC.
 *  @param[in]   epca9420_preq_chrg_timer   pre-qualification charging timer.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_Set_preq_timer() returns the status.
 */
int32_t PCA9420_Set_preq_timer(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_preq_chrg_timer epca9420_preq_chrg_timer);

/*! @brief       The interface function to configure top-off charging timer.
 *  @details     This function is to configure top-off battery charging timer.
 *  @param[in]   pSensorHandle 		     handle to the PMIC.
 *  @param[in]   epca9420_topoff_timer   top-off charging timer.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_Set_topoff_timer() returns the status.
 */
int32_t PCA9420_Set_topoff_timer(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_topoff_timer epca9420_topoff_timer);

//Get battery configuration functions

/*! @brief       The interface function to get top-off charge current.
 *  @details     This function is to read the top-off charge current.
 *  @param[in]   pSensorHandle	handle to the PMIC.
 *  @param[in]   pBuffer      	handle to the output buffer.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_Get_battery_topoff_cur() returns the status.
 */
int32_t PCA9420_Get_battery_topoff_cur(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_bat_topoff_cur *pBuffer);

/*! @brief       The interface function to get low-battery charge current.
 *  @details     This function is to read the low-battery charge current.
 *  @param[in]   pSensorHandle	handle to the PMIC.
 *  @param[in]   pBuffer      	handle to the output buffer.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_Get_low_battery_chrg_cur() returns the status.
 */
int32_t PCA9420_Get_low_battery_chrg_cur(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_low_bat_chrg_cur *pBuffer);

/*! @brief       The interface function to get dead battery charge current.
 *  @details     This function is to read the dead battery charge current.
 *  @param[in]   pSensorHandle	handle to the PMIC.
 *  @param[in]   pBuffer      	handle to the output buffer.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_Get_dead_chrg_cur() returns the status.
 */
int32_t PCA9420_Get_dead_chrg_cur(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_dead_bat_chrg_cur *pBuffer);

/*! @brief       The interface function to read fast charging timer.
 *  @details     This function is to read fast battery charging timer.
 *  @param[in]   pSensorHandle  handle to the PMIC.
 *  @param[in]   pBuffer      	handle to the output buffer.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_Get_fast_chrg_timer() returns the status.
 */
int32_t PCA9420_Get_fast_chrg_timer(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_fast_chrg_timer *pBuffer);

/*! @brief       The interface function to read pre-qualification/low battery charging timer.
 *  @details     This function is to read pre-qualification/low battery charging timer.
 *  @param[in]   pSensorHandle  handle to the PMIC.
 *  @param[in]   pBuffer      	handle to the output buffer.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_Get_preq_timer() returns the status.
 */
int32_t PCA9420_Get_preq_timer(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_preq_chrg_timer *pBuffer);

/*! @brief       The interface function to read top-off battery charging timer.
 *  @details     This function is to read top-off battery charging timer.
 *  @param[in]   pSensorHandle  handle to the PMIC.
 *  @param[in]   pBuffer      	handle to the output buffer.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_Get_topoff_timer() returns the status.
 */
int32_t PCA9420_Get_topoff_timer(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_topoff_timer *pBuffer);

/*! @brief       The interface function to read external thermal resistance/current.
 *  @details     This function is to read external thermal resistance/current.
 *  @param[in]   pSensorHandle  handle to the PMIC.
 *  @param[in]   pBuffer      	handle to the output buffer.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_Get_thmstr_resistance() returns the status.
 */
int32_t PCA9420_Get_thmstr_resistance(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_ntc_res_sel *pBuffer);

/*! @brief       The interface function to read ntc beta value.
 *  @details     This function is to read the ntc beta value.
 *  @param[in]   pSensorHandle  handle to the PMIC.
 *  @param[in]   pBuffer      	handle to the output buffer.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_Get_ntc_beta_val() returns the status.
 */
int32_t PCA9420_Get_ntc_beta_val(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_ntc_beta_val *pBuffer);

/*! @brief       The interface function to read thermal regulation threshold.
 *  @details     This function is to read thermal regulation threshold.
 *  @param[in]   pSensorHandle  handle to the PMIC.
 *  @param[in]   pBuffer      	handle to the output buffer.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_Get_thrml_reg_thshld() returns the status.
 */
int32_t PCA9420_Get_thrml_reg_thshld(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_thrml_reg_thshld *pBuffer);

/*! @brief       The interface function to configure ntc beta value.
 *  @details     This function is to configure ntc beta value.
 *  @param[in]   pSensorHandle 					handle to the PMIC.
 *  @param[in]   epca9420_ntc_beta_val      	ntc beta value.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_Set_ntc_beta_val() returns the status.
 */
int32_t PCA9420_Set_ntc_beta_val(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_ntc_beta_val epca9420_ntc_beta_val);

/*! @brief       The interface function to configure thermal regulation threshold voltage.
 *  @details     This function is to configure thermal regulation threshold voltage.
 *  @param[in]   pSensorHandle 					handle to the PMIC.
 *  @param[in]   epca9420_thrml_reg_thshld      	top-off charging timer.
 *  @constraints This can be called any number of times only after PCA9420_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::PCA9420_Set_thrml_reg_thshld() returns the status.
 */
int32_t PCA9420_Set_thrml_reg_thshld(pca9420_i2c_sensorhandle_t *pSensorHandle, enum _pca9420_thrml_reg_thshld epca9420_thrml_reg_thshld);

#endif


#endif // PCA9420_UK_H_
