#ifndef RAM_H
#define RAM_H

/* see main.c for definition */

#ifndef RAM_FAST

uint8* _RamAddr(uint16 address) {
	if (!orion128) return &mem[address];
	return dispatcher && (address < 0x4000) ? &mem16[address] : 
	 		( (address < 0xf000) || fullram ? &mem[address] : &mem1024[address]);  
}
/*
uint8 _RamRead(uint16 address) {
	return *_RamAddr(address); 
}
*/
#define _RamRead(x) (*_RamAddr(x)) 

void _RamWrite(uint16 address, uint8 value) {
	if ((! orion128) || fullram || (address < 0xf800))
		*_RamAddr(address)=value;
}
#endif

#endif
