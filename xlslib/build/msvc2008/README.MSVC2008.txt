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

- xlslib

[TODO: download URLs]


4.  Layout of the source tree
-----------------------------

The following directory layout for xlslib and the additional libraries is
recommended for achieving a hassle-free build. Note that the current project
configuration includes these libraries in the executable, thus creating a
'statically linked' executable.

You may of course change your setup to use the DLL build versions of the
libraries, but be aware that the 'YMMV' caveat applies as that configuration
has not been tested with the latest xlslib build(s).

[TODO: dirtree]


5.  Matching MSVC2008 project files for these libraries
-------------------------------------------------------

The libraries listed above do not come with suitable MSVC2008 project files,
but we provide these with xlslib for your convenience. Note that these
project files assume the libraries listed above are installed according to
the 'Layout of the source tree' described above.

The necessary library project files are available in the
./build/msvc2008/ directory.

[TODO: dirtree / file list?]



[Re]building xlslib on Win32 using MSVC2008/SP1
==============================================

Simply load the MSVC2008 solution file './build/msvc2008/xlslib.sln' and
choose 'Build' or 'Rebuild All', depending on your preference.

After a successful build you should have a fresh 'xlslib.exe' executable in
the ./build/msvc2008/bin/ directory.

Note that the bin/ directory mentioned here may also include several DLL
and/or other EXE files, which were constructed while building the libraries
listed above. These are not mandatory for running xlslib.exe though (unless
of course you edited your project to use the dynamic libraries (DLLs)
instead) and are meant for testing the libraries as you wish (retest.exe is
for testing the 'tre' library, 'ia_ssl_win32.exe' is for testing the OpenSSL
libary).



Testing the build
=================

You might want to read the man page before running xlslib. A few example
batch files (*.BAT) of xlslib use are included in the ./tests/ directory.

!!! WARNING !!!

Please note that none of those sites have granted permission to be used as
'test sites' so you may be IP-locked or even worse when abusing these.
Merely consider these test files as examples of use and change the site URLs
to your own before commencing.

!!! WARNING !!!



Support
=======

This port comes as-is with no warranty, express nor implied.

If you require support, make sure you have read all documentation [files]
available, before asking your question in the xlslib mailing list. When you
do, make sure you SPECIFICALLY state in your SUBJECT LINE that your issue is
in regard to the Win32 MSVC2008 build, for example by prefixing your subject
line with a '[Win32-MSVC2008]' tag like this example:

  [Win32-MSVC2008] Has anyone successfully built xlslib with the MSVC2K8 Express edition?

(Answer: not at the moment of this writing.)

