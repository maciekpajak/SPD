#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <vector>
#include <chrono>

std::vector<int> odwroc(std::vector<int> lst)
{
	std::vector<int> lst_swp;
	while (!lst.empty())
	{
		lst_swp.push_back(lst.back());
		lst.pop_back();
	}
	return lst_swp;
}

class ListaZadan {
public:
	unsigned int nZadan;
	std::vector<int> Nr;
	std::vector<int> R;
	std::vector<int> P;
	std::vector<int> Q;

	bool usunZadanie(unsigned int ind)
	{
		if (ind > nZadan - 1) return false;
		Nr.erase(Nr.begin() + ind);
		R.erase(R.begin() + ind);
		P.erase(P.begin() + ind);
		Q.erase(Q.begin() + ind);
		nZadan -= 1;
		return true;
	}

	bool dodajZadanie(int nr, int r, int p, int q) 
	{ 
		Nr.push_back(nr); 
		R.push_back(r); 
		P.push_back(p); 
		Q.push_back(q);
		nZadan++;
		return true;
	}

	bool isEmpty() { return nZadan == 0 ? true : false; }

	bool zamienRQ() 
	{
		std::vector<int> tmp = Q;
		Q = R;
		R = tmp;
		return true;
	}
	bool odwroclst() 
	{
		Nr = odwroc(Nr);
		R = odwroc(R);
		P = odwroc(P);
		Q = odwroc(Q);
		return true;
	}

	ListaZadan(int n) 
	{
		nZadan = n;
		Nr = std::vector<int>(n);
		R = std::vector<int>(n);
		P = std::vector<int>(n); 
		Q = std::vector<int>(n);
	}
	ListaZadan() 
	{
		nZadan = 0;
	}
};

void wyswietl_kolejnosc(std::vector<int> v)
{
	std::cout << "kolejnosc: ";
	for (int i = 0; i < (int)v.size(); i++)
	{
		std::cout << v[i] + 1 << ' ';
	}
	std::cout << std::endl;
}

void swap(std::vector<int> &v, int ind1, int ind2)
{
	int tmp = v[ind1];
	v[ind1] = v[ind2];
	v[ind2] = tmp;
}

int min_vect_ind(std::vector<int> v)
{
	return std::distance(v.begin(), std::min_element(v.begin(), v.end()));
}
int min_vect_val(std::vector<int> v)
{
	return v[std::distance(v.begin(), std::min_element(v.begin(), v.end()))];
}
int max_vect_ind(std::vector<int> v)
{
	return std::distance(v.begin(), std::max_element(v.begin(), v.end()));
}
int max_vect_val(std::vector<int> v)
{
	return v[std::distance(v.begin(), std::max_element(v.begin(), v.end()))];
}

int Cmax(ListaZadan lst, std::vector<int> K)
{
	int t = 0, cmax = 0, k;
	for (int i = 0; i < (int)K.size(); i++)
	{
		k = lst.Nr[K[i]];
		t = std::max(t, lst.R[k]) + lst.P[k];
		cmax = std::max(t + lst.Q[k], cmax);
	}
	return cmax;
}

std::vector<int> Schrage(ListaZadan N)
{
	int t = min_vect_val(N.R), j;
	//G - zbior zadan do uszeregowania
	//N - zbior zadan nieuszeregowanych
	std::vector<int> K;
	ListaZadan G;
	while (!G.isEmpty() or !N.isEmpty())
	{
		while (!N.isEmpty() and (min_vect_val(N.R) <= t))
		{
			j = min_vect_ind(N.R);
			G.dodajZadanie(N.Nr[j], N.R[j], N.P[j], N.Q[j]);
			N.usunZadanie(j);

		}
		if (G.isEmpty())
		{
			t = min_vect_val(N.R);
		}
		else
		{
			j = max_vect_ind(G.Q);
			K.push_back(G.Nr[j]);
			t = t + G.P[j];
			G.usunZadanie(j);
		}
	}
	return K;
}

std::vector<int> SortR(ListaZadan N)
{
	std::vector<int> K;
	int j;
	while (!N.isEmpty())
	{
		j = min_vect_ind(N.R);
		K.push_back(N.Nr[j]);
		N.usunZadanie(j);
	}
	return K;
}
std::vector<int> SortQ(ListaZadan N)
{
	std::vector<int> K;
	int j;
	while (!N.isEmpty())
	{
		j = min_vect_ind(N.Q);
		K.push_back(N.Nr[j]);
		N.usunZadanie(j);
	}
	return K;
}

bool czy_w_wektorze(std::vector<int> v, int a)
{
	for (int i = 0; i < (int) v.size(); i++)
	{
		if (a == v[i]) return true;
	}
	return false;
}

std::vector<int> SortRQ(ListaZadan N)
{
	unsigned int n = N.Nr.size();
	std::vector<int> Kr, Kq, K(n,-1);
	unsigned int i = 0, j = 0, k = 0;
	Kr = SortR(N);
	Kq = SortQ(N);
	
	while (czy_w_wektorze(K, -1))
	{
		if (!czy_w_wektorze(K, Kr[k]))
		{
			K[i] = Kr[k];
			i++;
		}
		if (!czy_w_wektorze(K, Kq[k]))
		{
			K[n - 1 - j] = Kq[k];
			j++;
		}
		k++;
	}

	return K;
}

