/******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.c
 *
 * Description: Source file for TM4C123GH6PM Microcontroller - Port Driver
 *
 * Author: Mohannad Sabry
 ******************************************************************************/

/*====================================================================
                        Include files
=====================================================================*/
#include "Port.h"
#include "Det.h"

#if((PORT_AR_RELEASE_MAJOR_VERSION != DET_AR_MAJOR_VERSION)\
   ||(PORT_AR_RELEASE_MINOR_VERSION != DET_AR_MINOR_VERSION)\
   ||(PORT_AR_RELEASE_PATCH_VERSION != DET_AR_PATCH_VERSION))
        #error "The AR version of Det.h does not match the expected version"
#endif

#include "Port_Regs.h"

/*====================================================================
                       Global Variables
=====================================================================*/
STATIC const Port_ConfigPin * Port_PortPins = NULL_PTR;
STATIC uint8 Port_Status = PORT_NOT_INITIALIZED;
/*====================================================================
                      Functions Implementation
=====================================================================*/

/************************************************************************************
* Service Name: Port_Init
* Service ID[hex]: 0x00
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): ConfigPtr - Pointer to configuration set
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to Initialize the Port Driver module.
************************************************************************************/
void Port_Init(const Port_ConfigType* ConfigPtr){
    boolean error=FALSE;
    /* Check that development error is on */
    #if(PORT_DEV_ERROR_DETECT==STD_ON)
    {
        /* check if the input configuration pointer is not a NULL_PTR */
        if(ConfigPtr==NULL_PTR){
            /* Report a Det error that configuration pointer is a a NULL_PTR */
            Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_SID, PORT_E_PARAM_CONFIG);
            /* Set error value to true */
            error=TRUE;
        }
        else{
            /* Do nothing */
        }

    }
    #endif
    {
        /* In-case there are no errors */
        if(error==FALSE){
            /*
             * Set the module state to initialized and point to the PB configuration structure using a global pointer.
             * This global pointer is global to be used by other functions to read the PB configuration structures
             */
            Port_PortPins=ConfigPtr->Pins;

            uint8 index;

            for(index=0;index<PORT_CONFIGURED_PINS;index++){
               volatile uint32 Port_base;
               /* Point to the correct PORT register according to the Port Id stored in the Port_Num member */
                switch(Port_PortPins[index].port_num){
                    case PORTA:Port_base=GPIO_PORTA_BASE_ADDRESS;
                               break;
                    case PORTB:Port_base=GPIO_PORTB_BASE_ADDRESS;
                               break;
                    case PORTC:Port_base=GPIO_PORTC_BASE_ADDRESS;
                               break;
                    case PORTD:Port_base=GPIO_PORTD_BASE_ADDRESS;
                               break;
                    case PORTE:Port_base=GPIO_PORTE_BASE_ADDRESS;
                               break;
                    case PORTF:Port_base=GPIO_PORTF_BASE_ADDRESS;
                               break;
                }

                /* Enable Clock of the Port */
                SYSCTL_RCGCGPIO_REG |= (1<<Port_PortPins[index].port_num);
                while(!(SYSCTL_PRGPIO_REG & Port_PortPins[index].port_num ));


                if( (Port_PortPins[index].port_num==PORTF && Port_PortPins[index].pin_num==PIN0)\
                  ||(Port_PortPins[index].port_num==PORTD && Port_PortPins[index].pin_num==PIN7)){
                    /* Unlock the GPIOCR register */
                    *((volatile uint32 *)(Port_base+PORT_LOCK_REG_OFFSET))    = 0x4C4F434B;
                    /* Clear then Set the corresponding bit in GPIOCR register to allow changes on this pin */
                    *((volatile uint32 *)(Port_base+PORT_COMMIT_REG_OFFSET)) &=  ~(1<<Port_PortPins[index].pin_num);
                    *((volatile uint32 *)(Port_base+PORT_COMMIT_REG_OFFSET)) |=  (1<<Port_PortPins[index].pin_num);
                }
                else if((Port_PortPins[index].port_num == PORTC) && (Port_PortPins[index].pin_num <= PIN3 )){
                    /* JTAG PINS FOR DEBUGGING ONLY */
                    continue;
                }
                else{
                    /* Do Nothing ... No need to unlock the commit register for this pin */
                }

                /* Check if Mode PORT_DIO_MODE */
                if(Port_PortPins[index].pin_mode==PORT_DIO_MODE){
                    /* Set Corresponding pin in Enable register */
                    *((volatile uint32 *)(Port_base+PORT_DIGITAL_ENABLE_REG_OFFSET)) |= (1<<Port_PortPins[index].pin_num);

                    /* Clear Corresponding pin in AFSEL register */
                    *((volatile uint32 *)(Port_base+PORT_ALT_FUNC_REG_OFFSET)) &= ~(1<<Port_PortPins[index].pin_num);

                    /* Clear Corresponding pin in AMSEL register */
                    *((volatile uint32 *)(Port_base+PORT_ANALOG_MODE_SEL_REG_OFFSET)) &= ~(1<<Port_PortPins[index].pin_num);

                    /* Clear Corresponding pins in PCTL register */
                    *((volatile uint32 *)(Port_base+PORT_CTL_REG_OFFSET)) &= ~(0xF<<Port_PortPins[index].pin_num*4);
                }
                /* Check if Mode PORT_ADC_MODE */
                else if(Port_PortPins[index].pin_mode==PORT_ADC_MODE){
                    /* Clear Corresponding pin in Enable register */
                    *((volatile uint32 *)(Port_base+PORT_DIGITAL_ENABLE_REG_OFFSET)) &= ~(1<<Port_PortPins[index].pin_num);

                    /* Clear Corresponding pin in AFSEL register */
                    *((volatile uint32 *)(Port_base+PORT_ALT_FUNC_REG_OFFSET)) &= ~(1<<Port_PortPins[index].pin_num);

                    /* Set Corresponding pin in AMSEL register */
                    *((volatile uint32 *)(Port_base+PORT_ANALOG_MODE_SEL_REG_OFFSET)) |= (1<<Port_PortPins[index].pin_num);

                    /* Clear Corresponding pins in PCTL register */
                    *((volatile uint32 *)(Port_base+PORT_CTL_REG_OFFSET)) &= ~(0xF<<Port_PortPins[index].pin_num*4);
                }
                else{
                    /* Set Corresponding pin in Enable register */
                    *((volatile uint32 *)(Port_base+PORT_DIGITAL_ENABLE_REG_OFFSET)) |= (1<<Port_PortPins[index].pin_num);

                    /* Set Corresponding pin in AFSEL register */
                    *((volatile uint32 *)(Port_base+PORT_ALT_FUNC_REG_OFFSET)) |= (1<<Port_PortPins[index].pin_num);

                    /* Clear Corresponding pin in AMSEL register */
                    *((volatile uint32 *)(Port_base+PORT_ANALOG_MODE_SEL_REG_OFFSET)) &= ~(1<<Port_PortPins[index].pin_num);

                    /* Clear Corresponding pins in PCTL register */
                    *((volatile uint32 *)(Port_base+PORT_CTL_REG_OFFSET)) &= ~(0xF<<Port_PortPins[index].pin_num*4);

                    /* Set mode in the Corresponding pins in PCTL register */
                    *((volatile uint32 *)(Port_base+PORT_CTL_REG_OFFSET)) |= (Port_PortPins[index].pin_mode<<Port_PortPins[index].pin_num*4);
                }

                /* Check if the direction is output */
                if(Port_PortPins[index].pin_direction==PORT_PIN_OUT){
                    /* Set Corresponding pin in DIR register */
                    *((volatile uint32 *)(Port_base+PORT_DIR_REG_OFFSET)) |= (1<<Port_PortPins[index].pin_num);

                    /* Set the intial value in the DATA register */
                    *((volatile uint32 *)(Port_base+PORT_DATA_REG_OFFSET)) |= (Port_PortPins[index].intial_value<<Port_PortPins[index].pin_num);
                }
                /* Check if the direction is input */
                else if(Port_PortPins[index].pin_direction==PORT_PIN_IN){
                    /* Clear Corresponding pin in DIR register */
                    *((volatile uint32 *)(Port_base+PORT_DIR_REG_OFFSET)) &= ~(1<<Port_PortPins[index].pin_num);

                    /* Check on the internal_resistor */
                    switch(Port_PortPins[index].internal_resistor){
                                       /* In case internal_resistor is off Clear Corresponding pin in Pull up and Pull Down registers */
                         case OFF:     *((volatile uint32 *)(Port_base+PORT_PULL_DOWN_REG_OFFSET)) &=  ~(1<<Port_PortPins[index].pin_num);
                                       *((volatile uint32 *)(Port_base+PORT_PULL_UP_REG_OFFSET))   &=  ~(1<<Port_PortPins[index].pin_num);
                                        break;


                                       /* In case internal_resistor is PULL_up Clear Corresponding pin in Pull down registers */
                        case PULL_UP:  *((volatile uint32 *)(Port_base+PORT_PULL_DOWN_REG_OFFSET)) &= ~(1<<Port_PortPins[index].pin_num);
                                       /* In case internal_resistor is PULL_UP Set Corresponding pin in Pull up registers */
                                       *((volatile uint32 *)(Port_base+PORT_PULL_UP_REG_OFFSET))   |=  (1<<Port_PortPins[index].pin_num);
                                       break;

                                        /* In case internal_resistor is PULL_DOWN Clear Corresponding pin in Pull up register */
                        case PULL_DOWN: *((volatile uint32 *)(Port_base+PORT_PULL_UP_REG_OFFSET))   &= ~(1<<Port_PortPins[index].pin_num);
                                        /* In case internal_resistor is PULL_DOWN Set Corresponding pin in Pull down register */
                                        *((volatile uint32 *)(Port_base+PORT_PULL_DOWN_REG_OFFSET)) |=  (1<<Port_PortPins[index].pin_num);
                                        break;
                    }
                }
                else{
                    /* Do nothing */
                }

            }
            /* Set the Module State to initialized*/
            Port_Status=PORT_INITIALIZED;
        }
        else{
            /* Do nothing */
        }
    }
}

