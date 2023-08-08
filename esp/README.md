# ESP

## 目次

- 今回の基板について
- ESPとは
  - 概要
  - リンク
  - 機能
  - 種類と比較
  - 注意事項
- NeoPixelとは
  - 概要
  - リンク
  - 互換品
  - 注意事項
- ESP32開発ボードの設計
  - C3版
  - 無印版
- PlatformIOとは
  - 概要
  - 導入
- Hello World!
  - LEDで
  - Serialで
  - HTTPで
- WebSocketコントローラ

## 目的

- ESPのモジュール品の使い方を知る
  - より安く
  - より小さく
- WebSocketの有用性

## 今回の基板について

- 部品
  - ESP32-C3-WROOM-02 モジュール
  - USBコネクタ
  - LM1117 3.3vレギュレータ
  - OSTW3535 NeoPixel互換
- 特徴
  - 部品点数が非常に少なく小型
    - 表面実装部品で統一
    - UEW(エナメル線)で配線
    - 極限まで部品を省略
      - USB-シリアル変換が載っていない
        - 多くの場合USBとマイコンでシリアル通信をするには専用の変換チップが必要
        - ESP32C3はUSB(USB CDC)を搭載しているので内部でエミュレートできる
      - NeoPixelの信号電圧レベル変換が載っていない
        - 本来はfetを使ったレベル変換回路が必要
        - 5v電源で動いているところに3.3vの信号を入れている
        - 動作保証外だが動く

## ESPとは

### 概要

- espressif社が開発しているIoTなマイコン
  - 5mm角くらいのチップ
  - モジュールとは
    - チップの周りにフラッシュメモリとかコンデンサとか水晶と一緒に小さな基板に載せたもの
    - 電源繋げれば動く状態の基板
  - 開発ボードとは
    - モジュールをコネクタやシリアル変換チップと一緒に基板に載せたもの
    - PCとすぐ繋げて開発できる基板
    - 普段我々が見ているのはこの姿

### リンク

- [ESP-IDFの公式ドキュメントのAPI Guides](https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/api-guides/)
  - ESP-IDF=公式の開発環境
  - チップの機能が分かる
  - 開発環境は違えど使うチップは同じなのでかなり参考になる
