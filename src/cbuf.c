/**
 * @file cbuf.c
 * @author ztnel (christian911@sympatico.ca)
 * @brief A lightweight circular buffer API
 * @version 0.1
 * @date 2022-10
 * 
 * @copyright Copyright © 2022 Christian Sargusingh
 * 
 */

#include <assert.h>
#include "cbuf.h"

static void inc_ptr(cbuf_t *cbuffer) {
  assert(cbuffer);
  // if the buffer is full increment tail and check reset condition for tail
  if (cbuffer->full && (++(cbuffer->tail) == cbuffer->max)) {
    cbuffer->tail = 0;
  }
  // increment head and check reset condition for head
  if (++(cbuffer->head) == cbuffer->max) {
    cbuffer->head = 0;
  }
  // if increment is invoked we know there is data in the buffer
  cbuffer->full = (cbuffer->tail == cbuffer->head);
}

static void dec_ptr(cbuf_t *cbuffer) {
  assert(cbuffer);
  // automatically the buffer is no longer full
  cbuffer->full = 0;
  if (++(cbuffer->tail) == cbuffer->max) {
    cbuffer->tail = 0;
  }
}

cbuf_t cbuf_init(uint8_t *buffer, size_t size) {
  cbuf_t cbuffer;
  cbuffer.buffer = buffer;
  cbuffer.max = size;
  cbuf_reset(&cbuffer);
  return cbuffer;
}

void cbuf_reset(cbuf_t *cbuffer) {
  assert(cbuffer);
  cbuffer->head = 0;
  cbuffer->tail = 0;
  cbuffer->full = 0;
}

void cbuf_put(cbuf_t *cbuffer, uint8_t data) {
  assert(cbuffer && cbuffer->buffer);
  cbuffer->buffer[cbuffer->head] = data;
  inc_ptr(cbuffer);
}

cbuf_err_t cbuf_get(cbuf_t *cbuffer, uint8_t *data) {
  assert(cbuffer && cbuffer->buffer && data);
  if (cbuf_empty(cbuffer)){
    return CBUF_EMPTY;
  }
  *data = cbuffer->buffer[cbuffer->tail];
  dec_ptr(cbuffer);
  return CBUF_SUCCESS;
}

uint8_t cbuf_empty(cbuf_t *cbuffer) {
  assert(cbuffer);
  return !cbuffer->full && !(cbuffer->tail == cbuffer->head);
}

size_t cbuf_size(cbuf_t *cbuffer) {
  assert(cbuffer);
  size_t size = cbuffer->max;
  if (!cbuffer->full) {
    if (cbuffer->head >= cbuffer->tail) {
      size = cbuffer->head - cbuffer->tail;
    } else {
      size = cbuffer->max + cbuffer->head - cbuffer->tail;
    }
  }
  return size;
}