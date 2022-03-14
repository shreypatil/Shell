## To Run :

````
$make -f mf.mk

$./shell
````

## File Structure
```
.
├── include
│   ├── cd.h 
│   ├── echo.h
│   ├── printwd.h
│   ├── execute.h
│   ├── ls.h
│   ├── gvars.h  
│   ├── colors.h 
│   ├── sighan.c
│   ├── replay.c
│   ├── myutils.c
│   ├── procll.c
│   ├── sig.c
│   ├── bg.c
│   ├── fg.c
│   ├── jobs.c 
│   └── pinfo.h
│ 
├── README.md
│
├── mf.mk  - make file
│
├── obj
│
└── src
    ├── cd.c
    ├── colors.c 
    ├── echo.c
    ├── execute.c  
    ├── ls.c
    ├── pinfo.c
    ├── printwd.c  
    ├── shell.c  
    ├── sighan.c
    ├── replay.c
    ├── myutils.c
    ├── procll.c
    ├── sig.c
    ├── bg.c
    ├── fg.c
    ├── jobs.c
    └── pinfo.c
    

```
## Functions / files:
1. shell.c - contains program loop, tokenizes string.
2. execute.c - handles redirects, calls necessary functions for commands.
3. cd - change working directory  ``` cd <path> ```
4. ls - list items in directory ``` ls -<tags?> <path?>```
5. printwd - print working directory ``` pwd ```
6. pinfo - displays info of a process ``` pinfo <pin?>```
7. echo - tokenizes string by space or tabs ``` echo <String>```
8. bg - changes status of bg process to running ``` bg <job_no>```
9. fg - brings bg process to fg ``` fg <job_no>```
10. jobs - displays bg processes ``` jobs -<tags>```
11. sig - sends signal to job ``` sig <job_no> <sig>```
12. replay - replays a command after intervals for period ``` replay -command <command> -internal <interval> -period <period>```
13. colors.c - colour formatting functions.
14. procll - functions for process linked list.
15. sighan.c - contains signal handlers.
16. gvars.h - contains global variables
17. myutils - utility functions



## Assumptions :

1. Max length of line = 128 characters.
   _Note : line is not the same as command. A line may have multiple commands seperated by a ';'._ 
2. Max. number of words per command = 16
3. Max Address Length = 64
4. Redirection tokens have to be seperated by a space
    eg : sort< text1.txt _(INVALID)_ 
        sort < text1.txt _(VALID)_
5. General format of redirects : 
    CMD1 < FILE | CMD2 | .... | CMDN >> CMDN+1

*Note : There might be some formatting differences from what bash(or similar) shells might display. Please consider this.*
