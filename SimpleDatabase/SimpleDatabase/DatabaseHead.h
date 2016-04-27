#pragma once
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <string> 
#include <vector>
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#define FIRST_NAME_SIZE 20
#define LAST_NAME_SIZE 20
#define SSN 12
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
using std::getline;
using std::vector;
using std::binary_search;

struct RecordInfo
{
	int ID;
	char firstName[FIRST_NAME_SIZE];
	char lastName[LAST_NAME_SIZE];	
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
	void createFile();
	void getInput();
	void displayPartRecord();
	void addRecord();
	void search();
	void searchSSN(vector<RecordInfo> &myRecs,  string key);
	void deleteRecord();
	long countRecords();
	bool cmpSSN(RecordInfo const& lhs, RecordInfo const& rhs);
	
private:
	FILE *myFile;
	struct RecordInfo temp;
	vector<RecordInfo> recs;

	

}; MiniDataBase mnb_Obj;

MiniDataBase::MiniDataBase()
{
	
}

MiniDataBase::~MiniDataBase()
{

}	

void MiniDataBase::createFile()
{
	char ch = 'y';
	myFile = fopen("MiniDataBase.bin", "wb");
	while(ch == 'y' || ch == 'Y')
	{
		
	}
}

void MiniDataBase::getInput()
{
		RecordInfo p;
		//myFile = fopen("MiniDataBase.bin", "ab+");
		fstream aFile;
		aFile.open("MiniDataBase.bin", ios::app | ios::binary);
		if(!aFile)
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
			cout << "Please enter a last name: ";
			cin >> p.lastName;
			cout << "Please enter social security number (in the format xxx-xx-xxxx): ";
			cin >> p.socialSecNum;
			cout << "Please enter your salary: ";
			cin >> p.salary;
			cout << "Please enter your age: ";
			cin >> p.age;
			aFile.write((char *)&p, sizeof(struct RecordInfo));
			aFile.close();
			return;
		}
	
	

}

bool MiniDataBase::cmpSSN(RecordInfo const& lhs, RecordInfo const& rhs)
{
	return lhs.socialSecNum.size() < rhs.socialSecNum.size();
}

void MiniDataBase::search()
{
	countRecords();
	long recordNum = countRecords();
	const int nameLength = 20;
	RecordInfo *s;
	s = new RecordInfo[recordNum];
	string searchSocial;
	cout << "Please enter a Social Security Number to search for (in the format xxx-xx-xxxx): ";
	cin >> searchSocial;
	myFile = fopen("MiniDataBase.bin", "rb");
	string fileName = "MiniDataBase.bin";
	ifstream fin(fileName.c_str(), ios::binary);
	
	cout << "\n\n";
	int i = 0;
	while (fin.read((char *) &s[i], sizeof(RecordInfo)))
	{
		i++;
	}

	recs.push_back(RecordInfo());
	for (int i = 0; i < recordNum; i++)
	{
		recs[i].ID = s[i].ID;
		strcpy(recs[i].firstName, s[i].firstName);
		strcpy(recs[i].lastName, s[i].lastName);
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

}

void MiniDataBase::searchSSN(vector<RecordInfo> &myRecs, string key)
{
	
	int start = 0;
	int end = myRecs.size() + 1;
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
			return;
		}
	}

}



long MiniDataBase::countRecords()
{
	long size;	
	myFile = fopen("MiniDataBase.bin", "rb");
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

