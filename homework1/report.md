# 41443123

作業一

## problem1

## 解題說明

本題目標為計算 Ackermann 函數 $A(m, n)$，並分別以遞迴（Recursive）與非遞迴（Non-recursive / Iterative）兩種方式進行實作。

Ackermann 函數的算術規則如下：

* 當 $m = 0$ 時：$A(m, n) = n + 1$

* 當 $m > 0$ 且 $n = 0$ 時：$A(m, n) = A(m - 1, 1)$

* 當 $m > 0$ 且 $n > 0$ 時：$A(m, n) = A(m - 1, A(m, n - 1))$

## 程式實作

以下為 C++ 實作程式碼，採用標準庫 `std::stack` 與函式指標結構來區隔非遞迴邏輯：

```cpp
#include <iostream>
#include <stack>

// 遞迴實作版本
int ackermannRecursive(int m, int n) {
    if (m == 0) return n + 1;
    if (n == 0) return ackermannRecursive(m - 1, 1);
    return ackermannRecursive(m - 1, ackermannRecursive(m, n - 1));
}

// 非遞迴實作版本 (使用 std::stack 模擬函數呼叫棧)
int ackermannIterative(int m, int n) {
    std::stack<int> s;
    s.push(m);

    while (!s.empty()) {
        int curr_m = s.top();
        s.pop();

        if (curr_m == 0) {
            n = n + 1;
        } else if (n == 0) {
            n = 1;
            s.push(curr_m - 1);
        } else {
            s.push(curr_m - 1);
            s.push(curr_m);
            n = n - 1;
        }
    }
    return n;
}

int main() {
    int m = 3, n = 2;
    std::cout << "Ackermann Recursive: " << ackermannRecursive(m, n) << std::endl;
    std::cout << "Ackermann Iterative: " << ackermannIterative(m, n) << std::endl;
    return 0;
}
```

## 效能分析

* **時間複雜度**：由於 Ackermann 函數為超指數級成長，計算時間開銷與最終結果數值直接相關，因此時間複雜度標記為 $\mathcal{O}(A(m, n))$。

* **空間複雜度**：

  * 遞迴版本：依賴系統隱式 Call Stack，最大呼叫深度為 $\mathcal{O}(A(m, n))$。

  * 非遞迴版本：使用顯式 `std::stack` 保存狀態，空間最大開銷同樣為 $\mathcal{O}(A(m, n))$。

## 測試與驗證

| 測試案例 | 輸入條件 $(m, n)$ | 預期與實際輸出 | 
| ----- | ----- | ----- | 
| 測試一 | $A(0, 3)$ | `4` | 
| 測試二 | $A(1, 1)$ | `3` | 
| 測試三 | $A(2, 1)$ | `5` | 
| 測試四 | $A(3, 2)$ | `29` | 
| 測試五 | $A(3, 4)$ | `125` | 

### 編譯結果

```bash
g++ src/problem1.cpp -std=c++17 -o problem1
./problem1
Ackermann Recursive: 29
Ackermann Iterative: 29
```

## 申論及開發報告

遞迴版本是直接依照 Ackermann 函式的數學定義進行實作，透過函式自己呼叫自己的方式完成計算，遞迴程式的結構較為簡潔，也容易理解，但當遞迴層數過深時，會消耗較多的記憶體，且可能降低執行效率，甚至造成 Stack Overflow，非遞迴版本則不使用函式自行呼叫的方式，而是利用 Stack 來模擬遞迴的執行過程。由於 Stack 具有後進先出（LIFO）的特性，可以將尚未完成的計算工作暫時保存，並依照正確的順序取出處理，因此能夠模擬原本遞迴函式的執行方式。

---

## problem2

## 解題說明

本題目標為計算一個集合的 Powerset（冪集），亦即找出該集合所有可能的子集合，並以遞迴方式完成。

例如：
$S = \{1, 2, 3\}$

每一個元素都有「選擇」和「不選擇」兩種情況：

1. 不選擇目前的元素。

2. 選擇目前的元素。

透過遞迴將每個元素分成這兩種情況，就可以找出所有可能的子集合。當所有元素都處理完時，就將目前的結果輸出。

因為每個元素都有 $2$ 種選擇，所以如果集合有 $n$ 個元素，總共有 $2^n$ 個子集合。

## 程式實作

以下採用 C++ 遞迴分支實作，並透過印出格式化符號呈現：

```cpp
#include <iostream>
#include <vector>

void powersetHelper(const std::vector<int>& inputSet, size_t pos, std::vector<int>& subset) {
    if (pos == inputSet.size()) {
        std::cout << "{ ";
        for (size_t i = 0; i < subset.size(); ++i) {
            std::cout << subset[i] << " ";
        }
        std::cout << "}\n";
        return;
    }

    // 情況 1：不包含目前元素
    powersetHelper(inputSet, pos + 1, subset);

    // 情況 2：包含目前元素
    subset.push_back(inputSet[pos]);
    powersetHelper(inputSet, pos + 1, subset);
    subset.pop_back(); // 回溯 restore
}

int main() {
    std::vector<int> S = {1, 2, 3};
    std::vector<int> subset;
    powersetHelper(S, 0, subset);
    return 0;
}
```

## 效能分析

* **時間複雜度**：包含 $n$ 個元素的集合共有 $2^n$ 個子集合，故演算法時間複雜度為 $\mathcal{O}(2^n)$。

* **空間複雜度**：遞迴呼叫深度與目前暫存子集大小最大不超過 $n$，因此空間複雜度為 $\mathcal{O}(n)$。

## 測試與驗證

測試輸入：$S = \{1, 2, 3\}$

執行輸出結果：

```text
{ }
{ 3 }
{ 2 }
{ 2 3 }
{ 1 }
{ 1 3 }
{ 1 2 }
{ 1 2 3 }
```

### 編譯結果

```bash
g++ src/problem2.cpp -std=c++17 -o problem2
./problem2
{ }
{ 3 }
{ 2 }
{ 2 3 }
{ 1 }
{ 1 3 }
{ 1 2 }
{ 1 2 3 }
```

## 申論及開發報告

這題是利用遞迴的方式來產生集合的所有子集合。每處理一個元素時，都會分成「選擇」和「不選擇」兩種情況，再繼續處理下一個元素。當所有元素都處理完成後，就會將目前產生的子集合輸出，因為每個元素都有兩種選擇，所以一個有 $n$ 個元素的集合，最後會產生 $2^n$ 個子集合，在開發過程中，我使用 vector 來存放原本的集合以及目前產生的子集合，並利用 index 判斷目前處理到哪一個元素，透過遞迴不斷處理「選擇」與「不選擇」的情況，最後成功產生所有可能的子集合，透過這次作業，我更加了解遞迴的使用方式，也了解到冪集的數量會隨者集合元素增加而快速增加。
