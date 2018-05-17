all:
	if [ ! -d "bin" ] ; then mkdir bin; fi ; cd src; make 

clean:
	rm -rf bin
