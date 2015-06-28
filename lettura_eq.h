#include "complessi.h"
#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#include <set>
#ifndef LETTURA_EQ
#define LETTURA_EQ
using namespace std;
void aumenta_noto( bool termine_noto, bool segno, C fattore, C &noto) {
        if (termine_noto) { 
            if (!segno) fattore *=-1;
            noto += fattore;
        }
}
vector<C> da_eq_a_riga(string str ,set<char>& incognite){
    vector<C> riga;
    map<char, C> coefficiente;
    C fattore = 1;
    bool segno = 0;
    bool termine_noto = 0;
    C noto=0;
    str.erase( remove_if( str.begin(), str.end(), ::isspace ), str.end() );
    istringstream s(str);
    while (s){
        char next = s.get();
        if ( (next == '=')) { 
            aumenta_noto( termine_noto, segno, fattore, noto);
            segno = 1;
            termine_noto=0;
            if (isalpha(s.peek()) && s.peek() != 'i') fattore = 1;
        }
        else if (isdigit(s.peek()) || isdigit(next) || s.peek() == '(' || next == '(' || next == 'i' || s.peek() == 'i'){
            if (next != '=') s.unget();
            aumenta_noto( termine_noto, segno, fattore,noto);
            s >> fattore; 
            termine_noto = 1;
        }
        else if ((next == '+' || next == '-') && isalpha(s.peek())) next == '-' ? fattore = -1 : fattore = 1;
        else if (isalpha(next)) {
            if (segno) fattore*=-1;
            coefficiente[next] =  coefficiente.count(next) ? coefficiente[next] + fattore : fattore;
            termine_noto = 0;
        }
    }
    // se l'ultimo elemento dell'equazione era un numero allora e' un termine noto
    aumenta_noto( termine_noto, segno, fattore,noto);
    for(const char& c:incognite) {
        if (!coefficiente.count(c)) coefficiente[c] = 0;
    }

    for(auto& n:coefficiente) { 
        incognite.insert(n.first); 
        riga.push_back(n.second);
    }

    riga.push_back(noto);
    return riga;
}
#endif
