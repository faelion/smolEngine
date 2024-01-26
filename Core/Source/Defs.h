#pragma once
// General config

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define FPS 60
//static const auto FDT = 1.0s / FPS;
#define WINDOW_TITLE "smolEngine"
#define FULLSCREEN false
#define FULLSCREEN_WINDOW false
#define BORDERLESS false
#define RESIZABLE true
#define FRAMECAP false
#define VSYNC true

//  NULL just in case ----------------------

#ifdef NULL
#undef NULL
#endif
#define NULL  0

// Deletes a buffer
#define RELEASE( x ) \
	{						\
	if( x != NULL )		\
		{					  \
	  delete x;			\
	  x = NULL;			  \
		}					  \
	}

// Deletes an array of buffers
#define RELEASE_ARRAY( x ) \
	{							  \
	if( x != NULL )			  \
		{							\
	  delete[] x;				\
	  x = NULL;					\
		}							\
							  \
	}

#define IN_RANGE( value, min, max ) ( ((value) >= (min) && (value) <= (max)) ? 1 : 0 )
#define MIN( a, b ) ( ((a) < (b)) ? (a) : (b) )
#define MAX( a, b ) ( ((a) > (b)) ? (a) : (b) )
#define TO_BOOL( a )  ( (a != 0) ? true : false )

typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

template <class VALUE_TYPE> void SWAP(VALUE_TYPE& a, VALUE_TYPE& b)
{
	VALUE_TYPE tmp = a;
	a = b;
	b = tmp;
}

// Standard string size
#define SHORT_STR	 32
#define MID_STR	    255
#define HUGE_STR   8192

// Joins a path and file
inline const char* const PATH(const char* folder, const char* file)
{
	static char path[MID_STR];
	sprintf_s(path, MID_STR, "%s/%s", folder, file);
	return path;
}

// Performance macros
#define PERF_START(timer) timer.Start()
#define PERF_PEEK(timer) LOG("%s took %f ms", __FUNCTION__, timer.ReadMs())


#ifdef SMOL_ENABLE_ASSERTS
#define SMOL_ASSERT(x, ...) {if(!(x)) { SMOL_ERROR("Assertion Failed: {0}", __VA_ARGS__);  __debugbreak(); } }
#define SMOL_ASSERT_MSG(...) { SMOL_ERROR("Assertion Failed: {0}", __VA_ARGS__);  __debugbreak(); } 
#define SMOL_CORE_ASSERT(x, ...) {if(!(x)) { SMOL_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define SMOL_CORE_ASSERT_MSG(...) { SMOL_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}
#else
#define SMOL_ASSERT(x, ...) 
#define SMOL_ASSERT_MSG(...)
#define SMOL_CORE_ASSERT(x, ...)
#define SMOL_CORE_ASSERT_MSG(...)
#endif


// General purpose
#define BIT(x) (1 << (x))
#define CLAMP(x, mn, mx) (x) < (mn) ? (x) = (mn) : (x) = (x); (x) > (mx) ? (x) = (mx) : (x) = (x)

#define PI 3.14159265359
#define PI_F 3.14159265359f

#define SMOL_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)



