# How to setup your userspace

The following assumes your name is **YOURNAME**, which must be different from already existing names.
Create your personal folder at **submissions/YOURNAME**.
You will need a script named **submissions/YOURNAME/build** to generate your executables, which takes two arguments in order :
- The folder pointing the sources of the puzzle you have to build.
- The folder pointing to the location where you have to put your binaries and temporary build files. Your entry point must be an executable called **run**.
You are free to use whatever extra files you need in that folder.

Assuming you are working on day 2, the sources of your puzzle solution must reside in the folder at **submissions/YOURNAME/days/02**. Do not put extra stuff in the **days** folder. Your puzzle folder on the other hand, may contain multiple puzzle input files whose name starts with **input**, on top of any other source file.

# The build

> ./run -n YOURNAME -d 02 -i OTHERNAME
Will call your build script to create the appropriate executable for day 2, run the inputs of OTHERNAME on it, and print the result. You can try different combinations of user, day, and input, including your own inputs.
Omitting an option will run all the possible choices for that option, but will not print the results unless only one output was generated. For example :
> ./run -n YOURNAME -d 14
Will run your solution for day 14 for your inputs, test inputs, and every other user's inputs.
> ./run -d 07
Will run every combination of user and input possible for day 7.

The **build** folder holds executables, temporary build files and output content. Make sure not to touch the folder **build/submissions/YOURNAME/DAY/outputs/** when creating temporary build files.
**build/outputlist** shows a list of all the files containing outputs related to the last call to the run script.

# Multi-output statistics

TODO
Not sure what is relevant to make here yet.
