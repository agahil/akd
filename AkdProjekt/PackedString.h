#pragma once
#include<stdio.h>
#include<iostream>
#include<string>
#include<map>
#include<list>

// Maksymalna wielkosc pliku
#define MAX_LENGTH     (1 << 20)

// Parametry kompresji
// Maksymalne przesuniecie
#define MAX_OFFSET_BITS     15
#define MAX_OFFSET          (1 << MAX_OFFSET_BITS)
// Minimalne dopasowanie
#define MIN_LENGTH          6
#define MIN_LENGTH_TO_COMPRESS 10
// Zlozenie 3 kolejnych bajtow z sekwencji wejsciowej
#define GET_HASH(x)     ((buffer[(x)] << 16) | (buffer[(x)+1] << 8) | (buffer[(x)+2]))
#define FIRST_CHAR 0

class PackedString :
	public std::string
{
private:
	char character;
	unsigned char character1;
	const int limitValue = MIN_LENGTH_TO_COMPRESS;
	char*value;
	unsigned char *buffer; //dane do zakodowania
	int buf_size;//wielkoœæ niezakodowanego tekstu
	int encoded_buf_size; //wielkoœæ zakodowanego tekstu
	int decodedDataLength; //d³ugoœæ niezakodowanego tekstu
	int encodedDataLength; //d³ugoœæ zakodowanego tekstu
	int allDataLength; //d³ugoœæ zakodowowanego + niezakodowanego je¿eli taki jest
	unsigned char *encodedData;
	unsigned char *decodedData;
	int encodedDataIndex; //aktualny indeks zakodowanego tekstu
	int decodedDataIndex; //aktualny indeks niezakodowanego tekstu
	int buf_pos;
	bool isEncoded;
	std::map <int, std::list<int> > map;

	void beforeEncode(int length);
	void lzssEncode(void);
	void lzssDecode();

	void writePair(unsigned char letter);
	void writeTripple(unsigned int offset, unsigned int length);
	void addToText(unsigned char *text, int offset, int length, int index);

public:

	PackedString();
	PackedString(const std::string& s);
	PackedString(const PackedString& s);
	PackedString(size_type length, const char& ch);
	PackedString(const char* str);
	PackedString(const char* str, size_type length);
	//
	PackedString(const std::string& str, size_type index, size_type length);
	//PackedString(std::input_iterator_tag start, std::input_iterator_tag end);
	~PackedString();

	PackedString& operator=(const std::string& s);
	PackedString& operator=(const PackedString& s);
	PackedString& operator=(const char* s);
	PackedString& operator=(char ch);
	char& operator[](size_type index);

	PackedString& operator+=(const std::string& s);
	PackedString& operator+=(const PackedString& s);
	PackedString& operator+=(const char* s);
	PackedString& operator+=(char ch);


	const char* c_str();

	bool empty();
	size_t size();
	void clear();
	void substr(int pos, int count);
	int find(char* s);

};

