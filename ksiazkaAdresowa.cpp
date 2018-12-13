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

class User
{
public:
	static int registeredUsers;
	static vector <User> users;
	int id;
	string username, password;

	User(string name, string pw)
	{
		username = name;
		password = pw;
		id = ++registeredUsers;
	}
};

void loadUsersFromFile(vector <User> &users);

int logIn(vector <User> &users);

bool userExist(vector <User> &users, string username);
void saveUserToFile(User user);
void registerUser(vector <User> &users);

void updateUserInFile(User user);
void editUser(vector <User> &users, int &loggedUser);

int findUserId(string loadedLine);
void loadContactsFromFile(vector<Person> &contacts, int loggedUser);

void saveContactToFile(Person person, int loggedUser);
int lastContactId();
void addNewContact(vector<Person> &contacts, int loggedUser);

void deleteContactFromFile(Person person);
void deleteContact(vector<Person> &contacts);

void updateContactInFile(Person person, int loggedUser);
void editContact(vector <Person> &contacts, int loggedUser);

void printContact(Person person);
void printHeaderRow();
void printAllContacts(vector<Person> &contacts);

void findContactByName(vector<Person> &contacts);
void findContactByLastName(vector<Person> &contacts);

int User::registeredUsers;
vector<User> User::users;


int main()
{
	vector <Person> contacts;
	loadUsersFromFile(User::users);
	int loggedUser = 0;


	while (1)
	{
		while (loggedUser == 0)
		{
			system("cls");
			cout << "1. Logowanie" << endl;
			cout << "2. Rejestracja" << endl;
			cout << "9. Zakoncz program" << endl;
			cout << "Twoj wybor: ";
			char menuChoice;
			cin >> menuChoice;
			switch (menuChoice)
			{
			case '1': loggedUser = logIn(User::users); break;
			case '2': registerUser(User::users); break;

			case '9': exit(0); break;
			}
		}

		loadContactsFromFile(contacts, loggedUser);

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
			char menuChoice;
			cin >> menuChoice;
			switch (menuChoice)
			{
			case '1': addNewContact(contacts, loggedUser); break;
			case '2': findContactByName(contacts); break;
			case '3': findContactByLastName(contacts); break;
			case '4': printAllContacts(contacts); break;
			case '5': deleteContact(contacts); break;
			case '6': editContact(contacts, loggedUser); break;
			case '7': editUser(User::users, loggedUser); break;
			case '8': loggedUser = 0; break;
			default: break;
			}
		}
	}
	return 0;
}

void loadUsersFromFile(vector <User> &users)
{
	fstream file;
	file.open("uzytkownicy.txt", ios::in);
	if (!file.good())
		return;
	else
	{
		string loadedLine;
		string username;
		string password;
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
					case 1: break;
					case 2: username = temporaryText; break;
					case 3: password = temporaryText; break;

					}
					temporaryText = "";
					loadedLineCounter++;
				}
				else
					temporaryText += loadedLine[i];
			}
			User temporaryUser(username, password);
			users.push_back(temporaryUser);
		}
	}
	file.close();
	file.clear();
	return;
}

int logIn(vector <User> &users)
{
	string userName, password;
	system("cls");
	cout << "Podaj login: ";
	cin >> userName;
	cout << "Podaj haslo: ";
	cin >> password;

	for (vector<User>::iterator it = users.begin(), end = users.end(); it != end; it++)
	{
		if (it->username == userName)
		{
			if (it->password == password)
			{
				cout << "Zalogowano pomyslnie" << endl;
				system("pause");
				return it->id;
			}

			else
			{
				cout << "Podano niepoprawne dane logowania" << endl;
				system("pause");
				return 0;
			}
		}
	}

	cout << "Podano niepoprawne dane logowania" << endl;
	system("pause");
	return 0;
}

