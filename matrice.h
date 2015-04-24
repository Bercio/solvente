#include <cmath>
#include <stdexcept>
#include <iostream>
#include <vector>
using namespace std;

struct matrice {
    M = vector<vector<C>>;
    rango_righe = M.size();
    rango_colonne = M[0].size();

    matrice ridotta(int i, int j){
        matrice n = this;
        n.M.erase(n.M.begin() + i);
        for(auto riga:n.M) riga.erase(riga.begin()+j);
        return n;
    }
    
    C det();
    matrice T() {
        matrice n;
        for(int i = 0; i < rango_righe; ++i){
            n.M.push_back(vector<C>);
            for(auto riga:M) n.M[i].push_back(riga[i]);
        }
        return n;
    }
    matrice operand*(matrice n){
        if (rango_colonne != n.rango_righe || rango_righe != n.rango_colonne) 
            throw invalid_argument("solo matrici il cui rango colonna coincide 
            col rango riga dell'altro fattore e viceversa possono essere 
            moltiplicate");
        matrice res;
        for(int j = 0; j < rango_righe; ++j){ 
            res.push_back(vector<C>);
            for(int i = 0; i < n.rango_colonne){
                res[j].push_back(M[j][i]*n.M[i][j]);
            }
        }
        return res;
    }
    matrice inversa() {
        matrice n;
        for(int i = 0; i < rango_righe; ++i){
            n.M.push_back(vector<C>);
            for(int j = 0; j < rango_colonne; ++j){
                n.M[i].push_back(pow(-1,i+j)*this->ridotta(i, j).det()/this->det());
            }
        }
        return n.T();
    }
};
            
    }
ostream& operator <<(ostream& o, matrice m) {
    for(auto riga:m.M) {
        for(auto e:riga) cout << e << " ";
        cout << "\n";
    }
    return o;
}
