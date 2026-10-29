/**
 * @file    can_protocol.c
 * @brief   CAN payload packing and endian-safe bit unpacking.
 * @author  Parth Wagh
 */

#include "can_protocol.h"
#include <string.h>

void can_protocol_pack_telemetry(can_frame_t *frame, const vehicle_telemetry_t *telemetry)
{
    frame->id = CAN_ID_ENGINE_TELEMETRY;
    frame->is_extended = false;
    frame->is_rtr = false;
    frame->dlc = 7;

    /* Byte 0-1: Engine RPM (Little Endian) */
    frame->data[0] = (uint8_t)(telemetry->engine_rpm & 0xFF);
    frame->data[1] = (uint8_t)((telemetry->engine_rpm >> 8) & 0xFF);

    /* Byte 2-3: Vehicle Speed x10 (Little Endian) */
    frame->data[2] = (uint8_t)(telemetry->vehicle_speed & 0xFF);
    frame->data[3] = (uint8_t)((telemetry->vehicle_speed >> 8) & 0xFF);

    /* Byte 4: Coolant Temperature (signed) */
    frame->data[4] = (uint8_t)telemetry->coolant_temp_c;

    /* Byte 5: Throttle percentage */
    frame->data[5] = telemetry->throttle_pct;

    /* Byte 6: System state */
    frame->data[6] = telemetry->system_state;

    frame->data[7] = 0x00;
}

bool can_protocol_unpack_telemetry(const can_frame_t *frame, vehicle_telemetry_t *telemetry)
{
    if (frame->id != CAN_ID_ENGINE_TELEMETRY || frame->dlc < 7) {
        return false;
    }

    telemetry->engine_rpm = (uint16_t)(frame->data[0] | ((uint16_t)frame->data[1] << 8));
    telemetry->vehicle_speed = (uint16_t)(frame->data[2] | ((uint16_t)frame->data[3] << 8));
    telemetry->coolant_temp_c = (int8_t)frame->data[4];
    telemetry->throttle_pct = frame->data[5];
    telemetry->system_state = frame->data[6];

    return true;
}
