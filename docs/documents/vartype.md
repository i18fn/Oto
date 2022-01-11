---
layout: default
title: "ドキュメント - 変数とデータ型"
---

# 変数とデータ型

Otoで変数を定義する際には、型の宣言は必要ありません。

```basic
abc = 123
```

上の場合、変数abcを初期値123として定義します。


Otoでは以下の型が使用できます

| 型名       | 説明                              | 宣言方法                                                      | 
| :--------: | :-------------------------------: | :----------------------------------------------------------: | 
| float      | 64bit幅の実数値を保持する          | f = 12.3                                                     | 
| oscillator | 基本波形を出力する発振器を保持する  | o = OSCIL[<出力波形番号>, <入力信号の波形>, <入力信号の周波数>] | 
| sound      | 音源情報を保持する                 | s = SOUND[<oscillator型の変数>]                               | 

<h2 id="float-type">float型</h2>

Otoには整数型は存在しません。そのため、全ての数値型は実数型になります。

```basic
/* どちらもfloat型 */
baba = 1
keke = 2.2
```

<h2 id="float-type-operation">float型の演算</h2>

float型の変数や定数は四則演算などの演算ができます。

```basic
ototo = 1 + (2 - 3 * 4 / 5)
oto = 1 >= 2
```

使用できる演算子は以下の通りです。

| 演算子 | 説明 | 備考 |
| :---: | :--: | :--: |
|   +   | 足し算 | |
|   -   | 引き算 | |
|   *   | 掛け算 | |
|   /   | 割り算 | |
|   %   | 割り算の余り | 小数点以下切り捨てで計算されます |
|   ==  | 2つの値が等しければ1、そうでないなら0 ||
|   !=  | 2つの値が等しくなければ1、そうでないなら0 ||
|   >   | 左の値 > 右の値なら1、そうでないなら0 ||
|   <   | 左の値 < 右の値なら1、そうでないなら0 ||
|   >=  | 左の値 >= 右の値なら1、そうでないなら0 ||
|   <=  | 左の値 <= 右の値なら1、そうでないなら0 ||
|  AND  | 左の値と右の値が両方とも0以外の値を持つのであれば1、そうでないなら0 ||
|   OR  | 左の値と右の値のどちらかでも0以外の値を持つのであれば1、そうでないなら0 ||

<h2 id="oscil-type">oscillator型</h2>

oscillator型は、基本波形の発振器を表す型です。基本的には、音源を自作する際の基礎となる波形(sin波など)を出力するために使用します。
今後のアップデートで、FM音源をサポートするために生み出された型です。

第1引数には出力波形の番号、第2引数には入力信号波形の番号、第3引数には入力信号波形の周波数を指定します。入力信号波形とその周波数は省略可能です。

```basic
/* 正弦波を出力するOscilの定義 */
osc = OSCIL[1]

/* 入力信号として200Hzの矩形波を指定して、のこぎり波を出力する */
fmosc = OSCIL[2, 3, 200]
```

波形番号は以下の通りです。

| 波形番号 | 波形の種類 |
| :-----: | :--------: |
|    1    |   正弦波   |
|    2    | のこぎり波 |
|    3    |   矩形波   |
|    4    |   三角波   |
|    5    |   ノイズ   |

Otoの標準ライブラリ内に基本波形のoscillatorは用意されているので、基本的にはそれを使うことをおすすめします。

<h2 id="sound-type">sound型</h2>

sound型は音源を表す型です。内部でoscillatorと接続されたフィルターの情報を保持しており、PLAY命令などの音を出力する命令では、その情報を利用して音色を作成します。

sound型の変数を定義するときには、oscillator型の変数が必要です。

```basic
osc = OSCIL[1]
s = SOUND[osc]
```

Otoでの音遊びプログラミングにおいて重要な型になります。詳しくはフィルターの項を参考にしてください。