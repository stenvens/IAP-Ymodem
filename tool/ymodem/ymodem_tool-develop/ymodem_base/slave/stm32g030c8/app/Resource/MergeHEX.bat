rem 如果Keil安装位置非C盘，则设置为新的fromelf.exe位置
C:\Keil_v5\ARM\ARMCC\bin\fromelf.exe --bin -o "..\Output\App.bin" "..\MDK-ARM\app\app.axf"
..\Resource\mergehex.exe --merge ..\Resource\BootLoader.hex   ..\\MDK-ARM\app\app.hex    --output ..\Output\dest.hex 




