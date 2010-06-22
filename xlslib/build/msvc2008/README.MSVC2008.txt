Notes for the MSVC2008 build/release
====================================

Requirements
============

1.  Compiler
------------

The solution and project files assume you have Microsoft Visual Studio 2008
installed, including Service Pack 1.


2.  DOS formatted solution and project files
-------------------------------------------

WARNING: the source distribution may have converted the MSVC solution and
project files to UNIX line termination format. MSVC2008 will FAIL to load
the solution and/or project files, when these are not formatted with DOS
line termination (a.k.a. CRLF format; UNIX is LF-only).

You may use UltraEdit32 (shareware), Notepad++ (also available at
Sourceforge) or any other text file editor capable of converting 'UNIX fomat
text files' to 'DOS format text files' to convert the solution (.sln) and
project (.vcproj) files back to DOS format before loading them into
MSVC2008.


3.  Additional libraries
------------------------

In order to successfully compile xlslib on Win32 you need the following
source libraries:

[none]



[Re]building xlslib on Win32 using MSVC2008/SP1
==============================================

Simply load the MSVC2008 solution file './build/msvc2008/xlslib.sln' and
choose 'Build' or 'Rebuild All', depending on your preference.

After a successful build you should have a fresh 'xlslib_dll.dll' DLL and 'xlslib_lib.lib' library plus a few test/sample applications in
the ./build/msvc2008/bin/ directory.

Note that the bin/ directory mentioned here may also include several DLL
and/or other EXE files, which were constructed while building the libraries
listed above. These are not mandatory for using the xlslib library or DLL itself though.
These samples are meant for testing the xlslib library as you wish.



Testing the build
=================

Source code for the sample/test applications can be found in the targets/test/ subdirectory.



Support
=======

This port comes as-is with no warranty, express nor implied.

If you require support, make sure you have read all documentation [files]
available, before asking your question in the xlslib forum. When you
do, make sure you SPECIFICALLY state in your SUBJECT LINE that your issue is
in regard to the Win32 MSVC2008 build, for example by prefixing your subject
line with a '[Win32-MSVC2008]' tag like this example:

  [Win32-MSVC2008] Has anyone successfully built xlslib with the MSVC2K8 Express edition?

(Answer: not at the moment of this writing.)

