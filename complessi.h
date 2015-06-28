#include <cmath>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#ifndef COMPLESSI_H
#define COMPLESSI_H
using namespace std;
int CIFRE_DECIMALI = 4;

bool zero(double& a, int precisione=CIFRE_DECIMALI) { return a > - pow(10, -precisione) && a < pow(10, -precisione);} 
struct C {
    // a + bi in C forall a, b in R
    double a;
    double b;
    C() = default;
    C(double f, double i=0): a(f), b(i) {}
    C& operator*=(C c) {a = (a*c.a-b*c.b);b = (a*c.b+c.a*b); return *this;}
    C& operator+=(C c) {a += c.a; b += c.b; return *this;}
    bool operator==(C c) {return a == c.a && b == c.b;}
    explicit operator double(){ if(::zero(b)) 
                        return double(a);
                       else 
                        throw invalid_argument(
                        "can't convert a complex number with an imaginary part to a double");}
    bool zero(int precisione=CIFRE_DECIMALI){return ::zero(a,precisione) && ::zero(b,precisione);}
};

C operator+(C d, C c) {return C(d.a+c.a,d.b+c.b);}
C operator-(C d, C c) {return C(d.a-c.a,d.b-c.b);}
C operator*(C d,C c) {return C(d.a*c.a-d.b*c.b,d.a*c.b+c.a*d.b);}
C operator/(C d,C c) {return C((d.a*c.a+d.b*c.b)/(pow(c.a,2)+pow(c.b,2)),(d.b*c.a - d.a*c.b)/(pow(c.a,2)+pow(c.b,2)));}
C pow(C c, int e) {
    C res = 1;
    for (int i = 0; i!=abs(e); ++i) res=res*c;
    //beware of special case where c == 0 + 0i
    return e > 0 || res == C(0) ? res : C(1)/res;
}

istream &operator>>(istream &is, C &c) {
        double a, b=0;
        char sign = '+';
        if (is.peek() == '-' || is.peek() == '+') sign = is.get();
        if (is.peek() == '(') {
            is.ignore();
            is >> a;
            char bsign = is.get();
            if (is.peek() == 'i') {
                bsign == '-' ? b = -1 : b = 1 ;
                is.ignore(2,')');
            }
            else {
                is.unget();
                is >> b;
                is.ignore(2,')');
            }
        }
        else if (is.peek() == 'i') {
            a = 0;
            b = 1;
            is.ignore();
        }
        else { 
            is >> a;
            if (is.peek() == 'i') {
                b = a;
                a = 0;
                is.ignore();
            }
        }
        sign == '-' ? c = C(a,b)*-1 : c = C(a,b);
        return is;
}

ostream &operator<<(ostream &o, C c) { 
    cout.precision(CIFRE_DECIMALI);
    if (zero(c.b)) return cout <<  c.a;
    else if (zero(c.a)) { 
        if (zero(c.b)) return cout << 0;
        else return cout <<  c.b << "i";
    }
    return cout << c.a << showpos << c.b << "i";
}

#endif
