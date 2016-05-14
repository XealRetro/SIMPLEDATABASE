#pragma once
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <string>
#include <vector>
#include <utility>
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

using std::fstream;
using std::ofstream;
using std::ifstream;
using std::ios;
using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::sort;
using std::vector;
using std::swap;

struct RecordInfo
{
	int ID;
	string firstName;
	string lastName;
	string socialSecNum;
	float salary;
	int age;
	bool operator < (const RecordInfo &data) const
	{
		return socialSecNum.size() < data.socialSecNum.size();
	}
}theRecs;

class MiniDataBase
{
public:
	MiniDataBase();
	~MiniDataBase();
	void getInput();
	void indexRecords();
	void addRecord();
	void search();
	void searchSSN(vector<RecordInfo> &myRecs, string key);
	void deleteRecord();
	long countRecords();
	void qSortNum(float numList[], int start, int finish);
	void qSortString(string SSN[], int start, int finish);

private:
	FILE *myFile;
	struct RecordInfo temp;
	vector<RecordInfo> recs;
};

MiniDataBase::MiniDataBase()
{
}

MiniDataBase::~MiniDataBase()
{
}

void MiniDataBase::getInput()
{
	RecordInfo p;
	fstream aFile;
	aFile.open("MiniDataBase.bin", ios::app | ios::binary);
	if (!aFile)
	{
		cout << "Could not open file. " << endl;
		return;
	}
	else if (countRecords() == 12)
	{
		cout << "You can only have 12 records in the database: \n";
		return;
	}
	else
	{
		p.ID = rand() % 999999;
		cout << "Random ID tag: " << p.ID << " created." << endl;
		cout << "Please enter a first name: ";
		cin >> p.firstName;
		while (p.firstName.size() > 20)
		{
			cout << "The first name entered is too long. Please enter one less then 20 characters\n";
			cin >> p.firstName;
		}
		cout << "Please enter a last name: ";
		cin >> p.lastName;
		while (p.lastName.size() > 20)
		{
			cout << "The last name entered is too long. Please enter one less then 20 characters\n";
			cin >> p.lastName;
		}
		cout << "Please enter social security number (in the format xxx-xx-xxxx): ";
		cin >> p.socialSecNum;
		while (p.socialSecNum.size() > 11 || p.socialSecNum.size() < 11 || p.socialSecNum[3] != '-' || p.socialSecNum[6] != '-')
		{
			cout << "The format if the SSN entered is incorrect. Please enter it again: \n";
			cin >> p.socialSecNum;
		}
		cout << "Please enter your salary: ";
		cin >> p.salary;
		cout << "Please enter your age: ";
		cin >> p.age;
		aFile.write((char *)&p, sizeof(struct RecordInfo));
		aFile.close();
		return;
	}
}

void MiniDataBase::search()
{
	long recordNum = countRecords();
	const int nameLength = 20;
	RecordInfo *s;
	s = new RecordInfo[recordNum];
	string searchSocial;
	cout << "Please enter a Social Security Number to search for (in the format xxx-xx-xxxx): ";
	cin >> searchSocial;
	while (searchSocial.size() > 11 || searchSocial.size() < 11 || searchSocial[3] != '-' || searchSocial[6] != '-')
	{
		cout << "The format if the SSN entered is incorrect. Please enter it again: \n";
		cin >> searchSocial;
	}
	myFile = fopen("MiniDataBase.bin", "rb+");
	string fileName = "MiniDataBase.bin";
	ifstream fin(fileName.c_str(), ios::binary);

	cout << "\n";
	int i = 0;
	while (fin.read((char *)&s[i], sizeof(RecordInfo)))
	{
		i++;
	}

	recs.push_back(RecordInfo());
	for (int i = 0; i < recordNum; i++)
	{
		recs[i].ID = s[i].ID;
		recs[i].firstName = s[i].firstName;
		recs[i].lastName = s[i].lastName;
		recs[i].socialSecNum = s[i].socialSecNum;
		recs[i].salary = s[i].salary;
		recs[i].age = s[i].age;
		recs.push_back(RecordInfo());
	}

	/*for (int j = 0; j < recs.size(); j++)
	{
	cout << recs[j].ID << endl;
	cout << recs[j].firstName << endl;
	cout << recs[j].lastName << endl;
	cout << recs[j].socialSecNum << endl;
	cout << recs[j].salary << endl;
	cout << recs[j].age << endl;
	}*/

	sort(recs.begin(), recs.end());

	/*
	for (int j = 0; j < recordNum; j++)
	{
	cout << s[j].ID << endl;
	cout << s[j].firstName << endl;
	cout << s[j].lastName << endl;
	cout << s[j].socialSecNum << endl;
	cout << s[j].salary << endl;
	cout << s[j].age << endl;
	cout << endl << endl;
	}*/

	searchSSN(recs, searchSocial);
	//delete[] s;
}

