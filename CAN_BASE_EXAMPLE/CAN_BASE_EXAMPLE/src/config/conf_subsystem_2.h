/*
 * conf_subsystem_2.h
 *
 * Created: 22/10/2022 21:55:10
 *  Author: Mel
 */ 


#ifndef CONF_SUBSYSTEM_2_H_
#define CONF_SUBSYSTEM_2_H_

/* CAN ID */
#define SS2_CAN_ID 0x46A

/* CAN commands IDs */
#define SS2_CMD_ID_TURN_ON_LED    0x0
#define SS2_CMD_ID_TURN_OFF_LED   0x1
#define SS2_CMD_ID_GET_LED_STATUS 0x2
#define SS2_CMD_ID_GET_TEMP       0x3

#endif /* CONF_SUBSYSTEM_2_H_ */