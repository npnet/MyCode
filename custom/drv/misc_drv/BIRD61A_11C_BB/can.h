#ifndef CAN_H
#define CAN_H


#define CAN_DEBUG   0

#define CAN1_EINT_GPIO_NUM   20
#define CAN2_EINT_GPIO_NUM   14

void can_init();
extern void can_start(void);
extern void can2_hw_rx(void);
extern void can1_hw_rx(void);
#endif
