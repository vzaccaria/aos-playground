CC=gcc 
READELF=readelf 
OBJDUMP=objdump
COPTS?=-fpic

%.x: %.o
	${CC} -g -O3 ${COPTS} $<  -o $@

%.o: %.c
	${CC} -c -g -O3 ${COPTS} -D_FORTIFY_SOURCE=0  -O3 $< -o $@

%.s: %.c
	${CC} -O3 ${COPTS}  -S $< -s 

## Complete info

%.info: %.o
	${READELF} -l $<

%.exinfo: %.x
	${READELF} -l $<

## Headers 

%.header: %.o
	${READELF} -h $<

%.hexheader: %.x
	${READELF} -h $<

## Sections

%.sections: %.o
	${READELF} -S $<

%.exsections: %.x
	${READELF} -S $<

## Relocations (only object files)

%.relocation-source: %.o
	${OBJDUMP} -d -M intel -S $< 

%.relocations: %.o
	${READELF} -r $<

%.symbols: %.o
	${READELF} -s $<

%.exsymbols: %.x
	${READELF} -s $<

## Disassemble final
%.exdis: %.x
	${OBJDUMP} --disassemble=main main.x

%.exvma: %.x
	${OBJDUMP} -h main.x




clean:
	rm -f *.x *.dump *.s
