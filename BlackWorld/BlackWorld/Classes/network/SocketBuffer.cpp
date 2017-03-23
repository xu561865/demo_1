#include "SocketBuffer.h"
#include "Util.h"
#include "SocketClient.h"

#define BYTE_SIZE 1
#define SHORT_SIZE 2
#define INT_SIZE 4
#define FLOAT_SIZE 4
#define LONG_SIZE 8


SocketBuffer::SocketBuffer(int capacity)
{

	this->buffer = new char[capacity];
	this->position = 0;
	this->capacity = capacity;
	this->limit = capacity;
}

SocketBuffer::SocketBuffer(char* data,int offset,int capacity)
{
	this->buffer = new char[capacity];
	this->position = 0;
	memcpy(this->buffer, data + offset, capacity);
	this->capacity = capacity;
	this->limit = this->capacity;
}

SocketBuffer::~SocketBuffer()
{
	if(this->buffer)
    {
        delete [] this->buffer;
    }
}

int SocketBuffer::remaining()
{
	return this->limit - this->position;
}

void SocketBuffer::setIntAt(int intValue, int index)
{
    if(index + INT_SIZE > this->capacity)
    {
        return;
    }
    
    for(int i = 0 ; i < INT_SIZE ; ++i)
    {
        buffer[index] = (char)((intValue >> (8 *(3 - i))) & 0xFF);
        index++;
    }
}

void SocketBuffer::put(const char* bytes, int offset, int len)
{
	if(this->position + len > this->capacity)
    {
		return;
	}
    
	memcpy(this->buffer + this->position, bytes + offset, len);
	this->position += len;
}

void SocketBuffer::putByte(signed char byteValue)
{
	if(this->position + BYTE_SIZE > this->capacity)
    {
		return;
	}
    
	this->buffer[this->position++] = byteValue;
}

void SocketBuffer::putInt(int intValue)
{
	if(this->position + INT_SIZE > this->capacity)
    {
		return;
	}
    
	for(int i = 0; i < INT_SIZE; ++i)
    {
		this->buffer[this->position++] = (char)((intValue >> (8 *(3 - i))) & 0xFF);
	}
}

void SocketBuffer::putFloat(float floatValue)
{
	if(this->position + INT_SIZE > this->capacity)
    {
		return;
	}
    
	signed char* pBytes = static_cast<signed char*>(static_cast<void*>(&floatValue));

	this->buffer[this->position++] = pBytes[3];
	this->buffer[this->position++] = pBytes[2];
	this->buffer[this->position++] = pBytes[1];
	this->buffer[this->position++] = pBytes[0];
}

void SocketBuffer::putShort(short shortValue)
{
	if(this->position + SHORT_SIZE > this->capacity)
    {
		return;
	}
    
	for(int i = 0; i < SHORT_SIZE; ++i)
    {
		this->buffer[this->position++] = (char)((shortValue >> (8 *(1 - i))) & 0xFF);
	}
}

void SocketBuffer::putLong(long long longValue)
{
	if(this->position + LONG_SIZE > this->capacity)
    {
		return;
	}
	
	for(int i = 0; i < LONG_SIZE; ++i)
    {
		this->buffer[this->position++] = (char)((longValue >> (LONG_SIZE *(8 - i))) & 0xFF);
	}
}

void SocketBuffer::putUTF(const char* strValue)
{
    size_t slen = strlen(strValue);
    
	if(this->position + 2 + slen > this->capacity)
    {
		return;
	}
	this->putShort(slen);
	if(strlen > 0)
    {
		put(strValue, 0, slen);
	}
}

void SocketBuffer::putUTF(const std::string& strValue)
{
	putUTF(strValue.c_str());
}

void SocketBuffer::putArray(std::vector<signed char>& byteV)
{
	putInt(byteV.size());
	for(int i = 0; i < byteV.size(); ++i)
    {
		putByte(byteV[i]);
	}	
}

void SocketBuffer::putArray(std::vector<bool>& boolV)
{
	putInt(boolV.size());
	for(int i = 0; i < boolV.size(); ++i)
    {
		putBool(boolV[i]);
	}	
}

