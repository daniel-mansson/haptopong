#include "pch.h"
#include "SoundPlayer.h"

SoundPlayer *SoundPlayer::ptr = nullptr;

SoundPlayer::SoundPlayer(const std::string& file) {
    ptr = this;
    
    // Initialize sound device and create audio stream
    BASS_Init(1,44100,0,0,NULL);
    
    // Load the data from the specified file
    HSTREAM file_stream = 1;
    file_stream = BASS_StreamCreateFile(FALSE, "../sound/classic.mp3",0,0,BASS_STREAM_DECODE);
    if (!file_stream)
    {
        printf("Error - MP3 audio file failed to load correctly.\n");
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
    m_stream = BASS_StreamCreate(m_infoBass.freq, m_infoBass.chans, 0, &MyStreamWriter, 0);
}

void SoundPlayer::play() const {
    
}

DWORD CALLBACK SoundPlayer::MyStreamWriter(HSTREAM handle, void *buf, DWORD len, void *user)
{
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