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

#ifndef MBLOCK_H_
#define MBLOCK_H_

#include <stdlib.h>

/*
 * Memory block providing compact storage
 * Implemented as a stack, every alloc pops a memory unit and free pushes a new
 * unit in. The stack starts full.
 */
struct mblock {
	void *top;
	unsigned int sz;
	unsigned int n;
};

/*
 * Should be aligned or can cause memory faults
 * Also, sizeof(_type) should be bigger or equal to pointer size or otherwise
 * everything will crash.
 */
#define MBLOCK_DECLARE(_name, _type, _n)                 \
	static _type _name ## _strg[(_n)];               \
	struct mblock _name = {.top = _name ## _strg,    \
			       .sz = sizeof(_type),      \
			       .n = (_n)};

/*
 * Called after MBLOCK_DECLARE()
 */
#define mblock_init(_mb)                                        \
	do {                                                    \
		char *_ptr = (_mb)->top;                        \
		char *_lmt = _ptr + (_mb)->sz * ((_mb)->n - 1); \
                                                                \
		while (_ptr < _lmt) {                           \
			*(char **)_ptr = _ptr + (_mb)->sz;      \
			_ptr += (_mb)->sz;                      \
		}                                               \
		*(char **)_ptr = NULL;                          \
	} while (0)

/*
 * Keep always a reference to the start position of the block, since it's
 * required to free it afterwards.
 */
static inline void *mblock_alloc(struct mblock *mb)
{
		char **tmp = mb->top;

		if (mb->top == NULL) /* Empty */
			return NULL;

		mb->top = *tmp;
		return tmp;
}

/*
 * No error checking, so *always* pass a valid top.
 */
static inline void mblock_free(struct mblock *mb, void *top)
{
		char **tmp = top;

		*tmp = mb->top;
		mb->top = top;
}

#endif /* MBLOCK_H_ */
