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


void wyswietl_kolejnosc(vector<Zadanie> tab)
{
	//cout << "kolejnosc: ";
	for (int i = 0; i < (int) tab.size(); i++)
	{
		cout << setw(2) <<  tab[i].Nr + 1 << ' ';
	}
	cout << endl;
}


int Cmax(vector<Zadanie> K)
{
	int t = 0, cmax = 0, idx = 0;
	for (int i = 0; i < (int)K.size(); i++)
	{
		t = max(t, K[i].R) + K[i].P;
		cmax = max(t + K[i].Q, cmax);
	}
	return cmax;
}

vector<Zadanie> Schrage(vector<Zadanie> N)
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


int SchragePRMT(vector<Zadanie> N)
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



int C(vector<Zadanie> K)
{
	int c = 0;
	for (int i = 0; i < (int)K.size(); i++)
	{
		c = max(c, K[i].R) + K[i].P;
	}
	return c;
}

int h(vector<Zadanie> K)
{
	int r1 = 100000000;
	for (int i = 0; i < (int)K.size(); i++)
	{
		r1 = min(r1, K[i].R);
	}
	int q1 = 100000000;
	for (int i = 0; i < (int)K.size(); i++)
	{
		q1 = min(q1, K[i].Q);
	}
	int p1 = 0;
	for (int i = 0; i < (int)K.size(); i++)
	{
		p1 += K[i].P;
	}

	return r1 + p1 + q1;
}


int r(vector<Zadanie> K)
{
	int r1 = 100000000;
	for (int i = 0; i < (int)K.size(); i++)
	{
		r1 = min(r1, K[i].R);
	}
	return r1;
}
int q(vector<Zadanie> K)
{
	int q1 = 100000000;
	for (int i = 0; i < (int)K.size(); i++)
	{
		q1 = min(q1, K[i].Q);
	}
	return q1;
}
int p(vector<Zadanie> K)
{
	int p1 = 0;
	for (int i = 0; i < (int)K.size(); i++)
	{
		p1 += K[i].P;
	}

	return p1;
}

vector<Zadanie> pi_ost = vector<Zadanie>();
int UB;

void Carlier(vector<Zadanie> N)
{

	make_heap(N.begin(), N.end(), CompR()); // Utworzenie kopca z N do algorytmu Schrage

	int U = Cmax(Schrage(N));
	vector<Zadanie> pi = Schrage(N);
	if (U < UB)
	{
		UB = U;
		pi_ost = pi;
	}

	
	// Wyznaczenie b ---------------------------------------//
	int b = -1;												//
	vector<Zadanie> tmp = pi;								//
	int cmax_pi = Cmax(pi);									//
	for (int i = (int)N.size() - 1; i >=0 ; i--)			//
	{														//
		int cmax_tmp = C(tmp) + pi[i].Q;					//
		if (cmax_pi == cmax_tmp)							//
		{													//
			b = i;											//
			break;											//
		}													//
		tmp.pop_back();										//
	}														//
	//------------------------------------------------------//

	// Wyznaczenie a ---------------------------------------//
	int a = -1;												//
	for (int i = 0; i < b; i++)								//
	{														//
		int p_sum = 0;										//
		for (int j = i; j <= b; j++)						//
			p_sum += pi[j].P;								//
		if (Cmax(pi) == pi[i].R + p_sum + pi[b].Q)			//
		{													//
			a = i;											//
			break;											//
		}													//
	}														//
	if (a == -1) // Jeœli a nie ma to od razu powrót		//
		return;												//
	//------------------------------------------------------//

	// Wyznaczenie c ---------------------------------------//
	int c = -1;												//
	for (int i = b; i >= a; i--)							//
	{														//
		if (pi[i].Q < pi[b].Q)								//
		{													//
			c = i;											//
			break;											//
		}													//
	}														//
	if (c == -1) // Jeœli c nie ma to powrót				//
		return;												//
	// -----------------------------------------------------//


	// Bloki krytyczne do wyznaczenia r(K), p(K), q(K) 
	// oraz h(K), h(K1) do dodatkowych eliminacji
	vector<Zadanie> K;
	for (int i = c + 1; i <= b; i++)
		K.push_back(pi[i]);
	vector<Zadanie> K1;
	for (int i = c; i <= b; i++)
		K1.push_back(pi[i]);

	// Podstawienie zmodyfikowanego r do zadania krytycznego
	for (int i = 0; i < (int)N.size(); i++)
	{
		if (N[i].Nr == pi[c].Nr)
		{
			N[i].R = max(pi[c].R, r(K) + p(K));
			break;
		}
	}


	int LB = max( max( h(K) , h(K1) ), SchragePRMT(N));
	if (LB < UB)
		Carlier(N);

	// Odtworzenie zmodyfikowanego r z zadania krytycznego
	for (int i = 0; i < (int)N.size(); i++)
	{
		if (N[i].Nr == pi[c].Nr)
		{
			N[i].R = pi[c].R;
			break;
		}
		
	}

	// Podstawienie zmodyfikowanego q do zadania krytycznego
	for (int i = 0; i < (int)N.size(); i++)
	{
		if (N[i].Nr == pi[c].Nr)
		{
			N[i].Q = max(pi[c].Q, q(K) + p(K));
			break;
		}
	}


	LB = max(max(h(K), h(K1)), SchragePRMT(N));
	if (LB < UB)
		Carlier(N);

	// Odtworzenie zmodyfikowanego q z zadania krytycznego
	for (int i = 0; i < (int)N.size(); i++)
	{
		if (N[i].Nr == pi[c].Nr)
		{
			N[i].Q = pi[c].Q;
			break;
		}
	}


	return;
}



