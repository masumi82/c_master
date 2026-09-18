# c_master — C言語 習熟演習

組込み（Raspberry Pi）を目標に、C言語を「使える」レベルまで鍛えるための1週間の演習環境です。
日ごとの予定は [ROADMAP.md](ROADMAP.md) を参照してください。

## 必要なもの
```bash
sudo apt install -y build-essential gdb valgrind
```

## 構成
```
common/          共通のビルド設定（common.mk）とテストフレームワーク Unity
phase0_env/      1日目の課題（開発環境）
phase1_lang/     2〜4日目の課題（言語の深部）
phase2_pi/       5日目の課題（Raspberry Pi 実機。Pi 上でビルドする）
docs/            設計書・計画書
```
各課題ディレクトリは `README.md`（課題文）、`src/`（あなたが実装する）、`test/`（提供テスト）、`Makefile` からなります。
`phase2_pi/` は実機を使うためテストがなく、進め方は課題の README に従います。

## 課題の進め方
1. 課題の `README.md` を読む
2. `make test` で、テストが失敗することを確認する
3. `src/` を実装する（`test/` は変更しない）
4. 以下がすべて通ったら完了
   ```bash
   make test
   make asan
   ```
   課題の README に「最終確認」がある場合は、そちらのコマンドをすべて通してください。
5. Claude Code に「phase1_lang/ex01_bitops をレビューして」のように依頼する。README の「説明できるようになるべきこと」についても質問されます
6. `git commit` して、ROADMAP.md のチェックボックスを埋める

## make ターゲット
| コマンド | 内容 |
|---|---|
| `make test` | テストをビルドして実行 |
| `make test OPT=-O2` | 最適化を有効にしてビルド（追加の警告が出ることがある） |
| `make asan` | AddressSanitizer + UndefinedBehaviorSanitizer 付きで実行 |
| `make valgrind` | valgrind memcheck 上で実行 |
| `make clean` | ビルド成果物を削除 |

## ライセンス
`common/unity/` は [ThrowTheSwitch/Unity](https://github.com/ThrowTheSwitch/Unity) v2.6.1（MIT License）です。
