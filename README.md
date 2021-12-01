# CrazyArcade
Members: Zhengyang Zou, Jiajun Ji, Chengji Yu, Yiping Dong, Dijing Zhang

### Overview
* **Game Rules**  
  * Basic rules  
    * Using bubbles to destruct cells and attack the opponent.  
  * Basic props  
    * Longer bubble  
    * More bubbles  
    * Speed up  
  * Key settings  
    * Player1:  
      * W/S/A/D -- Move  
      * SPACE -- Place Bubble  
    * Player2:  
      * UP/DOWN/LEFT/RIGHT -- Move  
      * ENTER -- Place Bubble  

### Install
* **Mac**
    ```
    ./install-mac.sh -p ../Some_Path
    ```
    Change `../Some_Path` to your own project path which contains YS library files.


* **Win (Based on the test on Visual Studio 2019 Community version)** 
    * First, create a new project and the use Console App for project.  
    * Second, remove default file and copy everything under `/Src`, `/Inc`, `/Ext/Win/Src`, `/Ext/Win/Inc`, `/Data/Map`, `/Data/Sound` folder from original path to `Your_Path_To_New_Project_File_Folder`.  
    * Third, press `Shift+Alt+A` or use Project->Add Existing Item function to add all copied files into current project.  
    * Last, run `main.cpp` to start the program.
    * (You have other options, e.g., use command line to compile. Make sure the files are under the same file directory and the command includes all the source file in /Src.)
    

### Code Construction
<img src="Data/Readme/code_structure.png" width="60%">


### Work Flow
<img src="Data/Readme/flow_chart.png" width="60%">