bool userExist(vector <User> &users, string username)
{
	for (vector<User>::iterator it = users.begin(), end = users.end(); it != end; it++)
	{
		if (it->username == username)
			return true;
	}
	return false;
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

void registerUser(vector <User> &users)
{
	system("cls");
	string username;
	cout << "Podaj login: ";
	cin >> username;
	while (userExist(users, username))
	{
		system("cls");
		cout << "Uzytkownik o podanym loginie istnieje. Podaj inny login: ";
		cin >> username;
	}
	string password;
	cout << "Podaj haslo: ";
	cin >> password;

	User temporaryUser(username, password);
	users.push_back(temporaryUser);
	saveUserToFile(temporaryUser);
	return;
}

void updateUserInFile(User user)
{
	fstream file, temp;
	file.open("uzytkownicy.txt", ios::in);
	temp.open("temp.txt", ios::out);
	if (file.good() && temp.good())
	{
		string loadedLine;
		while (getline(file, loadedLine))
		{
			if (loadedLine[0] - '0' == user.id)
			{
				temp << user.id << "|";
				temp << user.username << "|";
				temp << user.password << "|" << endl;
				continue;
			}
			temp << loadedLine << endl;
		}
	}
	file.close();
	file.clear();
	temp.close();
	temp.clear();
	remove("uzytkownicy.txt");
	rename("temp.txt", "uzytkownicy.txt");
}
void editUser(vector <User> &users, int &loggedUser)
{
	system("cls");
	cout << "Podaj stare haslo: ";
	string oldPassword;
	cin >> oldPassword;

	string newPassword;
	for (vector<User>::iterator it = users.begin(), end = users.end(); it != end; ++it)
	{
		if (it->id == loggedUser)
		{
			if (it->password == oldPassword)
			{
				cout << "Podaj nowe haslo: ";
				cin >> newPassword;
				it->password = newPassword;
				updateUserInFile(*it);
				cout << "Haslo zostalo zmienione" << endl;
				system("pause");
				return;
			}
			else
			{
				cout << "Podano niepoprawne haslo. Nastapi automatyczne wylogowanie" << endl;
				Sleep(2000);
				loggedUser = 0;
				return;
			}

		}
	}
}

int findUserId(string loadedLine)
{
	size_t start, end;
	start = loadedLine.find('|') + 1;
	end = loadedLine.find('|', start);
	int digits = end - start;
	return atoi(loadedLine.substr(start, digits).c_str());
}

void loadContactsFromFile(vector<Person> &contacts, int loggedUser)
{
	fstream file;
	file.open("dane_adresowe.txt", ios::in);
	if (!file.good())
		return;
	else
	{
		string loadedLine;
		Person temporaryPerson;
		while (getline(file, loadedLine))
		{
			if (findUserId(loadedLine) != loggedUser)
				continue;
			string temporaryText = "";
			int loadedLineCounter = 1;
			for (int i = 0; i < loadedLine.length(); i++)
			{
				if (loadedLine[i] == '|' || loadedLine[i] == '\0')
				{
					switch (loadedLineCounter)
					{
					case 1: temporaryPerson.id = atoi(temporaryText.c_str()); break;
					case 2: break;
					case 3: temporaryPerson.firstName = temporaryText; break;
					case 4: temporaryPerson.lastName = temporaryText; break;
					case 5: temporaryPerson.nrTel = temporaryText; break;
					case 6: temporaryPerson.email = temporaryText; break;
					case 7: temporaryPerson.adres = temporaryText; break;
					}
					temporaryText = "";
					loadedLineCounter++;
				}
				else
					temporaryText += loadedLine[i];
			}
			contacts.push_back(temporaryPerson);
		}
	}
	file.close();
	file.clear();
	return;
}

void saveContactToFile(Person person, int loggedUserID)
{
	fstream file;
	file.open("dane_adresowe.txt", ios::out | ios::app);
	if (file.good())
	{
		file << person.id << "|";
		file << loggedUserID << "|";
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

int lastContactId()
{
	fstream file;
	file.open("dane_adresowe.txt", ios::in);
	if (!file.good())
		return 0;
	else
	{
		file.seekg(-3, ios_base::end);
		if ((int)file.tellg() <= 1)
			return 0;
		else
		{
			while (1)
			{
				char ch;
				file.get(ch);
				if (ch == '\n')
					break;
				file.seekg(-2, ios_base::cur);
			}
			string lastLine;
			getline(file, lastLine);
			return lastLine[0] - '0';
		}
	}
}

void addNewContact(vector<Person> &contacts, int loggedUser)
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

	temporaryPerson.id = lastContactId() + 1;

	contacts.push_back(temporaryPerson);
	saveContactToFile(temporaryPerson, loggedUser);
	return;
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

void deleteContact(vector<Person> &contacts)
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
			}
			system("pause");
			return;
		}
	}
	cout << "Nie znaleziono danego adresata. Nacisnij dowolny przycisk by powrocic do menu" << endl;
	system("pause");
	return;
}

void updateContactInFile(Person person, int loggedUserID)
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
				temp << loggedUserID << "|";
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

void editContact(vector<Person> &contacts, int loggedUserId)
{
	system("cls");
	cout << "Podaj nr ID kontaktu do edycji: ";
	int contactToEdit;
	cin >> contactToEdit;
	for (vector<Person>::iterator it = contacts.begin(), end = contacts.end(); it != end; ++it)
	{
		if (it->id == contactToEdit)
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
				updateContactInFile(*it, loggedUserId);
			}
		}
	}
	cout << "Nie znaleziono danego adresata. Nacisnij dowolny przycisk by powrocic do menu" << endl;
	system("pause");
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

void printAllContacts(vector<Person> &contacts)
{
	if (contacts.size() == 0)
	{
		system("cls");
		cout << "Ksiazka adresowa jest pusta" << endl;
	}
	else
	{
		printHeaderRow();
		for (vector<Person>::iterator it = contacts.begin(), end = contacts.end(); it != end; ++it)
		{
			printContact(*it);
		}
	}
	system("pause");
}

void findContactByName(vector<Person> &contacts)
{
	string firstName;
	system("cls");
	cout << "Podaj imie do wyszukania: ";
	cin >> firstName;

	printHeaderRow();
	for (vector<Person>::iterator it = contacts.begin(), end = contacts.end(); it != end; ++it)
	{
		if (it->firstName == firstName)
			printContact(*it);
	}
	system("pause");
}

void findContactByLastName(vector<Person> &contacts)
{
	{
		string lastName;
		system("cls");
		cout << "Podaj nazwisko do wyszukania: ";
		cin >> lastName;

		printHeaderRow();
		for (vector<Person>::iterator it = contacts.begin(), end = contacts.end(); it != end; ++it)
		{
			if (it->lastName == lastName)
				printContact(*it);
		}
		system("pause");
	}
}