/************************************************************************************
* Service Name: Port_SetPinDirection
* Service ID[hex]: 0x01
* Sync/Async: Synchronous
* Reentrancy: reentrant
* Parameters (in): Pin - Port Pin ID number , Direction - Port Pin Direction
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to Sets the port pin direction.
************************************************************************************/
/* Check that PORT_SET_PIN_DIRECTION_API is on */
#if(PORT_SET_PIN_DIRECTION_API==STD_ON)
/* Function for Port Set Pin Direction API */
void Port_SetPinDirection( Port_PinType Pin, Port_PinDirectionType Direction ){
    boolean error=FALSE;
/* Check that development error is on */
#if(PORT_DEV_ERROR_DETECT==STD_ON)
       /* Check that Port Module is Initialized */
       if(Port_Status==PORT_NOT_INITIALIZED){
           /* Report a Det Error that Port Module wasn't Initialized */
           Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIRECTION_SID, PORT_E_UNINIT);
           /* Set error value to true*/
           error=TRUE;
       }
       else{
           /* Do nothing */
       }
       /* Check if the used channel is within the valid range */
       if(Pin>=PORT_CONFIGURED_PINS){
           /* Report a Det Error that used channel is not in the valid range */
           Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,PORT_SET_PIN_DIRECTION_SID , PORT_E_PARAM_PIN);
           /* Set error value to true*/
           error=TRUE;
       }
       else{
           /* Do nothing */
       }
       /* Check that the Port Pin direction is changeable */
       if(Port_PortPins[Pin].direction_changeable==STD_OFF){
           /* Report a Det error that the pin direction is unchangeable */
           Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,PORT_SET_PIN_DIRECTION_SID , PORT_E_DIRECTION_UNCHANGEABLE);
           /* Set error value to true*/
           error=TRUE;
       }
       else{
           /* Do nothing */
       }

