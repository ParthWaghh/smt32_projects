#!/usr/bin/env python3
"""
can_frame_decoder.py — Parse raw CAN trace files and extract decoded physical signals.
"""

import sys
import struct
from pathlib import Path

def decode_can_log(log_path: Path):
    print(f"Decoding CAN trace from: {log_path.name}")
    print(f"{'Time(s)':<10} {'ID':<8} {'DLC':<4} {'Engine RPM':<12} {'Speed (km/h)':<14} {'Coolant(C)':<12} {'Throttle':<10}")
    print("-" * 72)
    
    with open(log_path, "r", encoding="utf-8") as f:
        for line in f:
            if "CAN1 Rx" in line and "0x100" in line:
                parts = line.strip().split()
                try:
                    time_str = parts[0]
                    can_id = parts[3]
                    dlc = int(parts[4])
                    payload_bytes = [int(b, 16) for b in parts[5:5+dlc]]
                    raw = bytes(payload_bytes)
                    rpm, speed_x10, temp, throttle, state = struct.unpack("<HHbBB", raw[:7])
                    speed = speed_x10 / 10.0
                    print(f"{time_str:<10} {can_id:<8} {dlc:<4} {rpm:<12} {speed:<14.1f} {temp:<12} {throttle}%")
                except Exception as e:
                    continue

if __name__ == "__main__":
    default_log = Path(__file__).resolve().parent.parent / "projects" / "06-can-communication" / "results" / "can-log.txt"
    log_file = Path(sys.argv[1]) if len(sys.argv) > 1 else default_log
    
    if not log_file.exists():
        print(f"Error: {log_file} does not exist.")
        sys.exit(1)
        
    decode_can_log(log_file)
