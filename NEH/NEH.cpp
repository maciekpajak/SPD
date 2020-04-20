#include <iostream>
#include <string>
#include <math.h>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <chrono>

#define MAX_LICZBA_ZADAN 500
#define MAX_LICZBA_MASZYN 20

using namespace std;

int macierzZadan[MAX_LICZBA_ZADAN][MAX_LICZBA_MASZYN];
int kolejnosc[MAX_LICZBA_ZADAN];
int sciezki[MAX_LICZBA_ZADAN + 2][MAX_LICZBA_MASZYN + 2][3];
int tmp[MAX_LICZBA_MASZYN + 2][3];

int cmax(vector<int>& permutacja, int n, int m)
{

	int czasy[MAX_LICZBA_ZADAN + 1][MAX_LICZBA_MASZYN + 1];
	for (int i = 0; i < MAX_LICZBA_ZADAN + 1; i++)
		for (int j = 0; j < MAX_LICZBA_MASZYN + 1; j++)
			czasy[i][j] = 0;

	for (int i = 1; i < n + 1; i++)
	{
		for (int j = 1; j < m + 1; j++)
		{
			int czas = max(czasy[i - 1][j], czasy[i][j - 1]) + macierzZadan[permutacja[i - 1]][j - 1];
			czasy[i][j] = czas;
		}
	}

	return czasy[n][m];
}

int qNEH(vector<int>& permutacjaStart, int n, int m)
{
	
	for (int i = 0; i < MAX_LICZBA_ZADAN + 2; i++)
		for (int j = 0; j < MAX_LICZBA_MASZYN + 2; j++)
			for (int k = 0; k < 3; k++)
			{
				sciezki[i][j][k] = 0;
				tmp[j][k] = 0;
			}


	int c, cmax, miejsceDocelowe = 0;
	for (int i = 0; i < n; i++)
	{
		int nrZad = permutacjaStart[i];
		for (int k = 0; k < m; k++)
			tmp[k+1][0] = macierzZadan[nrZad][k];

		
		cmax = 10000000;
		for (int j = 1; j < i + 2; j++)
		{ 
			c = 0;
			for (int k = 1; k < m+1; k++)
			{
				if (j < i + 1)
				{
					tmp[k][1] = max(tmp[k-1][1], sciezki[j - 1][k][1]) + macierzZadan[nrZad][k-1];
					c = max(c, tmp[k][1]  + sciezki[j][k][2]);
				}
				else if (j == i + 1)
				{
					tmp[m + 1 - k][2] = max(tmp[m + 1 - k + 1][2], sciezki[j][k][2]) + macierzZadan[nrZad][m - k];
					c = max(c, tmp[m + 1 - k][2] + sciezki[j - 1][m + 1 - k][1]);
				}
			}
			if (c < cmax)
			{
				cmax = c;
				miejsceDocelowe = j;
			}

		}
		// Przesuniecie 
		for (int j = MAX_LICZBA_ZADAN + 1; j > miejsceDocelowe; j--)
			for (int k = 0; k < m; k++)
				for (int m = 0; m < 3; m++)
				{
					sciezki[j][k + 1][m] = sciezki[j - 1][k + 1][m];
					kolejnosc[j - 2] = kolejnosc[j - 3];
				}
		//Wstawienie nr zadania na odpowiednie miejsce
		kolejnosc[miejsceDocelowe - 1] = nrZad;

		//Aktualizacja sciezek
		for (int k = 0; k < m; k++)
			sciezki[miejsceDocelowe][k + 1][0] = macierzZadan[nrZad][k];

		for (int j = 1; j < i + 2; j++)
		{
			for (int k = 0; k < m; k++)
			{
				sciezki[j][k + 1][1] = max(sciezki[j][k][1], sciezki[j - 1][k + 1][1]) + sciezki[j][k + 1][0];
				sciezki[i + 2 - j][m - k][2] = max(sciezki[i + 2 - j][m - k + 1][2], sciezki[i + 2 - j + 1][m - k][2]) + sciezki[i + 2 - j][m - k][0];
			}
		}

		for (int j = 0; j < MAX_LICZBA_MASZYN + 2; j++)
			for (int k = 0; k < 3; k++)
			{
				tmp[j][k] = 0;
			}

	}

	return sciezki[1][1][2];
}

