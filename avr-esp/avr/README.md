# AVR

## 目次

- AVRとは
- AVRな理由
- AVRの新旧
- ライター
- 今回の基板について
- ツール
- 環境構築
- Lチカ
  - delay
  - Timer
- PWM
  - 滑らかにLチカ
  - スピーカ
- 電子オルゴール

## 8bitマイコン

- 8bitマイコン二大巨頭
  - AVR (Atmel社)
  - PIC (Microchip社)
- 2016年にMicrochip社がAtmel社を買収
  - AVRは引き続き開発されている
  - 新しい型番のAVRも発表されている

## AVRとは

- RISCベース
  - AさんとVさんのRISCマイコンが語源
- いくつか種類がある
  - AT90S
    - 一番古いシリーズ
    - もう使わない
  - ATmtega
    - 一番メジャーなシリーズ
    - 例えば
      - ATmega328p
        - Arduino UNO
      - ATmega1284p
        - 古いEnder3
      - ATmega4809
        - 比較的最近出た
        - Arduino Nano Every
  - ATtiny
    - ATmegaより物理的に小さくピンが少ないシリーズ
    - 例えば
      - ATtiny10
        - 米粒サイズ
      - ATtiny13A
      - ATtiny2313
        - USBasp書込機
      - ATtiny44
      - ATtiny45, 85
        - DigiSpark
      - ATtiny202, 402
        - 最近出た

  - 他にも
    - ATxmega
    - AVR Dx
    - ...

## AVRな理由

### 高速な処理

- AVRは基本的に1命令1クロック
  - さらにRISCベースで省命令
- PICは基本的に1命令4クロック
- 単純にAVRは4倍速

### オープンで強力なコンパイラ

- AVRはavr-gcc
  - 無料でフル機能
  - オープンソース
- PICはxc8
  - 無料版は最適化が効かない
    - 有料版は$1000
  - Microchip社の製品
  - 弱体化したavr-gccも同梱されているという噂 :kowai_hanasi:

## AVRの新旧

- Microchipに買収されてから変わった

  ||ATtiny45 (旧)|Attiny402 (新)|
  |---|---|---|
  |フラッシュ/kB|4|4|
  |RAM/B|256|256|
  |EEPROM/B|256|128|
  |内蔵クロック/MHz|8 (PLL: 16)|20|
  |動作電圧/V|2.7~5.5|1.8~5.5|
  |書込方式|ISP|UPDI|
  |乗算器||○|
  |タイマー (PWM)|8bit \*2|16bit \*2 = 8bit \*4|
  |ADC|10bit 15kSPS \*4|10bit 115kSPS \*6|
  |I²C|○|○|
  |SPI|○|○|
  |UART||○|
  |DigiKeyプライス|¥212|¥79|

  - 内蔵クロックだけで20MHz出せるようになった
  - コアが変わった
    - 旧: tiny専用の貧弱なコア
      - 乗算は加算で対応していた
    - 新: xmega系の強力なコア
      - 専用の乗算器がある
  - メモリ空間の構造が変わった
    - 旧: RAMとROMが完全に分離
      - constでもPROGMEMしないとRAMを消費
      - ROMへのアクセスは専用の命令
    - 新
      - PROGMEM無しでもRAMを消費しなくなった
      - RAMのアクセスと同じ命令でROMにアクセスできる
        - アクセスが早くなったわけではない
  - レジスタの名前がいろいろ変わった
  - 書き込み方式が変わった
    - 場合によっては書き込み機を買い替える必要がある
    - 旧: ISP
      - 必要なピン 5本
        - MISO
        - MOSI
        - SCK
        - RST
        - GND
      - 中身はSPI通信
    - 新: UPDI
      - 必要なピン 2本
        - UPDI
        - GND
      - 作業が楽になる

## ライター

- 書き込み機
- マイコンにプログラムを書き込むには専用のライターが必要
  - Arduinoはプログラムを書くプログラムが最初から書いてあるからシリアル変換で済む
- AVR
  - ISP系
    - AVRISP
      - Atmel純正
    - USBasp
      - コミュニティが開発
      - ドライバが必要
      - 製作にライターが必要
      - 亜種
        - USBtinyISP
    - HIDasp
      - コミュニティが開発
    - Arduino as ISP
      - Arduinoに入れるとISPとして使えるプログラム
  - UPDI系
    - Atmel-IcE
      - Atmel純正
      - ISPも可
    - serialUPDI
      - コミュニティが開発
      - 回路が単純
- PIC
  - 前提としてPICは型番ごとに書き込み方法が違う
  - PICKit
    - Microchip純正
  - PickerFT
    - 個人開発
    - 対応機種が豊富
    - 現在も開発されている
    - AVRも可
    - 専用ソフトを使う
  - JDMライタ
    - コミュニティが開発
    - 回路が単純
    - 専用ソフトを使う
    - 亜種
      - RCDライタ

## 今回の基板について

- ATTiny202
  - フラッシュ: ***2kB***
  - RAM: 128B
- CH340K
  - SerialUPDI
- LED@PA3
- スイッチ@PA7

## ツール

### avr-gcc

- AVR用のGNU C言語コンパイラ
- 中身はAVR Libcとして開発
  - brewのlibcは古くてt202に対応していない?
- <https://www.nongnu.org/avr-libc/user-manual/>
- オプションで指定
  - AVRの型番

#### avr-binutil

- `avr-size`や`avr-objcopy`等のこまごましたコマンド群
- `avr-objcopy`は.hexへの変換によく使う

### avrdude

- AVRにプログラムを書き込むツール
- 様々なAVRとライターに対応
- オプションで指定
  - AVRの型番
  - 使うライター
  - ライターのあるポート
    - ポートの調べ方
      - Windows: デバイスマネージャ(Win&X+M)>COMポート
      - Mac: `ls -l /dev/tty.usb*`

### make

- コンパイル作業を自動化するツール
- AVR関係なしによく使われる

## 環境構築

### windows

- scoopのインストール <https://scoop.sh/>
- ツールのインストール

  ```txt
  scoop install avr-gcc
  ```

  - (scoopではavrdudeとmakeはavr-gccに含まれている)

### mac

- homebrewのインストール <https://brew.sh/index_ja>
  - M1 Macだとパスを通す必要があるらしい
- ツールのインストール

  ```txt
  brew install avrdude
  brew tap osx-cross/avr
  brew install avr-gcc@12 avr-binutils
  ```

  - (makeはhomebrewを入れる過程で入るらしい)
