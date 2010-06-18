rem
rem create distribution files for www.hebbut.net
rem
                          
set name=CreateDLL-1.2.6.i_a
                                                       
rmdir /q /s  .\distrib
mkdir .\distrib

pushd .\distrib

del *.bak
del *.exe 
del *.tar
del *.bz2

popd

rem create 7z files for the source distros, etc.

7z a -r -ssw -scsWIN -y -x@create-distribs.CreateDLL.exclude -x@create-distribs.CreateDLL.src.exclude .\distrib\%name%.full-src.7z CreateDLL.* OptionParser.* create-distribs.CreateDLL.*
7z a -r -ssw -scsWIN -y -x@create-distribs.CreateDLL.exclude -x@create-distribs.CreateDLL.exe.exclude .\distrib\%name%.bin-win32.7z CreateDLL.* OptionParser.*
                                                          
                                                          
