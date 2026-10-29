#!/usr/bin/env python3
"""
generate_diagrams.py — Generates professional engineering demo diagrams
for docs/images/ (Project 01, 02, and 03).
"""

import matplotlib.pyplot as plt
import numpy as np
from pathlib import Path

# Style setup for professional embedded dark-mode diagrams
plt.style.use('dark_background')
out_dir = Path(__file__).resolve().parent.parent / "docs" / "images"
out_dir.mkdir(parents=True, exist_ok=True)

def generate_gpio_demo():
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 6), gridspec_kw={'height_ratios': [1, 1.2]})
    fig.patch.set_facecolor('#0f172a')
    ax1.set_facecolor('#1e293b')
    ax2.set_facecolor('#1e293b')

    # Subplot 1: Digital Logic Timing Diagram
    time = np.linspace(0, 10, 1000)
    # Button press from t=4 to t=8
    btn = np.where((time >= 4) & (time <= 8), 0, 1) # Active Low
    # Sequence mode (t < 4): cycling LEDs
    # Sync blink mode (4 <= t <= 8): all blinking together
    # Sequence mode (t > 8)
    led_r = np.zeros_like(time)
    led_y = np.zeros_like(time)
    led_g = np.zeros_like(time)

    for i, t in enumerate(time):
        if t < 4 or t > 8:
            cycle = (t % 1.5)
            if cycle < 0.5:
                led_r[i] = 1
            elif cycle < 1.0:
                led_y[i] = 1
            else:
                led_g[i] = 1
        else: # Synchronous 2 Hz blink
            blink = int((t - 4) * 4) % 2
            led_r[i] = blink
            led_y[i] = blink
            led_g[i] = blink

    ax1.plot(time, btn + 3.6, color='#38bdf8', lw=2, label='SW1 (PB0 Active-Low Input)')
    ax1.plot(time, led_r + 2.4, color='#ef4444', lw=2, label='LED Red (PA0)')
    ax1.plot(time, led_y + 1.2, color='#eab308', lw=2, label='LED Yellow (PA1)')
    ax1.plot(time, led_g + 0.0, color='#22c55e', lw=2, label='LED Green (PA2)')

    ax1.set_yticks([0.5, 1.7, 2.9, 4.1])
    ax1.set_yticklabels(['PA2 (Green)', 'PA1 (Yellow)', 'PA0 (Red)', 'PB0 (Button)'], fontsize=9, color='#94a3b8')
    ax1.set_xlabel('Time (seconds)', fontsize=9, color='#94a3b8')
    ax1.set_title('Project 01 — GPIO Waveforms & State Transition Analysis', fontsize=12, fontweight='bold', color='#f8fafc', pad=10)
    ax1.grid(True, linestyle='--', alpha=0.3, color='#475569')
    ax1.axvspan(4, 8, color='#38bdf8', alpha=0.1, label='Button Depressed (Synchronous Mode)')
    ax1.legend(loc='upper right', fontsize=8, facecolor='#1e293b', edgecolor='#475569')

    # Subplot 2: Hardware Block / Wiring Map
    ax2.axis('off')
    ax2.text(0.5, 0.85, 'STM32F103C8T6 Hardware Interfacing Architecture', 
             ha='center', fontsize=11, fontweight='bold', color='#38bdf8')
    
    blocks = [
        ('MCU Pin', 'Signal Name', 'Interface Circuit', 'Electrical Specs'),
        ('PA0 (Pin 10)', 'LED_RED', '330 Ω Current Limit → Red LED → GND', 'V_f = 1.9V, I_d = 4.2 mA'),
        ('PA1 (Pin 11)', 'LED_YELLOW', '330 Ω Current Limit → Yellow LED → GND', 'V_f = 2.0V, I_d = 3.9 mA'),
        ('PA2 (Pin 12)', 'LED_GREEN', '330 Ω Current Limit → Green LED → GND', 'V_f = 2.8V, I_d = 1.5 mA'),
        ('PB0 (Pin 18)', 'BTN_USER', 'SPST Tactile Switch to GND (Pull-Up)', 'Internal ~40 kΩ Pull-Up, active-low'),
        ('PC13 (Pin 2)', 'LED_STATUS', 'On-board Blue Pill Heartbeat Indicator', 'Active-low sink drive')
    ]
    
    y = 0.65
    for row in blocks:
        weight = 'bold' if y == 0.65 else 'normal'
        color = '#38bdf8' if y == 0.65 else '#e2e8f0'
        ax2.text(0.05, y, row[0], fontsize=9, fontweight=weight, color=color)
        ax2.text(0.25, y, row[1], fontsize=9, fontweight=weight, color=color)
        ax2.text(0.48, y, row[2], fontsize=9, fontweight=weight, color=color)
        ax2.text(0.85, y, row[3], fontsize=9, fontweight=weight, color=color)
        y -= 0.11

    plt.tight_layout()
    plt.savefig(out_dir / "project-01-gpio-demo.png", dpi=200, facecolor=fig.get_facecolor())
    plt.close()
    print("Generated project-01-gpio-demo.png")

