rem 如果Keil安装位置非C盘，则设置为新的fromelf.exe位置
C:\Keil_v5\ARM\ARMCC\bin\fromelf.exe --bin -o "..\Output\App.bin" "..\MDK\output\release\output.axf"
..\Resource\mergehex.exe --merge ..\Resource\BootLoader.hex   ..\MDK\output\release\output.hex    --output ..\Output\dest.hex 




