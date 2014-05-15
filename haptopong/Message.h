#pragma once

enum MessageType
{
	C_HELLO = 0,

	S_WELCOME = 100,
	
	G_HIT_BALL = 200,
	G_UPDATE_POS = 201,
	G_BALLEVENT = 202
};

class Message 
{
public:
	//Adds type + data to buffer, moves the pointer to after the message.
	virtual void addToBuffer(unsigned char*& buffer) const = 0;
	
	//Sets the data of a message from the buffer, EXCLUDING the type. Moves 
	//the pointer to the next message.
	virtual void setFromBuffer(unsigned char*& buffer) = 0;

	virtual int getSize() const = 0;
	virtual const unsigned char* getData() const = 0;
	virtual unsigned char getType() const = 0;
};

typedef std::shared_ptr<Message> MessagePtr;
