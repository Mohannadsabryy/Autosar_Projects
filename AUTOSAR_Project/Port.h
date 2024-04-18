/******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.h
 *
 * Description: Header file for TM4C123GH6PM Microcontroller - Port Driver.
 *
 * Author: Mohannad Sabry
 ******************************************************************************/

#ifndef PORT_H_
#define PORT_H_

/*====================================================================
                     File Version Information
=====================================================================*/

/* Id for the company in the AUTOSAR
 * for example Mohannad ID = 5000 :) */
#define PORT_VENDOR_ID    (5000U)


/* PORT Module Id */
#define PORT_MODULE_ID    (124U)

/* PORT Instance Id */
#define PORT_INSTANCE_ID  (0U)


/*
 * AUTOSAR Version 4.0.3
*/
#define PORT_AR_RELEASE_MAJOR_VERSION     (4U)
#define PORT_AR_RELEASE_MINOR_VERSION     (0U)
#define PORT_AR_RELEASE_PATCH_VERSION     (3U)

/*
 * Module Version 1.0.0
*/
#define PORT_SW_MAJOR_VERSION             (1U)
#define PORT_SW_MINOR_VERSION             (0U)
#define PORT_SW_PATCH_VERSION             (0U)


/*====================================================================
                        Include files
=====================================================================*/
#include "Std_Types.h"

#if((PORT_AR_RELEASE_MAJOR_VERSION != STD_TYPES_AR_RELEASE_MAJOR_VERSION)\
   ||(PORT_AR_RELEASE_MINOR_VERSION != STD_TYPES_AR_RELEASE_MINOR_VERSION)\
   ||(PORT_AR_RELEASE_PATCH_VERSION != STD_TYPES_AR_RELEASE_PATCH_VERSION))
        #error "The AR version of Std_Types.h does not match the expected version"
#endif

#include "Port_Cfg.h"

#if((PORT_AR_RELEASE_MAJOR_VERSION != PORT_CFG_AR_RELEASE_MAJOR_VERSION)\
   ||(PORT_AR_RELEASE_MINOR_VERSION != PORT_CFG_AR_RELEASE_MINOR_VERSION)\
   ||(PORT_AR_RELEASE_PATCH_VERSION != PORT_CFG_AR_RELEASE_PATCH_VERSION))
        #error "The AR version of Port_Cfg.h does not match the expected version"
#endif

#if((PORT_SW_MAJOR_VERSION  != PORT_CFG_SW_MAJOR_VERSION)\
   ||(PORT_SW_MINOR_VERSION != PORT_CFG_SW_MINOR_VERSION)\
   ||(PORT_SW_PATCH_VERSION != PORT_CFG_SW_PATCH_VERSION))
        #error "The SW version of Port_Cfg.h does not match the expected version"
#endif

/* Non AUTOSAR files */
#include "Common_Macros.h"

/*====================================================================
                        DET Error Codes
=====================================================================*/

/* Det code to report Invalid Port Pin ID requested */
#define PORT_E_PARAM_PIN          (uint8)(0x0A)

/* Det code to report Port Pin not configured as changeable */
#define PORT_E_DIRECTION_UNCHANGEABLE (uint8)(0x0B)

/* Det code to report API Port_Init service called with wrong parameter */
#define PORT_E_PARAM_CONFIG       (uint8)(0x0C)

/* Det code to report API Port_SetPinMode service called when mode is unchangeable. */
#define PORT_E_PARAM_INVALID_MODE (uint8)(0x0D)
#define PORT_E_MODE_UNCHANGEABLE  (uint8)(0x0E)

/* Det code to report API service called without module initialization */
#define PORT_E_UNINIT             (uint8)(0x0F)

/* Det code to report APIs called with a Null Pointer */
#define PORT_E_PARAM_POINTER      (uint8)(0x10)

/*====================================================================
                        API Service Id Macros
=====================================================================*/

/* Service ID for Port Init */
#define PORT_INIT_SID                      (uint8)(0x00)

/* Service ID for Port set pin direction */
#define PORT_SET_PIN_DIRECTION_SID         (uint8)(0x01)

/* Service ID for Port set pin direction */
#define PORT_RFRESH_PORT_DIRECTION_SID     (uint8)(0x02)

/* Service ID for Port get version info */
#define PORT_GET_VERSION_INFO_SID          (uint8)(0x03)

/* Service ID for Port set pin mode */
#define PORT_SET_PIN_MODE_SID              (uint8)(0x04)

/*====================================================================
                     Macros for Port Status
=====================================================================*/

#define PORT_INITIALIZED                         (1U)

#define PORT_NOT_INITIALIZED                     (0U)

/*====================================================================
                       Module Types
=====================================================================*/

/* Data type for the symbolic name of a port pin */
typedef uint8 Port_PinType;

/* Different port pin modes */
typedef uint8 Port_PinModeType;

/* Possible directions of a port pin */
typedef enum{
    PORT_PIN_IN,
    PORT_PIN_OUT
}Port_PinDirectionType;

/* Description: Enum to hold internal resistor type for PIN */
typedef enum
{
    OFF,
    PULL_UP,
    PULL_DOWN
}Port_InternalResistor;

/* General mode type for Dio*/
#define PORT_DIO_MODE                            (Port_PinType)0

/* General mode type for Adc*/
#define PORT_ADC_MODE                            (Port_PinType)10


/* Description: Enum to hold Port pin mode from mode list */
typedef enum{
    PORT_PIN_MODE_ADC,
    PORT_PIN_MODE_CAN,
    PORT_PIN_MODE_DIO,
    PORT_PIN_MODE_DIO_GPT,
    PORT_PIN_MODE_DIO_WDG,
    PORT_PIN_MODE_ICU,
    PORT_PIN_MODE_LIN,
    PORT_PIN_MODE_PWM,
    PORT_PIN_MODE_SPI,
    PORT_PIN_MODE_UART
}Port_PinMode;

/* Description: Enum to hold Port pin mode from mode list */
typedef struct{
    uint8 port_num;
    Port_PinType pin_num;
    Port_PinDirectionType pin_direction;
    Port_InternalResistor internal_resistor;
    Port_PinModeType pin_mode;
    uint8 mode_changeable;
    uint8 direction_changeable;
    uint8 intial_value;

}Port_ConfigPin;

/* Data Structure required for initializing the Port Driver */
typedef struct{
    Port_ConfigPin Pins[PORT_CONFIGURED_PINS];
}Port_ConfigType;




/*====================================================================
                      Function Prototypes
=====================================================================*/

/* Function for PORT Initialization API */
void Port_Init(const Port_ConfigType* ConfigPtr);


#if(PORT_SET_PIN_DIRECTION_API==STD_ON)

/* Function for Port Set Pin Direction API */
void Port_SetPinDirection( Port_PinType Pin, Port_PinDirectionType Direction );

#endif

/* Function for Port Refresh Port Direction API*/
void Port_RefreshPortDirection( void );

/* Function for Port Get Version Info API*/
#if(PORT_VERSION_INFO_API==STD_ON)
    void Port_GetVersionInfo( Std_VersionInfoType* versioninfo );
#endif

/* Function for Port Set Pin Mode API*/
#if(PORT_SET_PIN_MODE_API==STD_ON)
void Port_SetPinMode( Port_PinType Pin, Port_PinModeType Mode );
#endif

/*====================================================================
                          External Variables
=====================================================================*/

/* Extern PB structures to be used by Port and other modules */
extern const Port_ConfigType Port_Configuration;

#endif /* PORT_H_ */
