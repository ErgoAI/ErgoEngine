@echo OFF

@set jenadir=%~dp0\..\..
@set ergodir=%~dp0\..\..\..\..

REM java -cp .;"%jenadir%\lib\*";"%ergodir%\ergo_lib\ergo2owl\java\ergoOWL.jar" ErgoOWL_GUI %HOME%
java -cp .;"%jenadir%\JenaAllInOne.jar";"%ergodir%\ergo_lib\ergo2java\java\ergoStudio.jar";"%ergodir%\ergo_lib\ergo2owl\java\ergoOWL.jar" com.coherentknowledge.ergo.owl.gui.ErgoOWL_GUI %HOME%
