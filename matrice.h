#include <cmath>
#include <stdexcept>
#include <iostream>
#include <vector>
#include "complessi.h"
using namespace std;

vector<C> operator/(vector<C> v, C c){
    if (c == C(0)) throw invalid_argument("can't divide by zero");
    vector<C> w;
    for (C& e: v) w.push_back(e/c);
    return w;
}
vector<C> operator*(vector<C> v, C c){
    vector<C> w;
    for (C& e: v) w.push_back(e*c);
    return w;
}
vector<C> operator+(vector<C> v, vector<C> w){
    vector<C> res;
    for(int i = 0; i < static_cast<int>(v.size()); ++i) res.push_back(v[i]+w[i]);
    return res;
}
vector<C> operator-(vector<C> v, vector<C> w){
    vector<C> res;
    for(int i = 0; i < static_cast<int>(v.size()); ++i) res.push_back(v[i]-w[i]);
    return res;
}
struct matrice {
    vector<vector<C>> M;
    int rango_righe = M.size();
    int rango_colonne = M[0].size();

    matrice(vector<vector<C>> v): M(v) {}
    matrice() = default;
    matrice ridotta(int i, int j){
        matrice n = *this;
        n.M.erase(n.M.begin() + i);
        for(vector<C>& riga:n.M) riga.erase(riga.begin()+j);
        return n;
    }
    void scambia(int i, int l){
        vector<C> v = M[i];
        M[i] = M[l];
        M[l] = v;
    }
        
    void soluta(){ 
        /* per ogni riga i seleziono l'elemento i della riga chiamato pivot. se l'elemento e' 0 allora scambio la riga con la successiva fino a che non e' 0 altrimenti
         * ignoro la colonna. Divido la riga i per il pivot. moltiplico poi tutte le righe successive j per -pivot/e_ji e le sostituisco alla somma tra loro e la riga i.*/
        for(int i = 0; i < rango_righe; ++i){
            int l = i;
            while(M[i][i].is_zero()) {
                ++l;
                if (l < rango_righe) scambia(i,l);
                else throw invalid_argument("non me mette tutti i fattori == 0 dai");
                }
            M[i] = M[i] / M[i][i];
            for(int n = i+1; n < rango_righe; ++n){
                if (M[n][i].is_zero()) continue;
                else M[n] =  (M[n] * (M[i][i]/M[n][i])) - M[i];
            }
        }
        for(int i = rango_righe-1; i >= 0; --i){
            //C& pivot = M[i][i];
            int l = i;
            while(M[i][i].is_zero()) {
                --l;
                if (l >= 0) scambia(i,l);
                else throw invalid_argument("non me mette tutti i fattori == 0 dai");
                }
            M[i] = M[i] / M[i][i];
            for(int n = i-1; n >= 0; --n){
                if (M[n][i].is_zero()) continue;
                else M[n] =  (M[n] * (M[i][i]/M[n][i])) - M[i];
            }
        }
    }
};
            
ostream& operator <<(ostream& o, matrice m) {
    for(vector<C>& riga:m.M) {
        for(C& e:riga) cout << e << " ";
        cout << "\n";
    }
    return o;
}
