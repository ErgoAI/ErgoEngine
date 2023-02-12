@echo OFF

@set jenadir=%~dp0\..\..
@set ergodir=%~dp0\..\..\..\..

REM java -cp .;"%jenadir%\lib\*";"%ergodir%\ergo_lib\ergo2sparql\java\ergoSPARQL.jar" ErgoSPARQL_GUI %HOME%
java -cp .;"%jenadir%\JenaAllInOne.jar";"%ergodir%\ergo_lib\ergo2java\java\ergoStudio.jar";"%ergodir%\ergo_lib\ergo2sparql\java\ergoSPARQL.jar" com.coherentknowledge.ergo.sparql.gui.ErgoSPARQL_GUI %HOME%
