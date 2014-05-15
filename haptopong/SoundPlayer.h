#pragma once

#include <bass.h>

class SoundPlayer
{
public:
    SoundPlayer(const char* file, const unsigned &startPos);
    void play(float frequency = 0);
    static DWORD CALLBACK MyStreamWriter(HSTREAM handle, void *buf, DWORD len, void *user);
    
private:
    QWORD m_streamLength;
    BASS_CHANNELINFO m_infoBass;
    char *m_data;
    HSTREAM m_stream;
    unsigned int m_pos;
    unsigned int m_startpos;
};
