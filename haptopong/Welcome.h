#pragma once

#include "Message.h"

class Welcome : public Message
{
public:

	Welcome(const std::string& name)
	{
		m_name = name;
		if(m_name.size() > 15)
			m_name.substr(15);

		memset(m_data, 0, sizeof(char) * 16);
		strcpy((char*)m_data, m_name.c_str());
	}

	Welcome()
	{
		memset(m_data, 0, sizeof(char) * 16);
	}
	
	virtual int getSize() const
	{
		return 1 + 16;
	}

	virtual const unsigned char* getData() const
	{
		return m_data;
	}

	virtual unsigned char getType() const
	{
		return S_WELCOME;
	}
	
	virtual void addToBuffer(unsigned char*& buffer) const
	{
		*buffer = getType();
		buffer += 1;

		for(int i = 0; i < 16; ++i)
		{
			*buffer = m_data[i];
			buffer += 1;
		}
	}
	
	virtual void setFromBuffer(unsigned char*& buffer)
	{
		for(int i = 0; i < 16; ++i)
		{
			m_data[i] = *buffer;
			buffer += 1;
		}
	}

private:

	std::string m_name;
	unsigned char m_data[16];
};

