/**
 * @file    can_protocol.h
 * @brief   Application layer CAN protocol specification and frame serialization.
 * @author  Parth Wagh
 */

#ifndef _CAN_PROTOCOL_H_
#define _CAN_PROTOCOL_H_

#include "can_driver.h"

/* Standard Message Identifiers */
#define CAN_ID_ENGINE_TELEMETRY   0x100U
#define CAN_ID_SYSTEM_STATUS      0x101U
#define CAN_ID_COMMAND_REQUEST    0x200U

/* Telemetry Payload Structure */
typedef struct {
    uint16_t engine_rpm;      /* 0 to 8000 RPM (1 RPM/bit) */
    uint16_t vehicle_speed;   /* 0 to 250 km/h (0.1 km/h resolution, scaled x10) */
    int8_t   coolant_temp_c;  /* -40 to 125 °C */
    uint8_t  throttle_pct;    /* 0 to 100 % */
    uint8_t  system_state;    /* Current operational state flag */
} vehicle_telemetry_t;

/* Public API */
void can_protocol_pack_telemetry(can_frame_t *frame, const vehicle_telemetry_t *telemetry);
bool can_protocol_unpack_telemetry(const can_frame_t *frame, vehicle_telemetry_t *telemetry);

#endif /* _CAN_PROTOCOL_H_ */