#endif
       /* In-case there are no errors */
       if(error==FALSE){
           volatile uint32 Port_base;
           /* Point to the correct PORT register according to the Port Id stored in the Port_Num member */
           switch(Port_PortPins[Pin].port_num){
               case PORTA:Port_base=GPIO_PORTA_BASE_ADDRESS;
               break;
               case PORTB:Port_base=GPIO_PORTB_BASE_ADDRESS;
               break;
               case PORTC:Port_base=GPIO_PORTC_BASE_ADDRESS;
               break;
               case PORTD:Port_base=GPIO_PORTD_BASE_ADDRESS;
               break;
               case PORTE:Port_base=GPIO_PORTE_BASE_ADDRESS;
               break;
               case PORTF:Port_base=GPIO_PORTF_BASE_ADDRESS;
               break;
           }

           if((Port_PortPins[Pin].port_num == PORTC) && (Port_PortPins[Pin].pin_num <= PIN3 )){
               /* JTAG PINS FOR DEBUGGING ONLY */
               return;
           }
           else{
               /* Do nothing */
           }

           /* Check if the direction is input */
           if(Direction==PORT_PIN_IN){
               /* Clear Corresponding pin in DIR register */
               *((volatile uint32 *)(Port_base+PORT_DIR_REG_OFFSET)) &= ~(1<<Port_PortPins[Pin].pin_num);
           }
           /* Check if the direction is output */
           else if(Direction==PORT_PIN_OUT){
               /* Set Corresponding pin in DIR register */
               *((volatile uint32 *)(Port_base+PORT_DIR_REG_OFFSET)) |= (1<<Port_PortPins[Pin].pin_num);
           }
           else{
               /* Do nothing */
           }

       }
       else{
           /* Do nothing */
       }
}
#endif

