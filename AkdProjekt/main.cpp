#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<iostream>
#include "PackedString.h"

int main(int argc, char* argv[])
{
	PackedString ps("abracadabradabraca");
	char c = ps[1];
	PackedString copy;
	PackedString newPs("ala");
	copy = ps;
	copy += newPs;

	getchar();
	return 0;
}