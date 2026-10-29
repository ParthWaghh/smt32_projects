# CAN Message Specification — Telemetry Bus

- **Protocol Version:** CAN 2.0A (Standard 11-bit Identifier)
- **Bus Speed:** 500 kbps (11 TQ Phase1, 6 TQ Phase2, 1 TQ Sync, Sample Point: 66.7%)
- **Byte Order:** Little Endian (Intel format)

---

## Message Definition: `0x100` — Engine Telemetry

**Transmitter:** Powertrain / Engine Control Node (STM32 Node)  
**Broadcast Period:** 100 ms (10 Hz)  
**DLC:** 7 Bytes  

| Byte Offset | Bit Field | Signal Name | Data Type | Range | Resolution / Scaling | Units | Description |
|:---:|:---:|:---|:---:|:---:|:---:|:---:|:---|
| 0–1 | 0–15 | `Engine_RPM` | `uint16_t` | 0 – 8,000 | 1 RPM / bit | RPM | Current crankshaft rotational speed |
| 2–3 | 16–31 | `Vehicle_Speed` | `uint16_t` | 0 – 2500 | 0.1 km/h / bit | km/h | Road speed (scaled by 10) |
| 4 | 32–39 | `Coolant_Temp` | `int8_t` | -40 – 125 | 1 °C / bit | °C | Engine coolant temperature |
| 5 | 40–47 | `Throttle_Pct` | `uint8_t` | 0 – 100 | 1 % / bit | % | Throttle pedal position |
| 6 | 48–55 | `System_State` | `uint8_t` | 0 – 3 | 1 / bit | enum | 0: Init, 1: Normal, 2: Warning, 3: Fault |

---

## Message Definition: `0x101` — Diagnostics & Faults

**Transmitter:** Sensor Node  
**Broadcast Period:** 500 ms (2 Hz) or Event-Driven  
**DLC:** 4 Bytes  

| Byte Offset | Signal Name | Data Type | Description |
|:---:|:---|:---:|:---|
| 0 | `Fault_Bitmask` | `uint8_t` | Bit 0: Over-temp, Bit 1: Over-speed, Bit 2: Sensor Open |
| 1 | `Bus_Off_Counter` | `uint8_t` | Number of recovered bus-off occurrences |
| 2–3 | `Supply_Voltage_mV` | `uint16_t` | Node operating rail in millivolts |