void SocketBuffer::putArray(std::vector<short>& shortV)
{
	putInt(shortV.size());
	for(int i = 0; i < shortV.size(); ++i)
    {
		putShort(shortV[i]);
	}
}

void SocketBuffer::putArray(std::vector<int>& intV)
{
	putInt(intV.size());
	for(int i = 0; i < intV.size(); ++i)
    {
		putInt(intV[i]);
	}
}

void SocketBuffer::putArray(std::vector<long long>& longV)
{
	putInt(longV.size());

	for(int i = 0; i < longV.size(); ++i)
    {
		putLong(longV[i]);
	}
}

void SocketBuffer::putArray(std::vector<std::string>& stringV)
{
	putInt(stringV.size());
	
	for(int i = 0; i < stringV.size(); ++i)
    {
		putUTF(stringV[i]);
	}
}



void SocketBuffer::getArray(std::vector<signed char>& byteV)
{
	int size = getInt();
	byteV.resize(size);
	for(int i = 0; i < size; ++i)
    {
		byteV[i] = getByte();
	}
}

void SocketBuffer::getArray(std::vector<bool>& boolV)
{
	int size = getInt();
	boolV.resize(size);
	for(int i = 0; i < size; ++i)
    {
		boolV[i] = getBool();
	}
}

void SocketBuffer::getArray(std::vector<short>& shortV)
{
	int size = getInt();
	shortV.resize(size);
	for(int i = 0; i< size; ++i)
    {
		shortV[i] = getShort();
	}
}

void SocketBuffer::getArray(std::vector<int>& intV)
{
	int size = getInt();
	intV.resize(size);
	for(int i = 0; i < size; ++i)
    {
		intV[i] = getInt();
	}
}

void SocketBuffer::getArray(std::vector<long long>& longV)
{
	int size = getInt();
	longV.resize(size);
	for(int i = 0; i < size; ++i)
    {
		longV[i] = getLong();
	}
}

void SocketBuffer::getArray(std::vector<std::string>& stringV)
{
	int size =getInt();
	getArray(stringV, size);
}

void SocketBuffer::getArray(std::vector<signed char>& byteV, int size)
{
	byteV.resize(size);
	for(int i = 0; i < size;++i)
    {
		byteV[i] = getByte();
	}
}

void SocketBuffer::getArray(std::vector<bool>& boolV, int size)
{
	boolV.resize(size);
	for(int i = 0; i < size;++i)
    {
		boolV[i] = getBool();
	}	
}

void SocketBuffer::getArray(std::vector<short>& shortV, int size)
{
	shortV.resize(size);
	for(int i = 0; i < size; ++i)
    {
		shortV[i] = getShort();
	}
}

void SocketBuffer::getArray(std::vector<int>& intV, int size)
{
	intV.resize(size);
	for(int i = 0; i < size; ++i)
    {
		intV[i] = getInt();
	}
}

void SocketBuffer::getArray(std::vector<long long>& longV, int size)
{
	longV.resize(size);
	for(int i = 0; i < size; ++i)
    {
		longV[i] = getLong();
	}
}

void SocketBuffer::getArray(std::vector<std::string>& stringV, int size)
{
	stringV.resize(size);
	for(int i = 0; i < size; ++i)
    {
		getUTF(stringV[i]);
	}
}

void SocketBuffer::getUTF(std::string& stringValue)
{
	short len = getShort();		
	if(len > 0)
    {
		stringValue.append(this->buffer + this->position, len);
		this->position += len;
	}
    else
    {
		stringValue = "";
	}
}

std::string SocketBuffer::getUTF()
{
    std::string str;
	getUTF(str);		
	return str;
}

int SocketBuffer::getPosition()
{
	return this->position;
}

void SocketBuffer::setPosition(int pos)
{
	if(pos > this->limit)
    {
        return;
	}
    
	this->position = pos;
}

int SocketBuffer::getLimit()
{
	return this->limit;
}

int SocketBuffer::getCapacity()
{
	return this->capacity;
}

char* SocketBuffer::getBuffer()
{
	return this->buffer;
}

