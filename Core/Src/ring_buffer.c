
#include "ring_buffer.h"

void ring_buffer_init(ring_buffer_t *ring_buffer, uint8_t *buffer, uint16_t capacity)
{
	ring_buffer -> buffer = buffer;
	ring_buffer -> capacity = capacity;

	ring_buffer -> head = 0;
	ring_buffer -> tail = 0;
	ring_buffer -> is_full = 0;
	ring_buffer -> is_empty = 1;

}
/**
 * @brief this function adds data in the ring buffer
 * @param data: the value to be added
 */

uint8_t ring_buffer_put(ring_buffer_t *ring_buffer, uint8_t data) //save data in the buffer
{
	ring_buffer -> buffer [ring_buffer -> head] = data; //where the head is, put the data.

	ring_buffer -> head = (ring_buffer -> head + 1) % ring_buffer -> capacity; //the module "%" returns the value of the division.
	//When the counter reaches the end of the memory it is rebooted so it can write data again

	//if(ring_buffer -> head >ring_buffer -> capacity) //another method to re arrange the counter when it get full.
	//{
	//	ring_buffer -> head;
	//}

	//if the head has already reached the end of the data, it comes back and reboot itself to write data again.
	//a buffer is full when the position of the head matches the position of the tail.

	if(ring_buffer -> is_full != 0)
	{
		ring_buffer -> tail = (ring_buffer ->tail + 1) % ring_buffer -> capacity; //if the buffer is still not full the tail moves one position.
	}

	if(ring_buffer -> head == ring_buffer -> tail)
	{
		ring_buffer -> is_full = 1; //when the head matches the tail the buffer gets full.
		return 0;
	}
	return 1;
}

/**
 * @brief this function reads data from the ring buffer
 * @param data: the pointer to the address to write the data
 */
uint8_t ring_buffer_get(ring_buffer_t *ring_buffer, uint8_t *data)
{
	if(ring_buffer_empty( ring_buffer) == 0) //it works whether the buffer is empty  or the tail is different from the head.
	{
		*data = ring_buffer -> buffer[ring_buffer -> tail];
		ring_buffer -> tail = (ring_buffer -> tail + 1) % ring_buffer -> capacity;//actualize the index(position) of the tail
		ring_buffer -> is_full = 0; // clean the "is_full" flag
		return 1;
	}
	return 0;
}

/**
 * @brief this function returns the size of the buffer
 */

uint16_t ring_buffer_size(ring_buffer_t *ring_buffer)
{
	uint16_t size = 0;

	if ((ring_buffer -> is_full == 0) && (ring_buffer -> tail <= ring_buffer-> head) ){

		size = ring_buffer -> head - ring_buffer -> tail;
	} else{

		size = ring_buffer -> head + (ring_buffer -> capacity - ring_buffer -> tail);
	}
	return size;
}

/**
 * @brief this function determines if the buffer is empty
 */

uint16_t ring_buffer_empty(ring_buffer_t *ring_buffer)
{

	if ((ring_buffer -> head == ring_buffer -> tail) && (ring_buffer -> is_full == 0)){
		ring_buffer -> is_empty = 1;
		return 1;
	}
	return 0;
}

/**
 * @brief this function resets the buffer to the default value
 */
void ring_buffer_reset(ring_buffer_t *ring_buffer){
		ring_buffer -> tail = 0;
		ring_buffer -> head = 0;
		ring_buffer -> is_full = 0;
}
/**
 * @brief this function determines if the buffer is full
 */

uint16_t ring_buffer_full(ring_buffer_t *ring_buffer){

	return (ring_buffer -> is_full == 0);
}
