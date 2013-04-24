#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdio.h>
#include <windows.h>

#include "fc14audiodecoder.h"
#include "FC.h"

#define STREAM_PUSH -1
#define STREAM_PULL 0

struct mp_plugin_info
{
    char name[100];
    DWORD version;
    DWORD stream_type;
};

struct mp_song_info
{
    char *format;
    char *name;
    char *artist;
    int duration;
    int subsongs;
    int voices;
    int steps;
    char *info;
};

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C"
{
#endif

void DLL_EXPORT mp_PluginInfo(struct mp_plugin_info *info);
bool DLL_EXPORT mp_Detect(char *filename, struct mp_song_info *songinfo);
bool DLL_EXPORT mp_InitPlugin(char *filename, int frequency, int bps, int channels);
DWORD DLL_EXPORT mp_FillBuffer(void *buffer, DWORD length);

#ifdef __cplusplus
}
#endif

#endif // __MAIN_H__
