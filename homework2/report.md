# 41143123

作業二

## 解題說明

本題要求實現一個多項式（Polynomial）抽象資料型態（ADT）。多項式由數個非零項目（Term）組成，每個項目包含「係數（Coefficient）」與「指數（Exponent）」。程式必須支援動態配置記憶體陣列來儲存項目，並實現多項式的相加、相乘、求值，以及多載流輸入（>>）與輸出（<<）運算子。

### 解題策略

1. 結構設計：定義 Term 類別儲存單一項目的係數與指數，並將其設為 Polynomial 的友元類別（Friend Class）。
2. 動態記憶體管理：在 Polynomial 內部使用動態陣列 termArray，並在容量（Capacity）不足時透過 NewTerm 函數自動將記憶體空間加倍。同時實作複製建構子與賦值運算子，確保深層複製（Deep Copy）以防記憶體錯誤。
3. 運算優化與排序：在相加（Add）時採用雙指標同步走訪兩個陣列，依指數降冪依序合併；相乘（Mult）則透過雙重迴圈逐項相乘，並呼叫 Add 函數自動合併同類項，確保多項式始終維持排序穩定性。

## 程式實作

以下為主要程式碼：

```cpp
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
        if (i > 0 && poly.termArray[i].coef > 0) os << " + ";
        else if (poly.termArray[i].coef < 0) {
            if (i > 0) os << " - ";
            else os << "-";
        }
        float absCoef = std::abs(poly.termArray[i].coef);
        if (absCoef != 1 || poly.termArray[i].exp == 0) os << absCoef;
        if (poly.termArray[i].exp > 0) {
            os << "x";
            if (poly.termArray[i].exp > 1) os << "^" << poly.termArray[i].exp;
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
        float c; int e;
        std::cout << "請輸入第 " << i + 1 << " 項的係數與指數: ";
        is >> c >> e;
        Polynomial temp;
        temp.NewTerm(c, e);
        poly = poly.Add(temp);
    }
    return is;
}
```
## 效能分析

1. 時間複雜度：
 多項式相加（Add）：設兩多項式項數分別為 \(m\) 與 \(n\)，時間複雜度為 \(O(m + n)\)。
 多項式相乘（Mult）：時間複雜度為 \(O(m^2 \cdot n)\)，其中每一次 Add 累加最高花費與目前累積項數呈線性相關的時間。
 多項式求值（Eval）：時間複雜度為 \(O(n)\)。
2. 空間複雜度：
   陣列儲存與動態配置空間複雜度皆為 \(O(n)\)，其中 \(n\) 為非零項目的總數量。

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數（多項式操作） | 預期輸出 | 實際輸出 |
| :--- | :--- | :--- | :--- |
| 測試一 | 輸入 P1: $3x^2 + 2x + 1$ | 印出正確多項式格式 | 成功印出 $3x^2 + 2x + 1$ |
| 測試二 | P1 + P2（相加運算） | 指數相同者係數相加 | 係數正確合併且降冪排列 |
| 測試三 | P1 * P2（相乘運算） | 逐項展開並自動合併同類項 | 成功產出正確相乘多項式 |
| 測試四 | P1.Eval(2)（代入 $x=2$ 求值）| 計算出精確浮點數值 | 輸出正確代入數值 |
| 測試五 | 輸入含有係數為 0 的項目 | 自動忽略零項，不佔用空間 | 成功過濾零項 |

### 編譯與執行指令

```shell
\$ g++ -std=c++17 -o poly poly.cpp
\$ ./poly
```

### 結論

1. 程式能正確執行多項式的動態動態配置，並隨項目增加自動安全地進行加倍擴展。
2. 透過多載 `<<` 與 `>>` 運算子，實現了直觀、人性化且符合數學書寫習慣的多項式輸入與輸出介面。
3. 經測試案例驗證，相加與相乘演算法均能精確處理同類項合併與降冪排序，動態記憶體解構亦無洩漏。

## 申論及開發報告

### 選擇動態陣列的原因

在本程式中，使用動態配置陣列（Dynamic Array）來儲存多項式項目的主要原因如下：

1. **高記憶體連續性與存取效率**
   動態陣列在記憶體中佔用連續空間，相較於鏈結串列（Linked List），它具有極佳的快取局部性（Cache Locality），在進行相加與求值走訪時，能大幅提升 CPU 讀取資料的效能。
2. **動態配置彈性與空間成本平衡**
   使用靜態陣列會限制多項式的最大項數，或造成極大的空間浪費。採用動態擴展（Capacity Doubling）策略，既保留了陣列隨機存取的優勢，又能在需要時彈性配置記憶體，且每筆資料不需額外儲存指標節點，節省了記憶體開銷。
3. **程式邏輯簡單直觀**  
   遞迴的寫法能夠清楚表達「將問題拆解為更小的子問題」的核心概念。  
   例如，計算 $\Sigma(n)$ 的過程可分解為：  

   $$
   \Sigma(n) = n + \Sigma(n-1)
   $$

   當 $n$ 等於 1 或 0 時，直接返回結果，結束遞迴。

4. **易於實現多項式乘法與加法運算**
   動態陣列在進行走訪時具有連續的記憶體位址，特別適合用來實作雙指標（Two-Pointer）的線性走訪合併。
   以本程式的相加（Add）邏輯為例：

```cpp
   while ((aPos < terms) && (bPos < poly.terms)) {
       if (termArray[aPos].exp == poly.termArray[bPos].exp) {
           float sumCoef = termArray[aPos].coef + poly.termArray[bPos].coef;
           if (sumCoef != 0) c.NewTerm(sumCoef, termArray[aPos].exp);
           aPos++; bPos++;
       } // ... 依指數大小依序走訪合併
   }
```

5. **動態擴展的語意清楚**
   在程式中，透過動態空間加倍（Capacity Doubling）的技術，讓使用者在輸入任意項數的多項式時，不需要事先宣告龐大的靜態空間。
   這種設計在語意上完全封裝了動態陣列管理的複雜度，對使用者而言非常透明。

透過動態陣列實作多項式 ADT，程式在存取效率、快取局部性與空間成本之間取得了極佳的平衡，非常適合用於項目密集的計算。然而，當多項式非常稀疏（即最高次方極大但非零項極少）且需要頻繁插入或刪除中間項目時，則可考慮改用鏈結串列（Linked List）架構來避免陣列資料頻繁搬移的問題。
