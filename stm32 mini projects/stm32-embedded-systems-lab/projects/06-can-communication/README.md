# Project 06 — CAN Communication

## 1. Objective

Implement Controller Area Network (CAN 2.0A/B) communication on the STM32F103C8T6. This project demonstrates automotive/industrial bus communication including frame formatting (Standard 11-bit and Extended 29-bit identifiers), hardware acceptance filter banks, mailboxes, and robust transmission/reception timeout handling.

Because common circuit simulators (such as Proteus Blue Pill models) lack native peripheral simulation of the STM32 bxCAN controller, this project provides both:
1. **Bare-metal bxCAN hardware driver** for physical transceivers (MCP2551 / TJA1050).
2. **Loopback & Virtual CAN test environment** documented in [`simulation/`](simulation/) for complete verification without physical multi-node bench setups.

## 2. Hardware and Tools

| Item | Detail |
|------|--------|
| **MCU** | STM32F103C8T6 (Blue Pill, 72 MHz, bxCAN peripheral) |
| **CAN Transceiver** | MCP2551 or TJA1050 (High-Speed CAN Transceiver, 5 V / 3.3 V level-shifted) |
| **CAN Bus Pins** | PA11 (CAN_RX) and PA12 (CAN_TX) |
| **Termination** | 120 Ω bus termination resistors at both line ends |
| **Tools** | STM32CubeIDE 1.14+, USB-to-CAN analyzer (Canable / Candlelight / PCAN) |
| **Bus Bitrate** | 500 kbps (Standard Automotive Diagnostic / Powertrain Rate) |

## 3. Concepts Demonstrated

- **bxCAN Peripheral Configuration:** Synchronous bit timing (Sync Segment, Prop Segment, Phase Seg 1, Phase Seg 2, SJW).
- **Bitrate Prescaling:** 500 kbps calculation derived from APB1 peripheral clock (36 MHz).
- **Hardware Acceptance Filtering:** 14 filter banks, 32-bit and 16-bit mask/list mode to filter specific CAN IDs at the silicon level.
- **Transmit Mailbox Management:** Managing Mailboxes 0, 1, 2, avoiding transmit queue priority inversion.
- **CAN Protocol Layer:** Payload encoding/decoding of vehicle speed, engine RPM, and temperature telemetry packets.

## 4. Circuit Diagram and Bus Topology

High-speed physical CAN bus topology:

```
[ STM32 Node 1 ]               [ STM32 Node 2 / Bus Analyzer ]
 PA12 (TX) ──► TXD                TXD ◄── PA12 (TX)
 PA11 (RX) ◄── RXD                RXD ──► PA11 (RX)
  [TJA1050/MCP2551]               [TJA1050/MCP2551]
     CANH ──────────── CANH Line ──────────── CANH
      │                 (120 Ω)                 │
     CANL ──────────── CANL Line ──────────── CANL
```

- **CANH / CANL Differential Pair:** Twisted pair wire for common-mode noise rejection.
- **120 Ω Resistor:** Placed between CANH and CANL across the differential pair at each physical terminus.

## 5. Firmware Architecture

```
main.c
  ├── can_driver_init(CAN_MODE_NORMAL, 500000)
  ├── can_driver_config_filter(FILTER_ID, MASK)
  └── Loop:
       ├── can_protocol_pack_telemetry(&msg, rpm, speed, temp)
       ├── can_driver_transmit(&msg)
       ├── can_driver_receive_poll(&rx_msg)
       └── can_protocol_unpack(&rx_msg)
```

## 6. Implementation

### Bit Timing Calculation for 500 kbps:
- APB1 Clock = 36 MHz
- Target Baudrate = 500,000 bps
- Total Time Quanta ($TQ$) = 18 ($1 \text{ Sync} + 11 \text{ BS1} + 6 \text{ BS2}$)
- Prescaler ($BRP$) = $\frac{36\,\text{MHz}}{18 \times 500\,\text{kHz}} = 4$
- Sample Point = $\frac{1 + 11}{18} \times 100\% = 66.7\%$ (within standard CAN specification)

## 7. Testing

See detailed test steps in [`tests/test_plan.md`](tests/test_plan.md).
- **Hardware Loopback Test:** Internal silicon loopback without external transceivers to verify software stack integrity.
- **Differential Bus Signal Integrity:** Verification of recessive (2.5 V) and dominant (CANH ~3.5 V, CANL ~1.5 V) levels.
- **ID Filter Rejection:** Verifying out-of-spec frames are discarded by hardware without CPU interrupt overhead.

## 8. Results

Captured CAN message traces and logic analyzer frames are logged in [`results/can-log.txt`](results/can-log.txt).

## 9. Limitations

- STM32F103 shares USB and CAN memory buffers; USB and bxCAN cannot be clocked simultaneously on basic F103 lines.
- Simulation tools like standard Proteus Blue Pill lack native bxCAN silicon emulation, necessitating hardware or virtual CAN loopback testbeds.

## 10. Future Improvements

- Implement CANopen or J1939 higher-layer protocol stack.
- Support CAN-FD (Flexible Datarate) using external SPI controllers (MCP2518FD) or STM32G4 / STM32H7 families.

## 11. References

- [STM32F103 Reference Manual (RM0008) — bxCAN Chapter](https://www.st.com/resource/en/reference_manual/cd00171190.pdf)
- [ISO 11898-1: Road vehicles — Controller area network](https://www.iso.org/standard/63648.html)
- [CiA 301 CANopen Application Layer](https://www.can-cia.org/)
