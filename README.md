# LoRa P2P Broadcast Network for Smart Endpoints (Bare-Metal C)

*A robust, low-latency, point-to-multipoint communication framework built on ARM Cortex-M microcontrollers bypassing the standard LoRaWAN stack.*

---

## 📖 Project Overview

This project implements a custom **Point-to-Multipoint (Broadcast) LoRa architecture** tailored for industrial monitoring use-cases (e.g., Smart Exit Signs, emergency notifications, sensor networks). 

Instead of relying on the heavy overhead of a standard LoRaWAN stack, this system implements a direct **Peer-to-Peer (P2P)** protocol using bare-metal C on ATSAM microcontrollers. The architecture explicitly relies on a single, highly capable transmitter node that broadcasts command sequences to an theoretically unlimited number of "thin-client" receiver nodes listening on the same RF parameters.

### 🎯 Key Design Philosophy: "Stateless Edge Nodes"
A major design choice in this architecture is the simplicity of the receiver. The receiver is intentionally stateless and requires zero configuration to join the network. It simply powers on, enters a continuous listening state (`RADIO_STATE_RX`), and reacts strictly to broadcast payloads. This approach gives the architecture incredible scalability—adding 1 or 1,000 new receivers requires zero changes to the network or the transmitter.

---

## 🏗️ System Architecture

### 1. The Broadcaster (Transmitter Node)
The transmitter acts as the central command unit. It manages complex hardware peripherals to ensure reliable transmission. 
* **Dynamic EUI Provisioning:** Uses an I2C interface to communicate with an onboard external EEPROM (`AT24MAC402`) to extract a globally unique IEEE EUI address at runtime (`eeprom_eui_read`).
* **Non-Blocking Timers:** Utilizes hardware-backed Software Timers (`SwTimerCreate`) to periodically broadcast payloads (`:PR\r\n`) without halting the CPU.
* **AT-Command Interface:** Exposes a monitored USART interface with interrupt-driven callbacks for manual AT-command debugging and control.

### 2. The Listener (Receiver Node)
The receiver utilizes a thin-client, stateless listener pattern.
* **Aggressive State Management:** The microcontroller aggressively manages the transceiver state, immediately forcing the radio back into `RADIO_STATE_RX` (`RECEIVE_START`) if it ever falls out of the receive state.
* **Link Diagnostics:** Continually monitors payload metrics via `LORAWAN_EVT_RX_DATA_AVAILABLE`, actively polling and calculating RSSI and SNR for incoming bursts to gauge signal health.
* **Hardware Watchdog:** Uses a hardware Watchdog Timer (WDT) to ensure continuous uptime without human intervention.

---

## ⚙️ Technical Specifications & RF Configuration

Both the Broadcaster and Listeners are hardcoded to matching custom RF parameters, bypassing traditional MAC layers for raw speed and reliability:

* **Microcontroller:** ATSAM (ARM Cortex-M family)
* **Framework:** Atmel Software Framework (ASF)
* **RF Protocol:** Custom LoRa P2P (Bypassing LoRaWAN MAC/Join)
* **Frequency:** 923.2 MHz (Specifically optimized for the Thai ISM band)
* **Spreading Factor:** SF_12 (Maximum range and penetration over throughput)
* **Output Power:** 15 dBm (Boosted to 47 for long-range penetration)
* **Watchdog Timeout:** 60,000 ms

---

## 💻 Tech Stack & Drivers Used
* **Languages:** Bare-Metal C
* **Core Drivers:** Hardware USART, Inter-Integrated Circuit (I2C), Watchdog Timer (WDT), Event Interrupts (NVIC)
* **Radio/Crypto Layer:** Microchip Radio HAL, AES Hardware Engine, LoRa Physical Layer
* **Memory:** External MAC/EEPROM (`AT24MAC402`)

---

## 🚀 Getting Started

### Prerequisites 
- Microchip Studio / Atmel Studio
- ATSAM development boards equipped with a LoRa radio module.
- Atmel-ICE or EDBG cable for flashing.

### Firmware Flashing
1. Flash the **Receiver Firmware** (`receiver_loraoffline_atsam_sw`) onto one or more edge nodes. They will immediately boot, initialize the Watchdog, and enter continuous `RX` mode.
2. Flash the **Transmitter Firmware** (`transmitter_lora_offline_atsam_sw`) onto the central node. It will initialize the I2C EEPROM, extract its EUI, and begin broadcasting.
3. Once powered, observe the on-board LEDs on the receiver nodes or connect via USART at `19200 baud` to view real-time RSSI/SNR diagnostics.
