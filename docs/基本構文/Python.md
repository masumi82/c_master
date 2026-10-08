# Python 基本構文

C を知っている前提で、C との違いをコメントに書いています。C は [C.md](C.md)、C++ は [Cpp.md](Cpp.md)。

## プログラムの骨格

```python
def main():
    print("hello")


if __name__ == "__main__":   # このファイルを直接実行したときだけ main() を呼ぶ
    main()
```

`{ }` と `;` はありません。ブロックは `:` とインデント（空白4つ）で表します。コンパイルは不要で、`python3 hello.py` で動きます。

## 型と変数

```python
n = 10            # int。宣言は不要で、代入した値で型が決まる
x = 3.14          # float（C の double 相当）
s = "abc"         # str。' ' でも " " でも同じ
ok = True         # bool。True / False
nothing = None    # 「値がない」の印（C の NULL に近い）

n = "ten"         # 同じ変数に別の型を入れ直せる
big = 2 ** 100    # int は桁あふれしない
```

## 演算子

```python
+  -  *  %  **           # 算術（** はべき乗）
/                        # 割り算。7 / 2 は 3.5
//                       # 切り捨て割り算。7 // 2 は 3
==  !=  <  >  <=  >=     # 比較
and  or  not             # 論理（C の && || !）
&  |  ^  ~  <<  >>       # ビット。C と同じ
in                       # 含まれるか。3 in [1, 2, 3] は True
+=  -=                   # ++ と -- はない
```

## 制御構文

```python
if n > 0:
    print("plus")
elif n == 0:             # else if は elif
    print("zero")
else:
    print("minus")

for i in range(3):       # 0, 1, 2。C の for (i = 0; i < 3; i++)
    print(i)

for name in ["a", "b"]:  # 要素を順に取り出す
    print(name)

while k < 3:
    k += 1
```

`break` と `continue` は C と同じです。

## 関数

```python
def add(a, b):
    return a + b

def greet(name, greeting="hello"):   # 引数に既定値を付けられる
    return f"{greeting}, {name}"

greet("pi")                          # hello, pi
greet("pi", greeting="hi")           # 名前を指定して渡せる

def min_max(values):
    return min(values), max(values)  # 複数の値を返せる

lo, hi = min_max([3, 1, 2])          # lo は 1、hi は 3
```

## リスト・辞書

```python
a = [10, 20, 30]         # リスト。伸び縮みする配列
a.append(40)
a[0]                     # 10
a[-1]                    # 40。負の添字は後ろから
a[1:3]                   # [20, 30]。スライス。終わりの添字は含まない
len(a)                   # 4

d = {"led": 17, "button": 27}   # 辞書。キーで引く表
d["led"]                 # 17
d["buzzer"] = 22         # 追加
"led" in d               # True

t = (1, 2)               # タプル。書き換えられないリスト
squares = [v * v for v in range(5)]   # 内包表記。[0, 1, 4, 9, 16]
```

## 文字列

```python
text = " led on 3 "
parts = text.strip().split()    # ['led', 'on', '3']
int(parts[2]) + 1               # 4。文字列から数値へは明示的に変換する
f"{x:.1f} {255:#04x}"           # '3.1 0xff'。f 文字列で値を埋め込む
```

## クラス

```python
from dataclasses import dataclass

@dataclass
class Sample:                   # C の struct に近い書き方
    id: int
    temp: float = 0.0

    def is_hot(self):           # メソッド。第1引数 self が自分自身
        return self.temp > 30.0

sm = Sample(id=1, temp=31.5)
sm.temp                         # 31.5
sm.is_hot()                     # True
```

## 例外とファイル

```python
try:
    int("abc")
except ValueError as e:         # エラーは戻り値でなく例外で伝わる
    print("error:", e)

with open("note.txt", encoding="utf-8") as f:   # ブロックを抜けると自動で閉じる
    for line in f:
        print(line.rstrip())
```

## モジュール

```python
import math                     # C の #include にあたる
math.sqrt(16)                   # 4.0
from math import sqrt           # 名前を直接取り込む書き方
```

## はまりどころ

- インデントが文法です。ずれると別のブロックになるか、`IndentationError` で止まります。
- `b = a` はリストをコピーしません。`a` と `b` が同じリストを指し、`b.append(50)` で `a` も変わります。C のポインタを代入したのと同じ動きです。コピーは `a.copy()`。
- `range(1, 4)` は 1, 2, 3。終わりの値は含みません。スライスも同じ。
- `/` は整数どうしでも小数を返します。C の `/` と同じ結果がほしいときは `//`。ただし負の数は `-7 // 2` が `-4` になり、0 方向に切り捨てる C とは違います。
- 引数の既定値にリストを書かないでください。`def f(x, items=[])` の `[]` は関数定義時に1個だけ作られ、呼ぶたびに使い回されます。
- 値の比較は `==`。`is` は「同じ実体か」を調べる演算子で、`None` との比較（`x is None`）に使います。

## 確認環境

コード例は Python 3.10.12 で実行し、コメントに書いた値と一致することを確かめました。
