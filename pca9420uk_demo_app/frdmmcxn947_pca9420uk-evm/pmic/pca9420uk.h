/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/**
 * @file pca9420uk.h
 * @brief This file contains the PCA9420UK PMIC register definitions, access macros etc
 */

#ifndef PCA9420UK_H_
#define PCA9420UK_H_

/**
 **
 **  @brief The PCA9420UK PMIC Internal Register Map.
 */
enum
{
	// System control registers
	PCA9420UK_DEV_INFO =      0x00,
	PCA9420UK_TOP_INT = 	  0x01,
	PCA9420UK_SUB_INT0 = 	  0x02,
	PCA9420UK_SUB_INT0_MASK = 0x03,
	PCA9420UK_SUB_INT1 =      0x04,
	PCA9420UK_SUB_INT1_MASK = 0x05,
	PCA9420UK_SUB_INT2 =      0x06,
	PCA9420UK_SUB_INT2_MASK = 0x07,

	PCA9420UK_TOP_CNTL0 = 0x09,
	PCA9420UK_TOP_CNTL1 = 0x0A,
	PCA9420UK_TOP_CNTL2 = 0x0B,
	PCA9420UK_TOP_CNTL3 = 0x0C,
	PCA9420UK_TOP_CNTL4 = 0x0D,

	// Battery charger control registers
	PCA9420UK_CHG_CNTL0 = 0x10,
	PCA9420UK_CHG_CNTL1 = 0x11,
	PCA9420UK_CHG_CNTL2 = 0x12,
	PCA9420UK_CHG_CNTL3 = 0x13,
	PCA9420UK_CHG_CNTL4 = 0x14,
	PCA9420UK_CHG_CNTL5 = 0x15,
	PCA9420UK_CHG_CNTL6 = 0x16,
	PCA9420UK_CHG_CNTL7 = 0x17,

	PCA9420UK_CHG_STATUS0 = 0x18,
	PCA9420UK_CHG_STATUS1 = 0x19,
	PCA9420UK_CHG_STATUS2 = 0x1A,
	PCA9420UK_CHG_STATUS3 = 0x1B,

	// Regulator control registers
	PCA9420UK_REG_STATUS =     0x20,
	PCA9420UK_ACT_DIS_CNTL_1 = 0x21,

	PCA9420UK_MODECFG_0_0 = 0x22,
	PCA9420UK_MODECFG_0_1 = 0x23,
	PCA9420UK_MODECFG_0_2 = 0x24,
	PCA9420UK_MODECFG_0_3 = 0x25,

	PCA9420UK_MODECFG_1_0 = 0x26,
	PCA9420UK_MODECFG_1_1 = 0x27,
	PCA9420UK_MODECFG_1_2 = 0x28,
	PCA9420UK_MODECFG_1_3 = 0x29,

	PCA9420UK_MODECFG_2_0 = 0x2A,
	PCA9420UK_MODECFG_2_1 = 0x2B,
	PCA9420UK_MODECFG_2_2 = 0x2C,
	PCA9420UK_MODECFG_2_3 = 0x2D,

	PCA9420UK_MODECFG_3_0 = 0x2E,
	PCA9420UK_MODECFG_3_1 = 0x2F,
	PCA9420UK_MODECFG_3_2 = 0x30,
	PCA9420UK_MODECFG_3_3 = 0x31,

	PCA9420UK_RESET_MONITOR = 0x71,
};

#define PCA9420UK_REG_SIZE_BYTES	1

#define PCA9420UK_DEVICE_ID_MASK   0xF8
#define PCA9420UK_DEVICE_ID_SHIFT  0x03
#define PCA9420UK_DEVICE_REV_MASK  0x07
#define PCA9420UK_DEVICE_REV_SHIFT 0x00

#define PCA9420UK_CHG_LOCK_MASK    0xA8
#define PCA9420UK_CHG_UNLOCK_MASK  0x00
#define PCA9420UK_CHG_LOCK_SHIFT   0x03

#define PCA9420_SW1_VOUT_MASK      0x80
#define PCA9420_SW2_VOUT_MASK  	   0x40
#define PCA9420_LDO1_VOUT_MASK 	   0x20
#define PCA9420_LDO2_VOUT_MASK     0x10

//charger status
#define PCA9420_CHRGR_IDLE_MASK		      0X00
#define PCA9420_CHRGR_DEAD_BAT_MASK       0X01
#define PCA9420_CHRGR_LOW_BAT_MASK        0X02
#define PCA9420_CHRGR_FAST_CHRG_MASK1     0X03
#define PCA9420_CHRGR_FAST_CHRG_MASK2     0X04
#define PCA9420_CHRGR_TOPOFF_MASK         0X05
#define PCA9420_CHRGR_DONE_MASK           0X06

//Battery status
#define PCA9420_BAT_MISS_MASK		  0X00
#define PCA9420_BAT_DETECT_MASK 	  0X01
#define PCA9420_BAT_DISCHRG_MASK      0X00
#define PCA9420_BAT_MISS_MASK 		  0X00
#define PCA9420_BAT_MISS_MASK		  0X00
#define PCA9420_BAT_MISS_MASK 		  0X00

//Battery temperature status
#define PCA9420_BAT_TEMP_NOMINAL_MASK     0X00
#define PCA9420_BAT_TEMP_COLD_MASK        0X01
#define PCA9420_BAT_TEMP_COOL_MASK        0X02
#define PCA9420_BAT_TEMP_WARM_MASK        0X03
#define PCA9420_BAT_TEMP_HOT_MASK         0X04

//Chip temperature status
#define PCA9420_CHIP_TEMP_MASK  	      0X04

//Safety Timer status
#define PCA9420_SAFE_TIMER_MASK	              0X00
#define PCA9420_PREQUAL_TIMER_MASK            0X01
#define PCA9420_FAST_TIMER_MASK               0X02
#define PCA9420_BAT_SHT_TEST_TIMER_MASK       0X03

#define PCA9420_TOP_CNTL3_MODE_I2C_MASK      (0x18)
#define PCA9420_TOP_CNTL3_MODE_I2C_SHIFT     (0x03)

#define PCA9420_MODECFG_0_SHIP_EN_MASK       (0x80)
#define PCA9420_MODECFG_0_MODE_CTRL_SEL_MASK (0x40)
#define PCA9420_MODECFG_0_SW1_OUT_MASK       (0x3F)
#define PCA9420_MODECFG_1_ON_CFG_MASK        (0x40)
#define PCA9420_MODECFG_1_SW2_OUT_MASK       (0x3F)
#define PCA9420_MODECFG_2_LDO1_OUT_MASK      (0xF0)
#define PCA9420_MODECFG_2_LDO1_OUT_SHIFT     (0x04)
#define PCA9420_MODECFG_3_LDO2_OUT_MASK      (0x3F)

#define PCA9420_WTCHDG_TIMER_RESET           (0x01)
#define PCA9420_CLR_INT_MASK                 (0xFF)
#define PCA9420_TOP_CNTL3_ON_GLT_LONG_SHIFT  (0X00)
#define PCA9420_TOP_CNTL3_ON_GLT_LONG_MASK   (0X03)

//Charging status
#define PCA9420_IN_PWR_STATUS_SHIFT       (0X06)
#define PCA9420_IN_PWR_STATUS_MASK        (0xC0)
#define PCA9420_BAT_DETAIL_STATUS_SHIFT   (0X04)
#define PCA9420_BAT_DETAIL_STATUS_MASK    (0X70)
#define PCA9420_BAT_CHG_STATUS_SHIFT      (0X00)
#define PCA9420_BAT_CHG_STATUS_MASK       (0X07)
#define PCA9420_TEMP_STATUS_SHIFT         (0X03)
#define PCA9420_TEMP_STATUS_MASK          (0X70)
#define PCA9420_SFTY_TIMER_SHIFT          (0x00)
#define PCA9420_SFTY_TIMER_MASK           (0x03)

#define PCA9420_TOP_CNTL3_SW_RESET_MASK   (0X05)
#define PCA9420_MODE_CNTL_SEL_MASK        (0X18)
#define PCA9420_MODE_CNTL_SEL_SHIFT       (0X03)
#define PCA9420_MODE_DIE_TEMP_MASK        (0X03)
#define PCA9420_MODE_DIE_TEMP_SHIFT       (0X00)
#define PCA9420_MODE_THML_STDN_MASK       (0X1C)
#define PCA9420_MODE_THML_STDN_SHIFT      (0X02)

//PMIC STATUS
#define PCA9420_SW2_VOL_MASK 		  (0X1F)
#define PCA9420_SW2_VOL_OFF_MASK	  (0X3F)

