// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define __NT__
#define __IDP__

#define USE_STANDARD_FILE_FUNCTIONS
#define _CRT_SECURE_NO_WARNINGS

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// TODO: reference additional headers your program requires here
#include <ida.hpp>
#include <idp.hpp>
#include <loader.hpp>
#include <funcs.hpp>
#include <name.hpp>
#include <bytes.hpp>
#include <area.hpp>
#include <allins.hpp>
#include <lines.hpp>
#include <name.hpp>