/************************************************************************************
* Service Name: Port_RefreshPortDirection
* Service ID[hex]: 0x02
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Refreshes port direction.
************************************************************************************/
void Port_RefreshPortDirection( void ){
    boolean error=FALSE;
/* Check that development error is on */
#if(PORT_DEV_ERROR_DETECT==STD_ON)
       /* Check that Port Module is Initialized */
       if(Port_Status==PORT_NOT_INITIALIZED){
           /* Report a Det Error that Port Module wasn't Initialized */
           Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_RFRESH_PORT_DIRECTION_SID, PORT_E_UNINIT);
           /* Set error value to true*/
           error=TRUE;
       }
       else{
           /* Do nothing */
       }
#endif
       /* In-case there are no errors */
       if(error==FALSE){
           uint8 index;
           /* Loop on the Configured Channels */
           for(index=0;index<PORT_CONFIGURED_PINS;index++){
               /* Check that the pin direction is changeable */
               if(Port_PortPins[index].direction_changeable==STD_ON){
                   continue;
               }
               else{
                   volatile uint32 Port_base;
                   /* Point to the correct PORT register according to the Port Id stored in the Port_Num member */
                   switch(Port_PortPins[index].port_num){
                       case PORTA:Port_base=GPIO_PORTA_BASE_ADDRESS;
                       break;
                       case PORTB:Port_base=GPIO_PORTB_BASE_ADDRESS;
                       break;
                       case PORTC:Port_base=GPIO_PORTC_BASE_ADDRESS;
                       break;
                       case PORTD:Port_base=GPIO_PORTD_BASE_ADDRESS;
                       break;
                       case PORTE:Port_base=GPIO_PORTE_BASE_ADDRESS;
                       break;
                       case PORTF:Port_base=GPIO_PORTF_BASE_ADDRESS;
                       break;
                   }
                   if((Port_PortPins[index].port_num == PORTC) && (Port_PortPins[index].pin_num <= PIN3 )){
                       /* JTAG PINS FOR DEBUGGING ONLY */
                       continue;
                   }
                   else{
                       /* Do nothing */
                   }
                   /* Check if the direction is output */
                   if(Port_PortPins[index].pin_direction==PORT_PIN_OUT){
                       /* Clear Corresponding pin in DIR register */
                       *((volatile uint32 *)(Port_base+PORT_DIR_REG_OFFSET)) &= ~(1<<Port_PortPins[index].pin_num);
                       /* Set Corresponding pin in DIR register */
                       *((volatile uint32 *)(Port_base+PORT_DIR_REG_OFFSET)) |= (1<<Port_PortPins[index].pin_num);
                   }
                   /* Check if the direction is input */
                   else if(Port_PortPins[index].pin_direction==PORT_PIN_IN){
                       /* Clear Corresponding pin in DIR register */
                       *((volatile uint32 *)(Port_base+PORT_DIR_REG_OFFSET)) &= ~(1<<Port_PortPins[index].pin_num);
                   }
                   else{
                       /* Do nothing */
                   }

               }
           }

       }
       else{
           /* Do nothing */
       }

}

