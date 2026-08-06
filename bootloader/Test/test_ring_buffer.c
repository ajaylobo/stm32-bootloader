/*
 * test_ring_buffer.c
 *
 *  Created on: 06-Aug-2026
 *      Author: ajayd
 */

#include "test_ring_buffer.h"
#include <stdio.h>

void RingBuffer_RunTests()
{
    RingBuffer_t rb;
    uint8_t data;

    RingBuffer_Init(&rb);

    if (RingBuffer_IsEmpty(&rb))
    {
        printf("PASS\n\n");
    }
    else
    {
        printf("FAIL\n\n");
    }

    RingBuffer_Print(&rb);
    RingBuffer_Push(&rb, 10);
    RingBuffer_Print(&rb);
    printf("Buffer[0] : %d\n", rb.buffer[0]);
    RingBuffer_Push(&rb, 20);
    RingBuffer_Push(&rb, 30);
    RingBuffer_Pop(&rb, &data);
    printf("%d\n", data);
    RingBuffer_Print(&rb);
    RingBuffer_Pop(&rb, &data);
    printf("%d\n", data);
    RingBuffer_Print(&rb);
    RingBuffer_Pop(&rb, &data);
	printf("%d\n", data);
	RingBuffer_Print(&rb);
	RingBuffer_Pop(&rb, &data);
	printf("%d\n", data);
    RingBuffer_Push(&rb, 40);
    RingBuffer_Push(&rb, 50);
	RingBuffer_Print(&rb);
    RingBuffer_Pop(&rb, &data);
	printf("%d\n", data);
    RingBuffer_Pop(&rb, &data);
	printf("%d\n", data);
	for(int i=0;i<RING_BUFFER_SIZE;i++)
	{
	    RingBuffer_Push(&rb,i);
	}
	if(RingBuffer_Push(&rb,100))
	{
	    printf("pass\n");
	}
	else
	{
	    printf("fail\n");
	}


    /* Test cases here */
}

void RingBuffer_Print(const RingBuffer_t *rb)
{
    printf("--------------------------\n");

    printf("Head  : %u\n", rb->head);

    printf("Tail  : %u\n", rb->tail);

    printf("Count : %u\n", rb->count);

    printf("--------------------------\n");
}
