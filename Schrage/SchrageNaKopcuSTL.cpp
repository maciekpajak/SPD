#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <vector>
#include <chrono>
#include <conio.h>
#include <stdlib.h>
#include <iomanip>

using namespace std;

class Zadanie
{
public:
	int Nr;
	int R;
	int P;
	int Q;

	void Set(int nr, int r, int p, int q)
	{
		Nr = nr;
		R = r;
		P = p;
		Q = q;
	}

	void Set(Zadanie& z)
	{
		Nr = z.Nr;
		R = z.R;
		P = z.P;
		Q = z.Q;
	}

	Zadanie(int nr, int r, int p, int q)
	{
		Nr = nr;
		R = r;
		P = p;
		Q = q;
	}

	Zadanie()
	{
		Nr = 0, R = 0; P = 0; Q = 0;
	}
};


struct CompR
{
	bool operator()(Zadanie& a, Zadanie& b)
	{
		return a.R > b.R;
	}
};

struct CompQ
{
	bool operator()(Zadanie& a, Zadanie& b)
	{
		return a.Q < b.Q;
	}
};


void wyswietl_kolejnosc(vector<Zadanie> tab, int n)
{
	cout << "kolejnosc: ";
	for (int i = 0; i < n; i++)
	{
		cout << tab[i].Nr + 1 << ' ';
	}
	cout << endl;
}


int Cmax(vector<Zadanie> K, int n)
{
	int t = 0, cmax = 0, idx = 0;
	for (int i = 0; i < n; i++)
	{
		t = max(t, K[i].R) + K[i].P;
		cmax = max(t + K[i].Q, cmax);
	}
	return cmax;
}

vector<Zadanie> Schrage(vector<Zadanie> N, int n)
{
	int t = N.front().R;
	//G - zbior zadan do uszeregowania
	//N - zbior zadan nieuszeregowanych
	vector<Zadanie> K;
	int i = 0;
	vector<Zadanie> G;
	while (!G.empty() or !N.empty())
	{
		while (!N.empty() and (N.front().R <= t))
		{
			G.push_back(N.front()); 
			push_heap(G.begin(), G.end(), CompQ());
			pop_heap(N.begin(), N.end(), CompR());
			N.pop_back();
		}
		if (G.empty())
		{
			t = N.front().R;
		}
		else
		{
			t = t + G.front().P;
			K.push_back(G.front());
			pop_heap(G.begin(), G.end(), CompQ());
			G.pop_back();
		}
	}
	return K;
}


int SchragePRMT(vector<Zadanie> N, int n)
{
	int t = 0, cmax = 0;
	//G - zbior zadan do uszeregowania
	//N - zbior zadan nieuszeregowanych
	vector<Zadanie> G;
	vector<Zadanie> N1 = N;
	Zadanie l(0, 0, 0, 10000000);
	while (!G.empty() or !N.empty())
	{
		while (!N.empty() and (N[0].R <= t))
		{
			Zadanie j = N.front();
			G.push_back(N.front());
			push_heap(G.begin(), G.end(), CompQ());
			pop_heap(N.begin(), N.end(), CompR());
			N.pop_back();

			if (j.Q > l.Q)
			{
				l.P = t - j.R;
				t = j.R;
				if (l.P > 0)
				{
					G.push_back(l);
					push_heap(G.begin(), G.end(), CompQ());
				}
			}

		}
		if (G.empty())
		{
			t = N[0].R;
		}
		else
		{
			l = G.front();
			pop_heap(G.begin(), G.end(), CompQ());
			G.pop_back();
			t = t + l.P;
			cmax = max(cmax, t + l.Q);

		}

	}
	return cmax;
}


int main()
{

	


	ifstream file("./schrage.data.txt", ios::in);
	if (!file.is_open())
	{
		cout << "Nie udalo sie otworzyc pliku";
		return -1;
	}

	char tmp;
	string line, nr;
	int n, cmax, cmaxPRMT, dataIdx = 0, subIdx = 0;
	bool czyWyswKol;
	double suma = 0;

	cout << "Czy wyswietlac kolejnosc? (Y/N) ";
	cin >> tmp;
	tmp == 'Y' ? czyWyswKol = true : czyWyswKol = false;

	while (!file.eof())
	{
		subIdx++;

		nr = to_string(dataIdx * 0.000001).substr(5);
		dataIdx++;
		while (getline(file, line) && (line != "data." + nr + ":"));
		if (file.eof()) break;
		file >> n;

		vector<Zadanie> lst;

		//Czytanie danych z pliku
		for (int i = 0; i < n; i++)
		{
			int nr, r, p, q;
			nr = i;
			file >> r >> p >> q;
			lst.push_back(Zadanie(nr, r, p, q)); 
			push_heap(lst.begin(), lst.end(), CompR());
		}

		make_heap(lst.begin(), lst.end(), CompR());


		auto t_start = std::chrono::high_resolution_clock::now();
		vector<Zadanie>  K = Schrage(lst, n);
		cmaxPRMT = SchragePRMT(lst, n);
		auto t_end = std::chrono::high_resolution_clock::now();
		double elapsed_time_ms = std::chrono::duration<double, std::milli >(t_end - t_start).count();
		suma += elapsed_time_ms;
		cmax = Cmax(K,n);

		cout << "data." + nr + ":" << "SCHR: " << setw(8) << cmax << " PRMT: " << setw(8) << cmaxPRMT << ' ';
		cout << '\t' << elapsed_time_ms << " ms" << endl;
		if (czyWyswKol)
			wyswietl_kolejnosc(K, n);
		if (subIdx == 10)
		{
			subIdx = 0;
			cout << "Total time:  " << suma / 10 << endl;
			suma = 0;
		}

	}
	
	file.close();

	system("pause");
	return 0;
}
