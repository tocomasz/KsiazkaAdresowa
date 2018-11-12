// ksiazkaAdresowa.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

struct Person
{
	int id;
	string firstName;
	string lastName;
	string nrTel;
	string email;
	string adres;
};

int loadContactsFromFile(Person *contacts, int registeredContactsCount);
void saveContactsToFile(Person person);
int addNewContact(Person *contacts, int registeredContactsCount);
void wyswietl(Person person);
void printHeaderRow();
void findContact(Person *contacts, int registeredContactsCount);

int main()
{
	Person contacts[1000];
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


int loadContactsFromFile(Person *contacts, int registeredContactsCount)
{
	fstream file;
	file.open("dane_adresowe.txt", ios::in);
	if (!file.good())
		return 0;
	else
	{
		string loadedLine;
		int loadedLineCounter = 1;
		while (getline(file, loadedLine))
		{
			switch (loadedLineCounter)
			{
			case 1: contacts[registeredContactsCount].id = atoi(loadedLine.c_str()); break;
			case 2: contacts[registeredContactsCount].firstName = loadedLine; break;
			case 3: contacts[registeredContactsCount].lastName = loadedLine; break;
			case 4: contacts[registeredContactsCount].nrTel = loadedLine; break;
			case 5: contacts[registeredContactsCount].email = loadedLine; break;
			case 6: contacts[registeredContactsCount].adres = loadedLine; break;
			}
			loadedLineCounter++;
			if (loadedLineCounter == 7)
			{
				loadedLineCounter = 1;
				registeredContactsCount++;
			}
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
		file << person.id << endl;
		file << person.firstName << endl;
		file << person.lastName << endl;
		file << person.nrTel << endl;
		file << person.email << endl;
		file << person.adres << endl;
		file.close();
		file.clear();
	}
	else
	{
		cout << "Blad zapisu pliku" << endl;
		system("pause");
	}
	
}

int addNewContact(Person *contacts, int registeredContactsCount)
{
	string firstName, lastName, nrTel, email, adres;

	system("cls");
	cout << "Podaj imie: ";
	cin >> firstName;
	cout << "Podaj nazwisko: ";
	cin >> lastName;
	cout << "Podaj nr telefonu: ";
	cin >> nrTel;
	cout << "Podaj adres email: ";
	cin >> email;
	cout << "Podaj adres: ";
	cin.ignore();
	getline(cin, adres);

	contacts[registeredContactsCount].id = registeredContactsCount+1;
	contacts[registeredContactsCount].firstName = firstName;
	contacts[registeredContactsCount].lastName = lastName;
	contacts[registeredContactsCount].nrTel = nrTel;
	contacts[registeredContactsCount].email = email;
	contacts[registeredContactsCount].adres = adres;
	saveContactsToFile(contacts[registeredContactsCount]);

	return registeredContactsCount + 1;
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

void findContact(Person *contacts, int registeredContactsCount)
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




