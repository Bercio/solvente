#include "lettura_eq.h"
#include "matrice.h"
#include <vector>
#include <string>
using namespace std;

int main(int argc, char *argv[])
{
    int inizio_eq = 1;
    if (argc == 1 || string(argv[1]) == "--help") 
        cout <<
        "\n Immettere come argomenti del programma le equazioni lineari del sistema"
        " di\nequazioni che si vuole risolvere. Le incognite possono essere"
        " qualsiasi\nlettera dell'alfabeto esclusa i, che è riservata alla"
        " rappresentazione dei\nnumeri complessi nel formato (a+-bi) o ,"
        " nel caso la parte reale sia uguale a 0, +-bi.\n\n"
        " Le equazioni possono presentare più' volte le stesse incognite,\n"
        "da un lato o dall'altro del segno =. Anche il termine noto può'"
        " essere\nscritto da un lato o dall'altro.\n"
        "Esempio: ax+by-cz=d-(f-gi)x, dove a,b,c sono fattori e x,y,z sono incognite.\n\n"
        " Specificare il numero di cifre decimali desiderate con -p o --precisione\n"
        "seguito da uno spazio e il numero di cifre decimali richiesto\n(4 se l'opzione non è specificata)\n"
        << endl;
    else {
         if (string(argv[1]) == "-p" || string(argv[1]) == "--precisione") { 
            CIFRE_DECIMALI = atoi(argv[2]);
            inizio_eq = 3;
         }
        vector<vector<C>> v;
        set<char> incognite; 
        for (int i = inizio_eq; i != argc; ++i){
            da_eq_a_riga(string(argv[i]),incognite);
        }
        for (int i = inizio_eq; i != argc; ++i){
            v.push_back(da_eq_a_riga(string(argv[i]),incognite));
        }
        matrice m(v);
        if (!m.solvibile()){ cout << "il sistema non ha soluzioni" << endl; return 0;}
        if (!m.unica(incognite)){  cout << "il sistema ha infinite soluzioni" << endl; return 0;} 
        else { 
            set<char>::iterator i = incognite.begin();
            vector<vector<C>>::iterator l = m.S.begin();
            for (; i != incognite.end() && l != m.S.end(); ++i, ++l){
                while (riga_vuota(*l)) {
                    if (l!=m.S.end()) ++l;
                    else return 0;
                }
                cout << *i << " = " << (*l).back() << endl;
            }
        }
    }
}
