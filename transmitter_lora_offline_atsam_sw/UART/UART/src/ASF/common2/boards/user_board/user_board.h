/**
 * \file
 *
 * \brief User board definition template
 *
 */

 /* This file is intended to contain definitions and configuration details for
 * features and devices that are available on the board, e.g., frequency and
 * startup time for an external crystal, external memory devices, LED and USART
 * pins.
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#ifndef USER_BOARD_H
#define USER_BOARD_H

#include <conf_board.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \ingroup group_common_boards
 * \defgroup user_board_group User board
 *
 * @{
 */

void system_board_init(void);

/** Name string macro */
#define BOARD_NAME                "SUNNY PROTOTYPE BOARD"

/** \name Resonator definitions
 *  @{ */
#define BOARD_FREQ_SLCK_XTAL      (32768U)
#define BOARD_FREQ_SLCK_BYPASS    (32768U)
#define BOARD_FREQ_MAINCK_XTAL    0 /* Not Mounted */
#define BOARD_FREQ_MAINCK_BYPASS  0 /* Not Mounted */
#define BOARD_MCK                 CHIP_FREQ_CPU_MAX
#define BOARD_OSC_STARTUP_US      15625
/** @} */

/** \name LED0 definitions
 *  @{ */
#define LED0_PIN                  PIN_PA19
#define LED0_ACTIVE               false
#define LED0_INACTIVE             !LED0_ACTIVE
/** @} */

/** \name LED1 definitions
 *  @{ */
#define LED1_PIN                  PIN_PA18
#define LED1_ACTIVE               false
#define LED1_INACTIVE             !LED1_ACTIVE
/** @} */

/** \name LED2 definitions
 *  @{ */
#define LED2_PIN                  PIN_PA07
#define LED2_ACTIVE               false
#define LED2_INACTIVE             !LED2_ACTIVE
/** @} */

/** \name SW0 definitions
 *  @{ */
#define SW0_PIN                   PIN_PA15
#define SW0_ACTIVE                false
#define SW0_INACTIVE              !SW0_ACTIVE
#define SW0_EIC_PIN               PIN_PA15
#define SW0_EIC_MUX               MUX_PB15A_EIC_EXTINT15
#define SW0_EIC_PINMUX            PINMUX_PB15A_EIC_EXTINT15
#define SW0_EIC_LINE              8
/** @} */

/** \name SW1 definitions
 *  @{ */
#define SW1_PIN                   PIN_PA22
#define SW1_ACTIVE                false
#define SW1_INACTIVE              !SW1_ACTIVE
#define SW1_EIC_PIN               PIN_PA22
#define SW1_EIC_MUX               MUX_PA22A_EIC_EXTINT6
#define SW1_EIC_PINMUX            PINMUX_PA22A_EIC_EXTINT6
#define SW1_EIC_LINE              8
/** @} */

/**
 * \name LED #0 definitions
 *
 * Wrapper macros for LED0, to ensure common naming across all Xplained Pro
 * boards.
 *
 *  @{ */
#define LED_0_NAME                "LED0 (yellow)"
#define LED_0_PIN                 LED0_PIN
#define LED_0_ACTIVE              LED0_ACTIVE
#define LED_0_INACTIVE            LED0_INACTIVE
#define LED0_GPIO                 LED0_PIN
#define LED0                      LED0_PIN

//#define LED_0_PWM3CTRL_MODULE     TCC0
//#define LED_0_PWM3CTRL_CHANNEL    3
//#define LED_0_PWM3CTRL_OUTPUT     3
//#define LED_0_PWM3CTRL_PIN        PIN_PA19F_TCC0_WO3
//#define LED_0_PWM3CTRL_MUX        MUX_PA19F_TCC0_WO3
//#define LED_0_PWM3CTRL_PINMUX     PINMUX_PA19F_TCC0_WO3
/** @} */

/**
 * \name LED #1 definitions
 *
 * Wrapper macros for LED1, to ensure common naming across all Xplained Pro
 * boards.
 *
 *  @{ */
#define LED_1_NAME                "LED1 (Green)"
#define LED_1_PIN                 LED1_PIN
#define LED_1_ACTIVE              LED1_ACTIVE
#define LED_1_INACTIVE            LED1_INACTIVE
#define LED1_GPIO                 LED1_PIN
#define LED1                      LED1_PIN

//#define LED_1_PWM2CTRL_MODULE     TCC0
//#define LED_1_PWM2CTRL_CHANNEL    2
//#define LED_1_PWM2CTRL_OUTPUT     2
//#define LED_1_PWM2CTRL_PIN        PINMUX_PA18F_TCC0_WO2
//#define LED_1_PWM2CTRL_MUX        MUX_PA18F_TCC0_WO2
//#define LED_1_PWM2CTRL_PINMUX     PINMUX_PA18F_TCC0_WO2
/** @} */

/** Number of on-board LEDs */

