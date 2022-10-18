/**
 * @file cbuf.h
 * @author ztnel (christian911@sympatico.ca)
 * @brief A lightweight circular buffer API
 * @version 0.1
 * @date 2022-10
 * 
 * @copyright Copyright © 2022 Christian Sargusingh
 * 
 */

#ifndef __CBUF_H__
#define __CBUF_H__

#include <stddef.h>
#include <stdint.h>

typedef int cbuf_err_t;

#define CBUF_SUCCESS (cbuf_err_t)0
#define CBUF_EMPTY (cbuf_err_t)1


typedef struct cbuf_t {
	uint8_t *buffer; // buffer data
	uint8_t full;	 // full state boolean (saves a buffer index for checking wrap condition)
	size_t head;	 // head position idx
	size_t tail; 	 // tail position idx
	size_t max;		 // maximum buffer size (bytes)
} cbuf_t;

cbuf_t cbuf_init(uint8_t *, size_t size);
void cbuf_reset(cbuf_t *);
void cbuf_put(cbuf_t *, uint8_t);
cbuf_err_t cbuf_get(cbuf_t *, uint8_t *);
uint8_t cbuf_full(cbuf_t *);
uint8_t cbuf_empty(cbuf_t *);
size_t cbuf_size(cbuf_t *);

#endif // __CBUF_H__
