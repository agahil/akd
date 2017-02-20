#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<iostream>
#include "PackedString.h"

int main(int argc, char* argv[])
{
	PackedString ps("abracadabradabraca abracadabradabraca abracadabradabraca abracadabradabraca abracadabradabraca abracadabradabraca abracadabradabracau");
	int findPos;
	if (!ps.empty())
		findPos = ps.find("cad");

	//Pobranie d³uoœci przechowywanych danych
	int s = ps.size() + 1;
	char* tab = new char[s];

	//Wykorzystanie funkcji c_str() 
	strcpy(tab, ps.c_str());
	
	//Wykorzystanie operatora indeksowania
	char c = ps[1];
	PackedString copy;
	PackedString newPs("ala");

	//Wyszukiwanie elementów w ci¹gu
	if (!ps.empty())
		findPos = ps.find("cad");

	ps.substr(5, 10);

	//Operatory
	copy = ps;
	copy += newPs;

	ps.clear();
	newPs.clear();
	

	return 0;
}