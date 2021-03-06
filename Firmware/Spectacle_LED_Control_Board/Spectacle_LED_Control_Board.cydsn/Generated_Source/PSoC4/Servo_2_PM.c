/*******************************************************************************
* File Name: Servo_2.c  
* Version 2.20
*
* Description:
*  This file contains APIs to set up the Pins component for low power modes.
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "Servo_2.h"

static Servo_2_BACKUP_STRUCT  Servo_2_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: Servo_2_Sleep
****************************************************************************//**
*
* \brief Stores the pin configuration and prepares the pin for entering chip 
*  deep-sleep/hibernate modes. This function must be called for SIO and USBIO
*  pins. It is not essential if using GPIO or GPIO_OVT pins.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None 
*  
* \sideeffect
*  For SIO pins, this function configures the pin input threshold to CMOS and
*  drive level to Vddio. This is needed for SIO pins when in device 
*  deep-sleep/hibernate modes.
*
* \funcusage
*  \snippet Servo_2_SUT.c usage_Servo_2_Sleep_Wakeup
*******************************************************************************/
void Servo_2_Sleep(void)
{
    #if defined(Servo_2__PC)
        Servo_2_backup.pcState = Servo_2_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            Servo_2_backup.usbState = Servo_2_CR1_REG;
            Servo_2_USB_POWER_REG |= Servo_2_USBIO_ENTER_SLEEP;
            Servo_2_CR1_REG &= Servo_2_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(Servo_2__SIO)
        Servo_2_backup.sioState = Servo_2_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        Servo_2_SIO_REG &= (uint32)(~Servo_2_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: Servo_2_Wakeup
****************************************************************************//**
*
* \brief Restores the pin configuration that was saved during Pin_Sleep().
*
* For USBIO pins, the wakeup is only triggered for falling edge interrupts.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None
*  
* \funcusage
*  Refer to Servo_2_Sleep() for an example usage.
*******************************************************************************/
void Servo_2_Wakeup(void)
{
    #if defined(Servo_2__PC)
        Servo_2_PC = Servo_2_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            Servo_2_USB_POWER_REG &= Servo_2_USBIO_EXIT_SLEEP_PH1;
            Servo_2_CR1_REG = Servo_2_backup.usbState;
            Servo_2_USB_POWER_REG &= Servo_2_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(Servo_2__SIO)
        Servo_2_SIO_REG = Servo_2_backup.sioState;
    #endif
}


/* [] END OF FILE */
