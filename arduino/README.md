# Arduino

## 目次

- Arduino
- Lチカ
- Serial.print
- ボタン
- 仕組みと自作
- Arduinoって結局何?

## Arduino

- 例えば...
  - 電源と電球を繋ぐ
    - 光る
  - 間にスイッチを入れる
    - スイッチを入れると光る
- 電動のスイッチ?
  - リレー
    - 電磁石+金属片
  - トランジスタ
    - 小電流で大電流が制御できる
- 自動で電球を点けたい
  - 電球を点ける電動のスイッチ
  - 電動のスイッチを動かすセンサ
- センサが反応した後もしばらくつけておきたい
  - コンデンサを使ってゆっくりな発振回路を作って......
- いちいちそんな回路作るのめんどくさいしでかい
- そんなあなたにArduino
  - 0Vと5Vを出せる端子がいっぱいある
  - 0~5Vの電圧を計れる端子もある
  - タイマーで時間を計れる
  - プログラムを書いてこれらを自由に制御
  - [秋月で売ってる](https://akizukidenshi.com/catalog/g/gM-07385/)
  - Amazonとかどこでも売ってる
- どうなる?
  - 人感センサーが反応したら電球付けて一定時間たったら消す
    - 人感センサーの信号を受け取るArduino
    - 一定時間待つArduino
    - 電球を点ける電動のスイッチを動かすArduino
- めでたしめでたし......

## Lチカ

- ArduinoとLEDを繋げて点滅させる
  - マイコン買ったら最初にやる儀式
  - さっきトランジスタに流していた電流でLEDは十分点灯する
  - LEDはV=IRではないので電流が一気に流れる
    - これを防ぐため直列に抵抗を繋ぐ
    - 電流制限抵抗
    - 330ohmが良く使われるが人間の視覚特性が対数なので10kohm位挟んでも普通に光る
- Arduinoについて
  - 使い方
    - PCにArduinoIDEをインストール
    - プログラムを書く
    - USBで繋ぐ
    - 「ボードを選択」する
      - 一緒にポートも選択
    - 「→」を押して書き込む
  - 各ピンには番号が振られている
    - プログラムからの指定にはこの番号を使う
    - ピンによって一部の機能が異なる
    - Arduinoの定番であるArduino UNOには基板にLEDが載っている
      - 13番ピンとGNDの間に繋がっている
  - 最初にピンの動作モードを定義する
    - 内部の回路が切り替わる
  - 基本的に出力も入力もONかOFFしかできない
    - ON==HIGH==5v
    - OFF==LOW==0v

```txt
追加の回路無し
```

```cpp
// これはコメント 動作には影響しない
// 式の最後に;付けるのを忘れずに
// void 名前(){} ←関数の宣言 名前();で波括弧の中が順番に動く
// setup, loop 関数はArduino側で定義されていて勝手に動く
// 色んな名前の関数があらかじめ定義されている

void setup(){// 電源入れたときに一度だけ動く
  pinMode(13,OUTPUT);// 13番ピンを出力ピンとして設定
}
void loop(){// setupの後無限に繰り返し動く
  digitalWrite(13,HIGH);// 13番ピンの出力をHIGH=5vに設定
  delay(500);// 500ミリ秒=0.5秒待つ
  digitalWrite(13,LOW);// 13番ピンの出力をLOW=0vに設定
  delay(500);
  // ここまで来たらまたloopをはじめから実行
}
```

## Serial.print

- USBで繋がっている機器と簡単なデータをやり取りできる
- ArduinoIDEウィンドウ右上の🔍から見れる

```txt
追加の回路無し
```

```cpp
void setup(){
  Serial.begin();// 通信を開く
}
void loop(){// setupの後無限に繰り返し動く
  Serial.print(millis());// millis()で起動後ミリ病を取得 送信
  Serial.print("Hello World!\n");// 文字列を送信
  delay(1000);
}
```

## ボタン

- ボタンを押したらLEDが光るようにする
- ボタンが押されていることを検出
  - Arduinoはピンの電圧がHIGHかLOWかを取得できる
    - `pinMode(ピン番号,INPUT);` ピンの設定
    - `digitalRead(ピン番号)==HIGH` HIGHかLOWを返す関数
  - ボタンが押されたらLOWになる回路を用意
    - スイッチが押されていないときのIOの電位を定めるために抵抗が必要
    - 抵抗とスイッチを入れ替えてHIGHになる回路も作れるがメジャーじゃない
    - LOWになる回路であれば`pinMode(ピン番号,INPUT_PULLUP);`で抵抗を省略できるから

```txt
GNDとioの間にスイッチ
5VとIOの間に大きめの抵抗(1k~47k, 10kをよく使う)
---|
 5V|-~~~-\
 IO|-----|
GND|--__-/
---|
```

```cpp
void setup(){// 電源入れたときに一度だけ動く
  pinMode(13,OUTPUT);// 13番ピンを出力ピンとして設定
  pinMode(10,INPUT);// 10番ピンを入力ピンとして設定
}
void loop(){// setupの後無限に繰り返し動く
  if(digitalRead(10)==HIGH)digitalWrite(13,HIGH);// HIGHならHIGHに
  else digitalWrite(13,LOW);// そうでないならLOWに
}
```

## 仕組みと自作

- Arduinoは自作できる
- Arduinoの構成要素
  - マイコン==MCU
    - Arduino UNO(Rev3)はAVRマイコン(ATMega328p)
    - ALU(CPU), RAM, フラッシュ(ROM), ピンの駆動回路が一つになっている
    - Arduino UNOはマイコンが載っているボードなのでマイコンボード
    - ブートローダ
      - マイコンに最初から入っているプログラム
      - USB-シリアル変換回路から来たデータをフラッシュに保存するプログラム
      - 電源投入時(再起動=リセット含む)一回だけ動く
        - 投入後1秒以内に通信があればプログラムの書込モードに入る
        - 無ければ既に書き込まれているプログラムの実行に移る
      - ブートローダを使わない書込方法
        - 専用の書込器AVRISPを使う
        - AVRISPはArduinoで代用できる
      - ブートローダを書き込むには
        - AVRISPを使う
  - USB-シリアル変換回路
    - シリアル=RS232=UART
    - 双方向通信ができる
    - USBとマイコンの橋渡し
    - Serial.print()でも使っている
    - チップ
      - FTDI社 FT232RL
        - 定番 高信頼
      - WCH社 CH340G
        - 定番 安い
      - Arduinoでは別のMCUにソフトウェアで変換の機能を持たせている
    - GND,RXD,TXD,DTR#の4ピンを使う
      - GND, RXD, TXDは通信に必要
        - Recieve(Transmit) Dataの略
        - XはTRANSferの意?
        - TXD-RXD, RXD-TXDで結線
      - DTR#はブートローダを呼び出すためリセットが必要なのでその線
        - Data Terminal Readyの略
          - 端末準備完了がtrue==LOW
          - MCUのRESET#に小容量のコンデンサを挟んで繋げる
            - MCUのリセットは信号が入っている間再起動し続ける
            - コンデンサでパルスにする
        - #は論理反転の意味
          - よく省略している人いるので注意!!
  - 電源回路
    - DCジャックの電源をマイコンに適した電圧に変換
    - USBからのみ動かす場合は不要
    - ヒューズなどの保護回路もここ
  - その他
    - 水晶発振子
      - 同様の機能のものがMCUに内蔵されているがちゃんとしたやつを用意すると時間に対する精度が上がる
      - セラミックのやつもある
    - LED
      - 基板に一個でも載ってると便利
    - 他にも色々載ってる

```txt
|--------=========|
|USB              |
|  USB-Serial     |
|                 \
|電源回路  ===MCU===|
|DC                |
|--------===-======|
```

写真だよ

![回路図](https://www.arduino.cc/en/uploads/Main/Arduino_Uno_Rev3-schematic.pdf)

- 書き込み時の流れ
  - PCがシリアル変換にデータを送る要求をする(COMポートを開く)
  - DTR#がLOWになりRESET#にLOWのパルスが入る
  - 即座にプログラムのデータが送り込まれブートローダが書込モードに入る

- 自作するには?
  - USBシリアル変換を用意する
  - MCU(ATMega328p)を用意する
    - ブートローダーを書き込む
    - 書き込み済みのも売ってる
  - 繋げる
  - 買い物リスト
    - FT232RLモジュールキット https://akizukidenshi.com/catalog/g/gK-06693/
    - 0.1uFコンデンサ x2
    - 328p ブートローダ書き込み済み https://akizukidenshi.com/catalog/g/gI-12774/
    - 発振器 https://akizukidenshi.com/catalog/g/gP-09576/
- ちなみに
  - 自作したArduino=クローンを売るときはArduinoを名乗っては行けない
  - Arduinoは商標登録されている
  - UNOは名乗れるので大多数はこれ

配線図だよ

## Arduinoって結局何?

- Arduino UNOに乗っているのはAVRマイコン
- ESP32もRPI-PicoもArduinoとして開発できる
- ArduinoはArduinoIDEとそれを使って開発できるボードを含めた大きな概念かも?
