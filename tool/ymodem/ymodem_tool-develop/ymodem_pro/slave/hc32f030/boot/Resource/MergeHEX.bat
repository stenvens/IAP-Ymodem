rem 如果Keil安装位置非C盘，则设置为新的fromelf.exe位置
C:\Keil_v5\ARM\ARMCC\bin\fromelf.exe --bin -o "..\Resource\BootLoader.bin" "..\MDK\output\release\BootLoader.axf"
copy "..\MDK\output\release\BootLoader.hex" "..\Resource\BootLoader.Hex"

