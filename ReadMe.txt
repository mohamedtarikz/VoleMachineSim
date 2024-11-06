Console App.:
1- you can choose to add instructions manually or by a file
2- if you choose to add manually you should write all the instructions seperated by an enter or a space
3- if you choose to add a file make sure the text file is in the same folder of the exe file and write the name of the text file
4- then you could choose to run one step or play all instructions or print the state of the machine
5- when all the instructions are done it ptints the state of the machine automatically

GUI:
1- you can add instructions via writing the instruction in the 4 text boxes at the top then pressing enter on the keyboard or the "ADD" button
2- you can add a file of instructions via the "load file" button
3- the "One Step" button runs only one step
4- the "Play/Pause" button toggles playing and pausing of all instructions
5- the speed control buttons allow you to choose the speed you want the machine to run when playing all instructions
6- the screen prints errors(red) and if a value is stored in memory address 00(white) and messages(green)
7- the register that has been updated in the last instruction should be highighted in blue
8- the memory address that should be processed in the next step will be highighted in yellow
9- the memory address that was updated in the last operation should be highighted in blue
10- if you hover on the IR on the bottom left a tooltip should inform you what this instruction does
11- if you hover on any button a tooltip should appear to explain what this button does

General Notes:
1- all values printed in the program are in hexa - except the PC in the Console app.
2- in addition - either two's complement or floating point formats - values that cause overflow and can't be represented in 8-bits would throw an error and won't complete the process, won't give an estimate!!
3- in console app. file name should be written with the extension