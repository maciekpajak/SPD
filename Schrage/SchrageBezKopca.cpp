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

	void Set(int nr,int r, int p, int q)
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

	Zadanie(int nr,int r, int p, int q)
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

class ListaZadan {
public:

	vector<Zadanie> Lista;

	bool usunZadanie(unsigned int ind)
	{
		if (ind > Lista.size() - 1 or ind < 0) return false;
		Lista.erase(Lista.begin() + ind);
		return true;
	}

	bool dodajZadanie(int nr, int r, int p, int q)
	{
		Lista.push_back(Zadanie(nr,r,p,q));
		return true;
	}

	bool dodajZadanie(Zadanie z)
	{
		Lista.push_back(z);
		return true;
	}

	bool isEmpty() { return Lista.size() == 0 ? true : false; }

	Zadanie& operator [](int idx)
	{
		return Lista[idx];
	}

	int MinR_idx()
	{
		int idx = 0,minR = 100000000;
		for (int i = 0; i < Lista.size(); i++)
		{
			if (Lista[i].R < minR)
			{
				minR = Lista[i].R;
				idx = i;
			}
		}
		return idx;
	}

	int MinR_val()
	{
		int idx = 0, minR = 100000000;
		for (int i = 0; i < Lista.size(); i++)
		{
			if (Lista[i].R < minR)
			{
				minR = Lista[i].R;
				idx = i;
			}
		}
		return minR;
	}


	int MaxQ_idx()
	{
		int idx = 0, maxQ = -1;
		for (int i = 0; i < Lista.size(); i++)
		{
			if (Lista[i].Q > maxQ)
			{
				maxQ = Lista[i].Q;
				idx = i;
			}
		}
		return idx;
	}

};


void wyswietl_kolejnosc(vector<int> tab, int n)
{
	cout << "kolejnosc: ";
	for (int i = 0; i < n; i++)
	{
		cout << tab[i] + 1 << ' ';
	}
	cout << endl;
}

int Cmax(ListaZadan lst, vector<int> K, int n)
{
	int t = 0, cmax = 0, k, idx = 0;
	for (int i = 0; i < n; i++)
	{
		idx = K[i];
		k = lst[idx].Nr;;
		t = max(t, lst[k].R) + lst[k].P;
		cmax = max(t + lst[k].Q, cmax);
	}
	return cmax;
}

vector<int> Schrage(ListaZadan N, int n)
{
	int t = N.MinR_val(), e;
	//G - zbior zadan do uszeregowania
	//N - zbior zadan nieuszeregowanych
	vector<int> K;
	int i = 0;
	ListaZadan G;
	while (!G.isEmpty() or !N.isEmpty())
	{
		while (!N.isEmpty() and (N.MinR_val() <= t))
		{
			e = N.MinR_idx();
			G.dodajZadanie(N[e]);
			N.usunZadanie(e);
		}
		if (G.isEmpty())
		{
			t = N.MinR_val();
		}
		else
		{
			e = G.MaxQ_idx();
			K.push_back(G[e].Nr);
			t = t + G[e].P;
			G.usunZadanie(e);
		}
	}
	return K;
}

int SchragePRMT(ListaZadan N, int n)
{
	int t = 0, j, l = 0, cmax = 0;
	//G - zbior zadan do uszeregowania
	//N - zbior zadan nieuszeregowanych
	ListaZadan G;
	ListaZadan N1 = N;
	while (!G.isEmpty() or !N.isEmpty())
	{
		while (!N.isEmpty() and (N.MinR_val() <= t))
		{
			j = N.MinR_idx();
			int tmpQ = N[j].Q;
			int tmpR = N[j].R;
			G.dodajZadanie(N[j]);
			N.usunZadanie(j);

			//tutaj
			if (tmpQ > N1[l].Q)
			{
				N1[l].P = t - tmpR;
				t = tmpR;
				if (N1[l].P > 0)
				{
					G.dodajZadanie(N1[l]);
				}
			}

		}
		if (G.isEmpty())
		{
			t = N.MinR_val();
		}
		else
		{
			j = G.MaxQ_idx();
			l = G[j].Nr;
			t = t + G[j].P;
			cmax = max(cmax, t + G[j].Q);
			G.usunZadanie(j);
		}

	}
	return cmax;
}


bool czy_w_wektorze(vector<int> v, int a)
{
	for (int i = 0; i < (int)v.size(); i++)
	{
		if (a == v[i]) return true;
	}
	return false;
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
	int n, cmax, cmaxPRMT,dataIdx = 0, subIdx = 0;
	bool czyWyswKol;
	double suma = 0;

	cout << "Czy wyswietlac kolejnosc? (Y/N) ";
	cin >> tmp;
	tmp == 'Y' ? czyWyswKol = true : czyWyswKol = false;
	
	//for (int dataIdx = 0; dataIdx <= 8; dataIdx++)
	while(!file.eof())
	{
		subIdx++;

		nr = to_string(dataIdx * 0.000001).substr(5);
		dataIdx++;
		while (getline(file, line) && (line != "data." + nr + ":"));
		if (file.eof()) break;

		file >> n;

		ListaZadan lst;

		//Czytanie danych z pliku
		for (int i = 0; i < n; i++)
		{
			int nr, r, p, q;
			nr = i;
			file >> r >> p >> q;
			lst.dodajZadanie(nr, r, p, q);
		}

		auto t_start = std::chrono::high_resolution_clock::now();
		vector<int>  K = Schrage(lst, n);
		cmaxPRMT = SchragePRMT(lst, n);
		auto t_end = std::chrono::high_resolution_clock::now();
		double elapsed_time_ms = std::chrono::duration<double, std::milli > (t_end - t_start).count();
		cmax = Cmax(lst, K, n);
		suma += elapsed_time_ms;

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
