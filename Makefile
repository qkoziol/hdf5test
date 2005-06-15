.PHONY:	doc bin

doc:
	cd src && $(MAKE) doc

bin:
	if [ ! -d bin ]; then mkdir bin; fi; \
	cd src && $(MAKE) bin

clean:
	cd src && $(MAKE) clean
	cd doc && $(MAKE) clean
	cd bin && $(MAKE) clean
