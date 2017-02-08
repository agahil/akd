#include "PackedString.h"
PackedString& PackedString::operator = (const std::string& s)
{
	if (this->value != s)
	{
		if (s.length() < limitValue)
		{
			value = new char[MAX_LENGTH];
			sprintf_s(value, MAX_LENGTH, "%s", s);

		}
		else
		{
			buffer = new unsigned char[MAX_LENGTH];
			for (int i = 0; i < s.length(); i++)
			{
				this->buffer[i] = s[i];
			}
			beforeEncode(s.length());
			lzssEncode();
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
	this->isEncoded = (s.isEncoded);
	return *this;
}
PackedString& PackedString::operator = (const char* s)
{
	if (this->value != s)
	{
		int len = strlen(s);
		if (strlen(s) < limitValue)
		{
			value = new char[MAX_LENGTH];
			sprintf_s(value, MAX_LENGTH, "%s", s);
		}
		else
		{
			buffer = new unsigned char[MAX_LENGTH];
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
PackedString& PackedString::operator += (const std::string& s)
{
	if (isEncoded)
	{
		buf_size = MAX_LENGTH;
		lzssDecode();
	}

	int length = strlen(value);
	int newLength = strlen(value) + s.length();
	buffer = new unsigned char[MAX_LENGTH];
	for (int i = 0; i < strlen(value); i++)
	{
		this->buffer[i] = value[i];
	}
	for (int i = 0; i < s.length(); i++)
	{
		this->buffer[i + length] = s[i];
	}

	if (strlen(this->value)>limitValue)
	{
		beforeEncode(newLength);
		lzssEncode();
	}
	return *this;
}
PackedString& PackedString::operator += (const PackedString& s)
{
	if (isEncoded)
	{
		buf_size = MAX_LENGTH;
		lzssDecode();
	}
	PackedString tmp = s;
	if (s.isEncoded)
	{
		tmp.buf_size = MAX_LENGTH;
		tmp.lzssDecode();
	}
	int length = strlen(value);
	int newLength = strlen(value) + strlen(tmp.value);
	buffer = new unsigned char[MAX_LENGTH];
	for (int i = 0; i < strlen(value); i++)
	{
		this->buffer[i] = value[i];
	}

	for (int i = 0; i < strlen(tmp.value); i++)
	{
		this->buffer[i + length] = tmp.value[i];
	}

	if (strlen(this->value)>limitValue)
	{
		beforeEncode(newLength);
		lzssEncode();
	}
	return *this;
}
PackedString& PackedString::operator += (const char* s)
{
	if (isEncoded)
	{
		buf_size = MAX_LENGTH;
		lzssDecode();
	}
	int length = strlen(value);
	int newLength = strlen(value) + strlen(s);
	buffer = new unsigned char[MAX_LENGTH];
	for (int i = 0; i < strlen(value); i++)
	{
		this->buffer[i] = value[i];
	}

	for (int i = 0; i < strlen(s); i++)
	{
		this->buffer[i + length] = s[i];
	}

	if (strlen(this->value)>limitValue)
	{
		beforeEncode(newLength);
		lzssEncode();
	}
	return *this;
}
PackedString& PackedString::operator += (char ch)
{
	if (isEncoded)
	{
		buf_size = MAX_LENGTH;
		lzssDecode();
	}
	int newLength = strlen(value) + 1;
	buffer = new unsigned char[newLength];
	for (int i = 0; i < strlen(value); i++)
	{
		this->buffer[i] = value[i];
	}
	delete[]value;
	value = new char[newLength];
	this->buffer[strlen(value)] = ch;

	for (int i = 0; i < strlen(value); i++)
	{
		this->value[i] = buffer[i];
	}

	if (strlen(this->value)>limitValue)
	{
		beforeEncode(newLength);
		lzssEncode();
	}
	return *this;
}
char& PackedString::operator[](size_type index)
{
	if (isEncoded){
		buf_size = MAX_LENGTH;
		lzssDecode();
		int length = strlen(value);
		char c = value[index];
		buffer = new unsigned char[MAX_LENGTH];
		for (int i = 0; i < length; i++)
		{
			buffer[i] = value[i];
		}

		beforeEncode(length);
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
		value = new char[MAX_LENGTH];
		sprintf_s(value, MAX_LENGTH, "%s", s);

	}
	else
	{
		buffer = new unsigned char[MAX_LENGTH];
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
	value = new char[strlen(s.value)];
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
		buffer = new unsigned char[MAX_LENGTH];
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
		value = new char[MAX_LENGTH];
		sprintf_s(value, MAX_LENGTH, "%s", str);
	}
	else
	{
		buffer = new unsigned char[MAX_LENGTH];
		for (int i = 0; i < strlen(str); i++)
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
		value = new char[MAX_LENGTH];
		sprintf_s(value, MAX_LENGTH, "%s", str);
	}
	else
	{
		buffer = new unsigned char[MAX_LENGTH];
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
			this->value[i] = str[index + i];
		}
	}
	else
	{
		buffer = new unsigned char[MAX_LENGTH];
		for (int i = 0; i < length; i++)
		{
			this->buffer[i] = str[index + i];
		}
		beforeEncode(length);
		lzssEncode();
	}
}



PackedString::~PackedString()
{
	if(value != NULL)
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
	int index = 0;
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
				}
				writeTripple(offset, length, index);
				index += 14;
				buf_pos += length;
			}
			else
			{
				writePair(buffer[buf_pos], index);
				index += 6;
				buf_pos++;
			}
		}
		else
		{
			writePair(buffer[buf_pos], index);
			index += 6;
			buf_pos++;
		}

	}
	delete[]buffer;
	isEncoded = true;
}
void PackedString::writePair(unsigned char letter, int index)
{

	sprintf_s(value + index, MAX_LENGTH, "%1d %3d ", 1, (int)letter);
}


