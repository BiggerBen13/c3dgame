#ifndef _R_TYPES_H
#define _R_TYPES_H

typedef struct {
	struct {
		float x, y, z;
	} pos;
	struct {
		float r, g, b, a;
	} color;
} vertex;

#endif
