/*!
\file metagen_def.h
\brief defines which you should use in your C++ class to create metadata automatically
\version 0.2
\date 23.00.2020
*/
#pragma once

//!auto metagen script will create metadata for this class
#define lsclass

//!this definition mustn't be parsed and included to metadata automatically. use it when metagen script can't parse something
#define NOT_METADATA