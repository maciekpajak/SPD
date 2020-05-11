#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <vector>
#include <chrono>                                                          
#include <conio.h>                                                          
#include <stdlib.h>  

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


class ListaZadanNaKopcu {
public:

	vector<Zadanie> Lista;


	void Swap(Zadanie& zA, Zadanie& zB)
	{
		Zadanie temp;
		temp = zA;
		zA = zB;
		zB = temp;

	}

	void ShiftDown_ByR(int node)
	{
		int next = 2 * node;
		while (next <= Lista.size())
		{
			if ((next < Lista.size()) && (Lista[next - 1].R > Lista[next].R))
			{
				next++;
			}
			if (Lista[node-1].R > Lista[next-1].R)
			{
				Swap(Lista[node-1], Lista[next-1]);
				node = next; next *= 2;
			}
			else
			{
				next = Lista.size() + 1;
			}
		}
	}

	void ShiftDown_ByQ(int node)
	{
		int next = 2 * node;
		while (next <= Lista.size())
		{
			if ((next < Lista.size()) && (Lista[next - 1].Q < Lista[next].Q))
			{
				next++;
			}
			if (Lista[node - 1].Q < Lista[next - 1].Q)
			{
				Swap(Lista[node - 1], Lista[next - 1]);
				node = next; next *= 2;
			}
			else
			{
				next = Lista.size() + 1;
			}
		}
	}


	void ShiftUp_ByR(int node)
	{
		int next = node / 2;
		while ((node > 1) && (Lista[next-1].R > Lista[node-1].R))
		{
			Swap(Lista[node-1], Lista[next-1]);
			node = next; next /= 2;
		}
	}


	void ShiftUp_ByQ(int node)
	{
		int next = node / 2;
		while ((node > 1) && (Lista[next - 1].Q < Lista[node - 1].Q))
		{
			Swap(Lista[node - 1], Lista[next - 1]);
			node = next; next /= 2;
		}
	}


	void Make_ByR()
	{
		for (int count = Lista.size() / 2 - 1; count > 0; count--)
		{
			ShiftDown_ByR(count);
		}
	}

	void Make_ByQ()
	{
		for (int count = Lista.size() / 2 - 1; count > 0; count--)
		{
			ShiftDown_ByQ(count);
		}
	}

	void Push_ByR(Zadanie z)
	{
		Lista.push_back(z);
		ShiftUp_ByR(Lista.size());
	}
	void Push_ByQ(Zadanie z)
	{
		Lista.push_back(z);
		ShiftUp_ByQ(Lista.size());
	}

	Zadanie Pop_ByR()
	{
		Swap(Lista[0], Lista.back());
		Zadanie z = Lista.back();
		Lista.pop_back();
		ShiftDown_ByR(1);
		return z;
	}
	Zadanie Pop_ByQ()
	{
		Swap(Lista[0], Lista.back());
		Zadanie z = Lista.back();
		Lista.pop_back();
		ShiftDown_ByQ(1);
		return z;
	}


	void HeapSort_ByR()
	{
		vector<Zadanie> tmp;
		Make_ByR();
		while (Lista.size() > 0)
		{
			tmp.push_back(Pop_ByR());
		}
		Lista = tmp;
	}

	void HeapSort_ByQ()
	{
		vector<Zadanie> tmp;
		Make_ByQ();
		while (Lista.size() > 0)
		{
			tmp.push_back(Pop_ByQ());
		}
		Lista = tmp;
	}


	void HeapShow()
	{
		for (int i = 0; i < Lista.size(); i++)
		{
			cout << '(' << Lista[i].Nr << ' ' << Lista[i].R << ' ' << Lista[i].P << ' ' << Lista[i].Q << ')' << endl;
		}
		cout << endl;
	}


	bool isEmpty() { return Lista.size() == 0 ? true : false; }

	ListaZadanNaKopcu()
	{
	}

	Zadanie& operator [](int idx)
	{
		return Lista[idx];
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
	int t = 0, cmax = 0, k, idx = 0;
	for (int i = 0; i < n; i++)
	{
		t = max(t, K[i].R) + K[i].P;
		cmax = max(t + K[i].Q, cmax);
	}
	return cmax;
}

vector<Zadanie> Schrage(ListaZadanNaKopcu N, int n)
{
	int t = N[0].R, p;
	//G - zbior zadan do uszeregowania
	//N - zbior zadan nieuszeregowanych
	vector<Zadanie> K;
	int i = 0;
	ListaZadanNaKopcu G;
	while (!G.isEmpty() or !N.isEmpty())
	{
		while (!N.isEmpty() and (N[0].R <= t))
		{
			G.Push_ByQ(N.Pop_ByR());
		}
		if (G.isEmpty())
		{
			t = N[0].R;
		}
		else
		{
			t = t + G[0].P;
			K.push_back(G.Pop_ByQ());
		}
	}
	return K;
}

int SchragePRMT(ListaZadanNaKopcu N, int n)
{
	int t = 0, cmax = 0;
	//G - zbior zadan do uszeregowania
	//N - zbior zadan nieuszeregowanych
	ListaZadanNaKopcu G;
	ListaZadanNaKopcu N1 = N;
	Zadanie l(0,0,0,10000000);
	while (!G.isEmpty() or !N.isEmpty())
	{
		while (!N.isEmpty() and (N[0].R <= t))
		{
			Zadanie j = N[0];
			G.Push_ByQ(N.Pop_ByR());

			if (j.Q > l.Q)
			{
				l.P = t - j.R;
				t = j.R;
				if (l.P > 0)
				{
					G.Push_ByQ(l);
				}
			}

		}
		if (G.isEmpty())
		{
			t = N[0].R;
		}
		else
		{
			l = G.Pop_ByQ();
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
	int n, cmax, cmaxPRMT;
	bool czyWyswKol;

	cout << "Czy wyswietlac kolejnosc? (Y/N) ";
	cin >> tmp;
	tmp == 'Y' ? czyWyswKol = true : czyWyswKol = false;

	for (int dataIdx = 0; dataIdx <= 8; dataIdx++)
	{
		nr = to_string(dataIdx * 0.000001).substr(5);
		while (getline(file, line) && (line != "data." + nr + ":"));
		file >> n;

		ListaZadanNaKopcu lst;

		//Czytanie danych z pliku
		for (int i = 0; i < n; i++)
		{
			int nr, r, p, q;
			nr = i;
			file >> r >> p >> q;
			lst.Push_ByR(Zadanie(nr, r, p, q));
		}

		lst.HeapSort_ByR();
		//lst.HeapShow();

		auto start = chrono::steady_clock::now();
		vector<Zadanie>  K = Schrage(lst, n);
		cmaxPRMT = SchragePRMT(lst, n);
		auto end = chrono::steady_clock::now();
		chrono::duration<double> time = end - start;
		cmax = Cmax(K, n);


		cout << "data." + nr + ":" << '\t' << "SCHR: " << cmax << " PRMT: " << cmaxPRMT << '\t' << time.count() * 1000000 << " micro sec" << endl;
		if (czyWyswKol)
			wyswietl_kolejnosc(K, n);

	}
	

	file.close();

	system("pause");
	return 0;
}
