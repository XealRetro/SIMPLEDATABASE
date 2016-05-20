#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include "DatabaseHead.h"

int main(void)
{
	MiniDataBase test;
	srand(time(NULL));
	int selection[12];

	cout << "**********SIMPLE DATABASE MENU**********" << endl;
	cout << "1. Add Record" << endl;
	cout << "2. Delete Record" << endl;
	cout << "3. Search Records" << endl;
	cout << "4. Display Records" << endl;
	cout << "5. Index Records" << endl;
	cout << "Please enter your selections into the queue (maximum of 12)" << endl;
	for (int w = 0; w < 12; w++)
	{
		cout << "Press enter after every selection" << endl;
		cin >> selection[w];
		while (selection[w] > 5 || selection[w] < 1)
		{
			cout << "Incorrect selection. Please try again!" << endl;
			cin >> selection[w];
		}
	}

	for (int e = 0; e < 12; e++)
	{
		if (selection[e] == 1)
		{
			test.getInput();
		}
		if (selection[e] == 2)
		{
			test.deleteRecord();
		}
		if (selection[e] == 3)
		{
			test.search();
		}
		if (selection[e] == 4)
		{
			test.displayRecords();
		}
		if (selection[e] == 5)
		{
			test.indexRecords();
		}
	}

	//test.getInput();
	//test.search();
	//test.indexRecords();
	//cout << "\n\n";
	//test.displayRecords();
	//test.deleteRecord();
	cout << "Done" << endl;;
	getchar();

	return 0;
}