int data1(std::ifstream &file)
{
	int cmax,n;
	std::string data;

	//czytanie naglowka i liczby zadan
	file >> data >> n;

	ListaZadan lst(n);

	//Czytanie danych z pliku
	for (int i = 0; i < n; i++)
		{
			lst.Nr[i] = i;
			file >> lst.R[i] >> lst.P[i] >> lst.Q[i];
		}

	std::vector<int>  K = SortRQ(lst);
	cmax = Cmax(lst, K);

	std::cout << data << std::endl;
	wyswietl_kolejnosc(K);
	std::cout << "cmax: " << cmax << std::endl;
	std::cout << "++++++++++++++++++++++++++++++++++++++" << std::endl;

	return cmax;
}
int data2(std::ifstream &file)
{
	int cmax, n;
	std::string data;

	file >> data >> n;

	ListaZadan lst(n);

	//Czytanie danych z pliku
	for (int i = 0; i < n; i++)
	{
		lst.Nr[i] = i;
		file >> lst.R[i] >> lst.P[i] >> lst.Q[i];
	}

	lst.zamienRQ();
	std::vector<int>  K = Schrage(lst);
	cmax = Cmax(lst, K);
	//dodatkowe ustawianie poprzez zamienianie parami elementów i sprawdzanie czy cmax jest mniejsze
	int tmp;
	for(int i = 0; i < K.size();i++)
		for (int j = 0; j < K.size(); j++)
		{
			std::vector<int>  K1 = K;
			swap(K1, i, j);
			tmp = Cmax(lst, K1);
			if (tmp < cmax)
			{
				cmax = tmp;
				K = K1;
			}
		}

	K = odwroc(K);
	std::cout << data << std::endl;
	wyswietl_kolejnosc(K);
	std::cout << "cmax: " << cmax << std::endl;
	std::cout << "++++++++++++++++++++++++++++++++++++++" << std::endl;

	return cmax;
}
int data3(std::ifstream &file)
{
	int cmax, n;
	std::string data;

	file >> data >> n; //wczystanie nazwy i liczby danych

	ListaZadan lst(n); //inicjalizacja liczby zadan

	//Czytanie danych z pliku
	for (int i = 0; i < n; i++)
	{
		lst.Nr[i] = i;
		file >> lst.R[i] >> lst.P[i] >> lst.Q[i];
	}

	//wstepne obliczenie kolejnosci 
	std::vector<int>  K = Schrage(lst); 
	cmax = Cmax(lst, K); //obliczenie cmax
	//dodatkowe ustawianie poprzez zamienianie parami elementów i sprawdzanie czy cmax jest mniejsze
	int tmp;
	for (int i = 0; i < K.size(); i++)
		for (int j = 0; j < K.size(); j++)
		{
			std::vector<int>  K1 = K;
			swap(K1, i, j);
			tmp = Cmax(lst, K1);
			if (tmp < cmax)
			{
				cmax = tmp;
				K = K1;
			}
		}
	
	std::cout << data << std::endl;
	wyswietl_kolejnosc(K);
	std::cout << "cmax: " << cmax << std::endl;
	std::cout << "++++++++++++++++++++++++++++++++++++++" << std::endl;

	return cmax;
}
int data4(std::ifstream &file)
{
	int cmax, n;
	std::string data;

	file >> data >> n; //wczystanie nazwy i liczby danych

	ListaZadan lst(n); //inicjalizacja liczby zadan

	//Czytanie danych z pliku
	for (int i = 0; i < n; i++)
	{
		lst.Nr[i] = i;
		file >> lst.R[i] >> lst.P[i] >> lst.Q[i];
	}

	
	std::vector<int>  K = SortRQ(lst); //obliczenie kolejnosci 
	cmax = Cmax(lst, K); //obliczenie cmax
	
	std::cout <<  data << std::endl;
	wyswietl_kolejnosc(K);
	std::cout << "cmax: " << cmax << std::endl;
	std::cout << "++++++++++++++++++++++++++++++++++++++" << std::endl;
	
	return cmax;
}

int main()
{

	std::ifstream file("./rpq.data.txt", std::ios::in);
	if (!file.is_open())
	{
		std::cout << "Nie udalo sie otworzyc pliku";
		return -1;
	}
	
	std::string data, ocena;

	int suma = 0;
	int cmax[4];
	int i = 1;

	auto start = std::chrono::steady_clock::now();

	cmax[0] = data1(file);
	cmax[1] = data2(file);
	cmax[2] = data3(file);
	cmax[3] = data4(file);

	auto end = std::chrono::steady_clock::now();

	suma = cmax[0] + cmax[1] + cmax[2] + cmax[3];

	std::cout << "SUMA: " << suma << std::endl;

	if (suma <= 168306) ocena = "2.5";
	if (suma <= 127362) ocena = "3.0";
	if (suma <= 103378) ocena = "3.5";
	if (suma <= 100926) ocena = "4.0";
	if (suma <= 100381) ocena = "4.5";
	if (suma <= 100010) ocena = "5.0";
	if (suma <= 100000) ocena = "5.5";

	std::cout << "Ocena: "<< ocena << std::endl;
	std::cout << "Total time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << " ms" << std::endl;

	file.close();

	system("pause");
	return 0;
}
