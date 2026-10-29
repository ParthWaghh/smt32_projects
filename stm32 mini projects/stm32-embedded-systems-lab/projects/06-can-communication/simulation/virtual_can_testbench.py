#!/usr/bin/env python3
"""
Virtual CAN Testbench & Protocol Validator
Validates frame serialization according to docs/can-message-spec.md.
"""

import struct

def pack_telemetry(rpm: int, speed_x10: int, coolant_temp: int, throttle: int, state: int) -> bytes:
    """Pack telemetry according to CAN ID 0x100 specification."""
    return struct.pack("<HHbBB", rpm, speed_x10, coolant_temp, throttle, state)

def unpack_telemetry(data: bytes):
    """Unpack raw 7-byte CAN payload."""
    rpm, speed_x10, temp, throttle, state = struct.unpack("<HHbBB", data)
    return {
        "engine_rpm": rpm,
        "vehicle_speed_kmh": speed_x10 / 10.0,
        "coolant_temp_c": temp,
        "throttle_pct": throttle,
        "system_state": state
    }

def main():
    print("=== STM32 CAN Telemetry Protocol Self-Test ===")
    test_data = pack_telemetry(rpm=3500, speed_x10=725, coolant_temp=85, throttle=55, state=1)
    print(f"Serialized Payload (7 bytes): {test_data.hex(' ').upper()}")
    
    decoded = unpack_telemetry(test_data)
    print("Decoded Dictionary:")
    for k, v in decoded.items():
        print(f"  {k}: {v}")

    assert decoded["engine_rpm"] == 3500
    assert decoded["vehicle_speed_kmh"] == 72.5
    assert decoded["coolant_temp_c"] == 85
    assert decoded["throttle_pct"] == 55
    assert decoded["system_state"] == 1
    print("\n[SUCCESS] Bitwise protocol compatibility check passed.")

if __name__ == "__main__":
    main()
