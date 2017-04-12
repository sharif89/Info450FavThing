// FavoriteThings.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <conio.h>
#include <list>

using namespace std;

const int READERROR = 100;
const int WRITEERROR = 200;
const int ARRAYSIZE = 100;


class Sneakers
{
	string sneakerName;
	string sneakerSize;
	string sneakerBrand;
	string sneakerColor;
	string sneakerPopularity;
	


public:
	Sneakers();
	Sneakers(string name, string size, string brand, string color, string popularity);
	void captureSneaker();
	void showSneaker();
	int saveSneaker(ofstream& outfile);
	bool sneakerDuplicate(Sneakers* name);
	friend class sneakerList;
};



class sneakerList
{
protected:
	Sneakers** list;
	int numrecords;
	int listsize;
	int reallocateArray();

public:
	sneakerList();
	~sneakerList();
	void getUserSneaker();
	void showSneakerList();
	int saveSneakerList(string filepath);
	int readSneakerList(string filepath);
	void addSneaker();
};



Sneakers::Sneakers()
{
	sneakerName = "";
	sneakerSize = "";
	sneakerBrand = "";
	sneakerColor = "";
	sneakerPopularity = "";
	
}


Sneakers::Sneakers(string name, string size, string brand, string color, string popularity)
{
	sneakerName = name;
	sneakerSize = size;
	sneakerBrand = brand;
	sneakerColor = color;
	sneakerPopularity = popularity;
	
}



bool Sneakers::sneakerDuplicate(Sneakers* name)
{
	if (sneakerName == name->sneakerName && sneakerBrand == name->sneakerBrand)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


void Sneakers::captureSneaker()
{
	cout << "Sneaker Name:  ";
	getline(cin, sneakerName);
	cout << "Sneaker Size:  ";
	getline(cin, sneakerSize);
	cout << "Sneaker Brand:  ";
	getline(cin, sneakerBrand);
	cout << "Sneaker Color:  ";
	getline(cin, sneakerColor);
	cout << "Sneaker Popularity (Hi/Med/Lo):  ";
	getline(cin, sneakerPopularity);
	
}


void Sneakers::showSneaker()
{
	cout << "\n______________________\n";
	cout << "Name: " << sneakerName << "\nSneaker Size: " << sneakerSize << "\nSneaker Brand: " << sneakerBrand
		<< "\nSneaker Color: " << sneakerColor << "\nSneaker Popularity " << sneakerPopularity << endl;
}



int Sneakers::saveSneaker(ofstream& outfile)
{
	if (outfile.is_open())
	{
		outfile << sneakerName << "|" << sneakerSize << "|" << sneakerBrand << "|" << sneakerColor << "|" << sneakerPopularity << endl;
		return 0;
	}
	else
		return WRITEERROR;
}


sneakerList::sneakerList()
{
	list = new Sneakers*[ARRAYSIZE];
	numrecords = 0;
	listsize = ARRAYSIZE;
}



sneakerList::~sneakerList()
{
	for (int i = 0; i < numrecords; i++)
	{
		delete list[i];
	}
	delete[]list;
}



int sneakerList::reallocateArray()
{
	Sneakers** temp;
	temp = new Sneakers*[listsize + ARRAYSIZE];
	listsize = listsize + ARRAYSIZE;
	for (int i = 0; i < numrecords; i++)
	{
		temp[i] = list[i];
	}
	delete[]list;
	list = temp;
	return 0;
}


void sneakerList::addSneaker()
{
	Sneakers* snkr;
	snkr = new Sneakers();
	bool snkrExist = false;

	snkr->captureSneaker();

	for (int i = 0; i < numrecords; i++)
	{
		if (list[i]->sneakerDuplicate(snkr))

		{
			cout << "\nSneaker exist in records. Please enter a new sneaker to add to list. Thanks." << endl;
			snkrExist = true;
		}
	}
	if (snkrExist == false)
	{
		list[numrecords] = snkr;
		numrecords++;
	}
}



void sneakerList::getUserSneaker()
{
	string answer = "Y";
	cout << "\nWould you like to add a sneaker to your list? (Y/N)" << endl;
	getline(cin, answer);
	while (answer == "Y" || answer == "y")
	{
		Sneakers* snkr = new Sneakers();
		addSneaker();
		cout << "\nWould you like to add a another sneaker to your list? (Y/N)" << endl;
		getline(cin, answer);
	}
}


void sneakerList::showSneakerList()
{
	for (int i = 0; i < numrecords; i++)
	{
		list[i]->showSneaker();
	}
}



int sneakerList::saveSneakerList(string filepath)
{
	ofstream output(filepath, ios::trunc);
	if (output)
	{
		for (int i = 0; i < numrecords; i++)
		{
			list[i]->saveSneaker(output);
		}
		output.close();
	}
	else
	{
		return WRITEERROR;
	}

	return 0;
}


int sneakerList::readSneakerList(string filepath)
{
	string iname, isize, ibrand, icolor, ipopularity;
	int count = 0;
	ifstream infile(filepath, ios::in);
	if (!infile)
	{
		cout << "File could not be opened" << endl;
		return READERROR;
	}
	while (!infile.eof())
	{
		if (numrecords == listsize)
		{
			reallocateArray();
		}
		getline(infile, iname, '|');
		if (!iname.empty())
		{
			getline(infile, isize, '|');
			getline(infile, ibrand, '|');
			getline(infile, icolor, '|');
			getline(infile, ipopularity);
			
			list[numrecords] = new Sneakers(iname, isize, ibrand, icolor, ipopularity);
			numrecords++;
		}
	}
	infile.close();
	return 0;
}


int main()
{
	sneakerList snkrlist;

	int error;
	string answer;
	string filepath;

	cout << "Create a list of your favorite sneakers! " << endl;
	cout << "To begin, enter a path to the file you wish to save on." << endl;
	

	
	getline(cin, filepath);
	ifstream file(filepath);
	error = snkrlist.readSneakerList(filepath);
	if (error)
	{
		cout << "File does not exist or the file path is incorrect, please re-enter a correct file path" << endl;
		
		getline(cin, filepath);
		ifstream file(filepath);
		error = snkrlist.readSneakerList(filepath);

		
	}

	
		snkrlist.getUserSneaker();
		snkrlist.saveSneakerList(filepath);
		snkrlist.showSneakerList();

		std::getchar();
		return 0;
	
}