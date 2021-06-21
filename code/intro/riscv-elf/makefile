PFX=/opt/riscv/toolchain/bin/riscv64-unknown-elf-

%.x: %.o
	${PFX}gcc $< -o $@

%.o: %.c
	${PFX}gcc -c -O3 $< -o $@

%.s: %.c
	${PFX}gcc -O3 -S $< -s 

## Complete info

%.info: %.o
	${PFX}readelf -l $<

%.exinfo: %.x
	${PFX}readelf -l $<

## Headers 

%.header: %.o
	${PFX}readelf -h $<

%.hexheader: %.x
	${PFX}readelf -h $<

## Sections

%.sections: %.o
	${PFX}readelf -S $<

%.exsections: %.x
	${PFX}readelf -S $<

## Relocations (only object files)

%.relocation-source: %.o
	${PFX}objdump -Sr $< 

%.relocations: %.o
	${PFX}readelf -r $<

%.symbols: %.o
	${PFX}readelf -s $<

%.exsymbols: %.x
	${PFX}readelf -s $<

## Disassemble final
%.exdis: %.x
	${PFX}objdump --disassemble=main main.x

%.exvma: %.x
	${PFX}objdump -h main.x




clean:
	rm -f *.x *.dump *.s
