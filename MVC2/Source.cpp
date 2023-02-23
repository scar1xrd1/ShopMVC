#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <Windows.h>
#include <string>
//#include "ShellAPI.h"
#include <chrono>
using namespace std;

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

void set_color(int text) // Изменение цвета текста консоли
{
	SetConsoleTextAttribute(h, (0 << 4) + text);
}

class UserData
{
	static int balance;

public:
	UserData()
	{
		
	}

	UserData(int b)
	{
		if (b <= 0)
		{
			srand(time(NULL));

			int r = 1 + rand() % 3;

			if (r == 1) {
				balance = 100 + rand() % 1000;
			}
			else if (r == 2) {
				balance = 200 + rand() % 2000;
			}
			else if (r == 3) {
				balance = 500 + rand() % 5000;
			}
		}
		else balance = b;		
	}

	void set_balance(int value) { if (value >= 0) balance = value; }
	int get_balance() { return balance; }
};

int UserData::balance;

class ProductModel
{
	string *product;
	int* buyed_product;
	int* price_product;
	int len, len_product;
	string last_check;

public:
	ProductModel()
	{
		len = 0;
		read_product("data/products.txt");
	}

	~ProductModel()
	{
		delete[] product;
		delete[] buyed_product;
		delete[] price_product;
		cout << "ProductModel destructor\n";
	}

	void create_check()
	{
		if (can_create_check())
		{
			string str, data;
			auto now = chrono::system_clock::now();
			time_t end_time = chrono::system_clock::to_time_t(now);

			fstream num("data/checks.txt");

			data = "Дата покупки: ";
			data += ctime(&end_time);
			data += "\n";

			getline(num, str);
			int n = stoi(str);
			str = to_string(++n);

			ofstream file("checks/check" + str + ".txt");

			last_check = "notepad checks/check" + str + ".txt";

			file << "Aboba";

			/*
			ДОДЕЛАТЬ СОЗДАНИЕ ЧЕКА!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			*/


			fstream num1("data/checks.txt");
			num1 << str;

			num1.close();
			num.close();
			file.close();

			cout << "Ваш чек сохранен в папке проекта. Папка называется: \"checks\"\nХотите ли вы открыть созданный файл?\n1. Да\n2. Нет\n-> ";
		}
		else cout << "Чтобы получить чек купите хотя-бы один товар!\n\n";
	}

	void read_product(string path)
	{
		string line;

		fstream f(path);
		fstream f1(path);
		fstream f2("data/prices.txt");

		while (getline(f, line))
		{
			len_product++;
		}

		product = new string[len_product];
		buyed_product = new int[len_product];		
		price_product = new int[len_product];		

		int it = 0;
		while (getline(f1, line))
		{
			product[it++] = line;
			buyed_product[it-1] = 0;
			
		}

		it = 0;
		while (getline(f2, line))
		{
			price_product[it++] = stoi(line);
		}

		f.close();
		f1.close();
		f2.close();
	}

	void buy_product(int index)
	{
		if (buyed_product[index] == 0) buyed_product[index] = 1;
	}

	bool can_create_check()
	{
		bool result = false;
		for (int i = 0; i < len_product; i++) { if (buyed_product[i] == 1) { result = true; break; } }
		return result;
	}

	string *get_product() { return product; }
	int get_len() { return len_product; }
	int get_buyed(int index) { return buyed_product[index]; }
	int get_price(int index) { return price_product[index]; }
	string get_last_check() { return last_check; }
};

class ConsoleView
{
	ProductModel* model;
	UserData* user_data;

public:
	ConsoleView(ProductModel* _model)
	{
		model = _model;
		user_data = new UserData;
	}

	void view()
	{
		cout << "Ваш баланс: " << user_data->get_balance() << "\n\n";

		for (int i = 0; i < model->get_len(); i++)
		{
			cout << i + 1 << ". " << model->get_product()[i] << " - " << model->get_price(i) << " грн";

			if (model->get_buyed(i) == 0) { set_color(2); cout << "\t(есть в наличии)\n"; set_color(7); }
			else { set_color(4); cout << "\t(нет в наличии)\n"; set_color(7); }

			if (i+1 == model->get_len()) cout << i + 2 << ". Получить чек и завершить покупки\n";
		} cout << "\nВыберите товар который хотите купить -> ";
	}
};

class Controller
{
	ProductModel* model;

public:
	Controller(ProductModel* _model)
	{
		model = _model;
	}

	void start()
	{
		int user;
		string u;
		ConsoleView view(model);

		while (true)
		{
			view.view();
			while (!(cin >> user) || (cin.peek() != '\n')) { cin.clear(); while (cin.get() != '\n'); cout << "\nТолько цифрами!\n-> "; }
			system("cls");			

			if (user >= 1 && user <= model->get_len())
			{
				model->buy_product(user-1);
			}
			else if (user == model->get_len() + 1)
			{
				model->create_check();

				if(model->can_create_check())
				{
					cin >> u;
					system("cls");

					if (u == "1")
					{
						string str = model->get_last_check();
						const char* ch = str.c_str();
						system(ch);
						return;
					}
					else return;
				}
			}
		}
	}
};

int main()
{
	setlocale(LC_ALL, "ru");
	
	//system("notepad products.txt");

	string user; 
	int u;

	cout << "1. Рандомный баланс\n2. Свой баланс\n-> ";
	cin >> user;
	
	if (user == "2")
	{
		cout << "Введите баланс -> ";
		while (!(cin >> u) || (cin.peek() != '\n')) { cin.clear(); while (cin.get() != '\n'); cout << "\nТолько цифрами!\n-> "; }
		system("cls");

		UserData ud(u);

		if (u <= 0) cout << "Поскольку вы ввели меньше одного, баланс выставится рандомно.\n\n";
	}
	else { UserData ud(0); system("cls"); }

	ProductModel model;
	ConsoleView view(&model);
	Controller controller(&model);
	controller.start();
}