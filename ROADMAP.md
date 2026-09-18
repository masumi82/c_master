# C言語 習熟ロードマップ（組込み / Raspberry Pi）

- 想定: 週3〜5時間 × 約6か月（合計約100時間）
- 進め方: 各フェーズの小課題で部品と知識を積み上げ、Phase 6 の卒業制作で Raspberry Pi 上に統合する
- 各課題は `make test && make asan` が通ったら、Claude に「phaseX/exY をレビューして」と依頼する

## 全体像

| Phase | 目安 | テーマ | 到達目標 |
|---|---|---|---|
| 0 | 1週 | 開発環境 | 厳格な警告・サニタイザ・デバッガ・単体テストを日常的に使える |
| 1 | 5週 | 言語の深部 | 未定義動作・整数昇格・ポインタ・ビット演算・メモリレイアウトを説明できる |
| 2 | 5週 | 組込み向け設計 | 動的確保なしで、テスト可能なモジュールを設計できる |
| 3 | 4週 | Raspberry Pi 実機 | 実機で GPIO / I2C を C から操作できる |
| 4 | 5週 | 並行・時間 | スレッド・シグナル・イベント待ちを安全に扱える |
| 5 | 3週 | 品質 | 規約と静的解析でコード品質を評価・改善できる |
| 6 | 3週〜 | 卒業制作 | 部品を統合した実機アプリを完成させる |

## Phase 0: 開発環境（1週）
- [ ] ex01_toolchain — gdb / valgrind の導入、`make test` / `make asan` / `make valgrind`、gdb の基本操作
- [ ] ex02_bug_hunt — 警告・ASan・UBSan で3つのバグを特定して直す

## Phase 1: 言語の深部（5週）
- [ ] ex01_bitops — ビット操作とフィールド抽出/挿入（レジスタ操作の基礎）
- [ ] ex02_endian — エンディアン変換とバイト列シリアライズ、パディング
- [ ] ex03_int_promotion — 整数昇格クイズ、オーバーフローしない算術
- [ ] ex04_cmd_table — 関数ポインタのテーブルによるコマンド振り分け
- 並行して読む: ポインタと配列の違い、`volatile` / `const`、記憶域クラス（`static` / `extern`）、未定義動作

## Phase 2: 組込み向け設計（5週）※課題は Phase 1 完了時に追加
- 静的メモリ設計（`malloc` を使わない）、リングバッファ
- 状態機械（switch 版 / テーブル版）
- 固定小数点演算
- 不透明型（opaque pointer）によるモジュール分割
- HAL 抽象化と TDD、テストダブル（spy / fake / mock）で LED / ボタンドライバを PC 上で開発

## Phase 3: Raspberry Pi 実機（4週）
- Pi 上でのネイティブビルド → PC からのクロスコンパイル
- GPIO: libgpiod と、`/dev/gpiomem` の mmap によるレジスタ直接操作の両方
- I2C（`/dev/i2c-*`）でセンサを読む、SPI / UART の概要
- Phase 2 の HAL の実機実装を作る

## Phase 4: 並行・時間（5週）
- シグナル、pthread、mutex / 条件変数、アトミック
- GPIO エッジイベント待ち（割り込みに相当）とスレッド間通知
- 周期処理、`clock_nanosleep`、ジッタ計測、リアルタイム性の限界

## Phase 5: 品質（3週）
- MISRA C / CERT C の考え方（主要ルールの理由を理解する）
- cppcheck / clang-tidy による静的解析
- gcov によるカバレッジ計測
- これまでのコードの改善

## Phase 6: 卒業制作（3週〜）
センサロガー: I2C センサ → リングバッファ → 状態機械 → ファイル / UART 出力。
HAL を差し替えて PC 上でもテストできる構成にする。

## その先（任意）
- Raspberry Pi Pico などでのベアメタル開発（スタートアップコード、リンカスクリプト、割り込みベクタ）
- FreeRTOS
- Linux カーネルモジュール / デバイスドライバ

## 副読本（任意）
- 『Effective C』 Robert C. Seacord
- 『Test-Driven Development for Embedded C』 James W. Grenning
- 『Making Embedded Systems』 Elecia White
- 『詳説 Cポインタ』 Richard Reese