signed char SocketBuffer::getByte()
{
	if(this->position + BYTE_SIZE > this->limit)
    {
		return 0;
	}
    
	return this->buffer[this->position++];
}

bool SocketBuffer::getBool()
{
	if(this->position + BYTE_SIZE > this->limit)
    {
		return false;
	}
    
	return this->buffer[this->position++] != 0;
}

int SocketBuffer::getLength(int offset)
{
    int lengthPos = this->position + offset;
    byte* pos = new byte[4];
    
    for(int i = 0; i < 4; ++i)
    {
        pos[i] = this->buffer[lengthPos + i];
    }
    
    for(int i = 0; i < 73; i ++)
    {
        printf("%d," , this->buffer[i]);
    }
    
    return SocketClient::bytesToInt(pos);
}

void SocketBuffer::getAsBytes(signed char* bytes)
{
    for(int i = 0; i < 4; i++)
    {
		bytes[i] = this->buffer[this->position++];
	}
}

int SocketBuffer::getInt()
{
	if(this->position + INT_SIZE > this->limit)
    {
		return 0;
	}
    
	int rt = 0;
	for(int i = 0; i < 4; ++i)
    {
		rt |=  ((this->buffer[this->position++] & 0xFF) << (8 *(3 - i)));
	}
    
	return rt;
}

float SocketBuffer::getFloat()
{
	if(position + FLOAT_SIZE > limit)
    {
		return 0;
	}
    
	float floatValue;
	signed char* pBytes = (signed char*)&floatValue;
	pBytes[3] = this->buffer[this->position++];
	pBytes[2] = this->buffer[this->position++];
	pBytes[1] = this->buffer[this->position++];
	pBytes[0] = this->buffer[this->position++];
    
	return floatValue;
}

short SocketBuffer::getShort()
{
	if(this->position + SHORT_SIZE > this->limit)
    {
		return 0;
	}
    
	short ret = 0;
	for(int i = 0; i < SHORT_SIZE; ++i)
    {
		ret |= ((this->buffer[this->position++] & 0xFF) << (8 *(1 - i)));
	}
    
	return ret;
}

long long SocketBuffer::getLong()
{
	if(this->position + LONG_SIZE > this->limit)
    {
		return 0;
	}
    
	long long ret = 0;
	for(int i = 0; i < LONG_SIZE; ++i)
    {
		ret = ((this->buffer[this->position++] & 0xFF) << (8 *(7 - i)));
	}
    
	return ret;
}

void SocketBuffer::get(char* bytes, int size)
{
	get(bytes, 0, size);
}

void SocketBuffer::get(char* bytes,int offset,int len)
{
	if(this->position + len > this->limit)
    {
		memset(bytes + offset, 0, len);
		return;
	}
    
	memcpy(bytes + offset, this->buffer + this->position, len);
	this->position += len;
}

void SocketBuffer::clear()
{
	this->position = 0;
	this->limit = this->capacity;
}

void SocketBuffer::flip()
{
	this->limit = this->position;
	this->position = 0;
}

void SocketBuffer::compact()
{
	if(this->position > 0)
    {
		for(int i = this->position; i < this->limit; ++i)
        {
			this->buffer[i - this->position] = this->buffer[i];
		}
	}
    
	this->position = this->limit - this->position;
	this->limit = this->capacity;
}

void SocketBuffer::rewind()
{
	this->position = 0;
}

void SocketBuffer::putBool(bool boolValue)
{
	if(this->position + 1 > this->capacity)
    {
		return;
	}
	
	if(boolValue)
    {
		this->buffer[this->position++] = (1&0xff);
    }
	else
    {
		this->buffer[this->position++] = (0&0xff);
    }
}

//这个方法只能用于打开的是文本文件的时候
std::string SocketBuffer::getLine()
{
    if(this->position >= this->capacity)
    {
        return "";
    }
    
    std::string line;
	for (int i = this->position; i < this->capacity; ++i)
    {
		if (this->buffer[i] == '\n')
        {
			line.append(this->buffer + this->position, i - this->position);
			this->position = i + 1;
			return line;
		}
	}
    
	line.append(this->buffer + this->position, this->capacity - this->position);
	this->position = this->capacity + 1;
    
	return line;
}


