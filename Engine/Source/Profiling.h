#pragma once

#include "Optick/include/optick.h"

#if RG_PROFILING
	#define RG_PROFILING_FUNCTION(...)			OPTICK_EVENT(__VA_ARGS__)
	#define RG_PROFILING_FRAME(...)				OPTICK_FRAME(__VA_ARGS__)
	#include "mmgr/mmgr.h"
#else
	#define RG_PROFILING_FUNCTION(...)			
	#define RG_PROFILING_FRAME(...)	
	#include "mmgr/mmgr.h"
#endif // !RG_PROFILING