#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <math.h>

using namespace std;
#define d 0.5


typedef struct {
	int w, s; // wartoœæ i ciê¿ar
	float ws; // wartoœæ/ciê¿ar
} paczka;


void sort_smin(paczka p[], int m, int n) {

	int i = m, j = n, piv;
	piv = p[i].s;
	do {
		while(p[i].s < piv) i++;
		while(p[j].s > piv) j--;
		if(i<=j) swap(p[i++],p[j--]);
		}
    	while(i<j);
	if(m<j) sort_smin(p, m, j);
	if(i<n) sort_smin(p, i, n);
	}

void sort_wmax(paczka p[], int m, int n) {

	int i = m, j = n, piv;
	piv = p[i].w;
	do {
		while(p[i].w > piv) i++;
		while(p[j].w < piv) j--;
		if(i<=j) swap(p[i++],p[j--]);
		}
    	while(i<j);
	if(m<j) sort_wmax(p, m, j);
	if(i<n) sort_wmax(p, i, n);
	}

void sort_wsmax(paczka p[], int m, int n) {

	int i = m, j = n;
	float piv = p[i].ws;
	do {
		while(p[i].ws > piv) i++;
		while(p[j].ws < piv) j--;
		if(i<=j) swap(p[i++],p[j--]);
		}
    	while(i<j);
	if(m<j) sort_wsmax(p, m, j);
	if(i<n) sort_wsmax(p, i, n);
	}

int maks = 0;
int bgen(int n, int ssum, int wsum, int b, paczka *p){

	if(n<-1) return 0;

	if(ssum+p[n].s<=b) {
		bgen(n-1, ssum+p[n].s, wsum+p[n].w, b, p);
		if(wsum>maks) { maks = wsum; }
		}
	bgen(n-1, ssum, wsum, b, p);
	if(wsum>maks) { maks = wsum; }
    }


