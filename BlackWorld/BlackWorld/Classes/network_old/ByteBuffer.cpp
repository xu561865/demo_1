#include "ByteBuffer.h"
#include "Util.h"
#include "SocketClient.h"

#define BYTE_SIZE 1
#define SHORT_SIZE 2
#define INT_SIZE 4
#define FLOAT_SIZE 4
#define LONG_SIZE 8

int readIntFromBuffer(byte* buffer, int position)
{
    int rt = 0;
    for(int i = 0 ; i < 4 ; i++)
    {
        rt |= ((buffer[position] & 0xFF) << (8 *(3 - i)));
        position++;
    }
    
    return rt; 
}


ByteBuffer::ByteBuffer(int capacity)
{

	this->buffer = new char[capacity];
	this->position = 0;
	this->capacity = capacity;
	this->limit = capacity;
}

ByteBuffer::ByteBuffer(char* data,int offset,int capacity)
{
	this->buffer = new char[capacity];
	this->position = 0;
	memcpy(this->buffer, data + offset, capacity);
	this->capacity = capacity;
	this->limit = this->capacity;
}

ByteBuffer::~ByteBuffer()
{
	if(this->buffer)
    {
        delete [] this->buffer;
    }
}

int ByteBuffer::remaining()
{
	return this->limit - this->position;
}

void ByteBuffer::setIntAt(int intValue, int index)
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

void ByteBuffer::put(const char* bytes, int offset, int len)
{
	if(this->position + len > this->capacity)
    {
		return;
	}
    
	memcpy(this->buffer + this->position, bytes + offset, len);
	this->position += len;
}

void ByteBuffer::putByte(signed char byteValue)
{
	if(this->position + BYTE_SIZE > this->capacity)
    {
		return;
	}
    
	this->buffer[this->position++] = byteValue;
}

void ByteBuffer::putInt(int intValue)
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

void ByteBuffer::putFloat(float floatValue)
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

void ByteBuffer::putShort(short shortValue)
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

void ByteBuffer::putLong(long long longValue)
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

void ByteBuffer::putUTF(const char* strValue)
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

void ByteBuffer::putUTF(const std::string& strValue)
{
	putUTF(strValue.c_str());
}

void ByteBuffer::putArray(std::vector<signed char>& byteV)
{
	putInt(byteV.size());
	for(int i = 0; i < byteV.size(); ++i)
    {
		putByte(byteV[i]);
	}	
}

void ByteBuffer::putArray(std::vector<bool>& boolV)
{
	putInt(boolV.size());
	for(int i = 0; i < boolV.size(); ++i)
    {
		putBool(boolV[i]);
	}	
}

void ByteBuffer::putArray(std::vector<short>& shortV)
{
	putInt(shortV.size());
	for(int i = 0; i < shortV.size(); ++i)
    {
		putShort(shortV[i]);
	}
}

void ByteBuffer::putArray(std::vector<int>& intV)
{
	putInt(intV.size());
	for(int i = 0; i < intV.size(); ++i)
    {
		putInt(intV[i]);
	}
}

void ByteBuffer::putArray(std::vector<long long>& longV)
{
	putInt(longV.size());

	for(int i = 0; i < longV.size(); ++i)
    {
		putLong(longV[i]);
	}
}

void ByteBuffer::putArray(std::vector<std::string>& stringV)
{
	putInt(stringV.size());
	
	for(int i = 0; i < stringV.size(); ++i)
    {
		putUTF(stringV[i]);
	}
}



void ByteBuffer::getArray(std::vector<signed char>& byteV)
{
	int size = getInt();
	byteV.resize(size);
	for(int i = 0; i < size; ++i)
    {
		byteV[i] = getByte();
	}
}

void ByteBuffer::getArray(std::vector<bool>& boolV)
{
	int size = getInt();
	boolV.resize(size);
	for(int i = 0; i < size; ++i)
    {
		boolV[i] = getBool();
	}
}

