#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<iostream>
#include "PackedString.h"

int main(int argc, char* argv[])
{
	PackedString ps("abracadabradabraca");
	PackedString copy;
	copy = ps;

	getchar();
	return 0;
}