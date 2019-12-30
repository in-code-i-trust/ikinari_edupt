ikinari_edupt
=============

UEFIアプリケーションとしてeduptをコンパイルできるようにしたリポジトリです。  
一部不完全な実装があります。  
  
パストレーサーとしての動作には手を加えていないため、そこに興味があればオリジナルを参照してください。  
https://github.com/githole/edupt  
  
### 必要なもの
- clang+LLD  
  - http://releases.llvm.org/download.html  
  - 9.0.0で確認
  - パスを通すこと
- QEMU
  - https://www.qemu.org/
  - 20190815確認
- OVMF 
  - https://sourceforge.net/projects/edk2/files/OVMF/
  - OVMF-X64-r15214.zipで確認

## 動かし方
### UEFI版
1. `emake.bat run`を実行するとQEMUで起動する  
  
実機で動かすにはFAT32フォーマットのUSBフラッシュメモリを用意し、`/EFI/BOOT/BOOTX64.EFI`をコピーする。  
一部の実機ではセキュアブートが有効になっているものがある。それでは起動できないため、セキュアブートは一時的に切る。  
※WindowsでBitLockerを有効にしている環境でセキュアブート設定を切り替えるとBitLocker回復キーが必要になるので要注意  

### Windows版の動かし方
1. `kernel32.lib`を`wmake.bat`と同じディレクトリに配置  
2. `wmake.bat run`を実行  
※オリジナル版を素直にVisual Studioでビルドするのが楽です  
  
EOF