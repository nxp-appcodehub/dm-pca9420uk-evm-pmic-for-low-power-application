Hardware requirements
===================
- Mini/micro C USB cable
- FRDM-MCXA153 board
- PCA9420UK-EVM board (https://www.nxp.com/part/PCA9420UK-EVM#/)
- 1-cell rechargeable lithium ion battery
- Personal Computer

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
NOTE: By default I2C controller operates in Interrupt mode. For I2C controller to operate in EDMA mode, Define
#define RTE_I2C2_DMA_EN 1 in RTE_Device.h file under the board directory.

===============
When the demo runs successfully, you can see the logs printed on the terminal. 

LOGS:
===============

ISSDK PCA9420UK-EVM PMIC driver example demonstration with comparator mode.

********** MAIN MENU **********
1. Device Information
2. PMIC Status
3. Regulator Status
4. Check Interrupt Status
5. Device Specific Settings
6. Battery Charging Settings
7. Voltage Regulator Group Settings
8. Enable/Disable Interrupt
******************************
===============

1)Choose options #1 to Get the Device Information.

LOGS:
=============
 Device ID = 0

 Device Revision = 1
===============

2) Choose option #2 to check "PMIC Status"
LOGS:
=============
********** PMIC STATUS **********

 MODE:  Mode 0

 SW1:   1.000000 V

 SW2:   1.800000 V

 LDO1:  1.800000 V

 LDO2:  3.300000 V

********************************
===============

3) Choose option #3 to check "Regulator Status" 
LOGS:
=============
************ REG POWER-GOOD STATUS ************

 VOUT_SW1:   VOUT_SW1 is OK.

 VOUT_SW2:   VOUT_SW2 is OK.

 VOUT_LDO1:  VOUT_LDO1 is OK.

 VOUT_LDO2:  VOUT_LDO2 is OK.
===============

4) Choose option #4 to check "Interrupt Status".
 
To check the status of different types of interrrupt like:
1) System level interrupt
2) Linear battery charger block interrupt
3) Voltage regulator interrupt

LOGS:
==============
 No interrupt occurred
==============

As shown above, currently there is no interrupt occurred.

5) Choose option #5 to configure "Device Specific Settings"
LOGS:
==============
********** DEVICE CONFIGURATION **********

1. VIN input Current Limit Selection

2. Pre-warning Voltage Threshold on ASYS

3. ASYS Input Source Selection

4. VIN Over-Voltage Protection Threshold

5. VIN Under-Voltage Lockout Threshold

6. ASYS Under-Voltage Lockout Threshold

7. Thermal Shutdown Threshold

8. Die Temperature Warning Threshold

9. Mode Selection

10. Long Glitch Timer on ON Key

11. Reset Watchdog Timer

12. Reset PCA9420UK-EVM

13. Exit from Device Configuration Setting
********************************
==============

--Some key configurations examples are given below:

Press #1 to configure VIN input current limit.
==============
Enter your choice :- 1

 Currently VIN input current limit is 370 mA/425 mA/489 mA.

Select VIN input current limit: (min/typ/max)------

1. 74 mA/85 mA/98 mA

2. 370 mA/425 mA/489 mA

Enter your choice :- 1

VIN input current limit is set to 74 mA/85 mA/98 mA.

Press Enter to goto Main Menu
==============

Press #9 to change the mode for device to operate in.
==============
Enter your choice :- 9

 Currently Device is in MODE 0.

Select MODE setting for device to operate------

1. Mode 0

2. Mode 1

3. Mode 2

4. Mode 3

Enter your choice :- 2

 Device is in MODE 1.

********** PMIC STATUS **********

 MODE:  Mode 1

 SW1:   1.200000 V

 SW2:   1.800000 V

 LDO1:  1.800000 V

 LDO2:  1.800000 V

********************************
==============

Press #12 to reset the PMIC device.

Enter your choice :- 12

********************************

 Device reset is done successfully.


 PCA9420UK-EVM initialized with default configurations.

********************************

NOTE: Refer above logs to configure other device settings.

6) Choose option #6 to configure "Battery Charging Settings".
It provides two sub-options:
1) Charge control: To configure settings required for battery charging.
2) Charge status: To check battery, battery charging and the charger status. 

Press #1 to configure charging parameters.

LOGS:
==============

Enter your choice :- 1

********************************

1. Fast Charge Current

2. Top-off Current

3. Pre-charge/Low Battery Charge Current

4. Dead Battery Charge Current

5. Fast Charge Timer

6. Pre-qualification Charge Timer

7. TOPOFF Timer

8. External Thermistor Current Selection

9. Thermistor Beta Value Selection

10. Thermal Regulation Threshold

11. Exit Charging control configuration
==============

---User can configure the charging current and charging timer for different charging profiles listed below:
1) Dead battery mode
2) Low-Voltage mode
3) Fast Charge mode
4) Top-OFF mode
5) Charging Done mode
6) Recharge mode

