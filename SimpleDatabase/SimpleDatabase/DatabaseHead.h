#pragma once
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <string>
#include <vector>
#include <iomanip>
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
using std::setprecision;
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
	void displayRecords();
	void indexRecords();
	void search();
	void deleteRecord();
	long countRecords();
	void qSortNum(float numList[], int start, int finish);	//QuickSort for age, salary, and ID numbers
	void qSortString(string SSN[], int start, int finish); //QuickSort for Social Security, First and Last names
	void searchSSN(vector<RecordInfo> &myRecs, string key);
	void setIndexNumber(int indexNumber);					//Set number of record value to be index
	int getIndexNumber();
	void setIDArr(int *id);									//These are the functions  to hold the arrays that will be indexed inorder to display them as they were indexed
	int *getIDArr();
	void setFNameArr(string *fArr);
	string *getFNameArr();
	void setLNameArr(string *lArr);
	string *getLNameArr();
	void setSSNArr(string *ssn);
	string *getSSNArr();
	void setSalArr(float *sal);
	float *getSalArr();
	void setAgeArr(int *age);
	int *getAgeArr();

private:
	FILE *myFile;
	struct RecordInfo temp;
	vector<RecordInfo> recs;
	int indexNum;
	int *idArr;
	string *strFArr;
	string *strLArr;
	string *ssnArr;
	float *salArr;
	int *ageArr;
};

MiniDataBase::MiniDataBase()
{
	indexNum = 0;
	string *strFArr = NULL;
	string *strLArr = NULL;
	string *ssnArr = NULL;
	float *salArr = NULL;
	int *ageArr = NULL;
}

