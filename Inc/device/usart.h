/*
 * usart.h
 *
 *  Created on: 2017年8月10日
 *      Author: redchenjs
 */

#ifndef DEVICE_USART_H_
#define DEVICE_USART_H_

#include "stdint.h"

#define RING_BUFFER_SIZE                         (1024 * 2)

typedef struct {
  uint8_t  data[RING_BUFFER_SIZE];
  uint16_t tail;
  uint16_t head;
} ring_buffer_t;

extern void usart3_receive_init(void);

#endif /* DEVICE_USART_H_ */