void PackedString::writeTripple(unsigned int offset, unsigned int length, int index)
{
	sprintf_s(value + index, MAX_LENGTH, "%1d %7d %3d ", 0, offset, length);
}


// Dekompresja LZSS
void PackedString::lzssDecode()
{
	char *buff;
	buff = new char[MAX_LENGTH];
	sprintf_s(buff, buf_size, "%s", value);

	for (int j = 0; j < strlen(value); j++)
	{
		value[j] = 0;
	}
	int index = 0;
	int i = 0;
	int buffIndex = 0;
	char character = ' ';
	while (sscanf_s(buff + buffIndex, "%d", &i) && character != '\0')
	{
		buffIndex++;
		if (i == 1)
		{
			int num = 0;
			sscanf_s(buff + buffIndex, "%d", &num);
			character = num;
			value[index] = character;
			index++;
			buffIndex += 5;
		}
		else if (i == 0)
		{
			int offset = 0;
			int length = 0;
			sscanf_s(buff + buffIndex, "%d %d", &offset, &length);
			addToText(value, offset, length, index);
			index += length;
			buffIndex += 13;
		}
	}
	delete[]buff;
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

bool PackedString::empty()
{
	if (strlen(value) == 0 && buf_size == 0)
		return true;

	return false;
}

size_t PackedString::size()
{
	if (isEncoded)
		return buf_pos;
	return strlen(value);
}

void PackedString::clear()
{
	if (isEncoded)
	{
		value = "";
		buf_size = 0;
		buf_pos = 0;
		isEncoded = false;
	}
	else
	{
		delete[] value;
	}
}

void PackedString::substr(int pos, int count)
{
	int sizeToSub = count;
	if (isEncoded)
	{
		buf_size = MAX_LENGTH;
		lzssDecode();
	}
	if (strlen(value) - pos < sizeToSub)
	{
		sizeToSub = strlen(value) - pos;
	}

	char *buff = new char[sizeToSub];
	memcpy(buff, value + pos, count);

	value = buff;
	if (strlen(value) > this->limitValue)
	{
		buffer = new unsigned char[MAX_LENGTH];
		for (int i = 0; i < sizeToSub; i++)
		{
			this->buffer[i] = buff[i];
		}
		beforeEncode(sizeToSub);
		lzssEncode();
	}
}

char* PackedString::find(char* s)
{
	if (isEncoded)
	{
		buf_size = MAX_LENGTH;
		lzssDecode();
	}
	auto result = std::strstr(value, s);

	if (strlen(value) > this->limitValue)
	{
		buffer = new unsigned char[MAX_LENGTH];
		for (int i = 0; i < strlen(value); i++)
		{
			this->buffer[i] = value[i];
		}
		beforeEncode(strlen(value));
		lzssEncode();
	}

	return result;
}