#define PCA9420_SW2_VOL_OFFSET_MASK	  (0X20)
#define PCA9420_SW2_VOL_OFFSET_SHIFT  (0X05)
#define PCA9420_LDO1_VOL_MASK  		  (0Xf0)
#define PCA9420_LDO1_VOL_SHIFT 		  (0X04)
#define PCA9420_LDO2_VOL_MASK 		  (0x1F)

#define PCA9420_LDO2_VOL_OFF_MASK 	  (0x3F)
#define PCA9420_LDO2_VOL_OFFSET_MASK  (0X20)
#define PCA9420_LDO2_VOL_OFFSET_SHIFT (0X05)

//Top Level Interrupt Status
#define PCA9420_SYS_INT_MASK		  (0X08)
#define PCA9420_SYS_INT_SHIFT		  (0X03)
#define PCA9420_BAT_INT_MASK 		  (0X04)
#define PCA9420_BAT_INT_SHIFT 		  (0X02)
#define PCA9420_BUCK_INT_MASK 		  (0X02)
#define PCA9420_BUCK_INT_SHIFT 		  (0X01)
#define PCA9420_LDO_INT_MASK		  (0X01)
#define PCA9420_LDO_INT_SHIFT 		  (0x00)

//Sub Level Interrupt Status
#define PCA9420_TEMP_PREWARN_MASK 	  (0x10)
#define PCA9420_TEMP_PREWARN_SHIFT	  (0x04)
#define PCA9420_THEM_SHDN_MASK        (0x08)
#define PCA9420_THEM_SHDN_SHIFT 	  (0x03)
#define PCA9420_ASYS_PREWARN_MASK 	  (0x04)
#define PCA9420_ASYS_PREWARN_SHIFT 	  (0x02)
#define PCA9420_WD_TMR_MASK 		  (0x02)
#define PCA9420_WD_TMR_SHIFT	      (0x01)
#define PCA9420_IN_PWR_MASK           (0x01)
#define PCA9420_IN_PWR_SHIFT          (0x00)

#define PCA9420_DEAD_TMR_MASK         (0x40)
#define PCA9420_DEAD_TMR_SHIFT		  (0x06)
#define PCA9420_VIN_ILIM_MASK  		  (0x20)
#define PCA9420_VIN_ILIM_SHIFT 		  (0x05)
#define PCA9420_FAST_TMR_MASK  		  (0x10)
#define PCA9420_FAST_TMR_SHIFT		  (0x04)
#define PCA9420_PREQ_TMR_MASK  		  (0x08)
#define PCA9420_PREQ_TMR_SHIFT 	      (0x03)
#define PCA9420_VBAT_DET_MASK  		  (0x04)
#define PCA9420_VBAT_DET_SHIFT        (0x02)
#define PCA9420_VBAT_OK_MASK  		  (0x02)
#define PCA9420_VBAT_OK_SHIFT 		  (0x01)
#define PCA9420_CHG_OK_MASK           (0x01)
#define PCA9420_CHG_OK_SHIFT 		  (0x00)

#define PCA9420_VOUTSW1_MASK 		  (0x08)
#define PCA9420_VOUTSW1_SHIFT		  (0x03)
#define PCA9420_VOUTSW2_MASK		  (0x04)
#define PCA9420_VOUTSW2_SHIFT 		  (0x02)
#define PCA9420_VOUTLDO1_MASK 		  (0x02)
#define PCA9420_VOUTLDO1_SHIFT		  (0x01)
#define PCA9420_VOUTLDO2_MASK 		  (0x01)
#define PCA9420_VOUTLDO2_SHIFT 		  (0x00)

#define PCA9420_MODE_ASYS_UVLO_MASK       (0XC0)
#define PCA9420_MODE_ASYS_UVLO_SHIFT      (0X06)

#define PCA9420_MODE_VIN_UVLO_MASK        (0X03)
#define PCA9420_MODE_VIN_UVLO_SHIFT       (0X00)

#define PCA9420_MODE_VIN_OVP_MASK         (0X04)
#define PCA9420_MODE_VIN_OVP_SHIFT        (0X02)

#define PCA9420_MODE_ASYS_INP_SEL_MASK    (0X30)
#define PCA9420_MODE_ASYS_INP_SEL_SHIFT   (0X04)

#define PCA9420_MODE_ASYS_PRE_VOL_MASK    (0XC0)
#define PCA9420_MODE_ASYS_PRE_VOL_SHIFT   (0X06)

#define PCA9420_MODE_VIN_ILIM_SEL_MASK    (0XE0)
#define PCA9420_MODE_VIN_ILIM_SEL_SHIFT   (0X05)


// battery control mask
#define PCA9420_NTC_EN_MASK 		  (0X04)
#define PCA9420_NTC_EN_SHIFT		  (0X02)

#define PCA9420_CHG_TIMER_EN_MASK 	  (0X02)
#define PCA9420_CHG_TIMER_EN_SHIFT    (0X01)

#define PCA9420_CHG_EN_MASK 		  (0X01)
#define PCA9420_CHG_EN_SHIFT		  (0X00)

#define PCA9420_MODE_ICHG_CC_MASK     	  (0X3F)
#define PCA9420_MODE_ICHG_CC_SHIFT    	  (0X00)

#define PCA9420_MODE_ICHG_TOPOFF_MASK     (0X3F)
#define PCA9420_MODE_ICHG_TOPOFF_SHIFT    (0X00)

#define PCA9420_MODE_ICHG_LOW_MASK        (0X3F)
#define PCA9420_MODE_ICHG_LOW_SHIFT       (0X00)

#define PCA9420_MODE_ICHG_DAED_TIMER_MASK  (0XC0)
#define PCA9420_MODE_ICHG_DEAD_TIMER_SHIFT (0X06)

#define PCA9420_MODE_ICHG_DAED_MASK        (0X3F)
#define PCA9420_MODE_ICHG_DEAD_SHIFT       (0X00)

#define PCA9420_VBAT_RESTART_MASK          (0X40)
#define PCA9420_VBAT_RESTART_SHIFT         (0X06)

#define PCA9420_VBAT_REG_MASK      	   (0X3F)
#define PCA9420_VBAT_REG_SHIFT     	   (0X00)

#define PCA9420_NTC_RES_SEL_MASK      	   (0X80)
#define PCA9420_NTC_RES_SEL_SHIFT     	   (0X07)

#define PCA9420_ICHG_FAST_TIMER_MASK 	   (0X30)
#define PCA9420_ICHG_FAST_TIMER_SHIFT 	   (0X04)

#define PCA9420_ICHG_PREQ_TIMER_MASK 	   (0X0C)
#define PCA9420_ICHG_PREQ_TIMER_SHIFT      (0X02)

#define PCA9420_T_TOPOFF_TIMER_MASK 	   (0X03)
#define PCA9420_T_TOPOFF_TIMER_SHIFT       (0X00)

#define PCA9420_NTC_BETA_MASK  		   (0XE0)
#define PCA9420_NTC_BETA_SHIFT 		   (0X04)

#define PCA9420_THM_REG_MASK  		   (0X07)
#define PCA9420_THM_REG_SHIFT          (0X00)

#define PCA9420_SW1_BLEED_SHIFT 	   (0x03)
#define PCA9420_SW1_BLEED_MASK 		   (0x08)
#define PCA9420_SW2_BLEED_SHIFT		   (0x02)
#define PCA9420_SW2_BLEED_MASK 		   (0x04)
#define PCA9420_LDO1_BLEED_SHIFT	   (0x01)
#define PCA9420_LDO1_BLEED_MASK 	   (0x02)
#define PCA9420_LDO2_BLEED_SHIFT	   (0x00)
#define PCA9420_LDO2_BLEED_MASK 	   (0x01)

#define PCA9420_SHIP_MODE_MASK 		   (0x80)
#define PCA9420_SHIP_MODE_SHIFT 	   (0X07)
#define PCA9420_MODE_CTRL_MASK 		   (0x40)
#define PCA9420_MODE_CTRL_SHIFT 	   (0X06)

#define PCA9420_SW1_VOL_MASK 		   (0X3F)
#define PCA9420_SW1_VOL_SHIFT 		   (0X00)
#define PCA9420_SW2_VOL_SHIFT          (0x00)
#define PCA9420_LDO1_VOL_SHIFT		   (0X04)
#define PCA9420_LDO2_VOL_SHIFT 		   (0X00)

#define PCA9420_MODE_WD_TIMER_MASK 	   (0xC0)
#define PCA9420_MODE_WD_TIMER_SHIFT    (0x06)

