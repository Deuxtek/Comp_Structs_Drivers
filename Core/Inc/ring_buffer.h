/*
 * ring_buffer.h
 *
 *  Created on: Oct 27, 2023
 *      Author: ANDRES CUELLAR
 */

#ifndef INC_RING_BUFFER_H_
#define INC_RING_BUFFER_H_

#include <stdint.h>

typedef struct{ //definition of a type- the type you will be using.
	uint8_t *buffer;//pointer of the memory. The "*" indicates that you have to receive a direction.
	uint16_t head;//head is used to write new data.
	uint16_t tail; //tail is used to read old data.

	//buffer is full when the position of the tail = position of the head.
	uint16_t capacity;//the capacity of the buffer until it gets full.
	uint16_t is_full; //indicates if the buffer is full.
	uint16_t is_empty;

}ring_buffer_t;

//the arguments are the structures that will be initialized, the position of the memory is where the data will be located.
void ring_buffer_init(ring_buffer_t *ring_buffer, uint8_t *buffer, uint16_t capacity);

uint8_t ring_buffer_put(ring_buffer_t *ring_buffer, uint8_t data);//save the data in the buffer.

uint8_t ring_buffer_get(ring_buffer_t *ring_buffer, uint8_t *data);

uint16_t ring_buffer_size(ring_buffer_t *ring_buffer);

uint16_t ring_buffer_empty(ring_buffer_t *ring_buffer);

uint16_t ring_buffer_full(ring_buffer_t *ring_buffer);

void ring_buffer_reset(ring_buffer_t *ring_buffer);

#endif /* INC_RING_BUFFER_H_ */
