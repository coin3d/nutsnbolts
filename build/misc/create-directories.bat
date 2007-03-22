@echo off

pushd %COINDIR%

if exist bin\*.* goto binexists
echo mkdir %COINDIR%\bin
mkdir bin
:binexists
if exist lib\*.* goto libexists
echo mkdir %COINDIR%\lib
mkdir lib
:libexists
if exist include\*.* goto includeexists
echo mkdir %COINDIR%\include
mkdir include
:includeexists
chdir include
if exist NutsnBolts\*.* goto nutsnboltsexists
echo mkdir %COINDIR%\include\NutsnBolts
mkdir NutsnBolts
:nutsnboltsexists
chdir NutsnBolts
if exist misc\*.* goto miscexists
echo mkdir %COINDIR%\include\NutsnBolts\misc
mkdir misc
:miscexists
if exist navigation\*.* goto navigationexists
echo mkdir %COINDIR%\include\NutsnBolts\navigation
mkdir navigation
:navigationexists
if exist nodes\*.* goto nodesexists
echo mkdir %COINDIR%\include\NutsnBolts\nodes
mkdir nodes
:nodesexists

popd

