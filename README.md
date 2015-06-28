% Risoluzione di sistemi di equazioni in `C++`
% Lorenzo Bercelli
% 1 giugno 2015

## Rappresentazione di sistemi di equazione

Per rappresentare un sistema di equazioni, sapendo che per risolverlo 
computazionalmente è necessario rappresentarlo come una matrice avente come 
colonne i coefficienti delle incognite, ho scelto un vettore di vettori di 
numeri complessi (un tipo creato da me nell'header `complessi.h`, che commento 
nel capitoletto [Rappresentazione di numeri 
complessi](#rappresentazione-di-numeri-complessi)) come rappresentazione in 
`C++` di un sistema. 

Ho poi scritto una funzione che costruisce la matrice semplificata applicando 
il metodo risolutivo di Gauss alla matrice di partenza al momento della sua 
inizializzazione. Sottraendo dal numero di righe della matrice iniziale il 
numero di righe con tutti gli elementi uguali a 0 della matrice semplificata ho 
dotato la matrice dell'attributo `rango`, un *int* rappresentante il rango 
della matrice.

Utilizzando il rango ho scritto una funzione che verifica che il sistema abbia 
soluzioni (che verifica cioè che il rango della matrice dei coefficienti non 
cambi con l'"aggiunta" alla matrice del vettore colonna dei termini noti, come 
ci insegna Capelli).
Infine ho scritto una funzione che verifica che il sistema abbia un'unica 
soluzione, che verifica cioè che il rango della matrice sia uguale al numero di 
incognite (variabile ottenuta durante la
[lettura di equazioni](#trasformazione-di-sistemi-di-equazioni-in-matrici)).

## Rappresentazione di numeri complessi

Per poter risolvere sistemi di equazione in $\mathbb{C}$ ho implementato una 
classe che rappresenta i numeri complessi e definisce alcune loro operazioni 
necessarie alla risoluzione di sistemi di equazione. 

Ho rappresentato i complessi come una coppia di *doubles*, `a` e `b`, il primo 
rappresentante la parte reale del numero e il secondo il fattore della parte 
immaginaria.

### Conversione tra complessi e doubles

Ho dotato la classe `complessi` di un costruttore che ha come argomenti due 
*doubles* che inizializzano la parte reale e la parte immaginaria del numero 
complesso. Ho scelto di rendere il secondo un `default argument` uguale 
a 0 così da poter convertire implicitamente doubles in complessi. 

Ho poi aggiunto un costruttore di default, così da poter inizializzare 
complessi senza fornire argomenti, e una sovrascrittura della conversione in 
*double*, così da poter convertire complessi con parte immaginaria nulla in 
*doubles*. Ho reso la conversione `explicit` per evitare di incappare in errori 
dovuti a conversioni non volute durante lo svolgimento di operazioni sui 
complessi.
Nel caso si tentasse di convertire numeri immaginari in doubles ho scelto di 
sollevare un'eccezione `domain_error`.

Infine ho scritto una funzione membro `zero()` che determina se parte reale 
e immaginaria del numero complesso siano entrambe uguali a zero entro le cifre 
decimali fornite come secondo argomento (di default uguale alla precisione 
scelta dall'utente attraverso l'opzione `-p`.)
Questa funzione viene chiamata sia per evitare di scrivere zeri superflui nella 
rappresentazione di numeri complessi (per esempio 0+5i) sia per determinare il 
rango di una matrice. Nel secondo caso la funzione viene chiamata con 15 cifre 
decimali di precisione (non di più per evitare *floating errors*), per evitare 
di determinare impossibili o indefiniti sistemi con soluzioni il cui valore 
assoluto sia minore di 1 su 10 elevato al numero di cifre decimali richiesto 
dall'utente. Da notare che questo può portare il programma a fornire soluzioni 
in apparenza errate: per esempio, se si richiedono al programma 
2 cifre decimali la soluzione del sistema $\begin{cases} 1000x=1 \\ 1000y 
  + 100x=1.1 \end{cases}$ risulterà essere $x=0.00, y=0.00$ a causa della 
  perdita di informazioni causata dall'approssimazione.

### Operazioni tra complessi

Per poter applicare il metodo di risoluzione di Gauss alle matrici con numeri 
complessi ho dunque implementato l'addizione, la sottrazione, la 
moltiplicazione, la divisione e la (dis)eguaglianza tra numeri complessi.

Per ultima ho creato una parziale funzione di potenza, che accetta come 
esponenti solo interi. Ho reputato eccessiva, in relazione all'utilità, la 
complessità necessaria per implementare una funzione di potenza completa. Se in 
futuro desiderassi un'implementazione completa dei numeri complessi potrei 
usufruire della classe `complex`, presente nella `stdlib`. Lo stesso discorso 
vale per l'implementazione delle radici quadrate.

~~~cpp
C pow(C c, int e) {
    C res = 1;
    for (int i = 0; i!=abs(e); ++i) res=res*c;
    //attenzione al caso speciale in cui c == 0 + 0i
    return e > 0 || res == C(0) ? res : C(1)/res;
}
~~~

### Lettura e scrittura di complessi

Infine ho definito una sovrascrittura dell'operatore `>>` che legge numeri 
complessi nelle forme $\pm a\pm bi$, $\pm a$ e $\pm bi$ e una sovrascrittura 
dell'operatore `<<` che li scrive nella forma $a \pm bi$, avendo cura di 
evitare la scrittura di eventuali zeri ridondanti.

## Trasformazione di sistemi di equazioni in matrici

Ho scelto di implementare il risolutore di sistemi di equazione come un 
programma per il terminale, che accetta come argomenti le equazioni da 
risolvere e emette i valori delle varie incognite con precisione scelta 
dall'utente attraverso l'opzione `-p` (4 cifre decimali di default).

Per ottenere questo comportamento ho dovuto innanzitutto scrivere una funzione 
che trasforma un'equazione lineare in un vettore di fattori di incognite -- con 
il termine noto come ultimo elemento -- e in un insieme di incognite, in ordine
coerente con quello dei fattori. Supportare la possibilità che la stessa 
incognita comparisse più volte all'interno dell'equazione, che i termini 
fossero scritti arbitrariamente a destra e sinistra del segno $=$ e che 
i fattori delle incognite fossero numeri complessi ha richiesto un certo 
sforzo, alleviato dalla capacità dell'`istringstream` di poter spostare avanti 
e indietro il proprio indice di lettura in una stringa.

Ho utilizzato una mappa (`map`) per registrare la somma dei vari coefficienti 
della stessa incognita

~~~cpp
coefficiente[next] =  coefficiente.count(next) ?
                      coefficiente[next] + fattore : fattore;
~~~
e un insieme (`set`) per evitare che la stessa incognita apparisse più di una 
volta.

È stato poi semplice in `main` processare ogni argomento fornito al programma
e costruire coi vettori risultanti la matrice rappresentante il sistema di 
equazioni da risolvere.

## Risoluzione di sistemi di equazione

Ottenuta la matrice rappresentante il sistema che si vuole risolvere 
e l'insieme delle sue incognite verifico che abbia una e una sola soluzione (in 
caso contrario scrivo su stdout se il sistema ha infinite o nessuna soluzione). 
Infine scrivo su stdout le incognite seguite dai valori estrapolati dalla 
matrice ridotta, curandomi di omettere le righe con tutti gli elementi uguali 
a zero.
