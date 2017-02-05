#include "PackedString.h"
PackedString& PackedString::operator = (const std::string& s)
{
	if (this->decodedText != s)
	{
		for (int i = 0; i < s.length(); i++)
		{
			this->decodedText[i] = s[i];
		}
	}
	return *this;
}
PackedString& PackedString::operator = (const PackedString& s)
{
	if (this != &s)
	{
		this->value = s.value;
	}
		
	return *this;
}
PackedString& PackedString::operator = (const char* s)
{
	if (this->value != s)
	{
		int len = strlen(s);
		if (strlen(s) < limitValue)
		{
			value = new char[strlen(s) ];
			for (int i = 0; i < strlen(s); i++)
			{
				this->value[i] = s[i];
			}
		}
		else
		{
			buffer = new char[strlen(s) ];
			for (int i = 0; i < strlen(s); i++)
			{
				this->buffer[i] = s[i];
			}
			beforeEncode(len);
			lzssEncode();
		}
	}
	return *this;
}
PackedString& PackedString::operator = (char ch)
{
	this->value[0] = ch;
	return *this;
}
char& PackedString::operator[](size_type index)
{
	if (isEncoded){
		lzssDecode();
		char c = value[index];
		lzssEncode();
		return c;
	}
	return value[index];
}

PackedString::PackedString() :std::string()
{

}
PackedString::PackedString(const std::string& s) 
{
	limitValue = 5;
	
	if (s.length() < limitValue)
	{
		value = new char[s.length()];
		for (int i = 0; i < s.length(); i++)
		{
			this->value[i] = s[i];
		}
	}
	else
	{
		buffer = new char[s.length()];
		for (int i = 0; i < s.length(); i++)
		{
			this->buffer[i] = s[i];
		}
		beforeEncode(s.length());
		lzssEncode();
	}
}
PackedString::PackedString(const PackedString& s)
{
	limitValue = 5;
	value = new char[strlen(s.value) ];
	this->value = s.value;
}
PackedString::PackedString(size_type length, const char& ch) 
{
	limitValue = 5;
	
	if (length < limitValue)
	{
		value = new char[length];
		for (int i = 0; i < length; i++)
		{
			this->value[i] = ch;
		}
	}
	else
	{
		buffer = new char[length];
		for (int i = 0; i < length; i++)
		{
			this->buffer[i] = ch;
		}
		beforeEncode(length);
		lzssEncode();
	}
}
PackedString::PackedString(const char* str) 
{
	limitValue = 5;
	int len = strlen(str);
	if (strlen(str) < limitValue)
	{
		value = new char[strlen(str)];
		for (int i = 0; i < strlen(str) ; i++)
		{
			this->value[i] = str[i];
		}
	}
	else
	{
		buffer = new char[len];
		for (int i = 0; i < strlen(str) ; i++)
		{
			this->buffer[i] = str[i];
		}
		beforeEncode(len);
		lzssEncode();
	}
}
PackedString::PackedString(const char* str, size_type length) 
{
	limitValue = 5;
	if (length < limitValue)
	{
		value = new char[length];
		for (int i = 0; i < length; i++)
		{
			this->value[i] = str[i];
		}
	}
	else
	{
		buffer = new char[length];
		for (int i = 0; i < length; i++)
		{
			this->buffer[i] = str[i];
		}
		beforeEncode(length);
		lzssEncode();
	}
}
PackedString::PackedString(const std::string& str, size_type index, size_type length) 
{
	limitValue = 5;
	if (length < limitValue)
	{
		value = new char[length];
		for (int i = 0; i < length; i++)
		{
			this->value[i] = str[index+i];
		}
	}
	else
	{
		buffer = new char[length];
		for (int i = 0; i < length; i++)
		{
			this->buffer[i] = str[index+i];
		}
		beforeEncode(length);
		lzssEncode();
	}
}



PackedString::~PackedString()
{
	delete[]value;
}


void PackedString::beforeEncode(int length)
{
	value = new char[MAX_LENGTH];
	buf_pos = 0;
	buf_size = length;
}
void PackedString::lzssEncode(void)
{
	int hash_val;
	
	while (buf_pos < buf_size)
	{

		hash_val = GET_HASH(buf_pos);
		map[hash_val].push_back(buf_pos);

		buf_pos++;
	}
	buf_pos = 0;

	while (buf_pos < buf_size)
	{
		int hash_val = GET_HASH(buf_pos);
		if (map[hash_val].size()>1)
		{
			std::list<int>::iterator iter = map[hash_val].begin();
			std::list<int>::iterator prev = map[hash_val].begin();
			int i = *iter;
			int length = 3;

			if (*iter != buf_pos)
			{
				iter++;
				while (*iter != buf_pos)
				{
					iter++;
					prev++;
				}

				int offset = *iter - *prev;
				while (buffer[*prev + length] == buffer[*iter + length])
				{
					if (*prev + 1 < *iter)
						length++;
					else
						break;
				}
				writeTripple(offset, length);
				buf_pos += length;
			}
			else
			{
				writePair(buffer[buf_pos]);
				buf_pos++;
			}
		}
		else
		{
			writePair(buffer[buf_pos]);
			buf_pos++;
		}

	}
	delete[]buffer;
	isEncoded = true;
}
void PackedString::writePair(unsigned char letter)
{

	sprintf_s(value, MAX_LENGTH, "%1d %3d\n", 1, (int)letter);
}


void PackedString::writeTripple(unsigned int offset, unsigned int length)
{
	sprintf_s(value, MAX_LENGTH, "%1d %7d %3d\n", 0, offset, length);
}


// Dekompresja LZSS
void PackedString::lzssDecode()
{
	buffer = new char[buf_size];
	sprintf_s(buffer, buf_size, "%s", value);
	for (int i = 0; i < buf_size; i++)
	{
		value[i] = 0;
	}
	int index = 0;
	int i = 0;
	char character = ' ';
	while (sscanf_s(buffer, "%d", &i) && character != '\0')
	{
		if (i == 1)
		{
			int num = 0;
			sscanf_s(buffer, "%d", &num);
			character = num;
			value[index] = character;
			index++;

		}
		else if (i == 0)
		{
			int offset = 0;
			int length = 0;
			sscanf_s(buffer, "%d %d", &offset, &length);
			addToText(value, offset, length, index);
			index += length;
		}
	}
	isEncoded = false;
}



void PackedString::addToText(char *text, int offset, int length, int index)
{

	int patternPos = index - offset;
	for (int i = 0; i < length; i++, patternPos++)
	{
		text[i + index] = text[patternPos];
	}
}