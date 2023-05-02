# Blackmax チュートリアル

ここでは cvmfs 上のパッケージを用いてコンパイルをします。

## BlackMax ドキュメント
* [paper](https://arxiv.org/abs/0711.3012)
* [Manual](https://arxiv.org/pdf/0902.3577.pdf)
* [Source code download](https://blackmax.hepforge.org/)

## ダウンロード & 解凍
```bash
$ cd YourFavoriteDirecotry
$ wget https://blackmax.hepforge.org/downloads/?f=BlackMax-2.02.0.tar.gz BlackMax-2.02.0.tar.gz
$ gunzip BlackMax-2.02.0.tar.gz
$ tar -xvf BlackMax-2.02.0.tar
$ cd BlackMax
```

## Makefileの変更
そのままだと動かないので、Makefile (`BlackMax/Makefile`)を修正する必要があります。
```diff
@@ -1,14 +1,14 @@
 F_FLAGS = -g
 C_FLAGS = -w -O0 -D_GNU_SOURCE -Df2cFortran -fpic 
-COMP    = -fno-globals -fno-automatic -finit-local-zero 
+COMP    = -fno-automatic -finit-local-zero 
 
 OBJECTS = BlackMax.o upinit.o upevnt.o initpy.o 
 
 ###################################################
 #### Change the location of these libraries #######
 
-PYTHIALIB = /data/rizvi/pythia-6.4.10-gcc4.1.2
-PDFLIB = /data/rizvi/lhapdf-5.8.2-gcc4.1.2/lib
+PYTHIALIB = /cvmfs/sft.cern.ch/lcg/releases/LCG_103/MCGenerators/pythia6/429.2/x86_64-centos7-gcc11-opt/lib/
+PDFLIB = `lhapdf-config --libdir`
 
 ###################################################
 
@@ -25,7 +25,7 @@
 
 all: $(OBJECTS) GetPdf.o IsPythiaOn1.o 
 	gcc $(COMP) $(OBJECTS) GetPdf.o IsPythiaOn1.o -lm -l$(F77LIB) \
-	-L $(PYTHIALIB) -lpythia \
+	-L $(PYTHIALIB) -lpythia6 \
 	-L $(PDFLIB) -lLHAPDF \
 	-o BlackMax
 	rm *.o
@@ -43,7 +43,7 @@
 
 PyCheck: PyCheck.o
 	$(F77COMP) $(COMP) PyCheck.o -lm -l$(F77LIB) \
-	-L $(PYTHIALIB) -lpythia \
+	-L $(PYTHIALIB) -lpythia6 \
 	-L $(PDFLIB) -lLHAPDF \
 	-o PyCheck
 	rm PyCheck.o
```

## Compile
```bash
# LCG releasesのロード
$ source /cvmfs/sft.cern.ch/lcg/views/LCG_103/x86_64-centos7-gcc11-opt/setup.sh
# LHAPDF 設定
$ export LD_LIBRARY_PATH=`lhapdf-config --libdir`:${LD_LIBRARY_PATH}
$ export LHAPATH=/cvmfs/sft.cern.ch/lcg/external/lhapdfsets/current
$ export LHAPDF_DATA_PATH=`lhapdf-config --datadir`:/cvmfs/sft.cern.ch/lcg/external/lhapdfsets/current
# コンパイル
$ make BlackMax
```

## Run

### 作業ディレクトリへ移動
```bash
$ mkdir YourFavoriteDir; cd YourFavoriteDir
$ cp BLACKMAX_SRC_DIR/* . # 実行ファイル + 設定ファイルをコピー
$ edit parameters.txt, L.26: 200 -> 10042  # PDF設定の変更
$ ./BlackMax
```
`BlackMaxLHArecord.lhe` と `output.txt`が作成されているはずです。
`BlackMaxLHArecord.lhe`が後段のシミュレーションで使用されます。

`parameters.txt`を変更することで、様々なコンフィグレーションでブラックホールの生成ができます。
`parameters.txt`の詳細は[マニュアル]()を参照してください。

### マスプロダクション用のスクリプト
* [run_blackmax.sh](scripts/run_blackmax.sh)
    * 4,5,6行目の変数 (Blackmaxをインストールしたディレクトリ、Blackmaxの出力を保存するディレクトリ、[template_input.txt](scripts/template_parameter.txt)のパス)を変更後使用してください。
    * いくつかの引数が使用できます。
        ```bash
        ./run_blackmax.sh CM_ENERGY M_PL M_BHMIN EVO_TYPE N_EXDIM PDFname SEED
        ```
        例えば、重心系エネルギー100TeV, プランク質量20TeV, ブラックホール質量の最小値20TeV, ADDモデル(1), 余剰次元の数 6, PDFはCTEQ6L1, 乱数シード 100000001とするときのコマンドは以下です。
        ```bash
        ./run_blackmax.sh 100000 20000 20000 1 6 CTEQ6L1 100000001
        ```
  
