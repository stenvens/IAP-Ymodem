rem 如果Keil安装位置非C盘，则设置为新的fromelf.exe位置
C:\Keil_v5\ARM\ARMCC\bin\fromelf.exe --bin -o "..\Resource\BootLoader.bin" "..\MDK-ARM\boot\boot.axf"
copy "..\MDK-ARM\boot\boot.hex" "..\Resource\BootLoader.Hex"

