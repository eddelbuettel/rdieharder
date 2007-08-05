
# this works on Debian but I probably need to add proper configure test...
PKG_CFLAGS =	-W -I. -DRDieHarder `gsl-config --cflags`
PKG_LIBS =	-ldieharder `gsl-config --libs`

sources := 	$(wildcard *.c)

objects :=	$(sources:%.c=%.o)

all :		RDieHarder.so

RDieHarder.so :	$(objects)
		$(CC) -shared -o $@ $(objects) $(PKG_LIBS) 

clean :
		rm -f $(objects) RDieHarder.so 
