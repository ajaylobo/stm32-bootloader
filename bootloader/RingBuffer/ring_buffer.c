/*
 * ring_buffer.c
 *
 *  Created on: 05-Aug-2026
 *      Author: ajayd
 */


#include "ring_buffer.h"

void RingBuffer_Init(RingBuffer_t *rb)
{
	rb->head = 0U;
	rb->tail = 0U;
	rb->count = 0U;
}

bool RingBuffer_IsEmpty(const RingBuffer_t *rb)
{
	return (rb->count == 0U);
}

bool RingBuffer_IsFull(const RingBuffer_t *rb)
{
	return (rb->count >= RING_BUFFER_SIZE);
}




bool RingBuffer_Push(RingBuffer_t *rb, uint8_t byte);
bool RingBuffer_Pop(RingBuffer_t *rb, uint8_t *byte);


