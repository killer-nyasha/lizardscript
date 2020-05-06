SHELL = /bin/sh

scriptArtifacts:
	python3.7 opcodes_from_switch.py
	python3.7 new_meta.py

lib:
	clang++ -c *.cpp *.cxx -O3 -std=c++14
	ar rs LizardScriptCore.a *.o

fullLib:
	$(MAKE) scriptArtifacts
	$(MAKE) lib