#define PCA9420_SW1_EN_SHIFT 		   (0x03)
#define PCA9420_SW1_EN_MASK 		   (0x08)
#define PCA9420_SW2_EN_SHIFT 		   (0x02)
#define PCA9420_SW2_EN_MASK 	   	   (0x04)
#define PCA9420_LDO1_EN_SHIFT 		   (0x01)
#define PCA9420_LDO1_EN_MASK 		   (0x02)
#define PCA9420_LDO2_EN_SHIFT  		   (0x00)
#define PCA9420_LDO2_EN_MASK 		   (0x01)

#define PCA9420_MODE_ON_CFG_MASK	   (0x40)
#define PCA9420_MODE_ON_CFG_SHIFT 	   (0x06)
#define PCA9420UK_EN_CHG_IN_WTCH_MASK  (0X40)
#define PCA9420UK_EN_CHG_IN_WTCH_SHIFT (0X02)

/*! @brief PCA9420 mode definition. */
enum LongGlitchTimer
{
   PCA9420_GLTH_Timer_4 = 0,
   PCA9420_GLTH_Timer_8 = 1,
   PCA9420_GLTH_Timer_12 = 2,
   PCA9420_GLTH_Timer_16 = 3,
};

typedef enum _pca9420_mode
{
	kPCA9420_Mode0 = 0x00U, /* Mode 0. */
	kPCA9420_Mode1 = 0x01U, /* Mode 1. */
	kPCA9420_Mode2 = 0x02U, /* Mode 2. */
	kPCA9420_Mode3 = 0x03U, /* Mode 3. */
} pca9420_mode_t;

/*! @brief PCA9420 Regulator definition. */
typedef enum _pca9420_regulator
{
	kPCA9420_RegulatorSwitch1 = 0x0U, /* Switch1 regulator. */
	kPCA9420_RegulatorSwitch2 = 0x03U, /* Switch2 regulator. */
	kPCA9420_RegulatorLdo1    = 0x02U, /* LDO1 regulator. */
	kPCA9420_RegulatorLdo2    = 0x01U, /* LDO2 regulator. */
} pca9420_regulator_t;

/*! @brief PCA9420 Interrupt Source definition. */
enum _pca9420_interrupt_source
{
	/* Sub_INT0 */
	kPCA9420_IntSrcSysVinOKChanged    = 0x1U,  /* VIN_OK changed interrupt source. */
	kPCA9420_IntSrcSysWdogTimeout     = 0x2U,  /* Watchdog timeout interrupt source. */
	kPCA9420_IntSrcSysAsysPreWarn     = 0x4U,  /* VSYS voltage falls below the VSYS_PRE_WRN interrupt source. */
	kPCA9420_IntSrcSysThermalShutdown = 0x8U,  /* Thermal shutdown (Die temp >= THM_STDN) interrupt source. */
	kPCA9420_IntSrcSysTempWarn        = 0x10U, /* Die temp pre-warning (Die temp >= T_WRN) interrupt source. */
	kPCA9420_IntSrcSysAll             = 0x1FU, /* All system interrupt sources. */

	/* Sub_INT1 */
	kPCA9420_IntSrcChgChgOk           = 0x100U,  /* CHG_OK changed interrupt source. */
	kPCA9420_IntSrcChgVbatOk          = 0x200U,  /* VBAT_OK changed interrupt source. */
	kPCA9420_IntSrcChgVbatDetOk       = 0x400U,  /* VBAT_DET_OK changed interrupt source. */
	kPCA9420_IntSrcChgPreChgTimeout   = 0x800U,  /* Pre-qualification charge timeout interrupt source. */
	kPCA9420_IntSrcChgFastChgTimeout  = 0x1000U, /* Fast charge timeout interrupt source. */
	kPCA9420_IntSrcChgInputCurrentLmt = 0x2000U, /* Input current limit interrupt source. */
	kPCA9420_IntSrcChgAll             = 0x3F00U, /* All charger interrupt sources. */

	/* Sub_INT2 */
	kPCA9420_IntSrcLdo2VoutOk   = 0x10000U, /* VOUTLDO2_OK changed interrupt source. */
	kPCA9420_IntSrcLdo1VoutOk   = 0x20000U, /* VOUTLDO1_OK changed interrupt source. */
	kPCA9420_IntSrcSw2VoutOk    = 0x40000U, /* VOUTSW2_OK changed interrupt source. */
	kPCA9420_IntSrcSw1VoutOk    = 0x80000U, /* VOUTSW1_OK changed interrupt source. */
	kPCA9420_IntSrcRegulatorAll = 0xF0000U, /* All regulator interrupt sources. */
};

/*! @brief PCA9420 VIN Current Limit Selection definition. */
enum _pca9420_vin_ilim
{
	kPCA9420_VinIlim_74_85_98      = 0x00U, /* min: 74mA, typ: 85mA, max: 98mA */
	kPCA9420_VinIlim_370_425_489   = 0x01U, /* min: 370mA, typ: 425mA, max: 489mA */
};

/*! @brief PCA9420 Ship Wakeup config definition. */
typedef enum _pca9420_ship_wkup_cfg
{
	kPCA9420_ShipWkup_PowerUp = 0x00U,   /* Upon VIN plug in, the chip will enable the battery charging process, and
                                              start the power-up sequence for LDO1/LDO2/SW1/SW2 per the setting */
	kPCA9420_ShipWkup_NoPowerUp = 0x10U, /* Upon VIN plug in, the chip will enable the charging process,
                                              LDO1/LDO2/SW1/SW2 remain in shutdown mode and the chip will only enable
                                              the power-up sequence upon ON pin falling edge */
} pca9420_ship_wkup_cfg_t;

/*! @brief PCA9420 Power Down Sequence Enable definition. */
typedef enum _pca9420_pwr_dn_en
{
	kPCA9420_PwrDnDisabled = 0x00U, /* Do not start power-down sequence */
	kPCA9420_PwrDnEnabled  = 0x08U, /* Start power-down sequence */
} pca9420_pwr_dn_en_t;

/*! @brief PCA9420 Ship Wakeup config definition. */
typedef enum _pca9420_chg_in_watchdog
{
	kPCA9420_ChgInWatchdogChargerContinue = 0x00U, /* When the watchdog timer expires, charger continue its operation */
	kPCA9420_ChgInWatchdogChargerDisabled = 0x04U, /* When the watchdog timer expires, charger is disabled */
} pca9420_chg_in_watchdog_t;

/*! @brief PCA9420 Power Good Comparator Enable definition. */
typedef enum _pca9420_pgood_en
{
	kPCA9420_PGoodDisabled = 0x00U, /* Output voltage power-good comparators are disabled. */
	kPCA9420_PGoodEnabled  = 0x01U, /* Output voltage power-good comparators are enabled. */
} pca9420_pgood_en_t;

/*! @brief PCA9420 VSYS Pre Warning Voltage Threshold definition. */
typedef enum _pca9420_asys_prewarning
{
	kPCA9420_AsysPreWarn3V3 = 0x00U, /* VSYS pre-warning voltage threshold 3.3V */
	kPCA9420_AsysPreWarn3V4 = 0x01U, /* VSYS pre-warning voltage threshold 3.4V */
	kPCA9420_AsysPreWarn3V5 = 0x02U, /* VSYS pre-warning voltage threshold 3.5V */
	kPCA9420_AsysPreWarn3V6 = 0x03U, /* VSYS pre-warning voltage threshold 3.6V */
} pca9420_asys_prewarning_t;

/*! @brief PCA9420 VSYS Input Source Selection definition. */
typedef enum _pca9420_asys_input_sel
{
	kPCA9420_AsysInputSelVbatVin =
			0x00U, /* VSYS is powered by either VBAT or VIN, VIN has higher priority if both are presented. */
			kPCA9420_AsysInputSelVbat = 0x01U, /* VSYS is powered by VBAT only. */
			kPCA9420_AsysInputSelVin  = 0x02U, /* VSYS is powered by VIN only. */
			kPCA9420_AsysInputSelNone = 0x03U, /* VSYS is disconnected to either VBAT or VIN (test purpose only). */
} pca9420_asys_input_sel_t;

/*! @brief PCA9420 VIN Over-voltage Protection Threshold Selection definition. */
typedef enum _pca9420_vin_ovp_sel
{
	kPCA9420_VinOvpSel5V5 = 0x00U, /* VIN Over-Voltage Protection threshold 5.5V. */
	kPCA9420_VinOvpSel6V0 = 0x01U, /* VIN Over-Voltage Protection threshold 6.0V. */
} pca9420_vin_ovp_sel_t;

