# ROOT ファイルの読み書き

## ROOT ドキュメント
* [公式 Get Started](https://root.cern/get_started/)
* [2020年ATLASソフトウェア講習資料](https://wiki.kek.jp/pages/viewpage.action?pageId=127280337)

## セットアップ
### login icepp / lxplus 等
```bash
$ source /cvmfs/sft.cern.ch/lcg/views/LCG_103/x86_64-centos7-gcc11-opt/setup.sh
```
### Mac
see [公式ドキュメント](https://root.cern/install/#macos-package-managers)


## Run

### 中身の確認
```bash
$ root ROOT_FILENAME
root [1] .ls
TFile**		BlackMaxLHArecord.root	
 TFile*		BlackMaxLHArecord.root	
  KEY: TProcessID	ProcessID0;1	eb53ef8c-e81c-11ed-b293-cb70529dbeef
  KEY: TTree	Delphes;1	Analysis tree
```
Delphesの出力ファイルの中身を見ると、"Delphes"という名前のTTreeが保存されていることがわかります。

```bash
root [2] Delphes->Print()
******************************************************************************
*Tree    :Delphes   : Analysis tree                                          *
*Entries :       10 : Total =         5471691 bytes  File  Size =    1733800 *
*        :          : Tree compression factor =   2.90                       *
******************************************************************************
*Br    0 :Event     : Int_t Event_                                           *
*Entries :       10 : Total  Size=      12741 bytes  File Size  =        128 *
*Baskets :        1 : Basket Size=      64000 bytes  Compression=   1.27     *
*............................................................................*
*Br    1 :Event.fUniqueID : UInt_t fUniqueID[Event_]                         *
*Entries :       10 : Total  Size=        778 bytes  File Size  =        135 *
*Baskets :        1 : Basket Size=      64000 bytes  Compression=   1.28     *
*............................................................................*
*Br    2 :Event.fBits : UInt_t fBits[Event_]                                 *
*Entries :       10 : Total  Size=        758 bytes  File Size  =        135 *
*Baskets :        1 : Basket Size=      64000 bytes  Compression=   1.25     *
*............................................................................*
*Br    3 :Event.Number : Long64_t Number[Event_]                             *
*Entries :       10 : Total  Size=        803 bytes  File Size  =        155 *
*Baskets :        1 : Basket Size=      64000 bytes  Compression=   1.35     *
*............................................................................*
```
`(TTreeのオブジェクト名)->Print()`でTTree内のファイルを一覧表示できます。

```bash
root [3] Delphes->MakeClass()
Info in <TTreePlayer::MakeClass>: Files: Delphes.h and Delphes.C generated from TTree: Delphes
(int) 0
```
`(TTreeのオブジェクト名)->MakeClass()`でTTree読み込み用のヘッダファイルが自動生成されます。"(TTreeのオブジェクト名).h"(ここでは`Delphes.h`)を見ることで、変数を確認できます。

### ROOTファイルスクリプト例
以下のいくつかの実装例は全て(ほぼ)同じ動作をします。Delphes root ファイルを読み込んでヒストグラム入りのroot ファイル(`hist.root`)を出力させています。

* C++
  * TBranch を使う実装 (省略します)
  * TTreeReader を使う実装
    * 一変数ずつ読み込む方法 [read_ntuple.cc](scripts/read_ntuple.cc)
      ```bash
      $ root "read_ntuple.cc(\"ROOT_FILENAME\")" 
      ```
    * クラスごとに読み込む方法 [read_ntuple_libDelphes.cc](scripts/read_ntuple_libDelphes.cc)
      * Delphes関連ファイルを読み込む必要があります。以下のようにしてc++ソースファイルと同じ場所にDelphesファイルをリンクさせてください。
        ```bash
        ln -s Delphes_SRC_DIR/classes .
        ln -s Delphes_SRC_DIR/external .
        ln -s Delphes_SRC_DIR/libDelphes.so .
        ```
        ```bash
        $ root "read_ntuple_libDelphes.cc(\"ROOT_FILENAME\")" 
        ```
* Python
  * PyROOT
    * [read_ntuple_libDelphes.py](scripts/read_ntuple_libDelphes.py) (Delphesファイルのリンクが必要です。C++の例と同様にしてください。)
      ```bash
      $ python read_ntuple_libDelphes.py -i ROOT_FILENAME -o hist.root
      ```

### hist.root の読み込み
* Interactiveにdraw
```bash
$ root hist.root
root [1] .lsa
TFile**		hist.root	
 TFile*		hist.root	s
  KEY: TH1D	h_truthElectrons_pt;1	
  KEY: TH1D	h_truthMuons_pt;1	
  KEY: TH1D	h_truthJets_pt;1	
  KEY: TH1D	h_jets_pt;1	
  KEY: TH1D	h_electrons_pt;1	
  KEY: TH1D	h_muons_pt;1	
  KEY: TH1D	h_met;1	
root [2] h_jets_pt->Draw();
```
* TBrowserを使う方法 (こちらのほうが簡単ですが、ssh接続先でrootを実行しているときは問題が発生するかもしれません。)
```bash
$ root hist.root
root [1] TBrowser a;
```