void MiniDataBase::searchSSN(vector<RecordInfo> &myRecs, string key)
{
	int start = 0;
	int end = myRecs.size() - 1;
	while (start < end)
	{
		int middle = (start + end) / 2;
		int comp = myRecs[middle].socialSecNum.compare(key);
		if (comp < 0)
		{
			start = middle + 1;
		}
		else if (comp > 0)
		{
			end = middle;
		}
		else
		{
			cout << "SSN found!\n";
			cout << "ID#: " << myRecs[middle].ID << endl;
			cout << "First Name: " << myRecs[middle].firstName << endl;
			cout << "Last Name: " << myRecs[middle].lastName << endl;
			cout << "SSN#: " << myRecs[middle].socialSecNum << endl;
			cout << "Salary: $" << myRecs[middle].salary << endl;
			cout << "Age: " << myRecs[middle].age << " years old. " << endl;
			return;
		}
	}
}

void MiniDataBase::indexRecords()
{
	int printView;		//Shall the data be displayed in ascending or descending order?
	int indexRec;		//Field to be indexed
	long countRec = countRecords();
	float *numRecList = NULL;
	string *sList = NULL;
	fstream aFile;
	aFile.open("MiniDataBase.bin", ios::in | ios::binary);
	if (!aFile)
	{
		cout << "Could not open file. " << endl;
		return;
	}
	else
	{
		cout << "Please enter the field you want to index\n";
		cout << "1 for the Social Security Number of each member in the file\n";
		cout << "2 for the Identification number of each member in the file\n";
		cout << "3 for the age of each member in the file\n";
		cout << "4 for the first name of every member in the file\n";
		cout << "5 for the last name of every member in the file\n";
		cout << "6 for the salary of every member\n";
		cout << "Enter index field: ";
		cin >> indexRec;
		while (indexRec > 6 || indexRec < 1)
		{
			cout << "That is out of scope. Please enter a correct index: ";
			cin >> indexRec;
		}
		cout << "Please enter if you want the daat to be displayed in ascending\nor descending order\n";
		cout << "0 for ascending\n";
		cout << "1 for descending\n";
		cin >> printView;
		while (printView > 1 || printView < 0)
		{
			cout << "That is out of scope. Please enter a correct index: ";
			cin >> indexRec;
		}

		if (indexRec == 1)
		{
			sList = new string[countRec];
			for (int i = 0; i < countRec; i++)
			{
				aFile.read((char *)&theRecs, sizeof(struct RecordInfo));
				sList[i] = theRecs.socialSecNum;
			}
			aFile.close();
			qSortString(sList, 0, countRec - 1);
			if (printView == 0)
			{
				for (int i = 0; i < countRec; i++)
				{
					cout << "SSN#: " << sList[i] << endl;
				}
				delete[] sList;
				sList = NULL;
				return;
			}
			else
			{
				for (int i = countRec - 1; i >= 0; i--)
				{
					cout << "SSN#: " << sList[i] << endl;
				}
				delete[] sList;
				sList = NULL;
				return;
			}
		}
		else if (indexRec == 2)
		{
			numRecList = new float[countRec];
			for (int i = 0; i < countRec; i++)
			{
				aFile.read((char *)&theRecs, sizeof(struct RecordInfo));
				numRecList[i] = theRecs.ID;
			}
			aFile.close();

			qSortNum(numRecList, 0, countRec - 1);

			if (printView == 0)
			{
				for (int i = 0; i < countRec; i++)
				{
					cout << "ID#: " << numRecList[i] << endl;
				}
				delete[] numRecList;
				numRecList - NULL;
				return;
			}
			else
			{
				for (int i = countRec - 1; i >= 0; i--)
				{
					cout << "ID#: " << numRecList[i] << endl;
				}
				delete[] numRecList;
				numRecList = NULL;
				return;
			}
		}
		else if (indexRec == 3)
		{
			numRecList = new float[countRec];
			for (int i = 0; i < countRec; i++)
			{
				aFile.read((char *)&theRecs, sizeof(struct RecordInfo));
				numRecList[i] = theRecs.age;
			}
			aFile.close();

			qSortNum(numRecList, 0, countRec - 1);
			if (printView == 0)
			{
				for (int i = 0; i < countRec; i++)
				{
					cout << "Age: " << numRecList[i] << " years old." << endl;
				}
				delete[] numRecList;
				numRecList - NULL;
				return;
			}
			else
			{
				for (int i = countRec - 1; i >= 0; i--)
				{
					cout << "Age: " << numRecList[i] << " years old." << endl;
				}
				delete[] numRecList;
				numRecList = NULL;
				return;
			}
		}
		else if (indexRec == 4)
		{
			sList = new string[countRec];
			for (int i = 0; i < countRec; i++)
			{
				aFile.read((char *)&theRecs, sizeof(struct RecordInfo));
				sList[i] = theRecs.firstName;
			}
			aFile.close();
			qSortString(sList, 0, countRec - 1);
			if (printView == 0)
			{
				for (int i = 0; i < countRec; i++)
				{
					cout << "First Name: " << sList[i] << endl;
				}
				delete[] sList;
				sList = NULL;
				return;
			}
			else
			{
				for (int i = countRec - 1; i >= 0; i--)
				{
					cout << "First Name: " << sList[i] << endl;
				}
				delete[] sList;
				sList = NULL;
				return;
			}
		}
		else if (indexRec == 5)
		{
			sList = new string[countRec];
			for (int i = 0; i < countRec; i++)
			{
				aFile.read((char *)&theRecs, sizeof(struct RecordInfo));
				sList[i] = theRecs.lastName;
			}
			aFile.close();
			qSortString(sList, 0, countRec - 1);
			if (printView == 0)
			{
				for (int i = 0; i < countRec; i++)
				{
					cout << "Last Name: " << sList[i] << endl;
				}
				delete[] sList;
				sList = NULL;
				return;
			}
			else
			{
				for (int i = countRec - 1; i >= 0; i--)
				{
					cout << "Last Name: " << sList[i] << endl;
				}
				delete[] sList;
				sList = NULL;
				return;
			}
		}
		else if (indexRec == 6)
		{
			numRecList = new float[countRec];
			for (int i = 0; i < countRec; i++)
			{
				aFile.read((char *)&theRecs, sizeof(struct RecordInfo));
				numRecList[i] = theRecs.salary;
			}
			aFile.close();
			qSortNum(numRecList, 0, countRec - 1);
			if (printView == 0)
			{
				for (int i = 0; i < countRec; i++)
				{
					cout << "Salary: $" << numRecList[i] << endl;
				}
				delete[] numRecList;
				numRecList = NULL;
				return;
			}
			else
			{
				for (int i = countRec - 1; i >= 0; i--)
				{
					cout << "Salary: $" << numRecList[i] << endl;
				}
				delete[] numRecList;
				numRecList = NULL;
				return;
			}
		}
	}
}

