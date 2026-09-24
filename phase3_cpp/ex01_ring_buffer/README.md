# Phase 3 / ex01_ring_buffer — C で書いたものを C++ で書き直す（任意・半日）

## 読み物: C と C++ の違いを、この課題で使う範囲だけ
C++ は C の上に大きな言語を載せたものですが、組込みで使う範囲はそれほど広くありません。この課題で実際に使うのは次の6点です（`nullptr` は参考）。

| C での書き方 | C++ での書き方 | 何が変わるか |
|---|---|---|
| `void ring_init(ring_t *r, ...)` を呼び忘れると未初期化 | コンストラクタ `RingBuffer() = default;` とメンバ初期化子 `head_ = 0` | 「初期化していない状態」が存在しなくなる（RAII の第一歩） |
| `void *buf` + `elem_size` で任意型を扱う | `template <typename T, std::size_t N>` | 型が違えばコンパイルエラー。`memcpy` も要らない |
| `int *out` に書き戻す | `T &out`（参照） | NULL チェック不要。呼び出し側は `pop(v)` と書くだけ |
| 生配列 `T buf[N]` | `std::array<T, N>` | 中身は同じ固定長配列。`.size()` が付き、関数へ渡してもポインタに退化しない |
| `#define CAP 4` | `static constexpr std::size_t capacity()` / `static_assert(N > 0, ...)` | 定数と検査をコンパイル時に型付きで行える |
| 「この関数は状態を変えない」はコメントで示す | `size() const` のように `const` メンバ関数 | 変えないはずの関数が変えたら、コンパイルエラー |
| `NULL` | `nullptr` | 整数 `0` と区別される |

**組込みで避けるもの（この課題でも禁止）**: `new`/`malloc`（ヒープ断片化と、確保失敗時の挙動が読めないため。固定長で足りるなら使わない）、`std::vector`（内部でヒープを使う）、例外と RTTI（コードサイズと実行時間が読めない。多くの組込みビルドは `-fno-exceptions -fno-rtti`）、`<iostream>`（巨大。`printf` か専用ロガーを使う）。

**C と同じ**: ビット演算、整数昇格、エンディアン、未定義動作の考え方は Phase 1 のまま C++ でも通用します。`std::uint16_t` など `<cstdint>` の型もそのままです。

## 目的
Phase 1 までに C で書いた考え方（固定長バッファ、`bool` で失敗を返す、`malloc` を使わない）を保ったまま、C++ の型システムに「初期化漏れ」「型の取り違え」「状態を変える関数の取り違え」を検出させます。
仕様は `src/ring_buffer.hpp` の各メソッドのコメント。

## 手順
1. `make test` で失敗を確認（`g++` は `build-essential` に入っています）
2. `src/ring_buffer.hpp` の `push / pop / size / empty / full / clear` を実装
3. `make test && make asan`

## ルール
- `new`、`malloc`、`std::vector`、例外（`throw`）、`<iostream>` を使わない
- `buf_` は `std::array` のまま。追加してよいメンバは整数のインデックスやカウンタだけ
- `test/` は変更しない

## テストについて
- `test_const_access` は `const RingBuffer<int, 4> &` 経由で `size()` などを呼びます。`const` を付け忘れると「テスト失敗」ではなく**コンパイルエラー**になります。これが C++ の `const` の働きです
- `test_struct_elements` は `Sample` 構造体を要素にします。`T` を `int` 決め打ちで実装すると通りません
- `test_wraparound` は書き込み位置が配列の末尾から先頭へ戻ることを確認します。`%` か `if` で戻してください

## 発展課題（任意）
- `static_assert(N > 0, ...)` の `N > 0` を `N > 100` に変えて `make test` し、エラーがどの行を指すか見る（戻すのを忘れずに）
- `RingBuffer<int, 4>` を `sizeof` で測り、C 版 `struct { int buf[4]; size_t head, count; }` と比べる（テンプレートは実行時コストを足さない）
- `Makefile` の `include` 行の**後ろ**に `CXXFLAGS += -fno-exceptions -fno-rtti` を足して、テストが同じく通ることを確認する（`include` より前に書くと `common_cpp.mk` の `:=` で上書きされて効きません）

## 説明できるようになるべきこと
- `T &out` と `T *out` の違い。参照が使えない場面（「無い」を表したいとき）はどうするか
- `size() const` の `const` は何を約束しているか。`const` を外すとどのテストがなぜ通らなくなるか
- `template <typename T, std::size_t N>` は「コンパイル時にコードを生成する」仕組みであること。`RingBuffer<int, 4>` と `RingBuffer<int, 8>` は別の型であること
- コンストラクタとメンバ初期化子（`= 0`、`{}`）で、C の `init()` 関数と比べて何が保証されるか
- なぜ組込みで `std::vector` と例外を避けるのか
