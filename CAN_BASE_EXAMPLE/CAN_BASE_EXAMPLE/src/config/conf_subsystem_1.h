/*
 * conf_subsystem_1.h
 *
 * Created: 22/10/2022 21:54:52
 *  Author: Mel
 */ 


#ifndef CONF_SUBSYSTEM_1_H_
#define CONF_SUBSYSTEM_1_H_

/* CAN ID */
#define SS1_CAN_ID 0x45A
#define SELF_CAN_ID 0x47A

/* CAN commands IDs */
#define SS1_CMD_ID_TURN_ON_LED    0x0
#define SS1_CMD_ID_TURN_OFF_LED   0x1
#define SS1_CMD_ID_GET_LED_STATUS 0x2
#define SS1_CMD_ID_GET_TEMP       0x3

#endif /* CONF_SUBSYSTEM_1_H_ */