/**
 * \name LED #2 definitions
 *
 * Wrapper macros for LED1, to ensure common naming across all Xplained Pro
 * boards.
 *
 *  @{ */
#define LED_2_NAME                "LED2 (Green)"
#define LED_2_PIN                 LED2_PIN
#define LED_2_ACTIVE              LED2_ACTIVE
#define LED_2_INACTIVE            LED2_INACTIVE
#define LED2_GPIO                 LED2_PIN
#define LED2                      LED2_PIN

//#define LED_1_PWM2CTRL_MODULE     TCC0
//#define LED_1_PWM2CTRL_CHANNEL    2
//#define LED_1_PWM2CTRL_OUTPUT     2
//#define LED_1_PWM2CTRL_PIN        PINMUX_PA18F_TCC0_WO2
//#define LED_1_PWM2CTRL_MUX        MUX_PA18F_TCC0_WO2
//#define LED_1_PWM2CTRL_PINMUX     PINMUX_PA18F_TCC0_WO2
/** @} */

/** Number of on-board LEDs */
#define LED_COUNT                 3

/**
 * \name Button #0 definitions
 *
 * Wrapper macros for SW0, to ensure common naming across all Xplained Pro
 * boards.
 *
 *  @{ */
#define BUTTON_0_NAME             "SW0"
#define BUTTON_0_PIN              SW0_PIN
#define BUTTON_0_ACTIVE           SW0_ACTIVE
#define BUTTON_0_INACTIVE         SW0_INACTIVE
#define BUTTON_0_EIC_PIN          SW0_EIC_PIN
#define BUTTON_0_EIC_MUX          SW0_EIC_MUX
#define BUTTON_0_EIC_PINMUX       SW0_EIC_PINMUX
#define BUTTON_0_EIC_LINE         SW0_EIC_LINE
/** @} */

/**
 * \name Button #1 definitions
 *
 * Wrapper macros for SW0, to ensure common naming across all Xplained Pro
 * boards.
 *
 *  @{ */
#define BUTTON_1_NAME             "SW1"
#define BUTTON_1_PIN              SW1_PIN
#define BUTTON_1_ACTIVE           SW1_ACTIVE
#define BUTTON_1_INACTIVE         SW1_INACTIVE
#define BUTTON_1_EIC_PIN          SW1_EIC_PIN
#define BUTTON_1_EIC_MUX          SW1_EIC_MUX
#define BUTTON_1_EIC_PINMUX       SW1_EIC_PINMUX
#define BUTTON_1_EIC_LINE         SW1_EIC_LINE
/** @} */

/** \name TCXO PWR Pin definitions
 *  @{ */
#define TCXO_PWR_PIN				PIN_PA09
#define TCXO_PWR_ACTIVE             true
#define TCXO_PWR_INACTIVE	        !TCXO_PWR_ACTIVE
/** @} */

/** \name Extension header #1 UART definitions
 *  @{
 */
#define EXT1_UART_MODULE              SERCOM0
#define EXT1_UART_SERCOM_MUX_SETTING  USART_RX_0_TX_0_XCK_1
#define EXT1_UART_SERCOM_PINMUX_PAD0  PINMUX_PA04D_SERCOM0_PAD0
#define EXT1_UART_SERCOM_PINMUX_PAD1  PINMUX_PA05D_SERCOM0_PAD1
#define EXT1_UART_SERCOM_PINMUX_PAD2  PINMUX_UNUSED
#define EXT1_UART_SERCOM_PINMUX_PAD3  PINMUX_UNUSED
#define EXT1_UART_SERCOM_DMAC_ID_TX   SERCOM0_DMAC_ID_TX
#define EXT1_UART_SERCOM_DMAC_ID_RX   SERCOM0_DMAC_ID_RX

#define EXT1_UART_SERCOM_PIN_PAD0     PIN_PA04D_SERCOM0_PAD0
#define EXT1_UART_SERCOM_PIN_PAD1     PIN_PA05D_SERCOM0_PAD1
/** @} */

/** @} */

/**
 * \brief Turns off the specified LEDs.
 *
 * \param led_gpio LED to turn off (LEDx_GPIO).
 *
 * \note The pins of the specified LEDs are set to GPIO output mode.
 */
#define LED_Off(led_gpio)     port_pin_set_output_level(led_gpio,true)

/**
 * \brief Turns on the specified LEDs.
 *
 * \param led_gpio LED to turn on (LEDx_GPIO).
 *
 * \note The pins of the specified LEDs are set to GPIO output mode.
 */
#define LED_On(led_gpio)      port_pin_set_output_level(led_gpio,false)

/**
 * \brief Toggles the specified LEDs.
 *
 * \param led_gpio LED to toggle (LEDx_GPIO).
 *
 * \note The pins of the specified LEDs are set to GPIO output mode.
 */
#define LED_Toggle(led_gpio)  port_pin_toggle_output_level(led_gpio)


/** @} */

#ifdef __cplusplus
}
#endif

#endif // USER_BOARD_H
