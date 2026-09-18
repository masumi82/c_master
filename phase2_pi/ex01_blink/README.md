# 5日目 / ex01_blink — Raspberry Pi で Lチカ

## 目的
PC 上で学んだ C を、実機の GPIO に対して使ってみます。libgpiod（Linux 標準の GPIO ライブラリ）で LED を点滅させます。

> この課題は実機がないと確認できません。作成時に確認したのは、libgpiod v2 のヘッダに対するコンパイルチェックだけです。
> 当日うまく動かないときは、エラーメッセージをそのまま Claude に見せてください。

## 準備するもの
- Raspberry Pi（Raspberry Pi OS 64bit）
- LED 1個、抵抗 1個（220〜330Ω）、ジャンパワイヤ

## 配線
| Pi のピン | 接続先 |
|---|---|
| 11番ピン（GPIO17） | 抵抗 → LED のアノード（足が長い方） |
| 9番ピン（GND） | LED のカソード（足が短い方） |

## 手順
1. Pi 上にこのリポジトリを持ってくる（`git clone` または `scp`）
2. ライブラリを入れる
   ```bash
   sudo apt install -y build-essential libgpiod-dev gpiod
   apt-cache policy libgpiod-dev   # バージョンが 2.x であることを確認
   ```
   1.x だった場合は、API が違うためこのコードはビルドできません。Claude に相談してください。
3. GPIO17 がどのチップにあるかを確認する
   ```bash
   gpioinfo | grep -n "GPIO17"
   ```
   `gpiochip0` 以外なら、`src/blink.c` の `CHIP_PATH` を書き換える
4. 配線だけ先に確認する（C を書く前に、LED が点くことを確かめる）
   ```bash
   gpioset GPIO17=1   # 点灯したまま待機する（Ctrl+C で終了）
   ```
5. `src/blink.c` の `blink()` を実装する（仕様は関数の上のコメント）
6. ビルドして実行する
   ```bash
   make run
   ```

## ヒント
- 初期化と後片付けは `main()` に書いてあります。`goto out` でまとめて解放する書き方は、Linux カーネルなど C のコードでよく使われます
- 失敗したときに LED を点けたまま終わらないようにするには、どうすればよいでしょうか

## 発展課題（任意）
- ex01_bitops の `bit_toggle` を使って、点灯状態を1つの変数で管理する
- `Ctrl+C` で止めたときにも LED を消してから終了する（`sigaction` を調べる）

## 説明できるようになるべきこと
- ユーザー空間のプログラムが、どういう経路で GPIO を操作しているか（`/dev/gpiochipN` とカーネルの関係）
- `main()` の後片付けが、確保と逆の順番になっている理由
