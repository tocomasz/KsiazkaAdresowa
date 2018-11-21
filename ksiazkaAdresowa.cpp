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

int loadContactsFromFile(vector<Person> &contacts);
void saveContactsToFile(Person person);
int addNewContact(vector<Person> &contacts, int registeredContactsCount);
int deleteContact(vector<Person> &contacts, int registeredContactsCount);
void deleteContactFromFile(Person person);
void editContact(vector <Person> &contacts);
void updateContactInFile(Person person);
void printContact(Person person);
void printHeaderRow();
void printAllContacts(vector<Person> &contacts, int registeredContactsCount);
void findContactByName(vector<Person> &contacts, int registeredContactsCount);
void findContactByLastName(vector<Person> &contacts, int registeredContactsCount);

int main()
{
	vector <Person> contacts;
	char menuChoice;
	int registeredContactsCount = 0;
	registeredContactsCount = loadContactsFromFile(contacts);
	while (1)
	{
		system("cls");
		cout << "1. Dodaj adresata" << endl;
		cout << "2. Wyszukaj po imieniu" << endl;
		cout << "3. Wyszukaj po nazwisku" << endl;
		cout << "4. Wyswietl wszystkich adresatow" << endl;
		cout << "5. Usun adresata" << endl;
		cout << "6. Edytuj adresata" << endl;
		cout << "9. Zakoncz program" << endl;
		cout << "Twoj wybor: ";

		cin >> menuChoice;
		switch (menuChoice)
		{
		case '1': registeredContactsCount = addNewContact(contacts, registeredContactsCount); break;
		case '2': findContactByName(contacts, registeredContactsCount); break;
		case '3': findContactByLastName(contacts, registeredContactsCount); break;
		case '4': printAllContacts(contacts, registeredContactsCount); break;
		case '5': registeredContactsCount = deleteContact(contacts, registeredContactsCount); break;
		case '6': editContact(contacts); break;
		case '9': exit(0); break;
		default: break;
		}
	}
	return 0;
}

int loadContactsFromFile(vector<Person> &contacts)
{
	int registeredContactsCount = 0;
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

	if (registeredContactsCount == 0)
		temporaryPerson.id = ++registeredContactsCount;
	else
	{
		temporaryPerson.id = contacts.back().id + 1;
		registeredContactsCount++;
	}

	contacts.push_back(temporaryPerson);
	saveContactsToFile(temporaryPerson);
	return registeredContactsCount;
}

int deleteContact(vector<Person> &contacts, int registeredContactsCount)
{
	system("cls");
	cout << "Podaj nr ID kontaktu do usuniecia: ";
	int contactToDelete;
	cin >> contactToDelete;
	for (vector<Person>::iterator it = contacts.begin(), end = contacts.end(); it != end; ++it)
	{
		if (it->id == contactToDelete)
		{
			printHeaderRow();
			printContact(*it);
			cout << endl;
			char choice;
			do
			{
				cout << "Czy na pewno chcesz usunac tego adresata? t/n :";
				cin >> choice;
			} while (choice != 't' && choice != 'n');

			if (choice == 't')
			{
				deleteContactFromFile(*it);
				contacts.erase(it);
				registeredContactsCount--;
			}
			system("pause");
			return registeredContactsCount;
		}
	}
	cout << "Nie znaleziono danego adresata. Nacisnij dowolny przycisk by powrocic do menu" << endl;
	system("pause");
	return registeredContactsCount;
}

void deleteContactFromFile(Person person)
{
	fstream file, temp;
	file.open("dane_adresowe.txt", ios::in);
	temp.open("temp.txt", ios::out);
	if (file.good() && temp.good())
	{
		string loadedLine;
		while (getline(file, loadedLine))
		{
			if (loadedLine[0] - '0' == person.id)
				continue;
			temp << loadedLine << endl;
		}
	}
	file.close();
	file.clear();
	temp.close();
	temp.clear();
	remove("dane_adresowe.txt");
	rename("temp.txt", "dane_adresowe.txt");
}

void editContact(vector<Person> &contacts)
{
	system("cls");
	cout << "Podaj nr ID kontaktu do edycji: ";
	int contactToDelete;
	cin >> contactToDelete;
	for (vector<Person>::iterator it = contacts.begin(), end = contacts.end(); it != end; ++it)
	{
		if (it->id == contactToDelete)
		{
			int choice;
			while(1)
			{
				system("cls");
				printHeaderRow();
				printContact(*it);
				cout << endl;
				cout << "Wybierz ktora dana zmienic: " << endl;
				cout << "1 - imie\n2 - nazwisko\n3 - nr telefonu\n4 - email\n5 - adres\n6 - powrot do menu" << endl;;
				cin >> choice;
				string change;
				switch (choice)
				{
				case 1: cout << "Podaj imie: "; cin.ignore(); getline(cin, it->firstName); break;
				case 2: cout << "Podaj nazwisko: "; cin.ignore(); getline(cin, it->lastName); break;
				case 3: cout << "Podaj nr telefonu: "; cin.ignore(); getline(cin, it->nrTel); break;
				case 4: cout << "Podaj email: "; cin.ignore(); getline(cin, it->email); break;
				case 5: cout << "Podaj adres: "; cin.ignore(); getline(cin, it->adres); break;
				}
				if (choice == 6)
					return;
				updateContactInFile(*it);
			}
		}
	}
	cout << "Nie znaleziono danego adresata. Nacisnij dowolny przycisk by powrocic do menu" << endl;
	system("pause");
}

void updateContactInFile(Person person)
{
	fstream file, temp;
	file.open("dane_adresowe.txt", ios::in);
	temp.open("temp.txt", ios::out);
	if (file.good() && temp.good())
	{
		string loadedLine;
		while (getline(file, loadedLine))
		{
			if (loadedLine[0] - '0' == person.id)
			{
				temp << person.id << "|";
				temp << person.firstName << "|";
				temp << person.lastName << "|";
				temp << person.nrTel << "|";
				temp << person.email << "|";
				temp << person.adres << "|" << endl;
				continue;
			}	
			temp << loadedLine << endl;
		}
	}
	file.close();
	file.clear();
	temp.close();
	temp.clear();
	remove("dane_adresowe.txt");
	rename("temp.txt", "dane_adresowe.txt");
}

void printContact(Person person)
{
	cout << setw(8) << left << person.id;
	cout << setw(12) << left << person.firstName;
	cout << setw(12) << left << person.lastName;
	cout << setw(12) << left << person.nrTel;
	cout << setw(20) << left << person.email;
	cout << setw(30) << left << person.adres << endl;
}

void printHeaderRow()
{
	system("cls");
	cout << setw(8) << left << "[NR ID]";
	cout << setw(12) << left << "[IMIE]";
	cout << setw(12) << left << "[NAZWISKO]";
	cout << setw(12) << left << "[NR TEL]";
	cout << setw(20) << left << "[EMAIL]";
	cout << setw(30) << left << "[ADRES]" << endl;
}

void printAllContacts(vector<Person> &contacts, int registeredContactsCount)
{
	printHeaderRow();
	for (int i = 0; i < registeredContactsCount; i++)
	{
		printContact(contacts[i]);
	}
	system("pause");
}

void findContactByName(vector<Person> &contacts, int registeredContactsCount)
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

void findContactByLastName(vector<Person> &contacts, int registeredContactsCount)
{
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
}