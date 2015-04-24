#include <cmath>
#include <stdexcept>
#include <iostream>
using namespace std;
struct C {
    // a + bi in C forall a, b in R
    double a;
    double b;
    C() = default;
    C(double f, double i=0): a(f), b(i) {}
    C operator+(C c) {return C(a+c.a,b+c.b);}
    C operator-(C c) {return C(a-c.a,b-c.b);}
    C operator*(C c) {return C(a*c.a-b*c.b,a*c.b+c.a*b);}
    C operator/(C c) {return C((a*c.a+b*c.b)/(pow(c.a,2)+pow(c.b,2)),(b*c.a - a*c.b)/(pow(c.a,2)+pow(c.b,2)));}
    C operator==(C c) {return a == c.a && b == c.b;}
    operator double(){ if(b == 0) 
                        return double(a);
                       else 
                        throw invalid_argument(
                        "can't convert a complex number with an imaginary part to a double");}
};

C pow(C c, int e) {
    C res = 1;
    for (int i = 0; i!=abs(e); ++i) res=res*c;
    //beware of special case where c == 0 + 0i
    return e > 0 || res == C(0) ? res : C(1)/res;
}

ostream &operator<<(ostream &o, C c) { 
    if (c.b > 0) return cout << c.a << " + " << c.b << "i";
    if (c.b == 0) return cout << c.a;
    if (c.b < 0) return cout << c.a << " - " << c.b << "i";
}