MiniDataBase::~MiniDataBase()
{
	delete[] strFArr;
	delete[] strLArr;
	delete[] ssnArr;
	delete[] salArr;
	delete[] ageArr;
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
	if (!myFile)
	{
		cout << "Could not open file!" << endl;
		return;
	}
	else if (countRecords() == 0)
	{
		cout << "No records in file. Nothing can be searched!" << endl;
		return;
	}
	else
	{
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
}

void MiniDataBase::searchSSN(vector<RecordInfo> &myRecs, string key)
{
	cout << "Nothing will be shown if the record is not found." << endl;
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
		else if (comp == 0)
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
	myFile = fopen("MiniDataBase.bin", "rb+");
	if (!myFile)
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
		RecordInfo *holdR;
		holdR = new RecordInfo[countRec];
		int p = 0;
		while (fread((char *)&holdR[p], sizeof(RecordInfo), 1, myFile))
		{
			p++;
		}
		recs.push_back(RecordInfo());
		for (int k = 0; k < countRec; k++)
		{
			recs[k].ID = holdR[k].ID;
			recs[k].firstName = holdR[k].firstName;
			recs[k].lastName = holdR[k].lastName;
			recs[k].socialSecNum = holdR[k].socialSecNum;
			recs[k].salary = holdR[k].salary;
			recs[k].age = holdR[k].age;
			recs.push_back(RecordInfo());
		}
		if (indexRec == 1)
		{
			sList = new string[countRec];
			ssnArr = new string[countRec];
			for (int i = 0; i < countRec; i++)
			{
				sList[i] = recs[i].socialSecNum;
			}
			fclose(myFile);

			qSortString(sList, 0, countRec - 1);
			if (printView == 0)
			{
				for (int i = 0; i < countRec; i++)
				{
					cout << "SSN#: " << sList[i] << endl;
					ssnArr[i] = sList[i];
				}
				setSSNArr(ssnArr);
				delete[] sList;
				sList = NULL;
				setIndexNumber(1);
				return;
			}
			else
			{
				for (int i = countRec - 1; i >= 0; i--)
				{
					cout << "SSN#: " << sList[i] << endl;
					ssnArr[i] = sList[i];
				}
				setSSNArr(ssnArr);
				delete[] sList;
				sList = NULL;
				setIndexNumber(1);
				return;
			}
		}
		else if (indexRec == 2)
		{
			numRecList = new float[countRec];
			idArr = new int[countRec];
			for (int i = 0; i < countRec; i++)
			{
				numRecList[i] = recs[i].ID;
			}
			fclose(myFile);

			qSortNum(numRecList, 0, countRec - 1);

			if (printView == 0)
			{
				for (int i = 0; i < countRec; i++)
				{
					cout << "ID#: " << numRecList[i] << endl;
					idArr[i] = numRecList[i];
				}
				setIDArr(idArr);
				delete[] numRecList;
				numRecList = NULL;
				setIndexNumber(2);
				return;
			}
			else
			{
				for (int i = countRec - 1; i >= 0; i--)
				{
					cout << "ID#: " << numRecList[i] << endl;
					idArr[i] = numRecList[i];
				}
				setIDArr(idArr);
				delete[] numRecList;
				numRecList = NULL;
				setIndexNumber(2);
				return;
			}
		}
		else if (indexRec == 3)
		{
			numRecList = new float[countRec];
			ageArr = new int[countRec];
			for (int i = 0; i < countRec; i++)
			{
				sList[i] = recs[i].age;
			}
			fclose(myFile);

			qSortString(sList, 0, countRec - 1);
			if (printView == 0)
			{
				for (int i = 0; i < countRec; i++)
				{
					cout << "Age: " << numRecList[i] << " years old." << endl;
					ageArr[i] = numRecList[i];
				}
				setAgeArr(ageArr);
				delete[] numRecList;
				numRecList = NULL;
				setIndexNumber(3);
				return;
			}
			else
			{
				for (int i = countRec - 1; i >= 0; i--)
				{
					cout << "Age: " << numRecList[i] << " years old." << endl;
					ageArr[i] = numRecList[i];
				}
				setAgeArr(ageArr);
				delete[] numRecList;
				numRecList = NULL;
				setIndexNumber(3);
				return;
			}
		}
		else if (indexRec == 4)
		{
			sList = new string[countRec];
			strFArr = new string[countRec];
			for (int i = 0; i < countRec; i++)
			{
				sList[i] = recs[i].firstName;
			}
			fclose(myFile);
			qSortString(sList, 0, countRec - 1);
			if (printView == 0)
			{
				for (int i = 0; i < countRec; i++)
				{
					cout << "First Name: " << sList[i] << endl;
					strFArr[i] = sList[i];
				}
				setFNameArr(strFArr);
				delete[] sList;
				sList = NULL;
				setIndexNumber(4);
				return;
			}
			else
			{
				for (int i = countRec - 1; i >= 0; i--)
				{
					cout << "First Name: " << sList[i] << endl;
					strFArr[i] = sList[i];
				}
				setFNameArr(strFArr);
				delete[] sList;
				sList = NULL;
				setIndexNumber(4);
				return;
			}
		}
		else if (indexRec == 5)
		{
			sList = new string[countRec];
			strLArr = new string[countRec];
			for (int i = 0; i < countRec; i++)
			{
				sList[i] = recs[i].lastName;
			}
			fclose(myFile);
			qSortString(sList, 0, countRec - 1);
			if (printView == 0)
			{
				for (int i = 0; i < countRec; i++)
				{
					cout << "Last Name: " << sList[i] << endl;
					strLArr[i] = sList[i];
				}
				setLNameArr(strLArr);
				delete[] sList;
				sList = NULL;
				setIndexNumber(5);
				return;
			}
			else
			{
				for (int i = countRec - 1; i >= 0; i--)
				{
					cout << "Last Name: " << sList[i] << endl;
					strLArr[i] = sList[i];
				}
				setLNameArr(strLArr);
				delete[] sList;
				sList = NULL;
				setIndexNumber(6);
				return;
			}
		}
		else if (indexRec == 6)
		{
			numRecList = new float[countRec];
			salArr = new float[countRec];
			for (int i = 0; i < countRec; i++)
			{
				numRecList[i] = recs[i].salary;
			}
			fclose(myFile);
			qSortNum(numRecList, 0, countRec - 1);
			if (printView == 0)
			{
				for (int i = 0; i < countRec; i++)
				{
					cout << "Salary: $" << numRecList[i] << endl;
					salArr[i] = numRecList[i];
				}
				setSalArr(salArr);
				delete[] numRecList;
				numRecList = NULL;
				setIndexNumber(6);
				return;
			}
			else
			{
				for (int i = countRec - 1; i >= 0; i--)
				{
					cout << "Salary: $" << numRecList[i] << endl;
				}
				setSalArr(salArr);
				delete[] numRecList;
				numRecList = NULL;
				setIndexNumber(6);
				return;
			}
			//if (indexRec == 1)
			//{
			//	sList = new string[countRec];
			//	ssnArr = new string[countRec];
			//	for (int i = 0; i < countRec; i++)
			//	{
			//		fread(&theRecs, sizeof(struct RecordInfo), 1, myFile);
			//		sList[i] = theRecs.socialSecNum;
			//	}
			//	fclose(myFile);
			//	qSortString(sList, 0, countRec - 1);
			//	if (printView == 0)
			//	{
			//		for (int i = 0; i < countRec; i++)
			//		{
			//			cout << "SSN#: " << sList[i] << endl;
			//			ssnArr[i] = sList[i];
			//		}
			//		setSSNArr(ssnArr);
			//		//delete[] sList;
			//		sList = NULL;
			//		setIndexNumber(1);
			//		return;
			//	}
			//	else
			//	{
			//		for (int i = countRec - 1; i >= 0; i--)
			//		{
			//			cout << "SSN#: " << sList[i] << endl;
			//			ssnArr[i] = sList[i];
			//		}
			//		setSSNArr(ssnArr);
			//		//delete[] sList;
			//		sList = NULL;
			//		setIndexNumber(1);
			//		return;
			//	}
			//}
			/*else if (indexRec == 2)
			{
				numRecList = new float[countRec];
				idArr = new int[countRec];
				for (int i = 0; i < countRec; i++)
				{
					fread(&theRecs, sizeof(struct RecordInfo), 1, myFile);
					numRecList[i] = theRecs.ID;
				}
				fclose(myFile);

				qSortNum(numRecList, 0, countRec - 1);

				if (printView == 0)
				{
					for (int i = 0; i < countRec; i++)
					{
						cout << "ID#: " << numRecList[i] << endl;
						idArr[i] = numRecList[i];
					}
					setIDArr(idArr);
					delete[] numRecList;
					numRecList = NULL;
					setIndexNumber(2);
					return;
				}
				else
				{
					for (int i = countRec - 1; i >= 0; i--)
					{
						cout << "ID#: " << numRecList[i] << endl;
						idArr[i] = numRecList[i];
					}
					setIDArr(idArr);
					delete[] numRecList;
					numRecList = NULL;
					setIndexNumber(2);
					return;
				}
			}
			else if (indexRec == 3)
			{
				numRecList = new float[countRec];
				for (int i = 0; i < countRec; i++)
				{
					fread(&theRecs, sizeof(struct RecordInfo), 1, myFile);
					numRecList[i] = theRecs.age;
				}
				fclose(myFile);

				qSortNum(numRecList, 0, countRec - 1);
				if (printView == 0)
				{
					for (int i = 0; i < countRec; i++)
					{
						cout << "Age: " << numRecList[i] << " years old." << endl;
					}
					delete[] numRecList;
					numRecList = NULL;
					setIndexNumber(3);
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
					setIndexNumber(3);
					return;
				}
			}
			else if (indexRec == 4)
			{
				sList = new string[countRec];
				for (int i = 0; i < countRec; i++)
				{
					fread(&theRecs, sizeof(struct RecordInfo), 1, myFile);
					sList[i] = theRecs.firstName;
				}
				fclose(myFile);
				qSortString(sList, 0, countRec - 1);
				if (printView == 0)
				{
					for (int i = 0; i < countRec; i++)
					{
						cout << "First Name: " << sList[i] << endl;
					}
					delete[] sList;
					sList = NULL;
					setIndexNumber(4);
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
					setIndexNumber(4);
					return;
				}
			}
			else if (indexRec == 5)
			{
				sList = new string[countRec];
				for (int i = 0; i < countRec; i++)
				{
					fread(&theRecs, sizeof(struct RecordInfo), 1, myFile);
					sList[i] = theRecs.lastName;
				}
				fclose(myFile);
				qSortString(sList, 0, countRec - 1);
				if (printView == 0)
				{
					for (int i = 0; i < countRec; i++)
					{
						cout << "Last Name: " << sList[i] << endl;
					}
					delete[] sList;
					sList = NULL;
					setIndexNumber(5);
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
					setIndexNumber(6);
					return;
				}
			}
			else if (indexRec == 6)
			{
				numRecList = new float[countRec];
				for (int i = 0; i < countRec; i++)
				{
					fread(&theRecs, sizeof(struct RecordInfo), 1, myFile);
					numRecList[i] = theRecs.salary;
				}
				fclose(myFile);
				qSortNum(numRecList, 0, countRec - 1);
				if (printView == 0)
				{
					for (int i = 0; i < countRec; i++)
					{
						cout << "Salary: $" << numRecList[i] << endl;
					}
					delete[] numRecList;
					numRecList = NULL;
					setIndexNumber(6);
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
					setIndexNumber(6);
					return;
				}
			}*/
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

	cout << "Please enter an ID of a record you want to delete: ";
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

void MiniDataBase::setIndexNumber(int indexNumber)
{
	this->indexNum = indexNumber;
}

int MiniDataBase::getIndexNumber()
{
	return this->indexNum;
}

void MiniDataBase::displayRecords()
{
	long thisNum = countRecords();
	RecordInfo *s;
	s = new RecordInfo[thisNum];
	myFile = fopen("MiniDataBase.bin", "rb+");
	string fileName = "MiniDataBase.bin";
	ifstream fin(fileName.c_str(), ios::binary);
	if (!myFile)
	{
		cout << "File not found!" << endl;
		return;
	}
	else if (countRecords() == 0)
	{
		cout << "No records to display!" << endl;
		return;
	}
	else {
		int i = 0;
		while (fin.read((char *)&s[i], sizeof(RecordInfo)))
		{
			i++;
		}

		recs.push_back(RecordInfo());
		for (int i = 0; i < countRecords(); i++)
		{
			recs[i].ID = s[i].ID;
			recs[i].firstName = s[i].firstName;
			recs[i].lastName = s[i].lastName;
			recs[i].socialSecNum = s[i].socialSecNum;
			recs[i].salary = s[i].salary;
			recs[i].age = s[i].age;
			recs.push_back(RecordInfo());
		}
		fclose(myFile);
		if (getIndexNumber() == 1)
		{
			string *temp;
			temp = getSSNArr();
			for (int j = 0; j < recs.size(); j++)
			{
				cout << "#SSN: " << recs[j].socialSecNum << endl;
				cout << "ID#: " << recs[j].ID << endl;
				cout << "First Name: " << recs[j].firstName << endl;
				cout << "Last Name: " << recs[j].lastName << endl;
				cout << "$: " << setprecision(8) << recs[j].salary << endl;
				cout << recs[j].age << " years old. " << endl;
				cout << endl;
			}
		}
		else if (getIndexNumber() == 2)
		{
			int *test;
			test = getIDArr();

			for (int j = 0; j < recs.size(); j++)
			{
				cout << "ID#: " << recs[j].ID << endl;
				cout << "First Name: " << recs[j].firstName << endl;
				cout << "Last Name: " << recs[j].lastName << endl;
				cout << "#SSN: " << recs[j].socialSecNum << endl;
				cout << "$: " << setprecision(8) << recs[j].salary << endl;
				cout << recs[j].age << " years old. " << endl;
				cout << endl;
			}
		}
		else if (getIndexNumber() == 3)
		{
			int *temp;
			temp = getAgeArr();

			for (int j = 0; j < recs.size(); j++)
			{
				cout << recs[j].age << " years old. " << endl;
				cout << "ID#: " << recs[j].ID << endl;
				cout << "First Name: " << recs[j].firstName << endl;
				cout << "Last Name: " << recs[j].lastName << endl;
				cout << "#SSN: " << recs[j].socialSecNum << endl;
				cout << "$: " << setprecision(8) << recs[j].salary << endl;
				cout << endl;
			}
		}
		else if (getIndexNumber() == 4)
		{
			string *temp;
			temp = getFNameArr();
			for (int j = 0; j < recs.size(); j++)
			{
				cout << "First Name: " << recs[j].firstName << endl;
				cout << "Last Name: " << recs[j].lastName << endl;
				cout << "ID#: " << recs[j].ID << endl;
				cout << "#SSN: " << recs[j].socialSecNum << endl;
				cout << "$: " << setprecision(8) << recs[j].salary << endl;
				cout << recs[j].age << " years old. " << endl;
				cout << endl;
			}
		}
		else if (getIndexNumber() == 5)
		{
			string *temp;
			temp = getLNameArr();
			for (int j = 0; j < recs.size(); j++)
			{
				cout << "Last Name: " << recs[j].lastName << endl;
				cout << "First Name: " << recs[j].firstName << endl;
				cout << "ID#: " << recs[j].ID << endl;
				cout << "#SSN: " << recs[j].socialSecNum << endl;
				cout << "$: " << setprecision(8) << recs[j].salary << endl;
				cout << recs[j].age << " years old. " << endl;
				cout << endl;
			}
		}
		else if (getIndexNumber() == 6)
		{
			float *temp;
			temp = getSalArr();
			for (int j = 0; j < recs.size(); j++)
			{
				cout << "$: " << setprecision(8) << recs[j].salary << endl;
				cout << "ID#: " << recs[j].ID << endl;
				cout << "First Name: " << recs[j].firstName << endl;
				cout << "Last Name: " << recs[j].lastName << endl;
				cout << "#SSN: " << recs[j].socialSecNum << endl;
				cout << recs[j].age << " years old. " << endl;
				cout << endl;
			}
		}

		//delete[] s;
		//s = NULL;
	}
}

void MiniDataBase::setIDArr(int *id)
{
	this->idArr = id;
}

int * MiniDataBase::getIDArr()
{
	return this->idArr;
}

void MiniDataBase::setFNameArr(string *fName)
{
	this->strFArr = fName;
}

string * MiniDataBase::getFNameArr()
{
	return this->strFArr;
}

void MiniDataBase::setLNameArr(string *lName)
{
	this->strLArr = lName;
}

string * MiniDataBase::getLNameArr()
{
	return this->strLArr;
}

void MiniDataBase::setSSNArr(string *ssn)
{
	this->ssnArr = ssn;
}

string * MiniDataBase::getSSNArr()
{
	return new string[20];
}

void MiniDataBase::setSalArr(float *sal)
{
	this->salArr = sal;
}

float * MiniDataBase::getSalArr()
{
	return this->salArr;
}

void MiniDataBase::setAgeArr(int *age)
{
	this->ageArr = age;
}

int * MiniDataBase::getAgeArr()
{
	return this->ageArr;
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