int main() {
	int n = 20; // liczba paczek
	int b; // zakres wag i wartoœci paczek

	paczka *p = new paczka[n]; // tworzenie tablicy paczek
	srand(time(0));
	for(int i=0; i<n; i++) {
		p[i].s = rand()%90 + 10;
		p[i].w = rand()%900 + 100;
	}

	b = 0;
	for(int x=0; x<n; x++) b+=p[x].s;
	b = b*d;
	printf("pojemnosc = %d\n",b);


// Progrmowanie dynamiczne (PD)
    cout<<"Programowanie dynamiczne:"<<endl;
	double_t start1 = 1.0*clock() / CLOCKS_PER_SEC;

	// tworzenie tabeli (b+1)*(n+1)
	int ** matrix = new int *[n+1];
    for(int i=0; i<=n; i++)
        matrix[i] = new int[b+1];

    //zerowanie zerowej kolumny i zerowego wiersza
    for(int i=0; i<b+1; i++) matrix[0][i] = 0;
    for(int i=0; i<n+1; i++) matrix[i][0] = 0;
    //cout << "Zrobione: 0" << endl;

    //dynamiczny algorytm plecakowy
    for(int i=1; i<=n; i++){
        for(int j=1; j<=b; j++){
            if(p[i-1].s > j) matrix[i][j] = matrix[i-1][j];
            else if(p[i-1].w + matrix[i-1][j-p[i-1].s] > matrix[i-1][j]) matrix[i][j] = p[i-1].w + matrix[i-1][j-p[i-1].s];
            else matrix[i][j] = matrix[i-1][j];
        }
    }

	double_t koniec1 = 1.0*clock() / CLOCKS_PER_SEC;
    cout << "PD = " << koniec1 - start1 << endl;
	cout << "Wartosc najwieksza = " << matrix[n][b] << endl;

// Pe³en przegl¹d (BF1)
    cout<<"Pelen przeglad (BF1):"<<endl;
	double_t start2 = 1.0*clock() / CLOCKS_PER_SEC;

    int x;
	int ssum, wsum, wmax = 0, smax =0;
	for(x=1; x<(1<<n); x++) { // dla wszystkich mo¿liwych uk³adów n zer i jedynek != od 0
		ssum = wsum = 0;
		for(int t=0; t<n; t++) { // jeœli t-ety bit od koñca x == 1 to dodajemy t-et¹ paczkê do rozwi¹zania
			if((x>>t)&1) { ssum+=p[t].s; wsum+=p[t].w; }
			}
		if((wsum > wmax) && (ssum <= b)) { wmax = wsum; smax = x; }
		}

	double_t koniec2 = 1.0*clock() / CLOCKS_PER_SEC;
	cout << "BF1 = "<< wmax <<endl;
	cout << "BF1 = "<< koniec2 - start2 << endl;

// Pe³en przegl¹d z eliminacj¹ rozwi¹zañ niedopuszczalnych (BF2)
    cout << "Przeglad z eliminacja:" << endl;
	double_t start3 = 1.0*clock() / CLOCKS_PER_SEC;

		bgen(n-1, 0, 0, b, p);


	double_t koniec3 = 1.0*clock() / CLOCKS_PER_SEC;
	cout << "BF2 = " << maks<< endl;
	cout << "BF2 = " << koniec3 - start3 << endl;

// Heurystyka - losowo (GH1)

	wsum=0;
    cout << "GH1 - losowo:" << endl;
	double_t start4 = 1.0*clock() / CLOCKS_PER_SEC;

	int sleft = b; // indeks
	wsum = 0;

	for (int i=0; i<n; i++) {
		if (p[i].s <= sleft) {
			sleft-=p[i].s;
			wsum+=p[i].w;
//			printf("chosen %d\n", i);
			}
		}

	double_t koniec4 = 1.0*clock() / CLOCKS_PER_SEC;
	cout << "GH1 = " << wsum << endl;
	cout << "GH1 = " << koniec4 - start4 << endl;
	float xgh1 = ((float)(matrix[n][b]-wsum)/matrix[n][b])*100;
	printf("(blad: %.2f%%)\n ", xgh1);

// Heurystyka - minimalny ciê¿ar (GH2)
    cout << "GH2 - minimalny ciezar:" << endl;
	double_t start5 = 1.0*clock() / CLOCKS_PER_SEC;

	sort_smin(p, 0, n-1);
	sleft = b;
	wsum = 0;
//	for (int i=0; i<n; i++) printf("%d: s=%d w=%d\n", i, p[i].s, p[i].w);

	for (int i=0; i<n; i++) {
		if (p[i].s > sleft) break; // jeœli najmniejszy element siê nie mieœci
		sleft-=p[i].s;
		wsum+=p[i].w;
//		printf("chosen %d\n", i);
		}
	double_t koniec5 = 1.0*clock() / CLOCKS_PER_SEC;
	cout << "GH2 = " << wsum << endl;
	cout << "GH2 = " << koniec5 - start5 << endl;
	float xgh2 = ((float)(matrix[n][b]-wsum)/matrix[n][b])*100;
	printf("(blad: %.2f%%)\n ", xgh2);

// Heurystyka - maksymalna wartoœæ (GH3)
    cout << "GH3 - maksymalna wartosc:" << endl;
	double_t start6 = 1.0*clock() / CLOCKS_PER_SEC;

	sort_wmax(p, 0, n-1);
	sleft = b;
	wsum = 0;
//	for (int i=0; i<n; i++) printf("%d: s=%d w=%d\n", i, p[i].s, p[i].w);

	for (int i=0; i<n; i++) {
		if (p[i].s <= sleft) {
			sleft-=p[i].s;
			wsum+=p[i].w;
//			printf("chosen %d\n", i);
			}
		}
	double_t koniec6 = 1.0*clock() / CLOCKS_PER_SEC;
	cout << "GH3 = " << wsum << endl;
	cout << "GH3 = " << koniec6 - start6 << endl;
	float xgh3 = ((float)(matrix[n][b]-wsum)/matrix[n][b])*100;
	printf("(blad: %.2f%%)\n", xgh3);

// Heurystyka - maksymalna wartoœæ/ciê¿ar (GH4)
    cout << "GH4 - maksymalna wartosc/ciezar:" << endl;
	double_t start7 = 1.0*clock() / CLOCKS_PER_SEC;

	for (int i=0; i<n; i++) p[i].ws = (float)p[i].w/p[i].s;	// obliczanie w/s
	sort_wsmax(p, 0, n-1);
//	for (int i=0; i<n; i++) printf("%d: s=%d w=%d ws=%.6f\n", i, p[i].s, p[i].w, p[i].ws);
	sleft = b;
	wsum = 0;

	for (int i=0; i<n; i++) {
		if (p[i].s <= sleft) {
			sleft-=p[i].s;
			wsum+=p[i].w;
//			printf("chosen %d\n", i);
			}
		}
	double_t koniec7 = 1.0*clock() / CLOCKS_PER_SEC;
	cout << "GH4 = " << wsum << endl;
	cout << "GH4 = " << koniec7 - start7 << endl;
	float xgh4 = ((float)(matrix[n][b]-wsum)/matrix[n][b])*100;
	printf("(blad: %.2f%%)\n ", xgh4);

	return 0;

}
