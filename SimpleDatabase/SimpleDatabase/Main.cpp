#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include "DatabaseHead.h"
#include <iostream>

int main()
{
	MiniDataBase test;
	srand(time(NULL));
	int count = 0;
	test.countRecords();

	//test.getInput();
		//count++;

	test.search();

	cout << "Done\n";
	getchar();
	getchar();
	return 0;
}