/*! @brief PCA9420 VIN Under-voltage Lockout Threshold Selection definition. */
typedef enum _pca9420_vin_uvlo_sel
{
	kPCA9420_VinUvloSel2V9 = 0x0U, /* VIN Under-Voltage Lockout threshold 2.9V. */
	kPCA9420_VinUvloSel3V1 = 0x1U, /* VIN Under-Voltage Lockout threshold 3.1V. */
	kPCA9420_VinUvloSel3V3 = 0x2U, /* VIN Under-Voltage Lockout threshold 3.3V. */
	kPCA9420_VinUvloSel3V5 = 0x3U, /* VIN Under-Voltage Lockout threshold 3.5V. */
} pca9420_vin_uvlo_sel_t;

/*! @brief PCA9420 VSYS Under-voltage Lockout Threshold Selection definition. */
typedef enum _pca9420_asys_uvlo_sel
{
	kPCA9420_AsysUvloSel2V4 = 0x00U, /* VSYS Under-Voltage Lockout threshold 2.4V. */
	kPCA9420_AsysUvloSel2V5 = 0x01U, /* VSYS Under-Voltage Lockout threshold 2.5V. */
	kPCA9420_AsysUvloSel2V6 = 0x02U, /* VSYS Under-Voltage Lockout threshold 2.6V. */
	kPCA9420_AsysUvloSel2V7 = 0x03U, /* VSYS Under-Voltage Lockout threshold 2.7V. */
} pca9420_asys_uvlo_sel_t;

/*! @brief PCA9420 Disable Charge Termination definition. */
typedef enum _pca9420_charge_term_dis
{
	kPCA9420_ChargeTermEnabled  = 0x00U, /* Enable the charge termination control. */
	kPCA9420_ChargeTermDisabled = 0x20U, /* Disable the charge termination control, fast charge timer is reset. */
} pca9420_charge_term_dis_t;

/*! @brief PCA9420 Thermal Shutdown Temperature Threshold definition. */
typedef enum _pca9420_them_shdn
{
	kPCA9420_ThemShdn95C  = 0x00U, /* Thermal shutdown temperature threshold 95C. */
	kPCA9420_ThemShdn100C = 0x01U, /* Thermal shutdown temperature threshold 100C. */
	kPCA9420_ThemShdn105C = 0x02U, /* Thermal shutdown temperature threshold 105C. */
	kPCA9420_ThemShdn110C = 0x03U, /* Thermal shutdown temperature threshold 110C. */
	kPCA9420_ThemShdn115C = 0x04U, /* Thermal shutdown temperature threshold 115C. */
	kPCA9420_ThemShdn120C = 0x05U, /* Thermal shutdown temperature threshold 120C. */
	kPCA9420_ThemShdn125C = 0x06U, /* Thermal shutdown temperature threshold 125C. */
} pca9420_them_shdn_t;

/*! @brief PCA9420 Die Temperature Warning Threshold definition. */
typedef enum _pca9420_die_temp_warning
{
	kPCA9420_DieTempWarn75C = 0x00U, /* Die temperature warning threshold 75C. */
	kPCA9420_DieTempWarn80C = 0x01U, /* Die temperature warning threshold 80C. */
	kPCA9420_DieTempWarn85C = 0x02U, /* Die temperature warning threshold 85C. */
	kPCA9420_DieTempWarn90C = 0x03U, /* Die temperature warning threshold 90C. */
} pca9420_die_temp_warning_t;

/*! @brief PCA9420 Mode I2C definition. */
typedef enum _pca9420_mode_i2c
{
	kPCA9420_ModeI2cMode0 = 0x00U, /* PMIC mode set to mode 0, in case MODE_CTRL_SEL == 0. */
	kPCA9420_ModeI2cMode1 = 0x08U, /* PMIC mode set to mode 1, in case MODE_CTRL_SEL == 0. */
	kPCA9420_ModeI2cMode2 = 0x10U, /* PMIC mode set to mode 2, in case MODE_CTRL_SEL == 0. */
	kPCA9420_ModeI2cMode3 = 0x18U, /* PMIC mode set to mode 3, in case MODE_CTRL_SEL == 0. */
} pca9420_mode_i2c_t;

/*! @brief PCA9420 ON Pin Long Glitch Timer definition. */
typedef enum _pca9420_on_glt_long
{
	kPCA9420_OnGltLong4s  = 0x00U, /* ON pin long glitch timer set to 4 sec. */
	kPCA9420_OnGltLong8s  = 0x01U, /* ON pin long glitch timer set to 8 sec. */
	kPCA9420_OnGltLong12s = 0x02U, /* ON pin long glitch timer set to 12 sec. */
	kPCA9420_OnGltLong16s = 0x03U, /* ON pin long glitch timer set to 16 sec. */
} pca9420_on_glt_long_t;

/* TODO: charger function */

/*! @brief PCA9420 Regulator Status definition. */
typedef enum _pca9420_reg_status
{
	kPCA9420_RegStatusVoutSw1OK  = 0x80U, /* SW1 VOUT Power good status */
	kPCA9420_RegStatusVoutSw2OK  = 0x40U, /* SW2 VOUT Power good status */
	kPCA9420_RegStatusVoutLdo1OK = 0x20U, /* LDO1 VOUT Power good status */
	kPCA9420_RegStatusVoutLdo2OK = 0x10U, /* LDO2 VOUT Power good status */
} pca9420_reg_status_t;

/*! @brief PCA9420 Regulator Control definition. */
typedef enum _pca9420_reg_ctl
{
	kPCA9420_RegCtlSw1Bleed  = 0x08U, /* SW1 output active discharge control (0: enable, 1: disable). */
	kPCA9420_RegCtlSw2Bleed  = 0x04U, /* SW2 output active discharge control (0: enable, 1: disable). */
	kPCA9420_RegCtlLdo1Bleed = 0x02U, /* LDO1 output active discharge control (0: enable, 1: disable). */
	kPCA9420_RegCtlLdo2Bleed = 0x01U, /* LDO2 output active discharge control (0: enable, 1: disable). */
} pca9420_reg_ctl_t;

/*! @brief PCA9420 Ship Mode Enablement definition. */
typedef enum _pca9420_ship_en
{
	kPCA9420_ShipModeDisabled = 0x00U, /* Ship mode disabled. */
	kPCA9420_ShipModeEnabled  = 0x80U, /* Ship mode enabled, chip enters state with lowest quiescent consumption. */
} pca9420_ship_en_t;

/*! @brief PCA9420 Mode Control Selection definition. */
typedef enum _pca9420_mode_sel
{
	kPCA9420_ModeSelI2C = 0x00U, /* Mode control select by I2C register bits. */
	kPCA9420_ModeSelPin = 0x40U, /* Mode control select by external pins. */
} pca9420_mode_sel_t;