void ByteBuffer::getArray(std::vector<short>& shortV)
{
	int size = getInt();
	shortV.resize(size);
	for(int i = 0; i< size; ++i)
    {
		shortV[i] = getShort();
	}
}

void ByteBuffer::getArray(std::vector<int>& intV)
{
	int size = getInt();
	intV.resize(size);
	for(int i = 0; i < size; ++i)
    {
		intV[i] = getInt();
	}
}

void ByteBuffer::getArray(std::vector<long long>& longV)
{
	int size = getInt();
	longV.resize(size);
	for(int i = 0; i < size; ++i)
    {
		longV[i] = getLong();
	}
}

void ByteBuffer::getArray(std::vector<std::string>& stringV)
{
	int size =getInt();
	getArray(stringV, size);
}

void ByteBuffer::getArray(std::vector<signed char>& byteV, int size)
{
	byteV.resize(size);
	for(int i = 0; i < size;++i)
    {
		byteV[i] = getByte();
	}
}

void ByteBuffer::getArray(std::vector<bool>& boolV, int size)
{
	boolV.resize(size);
	for(int i = 0; i < size;++i)
    {
		boolV[i] = getBool();
	}	
}

void ByteBuffer::getArray(std::vector<short>& shortV, int size)
{
	shortV.resize(size);
	for(int i = 0; i < size; ++i)
    {
		shortV[i] = getShort();
	}
}

void ByteBuffer::getArray(std::vector<int>& intV, int size)
{
	intV.resize(size);
	for(int i = 0; i < size; ++i)
    {
		intV[i] = getInt();
	}
}

void ByteBuffer::getArray(std::vector<long long>& longV, int size)
{
	longV.resize(size);
	for(int i = 0; i < size; ++i)
    {
		longV[i] = getLong();
	}
}

void ByteBuffer::getArray(std::vector<std::string>& stringV, int size)
{
	stringV.resize(size);
	for(int i = 0; i < size; ++i)
    {
		getUTF(stringV[i]);
	}
}

void ByteBuffer::getUTF(std::string& stringValue)
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

std::string ByteBuffer::getUTF()
{
    std::string str;
	getUTF(str);		
	return str;
}

int ByteBuffer::getPosition()
{
	return this->position;
}

void ByteBuffer::setPosition(int pos)
{
	if(pos > this->limit)
    {
        return;
	}
    
	this->position = pos;
}

int ByteBuffer::getLimit()
{
	return this->limit;
}

int ByteBuffer::getCapacity()
{
	return this->capacity;
}

char* ByteBuffer::getBuffer()
{
	return this->buffer;
}

signed char ByteBuffer::getByte()
{
	if(this->position + BYTE_SIZE > this->limit)
    {
		return 0;
	}
    
	return this->buffer[this->position++];
}

bool ByteBuffer::getBool()
{
	if(this->position + BYTE_SIZE > this->limit)
    {
		return false;
	}
    
	return this->buffer[this->position++] != 0;
}

int ByteBuffer::getLength(int offset)
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

void ByteBuffer::getAsBytes(signed char* bytes)
{
    for(int i = 0; i < 4; i++)
    {
		bytes[i] = this->buffer[this->position++];
	}
}

int ByteBuffer::getInt()
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

float ByteBuffer::getFloat()
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

short ByteBuffer::getShort()
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

long long ByteBuffer::getLong()
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

void ByteBuffer::get(char* bytes, int size)
{
	get(bytes, 0, size);
}

void ByteBuffer::get(char* bytes,int offset,int len)
{
	if(this->position + len > this->limit)
    {
		memset(bytes + offset, 0, len);
		return;
	}
    
	memcpy(bytes + offset, this->buffer + this->position, len);
	this->position += len;
}

void ByteBuffer::clear()
{
	this->position = 0;
	this->limit = this->capacity;
}

void ByteBuffer::flip()
{
	this->limit = this->position;
	this->position = 0;
}

void ByteBuffer::compact()
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

void ByteBuffer::rewind()
{
	this->position = 0;
}

void ByteBuffer::putBool(bool boolValue)
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
std::string ByteBuffer::getLine()
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


