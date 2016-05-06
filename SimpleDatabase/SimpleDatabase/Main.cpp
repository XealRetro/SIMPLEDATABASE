#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include "DatabaseHead.h"

int main(void)
{
	MiniDataBase test;
	srand(time(NULL));
	int count = 0;
	//test.countRecords();

	//test.getInput();
		//count++;

	//test.search();
	test.indexRecords();
	//test.deleteRecord();
	cout << "Done\n";
	getchar();
	getchar();
	return 0;
}