int main()
{

	ifstream file("./carlier.data.txt", ios::in);
	if (!file.is_open())
	{
		cout << "Nie udalo sie otworzyc pliku";
		return -1;
	}

	char tmp;
	string line, nr;
	int n, cmax, dataIdx = 0;
	bool czyWyswKol = true;

	cout << "Czy wyswietlac kolejnosc? (Y/N) ";
	cin >> tmp;
	tmp == 'Y' ? czyWyswKol = true : czyWyswKol = false;

	while (!file.eof())
	{
		nr = to_string(dataIdx * 0.000001).substr(5);
		dataIdx++;
		while (getline(file, line) && (line != "data." + nr + ":"));
		if (file.eof()) break;
		file >> n;

		vector<Zadanie> lst;

		for (int i = 0; i < n; i++)
		{
			int nr, r, p, q;
			nr = i;
			file >> r >> p >> q;
			lst.push_back(Zadanie(nr, r, p, q));
			push_heap(lst.begin(), lst.end(), CompR());
		}

		make_heap(lst.begin(), lst.end(), CompR());

		UB = 100000000;
		//lst.HeapShow();
		//vector<Zadanie> pi = Schrage(lst);
		auto t_start = std::chrono::high_resolution_clock::now();
		try {
			Carlier(lst);	
		}
		catch (exception ex)
		{
			cout << ex.what() << endl;
			system("PAUSE");
		}
		auto t_end = std::chrono::high_resolution_clock::now();
		double elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();
		cmax = Cmax(pi_ost);

		double elapsed_time_s = elapsed_time_ms / 1000;
		double elapsed_time_min = elapsed_time_s / 60;
		double elapsed_time_h = elapsed_time_min / 60;
		double hours = floor(elapsed_time_h);
		double minutes = floor(elapsed_time_min - hours * 60);
		double sec = floor(elapsed_time_s - hours * 60.0 * 60.0 - minutes * 60.0);
		double milis = floor(elapsed_time_ms - hours * 60.0 * 60.0 * 1000.0 - minutes * 60.0 * 1000.0 - sec * 1000.0);

		cout << "data." + nr + ":" << "  CARL: " << setw(5) << cmax << '\t';
		cout << setw(3) << setfill('0') << hours << ':';
		cout << setw(2) << minutes << ':';
		cout << setw(2) << sec << ':';
		cout << setw(3) << milis; 
		cout << setfill(' ') << endl;
		if (czyWyswKol)
			wyswietl_kolejnosc(pi_ost);
	}

	file.close();

	system("pause");
	return 0;
}
