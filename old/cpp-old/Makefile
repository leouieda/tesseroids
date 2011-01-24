# clean é pra limpar
CCMP = gcc
CPPCMP = g++
BINPATH = ../bin

all: tess divmod

tess: pot gx gy gz gxx gyy gzz gxy gxz gyz

pot:
	${CPPCMP} -Wall -ansi -g2 tesspot.cpp -o ${BINPATH}/tesspot
	
gz:
	${CPPCMP} -Wall -ansi -g2 tessgz.cpp -o ${BINPATH}/tessgz
    
gx:
	${CPPCMP} -Wall -ansi -g2 tessgx.cpp -o ${BINPATH}/tessgx

gy:
	${CPPCMP} -Wall -ansi -g2 tessgy.cpp -o ${BINPATH}/tessgy

gzz:
	${CPPCMP} -Wall -ansi -g2 tessgzz.cpp -o ${BINPATH}/tessgzz
    
gxx:
	${CPPCMP} -Wall -ansi -g2 tessgxx.cpp -o ${BINPATH}/tessgxx

gyy:
	${CPPCMP} -Wall -ansi -g2 tessgyy.cpp -o ${BINPATH}/tessgyy

gxy:
	${CPPCMP} -Wall -ansi -g2 tessgxy.cpp -o ${BINPATH}/tessgxy
	
gxz:
	${CPPCMP} -Wall -ansi -g2 tessgxz.cpp -o ${BINPATH}/tessgxz
	
gyz:
	${CPPCMP} -Wall -ansi -g2 tessgyz.cpp -o ${BINPATH}/tessgyz
	
divmod:
	${CPPCMP} -Wall -ansi -g2 divmod.cpp -o ${BINPATH}/divmod

clean:
	rm ${BINPATH}/*
