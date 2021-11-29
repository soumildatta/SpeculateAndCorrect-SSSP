# Speculate And Correct Algorithm

This project was created as my senior project.

This project proposes an optimized version of the Bellman-Ford SSSP algorithm, which can carry out the SSSP algorithm more efficiently than the standard Bellman-Ford algorithm.

## Dependencies
**Hardware Requirements:**
* CPU with support for more than 1 thread 
**Software Requirements:**
* UNIX-Like Operating System (Linux, macOS, etc.)
* g++ compiler (GCC) to compile C++ code
  * To install GCC on Ubuntu, follow the steps from the link [here](https://linuxize.com/post/how-to-install-gcc-compiler-on-ubuntu-18-04/).
  * To install GCC on macOS, install the Command-Line tools from Xcode. Follow the steps from the discussion answer in the link [here](https://discussions.apple.com/thread/8336714).
* Cmake (Version 3.22.0+)
  * Cmake can be downloaded and installed from the link [here](https://cmake.org).
* Git CLI
* Eclipse CDT IDE (Version 2021-06+)

**Cloning project from GitHub**\
In order to run the project on your local machine, [this project repository](https://github.com/soumildatta/SpeculateAndCorrect-Algorithm) needs to be cloned.\
To clone the repository, open a terminal and run the following command (requires Git CLI):\
`git clone https://github.com/soumildatta/SpeculateAndCorrect-Algorithm.git`

**Building the project:**\
This project contains several experiments and other directories housing data and common scripts. Change directories to the project directory using the command:\
`cd SpeculateAndCorrect-Algorithm`

**To build a specific experiment with Cmake:**\
This project supports the creation of makefiles with Cmake.
* Change directory into the desired experiment directory with the cd command
* Use cmake to create a makefile for the experiment (requires Cmake):\
	`cmake .`
* Use the generated makefile to compile the experiment with the following command:\
	`make`
* Run the generated executable with the required arguments (complete list of arguments in the use section):\
	`./experimentName <arguments . . . >`

## Usage
The project contains several standalone experiments that does the SSSP algorithm on graph datasets. These experiments are in their own directories within the project. The experiments present in the project are:
1. **BellmanFord**: The standard serial Bellman-Ford algorithm implementation 
2. **SpecAndCorr**: The new algorithm implemented serially
3. **ParallelSpecAndCorr**: The new algorithm implemented in parallel

Other directories contain important scripts, datasets, and solution files used in the project. These directories are listed below:
1. **shared**: Contains helper scripts used by the experiments
2. **utility**: Contains symbolic links to the helper scripts in the shared folder 
3. **performance**: Contains .csv files that are the performance outputs of the experiments
4. **solution files**: Contains solution files that can be used with the different datasets

These projects can be run on two different environments based on the types of user and their needs.
The two types of possible users are:
1. Researchers in academia 
2. Casual users simply looking to run the SSSP algorithm on graphs more efficiently

### Usage for Researchers
Those in academia would want to assess the code and fine tune things, as well as improve upon the existing code. To do this, the best option for running the experiments is to use the Eclipse CDT IDE.
1. After cloning the repository to the local machine, start eclipse and set the eclipse workspace to the project directory as shown in the example below (Note: your project directory may be located somewhere else):
![image](https://user-images.githubusercontent.com/35431692/143920176-1446674f-416b-4c2c-8635-d6ce89001d9d.png)
2. Now, the experiment folders can be added as projects into Eclipse. 
  * To do this, go to File > New > C/C++ Project
  * Click on C++ Managed Build 
  * Enter the name of the experiment you want to run into the “Project Name” text box (is case sensitive). For example, to import the “BellmanFord” experiment, you would type in the following (Note: if you see a “Directory with specified name already exists” message at the top, the experiment name has been entered correctly”):
![image](https://user-images.githubusercontent.com/35431692/143920326-dd188744-6ed5-4777-ba86-0122ced9a6ae.png)
  * Note: For MacOS users, select the “MacOSX GCC” toolchain from the toolchains selection window above.
  * Finally, click finish.
3. Before we can run the experiment, we need to make sure eclipse compiles the project with the c++17 flag. To do this, follow these steps:
  * Right click on the experiment in the project explorer and click on properties from the dropdown.
  * On left side of the next window, click on C/C++ Build > Settings
  * Click on GCC C++ Compiler
  * Change the Command “g++” to “g++ std=c++17” and click on the Apply and Close.
![image](https://user-images.githubusercontent.com/35431692/143920444-042ba67e-1f40-40d1-905a-b7cd35a9a23a.png)
4. The last step is to specify the command line arguments. To fo this, follow these steps:
  * On the top toolbar of Eclipse, next to the run button, click on the little dropdown arrow.
  * Click on “Run Configurations…” from the dropdown menu.
![image](https://user-images.githubusercontent.com/35431692/143920506-f1c0ac85-a007-404c-8a4a-8e9754ca2084.png)
  * On the left side of this window, double click the “C/C++ Application” dropdown.
  * Then click on the name of the project.
  * On the right side, click on the “Arguments” tab. This is where you can enter your command line arguments. The required arguments are explained in detail in the “Command Line Arguments” section of this document. An example for the BellmanFord Project is shown below:
![image](https://user-images.githubusercontent.com/35431692/143920606-3fe7d72e-a6e3-4f67-a448-9db0df939d61.png)
  * Finally, click Apply.
5. You are now ready to build and run the experiment. To build the project, make sure the project is selected in the project explorer and press the key combination: 
  * For Linux: **Control + B**
  * For MacOS: **Command + B**
6. Now, click the run button (boxed in red on the visual below) on the top bar and the project should start by processing the graph.
![image](https://user-images.githubusercontent.com/35431692/143920704-2b922420-6fcb-4e16-b3f9-0bbd5f2bfc37.png)

### Usage for Casual Users
Casual users who just want to view the performance improvement, generate statistics, and conduct SSSP algorithms on datasets might not have the need to view or manipulate the code directly. There is an easier way to build and run the project without using the Eclipse IDE in this case. The user will require Cmake to be installed on their machine.
1. After cloning the repository to the local machine, open a terminal and change directories into the project directory with the cd command. 
2. Choose an experiment to run, and change directories into that experiment directory with the cd command. 
3. This project supports the creation of Makefiles with Cmake. To use this feature, run the following command in the experiment directory:\
	`cmake .`
4. This creates a makefile for the project. We can now use this makefile with the command below to build and create an executable that can be used to run the experiment:\
	`make`
5. The executable has been created, and can now be used along with the command line arguments (specified in detail in the “Command Line Arguments” section of this manual) to run the experiment. An example with the SpecAndCorr experiment is shown below:\
	`./SpecAndCorr USA-road-d.NY.gr NY.SSSPSolution output.csv 10 0`
\
**Example Run:**
![image](https://user-images.githubusercontent.com/35431692/143920880-315054f3-47e2-43b7-8157-3eb8e36a755e.png)

### Command Line Arguments 
This project contains 2 types of experiment algorithms:
1. Serial Algorithms 
2. Parallel Algorithms (denoted by the word Parallel in the experiment name)

Each type of experiment has the same argument list and order. The argument list for each is specified below as well as the definitions:\

**Serial Algorithms:**\
\<InputFilename> \<VerifyFilename> \<OutputFilename.csv> \<NumIterations> \<SourceNodeIndex>\
Example: \
	`./BellmanFord USA-road-d.NY.gr NY.SSSPSolution output.csv 10 0`


**Parallel Algorithms:**\
\<InputFilename> \<VerifyFilename> \<OutputFilename> \<NumIterations> \<SourceNodeIndex> \<OPTIONAL = MaxNumThreads>\
Example:\
	`./ParallelSpecAndCorr USA-road-d.NY.gr NY.SSSPSolution output.csv 10 0 15`

**Definitions:**
* **InputFileName** - This is the graph dataset that will be parsed by the program (Note: the project currently only supports .gr file format from the DIMACS Shortest Path Implementation challenge).
* **VerifyFilename** - This is the solution file that will be used to check the correctness of the algorithms. Several solution files for the 9th DIMACS Shortest Path Implementation challenge datasets are included in this project in the “Solution Files” directory.
* **OutputFilename** - This is the output file that prints the performance results for the experiments. These files are of type .csv and the extension must be added to the filename to properly view the results. The serial experiments contain the iteration and the time the algorithm took to complete in that iteration. The parallel experiments contain the number of threads, the speedup at the number of threads, and the average time taken by the algorithm.
* **NumIterations** - This is the number of times the program will run the algorithm on the graph to get a better estimate for the average time taken by the algorithm
* **SourceNodeIndex** - This is the index of the source node from which the distance to the other nodes will be calculated.
* **MaxNumThreads** - In a parallel experiment, this is the number of threads that the program will use at the last thread iteration. This is an optional argument: if it is not specified, the program will use the maximum number of threads that the hardware supports. 

### Datasets
This project uses roadmap datasets from the [9th DIMACS Implementation Challenge](http://www.diag.uniroma1.it/challenge9/) - Shortest Paths. The datasets can be downloaded from the link [here.](http://www.diag.uniroma1.it/challenge9/download.shtml) 

### Solution Files
Each experiment algorithm in this project has been tested against solution files that were generated during the research. These solution files have been compared against solutions from the Gunrock Benckmarking Suite. Some solution files are included in the directory called "solution files".
  
## Maintenance and Enhancement
This project is well organized to aid in the extension and enhancement of the algorithms and other aspects of the program.

### Update Helper Scripts
  The helper scripts used in this project by the experiments are housed in the folder called “shared” and can be maintained and enhanced. The experiments in the projects contain symbolic links to the scripts in this directory - if the code in this directory is changed, then it will be changed anywhere else it is used. This makes it easier to change redundant code in multiple locations at once and aids in the maintenance of the codebase.

### Add new Helper Scripts
Additional helper scripts must be added to the “shared” directory. Once the helper script has been added, head to the “utility” folder and open the createLinks.sh file with a text editor. This file is used to create the symbolic links in the utility directory which can then be copied over to the experiments. 
To create a new symbolic link:
* Add a new line with the following command for the new script:\
	`ln -s ../shared/newScriptName ./newScriptName`
* Now, open a terminal window and cd into the directory with the createLinks.sh file. Here, we need to run this file. To run this file, simply run the following command:\
	`./createLinks.sh`
  
### Add New Algorithms/Experiments
The addition of new algorithms or experiments for comparison is easy. Simply create a new folder, copy the symbolic links for the helper scripts from the utilities folder, and create the new program with the same output file format. 

### Contribute to GitHub Repository
If the changes you made are significant, you are welcome to contribute your efforts to the repository. To make contributions, fork the repository, make your changes, and create a pull request explaining the changes made. Your contributions may be accepted if they meet the standards of the project and do not contain breaking changes.
