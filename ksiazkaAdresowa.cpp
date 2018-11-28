
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

struct User
{
	int id;
	string username, password;
};


int logIn(vector <User> &users, int registeredUsers);
int registerUser(vector <User> &users, int registeredUser);
int loadUsersFromFile(vector <User> &users);
void saveUserToFile(User user);
int loadContactsFromFile(vector<Person> &contacts);
void saveContactsToFile(Person person);
int addNewContact(vector<Person> &contacts, int registeredContacts);
int deleteContact(vector<Person> &contacts, int registeredContacts);
void deleteContactFromFile(Person person);
void editContact(vector <Person> &contacts);
void updateContactInFile(Person person);
void printContact(Person person);
void printHeaderRow();
void printAllContacts(vector<Person> &contacts, int registeredContacts);
void findContactByName(vector<Person> &contacts, int registeredContacts);
void findContactByLastName(vector<Person> &contacts, int registeredContacts);

int main()
{
	vector <Person> contacts;
	vector <User> users;
	int registeredContacts = 0;
	int registeredUsers = 0;
	registeredUsers = loadUsersFromFile(users);
	int loggedUser = 0;
	char menuChoice;


	while (1)
	{

		while (loggedUser == 0)
		{
			system("cls");
			cout << "1. Logowanie" << endl;
			cout << "2. Rejestracja" << endl;
			cout << "9. Zakoncz program" << endl;
			cout << "Twoj wybor: ";

			cin >> menuChoice;
			switch (menuChoice)
			{
			case '1': loggedUser = logIn(users, registeredUsers); break;
			case '2': registeredUsers = registerUser(users, registeredUsers); break;

			case '9': exit(0); break;
			}
		}

		registeredContacts = loadContactsFromFile(contacts);
		while (loggedUser)
		{
			system("cls");
			cout << "1. Dodaj adresata" << endl;
			cout << "2. Wyszukaj po imieniu" << endl;
			cout << "3. Wyszukaj po nazwisku" << endl;
			cout << "4. Wyswietl wszystkich adresatow" << endl;
			cout << "5. Usun adresata" << endl;
			cout << "6. Edytuj adresata" << endl;
			cout << "7. Zmien haslo" << endl;
			cout << "8. Wyloguj sie" << endl;
			cout << "Twoj wybor: ";

			cin >> menuChoice;
			switch (menuChoice)
			{
			case '1': registeredContacts = addNewContact(contacts, registeredContacts); break;
			case '2': findContactByName(contacts, registeredContacts); break;
			case '3': findContactByLastName(contacts, registeredContacts); break;
			case '4': printAllContacts(contacts, registeredContacts); break;
			case '5': registeredContacts = deleteContact(contacts, registeredContacts); break;
			case '6': editContact(contacts); break;
			case '8': loggedUser = 0; break;
			default: break;
			}
		}
	}
	return 0;
}

int registerUser(vector <User> &users, int registeredUsers)
{
	User temporaryUser;
	system("cls");
	cout << "Podaj login: ";
	cin >> temporaryUser.username;
	//TODO sprawdz czy uzytkownik istnieje
	cout << "Podaj haslo: ";
	cin >> temporaryUser.password;

	if (registeredUsers == 0)
		temporaryUser.id = ++registeredUsers;
	else
	{
		temporaryUser.id = users.back().id + 1;
		registeredUsers++;
	}

	users.push_back(temporaryUser);
	saveUserToFile(temporaryUser);
	return registeredUsers;
}

int logIn(vector <User> &users, int registeredUsers)
{
	string userName, password;
	system("cls");
	cout << "Podaj login: ";
	cin >> userName;
	for (vector<User>::iterator it = users.begin(), end = users.end(); it != end; it++)
	{
		if (it->username == userName)
		{
			cout << "Podaj haslo: ";
			cin >> password;
			if (it->password == password)
			{
				cout << "Zalogowano poprawnie" << endl;
				system("pause");
				return it->id;
			}
			cout << "Podano niepoprawne haslo" << endl;
			system("pause");
			return 0;
		}
	}
	cout << "Nie znaleziono uzytkownika o podanym loginie" << endl;
	system("pause");
	return 0;
}

int loadUsersFromFile(vector <User> &users)
{
	int registeredUsers = 0;
	fstream file;
	file.open("uzytkownicy.txt", ios::in);
	if (!file.good())
		return 0;
	else
	{
		string loadedLine;
		User temporaryUser;
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
					case 1: temporaryUser.id = atoi(temporaryText.c_str()); break;
					case 2: temporaryUser.username = temporaryText; break;
					case 3: temporaryUser.password = temporaryText; break;

					}
					temporaryText = "";
					loadedLineCounter++;
				}
				else
					temporaryText += loadedLine[i];
			}
			users.push_back(temporaryUser);
			registeredUsers++;
		}
	}
	file.close();
	file.clear();
	return registeredUsers;
}

void saveUserToFile(User user)
{
	fstream file;
	file.open("uzytkownicy.txt", ios::out | ios::app);
	if (file.good())
	{
		file << user.id << "|";
		file << user.username << "|";
		file << user.password << "|" << endl;
		file.close();
		file.clear();
	}
	else
	{
		cout << "Blad zapisu pliku" << endl;
		system("pause");
	}
}

int loadContactsFromFile(vector<Person> &contacts)
{
	int registeredContacts = 0;
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
			registeredContacts++;
		}
	}
	file.close();
	file.clear();
	return registeredContacts;
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

int addNewContact(vector<Person> &contacts, int registeredContacts)
{
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

	if (registeredContacts == 0)
		temporaryPerson.id = ++registeredContacts;
	else
	{
		temporaryPerson.id = contacts.back().id + 1;
		registeredContacts++;
	}

	contacts.push_back(temporaryPerson);
	saveContactsToFile(temporaryPerson);
	return registeredContacts;
}



int deleteContact(vector<Person> &contacts, int registeredContacts)
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
				registeredContacts--;
			}
			system("pause");
			return registeredContacts;
		}
	}
	cout << "Nie znaleziono danego adresata. Nacisnij dowolny przycisk by powrocic do menu" << endl;
	system("pause");
	return registeredContacts;
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
			while (1)
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

void printAllContacts(vector<Person> &contacts, int registeredContacts)
{
	if (registeredContacts == 0)
	{
		system("cls");
		cout << "Ksiazka adresowa jest pusta" << endl;
	}
	else
	{
		printHeaderRow();
		for (int i = 0; i < registeredContacts; i++)
		{
			printContact(contacts[i]);
		}
	}
	system("pause");
}

void findContactByName(vector<Person> &contacts, int registeredContacts)
{
	string firstName;
	system("cls");
	cout << "Podaj imie do wyszukania: ";
	cin >> firstName;

	printHeaderRow();
	for (int i = 0; i < registeredContacts; i++)
	{
		if (contacts[i].firstName == firstName)
			printContact(contacts[i]);
	}
	system("pause");
}

void findContactByLastName(vector<Person> &contacts, int registeredContacts)
{
	{
		string lastName;
		system("cls");
		cout << "Podaj nazwisko do wyszukania: ";
		cin >> lastName;

		printHeaderRow();
		for (int i = 0; i < registeredContacts; i++)
		{
			if (contacts[i].lastName == lastName)
				printContact(contacts[i]);
		}
		system("pause");
	}
}