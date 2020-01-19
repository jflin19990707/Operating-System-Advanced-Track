#include<string.h>
volatile unsigned long* UART0DR = (unsigned long*)0xa01f1000;
#define MMU_FULL_ACCESS     (3 << 10)   
#define MMU_DOMAIN          (0 << 5)   
#define MMU_SPECIAL         (1 << 4)  
#define MMU_CACHEABLE       (1 << 3) 
#define MMU_BUFFERABLE      (1 << 2)
#define MMU_SECTION         (2)    
#define MMU_SECDESC         (MMU_FULL_ACCESS | MMU_DOMAIN | MMU_SPECIAL | \
                             MMU_SECTION)
#define MMU_SECDESC_WB      (MMU_FULL_ACCESS | MMU_DOMAIN | MMU_SPECIAL | \
                                                          MMU_CACHEABLE | MMU_BUFFERABLE | MMU_SECTION)
#define MMU_SECTION_SIZE    0x00100000

unsigned long *mmu_tlb_base=(unsigned long*)0x00004000;
inline void mmu_enable();

void mmu_init(){

#define TLB_SET(X,Y) *(mmu_tlb_base + ( ((unsigned long)(X))>>20)) \
			=( ( ((unsigned long)(Y)) &0xfff00000)|MMU_SECDESC_WB)
#define TLB_BASE(x) __asm__ ("MRC p15,0,%0,c2,c0,0"::"r"(x))

	TLB_SET(0x00000000,0x00000000);
	TLB_SET(0x00100000,0x00100000);
	TLB_SET(0x00100000,0x00100000);
	TLB_SET(0xc0100000,0x00100000);
	TLB_SET(0xc0200000,0x00200000);
	
/*	TLB_SET(0x101f1000,0x101f1000);
	TLB_SET(0xc0008000,0x00008000);
	TLB_SET(0xc0100000,0x00100000);
	TLB_SET(0xc0200000,0x00200000);
	TLB_SET(0xc0300000,0x00300000);
*/
	TLB_SET(0xA01f1000,0x101f1000);
	//TLB_SET(0xB01f1000,0x101f1000);
	TLB_BASE(0x4000);
	
		mmu_enable();
	
}

inline void mmu_enable(){ 
     unsigned long ttb = 0x4000;
 
 __asm__(
     "mov    r0, #0\n"
     "mcr    p15, 0, r0, c7, c7, 0\n"    //clear c7
     "mcr    p15, 0, r0, c7, c10, 4\n"   
     "mcr    p15, 0, r0, c8, c7, 0\n"   
 
     "mov    r4, %0\n"                   
     "mcr    p15, 0, r4, c2, c0, 0\n"   //c2 is ttb base 
 
     "mvn    r0, #0\n"                  
     "mcr    p15, 0, r0, c3, c0, 0\n"   //clear c3
     "mrc    p15, 0, r0, c1, c0, 0\n"    //reset c1
 
     "bic    r0, r0, #0x3000\n"          
     "bic    r0, r0, #0x0300\n"          
     "bic    r0, r0, #0x0087\n"          
  
     "orr    r0, r0, #0x0002\n"          
     "orr    r0, r0, #0x0004\n"          
     "orr    r0, r0, #0x1000\n"          
     "orr    r0, r0, #0x0001\n"          
 
    
     "mcr    p15, 0, r0, c1, c0, 0\n"    
     : 
     : "r" (ttb) 
     );
 
}

void u_prints(char *str){
	while(*str)
	*UART0DR = *str++;
}
inline void c_put(unsigned int c){
	
	*UART0DR = c;
}

inline void i_put(unsigned int i){
	if(i<10)
		c_put(i%10+'0');
	else
		c_put(i%10+'A');
	
}
void h_put(unsigned int i){

	int j;
	u_prints("0x");

	for(j=0;j<8;j++,i<<=4)
		i_put(i>>28);

}
	
void c_entry(void){

	u_prints("hello,versatilepb\n");

}
void c_strmove(char * addr, char *dest,int length){

	
	while( length-- > 0){
	*dest++ = *addr++;
	}

}
void setup_arch(void){



}
//extern int fcpp(void);
void __logo(void){

	int value;

	u_prints("The real Core for ARM\n");
	__asm("mov %0,pc\n\t":"=r"(value));
	h_put(value);
	c_entry();
	//h_put(fcpp());
}
void u_prints2(char *str, unsigned long* addr){
	while(*str)
	*addr = *str++;
}
void main(void){

	extern char __lds_kernel_start;
	extern char __lds_kernel_end;
	extern char  __lds_kernel_size;
	mmu_init();
	u_prints("start:\n");
	h_put(&__lds_kernel_start);
	u_prints("end:\n");
	h_put(&__lds_kernel_end);
	u_prints("size:\n");
	h_put(&__lds_kernel_size);
	__logo();
        u_prints("YOU ARE WIN!");
	while(1);

}

