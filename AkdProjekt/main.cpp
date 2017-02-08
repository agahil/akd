#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<iostream>
#include "PackedString.h"

int main(int argc, char* argv[])
{
	PackedString ps("abracadabradabraca");
	int s = ps.size() + 1;
	char* tab = new char[s];
	strcpy(tab, ps.c_str());
	char c = ps[1];
	PackedString copy;
	PackedString newPs("ala");
	ps.substr(5, 10);
	//copy = ps;
	//copy += newPs;
	if (!ps.empty())
		printf("Wielkosc: " + ps.size());
	auto tmp = ps.find("cad");

	ps.clear();
	if (!newPs.empty())
		printf("Wielkosc: " + newPs.size());
	newPs.clear();
	getchar();
	return 0;
}