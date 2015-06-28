#include <cmath>
#include <set>
#include <exception>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "complessi.h"
using namespace std;
#ifndef MATRICE_H
#define MATRICE_H
ostream& operator <<(ostream& o, vector<C>& v) { for(C& e:v) {cout << e << " ";} return cout;}
ostream& operator <<(ostream& o, vector<vector<C>>& m) { for(vector<C>& e:m) {cout << e << "\n";} return cout;}

//verificare che il sistema abbia o meno un'unica soluzione indipendentemente 
//dalle cifre decimali che si vogliono mostrare, dunque uso 15 cifre (per 
//essere sicuro di evitare floating errors
bool riga_vuota(vector<C>& v){ return all_of(v.begin(), v.end(), [](C c){return c.zero(15);});}

vector<C> operator/(vector<C> v, C c){
    if (c == C(0)) throw invalid_argument("can't divide by zero");
    transform(v.begin(), v.end(), v.begin(), [c](C e) { return e/c;});
    return v;
}
vector<C> operator*(vector<C> v, C c){
    transform(v.begin(), v.end(), v.begin(), [c](C e) { return e*c;});
    return v;
}
vector<C> operator+(vector<C> v, vector<C> w){
    transform(v.begin(), v.end(), w.begin(), v.begin(), [](C a, C b) { return a + b;});
    return v;
}
vector<C> operator-(vector<C> v, vector<C> w){
    transform(v.begin(), v.end(), w.begin(), v.begin(), [](C a, C b) { return a - b;});
    return v;
}

struct matrice {
    vector<vector<C>> M;
    //int numero_righe = M.size();
    //int numero_colonne = M[0].size();
    int numero_righe;
    int numero_colonne;
    vector<vector<C>> S;
    int rango;

    matrice(vector<vector<C>> v, bool sistema=true): M(v) {
    numero_righe = M.size();
    numero_colonne = M[0].size();
    solvi(sistema);
    rango = numero_righe - count_if(S.begin(), S.end(), riga_vuota);
    cout << M << S << endl;
    }
    matrice() = default;
    void solvi(bool sistema=true){ 
        /* per ogni riga i seleziono l'elemento i della riga chiamato pivot. se l'elemento e' 0 allora scambio la riga con la successiva fino a che non e' 0 altrimenti
         * ignoro la colonna. Divido la riga i per il pivot. moltiplico poi tutte le righe successive j per -pivot/e_ji e le sostituisco alla somma tra loro e la riga i.*/
        S = M;
        int j = 0;
        for(int i = 0; i < numero_righe && j < numero_colonne-sistema*1; ++i,++j){
            int l = i;
            while(S[i][j].zero()) {
                ++l;
                if (l < numero_righe-1) swap_ranges(S[i].begin(),S[i].end(), S[l].begin());
		else if (j < numero_colonne-1-sistema*1) {++j; l = i;}
                else {return;}
	    }
            S[i] = S[i] / S[i][j];
            for(int n = i+1; n < numero_righe; ++n){
                if (S[n][j].zero()) continue;
                else S[n] =  (S[n] * (S[i][j]/S[n][j])) - S[i];
            }
        }
        j = numero_colonne-1-sistema*1;
        for(int i = numero_righe-1; i >= 0 && j >= 0; --i, --j){
            int l = i;
            while(S[i][j].zero()) {
                --l;
                if (l >=0) swap_ranges(S[i].begin(),S[i].end(), S[l].begin());
		else if (j >0) {--j; l = i; }
                else { return;}
	    }
            S[i] = S[i] / S[i][j];
            for(int n = i-1; n >= 0; --n){
                if (S[n][j].zero()) continue;
                else S[n] =  (S[n] * (S[i][j]/S[n][j])) - S[i];

            }
        }
    }
       
    friend std::ostream& operator <<(ostream& o, matrice m) {
    for (vector<C>& riga:m.M){
            for(C& e:riga) cout << e << " ";
                    cout << "\n";
            }
    return o;
    }
    bool solvibile(){
        vector<vector<C>> a;
        for(auto& riga:M) {
            vector<C> v;
            for(auto e = riga.begin(); e != riga.end()-1; ++e) v.push_back(*e);
            a.push_back(v);
        }
        matrice n(a, false);
        return n.rango == rango;
    }
    bool unica(set<char> incognite){
      return rango == (int)incognite.size();
    }
};
ostream& operator <<(ostream& o, matrice m);

#endif
