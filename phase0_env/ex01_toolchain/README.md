# Phase 0 / ex01_toolchain — 開発環境を整える

## 目的
以降の全課題で使う道具（厳格な警告・単体テスト・サニタイザ・valgrind・gdb）を一通り動かす。

## 手順
1. ツールを入れる
   ```bash
   sudo apt update && sudo apt install -y gdb valgrind
   gdb --version && valgrind --version
   ```
2. 失敗を確認する: `make test` → テストが失敗する（まだ実装していないため）
3. `src/clamp.c` の `clamp_i32` を実装する（仕様は `src/clamp.h`）
4. 次の3つがすべて通ることを確認する
   ```bash
   make test
   make asan
   make valgrind
   ```
5. gdb を触る
   ```bash
   gdb build/runner-O0
   (gdb) break clamp_i32
   (gdb) run
   (gdb) info args        # 引数の値
   (gdb) next             # 1行進む
   (gdb) print v          # 変数の値
   (gdb) finish           # 関数から抜けて戻り値を表示
   (gdb) bt               # 呼び出し履歴
   (gdb) continue
   (gdb) quit
   ```

## 説明できるようになるべきこと
- `-Wall -Wextra -Wconversion -Werror` はそれぞれ何をするか
- `make asan` と `make valgrind` は何を検出するか、どう違うか
- テストが通ることと、コードが正しいことは同じか