/*! @brief PCA9420 SW1 Output Voltage definition. */
typedef enum _pca9420_sw1_out
{
	kPCA9420_Sw1OutVolt0V500 = 0x00U, /* SW1 output voltage 0.500V. */
	kPCA9420_Sw1OutVolt0V525 = 0x01U, /* SW1 output voltage 0.525V. */
	kPCA9420_Sw1OutVolt0V550 = 0x02U, /* SW1 output voltage 0.550V. */
	kPCA9420_Sw1OutVolt0V575 = 0x03U, /* SW1 output voltage 0.575V. */
	kPCA9420_Sw1OutVolt0V600 = 0x04U, /* SW1 output voltage 0.600V. */
	kPCA9420_Sw1OutVolt0V625 = 0x05U, /* SW1 output voltage 0.625V. */
	kPCA9420_Sw1OutVolt0V650 = 0x06U, /* SW1 output voltage 0.650V. */
	kPCA9420_Sw1OutVolt0V675 = 0x07U, /* SW1 output voltage 0.675V. */
	kPCA9420_Sw1OutVolt0V700 = 0x08U, /* SW1 output voltage 0.700V. */
	kPCA9420_Sw1OutVolt0V725 = 0x09U, /* SW1 output voltage 0.725V. */
	kPCA9420_Sw1OutVolt0V750 = 0x0AU, /* SW1 output voltage 0.750V. */
	kPCA9420_Sw1OutVolt0V775 = 0x0BU, /* SW1 output voltage 0.775V. */
	kPCA9420_Sw1OutVolt0V800 = 0x0CU, /* SW1 output voltage 0.800V. */
	kPCA9420_Sw1OutVolt0V825 = 0x0DU, /* SW1 output voltage 0.825V. */
	kPCA9420_Sw1OutVolt0V850 = 0x0EU, /* SW1 output voltage 0.850V. */
	kPCA9420_Sw1OutVolt0V875 = 0x0FU, /* SW1 output voltage 0.875V. */
	kPCA9420_Sw1OutVolt0V900 = 0x10U, /* SW1 output voltage 0.900V. */
	kPCA9420_Sw1OutVolt0V925 = 0x11U, /* SW1 output voltage 0.925V. */
	kPCA9420_Sw1OutVolt0V950 = 0x12U, /* SW1 output voltage 0.950V. */
	kPCA9420_Sw1OutVolt0V975 = 0x13U, /* SW1 output voltage 0.975V. */
	kPCA9420_Sw1OutVolt1V000 = 0x14U, /* SW1 output voltage 1.000V. */
	kPCA9420_Sw1OutVolt1V025 = 0x15U, /* SW1 output voltage 1.025V. */
	kPCA9420_Sw1OutVolt1V050 = 0x16U, /* SW1 output voltage 1.050V. */
	kPCA9420_Sw1OutVolt1V075 = 0x17U, /* SW1 output voltage 1.075V. */
	kPCA9420_Sw1OutVolt1V100 = 0x18U, /* SW1 output voltage 1.100V. */
	kPCA9420_Sw1OutVolt1V125 = 0x19U, /* SW1 output voltage 1.125V. */
	kPCA9420_Sw1OutVolt1V150 = 0x1AU, /* SW1 output voltage 1.150V. */
	kPCA9420_Sw1OutVolt1V175 = 0x1BU, /* SW1 output voltage 1.175V. */
	kPCA9420_Sw1OutVolt1V200 = 0x1CU, /* SW1 output voltage 1.200V. */
	kPCA9420_Sw1OutVolt1V225 = 0x1DU, /* SW1 output voltage 1.225V. */
	kPCA9420_Sw1OutVolt1V250 = 0x1EU, /* SW1 output voltage 1.250V. */
	kPCA9420_Sw1OutVolt1V275 = 0x1FU, /* SW1 output voltage 1.275V. */
	kPCA9420_Sw1OutVolt1V300 = 0x20U, /* SW1 output voltage 1.300V. */
	kPCA9420_Sw1OutVolt1V325 = 0x21U, /* SW1 output voltage 1.325V. */
	kPCA9420_Sw1OutVolt1V350 = 0x22U, /* SW1 output voltage 1.350V. */
	kPCA9420_Sw1OutVolt1V375 = 0x23U, /* SW1 output voltage 1.375V. */
	kPCA9420_Sw1OutVolt1V400 = 0x24U, /* SW1 output voltage 1.400V. */
	kPCA9420_Sw1OutVolt1V425 = 0x25U, /* SW1 output voltage 1.425V. */
	kPCA9420_Sw1OutVolt1V450 = 0x26U, /* SW1 output voltage 1.450V. */
	kPCA9420_Sw1OutVolt1V475 = 0x27U, /* SW1 output voltage 1.475V. */
	kPCA9420_Sw1OutVolt1V500 = 0x28U, /* SW1 output voltage 1.500V. */
	kPCA9420_Sw1OutVolt1V800 = 0x3FU, /* SW1 output voltage 1.800V. */
} pca9420_sw1_out_t;

/*! @brief PCA9420 ON Key Config definition for mode switch. */
typedef enum _pca9420_on_cfg
{
	kPCA9420_OnCfgEnableModeSwitch =
			0x00U, /* Upon valid falling edge applied on ON pin, the device will switch back to mode 0. */
			kPCA9420_OnCfgDisableModeSwitch = 0x40U, /* Upon valid falling edge applied on ON pin, no mode switch. */
} pca9420_on_cfg_t;

/*! @brief PCA9420 SW2 Output Voltage definition. */
typedef enum _pca9420_sw2_out
{
	kPCA9420_Sw2OutVolt1V500 = 0x00U, /* SW2 output voltage 1.500V. */
	kPCA9420_Sw2OutVolt1V525 = 0x01U, /* SW2 output voltage 1.525V. */
	kPCA9420_Sw2OutVolt1V550 = 0x02U, /* SW2 output voltage 1.550V. */
	kPCA9420_Sw2OutVolt1V575 = 0x03U, /* SW2 output voltage 1.575V. */
	kPCA9420_Sw2OutVolt1V600 = 0x04U, /* SW2 output voltage 1.600V. */
	kPCA9420_Sw2OutVolt1V625 = 0x05U, /* SW2 output voltage 1.625V. */
	kPCA9420_Sw2OutVolt1V650 = 0x06U, /* SW2 output voltage 1.650V. */
	kPCA9420_Sw2OutVolt1V675 = 0x07U, /* SW2 output voltage 1.675V. */
	kPCA9420_Sw2OutVolt1V700 = 0x08U, /* SW2 output voltage 1.700V. */
	kPCA9420_Sw2OutVolt1V725 = 0x09U, /* SW2 output voltage 1.725V. */
	kPCA9420_Sw2OutVolt1V750 = 0x0AU, /* SW2 output voltage 1.750V. */
	kPCA9420_Sw2OutVolt1V775 = 0x0BU, /* SW2 output voltage 1.775V. */
	kPCA9420_Sw2OutVolt1V800 = 0x0CU, /* SW2 output voltage 1.800V. */
	kPCA9420_Sw2OutVolt1V825 = 0x0DU, /* SW2 output voltage 1.825V. */
	kPCA9420_Sw2OutVolt1V850 = 0x0EU, /* SW2 output voltage 1.850V. */
	kPCA9420_Sw2OutVolt1V875 = 0x0FU, /* SW2 output voltage 1.875V. */
	kPCA9420_Sw2OutVolt1V900 = 0x10U, /* SW2 output voltage 1.900V. */
	kPCA9420_Sw2OutVolt1V925 = 0x11U, /* SW2 output voltage 1.925V. */
	kPCA9420_Sw2OutVolt1V950 = 0x12U, /* SW2 output voltage 1.950V. */
	kPCA9420_Sw2OutVolt1V975 = 0x13U, /* SW2 output voltage 1.975V. */
	kPCA9420_Sw2OutVolt2V000 = 0x14U, /* SW2 output voltage 2.000V. */
	kPCA9420_Sw2OutVolt2V025 = 0x15U, /* SW2 output voltage 2.025V. */
	kPCA9420_Sw2OutVolt2V050 = 0x16U, /* SW2 output voltage 2.050V. */
	kPCA9420_Sw2OutVolt2V075 = 0x17U, /* SW2 output voltage 2.075V. */
	kPCA9420_Sw2OutVolt2V100 = 0x18U, /* SW2 output voltage 2.100V. */

	kPCA9420_Sw2OutVolt2V700 = 0x20U, /* SW2 output voltage 2.700V. */
	kPCA9420_Sw2OutVolt2V725 = 0x21U, /* SW2 output voltage 2.725V. */
	kPCA9420_Sw2OutVolt2V750 = 0x22U, /* SW2 output voltage 2.750V. */
	kPCA9420_Sw2OutVolt2V775 = 0x23U, /* SW2 output voltage 2.775V. */
	kPCA9420_Sw2OutVolt2V800 = 0x24U, /* SW2 output voltage 2.800V. */
	kPCA9420_Sw2OutVolt2V825 = 0x25U, /* SW2 output voltage 2.825V. */
	kPCA9420_Sw2OutVolt2V850 = 0x26U, /* SW2 output voltage 2.850V. */
	kPCA9420_Sw2OutVolt2V875 = 0x27U, /* SW2 output voltage 2.875V. */
	kPCA9420_Sw2OutVolt2V900 = 0x28U, /* SW2 output voltage 2.900V. */
	kPCA9420_Sw2OutVolt2V925 = 0x29U, /* SW2 output voltage 2.925V. */
	kPCA9420_Sw2OutVolt2V950 = 0x2AU, /* SW2 output voltage 2.950V. */
	kPCA9420_Sw2OutVolt2V975 = 0x2BU, /* SW2 output voltage 2.975V. */
	kPCA9420_Sw2OutVolt3V000 = 0x2CU, /* SW2 output voltage 3.000V. */
	kPCA9420_Sw2OutVolt3V025 = 0x2DU, /* SW2 output voltage 3.025V. */
	kPCA9420_Sw2OutVolt3V050 = 0x2EU, /* SW2 output voltage 3.050V. */
	kPCA9420_Sw2OutVolt3V075 = 0x2FU, /* SW2 output voltage 3.075V. */
	kPCA9420_Sw2OutVolt3V100 = 0x30U, /* SW2 output voltage 3.100V. */
	kPCA9420_Sw2OutVolt3V125 = 0x31U, /* SW2 output voltage 3.125V. */
	kPCA9420_Sw2OutVolt3V150 = 0x32U, /* SW2 output voltage 3.150V. */
	kPCA9420_Sw2OutVolt3V175 = 0x33U, /* SW2 output voltage 3.175V. */
	kPCA9420_Sw2OutVolt3V200 = 0x34U, /* SW2 output voltage 3.200V. */
	kPCA9420_Sw2OutVolt3V225 = 0x35U, /* SW2 output voltage 3.225V. */
	kPCA9420_Sw2OutVolt3V250 = 0x36U, /* SW2 output voltage 3.250V. */
	kPCA9420_Sw2OutVolt3V275 = 0x37U, /* SW2 output voltage 3.275V. */
	kPCA9420_Sw2OutVolt3V300 = 0x38U, /* SW2 output voltage 3.300V. */
} pca9420_sw2_out_t;

