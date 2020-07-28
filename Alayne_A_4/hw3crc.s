//Assembly program Alayne Anderson
/* PROGRAM DESCRIPTION: This program file is the C code converted into assembly program of the file crc.c, it which generates a table of (the last 2 digits) of 
 hexidecimal, dependant on the message you feed the command line when you make and execute hw3a1, it will take each character of the message and generate the
  hexidecimal CRC address on the table. More information can be found on the pdf file program assignment. 
 
  CRC is computed as the polynomial division of the Message M divided by the CRC Polynomial C. The checksum takes the form of the reminder R resulting from dividing M/C * There are many variants based on different polynomials. The length of the resulting checksum depends on the length of the CRC Polynomial.
  CRC-8 implementation will be based on the fact that the number of residues for a given CRC polynomial is fixed. So we can precompute every possible residue and use 
  that table to compute the CRC for a given message. For the case of CRC-8 we have 256 possible residues, so the size of the precomputed CRC table is 256 bytes.
 */

.global crcInit
.global crcFast
.global crcTable
					/*  REGISTER TABLE 		*/
					/*==============================*/	
					/*  char remainder = { %rdi }   */
					/*  char bit = { %rsi }  	*/
					/*  int dividend = { %rdx }  	*/
					/*  char crcTable = { %rcx } 	*/

.data

crcTable:				/*char crcTable[256] = {0}; */
	.rept	256				/*INTIALIZE EVERY INDEX TO 0*/
	.byte 0
	.endr
	
.text

/*  FUNCTION 1  */
crcInit:

.L1:				
	leaq crcTable, %rcx		/*crcTable -> %rcx 			*/	
	mov $0, %rdi			/*remainder -> %rdi 			*/
	mov $8, %rsi			/*for(bit = 8; ~~~~ ; ~~~~)    		*/	
	mov $0, %rdx			/*for(dividend = 0; ~~~~~~~ ; ~~~~~~)   */ 
	
.outerfor:					 	
	mov %rdx, %rdi			/*remainder = dividend 			*/	

	jmp .innerfor				/*enter nested for loop		*/

.innerfor:
	cmp $0, %rsi			/*for(~~~~~ ; bit > 0 ; ~~~~) 		*/ 
	jg .if						/*jump to the if label if %rsi (bit) is greater than 0			*/
	
.if:
	test $0x80, %rdi		/*if(remainder & 0x80)			*/				
	je .else					/*IF NO BITS IN COMMON  */					

	sal %rdi			/*remainder = (remainder << 1) ~~~~ 	*/
	xor $0xD5, %rdi			/*remainder = remainder ^ POLYNOMIAL 	*/

	jmp .innerforcheck				/*after argument jump to done2 to decrement the for2 loop value		*/

.else:
	sal %rdi			/*remainder = (remainder << 1) 		*/

	jmp .innerforcheck				/*after argument jump to done2 to decrement the for2 loop value		*/

.innerforcheck:
	dec %rsi			/*for(~~~~~ ; ~~~~~ ; --bit) 		*/	
	
	cmp $0, %rsi			/*for(~~~~~ ; bit > 0 ; ~~~~) 		*/ 
	jg .innerfor					/*if the value is still greater than 0 then loop the inner for loop again	*/
	
	jmp .outerforcheck				/*jump back to the outer for loop if %rsi (bit) is no longer greater than 0 0	*/ 

.outerforcheck:				
	mov $8, %rsi			/*for(bit = 8; ~~~~ ; ~~~~)    [RESET INNER FOR LOOP value] 				*/	

	leaq (%rcx, %rdx,1), %r10	
					/*crcTable[dividend] = ~~~~~~ 		*/	
	movb %dil, (%r10)		/*crcTable[dividend] = remainder 	*/	
	
	inc %rdx			/*for( ~~~~ ; ~~~~~~ ; ++dividend) 	*/
	cmp $255, %rdx			/*for(~~~ ; dividend < 256; ~~~~) 	*/
	jle .outerfor					/*jump back to for loop 1 if the dividend (%rax) is still less than 255 	*/
	
	jmp .finished					/*if this condition is false then we are finished and can return to main.c 	*/

.finished:
	
	ret


/*  FUNCTION 2  */
crcFast:			

					/*REGISTER TABLE 			*/
					/*======================================*/	
					/*unsigned char data = { %rbx }  	*/
					/*char remainder = { %rax } 		*/
					/*long byte = { %r12 } 			*/
					/*char * message = { %r8 } 		*/
					/*long nBytes = { %r9 }			*/ 
					/*message[byte] = { %r11 }		*/
					/*crcTable[data] = { %r13 }		*/

.L3:		
	leaq crcTable, %rcx 		/*crcTable -> %rcx 			*/	
	movq $0, %r12			/*for(long byte = 0; ~~~~ ; ~~~~) 	*/
	movq $0, %rax			/*remainder = 0				*/
	movq $0, %rbx			/*data = 0				*/


.forfast:		
	
	leaq (%rdi, %r12,1), %r11		/* ~~~~ = message[byte] ~~~~~~					*/	
	mov %rax, %r14  			/*move remainder into separate register -> %r14			*/

	movb (%r11), %bl			/*data = message[byte] */ /*bl is same reg, smaller slice	*/
	xorb %r14b, %bl 			/*data = data^remainder 					*/

	leaq (%rcx, %rbx,1), %r13		/*~~~~ = crcTable[data]						*/		
	movb (%r13), %al			/*remainder = crcTable[data]					*/ 	
	
.forfastcheck:	
	
	inc %r12			/*for(~~~~; ~~~~ ; ++byte)		*/
	cmp %rsi, %r12			/*for(~~~~; byte < nBytes; ~~~~~) 	*/

	jl .forfast	
					/*return remainder;			*/
	ret		
	

