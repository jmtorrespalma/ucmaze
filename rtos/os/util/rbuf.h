/*
 * Copyright (C) 2018 Juan Manuel Torres Palma <j.m.torrespalma@gmail.com>
 * This file is part of ucmaze.
 *
 * ucmaze is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ucmaze is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ucmaze.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef RBUF_H_
#define RBUF_H_

#include <stdlib.h>

/*
 * Ring buffer implementation.
 */
struct rbuf {
	void *head;
	void *tail;
	void *buff;
	unsigned int n;
	unsigned int offset;
	char empty;
};

#define rbuf_ptr_offbounds(_rb, _ptr) \
	((_ptr) >= (char *)(rb)->buff + (rb)->n * (rb)->offset)

/*
 * It's aligned so must be a multiple of 2.
 */
#define RBUF_DECLARE(_name, _type, _n)                      \
	static _type _name ## _strg[(_n)]                   \
	__attribute__ ((__aligned__ (sizeof(_type))));      \
	struct rbuf _name = {.head = _name ## _strg,        \
			     .tail = _name ## _strg,        \
			     .buff = _name ## _strg,        \
			     .n = (_n),                     \
			     .offset = (sizeof(_type)),     \
			     .empty = 1};

static inline int rbuf_empty(struct rbuf *rb)
{
	return (rb->tail == rb->head && rb->empty);
}

static inline int rbuf_full(struct rbuf *rb)
{
	return (rb->tail == rb->head && !rb->empty);
}

/*
 * Insert a new element in the buffer if not full. Return -1 otherwise.
 */
static inline int rbuf_push(struct rbuf *rb, void *item)
{
		char *head = rb->head;
		char *itemp = item;

		if (rbuf_full(rb))
			return -1;

		for (int i = 0; i < rb->offset; ++i)
			*head++ = *itemp++;

		if (rbuf_ptr_offbounds(rb, head))
			head = rb->buff;

		rb->head = head;
		rb->empty = 0;

		return 0;
}

/*
 * Pop an element from the buffer if not empty, return -1 otherwise.
 */
static inline int rbuf_pop(struct rbuf *rb, void *item)
{
		char *tail = rb->tail;
		char *itemp = item;

		if (rbuf_empty(rb))
			return -1;

		for (int i = 0; i < rb->offset; ++i)
			*itemp++ = *tail++;

		if (rbuf_ptr_offbounds(rb, tail))
			tail = rb->buff;

		rb->tail = tail;
		if (tail == rb->head)
			rb->empty = 1;

		return 0;
}

#endif /* RBUF_H_ */