/*! @brief PCA9420 LDO1 Output Voltage definition. */
typedef enum _pca9420_ldo1_out
{
	kPCA9420_Ldo1OutVolt1V700 = 0x00U, /* LDO1 output voltage 1.700V. */
	kPCA9420_Ldo1OutVolt1V725 = 0x01U, /* LDO1 output voltage 1.725V. */
	kPCA9420_Ldo1OutVolt1V750 = 0x02U, /* LDO1 output voltage 1.750V. */
	kPCA9420_Ldo1OutVolt1V775 = 0x03U, /* LDO1 output voltage 1.775V. */
	kPCA9420_Ldo1OutVolt1V800 = 0x04U, /* LDO1 output voltage 1.800V. */
	kPCA9420_Ldo1OutVolt1V825 = 0x05U, /* LDO1 output voltage 1.825V. */
	kPCA9420_Ldo1OutVolt1V850 = 0x06U, /* LDO1 output voltage 1.850V. */
	kPCA9420_Ldo1OutVolt1V875 = 0x07U, /* LDO1 output voltage 1.875V. */
	kPCA9420_Ldo1OutVolt1V900 = 0x08U, /* LDO1 output voltage 1.900V. */
} pca9420_ldo1_out_t;

/*! @brief PCA9420 Watchdog Timer Setting definition. */
typedef enum _pca9420_wd_timer
{
	kPCA9420_WdTimerDisabled = 0x00U, /* Watch dog timer disabled. */
	kPCA9420_WdTimer16s      = 0x01U, /* Watch dog timer timeout value is 16 sec. */
	kPCA9420_WdTimer32s      = 0x02U, /* Watch dog timer timeout value is 32 sec. */
	kPCA9420_WdTimer64s      = 0x03U, /* Watch dog timer timeout value is 64 sec. */
} pca9420_wd_timer_t;

/*! @brief PCA9420 LDO2 Output Voltage definition. */
typedef enum _pca9420_ldo2_out
{
	kPCA9420_Ldo2OutVolt1V500 = 0x00U, /* LDO2 output voltage 1.500V. */
	kPCA9420_Ldo2OutVolt1V525 = 0x01U, /* LDO2 output voltage 1.525V. */
	kPCA9420_Ldo2OutVolt1V550 = 0x02U, /* LDO2 output voltage 1.550V. */
	kPCA9420_Ldo2OutVolt1V575 = 0x03U, /* LDO2 output voltage 1.575V. */
	kPCA9420_Ldo2OutVolt1V600 = 0x04U, /* LDO2 output voltage 1.600V. */
	kPCA9420_Ldo2OutVolt1V625 = 0x05U, /* LDO2 output voltage 1.625V. */
	kPCA9420_Ldo2OutVolt1V650 = 0x06U, /* LDO2 output voltage 1.650V. */
	kPCA9420_Ldo2OutVolt1V675 = 0x07U, /* LDO2 output voltage 1.675V. */
	kPCA9420_Ldo2OutVolt1V700 = 0x08U, /* LDO2 output voltage 1.700V. */
	kPCA9420_Ldo2OutVolt1V725 = 0x09U, /* LDO2 output voltage 1.725V. */
	kPCA9420_Ldo2OutVolt1V750 = 0x0AU, /* LDO2 output voltage 1.750V. */
	kPCA9420_Ldo2OutVolt1V775 = 0x0BU, /* LDO2 output voltage 1.775V. */
	kPCA9420_Ldo2OutVolt1V800 = 0x0CU, /* LDO2 output voltage 1.800V. */
	kPCA9420_Ldo2OutVolt1V825 = 0x0DU, /* LDO2 output voltage 1.825V. */
	kPCA9420_Ldo2OutVolt1V850 = 0x0EU, /* LDO2 output voltage 1.850V. */
	kPCA9420_Ldo2OutVolt1V875 = 0x0FU, /* LDO2 output voltage 1.875V. */
	kPCA9420_Ldo2OutVolt1V900 = 0x10U, /* LDO2 output voltage 1.900V. */
	kPCA9420_Ldo2OutVolt1V925 = 0x11U, /* LDO2 output voltage 1.925V. */
	kPCA9420_Ldo2OutVolt1V950 = 0x12U, /* LDO2 output voltage 1.950V. */
	kPCA9420_Ldo2OutVolt1V975 = 0x13U, /* LDO2 output voltage 1.975V. */
	kPCA9420_Ldo2OutVolt2V000 = 0x14U, /* LDO2 output voltage 2.000V. */
	kPCA9420_Ldo2OutVolt2V025 = 0x15U, /* LDO2 output voltage 2.025V. */
	kPCA9420_Ldo2OutVolt2V050 = 0x16U, /* LDO2 output voltage 2.050V. */
	kPCA9420_Ldo2OutVolt2V075 = 0x17U, /* LDO2 output voltage 2.075V. */
	kPCA9420_Ldo2OutVolt2V100 = 0x18U, /* LDO2 output voltage 2.100V. */

	kPCA9420_Ldo2OutVolt2V700 = 0x20U, /* LDO2 output voltage 2.700V. */
	kPCA9420_Ldo2OutVolt2V725 = 0x21U, /* LDO2 output voltage 2.725V. */
	kPCA9420_Ldo2OutVolt2V750 = 0x22U, /* LDO2 output voltage 2.750V. */
	kPCA9420_Ldo2OutVolt2V775 = 0x23U, /* LDO2 output voltage 2.775V. */
	kPCA9420_Ldo2OutVolt2V800 = 0x24U, /* LDO2 output voltage 2.800V. */
	kPCA9420_Ldo2OutVolt2V825 = 0x25U, /* LDO2 output voltage 2.825V. */
	kPCA9420_Ldo2OutVolt2V850 = 0x26U, /* LDO2 output voltage 2.850V. */
	kPCA9420_Ldo2OutVolt2V875 = 0x27U, /* LDO2 output voltage 2.875V. */
	kPCA9420_Ldo2OutVolt2V900 = 0x28U, /* LDO2 output voltage 2.900V. */
	kPCA9420_Ldo2OutVolt2V925 = 0x29U, /* LDO2 output voltage 2.925V. */
	kPCA9420_Ldo2OutVolt2V950 = 0x2AU, /* LDO2 output voltage 2.950V. */
	kPCA9420_Ldo2OutVolt2V975 = 0x2BU, /* LDO2 output voltage 2.975V. */
	kPCA9420_Ldo2OutVolt3V000 = 0x2CU, /* LDO2 output voltage 3.000V. */
	kPCA9420_Ldo2OutVolt3V025 = 0x2DU, /* LDO2 output voltage 3.025V. */
	kPCA9420_Ldo2OutVolt3V050 = 0x2EU, /* LDO2 output voltage 3.050V. */
	kPCA9420_Ldo2OutVolt3V075 = 0x2FU, /* LDO2 output voltage 3.075V. */
	kPCA9420_Ldo2OutVolt3V100 = 0x30U, /* LDO2 output voltage 3.100V. */
	kPCA9420_Ldo2OutVolt3V125 = 0x31U, /* LDO2 output voltage 3.125V. */
	kPCA9420_Ldo2OutVolt3V150 = 0x32U, /* LDO2 output voltage 3.150V. */
	kPCA9420_Ldo2OutVolt3V175 = 0x33U, /* LDO2 output voltage 3.175V. */
	kPCA9420_Ldo2OutVolt3V200 = 0x34U, /* LDO2 output voltage 3.200V. */
	kPCA9420_Ldo2OutVolt3V225 = 0x35U, /* LDO2 output voltage 3.225V. */
	kPCA9420_Ldo2OutVolt3V250 = 0x36U, /* LDO2 output voltage 3.250V. */
	kPCA9420_Ldo2OutVolt3V275 = 0x37U, /* LDO2 output voltage 3.275V. */
	kPCA9420_Ldo2OutVolt3V300 = 0x38U, /* LDO2 output voltage 3.300V. */
} pca9420_ldo2_out_t;

