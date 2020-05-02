#ifndef PCH_H
#define PCH_H

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#define FORVARIADIC(...) std::initializer_list<int>({ (__VA_ARGS__, 0)... });

#endif //PCH_H
