#include <iostream>
#include <cmath>
#include <algorithm>

class Polynomial;

class Term {
    friend class Polynomial;
    friend std::ostream& operator<<(std::ostream& os, const Polynomial& poly);
    friend std::istream& operator>>(std::istream& is, Polynomial& poly);
private:
    float coef; 
    int exp;    
};

class Polynomial {
public:
    Polynomial();                               
    ~Polynomial();                           
    Polynomial(const Polynomial& poly);        
    Polynomial& operator=(const Polynomial& poly);

    Polynomial Add(Polynomial poly);           
    Polynomial Mult(Polynomial poly);          
    float Eval(float f);                      

    void NewTerm(const float c, const int e);   

    friend std::ostream& operator<<(std::ostream& os, const Polynomial& poly);
    friend std::istream& operator>>(std::istream& is, Polynomial& poly);

private:
    Term *termArray;
    int capacity;   
    int terms;      
};


Polynomial::Polynomial() {
    capacity = 1;
    terms = 0;
    termArray = new Term[capacity];
}


Polynomial::~Polynomial() {
    delete[] termArray;
}


Polynomial::Polynomial(const Polynomial& poly) {
    capacity = poly.capacity;
    terms = poly.terms;
    termArray = new Term[capacity];
    for (int i = 0; i < terms; i++) {
        termArray[i] = poly.termArray[i];
    }
}


Polynomial& Polynomial::operator=(const Polynomial& poly) {
    if (this != &poly) {
        delete[] termArray;
        capacity = poly.capacity;
        terms = poly.terms;
        termArray = new Term[capacity];
        for (int i = 0; i < terms; i++) {
            termArray[i] = poly.termArray[i];
        }
    }
    return *this;
}


void Polynomial::NewTerm(const float c, const int e) {
    if (c == 0) return; 
    if (terms == capacity) {
        capacity *= 2;
        Term *temp = new Term[capacity];
        for (int i = 0; i < terms; i++) {
            temp[i] = termArray[i];
        }
        delete[] termArray;
        termArray = temp;
    }
    termArray[terms].coef = c;
    termArray[terms].exp = e;
    terms++;
}


Polynomial Polynomial::Add(Polynomial poly) {
    Polynomial c;
    int aPos = 0, bPos = 0;
    while ((aPos < terms) && (bPos < poly.terms)) {
        if (termArray[aPos].exp == poly.termArray[bPos].exp) {
            float sumCoef = termArray[aPos].coef + poly.termArray[bPos].coef;
            if (sumCoef != 0) {
                c.NewTerm(sumCoef, termArray[aPos].exp);
            }
            aPos++; bPos++;
        } else if (termArray[aPos].exp < poly.termArray[bPos].exp) {
            c.NewTerm(poly.termArray[bPos].coef, poly.termArray[bPos].exp);
            bPos++;
        } else {
            c.NewTerm(termArray[aPos].coef, termArray[aPos].exp);
            aPos++;
        }
    }
 
    for (; aPos < terms; aPos++) {
        c.NewTerm(termArray[aPos].coef, termArray[aPos].exp);
    }
    for (; bPos < poly.terms; bPos++) {
        c.NewTerm(poly.termArray[bPos].coef, poly.termArray[bPos].exp);
    }
    return c;
}


Polynomial Polynomial::Mult(Polynomial poly) {
    Polynomial c;
    for (int i = 0; i < terms; i++) {
        Polynomial temp;
        for (int j = 0; j < poly.terms; j++) {
            temp.NewTerm(termArray[i].coef * poly.termArray[j].coef, 
                         termArray[i].exp + poly.termArray[j].exp);
        }
        c = c.Add(temp); 
    }
    return c;
}


float Polynomial::Eval(float f) {
    float result = 0;
    for (int i = 0; i < terms; i++) {
        result += termArray[i].coef * std::pow(f, termArray[i].exp);
    }
    return result;
}


std::ostream& operator<<(std::ostream& os, const Polynomial& poly) {
    if (poly.terms == 0) {
        os << "0";
        return os;
    }
    for (int i = 0; i < poly.terms; i++) {
        if (i > 0 && poly.termArray[i].coef > 0) {
            os << " + ";
        } else if (poly.termArray[i].coef < 0) {
            if (i > 0) os << " - ";
            else os << "-";
        }
        
        float absCoef = std::abs(poly.termArray[i].coef);
        if (absCoef != 1 || poly.termArray[i].exp == 0) {
            os << absCoef;
        }
        
        if (poly.termArray[i].exp > 0) {
            os << "x";
            if (poly.termArray[i].exp > 1) {
                os << "^" << poly.termArray[i].exp;
            }
        }
    }
    return os;
}


std::istream& operator>>(std::istream& is, Polynomial& poly) {
   
    delete[] poly.termArray;
    poly.capacity = 1;
    poly.terms = 0;
    poly.termArray = new Term[poly.capacity];

    int numTerms;
    std::cout << "請輸入非零項目的數量: ";
    if (!(is >> numTerms)) return is;

    for (int i = 0; i < numTerms; i++) {
        float c;
        int e;
        std::cout << "請輸入第 " << i + 1 << " 項的係數與指數: ";
        is >> c >> e;
        

        Polynomial temp;
        temp.NewTerm(c, e);
        poly = poly.Add(temp);
    }
    return is;
}


int main() {
    Polynomial p1, p2;

    std::cout << "--- 輸入多項式 1 ---\n";
    std::cin >> p1;
    std::cout << "多項式 1: " << p1 << "\n\n";

    std::cout << "--- 輸入多項式 2 ---\n";
    std::cin >> p2;
    std::cout << "多項式 2: " << p2 << "\n\n";

    Polynomial sum = p1.Add(p2);
    std::cout << "相加總和 (P1 + P2): " << sum << "\n";

    Polynomial product = p1.Mult(p2);
    std::cout << "相乘乘積 (P1 * P2): " << product << "\n";

    float x = 2.0;
    std::cout << "當 x = " << x << " 時，P1 的計算結果: " << p1.Eval(x) << "\n";

    return 0;
}

