#!/usr/bin/env python3
"""
parse_uart_log.py — Parse STM32 UART telemetry logs into CSV / tabular format.
Extracts: Timestamp, ADC Raw, Voltage (mV), Status.
"""

import sys
import re
import csv
from pathlib import Path

# Match lines like: [T:00006000] ADC0=2048 V=1650mV STATUS=OK
LOG_PATTERN = re.compile(
    r"^\[T:(?P<timestamp>\d+)\]\s+ADC0=(?P<raw>\d+)\s+V=(?P<voltage>\d+)mV\s+STATUS=(?P<status>\w+)"
)

def parse_log(input_file: Path, output_csv: Path = None):
    parsed_records = []
    
    with open(input_file, "r", encoding="utf-8", errors="ignore") as f:
        for line in f:
            match = LOG_PATTERN.search(line.strip())
            if match:
                record = {
                    "timestamp_ms": int(match.group("timestamp")),
                    "adc_raw": int(match.group("raw")),
                    "voltage_mv": int(match.group("voltage")),
                    "status": match.group("status")
                }
                parsed_records.append(record)
                
    print(f"Parsed {len(parsed_records)} valid telemetry frames from {input_file}")
    
    if output_csv and parsed_records:
        with open(output_csv, "w", newline="", encoding="utf-8") as out:
            writer = csv.DictWriter(out, fieldnames=["timestamp_ms", "adc_raw", "voltage_mv", "status"])
            writer.writeheader()
            writer.writerows(parsed_records)
        print(f"Exported clean dataset to {output_csv}")
        
    return parsed_records

if __name__ == "__main__":
    default_log = Path(__file__).resolve().parent.parent / "projects" / "04-uart-sensor-monitor" / "results" / "uart-log.txt"
    default_out = Path(__file__).resolve().parent / "test-results" / "uart_parsed.csv"
    
    log_path = Path(sys.argv[1]) if len(sys.argv) > 1 else default_log
    out_path = Path(sys.argv[2]) if len(sys.argv) > 2 else default_out
    
    if not log_path.exists():
        print(f"Error: input log {log_path} not found.")
        sys.exit(1)
        
    out_path.parent.mkdir(parents=True, exist_ok=True)
    records = parse_log(log_path, out_path)
    for r in records[:5]:
        print(f"  {r}")
