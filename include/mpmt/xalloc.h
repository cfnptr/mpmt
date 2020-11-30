#pragma once
#include <stdlib.h>

/*
 *
 * Allocates a new memory block with specified size.
 * Raises abort if allocation result is null.
 *
 * Returns pointer to the beginning of the block.
 */
#define xmalloc(size) \
({ \
	void* _pointer = malloc((size)); \
	if (_pointer == NULL) abort(); \
	_pointer; \
})

/*
 *
 * Allocates a new memory block with specified count and size.
 * And fills allocated memory with zero bytes.
 * Raises abort if allocation result is null.
 *
 * Returns pointer to the beginning of the block.
 */
#define xcalloc(count, size) \
({ \
	void* _pointer = calloc((count), (size)); \
	if (_pointer == NULL) abort(); \
	_pointer; \
})

/*
 *
 * Reallocates an existing memory block with specified size.
 * Raises abort if reallocation result is null.
 *
 * Returns pointer to the beginning of the block.
 */
#define xrealloc xrealloc(pointer, size) \
({ \
	void* _pointer = realloc(pointer, size); \
	if (_pointer == NULL) abort(); \
	_pointer; \
})