- [Arduino-ESP32の公式ドキュメント](https://docs.espressif.com/projects/arduino-esp32/en/latest/)
  - Arduino環境でのESPの叩き方が分かる

### 機能

- WiFi
- Bluetooth
- 性能が良い

### 種類と比較

- ESP8266
- ESP32
  - ESP32(無印)
  - ESP32-C3
  - ESP32-S3

||8266|無印|C3|S3|
|---|---|---|---|---|
|モジュール名称|ESP-WROOM-02D|ESP32-WROOM-32E|ESP32-C3-WROOM-02|ESP32-S3-WROOM-1|
|サイズmm|18x20x3|18x25.5x3.1|18×20×3.2|18x25.5x3.1|
|ピン間隔mm|1.5|1.27|1.5|1.27|
|コア数|1|2|1|2|
|CPUアーキテクチャ|L106 (?)|Xtensa LX6|RISC-V|Xtensa LX7|
|フラッシュ容量MB|2, 4|4, 8, 16|4|4, 8, 16|
|PWMチャネル数|8|8|6|8|
|USB CDC|||o|o|
|USB OTG||||o|
|WiFi|o|o|o|o|
|Bluetooth Classic||o|||
|Bluetooth Low Energy||o|o|o|
|Ethernet||o|||
|秋月プライス|¥360 (2MB)|¥480 (16MB)|¥310|¥530 (16MB)|

- C3はバランスが良くて小さくて安い
- 1.5mmピッチの変換基板はそうそう売ってない
- 8266とc3はピン互換ではない
- 無印とC3を用途に応じて使い分けるのがベスト
  - 一つでも当てはまれば無印行き
    - PWMが8ch欲しい
    - Bluetooth Classicが欲しい
    - Ethernetが欲しい
    - クソデカフラッシュ

### 注意事項

- 電源もIOも3.3V
  - Arduino UNOは5V
  - 互換性のない周辺機器の例
    - 4.8V以上ないと動かない
    - 3.6V以上あると壊れる
  - 逆にESPのIOに間違えて5Vとか加えるとESPが壊れる
  - データシートはよく読もう
  - 相対的に電圧の高い部品の扱いは慎重に
- Arduinoに比べると電力食い
  - WiFi機能を使うと顕著
  - 割とアチアチになる
  - 最低限常時300mAは流せた方がいい
- WiFi使用時は一部のADCが使えない
  - ADCはアナログ電圧を読み取る機能で用いる
  - ESPはADC1,ADC2の2つのADCを持っている
  - アナログ電圧が読めるピンはどちらかのADCと繋がっている
  - WiFi使用時に使えないのはADC2
    - 常に同じ値を返すようになる
  - ADC1は無印だと32~39、C3だと5以外
  - リファレンスを見るとADC2~mode is no longer supported due to hardware limitationになっている

## NeoPixelとは

### 概要

- フルカラーLED
  - RGB3色のLEDが一つずつ入っている
- 信号線一本で制御できる
  - LEDと一緒にICが入っている
  - 複数個のNeoPixelも1本の信号線で扱える
    - 数珠繋ぎにしたNeoPixelにシリアル信号をまとめて流せる
- 5mm角4端子のチップ形状であることが多い
- 最近のESPの開発ボードには大体載ってる

### リンク

- ht-deko氏: <https://ht-deko.com/arduino/neopixel.html>

### 互換品

- WorldSemi社が最初(多分)に売り出した
  - WS2812B
    - 本家
  - WS2813B
    - 数珠つなぎの途切れに強くなった改良版
- 他社も互換品を売り出している
  - OptoSupply
    - LED大手
  - DONGGUAN OPSCO OPTOELECTRONICS CO., LTD
    - 秋月で売ってる
- 基本的にどれ選んでも同じ
- 選定する際の注意事項
  - 動作電圧が12Vのものがある
  - 稀にシリアル信号のフォーマットが異なるものがある
    - 主流はGRB
  - 稀に信号線が特殊なものがある

### 注意事項

- 動作電圧は基本5V
  - 動かない例
    - LEDの電源に3.3Vを供給しても動かない可能性がある
    - 5Vの電源に繋げて動かすと3.3Vの信号は無視される可能性がある
      - 解決策:「レベルシフト fet」で検索
  - データシートを読もう
  - でも意外と動いちゃったりする
    - 動けば正義なので問題なし
    - 動かなくても文句言えない
- チップLEDはとても熱に弱い
  - LEDがそもそも熱に弱い
  - チップ部品は物理的に小さいのですぐ熱くなる
  - はんだ付け
    - 加熱時間は最短を心掛ける
      - 予備はんだの活用
    - 放熱の時間を確保
      - 指で触ってやるだけでも冷める
    - 近くに放熱性能の高い部品をなるべく置かない
      - その部品をはんだ付けした熱が配線を伝ってLEDを破壊しに来る
      - 熱的になるべく切り離す

## ESP32開発ボードの設計

- 要件
  - USB電源で動く
  - USBから書き込みができる
  - USB経由でSerial.printできる

### C3版

- USBとC3のUSBピンを繋ぐ
- USBの5vを3.3vに落とす
- ENをプルアップ
- 電源に挟む0.1uFはモジュールが内蔵しているので要らない

```txt
-----------
|      GND|-->GND
| USB   D+|-->D+
|       D-|-->D-
|     VBUS|-->5V
-----------

      ----------------
      |  LM1117-3.3  |
 5V>--|IN         OUT|-->3V3
GND>--|GND           |
      ----------------

3V3>--~~~-->EN
      10k

      -----------------------
      |  ESP32-C3-WROOM-02  |
3V3>--|3V3                  |
 EN>--|EN                   |
      |                     |
 D+>--|IO19                 |
 D->--|IO18                 |
      |                     |
GND>--|GND                  |
      -----------------------
```

### 無印版

- シリアル変換に必要なコンデンサを用意
- シリアル変換に自動書き込み回路をつける
  - fet2つ+コンデンサ
- USBとシリアル変換のUSBピンを繋ぐ
  - 無印はUSBの機能を内蔵していないので変換チップが必要
- シリアル変換のTX,RXとESPのRX,TXピンを繋ぐ
- 自動書き込み回路とESPのIO0, ENピンを繋ぐ
- ENをプルアップ
- USBの5vを3.3vに落とす
- 電源に挟む0.1uFはモジュールが内蔵しているので要らない

```txt
-----------
|      GND|-->GND
| USB   D+|-->D+
|       D-|-->D-
|     VBUS|-->5V
-----------

5V>--||--<GND
    0.1u  (以後全て0.1uF)
        ------------
        |  CH340K  |
 5V>----|Vcc     TX|-->RX
GND>-||-|V3      RX|-->TX
GND>----|GND       |
        |          |
 D+>----|D+     DTR|-->DTR
 D->----|D-     RTS|-->RTS
        ------------
        
      -------------         -------------
      |          S|--<DTR>--|G          |
IO0<--|D BSS138   |         |  BSS138  D|-->EN
      |          G|--<RTS>--|S          |
      -------------         -------------
EN>--||--<GND

      ----------------
      |  LM1117-3.3  |
 5V>--|IN         OUT|-->3V3
GND>--|GND           |
      ----------------

3V3>--~~~-->EN
      10k

      ---------------------
      |  ESP32-WROOM-32E  |
3V3>--|3V3                |
 EN>--|EN                 |
IO0>--|IO0                |
      |                   |
 RX>--|RX                 |
 TX>--|TX                 |
      |                   |
GND>--|GND                |
      ---------------------
```

実装するときは裏返してはんだ付けしてUEWで配線すると変換基板を使わずに済む

## PlatformIOとは

### 概要

- マイコン開発環境およびIDE
  - できることはArduinoIDEと同じ
- いくつかの導入方法がある
  - 拡張機能としてのPlatformIO
    - vscodeとかatomの拡張機能として導入
  - CLIツールとしてのPlatformIO
    - Pythonのパッケージとして公開されている
    - PlatformIO Coreの名称で区別される
  - 単体のGUIアプリとしては存在しない
- メリット
  - ArduinoIDEの上位互換
  - 使い慣れたエディタの恩恵を受けられる
  - ArduinoIDEより起動が早い
  - ライブラリの依存関係を深く考えないでいい
  - Arduino以外の環境での開発もできる
- デメリット
  - 拡張機能として入れるといつものエディタの
    - 起動が遅くなる
    - 他のファイルの編集の時もでてきて邪魔
  - CLIの方はデメリット無いと思っている

### 導入

- 今回はCLIの方を入れていく
- [公式のチュートリアル](https://docs.platformio.org/en/stable/core/installation/methods/installer-script.html#installer-script-recommended)に従って進める
  - [インストールスクリプト](https://github.com/platformio/platformio-core-installer/blob/develop/get-platformio.py)をダウンロード
  - 適当なディレクトリでこれを実行(`python get-platformio.py`)
  - ユーザーのところに`.platformio`フォルダができる
  - [環境変数を`.platformio/penv/Scripts`に通す](https://docs.platformio.org/en/stable/core/installation/shell-commands.html)
  - ターミナルで`pio`コマンドが動けばok

## Hello World

### LEDで

- NeoPixelでLチカ
- 適当なフォルダを作って以下の通り二つのファイルを用意する  

  ```txt
  hoge
    platformio.ini
    src
      main.cpp
  ```

- platformioに設定を書く
  - [リファレンス](https://docs.platformio.org/en/stable/projectconf/index.html)
  - C3無印両対応の設定ファイル

  ```ini
  [platformio]
  name=hoge
  description=hogehoge
  default_envs=c3 ;無印の人はここをmujiにしておく

  [env]
  platform=espressif32
  framework=arduino
  targets=upload ;これ書いとくと-t upload要らないので楽

  [env:c3]
  board=esp32-c3-devkitc-02

  [env:muji]
  board=esp32dev
  ```

- main.cppにコードを書く
  - NeoPixel(RGB LED)を虹色に光らせるサンプル

  ```cpp
  void setup(){}
  void loop(){neopixelWrite(0,
   (sin(millis()/1000.    )*.5+.5)*16.,// フル出力だと眩しい
   (sin(millis()/1000.+2.1)*.5+.5)*16.,
   (sin(millis()/1000.+4.2)*.5+.5)*16.
  );delay(1);}
  ```

- ターミナルからコマンドを叩いて書き込む
  - まず開発ボードをpcに差す
  - `platformio.ini`のディレクトリで`pio run`
    - .iniを書き換えなくても`-e`オプションからenvを変更できる
    - 本当は`pio run -t upload`と書かないとアップロードはされないが今回は.iniに書いてあるので省略

### Serialで

- .iniの`[env:c3]`に書き足す
  
  ```ini
  build_flags= ;USBシリアル有効化
    -D ARDUINO_USB_MODE=1
    -D ARDUINO_USB_CDC_ON_BOOT=1
  ```

- main.cppを以下に差し替え
  - シリアルモニタに`Hello World! ミリ秒`を吐くサンプル
  
  ```cpp
  void setup(){Serial.begin();}
  void loop(){Serial.printf("Hello World! %u\n",millis());delay(100);}
  ```

- 書き込む
- シリアルモニタを見る
  - `pio device monitor`

### HTTPで

- の前に
  - LittleFS
    - ESPのフラッシュを間借りしてファイルシステムを作れる
    - ディレクトリ構造を持てる
    - プログラム本体とは別で管理できる
  - SmartConfig
    - スマホからESPの接続先WiFiを指定できる
    - 無料の専用のアプリを使う
      - Esptouch
      - 使い方
        - Esptouchの入った端末を用意
        - 用意した端末をESPに繋がせたいWiFiに接続
        - EsptouchからSmartConfigを選択
        - パスワードを入れてconfirm
        - ESPがWiFi接続に成功すると表示が出る
- .iniの`[env]`に書き足す
  
  ```ini
  board_build.filesystem=littlefs ;ファイルシステムにlittlefsを使用
  lib_deps= ;使うライブラリをgithubから取得
    https://github.com/dvarrel/AsyncTCP.git
    https://github.com/dvarrel/ESPAsyncWebSrv.git
  ```

- ファイルシステムに書き込むファイルを用意
  - .iniのある階層に`data`フォルダを作る
  - `data/index.html`を作って適当にHTMLを書く
- ファイルイメージをビルドして書き込む
  - `pio run -t uploadfs`
- main.cppを編集
  - LittleFSのサーバーを立てるサンプル
  - main.cpp `samples/server/src/main.cpp`
- 書き込む
- シリアルモニタを見る
  - SmartConfig startedが出たらEsptouchで設定
  - 同じWiFiに接続した機器のブラウザにIPを打ち込む

## WebSocketコントローラ

- WiFiを使ってスマホと高速双方向通信
- ロボットのコントローラーに便利
- の前に
  - WebSocket
    - [MDNのリファレンス](https://developer.mozilla.org/ja/docs/Web/API/WebSockets_API)
    - サーバーとクライアントで双方向に通信できる
    - テキスト送信とバイナリ送信の2つのモードがある
  - JavaScript

    ```js
    // HTMLの<script><script/>の中に書く

    // 変数
    let x=1; // 宣言
    x="hello"; // 変数に型が紐づけられていない
    const one=1; // こっちは定数

    // 関数(Funcion)
    const fn=(a,b,c)=>{return a+b+c}; // 変数と同等に扱うことができる
    fn(1,2,3)==6;
    isNaN(fn(1,2))==true; // 呼び出し時の引数が不足、超過していても実行される

    // 配列(Array)
    const arr=[1,3,4,8];
    arr[1]=2; // constでも中身は変更可能

    // オブジェクト(Object)
    const obj={a:1,b:"bb",c:x=>x+x};
    obj.a=0; // constでも中身は変更可能
    obj.c();

    // クラス
    const cls=class{
    constructor(){}
    a(){return 1;}
    };
    new cls().a()==1;

    // 既存のクラスの呼び出しと設定
    const ws=new WebSocket('ws://192.168.0.135/ws');
    ws.binaryType='arraybuffer'; // バイナリを受信したらArrayBufferに変換
    ws.onopen=e=>{ // イベント駆動 引数にオブジェクトが渡される 返値は無視される
    console.log(e); // 標準のログ
    };
    ws.onclose=e=>console.log(e);
    ws.onmessage=e=>console.log(e.data); // 受信データを吐く ここにStringかArrayBufferが入っている

    // 型付き配列(TypedArray)とArrayBuffer jsでバイナリ列を扱う
    /*
    jsは仮想マシンで動く
    仮想マシン --- Array --- 人間

    WebGLの登場で直接バイナリ列を扱う仕組みが必要になり策定された
    メモリ --- ArrayBuffer ---TypedArray --- 人間
              \--DataView --- 人間
    ArrayBufferは直接操作できない
    TypedArrayかDataViewを介して操作する
    */
    const
    ab=new ArrayBuffer(4), // 4byteのメモリを確保

    u8=new Uint8Array(ab), // 確保したメモリを符号なし8bit整数の配列として見る配列
    f32=new Float32Array(ab); // 同じメモリを32bit浮動小数点数として見る配列
    //これらの総称がTypedArray
    u8[0]=1; // 確保したメモリをUint8Array経由で操作
    f32[0]; // 確保したメモリをFloat32Array経由で読取

    new Uint8Array(arr); // 普通の配列から変換と同時にメモリ確保
    // メモリの開放はその参照がいかなる定数変数にも格納されていない状態になったときに自動的に行われる

    ws.send(new Uint8Array(arr));// websocketで送信 send関数はTypedArrayなどのバイナリデータを持つ型とStringのみを受け付ける
    ```

- ファイルを編集
  - LittleFSのサーバーとwebsocketのサーバーを立てるサンプル
  - main.cpp `samples/websocket/src/main.cpp`
  - index.html `samples/websocket/data/index.html`
- ファイルシステムの書き込み
- プログラムの書き込み
- EsptouchでWiFiに接続
- シリアルモニタのIPを見て接続
