#ifndef TRIMESH_ENDIAN_H
#define TRIMESH_ENDIAN_H
/*
Szymon Rusinkiewicz
Stanford Graphics Lab

trimesh_endian.h
Byte swapping for big/little-endian architectures
*/

#if defined(i386) || defined(WIN32)
#  define we_are_little_endian 1
#else
#  define we_are_little_endian 0
#endif

#define we_are_big_endian (!we_are_little_endian)


static inline void swap_2(unsigned short &x)
{
	x = ((x & 0xff) << 8) | (x >> 8);
}

static inline void swap_2(short &x)
{
	swap_2(* (unsigned short *)(&x));
}

static inline void swap_4(unsigned &x)
{
	x = (x << 24) |
	    ((x << 8) & 0x00ff0000u) |
	    ((x >> 8) & 0x0000ff00u) |
	    (x >> 24);
}

static inline void swap_4(int &x)
{
	swap_4(* (unsigned *)(&x));
}

static inline void swap_float(float &x)
{
	swap_4(* (unsigned *)(&x));
}


#endif
