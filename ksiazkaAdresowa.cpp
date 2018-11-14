// ksiazkaAdresowa.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>
#include <iomanip>
#include <vector>

using namespace std;

struct Person
{
	int id;
	string firstName, lastName, nrTel, email, adres;
};

int loadContactsFromFile(vector<Person> &contacts, int registeredContactsCount);
void saveContactsToFile(Person person);
int addNewContact(vector<Person> &contacts, int registeredContactsCount);
void printContact(Person person);
void printHeaderRow();
void findContact(vector<Person> &contacts, int registeredContactsCount);

int main()
{
	vector <Person> contacts;
	char menuChoice;
	int registeredContactsCount = 0;
	registeredContactsCount = loadContactsFromFile(contacts, registeredContactsCount);
	while (1)
	{
		system("cls");
		cout << "1. Dodaj nowa osobe" << endl;
		cout << "2. Szukaj" << endl;
		cout << "9. Zakoncz proogram" << endl;

		cin >> menuChoice;
		if (menuChoice == '1')
		{
			registeredContactsCount = addNewContact(contacts, registeredContactsCount);
		}
		else if (menuChoice == '2')
		{
			findContact(contacts, registeredContactsCount);
		}
		else if (menuChoice == '9')
		{
			exit(0);
		}
	}
	return 0;
}


int loadContactsFromFile(vector<Person> &contacts, int registeredContactsCount)
{
	fstream file;
	file.open("dane_adresowe.txt", ios::in);
	if (!file.good())
		return 0;
	else
	{
		string loadedLine;
		Person temporaryPerson;
		while (getline(file, loadedLine))
		{
			string temporaryText = "";
			int loadedLineCounter = 1;
			for (int i = 0; i < loadedLine.length(); i++)
			{
				if (loadedLine[i] == '|' || loadedLine[i] == '\0')
				{
					switch (loadedLineCounter)
					{
					case 1: temporaryPerson.id = atoi(temporaryText.c_str()); break;
					case 2: temporaryPerson.firstName = temporaryText; break;
					case 3: temporaryPerson.lastName = temporaryText; break;
					case 4: temporaryPerson.nrTel = temporaryText; break;
					case 5: temporaryPerson.email = temporaryText; break;
					case 6: temporaryPerson.adres = temporaryText; break;
					}
					temporaryText = "";
					loadedLineCounter++;
				}
				else
					temporaryText += loadedLine[i];
			}
			contacts.push_back(temporaryPerson);
			registeredContactsCount++;
		}
		
	}
	file.close();
	file.clear();
	return registeredContactsCount;
}

void saveContactsToFile(Person person)
{
	fstream file;
	file.open("dane_adresowe.txt", ios::out | ios::app);
	if (file.good())
	{
		file << person.id << "|";
		file << person.firstName << "|";
		file << person.lastName << "|";
		file << person.nrTel << "|";
		file << person.email << "|";
		file << person.adres << "|" << endl;
		file.close();
		file.clear();
	}
	else
	{
		cout << "Blad zapisu pliku" << endl;
		system("pause");
	}
	
}

int addNewContact(vector<Person> &contacts, int registeredContactsCount)
{
	string firstName, lastName, nrTel, email, adres;
	Person temporaryPerson;
	system("cls");
	cout << "Podaj imie: ";
	cin >> temporaryPerson.firstName;
	cout << "Podaj nazwisko: ";
	cin >> temporaryPerson.lastName;
	cout << "Podaj nr telefonu: ";
	cin >> temporaryPerson.nrTel;
	cout << "Podaj adres email: ";
	cin >> temporaryPerson.email;
	cout << "Podaj adres: ";
	cin.ignore();
	getline(cin, temporaryPerson.adres);
	temporaryPerson.id = ++registeredContactsCount;
	contacts.push_back(temporaryPerson);
	saveContactsToFile(temporaryPerson);

	return registeredContactsCount;
}

void printContact(Person person)
{
	cout << setw(12) << left << person.firstName;
	cout << setw(12) << left << person.lastName;
	cout << setw(12) << left << person.nrTel;
	cout << setw(20) << left << person.email;
	cout << setw(30) << left << person.adres << endl;
}

void printHeaderRow()
{
	system("cls");
	cout << "Znaleziono nastepujace osoby: " << endl;
	cout << setw(12) << left << "[IMIE]";
	cout << setw(12) << left << "[NAZWISKO]";
	cout << setw(12) << left << "[NR TEL]";
	cout << setw(20) << left << "[EMAIL]";
	cout << setw(30) << left << "[ADRES]" << endl;
}

void findContact(vector<Person> &contacts, int registeredContactsCount)
{
	char menuChoice;
	while (1)
	{
		system("cls");
		cout << "1. Szukaj po imieniu" << endl;
		cout << "2. Szukaj po nazwisku" << endl;
		cout << "3. Pokaz wszystkich" << endl;
		cout << "4. Powrot do menu glownego" << endl;
		cin >> menuChoice;
		if (menuChoice == '1')
		{
			string firstName;
			system("cls");
			cout << "Podaj imie do wyszukania: ";
			cin >> firstName;

			printHeaderRow();
			for (int i = 0; i < registeredContactsCount; i++)
			{
				if (contacts[i].firstName == firstName)
					printContact(contacts[i]);
			}
			system("pause");
		}
		else if (menuChoice == '2')
		{
			string lastName;
			system("cls");
			cout << "Podaj nazwisko do wyszukania: ";
			cin >> lastName;
			printHeaderRow();
			for (int i = 0; i < registeredContactsCount; i++)
			{
				if (contacts[i].lastName == lastName)
					printContact(contacts[i]);
			}
			system("pause");
		}
		else if (menuChoice == '3')
		{
			printHeaderRow();
			for (int i = 0; i < registeredContactsCount; i++)
			{
				printContact(contacts[i]);
			}
			system("pause");
		}
		else if (menuChoice == '4')
			break;
	}
}
