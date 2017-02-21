#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<iostream>
#include "PackedString.h"

int main(int argc, char* argv[])
{
	PackedString ps("abracadabradabraca abracadabradabraca abracadabradabraca abracadabradabraca abracadabradabraca abracadabradabraca abracadabradabracau");
	PackedString newPs("ala");
	PackedString copy;
	//Operatory
	copy = ps;
	copy += newPs;
	//konstruktor kopiuj¹cy
	PackedString pp(ps);
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
	PackedString copyString;


	ps.substr(5, 10);
	copyString = "lala";



	ps.clear();
	newPs.clear();
	

	return 0;
}