--Press #3 to configure pre-charge/low battery charge current

********************************

Enter your choice :- 3

 Currently pre-charge current is 8mA.

Select pre-charge(low battery charge) current------

1. 0 mA

2. 8 mA

3. 16 mA

4. 24 mA

Enter your choice :- 1

Pre-charge current is set to 0mA.

*************************************

NOTE: Refer above logs to configure other charging parameters.

---Press #2 to check the charging status.

PCA9420UK-EVM can be a linear battery charger for charging of 1-cell rechargeable lithium ion battery. 
Battery can be connected on TP5 (VBAT point).

Charge detail status when 1-cell lithium ion battery is not connected.

************** Charge Detail Status *****************

 VIN STATUS:     Input power is valid. (VIN>VIN_ULVO, ASYS>VBAT+VINBAT_HEADROOM and VIN<VIN_OVP)

 BAT STATUS:     Battery detection in-progress

 BAT CHG STATUS: Charger in Idle State

 TS STATUS:      Battery Temperature Nominal

 SAFETY STATUS:  Safety Timers having No Effect on Battery Charging

***************************************************

7) Choose option #7 to configure "Voltage Regulator Group Settings".

PCA9420UK-EVM board has 4 voltage regulators:
1) SW1 Buck regulator
2) SW2 Buck regulator
3) LDO1 
4) LDO2

--Press #1 to #4 to select specific mode

******************************

 Select any one MODE to configure voltage regulator settings

 1. MODE 0

 2. MODE 1

 3. MODE 2

 4. MODE 3

 5. Exit from Voltage Regulator Settings
********************************

After entering specific mode, PCA9420 provides various voltage regulator configurations.

Enter your choice :- 1

 Mode 0 is selected.

********************************

1. Enable/Disable Ship Mode

2. Enable/Disable Watchdog Timer

3. On Pin Mode Configuration

4. Mode Control Selection

5. SW1 Buck Regulator

6. SW2 Buck Regulator

7. LDO1

8. LDO2

9. Exit from Group Setting

********************************

--Press #5 to configure the SW1 buck regulator output voltage.

Enter your choice :- 5

1. Enable

2. Disable

3. Exit

Enter your choice :- 1

Enter SW1 output voltage from (0.5 V to 1.5 V with 25 mV/step) or fixed 1.8V ---------
1.425000

********** PMIC STATUS **********

 MODE:  Mode 0

 SW1:   1.425000 V

 SW2:   1.800000 V

 LDO1:  1.800000 V

 LDO2:  3.300000 V

********************************

---Similarly other voltage regulators output can be configured. 
Shipmode enable/disable, watchdog timer enable/disable and ON pin configurations can also be done.

NOTE: For mode selection via MODESEL0 and MODESEL1 external line, configure jumper J7 and J9 on the board.

8) Choose option #8 to "Enable/Disbale Interrupts".

PCA9420UK-EVM has 3 type of interrupt:
1) System level interrupt
2) Linear Battery charger block interrupt
3) Voltage regulator interrupt

--Press #1 for System level interrupt configurations

Enter your choice :- 1

*****************************

************ SYSTEM LEVEL INTERRUPT STATE ************

 Die temperature pre-warning Interrupt:  Disabled.

 Thermal shutdown Interrupt:             Disabled.

 ASYS pre-warning  Interrupt:            Disabled.

 Watchdog timer expiration Interrupt:    Disabled.

 Input voltage Interrupt:                Disabled.


 *****Select any one to enable/disable particular Interrupt*****

1. Die temperature pre-warning interrupt

2. Thermal shutdown interrupt

3. ASYS pre-warning interrupt

4. Watchdog timer expiration interrupt

5. Input voltage interrupt

6. Exit
*****************************************************

By default all the interrupts are disabled, so enabled die-temperature pre-warning interrupt.

*****************************

1. Enable

2. Disable

3. Exit

Enter your choice :- 1

Interrupt Enabled!!!

************ SYSTEM LEVEL INTERRUPT STATE ************

 Die temperature pre-warning Interrupt:  Enabled.

 Thermal shutdown Interrupt:             Disabled.

 ASYS pre-warning  Interrupt:            Disabled.

 Watchdog timer expiration Interrupt:    Disabled.

 Input voltage Interrupt:                Disabled.


 *****Select any one to enable/disable particular Interrupt*****

1. Die temperature pre-warning interrupt

2. Thermal shutdown interrupt

3. ASYS pre-warning interrupt

4. Watchdog timer expiration interrupt

5. Input voltage interrupt

6. Exit
*******************************************************

Note 1: PCA9420_I2C_Initialize() API should be called first in order to use other APIs for differnt device features.
User can refer p3t1085uk_drv.h header file for more information.
