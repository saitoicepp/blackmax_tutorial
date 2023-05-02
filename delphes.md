# Delphes チュートリアル

## Delphes ドキュメント
* [document](https://arxiv.org/abs/1307.6346)
* [公式webサイト](https://cp3.irmp.ucl.ac.be/projects/delphes)
* [source code](https://github.com/delphes/delphes)

## ダウンロード & 解凍
```bash
$ cd YourFavoriteDirecotry
$ wget http://cp3.irmp.ucl.ac.be/downloads/Delphes-3.5.0.tar.gz
$ tar -zxf Delphes-3.5.0.tar.gz
$ cd Delphes-3.5.0
```

## Compile
Pythia は LCG releases 上のものを使用します。
```bash
$ source /cvmfs/sft.cern.ch/lcg/views/LCG_103/x86_64-centos7-gcc11-opt/setup.sh
$ export HAS_PYTHIA8=true
$ make
```

## Run

### 作業ディレクトリへ移動
```bash
# 作業ディレクトリへ移動
$ mkdir YourFavoriteDir; cd YourFavoriteDir
# LHAPDF の設定
$ lhapdflib=$(lhapdf-config --libdir)
$ lhapdfdata=$(lhapdf-config --datadir)
$ export LD_LIBRARY_PATH=${lhapdflib}:${LD_LIBRARY_PATH}
$ export LHAPATH=/cvmfs/sft.cern.ch/lcg/external/lhapdfsets/current
$ export LHAPDF_DATA_PATH=${lhapdfdata}:/cvmfs/sft.cern.ch/lcg/external/lhapdfsets/current
# 検出器設定ファイルをコピー
$ cp -r Delphes_SRC_DIR/cards/FCC .
# Pythia8 コマンドファイルの作成
$ cat << EOF > setup.cmnd
Main:numberOfEvents       = 10
Beams:frameType           = 4
Beams:LHEF                = ...../BlackMaxLHArecord.lhe   #(BlackMaxの出力ファイルのパス)
PDF:pSet                  = LHAPDF6:cteq6l1
EOF
# 実行
$ DELPHES_SRC_DIR/DelphesPythia8 FCC/FCChh.tcl setup.cmnd output.root
```
コマンドファイルの記述については[Pythia8 manual](https://pythia.org/latest-manual/Welcome.html)を参照してください。例えば`Beams:frameType`の記述は左上の検索窓で`Beams:frameType`と入力することで参照することができます。

`DelphesPythia8` コマンド実行後、`output.root`が作成されたはずです。

### マスプロダクション用のスクリプト
* [run_delphes.sh](scripts/run_delphes.sh)
    * 4,5行目の変数 (Delphes をインストールしたディレクトリ、Delphes の出力を保存するディレクトリ)を変更後使用してください。
    * 引数は一つで、BlackMaxの出力ファイルです。
        ```bash
        ./run_delphes.sh LHEFILE
        ```
  
