# C言語習熟ロードマップ & 演習環境 設計書

- 作成日: 2026-09-18
- 対象者: C言語の基礎学習を一通り終えた学習者
- 目的: 組込み（Raspberry Pi）で通用するC言語の習熟
- 学習時間: 1日3〜4時間 × 5日（合計15〜20時間）※当初は週3〜5時間 × 約6か月。第6章の改訂を参照
- 実機: Raspberry Pi（Raspberry Pi OS 64bit を想定。モデルは Phase 3 開始時に確認）
- 開発PC: WSL2 Ubuntu 22.04 / gcc 11.4 / make / cmake（gdb・valgrind は未導入）

## 1. 方針

ハイブリッド型: フェーズごとの小課題（kata）で部品と知識を積み上げ、
Phase 6 の卒業制作でそれらを Raspberry Pi 上に統合する。

- 課題は「仕様（README）＋テスト提供済み → 学習者が実装 → `make test` 通過 → Claude がレビュー」の形式。
- 可能な限り PC（WSL）上でテスト可能な設計にし、実機依存部は HAL で分離する。
- 演習環境は Phase 0〜1 の課題のみ先行作成し、以降は進捗に合わせて追加する（YAGNI）。

## 2. ロードマップ

| Phase | 期間 | テーマ | 到達目標 | 主な演習 |
|---|---|---|---|---|
| 0 | 1週 | 開発環境 | 厳格な警告・サニタイザ・デバッガ・単体テストを日常的に使える | 仕込まれたバグを警告/ASan/UBSan/gdb で特定する |
| 1 | 5週 | 言語の深部 | 未定義動作・整数昇格・ポインタ・ビット演算・メモリレイアウトを説明できる | ビット操作マクロ、エンディアン変換＋バイト列シリアライズ、関数ポインタによるコマンドテーブル |
| 2 | 5週 | 組込み向け設計 | 動的確保なしで、テスト可能なモジュールを設計できる | リングバッファ、状態機械、固定小数点演算、HAL 越しの LED/ボタンドライバ（TDD＋テストダブル） |
| 3 | 4週 | Raspberry Pi 実機 | 実機で GPIO/I2C を C から操作できる | Lチカ（libgpiod と mmap レジスタ操作の両方）、I2C センサ読み取り |
| 4 | 5週 | 並行・時間 | スレッド・シグナル・イベント待ちを安全に扱える | GPIO エッジイベント待ち＋スレッド間通知、周期処理とジッタ計測 |
| 5 | 3週 | 品質 | 規約と静的解析でコード品質を評価・改善できる | MISRA C / CERT C 観点のセルフレビュー、cppcheck・clang-tidy、gcov カバレッジ |
| 6 | 3週〜 | 卒業制作 | 部品を統合した実機アプリを完成させる | センサロガー: I2C センサ → リングバッファ → 状態機械 → ファイル/UART 出力（PC 上でもテスト可能） |

発展（任意）: Pico 等でのベアメタル、FreeRTOS、Linux カーネルモジュール。

副読本（任意）: 『Effective C』『Test-Driven Development for Embedded C』『Making Embedded Systems』『詳説 Cポインタ』。

## 3. 演習環境の構成

```
c_master/
  ROADMAP.md              # 第2章の内容＋各フェーズの到達目標チェックリスト
  README.md               # 環境構築・ビルド・テスト・レビュー依頼の方法
  common/
    common.mk             # 共通ビルドルール
    unity/                # Unity テストフレームワーク（ThrowTheSwitch/Unity の src を同梱）
  phase0_env/
    ex01_<name>/
      README.md           # 課題文・仕様・ヒント・到達確認
      src/                # 学習者が実装するファイル（関数スタブ入り）
      test/               # 提供テスト（Unity）
      Makefile            # include ../../common/common.mk
  phase1_lang/
    ex01_<name>/ ...
```

### 3.1 共通ビルドルール（common.mk）

- フラグ: `-std=c11 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -g -O0`（警告はエラー扱い: `-Werror`）
- ターゲット:
  - `make test` : テストをビルドして実行
  - `make asan` : `-fsanitize=address,undefined` 付きでテスト実行
  - `make clean`
- 各課題の Makefile は `SRCS` などの変数を定義して common.mk を include するだけにする。

### 3.2 課題の形式

- README: 目的 / 仕様（関数シグネチャ・振る舞い・エラー時の挙動）/ ヒント / 「説明できるようになるべきこと」
- src: シグネチャのみのスタブ。初期状態では `make test` が失敗する（テストのビルドは通る）。
- test: 正常系・境界値・異常系を含むテスト。
- 模範解答はリポジトリに置かない（レビュー時に議論する）。

### 3.3 初期作成する課題（Phase 0〜1）

Phase 0:
- ex01_toolchain: gdb/valgrind 導入確認と、`make test` / `make asan` の動作確認
- ex02_bug_hunt: バグ（範囲外アクセス・未初期化・符号付きオーバーフロー）入りコードを、警告・ASan/UBSan・gdb で特定して修正する

Phase 1:
- ex01_bitops: ビットの set/clear/toggle/test、フィールドの抽出/挿入（レジスタ操作を想定）
- ex02_endian: エンディアン判定、`uint16/32` とバイト列の相互変換（BE/LE）、構造体のシリアライズ
- ex03_int_promotion: 整数昇格・算術変換の予想問題（予想→実行で検証）と安全な加算・乗算（オーバーフロー検出）
- ex04_cmd_table: 関数ポインタのテーブルで、文字列コマンドを dispatch する

Phase 2 以降の課題は、Phase 1 完了時に同じ形式で追加する。

## 4. 検証方法

- 各課題で、スタブのまま `make test` を実行し「ビルドは成功し、テストは失敗する」ことを確認する。
- 作成者（Claude）がスクラッチで参照実装を書き、`make test` と `make asan` が通ることを確認してから削除する（テストの正しさを担保するため）。

## 5. 範囲外

- Phase 2 以降の課題の実体（段階的に追加）
- CI 環境
- 実機向けのクロスコンパイル環境（Phase 3 で構築）

## 6. 改訂（2026-09-18）: 1週間版に縮小

学習者の想定期間が1週間（1日3〜4時間 × 5日）だったため、規模を縮小した。

- ROADMAP.md を日ごとの1週間プランに置き換える。1〜4日目は既存の Phase 0〜1 の6課題を使い、5日目に実機演習を1つ行う。
- 旧 Phase 2〜6 は、ROADMAP.md の「この先の学習（任意）」に数行で残す。追加の課題は作らない。
- 実機演習 `phase2_pi/ex01_blink` を追加する。libgpiod v2 で GPIO17 の LED を点滅させる。`blink()` だけを学習者が実装し、初期化と後片付けは提供する。
  - 実機がないため単体テストはなく、`common.mk` も使わない（Pi 上で `make run`）。
  - 作成時の検証は、libgpiod 2.1.3 のヘッダに対する `-fsyntax-only` のコンパイルチェックのみとする。
