#include "pch.h"
#include "SoundPlayer.h"

SoundPlayer::SoundPlayer(const char* file, const unsigned &startpos) :
m_pos(0)
//m_startpos(startpos)
{
    
    // Load the data from the specified file
    HSTREAM file_stream = 1;
    file_stream = BASS_StreamCreateFile(FALSE, file ,startpos,0,BASS_STREAM_DECODE);
    if (!file_stream)
    {
        printf("%s Error - MP3 AUDIODESCRIPTION file failed to load correctly.\n", file);
        std::exit(EXIT_FAILURE);
    }
    
    // Get the length and header info from the loaded file
    //stream_length = BASS_StreamGetLength(file_stream);
    m_streamLength = BASS_ChannelGetLength(file_stream, 0);
    BASS_ChannelGetInfo(file_stream, &m_infoBass);
    
    // Get the audio samples from the loaded file
    m_data = new char[(unsigned int)m_streamLength];
    BASS_ChannelGetData(file_stream, m_data, (unsigned int)m_streamLength);
    
    // Set playing to begin at the beginning of the loaded data
    m_stream = BASS_StreamCreate(m_infoBass.freq, m_infoBass.chans, 0, &MyStreamWriter, this);
}

void SoundPlayer::play(float frequency, float volume)
{
    BASS_ChannelStop(m_stream);
    
    BASS_ChannelSetAttribute(m_stream, BASS_ATTRIB_FREQ, (float)(m_infoBass.freq * frequency));
    BASS_ChannelSetAttribute(m_stream, BASS_ATTRIB_VOL, (float)chai3d::cClamp01(volume));
    
    //m_pos = m_startpos;
    m_pos = 0;
    

    BASS_ChannelPlay(m_stream, FALSE);
}

DWORD CALLBACK SoundPlayer::MyStreamWriter(HSTREAM handle, void *buf, DWORD len, void *user)
{
    SoundPlayer* ptr = (SoundPlayer*)user;
    
    // Cast the buffer to a character array
    char *d=(char*)buf;
    
    // write requested amount of data from current position forwards
    int up = len + ptr->m_pos;
    if (up > ptr->m_streamLength)
        up = (unsigned int)ptr->m_streamLength;
    
    for (int i=ptr->m_pos; i<up; i+=1)
    {
        d[(i-ptr->m_pos)] = ptr->m_data[i];
    }
    
    int amt = (up-ptr->m_pos);
    ptr->m_pos += amt;
    return amt;

    return 0;
}