def generate_adc_demo():
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(11, 5))
    fig.patch.set_facecolor('#0f172a')
    ax1.set_facecolor('#1e293b')
    ax2.set_facecolor('#1e293b')

    # Plot 1: 12-Bit SAR ADC Calibration Curve
    raw_ideal = np.linspace(0, 4095, 100)
    voltage_ideal = raw_ideal * (3300.0 / 4095.0)
    noise = np.random.normal(0, 8, 100)
    raw_measured = np.clip(raw_ideal + noise, 0, 4095)
    voltage_measured = raw_measured * (3300.0 / 4095.0)

    ax1.plot(voltage_ideal, raw_ideal, '--', color='#64748b', label='Ideal Transfer (12-bit)')
    ax1.scatter(voltage_measured[::4], raw_measured[::4], color='#38bdf8', s=25, label='Measured Samples (PA0 Pot)')
    ax1.set_title('12-Bit ADC Transfer Linearity ($V_{REF} = 3.3V$)', fontsize=11, fontweight='bold', color='#f8fafc')
    ax1.set_xlabel('Input Voltage (mV)', fontsize=9, color='#94a3b8')
    ax1.set_ylabel('Raw Digital Value (LSB: 0–4095)', fontsize=9, color='#94a3b8')
    ax1.grid(True, linestyle='--', alpha=0.3, color='#475569')
    ax1.legend(loc='lower right', fontsize=8, facecolor='#1e293b', edgecolor='#475569')

    # Plot 2: LM35 Temperature Calibration (10 mV / °C)
    temp = np.linspace(0, 100, 50)
    lm35_v = temp * 10.0 # mV
    ax2.plot(temp, lm35_v, color='#f59e0b', lw=2, label='LM35 Scaling: 10 mV / °C')
    ax2.axhline(600, color='#ef4444', linestyle=':', label='Warning Threshold (60°C / 600mV)')
    ax2.fill_between(temp, 0, 600, color='#22c55e', alpha=0.1, label='Nominal Operating Zone')
    ax2.fill_between(temp, 600, 1000, color='#ef4444', alpha=0.1)

    ax2.set_title('LM35 Analog Temperature Sensor Transfer', fontsize=11, fontweight='bold', color='#f8fafc')
    ax2.set_xlabel('Temperature (°C)', fontsize=9, color='#94a3b8')
    ax2.set_ylabel('Sensor Output Voltage (mV)', fontsize=9, color='#94a3b8')
    ax2.grid(True, linestyle='--', alpha=0.3, color='#475569')
    ax2.legend(loc='upper left', fontsize=8, facecolor='#1e293b', edgecolor='#475569')

    plt.suptitle('Project 02 — ADC Dual-Channel Transfer Characteristics & Conversion Calibration', 
                 fontsize=12, fontweight='bold', color='#38bdf8', y=0.98)
    plt.tight_layout()
    plt.savefig(out_dir / "project-02-adc-demo.png", dpi=200, facecolor=fig.get_facecolor())
    plt.close()
    print("Generated project-02-adc-demo.png")

def generate_pwm_demo():
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 6))
    fig.patch.set_facecolor('#0f172a')
    ax1.set_facecolor('#1e293b')
    ax2.set_facecolor('#1e293b')

    # Plot 1: Multi-Duty Oscilloscope Trace (1 kHz PWM, Period = 1.0 ms)
    t = np.linspace(0, 3.0, 3000) # 3 ms (3 periods)
    period = 1.0
    
    def pwm_wave(duty):
        return np.where((t % period) < (period * duty), 3.3, 0.0)

    ax1.plot(t, pwm_wave(0.25) + 8.0, color='#38bdf8', lw=1.8, label='25% Duty (CCR1 = 250)')
    ax1.plot(t, pwm_wave(0.50) + 4.0, color='#a855f7', lw=1.8, label='50% Duty (CCR1 = 500)')
    ax1.plot(t, pwm_wave(0.75) + 0.0, color='#22c55e', lw=1.8, label='75% Duty (CCR1 = 750)')

    ax1.set_yticks([1.65, 5.65, 9.65])
    ax1.set_yticklabels(['75% PWM', '50% PWM', '25% PWM'], fontsize=9, color='#94a3b8')
    ax1.set_xlabel('Time (milliseconds) [f = 1 kHz, T = 1.0 ms]', fontsize=9, color='#94a3b8')
    ax1.set_title('Project 03 — TIM2 PWM Output Waveforms at 1 kHz (Oscilloscope View)', fontsize=11, fontweight='bold', color='#f8fafc')
    ax1.grid(True, linestyle='--', alpha=0.3, color='#475569')
    ax1.legend(loc='upper right', fontsize=8, facecolor='#1e293b', edgecolor='#475569')

    # Plot 2: Transfer function ADC In -> PWM Duty Out
    adc_val = np.linspace(0, 4095, 100)
    duty_pct = (adc_val / 4095.0) * 100.0
    v_out_avg = (duty_pct / 100.0) * 3.3

    ax2.plot(adc_val, duty_pct, color='#38bdf8', lw=2, label='Duty Cycle (%)')
    ax2_r = ax2.twinx()
    ax2_r.plot(adc_val, v_out_avg, color='#f59e0b', linestyle='--', lw=2, label='Avg Output Voltage ($V_{AVG}$)')
    
    ax2.set_xlabel('ADC Raw Reading (Potentiometer on PA0)', fontsize=9, color='#94a3b8')
    ax2.set_ylabel('PWM Duty Cycle (%)', fontsize=9, color='#38bdf8')
    ax2_r.set_ylabel('Effective Filtered Output (V)', fontsize=9, color='#f59e0b')
    ax2.set_title('Closed-Loop Dimmer Transfer: ADC Input to PWM Output', fontsize=11, fontweight='bold', color='#f8fafc')
    ax2.grid(True, linestyle='--', alpha=0.3, color='#475569')

    plt.tight_layout()
    plt.savefig(out_dir / "project-03-pwm-demo.png", dpi=200, facecolor=fig.get_facecolor())
    plt.close()
    print("Generated project-03-pwm-demo.png")

if __name__ == "__main__":
    generate_gpio_demo()
    generate_adc_demo()
    generate_pwm_demo()
