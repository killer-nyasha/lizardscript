lizard_script:
	cd LizardScriptCore && \
	$(MAKE) fullLib && \
	mv LizardScriptCore.a ../LizardScript/LizardScriptCore.a && \
	cd ../LizardScript && \
	clang++ -c LizardScript.cpp -O3 -std=c++14 && \
	clang++ LizardScript.o LizardScriptCore.a -o LizardScript