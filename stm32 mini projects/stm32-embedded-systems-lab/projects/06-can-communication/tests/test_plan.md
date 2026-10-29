# Test Plan — Project 06: CAN Communication

## Test Setup

| Item | Specification |
|------|---------------|
| MCU | STM32F103C8T6 (bxCAN Peripheral) |
| Physical Layer | TJA1050 / MCP2551 Transceiver, 120 Ω termination resistors |
| Bitrate | 500 kbps (Calculated APB1 36 MHz / 4 / 18 TQ) |
| Validation Tools | Hardware Loopback, SocketCAN / Python-can vcan / Canable Analyzer |

---

## Test Cases

### TC-CAN-01: Bit Timing & Prescaler Verification
- **Objective:** Verify bit duration on bus equals 2.0 μs (1 / 500,000 baud).
- **Procedure:** Oscilloscope differential probe across CANH and CANL during continuous dominant bit transmission.
- **Expected Result:** Nominal bit time 2.000 μs ± 0.5%.

### TC-CAN-02: Silicon Loopback & FIFO Mailbox Ingestion
- **Objective:** Verify firmware driver sends frame from Tx mailbox and receives it in Rx FIFO0 without external bus connection.
- **Procedure:** Initialize driver in `CAN_OPERATING_MODE_LOOPBACK`. Dispatch frame `0x100`.
- **Expected Result:** Reception flag asserted; frame unpacked with zero bit mismatches.

### TC-CAN-03: Acceptance Filter Mask Discrimination
- **Objective:** Verify Filter Bank 0 rejects unauthorized CAN IDs at the silicon level.
- **Procedure:** Send frames with IDs `0x100` (allowed), `0x105` (allowed), and `0x250` (forbidden).
- **Expected Result:** IDs `0x100` and `0x105` accepted into FIFO0; ID `0x250` dropped silently with 0 CPU cycle consumption.

### TC-CAN-04: Bus-Off Recovery & Automatic Retransmission
- **Objective:** Validate behavior when CAN lines are temporarily shorted to GND or disconnected.
- **Procedure:** Short CANH to CANL for 500 ms while transmitting, then release short.
- **Expected Result:** TEC (Transmit Error Counter) increases, automatic retransmission triggers upon line restoration, and node clears bus-off state.