/*! @brief PCA9420 Mode Configuration definition. */
typedef struct _pca9420_modecfg
{
	pca9420_ship_en_t shipModeEnable;
	pca9420_mode_sel_t modeSel;
	pca9420_on_cfg_t onCfg;
	pca9420_wd_timer_t wdogTimerCfg;
	pca9420_sw1_out_t sw1OutVolt;
	pca9420_sw2_out_t sw2OutVolt;
	pca9420_ldo1_out_t ldo1OutVolt;
	pca9420_ldo2_out_t ldo2OutVolt;
	bool enableSw1Out;
	bool enableSw2Out;
	bool enableLdo1Out;
	bool enableLdo2Out;
} pca9420_modecfg_t;

/*! @brief PCA9420 regulators voltage definition. */
typedef struct _pca9420_regulator_mv
{
	uint32_t mVoltSw1;  /* SW1 milli volt. */
	uint32_t mVoltSw2;  /* SW2 milli volt. */
	uint32_t mVoltLdo1; /* LDO1 milli volt. */
	uint32_t mVoltLdo2; /* LDO2 milli volt. */
} pca9420_regulator_mv_t;


enum _pca9420_bat_chrg_cur
{
	kPCA9420_ICHG_CC_0  = 0x00,
	kPCA9420_ICHG_CC_5  = 0x01,
	kPCA9420_ICHG_CC_10 = 0x02,
	kPCA9420_ICHG_CC_15 = 0x03,
	kPCA9420_ICHG_CC_20 = 0x04,
	kPCA9420_ICHG_CC_25 = 0x05,
	kPCA9420_ICHG_CC_30 = 0x06,
	kPCA9420_ICHG_CC_35 = 0x07,
	kPCA9420_ICHG_CC_40 = 0x08,
	kPCA9420_ICHG_CC_45 = 0x09,
	kPCA9420_ICHG_CC_50 = 0x0A,
	kPCA9420_ICHG_CC_55 = 0x0B,
	kPCA9420_ICHG_CC_60 = 0x0C,
	kPCA9420_ICHG_CC_65 = 0x0D,
	kPCA9420_ICHG_CC_70 = 0x0E,
	kPCA9420_ICHG_CC_75 = 0x0F,
	kPCA9420_ICHG_CC_80 = 0x10,
	kPCA9420_ICHG_CC_85 = 0x11,
	kPCA9420_ICHG_CC_90 = 0x12,
	kPCA9420_ICHG_CC_95 = 0x13,
	kPCA9420_ICHG_CC_100 = 0x14,
	kPCA9420_ICHG_CC_105 = 0x15,
	kPCA9420_ICHG_CC_110 = 0x16,
	kPCA9420_ICHG_CC_115 = 0x17,
	kPCA9420_ICHG_CC_120 = 0x18,
	kPCA9420_ICHG_CC_125 = 0x19,
	kPCA9420_ICHG_CC_130 = 0x1A,
	kPCA9420_ICHG_CC_135 = 0x1B,
	kPCA9420_ICHG_CC_140 = 0x1C,
	kPCA9420_ICHG_CC_145 = 0x1D,
	kPCA9420_ICHG_CC_150 = 0x1E,
	kPCA9420_ICHG_CC_155 = 0x1F,
	kPCA9420_ICHG_CC_160 = 0x20,
	kPCA9420_ICHG_CC_165 = 0x21,
	kPCA9420_ICHG_CC_170 = 0x22,
	kPCA9420_ICHG_CC_175 = 0x23,
	kPCA9420_ICHG_CC_180 = 0x24,
	kPCA9420_ICHG_CC_185 = 0x25,
	kPCA9420_ICHG_CC_190 = 0x26,
	kPCA9420_ICHG_CC_195 = 0x27,
	kPCA9420_ICHG_CC_200 = 0x28,
	kPCA9420_ICHG_CC_205 = 0x29,
	kPCA9420_ICHG_CC_210 = 0x2A,
	kPCA9420_ICHG_CC_215 = 0x2B,
	kPCA9420_ICHG_CC_220 = 0x2C,
	kPCA9420_ICHG_CC_225 = 0x2D,
	kPCA9420_ICHG_CC_230 = 0x2E,
	kPCA9420_ICHG_CC_235 = 0x2F,
	kPCA9420_ICHG_CC_240 = 0x30,
	kPCA9420_ICHG_CC_245 = 0x31,
	kPCA9420_ICHG_CC_250 = 0x32,
	kPCA9420_ICHG_CC_255 = 0x33,
	kPCA9420_ICHG_CC_260 = 0x34,
	kPCA9420_ICHG_CC_265 = 0x35,
	kPCA9420_ICHG_CC_270 = 0x36,
	kPCA9420_ICHG_CC_275 = 0x37,
	kPCA9420_ICHG_CC_280 = 0x38,
	kPCA9420_ICHG_CC_285 = 0x39,
	kPCA9420_ICHG_CC_290 = 0x3A,
	kPCA9420_ICHG_CC_295 = 0x3B,
	kPCA9420_ICHG_CC_300 = 0x3C,
	kPCA9420_ICHG_CC_305 = 0x3D,
	kPCA9420_ICHG_CC_310 = 0x3E,
	kPCA9420_ICHG_CC_315 = 0x3F,
};

enum _pca9420_bat_topoff_cur
{
	kPCA9420_ICHG_TOPOFF_0  = 0x00,
	kPCA9420_ICHG_TOPOFF_1 = 0x01,
	kPCA9420_ICHG_TOPOFF_2 = 0x02,
	kPCA9420_ICHG_TOPOFF_3  = 0x03,
	kPCA9420_ICHG_TOPOFF_4 = 0x04,
	kPCA9420_ICHG_TOPOFF_5 = 0x05,
	kPCA9420_ICHG_TOPOFF_6 = 0x06,
	kPCA9420_ICHG_TOPOFF_7 = 0x07,
	kPCA9420_ICHG_TOPOFF_8 = 0x08,
	kPCA9420_ICHG_TOPOFF_9 = 0x09,
	kPCA9420_ICHG_TOPOFF_10 = 0x0A,
	kPCA9420_ICHG_TOPOFF_11 = 0x0B,
	kPCA9420_ICHG_TOPOFF_12 = 0x0C,
	kPCA9420_ICHG_TOPOFF_13 = 0x0D,
	kPCA9420_ICHG_TOPOFF_14 = 0x0E,
	kPCA9420_ICHG_TOPOFF_15 = 0x0F,
	kPCA9420_ICHG_TOPOFF_16 = 0x10,
	kPCA9420_ICHG_TOPOFF_17 = 0x11,
	kPCA9420_ICHG_TOPOFF_18 = 0x12,
	kPCA9420_ICHG_TOPOFF_19 = 0x13,
	kPCA9420_ICHG_TOPOFF_20 = 0x14,
	kPCA9420_ICHG_TOPOFF_21 = 0x15,
	kPCA9420_ICHG_TOPOFF_22 = 0x16,
	kPCA9420_ICHG_TOPOFF_23 = 0x17,
	kPCA9420_ICHG_TOPOFF_24 = 0x18,
	kPCA9420_ICHG_TOPOFF_25 = 0x19,
	kPCA9420_ICHG_TOPOFF_26 = 0x1A,
	kPCA9420_ICHG_TOPOFF_27 = 0x1B,
	kPCA9420_ICHG_TOPOFF_28 = 0x1C,
	kPCA9420_ICHG_TOPOFF_29 = 0x1D,
	kPCA9420_ICHG_TOPOFF_30 = 0x1E,
	kPCA9420_ICHG_TOPOFF_31 = 0x1F,
	kPCA9420_ICHG_TOPOFF_32 = 0x20,
	kPCA9420_ICHG_TOPOFF_33 = 0x21,
	kPCA9420_ICHG_TOPOFF_34 = 0x22,
	kPCA9420_ICHG_TOPOFF_35 = 0x23,
	kPCA9420_ICHG_TOPOFF_36 = 0x24,
	kPCA9420_ICHG_TOPOFF_37 = 0x25,
	kPCA9420_ICHG_TOPOFF_38 = 0x26,
	kPCA9420_ICHG_TOPOFF_39 = 0x27,
	kPCA9420_ICHG_TOPOFF_40 = 0x28,
	kPCA9420_ICHG_TOPOFF_41 = 0x29,
	kPCA9420_ICHG_TOPOFF_42 = 0x2A,
	kPCA9420_ICHG_TOPOFF_43 = 0x2B,
	kPCA9420_ICHG_TOPOFF_44 = 0x2C,
	kPCA9420_ICHG_TOPOFF_45 = 0x2D,
	kPCA9420_ICHG_TOPOFF_46 = 0x2E,
	kPCA9420_ICHG_TOPOFF_47 = 0x2F,
	kPCA9420_ICHG_TOPOFF_48 = 0x30,
	kPCA9420_ICHG_TOPOFF_49 = 0x31,
	kPCA9420_ICHG_TOPOFF_50 = 0x32,
	kPCA9420_ICHG_TOPOFF_51 = 0x33,
	kPCA9420_ICHG_TOPOFF_52 = 0x34,
	kPCA9420_ICHG_TOPOFF_53 = 0x35,
	kPCA9420_ICHG_TOPOFF_54 = 0x36,
	kPCA9420_ICHG_TOPOFF_55 = 0x37,
	kPCA9420_ICHG_TOPOFF_56 = 0x38,
	kPCA9420_ICHG_TOPOFF_57 = 0x39,
	kPCA9420_ICHG_TOPOFF_58 = 0x3A,
	kPCA9420_ICHG_TOPOFF_59 = 0x3B,
	kPCA9420_ICHG_TOPOFF_60 = 0x3C,
	kPCA9420_ICHG_TOPOFF_61 = 0x3D,
	kPCA9420_ICHG_TOPOFF_62 = 0x3E,
	kPCA9420_ICHG_TOPOFF_63 = 0x3F,
};

