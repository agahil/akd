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
		character = value[index];
		buffer = new unsigned char[MAX_LENGTH];
		for (int i = 0; i < length; i++)
		{
			buffer[i] = value[i];
		}

		beforeEncode(length);
		lzssEncode();
		return character;
	}
	return value[index];
}

PackedString::PackedString() :std::string()
{
	encoded_buf_size = 0;
	decodedDataLength = 0;
	encodedDataLength = 0;
	allDataLength = 0;
	encodedDataIndex = 0;
	decodedDataIndex = 0;
	buf_pos = 0;

}
PackedString::PackedString(const std::string& s):PackedString::PackedString()
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
PackedString::PackedString(const PackedString& s) :PackedString::PackedString()
{
	value = new char[MAX_LENGTH];
	this->buf_pos = s.buf_pos;
	this->buf_size = s.buf_size;
	this->value = s.value;
}

PackedString::PackedString(size_type length, const char& ch) :PackedString::PackedString()
{
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
PackedString::PackedString(const char* str) :PackedString::PackedString()
{
	int len = strlen(str);
	decodedData = new unsigned char[len+1];
	decodedDataLength = len;
	memcpy(decodedData, str, len);
	decodedData[len] = '\0';

	if (len > limitValue)
	{
		buffer = decodedData;
		//memcpy(buffer, decodedData, len);
		beforeEncode(len);
		lzssEncode();
		//delete[] decodedData; //je¿eli tworzona jest kopia w buffer to odkomentowaæ
		decodedData = NULL;
	}
}
PackedString::PackedString(const char* str, size_type length) :PackedString::PackedString()
{
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
PackedString::PackedString(const std::string& str, size_type index, size_type length) :PackedString::PackedString()
{
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
		delete[]value;
}
const char* PackedString::c_str()
{
	if (isEncoded)
	{
		buf_size = MAX_LENGTH;
		lzssDecode();
	}
	char* strPtr = new char[MAX_LENGTH];
	sprintf_s(strPtr, MAX_LENGTH, "%s%s", value,'\0');

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

	return strPtr;
}
/////////////////////////////LZSS encoding and decoding//////////////////////////////////////////////
void PackedString::beforeEncode(int length)
{
	value = new char[MAX_LENGTH];
	buf_pos = 0;
	buf_size = length;
	decodedDataLength = length;
	encodedData = new unsigned char[length * 2]; //*2 ze wzglêdu kodowania binarnego: przypadek t
	encodedDataIndex = 0;
	for (std::map <int, std::list<int> >::iterator it = map.begin(); it != map.end(); it)
	{
		auto tmp = (*it).second;
		tmp.clear();
		(*it).second = tmp;
		it = map.erase(it);
	}
}
void PackedString::lzssEncode(void)
{
	// do oprogramowania
	int hashx = 0;
	
	while (buf_pos < decodedDataLength)
	{
		int buf_pos_iter = buf_pos;
		char actualChar = buffer[buf_pos];

		//sprawdzenie dopasowania
		bool isCorrect = false;
		int maxLength = 1;
		int maxLengthIndex = 0;
		if (buf_pos > 3)
		{
			hashx = GET_HASH(buf_pos);
			auto iterr = map.find(hashx);
			if (iterr != map.end())
			{
				std::list<int> tmpList = map.find(hashx)->second;
				int tmp = tmpList.size();
				int maxAndLastIndex = buf_pos;
				if (tmp > 0)
				{
					maxLength = 3;
					maxLengthIndex = *(tmpList.begin());
				}
				for (std::list<int>::iterator it = tmpList.begin(); it != tmpList.end(); it++)
				{
					int tmpPosition = buf_pos + 3;
					int indexPrevPosition = *it;
					int tmpIndexPrev = indexPrevPosition + 3;
					int tmpPrevMaxLength = 3;

					if (buf_pos - indexPrevPosition >= MAX_OFFSET)
					{
						break;
					}
					while (buffer[tmpIndexPrev] == buffer[tmpPosition] && tmpIndexPrev < buf_pos)
					{
						tmpIndexPrev++;
						tmpPosition++;
						tmpPrevMaxLength++;
					}

					if (tmpPrevMaxLength > maxLength)
					{
						maxLength = tmpPrevMaxLength;
						maxLengthIndex = indexPrevPosition;
					}
					if (maxLength > MAX_LENGTH)
					{
						break;
					}
					if (maxLength >= MIN_LENGTH)
					{
						isCorrect = true;
					}
				}
			}
		}

		//zapis wyjscia
		if (isCorrect)
		{
			writeTripple(buf_pos - maxLengthIndex, maxLength);
		}
		else
		{
			maxLength = 1;
			if (buf_pos == 0)
			{
				if (buffer[0] == FIRST_CHAR)
				{
					writeTripple(1, 1);
				}
				else
				{
					writePair(buffer[0]);
				}
			}
			else
			{
				writePair(buffer[buf_pos]);
			}
		}
		buf_pos += maxLength;

		//dodanie do tab hash

		while (buf_pos_iter < buf_pos)
		{
			hashx = GET_HASH(buf_pos_iter);
			map[hashx].push_front(buf_pos_iter);
			buf_pos_iter++;
		}
	}
	delete[]buffer;
	encodedDataLength = decodedDataLength;
	decodedDataLength = 0;
	isEncoded = true;
}
void PackedString::writePair(unsigned char letter)
{
	encodedData[encodedDataIndex] = 1;
	encodedDataIndex++;
	encodedData[encodedDataIndex] = letter;
	encodedDataIndex++;

//	sprintf_s(value, MAX_LENGTH, "%1d %3d ", 1, (int)letter);
}


void PackedString::writeTripple(unsigned int offset, unsigned int length)
{
	unsigned short int off = (unsigned short int) offset;
	unsigned short int len = (unsigned short int) length;
	encodedData[encodedDataIndex] = 0;
	encodedDataIndex++;

	encodedData[encodedDataIndex] = (off >> 8) & 0xFF;
	encodedDataIndex++;
	encodedData[encodedDataIndex] = (off) & 0xFF;
	encodedDataIndex++;

	encodedData[encodedDataIndex] = (len >> 8) & 0xFF;
	encodedDataIndex++;
	encodedData[encodedDataIndex] = (len) & 0xFF;
	encodedDataIndex++;
//	sprintf_s(value, MAX_LENGTH, "%1d %7d %3d ", 0, offset, length);
}


// Dekompresja LZSS
void PackedString::lzssDecode()
{
	if (encodedDataLength == 0)
		return;
	if (decodedData != NULL)
		delete[] decodedData;
	decodedData = new unsigned char[encodedDataLength + 1];
	int index = 0;
	unsigned char flag = 0;
	unsigned char decodedChar = 0;
	while (index < encodedDataIndex)
	{
		flag = encodedData[index++];
		if (flag == 1)
		{
			int num = 0;
			decodedChar = encodedData[index++];
			decodedData[decodedDataIndex] = decodedChar;
			decodedDataIndex++;
		}
		else if (flag == 0)
		{
			int offset = 0;
			int length = 0;
			offset = (encodedData[index++] << 8) & 0xFF;
			offset = (encodedData[index++]) & 0xFF;
			length = (encodedData[index++] << 8) & 0xFF;
			length = (encodedData[index++]) & 0xFF;
			addToText(decodedData, offset, length, decodedDataIndex);
			decodedDataIndex += length;
		}
	}
	decodedDataLength = decodedDataIndex;
	decodedData[decodedDataIndex++] = '\0';
	encodedDataLength = 0;
	delete[] encodedData;
	encodedData = NULL;
	isEncoded = false;
}



void PackedString::addToText(unsigned char *text, int offset, int length, int index)
{

	int patternPos = index - offset;
	for (int i = 0; i < length; i++, patternPos++)
	{
		text[index + i] = text[patternPos];
	}
}

bool PackedString::empty()
{
	if (decodedDataLength == 0 && encodedDataLength == 0)
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
		if(value != NULL)
			value ="";
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

	//value = buff;
	if (sizeToSub > this->limitValue)
	{
		buffer = new unsigned char[MAX_LENGTH];
		for (int i = 0; i < sizeToSub; i++)
		{
			this->buffer[i] = buff[i];
		}
		beforeEncode(sizeToSub);
		/*for (std::map <int, std::list<int> >::iterator it = map.begin(); it != map.end(); it)
		{
			auto tmp = (*it).second;
			tmp.clear();
			(*it).second = tmp;

			it = map.erase(it);
		}*/
		lzssEncode();
	}

	delete[] buff;

}

int PackedString::find(char* s)
{
	if (isEncoded)
	{
		buf_size = MAX_LENGTH;
		lzssDecode();
	}
	int resultIndex = -1;
	int iIndex = 0;
	int jIndex = 0;
	int slength = strlen(s);
	for (iIndex = 0; iIndex < decodedDataLength; iIndex++)
	{
		for (jIndex = 0; (jIndex < slength) && (iIndex + slength < decodedDataLength); jIndex++)
		{
			if (decodedData[iIndex + jIndex] != s[jIndex])
			{
				break;
			}
		}
		if (jIndex == slength)
		{
			resultIndex = iIndex;
			break;
		}
	}
	

	if (decodedDataLength > this->limitValue)
	{
		buffer = decodedData;
		beforeEncode(decodedDataLength);
		lzssEncode();
		decodedData = NULL;
	}

	return resultIndex;
}