void MiniDataBase::deleteRecord()
{
	FILE *newFile;
	int found = 0;
	int theID;
	myFile = fopen("MiniDataBase.bin", "rb+");
	if (!myFile)
	{
		cout << "Could not open file!\n";
		return;
	}

	newFile = fopen("temp.bin", "wb+");
	if (!newFile)
	{
		cout << "Could not open temp file.\n";
		return;
	}

	cout << "Please enter a last name of a record you want to delete: ";
	cin >> theID;
	while (fread(&theRecs, sizeof(struct RecordInfo), 1, myFile) != NULL)
	{
		if (theID == theRecs.ID)
		{
			cout << "The record with the requested name had been found and deleted.\n\n";
			found = 1;
		}
		else
		{
			fwrite(&theRecs, sizeof(struct RecordInfo), 1, newFile);
		}
	}
	if (!found)
	{
		cout << "No such record was found.\n\n";
	}

	fclose(myFile);
	fclose(newFile);

	remove("MiniDataBase.bin");
	rename("temp.bin", "MiniDataBase.bin");
	return;
}

void MiniDataBase::qSortNum(float numList[], int start, int finish)
{
	if (start < finish)
	{
		int left = start + 1;
		int right = finish;
		int p = numList[start];

		while (left < right)
		{
			if (numList[left] <= p)
			{
				left++;
			}
			else if (numList[right] >= p)
			{
				right--;
			}
			else
			{
				swap(numList[left], numList[right]);
			}
		}

		if (numList[left] < p)
		{
			swap(numList[left], numList[start]);
			left--;
		}
		else
		{
			left--;
			swap(numList[left], numList[start]);
		}

		qSortNum(numList, start, left);
		qSortNum(numList, right, finish);
	}
}

void MiniDataBase::qSortString(string SSN[], int start, int finish)
{
	int left = start;
	int right = finish;
	string pivot = SSN[finish];
	string temp;
	while (left < right)
	{
		while ((SSN[left].compare(pivot) < 0) && (left < right))
		{
			left++;
		}
		while ((SSN[right].compare(pivot) > 0) && (right < left))
		{
			right--;
		}
		if (left <= right)
		{
			temp = SSN[left];
			SSN[left] = SSN[right];
			SSN[right] = temp;
			left++;
			right--;
		}
	}

	if (start < right)
	{
		qSortString(SSN, start, right);
	}
	if (left < finish)
	{
		qSortString(SSN, left, finish);
	}
}

long MiniDataBase::countRecords()
{
	long size;
	myFile = fopen("MiniDataBase.bin", "rb+");
	if (!myFile)
	{
		cout << "Could not open file. " << endl;
		return 0;
	}
	else
	{
		fseek(myFile, 0, SEEK_END);
		size = ftell(myFile);
		long recordSize = size / sizeof(struct RecordInfo);
		//cout << "Number of records in file: " << recordSize << endl;
		return recordSize;
	}
}