/************************************************************************************
* Service Name: Port_GetVersionInfo
* Service ID[hex]: 0x03
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): versioninfo - Pointer to where to store the version information of this module.
* Return value: None
* Description: Returns the version information of this module.
************************************************************************************/
#if(PORT_VERSION_INFO_API==STD_ON)
void Port_GetVersionInfo( Std_VersionInfoType* versioninfo ){
    boolean error=FALSE;
/* Check that development error is on */
#if(PORT_DEV_ERROR_DETECT==STD_ON)
    /* check if the versioninfo pointer is not a NULL_PTR */
    if(versioninfo==NULL_PTR){
        /* Report a Det error that versioninfo pointer is a null pointer*/
        Det_ReportError(PORT_MODULE_ID,PORT_INSTANCE_ID,PORT_GET_VERSION_INFO_SID,PORT_E_PARAM_POINTER);
        error=TRUE;
    }
    else{
        /* Do nothing */
    }
#endif
    if(error==FALSE){
        /* Copy the module Id */
        versioninfo->moduleID=(uint16)PORT_MODULE_ID;
        /* Copy the vendor Id */
        versioninfo->vendorID=(uint16)PORT_VENDOR_ID;
        /* Copy Software Major Version */
        versioninfo->sw_major_version=(uint8)PORT_SW_MAJOR_VERSION;
        /* Copy Software Minor Version */
        versioninfo->sw_minor_version=(uint8)PORT_SW_MINOR_VERSION;
        /* Copy Software Patch Version */
        versioninfo->sw_patch_version=(uint8)PORT_SW_PATCH_VERSION;
    }
    else{
        /* Do Nothing */
    }
}
#endif

/************************************************************************************
* Service Name: Port_SetPinMode
* Service ID[hex]: 0x04
* Sync/Async: Synchronous
* Reentrancy: reentrant
* Parameters (in): Pin - Port Pin ID number, Mode - New Port Pin mode to be set on port pin
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Sets the port pin mode.
************************************************************************************/
#if(PORT_SET_PIN_MODE_API==STD_ON)
void Port_SetPinMode( Port_PinType Pin, Port_PinModeType Mode ){
    boolean error=FALSE;
/* Check that development error is on */
#if(PORT_DEV_ERROR_DETECT==STD_ON)
    /* Check that Port Module is Initialized */
    if(Port_Status==PORT_NOT_INITIALIZED){
        /* Report a Det Error that Port Module wasn't Initialized */
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID, PORT_E_UNINIT);
        /* Set error value to true*/
        error=TRUE;
    }
    else{
        /* Do nothing */
    }

    /* Check if the used channel is within the valid range */
    if(Pin>=PORT_CONFIGURED_PINS){
        /* Report a Det Error that used channel is not in the valid range */
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,PORT_SET_PIN_MODE_SID , PORT_E_PARAM_PIN);
        /* Set error value to true*/
        error=TRUE;
    }
    else{
        /* Do nothing */
    }
    /* Check that the Port Pin mode is changeable */
    if(Port_PortPins[Pin].mode_changeable==STD_OFF){
        /* Report a Det error that the pin mode is unchangeable */
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,PORT_SET_PIN_MODE_SID , PORT_E_MODE_UNCHANGEABLE);
        /* Set error value to true*/
        error=TRUE;
    }
    else{
        /* Do nothing */
    }
    /* Check that the Port Pin mode is valid */
    if( (Mode>10 && Mode<14) || (Mode>15) ){
        /* Check that the Port Pin mode is not valid */
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,PORT_SET_PIN_MODE_SID , PORT_E_PARAM_INVALID_MODE);
        /* Set error value to true*/
        error=TRUE;
    }
    else{
        /* Do nothing */
    }
