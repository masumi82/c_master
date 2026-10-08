# C 基本構文

見返して使うための早見表です。Python は [Python.md](Python.md)、C++ は [Cpp.md](Cpp.md)。

## プログラムの骨格

```c
#include <stdio.h>   // 標準ライブラリは <>、自作ヘッダは ""

int main(void)
{
    printf("hello\n");
    return 0;        // 0 が正常終了
}
```

文は `;` で終わり、ブロックは `{ }` で囲みます。

## 型と変数

```c
int      n = 10;         // 整数。幅は処理系依存（多くは32ビット）
double   x = 3.14;       // 浮動小数点
char     c = 'A';        // 1バイト。文字は ' '、文字列は " "
unsigned int u = 10u;    // 符号なし
uint8_t  reg = 0xFF;     // <stdint.h> の固定幅型。レジスタや通信データはこちら
const int max = 100;     // 書き換え不可
```

## 演算子

```c
+  -  *  /  %            // 算術（% は余り）
==  !=  <  >  <=  >=     // 比較（結果は 1 か 0）
&&  ||  !                // 論理
&  |  ^  ~  <<  >>       // ビット
=  +=  -=  ++  --        // 代入、インクリメント
sizeof(x)                // バイト数
cond ? a : b             // 条件演算子
```

## 制御構文

```c
if (n > 0) {
    // ...
} else if (n == 0) {
    // ...
} else {
    // ...
}

switch (cmd) {
case 1:
    do_a();
    break;               // 書かないと次の case に流れる
default:
    break;
}

for (int i = 0; i < 10; i++) { /* 回数が決まっているとき */ }
while (cond) { /* 条件が真の間 */ }
do { /* 最低1回は実行 */ } while (cond);
```

`break` でループを抜け、`continue` で次の周に進みます。

## 関数

```c
int add(int a, int b);   // プロトタイプ宣言（ヘッダに書く）

int add(int a, int b)    // 定義
{
    return a + b;
}
```

引数は値渡しで、関数にはコピーが渡ります。呼び出し元の変数を書き換えたいときはポインタを渡します。

## 配列と文字列

```c
int a[5] = {1, 2, 3};                    // 残りは 0。添字は 0〜4
size_t len = sizeof(a) / sizeof(a[0]);   // 要素数。配列を宣言したスコープ内でだけ使える
char s[] = "abc";                        // 実体は 'a' 'b' 'c' '\0' の4バイト
```

## ポインタ

```c
int  x = 10;
int *p = &x;             // & でアドレスを取る
*p = 20;                 // * でその先を読み書き。x が 20 になる

void set_zero(int *out) { *out = 0; }
set_zero(&x);            // ポインタ渡しで呼び出し元の x を書き換える

int *q = a;              // 配列名は先頭要素へのポインタになる
                         // a[i] と *(a + i) は同じ意味
```

## 構造体・enum・typedef

```c
typedef struct {
    uint8_t id;
    int     value;
} sensor_t;

sensor_t sen = { .id = 1, .value = 100 };
sen.value = 200;         // 実体からは .
sensor_t *ps = &sen;
ps->value = 300;         // ポインタ経由は ->

typedef enum { LED_OFF, LED_ON } led_state_t;   // 0, 1 と順に振られる
```

## プリプロセッサ

```c
#define BUF_SIZE 64             // 定数マクロ。コンパイル前に置き換えられる
#define SQUARE(x) ((x) * (x))   // 引数付きは全体と各引数を括弧で囲む

#ifndef SENSOR_H                // インクルードガード。ヘッダの二重読み込みを防ぐ
#define SENSOR_H
// 宣言を書く
#endif
```

## はまりどころ

- `if (x = 5)` は代入で、常に真になります。比較は `==`。
- 整数どうしの割り算は切り捨て。`5 / 2` は `2`、`5 / 2.0` なら `2.5` です。
- `switch` は `break` を忘れると下の `case` まで実行されます。
- 配列を関数に渡すとポインタになり、サイズ情報が消えます。要素数は別の引数で渡してください。
- `char` が符号付きかどうかは処理系で変わります。Raspberry Pi（ARM Linux）は符号なし、x86 は符号付き。バイトデータを `uint8_t` で持てば差が出ません。
- 初期化していないローカル変数の値は不定です。

`const`、`static`、`extern` は `answers/phase1_lang/読み物_型とstatic_extern_const_課題別.md` に書いてあります（`answers/` は git 管理外で、手元にだけあります）。

## 確認環境

コード例は gcc 11.4（`-std=c11 -Wall -Wextra -pedantic`、x86_64）で警告なしにコンパイルし、実行して確かめました。演算子の一覧とインクルードガードは対象外です。
