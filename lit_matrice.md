% Risoluzione di sistemi di equazioni in `C++`
%
% Lorenzo Bercelli

# Rappresentazione di matrici

Sapendo che per risolvere un sistema di equazioni è necessario rappresentare le 
equazione come righe di una matrice, avente come colonne i vari coefficienti 
delle incognite, ho inanzitutto scelto di rappresentare una matrice come un 
vettore di vettori di numeri complessi (un tipo creato da me nell'header 
`complessi.h`, che commento nel capitoletto [Rappresentazione di numeri 
complessi](#rappresentazione-di-numeri-complessi)). 

Ho poi creato attorno a questa rappresentazione una classe, `matrice`, così da 
poterla poi dotare delle funzioni membro che mi sarebbero necessitate.
A questa classe ho poi aggiunto per comodità gli attributi `rango_righe` 
e `rango_colonne`, il primo equivalente al numero di vettori(righe) contenuti 
nella matrice e il secondo al numero di elementi in ognuno dei suddetti 
vettori(righe).

```cpp
#include <cmath>
#include <stdexcept>
#include <iostream>
#include <vector>
using namespace std;

struct matrice {
    M = vector<vector<C>>;
    rango_righe = M.size();
    rango_colonne = M[0].size();
    };
```

## Calcolo del determinante

Poichè, una volta rappresentato un sistema di equazioni come una matrice *A* 
e i termini noti e le incognite come due vettori *b* e *x*, la soluzione del 
sistema è data da $x = A^{-1} \cdot b$, risulta necessario creare una funzione 
che inverta una matrice data. Per "tradurre" in C++ la formula $$A^{-1} 
= \frac{1}{|A|} (\text{cof }A)^T$$ (dove cof *A* indica la matrice dei 
cofattori di A) ho inanzitutto implementato la formula di Laplace per il 
determinante: $$\det(A) = \sum_{i=1}(-1)^{i+j}a_{i,j}M_{i,j}$$ dove $M_{i,j}$ 
indica il determinante di *A* privata della riga *i* e della colonna *j*; Per 
implementare questa "riduzione" della matrice ho preferito creare una funzione 
"aiutante" aggiuntiva, `ridotta`, anche in previsione della sua utilità nel 
calcolare l'inversa.

```cpp
    matrice ridotta(int i, int j){
        matrice n = this;
        n.M.erase(n.M.begin() + i);
        for(auto riga:n.M) riga.erase(riga.begin()+j);
        return n;
    }
```

Per calcolare il determinante di qualsiasi matrice basta dunque applicare una 
funzione ricorsiva che riduca la matrice fino ad una matrice 1x1 e vi applichi 
dunque le dovute moltiplicazioni e somme:

```cpp
    C det();
```

## Trasposizione della matrice

Avendo la funzione per calcolare il determinante e la funzione per ridurre una 
matrice, applicando le quali posso facilmente trovare la matrice di cofattori,
necessito ora solamente di una funzione per calcolarne la trasposta, al fine 
poi di trovare l'inversa.

``` cpp
    matrice T() {
        matrice n;
        for(int i = 0; i < rango_righe; ++i){
            n.M.push_back(vector<C>);
            for(auto riga:M) n.M[i].push_back(riga[i]);
        }
        return n;
    }
```

## Inversa della matrice

Avendo implementato dunque tutte le funzioni che mi necessitavano, Ho infine 
scritto una funzione, `inversa`, che applicasse la formula precedentemente 
citata $$A^{-1} = \frac{1}{|A|} (\text{cof }A)^T$$.

~~~cpp
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
~~~

## Moltiplicazione tra matrici

Visto che $x = A^{-1}\cdot b$, per risolvere un sistema di equazioni devo poter 
moltiplicare una matrice per un vettore, che altro non è che una matrice con 
una sola colonna. Tanto vale dunque per amor di generalità implementare la 
moltiplicazione tra matrici, che per rispettare la notazione convenzionale ho 
scelto di dichiarere come overload dell'operatore `*`. 

Nel caso l'utente tentasse di moltiplicare matrici aventi ranghi discordi ho 
scelto di lanciare un'eccezione `invalid_argument`.

```cpp
    matrice operand*(matrice n){
        if (rango_colonne != n.rango_righe || rango_righe != n.rango_colonne) 
            throw invalid_argument("solo matrici il cui rango colonna coincide "
                                   "col rango riga dell'altro fattore "
                                   "e viceversa possono essere moltiplicate");
        matrice res;
        for(int j = 0; j < rango_righe; ++j){ 
            res.push_back(vector<C>);
            for(int i = 0; i < n.rango_colonne){
                res[j].push_back(M[j][i]*n.M[i][j]);
            }
        }
        return res;
    }
```

## Rappresentazione grafica di una matrice

Per mostrare all'utente il risultato, e anche per testare il programma durante 
lo sviluppo, ho definito un overload dell'operatore `<<` che mostrasse la 
matrice come colonne divise da spazi e righe divise da capoversi.

```cpp
ostream& operator <<(ostream& o, matrice m) {
    for(auto riga:m.M) {
        for(auto e:riga) cout << e << " ";
        cout << "\n";
    }
    return o;
}
```

# Rappresentazione di numeri complessi

Per poter risolvere sistemi di equazione in C ho implementato una classe che 
rappresentasse i numeri complessi e definisse alcune loro operazioni necessarie 
alla risoluzione di sistemi di equazione. 

Ho rappresentato i complessi come una coppia di double, `a` e `b`, il primo 
rappresentate la parte reale del numero e il secondo il fattore della parte 
immaginaria.

~~~cpp
#include <cmath>
#include <stdexcept>
#include <iostream>
using namespace std;
struct C {
    // a + bi in C forall a, b in R
    double a;
    double b;
};
~~~

## Conversione tra complessi e doubles

Ho poi creato un costruttore che avesse come argomenti due double, il primo 
rappresentante la parte reale e il secondo il fattore della parte immaginaria.
Ho scelto di rendere il secondo un `default argument` uguale a 0 così da poter 
convertire implicitamente doubles in complessi. 

Ho poi aggiunto un costruttore di default, così da poter inizializzare 
complessi senza fornire argomenti, e un overload della conversione in double, 
così da poter implicitamente convertire complessi con parte immaginaria nulla 
in doubles. Nel caso si tentasse di convertire numeri immaginari in doubles ho 
scelto di lanciare un'eccezione `invalid argument`.

~~~cpp
    C() = default;
    C(double f, double i=0): a(f), b(i) {}
    operator double(){ if(b == 0) return double(a);
                       else throw invalid_argument(
                       "impossibile convertire numeri complessi con parte "
                       "immaginaria in doubles");}
~~~

## Operazioni tra complessi

Infine ho implementato l'addizione, la sottrazione, la moltiplicazione, la 
divisione e la (dis)eguaglianza tra numeri complessi.

~~~cpp
    C operator+(C c)  {return C(a+c.a,b+c.b);}
    C operator-(C c)  {return C(a-c.a,b-c.b);}
    C operator*(C c)  {return C(a*c.a-b*c.b,a*c.b+c.a*b);}
    C operator/(C c)  {return C((a*c.a+b*c.b)/(pow(c.a,2)+pow(c.b,2)),
                            (b*c.a - a*c.b)/(pow(c.a,2)+pow(c.b,2)));}
    C operator==(C c) {return a == c.a && b == c.b;}
    C operator!=(C c) {return !(this == c);}
~~~

Per ultima ho creato una parziale funzione di potenza, che accetta come 
esponenti solo interi. Ho reputato eccessiva, in relazione all'utilità, la 
complessita necessaria per implementare una funzione di potenza completa. Se in 
futuro desiderassi un'implementazione completa dei numeri complessi potrei 
usufruire della classe `complex`, presente nella `stdlib`. Stesso discorso vale 
per l'implementazione delle radici quadrate.

~~~cpp
C pow(C c, int e) {
    C res = 1;
    for (int i = 0; i!=abs(e); ++i) res=res*c;
    //attenzione al caso speciale in cui c == 0 + 0i
    return e > 0 || res == C(0) ? res : C(1)/res;
}
~~~

## Rappresentazione grafica dei complessi

Infine ho definito un overload dell'operatore `<<` per i complessi che li 
rappresentasse nella forma $a \pm bi$.

~~~cpp
ostream &operator<<(ostream &o, C c) {
    if (c.b > 0) return cout << c.a << " + " << c.b << "i";
    if (c.b == 0) return cout << c.a;
    if (c.b < 0) return cout << c.a << " - " << c.b << "i";
}
~~~

# Trasformazione di sistemi di equazioni in matrici

Ho scelto di implementare il risolutore di sistemi di equazione come un 
programma per il terminale, che accetti come argomenti le equazioni da 
risolvere e ritorni i valori delle varie incognite.

Per ottenere questo comportamento ho dovuto innanzitutto trasformare una serie 
di equazioni nel tradizionale formato "ax+by = c" in una moltiplicazione tra 
l'inversa della matrice dei fattori delle incognite e il vettore dei termini 
noti.

Ho iniziato traducendo un'equazione in un vettore composto dai fattori delle 
incognite e, all'ultimo membro, dal termine noto.
~~~cpp
#include <iostream>
#include <string>
#include <vector>
#include "matrice.h"
using namespace std;
vector<C> trad_eq(string s){
\\TODO: usa atod invece del loop;
    string fattore;
    vectro<C> v;
    for(auto simbolo:s){
        if !isalpha(simbolo) fattore += simbolo
        else {
            v.push_back(C(fattore));
            fattore = "";
        }
    v.push_back(C(fattore));
    }
    return v
}



trasformare Uso una map per mappare da sinistra a destra le incognite della 
prima equazione agli indici della loro posizione della matrice che vado 
creando. In questo modo posso accettare ordini diversi di incognite tra le 
varie equazioni.
