#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include "md5Hash.cpp"
using namespace std;


template <typename T>
T get_input(const std::string& strQuery);

//Datale user-ului simulate ca ar fi extrase din database;
string usernameFromDatabase = "";
ConstexprHashes::md5_type passwordFromDatabase; //custom type 

//Definirea Metodelor si proprietatilor
string get_username();
string get_password(bool registering);
void save_user(string username, ConstexprHashes::md5_type password);
ConstexprHashes::md5_type encrypt(std::string msg);
bool IsLoggedIn = false;

void login();
void register_user();
void main_menu();

int main()
{
	main_menu();
	return 1;
}

template <typename T>
T get_input(const string& strQuery)
{
	std::cout << strQuery << "\n> ";
	T out = T();

	while (!(std::cin >> out)) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Error!" "\n";
		std::cout << strQuery << "\n> ";
	}

	return out;
}

string get_password(bool registering)
{
	if (registering) {
		string password1 = get_input <string>("Va rugam sa introduceti Parola.");
		string password2 = get_input <string>("Va rugam sa introduceti Parola din nou.");
		while (password1 != password2) {
			std::cout << "Eroare! Parolele introduse nu corespund!" "\n";
			password1 = get_input <string>("Va rugam sa introduceti Parola.");
			password2 = get_input <string>("Va rugam sa introduceti Parola din nou.");
		}
		return password1;
	}
	else {
		std::string password = get_input <std::string>("Va rugam sa introduceti Parola.");
		return password;
	}
}

string get_username()
{
	string username = get_input <string>("Va rugam sa introduceti un Username");
	cout << "Username: \"" << username << "\"\n";
	return username;
}

void login()
{
	if (usernameFromDatabase != "") {
		string username = get_username();
		while (username != usernameFromDatabase) {
			cout << "Username incorrect!" "\n";
			username = get_username();
		}
		string password = get_password(false);
		ConstexprHashes::md5_type dehashedPassword = encrypt(password);
		while (passwordFromDatabase != dehashedPassword) {
			cout << "Parola incorrecta" "\n";
			password = get_password(false);
			dehashedPassword = encrypt(password);
		}
		IsLoggedIn = true;
		main_menu();
	}
	else {
		std::cout << "Nici un user inregistrat in Baza de date!" "\n";
		int choice = get_input <int>(
			"Ati dori sa va inregistrati?" "\n"
			"[1] Da" "\n"
			"[2] Nu" "\n");
		switch (choice)
		{
		case 1:
			register_user();
			break;
		default:
			break;
		}
	}
}

void main_menu()
{
	//Verificare daca userul este deja loggat
	if (!IsLoggedIn) {
		int choice = get_input <int>(
			"Salut, ati dori sa va logati sau sa va inregistrati?" "\n"
			"[1] Logare" "\n"
			"[2] Inregistrare" "\n"
			"[3] Iesi");

		switch (choice)
		{
		case 1:
			login();
			break;
		case 2:
			register_user();
			break;
		}
	}
	else
	{
		cout << "Domnule " << usernameFromDatabase << ", Acum sunteti Logat cu success!\nPuteti continua!";
	}
}

void register_user()
{
	std::string username = get_username();
	std::string password = get_password(true);
	ConstexprHashes::md5_type HashedPassword = encrypt(password);
	save_user(username, HashedPassword);
	main_menu();
}

void save_user(string username, ConstexprHashes::md5_type password)
{
	usernameFromDatabase = username;
	passwordFromDatabase = password;
}

ConstexprHashes::md5_type encrypt(string msg)
{
	char v[sizeof(msg)];
	std::copy(msg.begin(), msg.end(), v);
	auto hash = ConstexprHashes::md5(v);
	return hash;
}
