#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;

const string file1 = "iannelli1.csv";
const string file2 = "iannelli.csv";
int id = 42;

ifstream ifile2;

void Azione1()
{
	string str;
	ofstream sw;
	ifstream sr;
	int index = 0, val, min = 10, max = 20;

	sw.open(file1.c_str(), ofstream::out);
	sr.open(file2.c_str(), std::ifstream::in);

	getline(sr, str);

	while (!sr.eof())
	{
		if (index == 0)
		{
			sw << str << ";MIOVALORE;CANCELLAZIONE LOGICA;CAMPO UNIVOCO" << endl;
		}
		else
		{
			val = min + (rand() % (max - min + 1));
			sw << str << ";" << val << ";false;" << index << endl;
		}

		index++;
		getline(sr, str);
	}

	sw.close();
	sr.close();
}

int Azione2()
{
	ifstream sr(file1);
	string str;

	getline(sr, str);

	sr.close();

	int cont = 1;
	size_t pos = str.find(';');

	while (pos != string::npos)
	{
		cont++;
		pos = str.find(';', pos + 1);
	}

	return cont;
}

int Azione3()
{
	ifstream sr(file1);
	int lungStr = 0, lungMax = 0, index = 0;
	string str;

	while (getline(sr, str))
	{
		lungStr = str.length();

		if (index != 0)
		{
			if (lungMax < lungStr)
			{
				lungMax = lungStr;
			}
		}

		index++;
	}
	
	sr.close();

	return lungMax;
}

vector<int> Azione3Avanzato()
{
	int funz = Azione2();
	ifstream sr(file1);
	string str;
	
	getline(sr, str);

	vector<int> lungMax(funz, 0);

	while (!sr.eof())
	{
		getline(sr, str);
		vector<string> split;
		split.reserve(funz);

		for (int i = 0; i < funz; i++)
		{
			sr.clear();
			sr.seekg(0, ios::beg);

			int cont = 0;

			while (!sr.eof())
			{
				getline(sr, str);
				cont++;

				if (cont != 1)
				{
					size_t pos = str.find(';');
					
					if (pos != string::npos)
					{
						string substr = str.substr(0, pos);
						int lungCur = substr.length();

						if (i == cont - 2 && lungMax[i] < lungCur)
						{
							lungMax[i] = lungCur;
						}
					}
				}
			}
		}
	}

	sr.close();

	return lungMax;
}

void Azione4()
{
	string str;
	int lungRec = 200;

	ifstream sr(file1);
	ofstream sw("temp.csv");

	while (getline(sr, str))
	{
		if (str.length() < lungRec)
		{
			str += std::string(lungRec - str.length(), ' ');
		}

		sw << str << endl;
	}

	sr.close();
	sw.close();

	if (remove(file1.c_str()) != 0)
	{
		cout << "Errore durante l'eliminazione del file originale." << endl;
	}

	if (rename("temp.csv", file1.c_str()) != 0)
	{
		cout << "Errore durante la sostituzione dei file." << endl;
	}
}

bool Azione5(string zona, string attuazione, string data, string tipoSosta)
{
	id++;
	int num, min = 10, max = 20;
	bool success = false;

	try
	{
		num = stoi(data);
		success = true;
	}
	catch (const invalid_argument&)
	{
		success = false;
	}

	bool ret;

	if (zona.empty() || attuazione.empty() || data.empty() || !success || tipoSosta.empty())
	{
		ret = false;
	}
	else
	{
		srand(static_cast<unsigned int>(std::time(nullptr)));

		ifstream sr(file1);
		int index = 0;
		string str;
		
		while (getline(sr, str))
		{
			index++;
		}

		sr.close();

		ofstream oStream(file1, ios::app);
		
		int r = min + (std::rand() % (max - min + 1));
		ostringstream record;
		record << id << ";" << static_cast<char>(toupper(zona[0])) << zona.substr(1) << ";" << attuazione << ";" << data << ";" << tipoSosta << ";" << r << ";true;" << index << std::string(200 - 8, ' ');

		oStream << record.str() << endl;
		oStream.close();

		ret = true;
	}

	return ret;
}

vector<string> Azione6()
{
	ifstream sr(file1);
	string str;
	int index = 0;
	vector<string> print(id, "");

	while (getline(sr, str))
	{
		vector<string> split;
		size_t pos = 0;

		while ((pos = str.find(';')) != string::npos)
		{
			split.push_back(str.substr(0, pos));
			str.erase(0, pos + 1);
		}

		split.push_back(str);

		if (split.size() >= 7 && split[6] == "false") 
		{
			if (!print[index].empty()) 
			{
				print[index] += ";";
			}

			print[index] += split[1] + ";" + split[2] + ";" + split[4];
		}

		index++;
	}

	sr.close();

	return print;
}

int Azione7(string ricerca)
{
	int ret = -2;
	int index = 0;
	string s;

	bool success = false;

	try
	{
		stoi(ricerca);
		success = true;
	}
	catch (invalid_argument)
	{
		success = false;
	}

	if (success || ricerca.empty())
	{
		ifstream sr(file1);

		while (getline(sr, s))
		{
			string del = ";";
			size_t pos = 0;
			string token;

			while ((pos = s.find(del)) != string::npos)
			{
				token = s.substr(0, pos);
				
				if (index != 0)
				{
					if (token == ricerca)
					{
						ret = index;
						sr.close();
						return ret;
					}

					ret = -1;
				}

				s.erase(0, pos + del.length());
			}

			index++;
		}

		sr.close();
	}

	return ret;
}

