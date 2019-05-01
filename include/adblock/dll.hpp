#ifndef ADBLOCK_API_DLL_HPP
#define ADBLOCK_API_DLL_HPP

#if defined _WIN32 || defined __CYGWIN__
    #define ADBLOCK_DLL_IMPORT __declspec(dllimport)
    #define ADBLOCK_DLL_EXPORT __declspec(dllexport)
    #define ADBLOCK_DLL_LOCAL
#else
    #define ADBLOCK_DLL_IMPORT __attribute__ ((visibility ("default")))
    #define ADBLOCK_DLL_EXPORT __attribute__ ((visibility ("default")))
    #define ADBLOCK_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#endif

#endif // ADBLOCK_API_DLL_HPP
