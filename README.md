
# File Locking and Loadable Modules

In this assignment we will write a simple system that manages data in a record based format, protecting reads and writes to the records using locks.

# Overview

We want to explore both full file and region based locking.  We will implement our two strategies in two separate loadable modules so that we can explore these strategies.


# Tasks

Some starter code is provided for you, which should allow you to begin development on Linux.

You will need to complete the code so that you can perform either whole-file or record-based locking by choosing a shared library, and in addition complete the work so that it also compiles and runs on Windows.


# Code provided

Here is a description of the code you have been provided.

In the main directory are these files:

* `rm-db.c` -- this code manages the reading and writing to the database via user supplied command on standard input.  This code should able to be used as-is
* `rm-lockwrapper.c` -- this code wraps up all the calls that require the shared library, including the opening and closing of the database, and the obtaining and releasing of locks.  You will need to do the following:
	* implement the loading and unloading of the specified lock module (library)
	* implement the call to open the database using the specific code in the shared library for the type of locking desired
	* implement the call to close the database, also using the shared library
	* implement the code to call the shared library implementation of the locking and unlocking code
* `rm-main.c` -- this code parses the command line and calls for the database operations to be performed.

* `rm-main.c` -- this code parses the command line and calls for the database operations to be performed.

* `include\os_defs.h` -- contains definitions to detect the various operating systems
* `include\os_libsymbols.h` -- contains OS-specific definitions to allow the use of shared libraries with the appropriate "decorator" material for use on Windows, as we discussed in class.

## Shared library sub directories

In the subdirectory `srcLockRegion` there are `Makefile`s for each platform as well as a starter C code file:

* `lockRegion.c` -- this file should contain all the code to open and close the database and gain and release locks using region based locking.  The actual names of the functions to perform this (that is, the API) are up to you.

In the subdirectory `srcLockFile` you are asked to create a second shared library that will perform *file based* locking instead of *region based* locking.

Both of the shared library sub directories should produce a shared library and place it in the `lib` subdirectory.


## Database user interface

The user commands to read/write the database are as follows:

* `W` _<ID>_ _<DATA>_
	* writes the string of _<DATA>_ into the database at record location _<ID>_.  Data will be truncated or padded to fit the record size
* `R` _<ID>_ _<DATA>_
	* reads the data at record location _<ID>_.
* `X`
	* exits the system

A sleep will be performed after each lock to allow you to try running commands with another `recordManager` program in another window.

## Running the `recordManager` program

To run the `recordManager` program, supply the filename of a loadable lock module, the name of a database file (which will be created if it does not exist) and the number of seconds to sleep before executing each command.

For example:

	./recordManager lib/libLockRegion.so mydata.db 3
	

This will place records in a file called `mydata.db` using the locking protocol supplied in `lib/libLockRegion.so` and sleep for three seconds before each command.


# Shared library lock implementations

The advantage of placing the locking code into a shared library is that at runtime a different strategy can be loaded by different instances of your program.

Try running the program multiple times from different windows, and trying reading/writing the records.

# Individual work

Use the provided code to get started.

The starter code is again provided to you as a [`git(1)`](https://man7.org/linux/man-pages/man1/git.1.html) repository:

```
	git clone "git@gitlab.socs.uoguelph.ca:3050F24/${USER}/A4.git" .
```

Use the examples in the course directory to get started.

All other submitted code is expected to be your own work, written specifically for this assignment, and any code included from elsewhere (such as the examples given) **must be noted and their source cited**.  All parts of your program should be written directly by you.


# Submission

As with A1, please submit by running the `handItIn` script.

Be sure to validate your submission by cloning your git repo to a new location and ensuring that all files you wish to submit are included.

