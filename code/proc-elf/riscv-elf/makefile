include ../riscv.mk

%.x: %.o
	${CC} $< -mno-relax -o $@

%.o: %.c
	${CC} -c -O3 $< -o $@

%.s: %.c
	${CC} -O3 -S $< -s 

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
	${OBJDUMP} -Sr $< 

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
