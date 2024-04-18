/******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port_Cfg.h
 *
 * Description: Pre-Compile Configuration Header file for TM4C123GH6PM Microcontroller - Port Driver
 *
 * Author: Mohannad Sabry
 ******************************************************************************/

#ifndef PORT_CFG_H_
#define PORT_CFG_H_

/*====================================================================
                     File Version Information
=====================================================================*/

/*
 * AUTOSAR Version 4.0.3
*/
#define PORT_CFG_AR_RELEASE_MAJOR_VERSION     (4U)
#define PORT_CFG_AR_RELEASE_MINOR_VERSION     (0U)
#define PORT_CFG_AR_RELEASE_PATCH_VERSION     (3U)

/*
 * Module Version 1.0.0
*/
#define PORT_CFG_SW_MAJOR_VERSION             (1U)
#define PORT_CFG_SW_MINOR_VERSION             (0U)
#define PORT_CFG_SW_PATCH_VERSION             (0U)

/*====================================================================
                     Configuration Parameters
=====================================================================*/

/* Pre-compile option for Development Error Detect */
#define PORT_DEV_ERROR_DETECT               (STD_ON)

/* Pre-processor switch to enable / disable the use of the function */
#define PORT_SET_PIN_DIRECTION_API          (STD_ON)

/* Pre-processor switch to enable / disable the use of the function */
#define PORT_SET_PIN_MODE_API               (STD_ON)

/* Pre-processor switch to enable / disable the use of the function */
#define PORT_VERSION_INFO_API               (STD_ON)

/* Number of the configured Port Channels */
#define PORT_CONFIGURED_PINS                   (2U)

/* Channel Index in the array of structures in Port_PBcfg.c */
#define PortConf_LED1_PIN_ID_INDEX             (uint8)0x00
#define PortConf_SW1_PIN_ID_INDEX              (uint8)0x01

/* PORT Configured Port ID's  */
#define PortConf_LED1_PORT_NUM                  (uint8)5 /* PORTF */
#define PortConf_SW1_PORT_NUM                   (uint8)5 /* PORTF */

/* PORT Configured Channel ID's */
#define PortConf_LED1_PIN_NUM                   (Port_PinType)1 /* Pin 1 in PORTF */
#define PortConf_SW1_PIN_NUM                    (Port_PinType)4 /* Pin 4 in PORTF */

/*Parameter to indicate if the direction is changeable on a port pin during runtime*/
#define PortConf_LED1_PIN_DIRECTION_CHANGEABLE  (STD_ON)

/*Parameter to indicate if the direction is changeable on a port pin during runtime*/
#define PortConf_SW1_PIN_DIRECTION_CHANGEABLE   (STD_ON)

/*Parameter to indicate if the mode is changeable on a port pin during runtime*/
#define PortConf_LED1_PIN_MODE_CHANGEABLE       (STD_ON)

/*Parameter to indicate if the mode is changeable on a port pin during runtime*/
#define PortConf_SW1_PIN_MODE_CHANGEABLE        (STD_ON)

/* Enum for Configure Pin Mode*/
typedef enum{
    PortF_Pin1_Dio,
    PortF_Pin1_U1RTS,
    PortF_Pin1_SSI1Rx,
    PortF_Pin1_M1PWM5=5,
    PortF_Pin1_PhB0,
    PortF_Pin1_T0CCP1,
    PortF_Pin1_C1o=9,
    PortF_Pin1_TRD1=14
}PortF_Pin1_Intial_Modes;

/* Enum for Configure Pin Mode*/
typedef enum{
    PortF_Pin4_Dio,
    PortF_Pin4_M1FAULT0=5,
    PortF_Pin4_IDX0,
    PortF_Pin4_T2CCP0,
    PortF_Pin4_USB0EPEN
}PortF_Pin4_Intial_Modes;

/* TM4C123GH6PM Ports */
typedef enum{
    PORTA,
    PORTB,
    PORTC,
    PORTD,
    PORTE,
    PORTF
}PORTS;

/* TM4C123GH6PM Pins */
typedef enum{
    PIN0,
    PIN1,
    PIN2,
    PIN3,
    PIN4,
    PIN5,
    PIN6,
    PIN7
}PINS;

#endif /* PORT_CFG_H_ */
