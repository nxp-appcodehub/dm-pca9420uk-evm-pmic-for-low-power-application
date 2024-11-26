/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*! File: frdm_pca9420uk_shield.h
* @brief The frdm_pca9420uk_shield.h file declares arduino pin mapping for frdm_pca9420uk_shield expansion board.
*/

#ifndef _FRDM_PCA9420UK_SHIELD_H_
#define _FRDM_PCA9420UK_SHIELD_H_

/* The shield name */
#define SHIELD_NAME "FRDM-PCA9420UK-EVM"

// PCA9420UK-EVM Information
#define PCA9420UK_I2C_ADDR 0x61

#define PCA9420_INT1_IRQ      GPIO00_IRQn
#define PCA9420_INT1_ISR      GPIO00_IRQHandler

// FRDM-PCA9420UK Shield Reset
#define RESET_GPIO A3

#endif /* _FRDM_PCA9420UK_SHIELD_H_ */