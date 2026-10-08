# C++ 基本構文

C の構文はほぼそのまま使えます。`if`、`for`、`switch` も、ポインタや構造体やビット演算も [C のまとめ](C.md)と同じ。ここには C++ で増えたものだけを書きます。Python は [Python.md](Python.md)。

## プログラムの骨格

```cpp
#include <iostream>             // C++ の標準ヘッダは .h が付かない

int main()
{
    std::cout << "hello" << std::endl;   // std:: は標準ライブラリの名前空間
    return 0;
}
```

ファイルは `.cpp`、コンパイルは `g++ -std=c++17 main.cpp` です。

## 型と変数

```cpp
bool ok = true;                 // bool が最初から使える
auto n = 10;                    // auto は右辺から型を決める。ここでは int
std::string s = "abc";          // 文字列型。<string>
s += "def";                     // 連結できる。s.size() は 6
int *p = nullptr;               // NULL の代わり
constexpr int kMax = 8;         // コンパイル時に決まる定数。#define の代わり
std::uint8_t reg = 0xFF;        // <cstdint>。C の固定幅型もそのまま
```

## 参照

変数の別名です。ポインタと同じく呼び出し元を書き換えられますが、`*` も `&` も書きません。

```cpp
int a = 1;
int &r = a;                     // r は a の別名
r = 5;                          // a が 5 になる

void add_one_ptr(int *x) { *x += 1; }   // C の書き方。呼ぶ側は add_one_ptr(&a)
void add_one_ref(int &x) { x += 1; }    // 参照。呼ぶ側は add_one_ref(a)
```

宣言と同時に相手を決める必要があり、あとから付け替えられません。`nullptr` にもできないので、「無い」を表したいときはポインタを使います。

## 関数

```cpp
int    area(int w, int h = 1) { return w * h; }     // 既定値つきの引数
double area(double r) { return 3.14 * r * r; }      // 同じ名前でも引数の型が違えば別の関数

area(3);        // 3
area(3, 4);     // 12
area(1.0);      // 3.14。double 版が呼ばれる
```

## クラス

```cpp
class Counter {
public:                                         // 外から使える部分
    Counter(int start = 0) : count_(start) {}   // コンストラクタ。: の後ろでメンバーを初期化
    ~Counter() {}                               // デストラクタ。消えるときに呼ばれる

    void up() { count_ += 1; }
    int  count() const { return count_; }       // const は「メンバーを変えない」の約束

private:                                        // クラスの中からしか触れない部分
    int count_;
};                                              // 最後に ; が要る

Counter c;                  // 作った時点でコンストラクタが動く
Counter d(100);
c.up();
c.count();                  // 1
d.count();                  // 100
```

Python の `self` にあたるものは `this` というポインタです。ただし引数には書かず、メンバーも `count_` と名前だけで使えます。Python の `__init__` がコンストラクタ、`self.count` が `count_` に対応します。

`struct` も同じ機能を持ちます。違いは、何も書かないときに `struct` は `public`、`class` は `private` になる点だけ。

## 変数の寿命とデストラクタ

```cpp
{
    Counter c;              // ここでコンストラクタ
    c.up();
}                           // ブロックを抜けるとデストラクタが自動で呼ばれる
```

片付けをデストラクタに書いておくと、ブロックを抜けたとき必ず実行されます。ファイルを閉じる、ロックを外すといった処理を書き忘れなくなる。この使い方を RAII と呼びます。

## 標準ライブラリの入れ物

```cpp
std::array<int, 4> arr = {10, 20, 30, 40};   // 固定長。<array>
std::vector<int>   v   = {1, 2, 3};          // 伸び縮みする。<vector>
v.push_back(4);

for (int e : v) {           // 要素を順に取り出す
    sum += e;
}
for (int &e : arr) {        // 参照で受けると書き換えられる
    e += 1;
}
arr.size();                 // 4。関数に渡してもサイズを失わない
```

## テンプレート

型を後から決める関数やクラスを書けます。

```cpp
template <typename T>
T max_of(T a, T b) { return (a > b) ? a : b; }

max_of(3, 7);               // T は int
max_of(1.5, 0.5);           // T は double
```

`std::array<int, 4>` の `<int, 4>` もテンプレートに型と数を渡している書き方です。

## enum class と名前空間

```cpp
enum class LedState { Off, On };
LedState st = LedState::On;     // LedState:: を付けて使う。int には勝手に変換されない

namespace sensor {
int twice(int x) { return x * 2; }
}
sensor::twice(8);               // 16。名前の衝突を避ける仕組み
```

## はまりどころ

- `for (int e : v)` は要素のコピーを受け取ります。書き換えたいときは `int &e`。
- `v[10]` のような範囲外アクセスは、C の配列と同じく未定義動作です。
- `const` を付けたメンバー関数の中でメンバーを書き換えると、コンパイルエラーになります。
- `private` のメンバーに外から触るのもコンパイルエラー。C なら実行して気づく間違いを、C++ はコンパイル時に止めます。
- 参照は初期化なしで宣言できません。`int &r;` はエラーです。

`phase3_cpp/ex01_ring_buffer` では、ここに書いたうち `<iostream>`、`std::vector`、`new`、例外が禁止です。理由は[その README](../../phase3_cpp/ex01_ring_buffer/README.md) の「組込みで避けるもの」にあります。

## 確認環境

コード例は g++ 11.4（`-std=c++17 -Wall -Wextra -pedantic`、課題の Makefile と同じ規格）で警告なしにコンパイルし、実行して確かめました。はまりどころに書いたコンパイルエラーも再現しています。
