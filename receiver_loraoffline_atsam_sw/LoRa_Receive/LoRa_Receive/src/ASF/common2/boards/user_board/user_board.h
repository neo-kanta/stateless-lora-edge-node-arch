/*
 * user_board.h
 *
 * Created: 4/10/2566 11:19:54
 *  Author: kantapit
 */ 

#ifndef USER_BOARD_H
#define USER_BOARD_H

#include <conf_board.h>
#include "samr34j18b.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \ingroup group_common_boards
 * \defgroup user_board_group User board
 *
 * @{
 */

#define __ROLLING_EXIT_SIGN__               0
#define __NEW_MCU_LORA__				    1

void system_board_init(void);

/** Name string macro */
#ifdef __NEW_MCU_LORA__
#define BOARD_NAME                          "NEW_MCU_LORA_ATSAMR34"
#elif __ROLLING_EXIT_SIGN__
#define  BOARD_NAME							"ROLLING_EXIT_SIGN"
#endif

/** \name Resonator definitions
 *  @{ */
#define BOARD_FREQ_SLCK_XTAL                (32768U)
#define BOARD_FREQ_SLCK_BYPASS              (32768U)
#define BOARD_FREQ_MAINCK_XTAL              0 /* Not Mounted */
#define BOARD_FREQ_MAINCK_BYPASS            0 /* Not Mounted */
#define BOARD_MCK                           CHIP_FREQ_CPU_MAX
#define BOARD_OSC_STARTUP_US                15625
/** @} */



#ifdef __ROLLING_EXIT_SIGN__

/** \name LED_MOTOR definitions
 *  @{ */
#define LED0_MOTOR_PIN                      PIN_PA17
#define LED0_MOTOR_ACTIVE                   false
#define LED0_MOTOR_INACTIVE                 !LED0_MOTOR_ACTIVE
//
#define LED_0_MOTOR_NAME                    "LED0_MOTOR"
#define LED_0_MOTOR_PIN                     LED0_MOTOR_PIN
#define LED_0_MOTOR_ACTIVE                  LED0_MOTOR_ACTIVE
#define LED_0_MOTOR_INACTIVE                LED0_MOTOR_INACTIVE
#define LED0_MOTOR_GPIO                     LED0_MOTOR_PIN
#define LED0                                LED0_MOTOR_PIN

/** @} */

/** \name LED_ALARM definitions
 *  @{ */
#define LED1_ALARM_PIN                      PIN_PA27
#define LED1_ALARM_ACTIVE                   false
#define LED1_ALARM_INACTIVE                 !LED1_ALARM_ACTIVE
//
#define LED_1_ALARM_NAME                    "LED1_ALARM"
#define LED_1_ALARM_PIN                     LED1_ALARM_PIN
#define LED_1_ALARM_ACTIVE                  LED1_ALARM_ACTIVE
#define LED_1_ALARM_INACTIVE                LED1_ALARM_INACTIVE
#define LED1_ALARM_GPIO                     LED1_ALARM_PIN
#define LED1                                LED1_ALARM_PIN

/** @} */

#define LED_MCU_BLT_COUNT                   2

/** \name PIC_DATA definitions (OUTPUT)
 * \brief Send data to PIC using **GPIO** (Active High) When Reive Data from MCU
 *  @{ */
#define GPIO4_PIC_PIN                       PIN_PA22
#define GPIO4_PIC_ACTIVE                    false
#define GPIO4_PIC_INACTIVE                  !GPIO4_PIC_ACTIVE
//
#define GPIO_4_PIC_NAME                     "GPIO4_PIC"
#define GPIO_4_PIC_PIN                      GPIO4_PIC_PIN
#define GPIO_4_PIC_ACTIVE                   GPIO4_PIC_ACTIVE
#define GPIO_4_PIC_INACTIVE                 GPIO4_PIC_INACTIVE
#define GPIO_4_POC_GPIO                     GPIO4_PIC_PIN
#define GPIO4                               GPIO4_PIC_PIN
/** @} */

/** \name MOTOR_IA definitions (OUTPUT)
 * \brief Drive motor pin
 *  @{ */
#define GPIO5_IA_PIN                        PIN_PA24
#define GPIO5_IA_ACTIVE                     false
#define GPIO5_IA_INACTIVE                   !GPIO5_IA_ACTIVE
//
#define GPIO_5_IA_NAME                      "GPIO5_IA"
#define GPIO_5_IA_PIN                       GPIO5_IA_PIN
#define GPIO_5_IA_ACTIVE                    GPIO5_IA_ACTIVE
#define GPIO_5_IA_INACTIVE                  GPIO5_IA_INACTIVE
#define GPIO_5_IA_GPIO                      GPIO5_IA_PIN
#define GPIO5                               GPIO5_IA_PIN
/** @} */

/** \name MOTOR_IB definitions (OUTPUT)
 * \brief Drive motor pin
 *  @{ */
#define GPIO6_IB_PIN                        PIN_PA25
#define GPIO6_IB_ACTIVE                     false
#define GPIO6_IB_INACTIVE                   !GPIO6_IB_ACTIVE
//
#define GPIO_6_IB_NAME                      "GPIO5_IA"
#define GPIO_6_IB_PIN                       GPIO6_IB_PIN
#define GPIO_6_IB_ACTIVE                    GPIO6_IB_ACTIVE
#define GPIO_6_IB_INACTIVE                  GPIO6_IB_INACTIVE
#define GPIO_6_IB_GPIO                      GPIO6_IB_PIN
#define GPIO6                               GPIO6_IB_PIN
/** @} */

#define GPIO_OUT_MCU_BLT_COUNT              3

/** \name SW0 definitions (TEST_MOTOR)
 * \brief Switch for Counter Clock Wise Gear Motor 
 *  @{ */
#define SW0_TEST_MOTOR_PIN                  PIN_PA16
#define SW0_TEST_MOTOR_ACTIVE               false
#define SW0_TEST_MOTOR_INACTIVE             !SW0_TEST_MOTOR_ACTIVE
#define SW0_TEST_MOTOR_EIC_PIN              PIN_PA16
#define SW0_TEST_MOTOR_EIC_MUX              MUX_PA16A_EIC_EXTINT0
#define SW0_TEST_MOTOR_EIC_PINMUX           PINMUX_PA16A_EIC_EXTINT0
#define SW0_TEST_MOTOR_EIC_LINE             8
//
#define SWITCH_0_TEST_MOTOR_NAME            "SW0_TEST_MOTOR"
#define SWITCH_0_TEST_MOTOR_PIN             SW0_TEST_MOTOR_PIN
#define SWITCH_0_TEST_MOTOR_ACTIVE          SW0_TEST_MOTOR_ACTIVE
#define SWITCH_0_TEST_MOTOR_INACTIVE        SW0_TEST_MOTOR_INACTIVE
#define SWITCH_0_TEST_MOTOR_EIC_PIN         SW0_TEST_MOTOR_EIC_PIN
#define SWITCH_0_TEST_MOTOR_EIC_MUX         SW0_TEST_MOTOR_EIC_MUX
#define SWITCH_0_TEST_MOTOR_EIC_PINMUX      SW0_TEST_MOTOR_EIC_PINMUX
#define SWITCH_0_TEST_MOTOR_EIC_LINE        SW0_TEST_MOTOR_EIC_LINE

/** @} */

/** \name SW1 definitions (CW)
 * \brief Switch for Clock Wise Gear Motor 
 *  @{ */
#define SW1_CW_PIN                          PIN_PA18
#define SW1_CW_ACTIVE                       false
#define SW1_CW_INACTIVE                     !SW1_CW_ACTIVE
#define SW1_CW_EIC_PIN                      PIN_PA18
#define SW1_CW_EIC_MUX                      MUX_PA18A_EIC_EXTINT2
#define SW1_CW_EIC_PINMUX                   PINMUX_PA18A_EIC_EXTINT2
#define SW1_CW_EIC_LINE                     8
//
#define SWITCH_1_CW_NAME                    "SW1_CW_MOTOR"
#define SWITCH_1_CW_PIN                     SW1_CW_PIN
#define SWITCH_1_CW_ACTIVE                  SW1_CW_ACTIVE
#define SWITCH_1_CW_INACTIVE                SW1_CW_INACTIVE
#define SWITCH_1_CW_EIC_PIN                 SW1_CW_EIC_PIN
#define SWITCH_1_CW_EIC_MUX                 SW1_CW_EIC_MUX
#define SWITCH_1_CW_EIC_PINMUX              SW1_CW_EIC_PINMUX
#define SWITCH_1_CW_EIC_LINE                SW1_CW_EIC_LINE
/** @} */

/** \name SW2 definitions (CCW)
 * \brief Switch for Counter Clock Wise Gear Motor 
 *  @{ */
#define SW2_CCW_PIN                         PIN_PA19
#define SW2_CCW_ACTIVE                      false
#define SW2_CCW_INACTIVE                    !SW2_CCW_ACTIVE
#define SW2_CCW_EIC_PIN                     PIN_PA19
#define SW2_CCW_EIC_MUX                     MUX_PA19A_EIC_EXTINT3
#define SW2_CCW_EIC_PINMUX                  PINMUX_PA19A_EIC_EXTINT3
#define SW2_CCW_EIC_LINE                    8
//
#define SWITCH_2_CCW_NAME                   "SW2_CCW_MOTOR"
#define SWITCH_2_CCW_PIN                    SW2_CCW_PIN
#define SWITCH_2_CCW_ACTIVE                 SW2_CCW_ACTIVE
#define SWITCH_2_CCW_INACTIVE               SW2_CCW_INACTIVE
#define SWITCH_2_CCW_EIC_PIN                SW2_CCW_EIC_PIN
#define SWITCH_2_CCW_EIC_MUX                SW2_CCW_EIC_MUX
#define SWITCH_2_CCW_EIC_PINMUX             SW2_CCW_EIC_PINMUX
#define SWITCH_2_CCW_EIC_LINE               SW2_CCW_EIC_LINE
/** @} */

/** \name ALARM definitions (INPUT)
 * \brief Receive GPIO from open contract
 *  @{ */
#define GPIO7_ALARM_PIN                     PIN_PA22
#define GPIO7_ALARM_ACTIVE                  false
#define GPIO7_ALARM_INACTIVE                !GPIO7_ALARM_ACTIVE
#define GPIO7_ALARM_EIC_PIN                 PIN_PA22
#define GPIO7_ALARM_EIC_MUX                 MUX_PA22A_EIC_EXTINT6
#define GPIO7_ALARM_EIC_PINMUX              PINMUX_PA22A_EIC_EXTINT6
#define GPIO7_ALARM_EIC_LINE                8
//
#define GPIO_IN_7_ALARM_NAME                "GPIO7_INPUT_ALARM"
#define GPIO_IN_7_ALARM_PIN                 GPIO7_ALARM_PIN
#define GPIO_IN_7_ALARM_ACTIVE              GPIO7_ALARM_ACTIVE
#define GPIO_IN_7_ALARM_INACTIVE            GPIO7_ALARM_INACTIVE
#define GPIO_IN_7_ALARM_EIC_PIN             GPIO7_ALARM_EIC_PIN
#define GPIO_IN_7_ALARM_EIC_MUX             GPIO7_ALARM_EIC_MUX
#define GPIO_IN_7_ALARM_PINMUX              GPIO7_ALARM_EIC_PINMUX
#define GPIO_IN_7_ALARM_LINE                GPIO7_ALARM_EIC_LINE
/** @} */

#define GPIO_IN_MCU_BLT_COUNT               3

/** \name ADC definitions (ADC INPUT)
 * \brief Analog to Digital Converter for Photo Sensor
 *  @{ */
#define EXT1_ADC_MODULE                     ADC
#define EXT1_ADC_11_CHANNEL                 11
#define EXT1_ADC_11_PIN                     PIN_PB03B_ADC_AIN11
#define EXT1_ADC_11_MUX                     MUX_PB03B_ADC_AIN11
#define EXT1_ADC_11_PINMUX                  PINMUX_PB03B_ADC_AIN11
/** @} */

#define ADC_TOTAL_PIN                       1

#endif /* __ROLLING_EXIT_SIGN__ */

/** \name DIP1 definitions 
 * \brief Switch for Counter Clock Wise Gear Motor 
 *  @{ */
#define DIP1_PIN                            PIN_PB23
#define DIP1_ACTIVE                         false
#define DIP1_INACTIVE                       !DIP1_ACTIVE  
#define DIP1_EIC_PIN                        PIN_PB23
#define DIP1_EIC_MUX                        MUX_PB23A_EIC_EXTINT7
#define DIP1_EIC_PINMUX                     PINMUX_PB23A_EIC_EXTINT7
#define DIP1_EIC_LINE                       8
//
#define DIP_1_NAME                          "DIP1"
#define DIP_1_PIN                           DIP1_PIN
#define DIP_1_ACTIVE                        DIP1_ACTIVE
#define DIP_1_INACTIVE                      DIP1_INACTIVE
#define DIP_1_EIC_PIN                       DIP1_EIC_PIN
#define DIP_1_EIC_MUX                       DIP1_EIC_MUX
#define DIP_1_EIC_PINMUX                    DIP1_EIC_PINMUX
#define DIP_1_EIC_LINE                      DIP1_EIC_LINE
/** @} */

/** \name DIP2 definitions 
 * \brief Switch for Counter Clock Wise Gear Motor 
 *  @{ */
#define DIP2_PIN                            PIN_PA28
#define DIP2_ACTIVE                         false
#define DIP2_INACTIVE                       !DIP2_ACTIVE  
#define DIP2_EIC_PIN                        PIN_PA28
#define DIP2_EIC_MUX                        MUX_PA28A_EIC_EXTINT8
#define DIP2_EIC_PINMUX                     PINMUX_PA28A_EIC_EXTINT8
#define DIP2_EIC_LINE                       8
//
#define DIP_2_NAME                          "DIP2"
#define DIP_2_PIN                           DIP2_PIN
#define DIP_2_ACTIVE                        DIP2_ACTIVE
#define DIP_2_INACTIVE                      DIP2_INACTIVE
#define DIP_2_EIC_PIN                       DIP2_EIC_PIN
#define DIP_2_EIC_MUX                       DIP2_EIC_MUX
#define DIP_2_EIC_PINMUX                    DIP2_EIC_PINMUX
#define DIP_2_EIC_LINE                      DIP2_EIC_LINE
/** @} */

/** \name DIP4 definitions 
 * \brief Switch for Counter Clock Wise Gear Motor 
 *  @{ */
#define DIP4_PIN                            PIN_PA06
#define DIP4_ACTIVE                         false
#define DIP4_INACTIVE                       !DIP4_ACTIVE  
#define DIP4_EIC_PIN                        PIN_PA06
#define DIP4_EIC_MUX                        MUX_PA06A_EIC_EXTINT6
#define DIP4_EIC_PINMUX                     PINMUX_PA06A_EIC_EXTINT6
#define DIP4_EIC_LINE                       8
//
#define DIP_4_NAME                          "DIP4"
#define DIP_4_PIN                           DIP4_PIN
#define DIP_4_ACTIVE                        DIP4_ACTIVE
#define DIP_4_INACTIVE                      DIP4_INACTIVE
#define DIP_4_EIC_PIN                       DIP4_EIC_PIN
#define DIP_4_EIC_MUX                       DIP4_EIC_MUX
#define DIP_4_EIC_PINMUX                    DIP4_EIC_PINMUX
#define DIP_4_EIC_LINE                      DIP4_EIC_LINE
/** @} */

/** \name DIP8 definitions 
 * \brief Switch for Counter Clock Wise Gear Motor 
 *  @{ */
#define DIP8_PIN                            PIN_PA07
#define DIP8_ACTIVE                         false
#define DIP8_INACTIVE                       !DIP8_ACTIVE  
#define DIP8_EIC_PIN                        PIN_PA07
#define DIP8_EIC_MUX                        MUX_PA07A_EIC_EXTINT7
#define DIP8_EIC_PINMUX                     PINMUX_PA07A_EIC_EXTINT7
#define DIP8_EIC_LINE                       8
//
#define DIP_8_NAME                          "DIP8"
#define DIP_8_PIN                           DIP8_PIN
#define DIP_8_ACTIVE                        DIP8_ACTIVE
#define DIP_8_INACTIVE                      DIP8_INACTIVE
#define DIP_8_EIC_PIN                       DIP8_EIC_PIN
#define DIP_8_EIC_MUX                       DIP8_EIC_MUX
#define DIP_8_EIC_PINMUX                    DIP8_EIC_PINMUX
#define DIP_8_EIC_LINE                      DIP8_EIC_LINE
/** @} */

#ifdef __ROLLING_EXIT_SIGN__
    #define GPIO_INPUT                      3 + GPIO_IN_MCU_BLT_COUNT
#else 
    #define GPIO_INPUT                      3
#endif /* __ROLLING_EXIT_SIGN__ */

/** \name UART1 definitions 
 * \brief Universal Asyncronous for Monitoring
 *  @{ */
#define EXT1_UART_MODULE                    SERCOM0
#define EXT1_UART_SERCOM_MUX_SETTING        USART_RX_1_TX_0_XCK_1
#define EXT1_UART_SERCOM_PINMUX_PAD0        PINMUX_PA04D_SERCOM0_PAD0
#define EXT1_UART_SERCOM_PINMUX_PAD1        PINMUX_PA05D_SERCOM0_PAD1
#define EXT1_UART_SERCOM_PINMUX_PAD2        PINMUX_UNUSED
#define EXT1_UART_SERCOM_PINMUX_PAD3        PINMUX_UNUSED
#define EXT1_UART_SERCOM_DMAC_ID_TX         SERCOM0_DMAC_ID_TX
#define EXT1_UART_SERCOM_DMAC_ID_RX         SERCOM0_DMAC_ID_RX
//
#define EXT1_UART_SERCOM_PIN_PAD0           PIN_PA04D_SERCOM0_PAD0
#define EXT1_UART_SERCOM_PIN_PAD1           PIN_PA05D_SERCOM0_PAD1
/** @} */

/** \name AT24MAC402 48 bits (I2C)
 * @brief Inter-Integrated Circuit(I2C) for get EUI from AT24MAC402
 * \brief getting 48 bits value
 *  @{ */
#define EXT3_I2C_MODULE                     SERCOM1
#define EXT3_I2C_SERCOM_PIN_PAD0            PINMUX_PA16C_SERCOM1_PAD0
#define EXT3_I2C_SERCOM_PIN_PAD1            PINMUX_PA17C_SERCOM1_PAD1
#define EXT3_I2C_SERCOM_DMAC_ID_TX          SERCOM1_DMAC_ID_TX
#define EXT3_I2C_SERCOM_DMAC_ID_RX          SERCOM1_DMAC_ID_RX
//
#define EXT1_PIN_10                         PIN_PA15
#define GPIO8_EUI_PIN                       PIN_PA15
#define GPIO8_EUI_ACTIVE                    false
#define GPIO8_EUI_INACTIVE                  !GPIO8_EUI_ACTIVE
//
#define GPIO_8_EUI_NAME                     "GPIO8_EUI-AT24MAC204(48bits)"
#define GPIO_8_EUI_PIN                      GPIO8_EUI_PIN
#define GPIO_8_EUI_ACTIVE                   GPIO8_EUI_ACTIVE
#define GPIO_8_EUI_INACTIVE                 GPIO8_EUI_INACTIVE
#define GPIO_8_EUI_GPIO                     GPIO8_EUI_PIN
#define GPIO8                               GPIO8_EUI_PIN

/** @} */

/** \name RF SWITCH definitions 
 * \brief for drive SKY13373-460LF using SX1276 in ATSAMR34J18B(SiP) 
 *  @{ */
#define RF_SWITCH_PIN			        	PIN_PA13
#define RF_SWITCH_ACTIVE                    true
#define RF_SWITCH_INACTIVE	                !RF_SWITCH_ACTIVE
/** @} */

/** \name TCXO PWR Pin definitions 
 * \brief Crytal Osillator for SX1276
 *  @{ */
#define TCXO_PWR_PIN				        PIN_PA09
#define TCXO_PWR_ACTIVE                     true
#define TCXO_PWR_INACTIVE	                !TCXO_PWR_ACTIVE
/** @} */

/** \name SPI & External Interrupt(DIOx) 
 * \brief Communication with SX1276/77/78/79 from Semtech
 *  @{ */
#define SX_RF_SPI                           SERCOM4
#define SX_RF_RESET_PIN                     PIN_PB15
#define SX_RF_SPI_CS                        PIN_PB31
#define SX_RF_SPI_MOSI                      PIN_PB30
#define SX_RF_SPI_MISO                      PIN_PC19
#define SX_RF_SPI_SCK                       PIN_PC18
//
#define SX_RF_SPI_SERCOM_MUX_SETTING		SPI_SIGNAL_MUX_SETTING_E
#define SX_RF_SPI_SERCOM_PINMUX_PAD0		PINMUX_PC19F_SERCOM4_PAD0
#define SX_RF_SPI_SERCOM_PINMUX_PAD1		PINMUX_UNUSED
#define SX_RF_SPI_SERCOM_PINMUX_PAD2		PINMUX_PB30F_SERCOM4_PAD2
#define SX_RF_SPI_SERCOM_PINMUX_PAD3		PINMUX_PC18F_SERCOM4_PAD3
//
/** \name DIO0 Pin definitions 
 * \brief DIO0 Mapping
 * \note 00 : RxDone
 * \note 01  : TxDone
 * \note 10 : CadDone
 * \note 11 : -
 *  @{ */
#define DIO0_PIN                            PIN_PB16
#define DIO0_ACTIVE                         true
#define DIO0_INACTIVE                       !DIO0_ACTIVE
#define DIO0_EIC_PIN                        PIN_PB16A_EIC_EXTINT0
#define DIO0_EIC_MUX                        MUX_PB16A_EIC_EXTINT0
#define DIO0_EIC_PINMUX                     PINMUX_PB16A_EIC_EXTINT0
#define DIO0_EIC_LINE                       0     
//
/** \name DIO1 Pin definitions 
 * \brief DIO1 Mapping
 * \note 00  : RxTimeout
 * \note 01  : FhssChaneChannel
 * \note 10  : CadDetected
 * \note 11  : -
 *  @{ */
#define DIO1_PIN                            PIN_PA11
#define DIO1_ACTIVE                         true
#define DIO1_INACTIVE                       !DIO1_ACTIVE
#define DIO1_EIC_PIN                        PIN_PA11A_EIC_EXTINT11
#define DIO1_EIC_MUX                        MUX_PA11A_EIC_EXTINT11
#define DIO1_EIC_PINMUX                     PINMUX_PA11A_EIC_EXTINT11
#define DIO1_EIC_LINE                       11
//
/** \name DIO2 Pin definitions 
 * \brief DIO2 Mapping
 * \note 00  : FhssChangeChannel
 * \note 01  : FhssChangeChannel
 * \note 10  : FhssChangeChannel
 * \note 11  : -
 *  @{ */
#define DIO2_PIN                            PIN_PA12
#define DIO2_ACTIVE                         true
#define DIO2_INACTIVE                       !DIO2_ACTIVE
#define DIO2_EIC_PIN                        PIN_PA12A_EIC_EXTINT12
#define DIO2_EIC_MUX                        MUX_PA12A_EIC_EXTINT12
#define DIO2_EIC_PINMUX                     PINMUX_PA12A_EIC_EXTINT12
#define DIO2_EIC_LINE                       12
//
/** \name DIO3 Pin definitions 
 * \brief DIO3 Mapping
 * \note 00  : CadDone
 * \note 01  : ValidHeader
 * \note 10  : PayloadCrcError
 * \note 11  : -
 *  @{ */
#define DIO3_PIN                            PIN_PB17
#define DIO3_ACTIVE                         true
#define DIO3_INACTIVE                       !DIO3_ACTIVE
#define DIO3_EIC_PIN                        PIN_PB17A_EIC_EXTINT1
#define DIO3_EIC_MUX                        MUX_PB17A_EIC_EXTINT1
#define DIO3_EIC_PINMUX                     PINMUX_PB17A_EIC_EXTINT1
#define DIO3_EIC_LINE                       1
//
/** \name DIO4 Pin definitions 
 * \brief DIO4 Mapping
 * \note 00  : CadDetected
 * \note 01  : PIILock
 * \note 10  : PIILock
 * \note 11  : -
 *  @{ */
#define DIO4_PIN                            PIN_PA10
#define DIO4_ACTIVE                         true
#define DIO4_INACTIVE                       !DIO4_ACTIVE
#define DIO4_EIC_PIN                        PIN_PA10A_EIC_EXTINT10
#define DIO4_EIC_MUX                        MUX_PA10A_EIC_EXTINT10
#define DIO4_EIC_PINMUX                     PINMUX_PA10A_EIC_EXTINT10
#define DIO4_EIC_LINE                       10
//
/** \name DIO5 Pin definitions 
 * \brief DIO5 Mapping
 * \note 00  : ModeReady
 * \note 01  : ClkOut
 * \note 10  : ClkOut
 * \note 11  : -
 *  @{ */
#define DIO5_PIN                            PIN_PB00
#define DIO5_ACTIVE                         true
#define DIO5_INACTIVE                       !DIO5_ACTIVE
#define DIO5_EIC_PIN                        PIN_PB00A_EIC_EXTINT0
#define DIO5_EIC_MUX                        MUX_PB00A_EIC_EXTINT0
#define DIO5_EIC_PINMUX                     PINMUX_PB00A_EIC_EXTINT0
#define DIO5_EIC_LINE                       0
//
#define SX_RF_RESET_HIGH		            true
#define SX_RF_RESET_LOW		                !SX_RF_RESET_HIGH
/** @} */

/**
 * \brief Turns off the specified LEDs.
 *
 * \param led_gpio LED to turn off (LEDx_GPIO).
 *
 * \note The pins of the specified LEDs are set to GPIO output mode.
 */
#define LED_Off(led_gpio)                   port_pin_set_output_level(led_gpio,true)

/**
 * \brief Turns on the specified LEDs.
 *
 * \param led_gpio LED to turn on (LEDx_GPIO).
 *
 * \note The pins of the specified LEDs are set to GPIO output mode.
 */
#define LED_On(led_gpio)                    port_pin_set_output_level(led_gpio,false)

/**
 * \brief Toggles the specified LEDs.
 *
 * \param led_gpio LED to toggle (LEDx_GPIO).
 *
 * \note The pins of the specified LEDs are set to GPIO output mode.
 */
#define LED_Toggle(led_gpio)                port_pin_toggle_output_level(led_gpio)

/**
 * \brief High the specified GPIO pin.
 *
 * \param gpio_pin Set GPIO to High.
 *
 * \note The pins of the specified are set to GPIO output mode.
 */
#define GPIO_High(gpio_pin)                 port_pin_set_output_level(gpio_pin,false)

/**
 * \brief High the specified GPIO pin.
 *
 * \param gpio_pin Set GPIO to Low
 *
 * \note The pins of the specified are set to GPIO output mode.
 */
#define GPIO_Low(gpio_pin)                  port_pin_set_output_level(gpio_pin,true)

/**
 * \brief Toggle the specified GPIO pin.
 *
 * \param gpio_pin Toggle GPIO State
 *
 * \note The pins of the specified are set to GPIO output mode.
 */
#define GPIO_Toggle(gpio_pin)               port_pin_toggle_output_level(gpio_pin)

/** @} */

#ifdef __cplusplus
}
#endif

#endif // USER_BOARD_H
