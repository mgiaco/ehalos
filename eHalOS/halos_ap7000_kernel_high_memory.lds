/* 

AVR32 (AP7000) Linker Skript, modified by mgiaco for HALOS
Version 0.0.1

*/

OUTPUT_FORMAT("elf32-avr32", "elf32-avr32", "elf32-avr32")
OUTPUT_ARCH(avr32:ap)
ENTRY(_start)
SEARCH_DIR("/home/mingwbuild/mingwavr32/avr32/lib");
INPUT(-lm)

MEMORY
  {
	FLASH (rxai)     : ORIGIN = 0x00000000, LENGTH = 5M
	HFLASH (rxai)    : ORIGIN = 0x80000000, LENGTH = 5M /*500000*/
	
		
	CPUSRAM (rwxa)   : ORIGIN = 0x24000000, LENGTH = 32K
	SDRAM   (rwxa)   : ORIGIN = 0x90180000, LENGTH = 3M  /* max 3584K */
								
  }
  
SECTIONS
{
  /* Read-only sections, merged into text segment: */
  PROVIDE (__executable_start = 0xa0000000); . = 0x00000000;
  _start_load_addr = . ;
  .interp         : AT(_start_load_addr)
  { *(.interp) 
  }  >HFLASH
  
  .reset : AT( LOADADDR(.interp) + SIZEOF(.interp) ) {  *(.reset) } >HFLASH
  . = . & 0x9fffffff;
   
  .hash           	: AT( LOADADDR(.reset) + SIZEOF(.reset) )	{ *(.hash) } >HFLASH 
  .dynsym         	: AT( LOADADDR(.hash) + SIZEOF(.hash) )	{ *(.dynsym) } >HFLASH
  .dynstr         	: AT( LOADADDR(.dynsym) + SIZEOF(.dynsym) )	{ *(.dynstr) } >HFLASH
  .gnu.version    	: AT( LOADADDR(.dynstr) + SIZEOF(.dynstr) )	{ *(.gnu.version) } >HFLASH
  .gnu.version_d  	: AT( LOADADDR(.gnu.version) + SIZEOF(.gnu.version) )	{ *(.gnu.version_d) } >HFLASH
  .gnu.version_r  	: AT( LOADADDR(.gnu.version_d) + SIZEOF(.gnu.version_d) )	{ *(.gnu.version_r) } >HFLASH
  .rel.init       	: AT( LOADADDR(.gnu.version_r ) + SIZEOF(.gnu.version_r) )	{ *(.rel.init) } >HFLASH
  .rela.init      	: AT( LOADADDR(.rel.init) + SIZEOF(.rel.init) )	{ *(.rela.init) } >HFLASH
  .rel.text       	: AT( LOADADDR(.rela.init) + SIZEOF(.rela.init) )	{ *(.rel.text .rel.text.* .rel.gnu.linkonce.t.*) } >HFLASH
  .rela.text      	: AT( LOADADDR(.rel.text) + SIZEOF(.rel.text) )	{ *(.rela.text .rela.text.* .rela.gnu.linkonce.t.*) } >HFLASH
  .rel.fini       	: AT( LOADADDR(.rela.text) + SIZEOF(.rela.text) )	{ *(.rel.fini) } >HFLASH
  .rela.fini      	: AT( LOADADDR(.rel.fini) + SIZEOF(.rel.fini) )	{ *(.rela.fini) } >HFLASH
  .rel.rodata     	: AT( LOADADDR(.rela.fini) + SIZEOF(.rela.fini) )  { *(.rel.rodata .rel.rodata.* .rel.gnu.linkonce.r.*) } >HFLASH
  .rela.rodata    	: AT( LOADADDR(.rel.rodata) + SIZEOF(.rel.rodata) )  { *(.rela.rodata .rela.rodata.* .rela.gnu.linkonce.r.*) } >HFLASH 
  .rel.data.rel.ro  : AT( LOADADDR(.rela.rodata) + SIZEOF(.rela.rodata) )  { *(.rel.data.rel.ro*) } >HFLASH
  .rela.data.rel.ro	: AT( LOADADDR(.rel.data.rel.ro) + SIZEOF(.rel.data.rel.ro) )  { *(.rel.data.rel.ro*) } >HFLASH
  .rel.data       	: AT( LOADADDR(.rela.data.rel.ro) + SIZEOF(.rela.data.rel.ro) )  { *(.rel.data .rel.data.* .rel.gnu.linkonce.d.*) } >HFLASH 
  .rela.data      	: AT( LOADADDR(.rel.data) + SIZEOF(.rel.data) )  { *(.rela.data .rela.data.* .rela.gnu.linkonce.d.*) } >HFLASH
  .rel.tdata	  	: AT( LOADADDR(.rela.data) + SIZEOF(.rela.data) )  { *(.rel.tdata .rel.tdata.* .rel.gnu.linkonce.td.*) } >HFLASH
  .rela.tdata	  	: AT( LOADADDR(.rel.tdata) + SIZEOF(.rel.tdata) )  { *(.rela.tdata .rela.tdata.* .rela.gnu.linkonce.td.*) } >HFLASH
  .rel.tbss	  		: AT( LOADADDR(.rela.tdata) + SIZEOF(.rela.tdata) )  { *(.rel.tbss .rel.tbss.* .rel.gnu.linkonce.tb.*) } >HFLASH 
  .rela.tbss	  	: AT( LOADADDR(.rel.tbss) + SIZEOF(.rel.tbss) )  { *(.rela.tbss .rela.tbss.* .rela.gnu.linkonce.tb.*) } >HFLASH 
  .rel.ctors      	: AT( LOADADDR(.rela.tbss) + SIZEOF(.rela.tbss) )  { *(.rel.ctors) } >HFLASH 
  .rela.ctors     	: AT( LOADADDR(.rel.ctors) + SIZEOF(.rel.ctors) )  { *(.rela.ctors) } >HFLASH
  .rel.dtors      	: AT( LOADADDR(.rela.ctors) + SIZEOF(.rela.ctors) )  { *(.rel.dtors) } >HFLASH 
  .rela.dtors     	: AT( LOADADDR(.rel.dtors) + SIZEOF(.rel.dtors) )  { *(.rela.dtors) } >HFLASH
  .rel.got        	: AT( LOADADDR(.rela.dtors) + SIZEOF(.rela.dtors) )  { *(.rel.got) } >HFLASH 
  .rela.got       	: AT( LOADADDR(.rel.got) + SIZEOF(.rel.got) )  { *(.rela.got) } >HFLASH
  .rel.bss        	: AT( LOADADDR(.rela.got) + SIZEOF(.rela.got) )  { *(.rel.bss .rel.bss.* .rel.gnu.linkonce.b.*) } >HFLASH
  .rela.bss       	: AT( LOADADDR(.rel.bss) + SIZEOF(.rel.bss) )  { *(.rela.bss .rela.bss.* .rela.gnu.linkonce.b.*) } >HFLASH 
  .rel.plt        	: AT( LOADADDR(.rela.bss) + SIZEOF(.rela.bss) )  { *(.rel.plt) } >HFLASH 
  .rela.plt       	: AT( LOADADDR(.rel.plt) + SIZEOF(.rel.plt) )  { *(.rela.plt) } >HFLASH
  .init           : AT( LOADADDR(.rela.plt) + SIZEOF(.rela.plt) )
  {
    KEEP (*(.init))
  } >HFLASH =0xd703d703
  .plt            : AT( LOADADDR(.init) + SIZEOF(.init) ) { *(.plt) } >HFLASH
 
  .text           : AT( LOADADDR(.plt) + SIZEOF(.plt) )
  {
    *(.text .stub .text.* .gnu.linkonce.t.*)
    KEEP (*(.text.*personality*))
    /* .gnu.warning sections are handled specially by elf32.em.  */
    *(.gnu.warning)
  } >HFLASH =0xd703d703  
  
  .fini           : AT( LOADADDR(.text) + SIZEOF(.text) )
  {
    KEEP (*(.fini))
  } >HFLASH =0xd703d703
  
  PROVIDE (__etext = .);
  PROVIDE (_etext = .);
  PROVIDE (etext = .);
  .rodata         : AT( LOADADDR(.fini) + SIZEOF(.fini) + 0x2) 
  { 
  	*(.rodata .rodata.* .gnu.linkonce.r.*) 
  } >HFLASH
  
  .rodata1        : AT( LOADADDR(.rodata) + SIZEOF(.rodata) ) { *(.rodata1) } >HFLASH
  .eh_frame_hdr : AT( LOADADDR(.rodata1) + SIZEOF(.rodata1) ) { *(.eh_frame_hdr) } >HFLASH
  .eh_frame       : AT( LOADADDR(.eh_frame_hdr) + SIZEOF(.eh_frame_hdr) ) ONLY_IF_RO 
  { 
  	KEEP (*(.eh_frame)) 
  } >HFLASH
  
  /* _lma_offset = */
  /* tmp = LOADADDR(.eh_frame); */
  _loadaddress_gcc = ALIGN( (LOADADDR(.eh_frame)+ SIZEOF(.eh_frame)),0x200);
  
  .gcc_except_table  : AT( _loadaddress_gcc ) ONLY_IF_RO
  {
  	/* tmp = LOADADDR(.eh_frame); */
    /* . = ALIGN(0x1000); */
  	KEEP (*(.gcc_except_table)) *(.gcc_except_table.*) 
  } >HFLASH
  
  .dalign	: AT( LOADADDR(.gcc_except_table) + SIZEOF(.gcc_except_table) ) 
  { 
  	. = ALIGN(8);
  	PROVIDE(_data_lma = .);
  	/* _data_lma = LOADADDR (.dalign); */
  } >HFLASH
  
  PROVIDE (_data = ORIGIN(SDRAM));
  . = ORIGIN(SDRAM);
  /* Exception handling  */
  .eh_frame       : AT( LOADADDR(.dalign) + SIZEOF(.dalign) ) ONLY_IF_RW { KEEP (*(.eh_frame)) } >SDRAM
  .gcc_except_table   : AT( LOADADDR(.eh_frame) + SIZEOF(.eh_frame) ) ONLY_IF_RW 
  { 
  	KEEP (*(.gcc_except_table)) *(.gcc_except_table.*) 
  } >SDRAM
    
  /* Thread Local Storage sections  */
  .tdata	  : AT( LOADADDR(.gcc_except_table) + SIZEOF(.gcc_except_table) ) { *(.tdata .tdata.* .gnu.linkonce.td.*) } >SDRAM
  .tbss		  : AT( LOADADDR(.tdata) + SIZEOF(.tdata) ) { *(.tbss .tbss.* .gnu.linkonce.tb.*) *(.tcommon) } >SDRAM
  /* Ensure the __preinit_array_start label is properly aligned.  We
     could instead move the label definition inside the section, but
     the linker would then create the section even if it turns out to
     be empty, which isn't pretty.  */
  PROVIDE (__preinit_array_start = ALIGN(32 / 8));
  .preinit_array     : AT( LOADADDR(.tbss) + SIZEOF(.tbss) ) { KEEP (*(.preinit_array)) } >SDRAM
  PROVIDE (__preinit_array_end = .);
  PROVIDE (__init_array_start = .);
  .init_array     : AT( LOADADDR(.preinit_array) + SIZEOF(.preinit_array) ) { KEEP (*(.init_array)) } >SDRAM
  PROVIDE (__init_array_end = .);
  PROVIDE (__fini_array_start = .);
  .fini_array     : AT( LOADADDR(.init_array) + SIZEOF(.init_array) ) { KEEP (*(.fini_array)) } >SDRAM
  PROVIDE (__fini_array_end = .);
  .ctors          : AT( LOADADDR(.fini_array) + SIZEOF(.fini_array) ) 
  {
    /* gcc uses crtbegin.o to find the start of
       the constructors, so we make sure it is
       first.  Because this is a wildcard, it
       doesn't matter if the user does not
       actually link against crtbegin.o; the
       linker won't look for a file to match a
       wildcard.  The wildcard also means that it
       doesn't matter which directory crtbegin.o
       is in.  */
    KEEP (*crtbegin*.o(.ctors))
    /* We don't want to include the .ctor section from
       from the crtend.o file until after the sorted ctors.
       The .ctor section from the crtend file contains the
       end of ctors marker and it must be last */
    KEEP (*(EXCLUDE_FILE (*crtend*.o ) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
  } >SDRAM
  .dtors          : AT( LOADADDR(.ctors) + SIZEOF(.ctors) ) 
  {
    KEEP (*crtbegin*.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend*.o ) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
  } >SDRAM
  .jcr            : AT( LOADADDR(.dtors) + SIZEOF(.dtors) )  { KEEP (*(.jcr)) } >SDRAM
  .data.rel.ro    : AT( LOADADDR(.jcr) + SIZEOF(.jcr) )  { *(.data.rel.ro.local) *(.data.rel.ro*) } >SDRAM
  .dynamic        : AT( LOADADDR(.data.rel.ro) + SIZEOF(.data.rel.ro) )  { *(.dynamic) } >SDRAM
  .got            : AT( LOADADDR(.dynamic) + SIZEOF(.dynamic) )  { *(.got.plt) *(.got) } >SDRAM
  .data           : AT( LOADADDR(.got) + SIZEOF(.got) ) 
  {
    *(.data .data.* .gnu.linkonce.d.*)
    KEEP (*(.gnu.linkonce.d.*personality*))
    SORT(CONSTRUCTORS)
  } >SDRAM
  
  .data1	: AT( LOADADDR(.data) + SIZEOF(.data) )  { *(.data1) } >SDRAM
  .balign	: AT( LOADADDR(.data1) + SIZEOF(.data1) )  { . = ALIGN(8); _edata = .; } >SDRAM
  _edata = .;
  PROVIDE (edata = .);
  __bss_start = .;
  .bss      : AT( LOADADDR(.balign) + SIZEOF(.balign) )
  {
   *(.dynbss)
   *(.bss .bss.* .gnu.linkonce.b.*)
   *(COMMON)
   /* Align here to ensure that the .bss section occupies space up to
      _end.  Align after .bss to ensure correct alignment even if the
      .bss section disappears because there are no input sections.  */
   . = ALIGN(8);
  } >SDRAM
  
  . = ALIGN(8);
  
  _end = .;
  PROVIDE (end = .); 
  __heap_start__ = ALIGN(8);
  . = ORIGIN(SDRAM) + LENGTH(SDRAM) - 0x1000;
  __heap_end__ = .;
  
  .hstack	ORIGIN(SDRAM) + LENGTH(SDRAM) - 0x1000 : AT( LOADADDR(.bss) + SIZEOF(.bss) )
  {
    _hstack = .;
    *(.stack)
    . = 0x1000;
    _hestack = .;
  } >SDRAM

  .stack	ORIGIN(CPUSRAM) + LENGTH(CPUSRAM) - 0x1000 : AT( LOADADDR(.hstack) + SIZEOF(.hstack) )
  {
    _stack = .;
    *(.stack)
    . = 0x1000;
    _estack = .;
  } >CPUSRAM
  
  /*
  .proc : 
  {
  	*(.processes)
  } >FLASH2 AT>FLASH2
  */
  
  /* Stabs debugging sections.  */
  .stab          0 : { *(.stab) }
  .stabstr       0 : { *(.stabstr) }
  .stab.excl     0 : { *(.stab.excl) }
  .stab.exclstr  0 : { *(.stab.exclstr) }
  .stab.index    0 : { *(.stab.index) }
  .stab.indexstr 0 : { *(.stab.indexstr) }
  .comment       0 : { *(.comment) }
  /* DWARF debug sections.
     Symbols in the DWARF debugging sections are relative to the beginning
     of the section so we begin them at 0.  */
  /* DWARF 1 */
  .debug          0 : { *(.debug) }
  .line           0 : { *(.line) }
  /* GNU DWARF 1 extensions */
  .debug_srcinfo  0 : { *(.debug_srcinfo) }
  .debug_sfnames  0 : { *(.debug_sfnames) }
  /* DWARF 1.1 and DWARF 2 */
  .debug_aranges  0 : { *(.debug_aranges) }
  .debug_pubnames 0 : { *(.debug_pubnames) }
  /* DWARF 2 */
  .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
  .debug_abbrev   0 : { *(.debug_abbrev) }
  .debug_line     0 : { *(.debug_line) }
  .debug_frame    0 : { *(.debug_frame) }
  .debug_str      0 : { *(.debug_str) }
  .debug_loc      0 : { *(.debug_loc) }
  .debug_macinfo  0 : { *(.debug_macinfo) }
  /* SGI/MIPS DWARF 2 extensions */
  .debug_weaknames 0 : { *(.debug_weaknames) }
  .debug_funcnames 0 : { *(.debug_funcnames) }
  .debug_typenames 0 : { *(.debug_typenames) }
  .debug_varnames  0 : { *(.debug_varnames) }
  
  /DISCARD/ : { *(.note.GNU-stack) }
      
}
