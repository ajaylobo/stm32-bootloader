/*
 * ring_buffer.h
 *
 *  Created on: 05-Aug-2026
 *      Author: ajayd
 */

#ifndef RINGBUFFER_RING_BUFFER_H_
#define RINGBUFFER_RING_BUFFER_H_

#include <stdint.h>
#include <stdbool.h>

#define RING_BUFFER_SIZE	(256U)


typedef struct {
	uint8_t buffer[RING_BUFFER_SIZE];
	uint8_t head;
	uint8_t tail;
	uint16_t count;

}RingBuffer_t;

void RingBuffer_Init(RingBuffer_t *rb);
bool RingBuffer_Push(RingBuffer_t *rb, uint8_t byte);
bool RingBuffer_Pop(RingBuffer_t *rb, uint8_t *byte);
bool RingBuffer_IsEmpty(const RingBuffer_t *rb);
bool RingBuffer_IsFull(const RingBuffer_t *rb);


#endif /* RINGBUFFER_RING_BUFFER_H_ */
