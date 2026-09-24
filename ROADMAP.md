# C言語 習熟ロードマップ（1週間 / 組込み・Raspberry Pi）

- 想定: 1日3〜4時間 × 5日（合計15〜20時間）
- 進め方: 4日目までは PC 上で、言語の「危ないところ」を課題で確かめる。5日目に Raspberry Pi 実機で LED を点滅させる
- 各課題は `make test && make asan` が通ったら（課題の README に「最終確認」があればそちらも）、Claude に「phaseX/exY をレビューして」と依頼する
- 発展課題はすべて任意。時間が余ったときだけ取り組む

## 1週間の予定

| 日 | 課題 | 目安 | 到達目標 |
|---|---|---|---|
| 1日目 | phase0_env/ex01_toolchain、ex02_bug_hunt | 3h | 警告・サニタイザ・デバッガでバグを捕まえられる |
| 2日目 | phase1_lang/ex01_bitops ＋ 読み物 | 3h | レジスタ操作のビット演算を未定義動作なしで書ける |
| 3日目 | phase1_lang/ex02_endian | 3〜4h | 整数・構造体とバイト列を、エンディアンやパディングに依存せず変換できる |
| 4日目 | phase1_lang/ex03_int_promotion、ex04_cmd_table | 4h | 整数の型変換を説明でき、関数ポインタで処理を振り分けられる |
| 5日目 | phase2_pi/ex01_blink、1週間のふり返り | 3h | 実機の GPIO を C から操作できる |

## チェックリスト
- [ ] 1日目: ex01_toolchain — gdb / valgrind の導入、`make test` / `make asan` / `make valgrind`、gdb の基本操作
- [ ] 1日目: ex02_bug_hunt — 警告・ASan・UBSan で3つのバグを特定して直す
- [ ] 2日目: ex01_bitops — ビット操作とフィールド抽出/挿入
- [ ] 2日目: 読み物 — `volatile` と `const` の意味、組込みで `volatile` が必要になる場面
- [ ] 3日目: ex02_endian — エンディアン変換とバイト列シリアライズ、パディング
- [ ] 4日目: ex03_int_promotion — 整数昇格クイズ、オーバーフローしない算術
- [ ] 4日目: ex04_cmd_table — 関数ポインタのテーブルによるコマンド振り分け
- [ ] 5日目: ex01_blink — Raspberry Pi で Lチカ
- [ ] 5日目: ふり返り — 各 README の「説明できるようになるべきこと」を Claude に説明してみる

## 補足（任意・半日）: C++ を C との違いで押さえる
C の1週間を終えたあと、または待ち時間に。C++ を本格的に学ぶのではなく、「C で書いたものを C++ の型システムに守らせる」感覚をつかむための1課題です。
- [ ] phase3_cpp/ex01_ring_buffer — README の読み物（30分）→ テンプレートのリングバッファ実装（2〜3h）。RAII、参照、`const` メンバ関数、`std::array`、`static_assert`

## この先の学習（任意）
1週間を終えて続けたくなったら、次の順がおすすめです。
1. 組込み向けの設計: リングバッファ、状態機械、HAL 抽象化と TDD（PC 上でテストできるドライバ）
2. 実機: I2C センサの読み取り、GPIO のイベント待ち（割り込みに相当）
3. 品質: cppcheck / clang-tidy、MISRA C / CERT C の考え方
4. さらに先: Raspberry Pi Pico でのベアメタル開発、FreeRTOS

## 副読本（任意）
- 『Effective C』 Robert C. Seacord
- 『Making Embedded Systems』 Elecia White