#endif
    /* In-case there are no errors */
    if(error==FALSE){
        volatile uint32 Port_base;
        /* Point to the correct PORT register according to the Port Id stored in the Port_Num member */
        switch(Port_PortPins[Pin].port_num){
            case PORTA:Port_base=GPIO_PORTA_BASE_ADDRESS;
            break;
            case PORTB:Port_base=GPIO_PORTB_BASE_ADDRESS;
            break;
            case PORTC:Port_base=GPIO_PORTC_BASE_ADDRESS;
            break;
            case PORTD:Port_base=GPIO_PORTD_BASE_ADDRESS;
            break;
            case PORTE:Port_base=GPIO_PORTE_BASE_ADDRESS;
            break;
            case PORTF:Port_base=GPIO_PORTF_BASE_ADDRESS;
            break;
        }

        if((Port_PortPins[Pin].port_num == PORTC) && (Port_PortPins[Pin].pin_num <= PIN3 )){
            /* JTAG PINS FOR DEBUGGING ONLY */
            return;
        }
        else{
            /* Do nothing */
        }

        /* Check if Mode PORT_DIO_MODE */
        if(Port_PortPins[Pin].pin_mode==PORT_DIO_MODE){
            /* Set Corresponding pin in Enable register */
            *((volatile uint32 *)(Port_base+PORT_DIGITAL_ENABLE_REG_OFFSET)) |= (1<<Port_PortPins[Pin].pin_num);

            /* Clear Corresponding pin in AFSEL register */
            *((volatile uint32 *)(Port_base+PORT_ALT_FUNC_REG_OFFSET)) &= ~(1<<Port_PortPins[Pin].pin_num);

            /* Clear Corresponding pin in AMSEL register */
            *((volatile uint32 *)(Port_base+PORT_ANALOG_MODE_SEL_REG_OFFSET)) &= ~(1<<Port_PortPins[Pin].pin_num);

            /* Clear Corresponding pins in PCTL register */
            *((volatile uint32 *)(Port_base+PORT_CTL_REG_OFFSET)) &= ~(0xF<<Port_PortPins[Pin].pin_num*4);
        }
        /* Check if Mode PORT_ADC_MODE */
        else if(Port_PortPins[Pin].pin_mode==PORT_ADC_MODE){
            /* Clear Corresponding pin in Enable register */
            *((volatile uint32 *)(Port_base+PORT_DIGITAL_ENABLE_REG_OFFSET)) &= ~(1<<Port_PortPins[Pin].pin_num);

            /* Clear Corresponding pin in AFSEL register */
            *((volatile uint32 *)(Port_base+PORT_ALT_FUNC_REG_OFFSET)) &= ~(1<<Port_PortPins[Pin].pin_num);

            /* Set Corresponding pin in AMSEL register */
            *((volatile uint32 *)(Port_base+PORT_ANALOG_MODE_SEL_REG_OFFSET)) |= (1<<Port_PortPins[Pin].pin_num);

            /* Clear Corresponding pins in PCTL register */
            *((volatile uint32 *)(Port_base+PORT_CTL_REG_OFFSET)) &= ~(0xF<<Port_PortPins[Pin].pin_num*4);
        }
        else{
            /* Set Corresponding pin in Enable register */
            *((volatile uint32 *)(Port_base+PORT_DIGITAL_ENABLE_REG_OFFSET)) |= (1<<Port_PortPins[Pin].pin_num);

            /* Set Corresponding pin in AFSEL register */
            *((volatile uint32 *)(Port_base+PORT_ALT_FUNC_REG_OFFSET)) |= (1<<Port_PortPins[Pin].pin_num);

            /* Clear Corresponding pin in AMSEL register */
            *((volatile uint32 *)(Port_base+PORT_ANALOG_MODE_SEL_REG_OFFSET)) &= ~(1<<Port_PortPins[Pin].pin_num);

            /* Clear Corresponding pins in PCTL register */
            *((volatile uint32 *)(Port_base+PORT_CTL_REG_OFFSET)) &= ~(0xF<<Port_PortPins[Pin].pin_num*4);

            /* Set mode in the Corresponding pins in PCTL register */
            *((volatile uint32 *)(Port_base+PORT_CTL_REG_OFFSET)) |= (Port_PortPins[Pin].pin_mode<<Port_PortPins[Pin].pin_num*4);
        }
    }
    else{
        /* Do nothing */
    }

}
#endif

