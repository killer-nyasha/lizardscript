#ifndef PCH_H
#define PCH_H

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#define FORVARIADIC(...) std::initializer_list<int>({ (__VA_ARGS__, 0)... });

#ifdef _WIN32
#define crossplatform_pause() system("pause")
#else
#define crossplatform_pause() system("read")
#endif


#endif //PCH_H
