# Virtual CAN & Simulation Guide

## Why Simulation is Different for CAN

Unlike GPIO, ADC, and UART, circuit simulation tools (e.g., standard Proteus SPICE models for STM32F103) **do not emulate the internal bxCAN silicon engine**. Attempting to connect virtual transceivers in Proteus will not generate valid CAN frames because the peripheral registers are non-functional in generic models.

## Simulation & Verification Alternatives

To ensure reproducible firmware testing without requiring two physical STM32 boards and transceivers, this project includes two industry-standard verification pathways:

---

### Pathway 1: STM32 Internal Silicon Loopback Mode

The STM32 bxCAN peripheral features an integrated internal test loopback mode:
- Transmit mailbox output is looped directly into the receive FIFO input internally inside the silicon.
- External CAN_TX and CAN_RX pins remain recessive, preventing bus collisions.
- Full filter bank matching, interrupt triggering, and payload unpacking can be validated natively on a single Blue Pill board via ST-Link.

**Configuration in `main.c`:**
```c
can_driver_init(CAN_OPERATING_MODE_LOOPBACK);
```

---

### Pathway 2: Virtual CAN Bus (SocketCAN / Python Testbench)

For host PC simulation (Linux / WSL or Windows with `python-can` virtual bus):

1. **Virtual CAN Interface Setup (Linux / macOS):**
   ```bash
   sudo ip link add dev vcan0 type vcan
   sudo ip link set up vcan0
   ```

2. **Running the Python Protocol Testbench:**
   Run the included `virtual_can_testbench.py` script:
   ```bash
   python virtual_can_testbench.py
   ```
   This script verifies the exact byte packing and Little Endian bit alignment defined in [`can-message-spec.md`](../docs/can-message-spec.md).
