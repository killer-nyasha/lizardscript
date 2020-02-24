/*!
\file crossplatform_tchar.h
\brief what if sometimes char will take 3 bytes? or 4 trytes? or 0.3725 fucking quantum bits? 
just use defines from this file in your project - this is a layer of abstraction from real char types.
\author killer-nyasha
\version 0.2
\date 23.00.2020
*/
#pragma once

//#define UNICODE
//else - UTF-8 or ASCII

#ifdef UNICODE
using TCHAR = wchar_t;
#define _tcscmp wcscmp
#define _tcslen wcslen
#define _tcscpy wcscpy
#define _tcsncpy wcsncpy
#define _tcstoll wcstoll
#define _tcstof wcstof
#define _T(str) L##str
#define tstring std::wstring
#else
using TCHAR = char;
#define _tcscmp strcmp
#define _tcslen strlen
#define _tcscpy strcpy
#define _tcsncpy strncpy
#define _tcstoll strtoll
#define _tcstof strtof
#define _T(str) str
#define tstring std::string
#endif