vector<int> neh(vector<int>& permutacjaStart, int n, int m)
{
	int wynik, miejsceDocelowe;

	vector<int> tmp, out;
	tmp.push_back(permutacjaStart[0]);

	for (int i = 1; i < n; i++)
	{
		miejsceDocelowe = 0;
		tmp.insert(tmp.begin(), permutacjaStart[i]);
		wynik = cmax(tmp, i + 1, m);
		tmp.erase(tmp.begin());
		for (int j = 1; j <= i; j++)
		{
			tmp.insert(tmp.begin() + j, permutacjaStart[i]);
			int c = cmax(tmp, i + 1, m);
			if (c < wynik)
			{
				wynik = c;
				miejsceDocelowe = j;
			}

			tmp.erase(tmp.begin() + j);
		}

		tmp.insert(tmp.begin() + miejsceDocelowe, permutacjaStart[i]);
	}

	return tmp;
}

int main()
{
	ifstream file("./neh.data.txt", ifstream::in);
	if (!file.is_open()) return -1;



	vector<int> permutacja(MAX_LICZBA_ZADAN);

	vector<pair<int, int>> wagi(MAX_LICZBA_ZADAN);

	char tmp;
	string line, nr;
	int n, m, wynik, roznica = 0;
	bool czyWyswKol, czyWyswPor, czyQuick;
	cout << "Czy z akceleracja? (Y/N) ";
	cin >> tmp;
	tmp == 'Y' ? czyQuick = true : czyQuick = false;
	cout << "Czy wyswietlac kolejnosc? (Y/N) ";
	cin >> tmp;
	tmp == 'Y' ? czyWyswKol = true : czyWyswKol = false;
	cout << "Czy wyswietlac porownanie? (Y/N) ";
	cin >> tmp;
	tmp == 'Y' ? czyWyswPor = true : czyWyswPor = false;

	chrono::duration<double> TotalTime;

	for (int dataIdx = 0 ; dataIdx <= 120; dataIdx++)
	{
		nr = to_string(dataIdx * 0.000001).substr(5);
		while (getline(file, line) && (line != "data." + nr  + ":"));
		file >> n >> m;
		for (int i = 0; i < n; i++)
		{
			int waga = 0;
			for (int j = 0; j < m; j++)
			{
				file >> macierzZadan[i][j];
				waga += macierzZadan[i][j];
			}
			wagi[i] = make_pair(waga, i);
		}

		//Wstepne sortowanie
		stable_sort(wagi.begin(), wagi.begin() + n, greater <>());


		for (int i = 0; i < n; i++)
		{
			permutacja[i] = wagi[i].second;
		}

		while (getline(file, line) && (line != "neh:"));
		int check;
		file >> check;

		
		auto start = std::chrono::steady_clock::now();
		
		if (czyQuick)
			wynik = qNEH(permutacja, n, m);
		else
		{
			vector<int> kol = neh(permutacja, n, m);
			wynik = cmax(kol, n, m);
		}
		auto end = std::chrono::steady_clock::now();
		chrono::duration<double> time = end - start;
		TotalTime += time;
		
		roznica += wynik - check;
		cout << "data." + nr + ":" << '\t' << wynik;
		if (czyWyswPor)
			cout << ' ' << check << ' ' << wynik - check;
		cout << '\t' << time.count() << endl;


		if (czyWyswKol)
		{
			for (int i = 0; i < n; i++)
			{
				cout << kolejnosc[i] + 1 << ' ';
			}
			cout << endl;
		}

	}
	if (czyWyswPor)
		cout << "Roznica:" << '\t' << roznica << endl;

	cout << "Total time:" << '\t' << TotalTime.count() << endl;
	system("pause");
	return 0;
}

