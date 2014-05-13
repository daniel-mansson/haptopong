#pragma once

#include <bass.h>

class SoundPlayer
{
public:
    SoundPlayer(const char* file);
    void play();
    static DWORD CALLBACK MyStreamWriter(HSTREAM handle, void *buf, DWORD len, void *user);
    
private:
    QWORD m_streamLength;
    BASS_CHANNELINFO m_infoBass;
    char *m_data;
    HSTREAM m_stream;
    unsigned int m_pos;
};