typedef enum _pca9420_low_bat_chrg_cur
{
	kPCA9420_ICHG_LOW_0  = 0x00,
	kPCA9420_ICHG_LOW_8  = 0x08,
	kPCA9420_ICHG_LOW_16 = 0x10,
	kPCA9420_ICHG_LOW_24 = 0x18,
} pca9420_low_bat_chrg_cur;

typedef enum _pca9420_dead_chrg_timer
{
	kPCA9420_ICHG_DEAD_TIMER_5  = 0x00,
	kPCA9420_ICHG_DEAD_TIMER_10 = 0x01,
	kPCA9420_ICHG_DEAD_TIMER_20 = 0x02,
	kPCA9420_ICHG_DEAD_TIMER_40 = 0x03,
} pca9420_dead_chrg_timer;

typedef enum _pca9420_dead_bat_chrg_cur
{
	kPCA9420_ICHG_DEAD_0  = 0x00,
	kPCA9420_ICHG_DEAD_4  = 0x04,
	kPCA9420_ICHG_DEAD_16 = 0x10,
	kPCA9420_ICHG_DEAD_20 = 0x14,
} pca9420_dead_bat_chrg_cur;

typedef enum _pca9420_threshld_rechrg
{
	kPCA9420_VBAT_RESTART_140  = 0x00,
	kPCA9420_VBAT_RESTART240  = 0x01,
} pca9420_threshld_rechrg;

enum _pca9420_bat_reg_vol
{
	kPCA9420_VBATREG_3_60 = 0x00,
	kPCA9420_VBATREG_3_62 = 0x01,
	kPCA9420_VBATREG_3_64 = 0x02,
	kPCA9420_VBATREG_3_66 = 0x03,
	kPCA9420_VBATREG_3_68 = 0x04,
	kPCA9420_VBATREG_3_70 = 0x05,
	kPCA9420_VBATREG_3_72 = 0x06,
	kPCA9420_VBATREG_3_74 = 0x07,
	kPCA9420_VBATREG_3_76 = 0x08,
	kPCA9420_VBATREG_3_78 = 0x09,
	kPCA9420_VBATREG_3_80 = 0x0A,
	kPCA9420_VBATREG_3_82 = 0x0B,
	kPCA9420_VBATREG_3_84 = 0x0C,
	kPCA9420_VBATREG_3_86 = 0x0D,
	kPCA9420_VBATREG_3_88 = 0x0E,
	kPCA9420_VBATREG_3_90 = 0x0F,
	kPCA9420_VBATREG_3_92 = 0x10,
	kPCA9420_VBATREG_3_94 = 0x11,
	kPCA9420_VBATREG_3_96 = 0x12,
	kPCA9420_VBATREG_3_98 = 0x13,
	kPCA9420_VBATREG_4_00 = 0x14,
	kPCA9420_VBATREG_4_02 = 0x15,
	kPCA9420_VBATREG_4_04 = 0x16,
	kPCA9420_VBATREG_4_06 = 0x17,
	kPCA9420_VBATREG_4_08 = 0x18,
	kPCA9420_VBATREG_4_10 = 0x19,
	kPCA9420_VBATREG_4_12 = 0x1A,
	kPCA9420_VBATREG_4_14 = 0x1B,
	kPCA9420_VBATREG_4_16 = 0x1C,
	kPCA9420_VBATREG_4_18 = 0x1D,
	kPCA9420_VBATREG_4_20 = 0x1E,
	kPCA9420_VBATREG_4_22 = 0x1F,
	kPCA9420_VBATREG_4_24 = 0x20,
	kPCA9420_VBATREG_4_26 = 0x21,
	kPCA9420_VBATREG_4_28 = 0x22,
	kPCA9420_VBATREG_4_30 = 0x23,
	kPCA9420_VBATREG_4_32 = 0x24,
	kPCA9420_VBATREG_4_34 = 0x25,
	kPCA9420_VBATREG_4_36 = 0x26,
	kPCA9420_VBATREG_4_38 = 0x27,
	kPCA9420_VBATREG_4_40 = 0x28,
	kPCA9420_VBATREG_4_42 = 0x29,
	kPCA9420_VBATREG_4_44 = 0x2A,
	kPCA9420_VBATREG_4_46 = 0x2B,
	kPCA9420_VBATREG_4_48 = 0x2C,
	kPCA9420_VBATREG_4_50 = 0x2D,
	kPCA9420_VBATREG_4_52 = 0x2E,
	kPCA9420_VBATREG_4_54 = 0x2F,
	kPCA9420_VBATREG_4_56 = 0x30,
	kPCA9420_VBATREG_4_58 = 0x31,
	kPCA9420_VBATREG_4_60 = 0x32,
};

enum _pca9420_ntc_res_sel
{
	kPCA9420_NTC_RES_SEL_100 = 0x00,
	kPCA9420_NTC_RES_SEL_10  = 0x01,
};

enum _pca9420_fast_chrg_timer
{
	kPCA9420_ICHG_FAST_TIMER_3  = 0x00,
	kPCA9420_ICHG_FAST_TIMER_5  = 0x01,
	kPCA9420_ICHG_FAST_TIMER_7  = 0x02,
	kPCA9420_ICHG_FAST_TIMER_9  = 0x03,
};

enum _pca9420_preq_chrg_timer
{
	kPCA9420_ICHG_PREQ_TIMER_15  = 0x00,
	kPCA9420_ICHG_PREQ_TIMER_30  = 0x01,
	kPCA9420_ICHG_PREQ_TIMER_45 = 0x02,
	kPCA9420_ICHG_PREQ_TIMER_60 = 0x03,
};

enum _pca9420_topoff_timer
{
	kPCA9420_TOPOFF_TIMER_0 = 0x00,
	kPCA9420_TOPOFF_TIMER_6_4  = 0x01,
	kPCA9420_TOPOFF_TIMER_12_8 = 0x02,
	kPCA9420_TOPOFF_TIMER_19_2 = 0x03,
};

enum _pca9420_ntc_beta_val
{
	kPCA9420_NTC_BETA_SEL_3434 = 0x00,
	kPCA9420_NTC_BETA_SEL_3610 = 0x01,
	kPCA9420_NTC_BETA_SEL_3934 = 0x02,
	kPCA9420_NTC_BETA_SEL_3950 = 0x03,
	kPCA9420_NTC_BETA_SEL_4100 = 0x04,
	kPCA9420_NTC_BETA_SEL_4311 = 0x05,
	kPCA9420_NTC_BETA_SEL_4543 = 0x06,
	kPCA9420_NTC_BETA_SEL_4750 = 0x07,
};

enum _pca9420_thrml_reg_thshld
{
	kPCA9420_THM_REG_80 = 0x00,
	kPCA9420_THM_REG_85 = 0x01,
	kPCA9420_THM_REG_90 = 0x02,
	kPCA9420_THM_REG_95 = 0x03,
	kPCA9420_THM_REG_100 = 0x04,
	kPCA9420_THM_REG_105 = 0x05,
	kPCA9420_THM_REG_110 = 0x06,
	kPCA9420_THM_REG_115 = 0x07,
};

enum _pca9420_vol_reg_source
{
	kPCA9420_SW1 = 0x01,
	kPCA9420_SW2 = 0x02,
	kPCA9420_LDO1 = 0x03,
	kPCA9420_LDO2 = 0x04,
};

#endif /* PCA9420UK_H_ */
