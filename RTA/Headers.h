#pragma once
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#if defined(_UNICODE)
#define DBGHELP_TRANSLATE_TCHAR
#endif


#include <Windows.h>
//#include <shellapi.h> // CommandLineToArgvW
//#include <windowsx.h>
#include <tchar.h>
#include <dbgHelp.h>
#if defined(USE_WER)
#include <WERAPI.H>
#pragma comment(lib,"wer.lib")
#endif

#include <D3D11.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <directxmath.h>
//using namespace DirectX;
