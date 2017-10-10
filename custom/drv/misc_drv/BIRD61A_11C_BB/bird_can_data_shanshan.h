
#ifndef BIRD_CAN_DATA_SHANSHAN_H
#define BIRD_CAN_DATA_SHANSHAN_H

#define SHANSHAN_CAN_DEBUG  0

#define CAN_SINGLE_UNIT_NUM 20
#define CAN_VOLTAGE_NUM 21
#define CAN_VOLTAGE_UNIT_LENGTH 13
#define CAN_VOLTAGE_DATA_LENGTH 3
#define CAN_TEMPERATURE_NUM 34
#define CAN_TEMPERATURE_UNIT_LENGTH 2
#define CAN_TEMPERATURE_DATA_LENGTH 6


void can_table_create_shanshan(void);
void can_rx_data_check_shanshan();
void can_data_shanshan_init(void);

#endif
