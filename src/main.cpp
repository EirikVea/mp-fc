#include "main.h"

void *g_decoder = NULL;

void DLL_EXPORT mp_PluginInfo(struct mp_plugin_info *info)
{
    strcpy(info->name,"Future Composer 1.3/1.4 plugin");
    info->version = 0x00000101;
}

bool DLL_EXPORT mp_Detect(char *filename, struct mp_song_info *songinfo)
{
    FILE *fp = fopen(filename,"rb");
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    rewind(fp);

    char *data = (char*)calloc(size * sizeof(char),sizeof(char));
    fread(data,sizeof(char),size,fp);

    fclose(fp);

    g_decoder = fc14dec_new();
    if(!fc14dec_detect(g_decoder,data,size))
    {
        fc14dec_delete(g_decoder);
        return false;
    }
    fc14dec_init(g_decoder,data,size);

    songinfo->format = (char*)malloc(strlen(fc14dec_format_name(g_decoder)) * sizeof(char));
    strcpy(songinfo->format,fc14dec_format_name(g_decoder));
    songinfo->name = NULL;
    songinfo->artist = NULL;
    songinfo->duration = fc14dec_duration(g_decoder);
    songinfo->info = NULL;
    songinfo->subsongs = -1;
    songinfo->voices = 4;
    songinfo->steps = -1;

    fc14dec_delete(g_decoder);

    return true;
}

bool DLL_EXPORT mp_InitPlugin(char *filename, int frequency, int bps, int channels)
{
    FILE *fp = fopen(filename,"rb");
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    rewind(fp);

    char *data = (char*)calloc(size * sizeof(char),sizeof(char));
    fread(data,sizeof(char),size,fp);

    fclose(fp);

    g_decoder = fc14dec_new();
    if(!fc14dec_detect(g_decoder,data,size))
    {
        fc14dec_delete(g_decoder);
        return false;
    }
    fc14dec_init(g_decoder,data,size);
    fc14dec_mixer_init(g_decoder,frequency,bps,channels,0);

    return true;
}

void DLL_EXPORT mp_GetSongInfo(struct mp_song_info *songinf)
{

}

DWORD DLL_EXPORT mp_GetPosition()
{
    return 0;
}

void DLL_EXPORT mp_SetPosition(DWORD pos)
{

}

void mp_RemoveStereoSep(short *buffer, DWORD length, float factor)
{
    short t1;
    for(int i = 0; i < (length >> 1) - 1; i += 1)
    {
        buffer[i] = ((float)buffer[i] * factor);
        buffer[i + 1] = (float)buffer[i + 1] * factor;
        t1 = buffer[i];
        buffer[i] += buffer[i + 1];
        buffer[i + 1] += t1;
    }
}

DWORD DLL_EXPORT mp_FillBuffer(void *buffer, DWORD length)
{
    fc14dec_buffer_fill(g_decoder,buffer,length);
    mp_RemoveStereoSep((short*)buffer,length,0.5);
    return length;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            // attach to process
            // return FALSE to fail DLL load
            break;

        case DLL_PROCESS_DETACH:
            // detach from process
            break;

        case DLL_THREAD_ATTACH:
            // attach to thread
            break;

        case DLL_THREAD_DETACH:
            // detach from thread
            break;
    }
    return TRUE; // succesful
}