int Azione8(string zona, string attuazione, string data, string tipoSosta, string ricerca)
{
	int count = Azione7(ricerca);
	int ret = 0, min = 10, max = 20;

	if (count == -2) 
	{
		ret = -2;
	}
	else if (count == -1) 
	{
		ret = -1;
	}
	else
	{
		ret = count;

		srand(static_cast<unsigned int>(time(nullptr)));
		ofstream oStream(file1, std::ios::in | std::ios::out | std::ios::binary);

		int r = min + (rand() % (max - min + 1));
		ostringstream record;
		record << id << ";" << static_cast<char>(std::toupper(zona[0])) << zona.substr(1) << ";" << attuazione << ";" << data << ";" << tipoSosta << ";" << r << ";true;" << count << std::string(200 - 8, ' ');

		oStream.seekp(0, ios::beg);
		oStream.seekp(200 * count, ios::cur);

		string recordStr = record.str();
		oStream.write(recordStr.c_str(), recordStr.size());

		oStream.close();
	}

	return ret;
}

void Azione9(string ricerca)
{
	int count = Azione7(ricerca);

	ifstream rStream(file1, ios::binary | ios::in);
	ofstream wStream(file1, ios::binary | ios::in | ios::out);

	const int recordSize = 200;
	rStream.seekg(recordSize * count, ios::beg);
	char buffer[recordSize];
	rStream.read(buffer, recordSize);

	string str(buffer, recordSize);

	wStream.seekp(recordSize * count, ios::beg);
	wStream.write(str.c_str(), recordSize);

	rStream.close();
	wStream.close();
}

int main()
{
	ifile2.open(file2);

	if (ifile2)
	{
		cout << "File presente." << endl;
		system("pause");
	}
	else
	{
		cout << "File non presente. Caricare un file in ../*nome soluzione*/*nome soluzione*" << endl;
		system("pause");
		exit(0);
	}

	ifile2.close();

	int azioneExe = 1;

	Azione1();
	Azione4();

	vector<int> vec;
	string lung = "";
	vector<string> vec2;

	string zona, attuazione, data, tipoSosta;
	bool azione5 = true;

	string ricerca;

	do
	{
		system("CLS");

		cout << "Funzionalita' disponibili:" << endl;
		cout << " 2 - Conta campi\n 3 - Lunghezza massima record\n31 - Lunghezza massima campi\n 5 - Aggiunta record in coda\n 6 - Visualli tre campi\n 7 - Ricerca per ID\n 8 - Modifica record\n 9 - Cancellazione logica\n 0 - Uscita" << endl;
		cout << "Inserire il numero corrispondente alla funzionalita' desiderata: ";

		try
		{
			cin >> azioneExe;
		}
		catch (...)
		{
			cout << "Errore rilevato." << endl;
			system("pause");
			exit(0);
		}

		switch (azioneExe)
		{
			case 2:
				cout << "Il numero di campi che compone il record e': " << Azione2() << endl;

				system("pause");
				break;

			case 3:
				cout << "La lunghezza massima dei record presenti e': " << Azione3() << endl;

				system("pause");
				break;

			case 31:
				vec = Azione3Avanzato();

				for (int i = 0; i < vec.size(); i++)
				{
					lung += vec[i] + "\n";
				}

				cout << "La lunghezza massima di ogni campo e': \n" << lung;

				system("pause");
				break;

			case 5:
				cout << "Nome zona: ";
				cin >> zona;

				cout << "Attuazione ";
				cin >> attuazione;

				cout << "Data (Anno): ";
				cin >> data;

				cout << "Tipo sosta: ";
				cin >> tipoSosta;

				azione5 = Azione5(zona, attuazione, data, tipoSosta);

				if (azione5)
				{
					cout << "Azione eseguita correttamente." << endl;
				}
				else
				{
					cout << "Compilare i campi correttamente." << endl;
				}

				system("pause");
				break;

			case 6:
				vec2 = Azione6();

				for (int i = 0; i < vec2.size(); i++)
				{
					cout << vec2[i] << endl;
				}

				system("pause");
				break;

			case 7:
				cout << "Inserire l'ID di ricerca: ";
				cin >> ricerca;

				if (Azione7(ricerca) == -2)
				{
					cout << "Compilare correttamente i campi." << endl;
				}
				else if (Azione7(ricerca) == -1)
				{
					cout << "Elemento corrispondente all'ID inserito inesistente." << endl;
				}
				else
				{
					cout << "Elemento corrispondente trovato alla riga: " << Azione7(ricerca) << "." << endl;
				}

				system("pause");
				break;

			case 8:
				cout << "Nome zona: ";
				cin >> zona;

				cout << "Attuazione ";
				cin >> attuazione;

				cout << "Data (Anno): ";
				cin >> data;

				cout << "Tipo sosta: ";
				cin >> tipoSosta;

				cout << "Ricerca: ";
				cin >> ricerca;

				if (Azione8(zona, attuazione, data, tipoSosta, ricerca) == -2)
				{
					cout << "Compilare correttamente i campi." << endl;
				}
				else if (Azione8(zona, attuazione, data, tipoSosta, ricerca) == -1)
				{
					cout << "Elemento corrispondente all'ID inserito inesistente." << endl;
				}
				else
				{
					cout << "Azione eseguita correttamente." << endl;
				}

				system("pause");
				break;

			case 9:
				cout << "Ricerca: ";
				cin >> ricerca;

				Azione9(ricerca);

				cout << "Azione eseguita correttamente." << endl;

				system("pause");
				break;
		}
		
	} while (azioneExe != 0);
}
