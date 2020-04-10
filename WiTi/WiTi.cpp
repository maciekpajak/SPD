#include <iostream>
#include <string>
#include <math.h>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <chrono>


#define MAX_LICZBA_ZADAN 20
#define LICZBA_PARAMETROW 3
#define CZAS_TRWANIA 0
#define WAGA 1
#define DEADLINE 2

using namespace std;

int macierzZadan[MAX_LICZBA_ZADAN][3];
vector<int> permutacjaOpt(MAX_LICZBA_ZADAN);

int pd(int n)
{
    //Obliczenie optymalnego rozwiazania
    int liczbaPodproblemow = (int)pow(2, n);
    vector<int> podproblemy(liczbaPodproblemow);
    int suma;
    for (int idx = 1; idx < liczbaPodproblemow; idx++)
    {
        suma = 0;
        for (int i = 0, b = 1; i < n; i++, b *= 2)
            if (idx & b) suma += macierzZadan[i][CZAS_TRWANIA];

        podproblemy[idx] = std::numeric_limits<int>::max();

        for (int i = 0, b = 1; i < n; i++, b *= 2)
            if (idx & b)  podproblemy[idx] = min( podproblemy[idx], podproblemy[idx - b] + macierzZadan[i][WAGA] * max(0, suma - macierzZadan[i][DEADLINE]));

    }

    //Znaleznienie permutacji dajacej optymalne rozwiazanie
    int nrZad = 0, k = liczbaPodproblemow - 1;
    for (int idx = n; idx > 0; idx--)
    { 
        suma = 0;
        for (int i = 0, b = 1; i < n; i++, b *= 2)
            if (k & b) suma += macierzZadan[i][CZAS_TRWANIA];

        int g = std::numeric_limits<int>::max();
        int h = 0;
        for (int i = 0, b = 1; i < n; i++, b *= 2)
        {
            if (k & b)
            {
                int tmp = podproblemy[k - b] + macierzZadan[i][WAGA] * max(0, suma - macierzZadan[i][DEADLINE]);
                if (g >= tmp)
                {
                    g = tmp;
                    h = k - b;
                    nrZad = i;
                }
            }
        }
        k = h;
        permutacjaOpt[idx - 1] = nrZad;
    }

    return podproblemy.back();
}

int witi(vector<int> &permutacja,  int n)
{
    int kara = 0 , czasZakonczenia = 0, spoznienie, nrZadania;

    for (int i = 0; i < n; i++)
    {
        nrZadania = permutacja[i];
        czasZakonczenia += macierzZadan[nrZadania][CZAS_TRWANIA];
        spoznienie = czasZakonczenia -  macierzZadan[nrZadania][DEADLINE] ;
        kara += max(spoznienie, 0) * macierzZadan[nrZadania][WAGA];
    }

    return kara;
}

int przegladZ(int n)
{
    vector<int> permutacja(n);
    int w = numeric_limits<int>::max();
    for (int i = 0; i < n; i++) permutacja[i] = i;

    do
    {
        w = min(w, witi(permutacja, n));
    } while (next_permutation(permutacja.begin(), permutacja.end()));

	
    return w;

}


int main()
{
    ifstream file("./witi.data.txt", ifstream::in);
    if (!file.is_open())
    {
        cout << "Nie udalo sie otworzyc pliku";
        return -1;
    }

    string line;
    int n;

     for (int dataIdx = 10; dataIdx <= 20; dataIdx++)
    {
        while (getline(file, line) && (line != "data." + std::to_string(dataIdx) + ":")); 

        file >> n;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < LICZBA_PARAMETROW; j++)
                file >> macierzZadan[i][j];

        // Algorytm PD
        auto start = std::chrono::steady_clock::now();
        int wynik = pd(n);
        auto end = std::chrono::steady_clock::now();


        std::chrono::duration<double> time = end - start;
        std::cout << "data." + std::to_string(dataIdx) + ":" << '\t' << wynik << '\t' << time.count() << endl;

        for (int i = 0; i < n; i++)
        {
            std::cout << permutacjaOpt[i] + 1 << ' ';
        }
        cout << endl;

        // Przeglad zupelny
        /*
        auto start = std::chrono::steady_clock::now();
        int wynik = przegladZ(n);
        auto end = std::chrono::steady_clock::now();

        std::chrono::duration<double>time = end - start;
        std::cout << "data." + std::to_string(dataIdx) + ":" << '\t' << wynik << '\t' << time.count() << endl;
        */
        

    }

    std::system("pause");
    return 0;
}