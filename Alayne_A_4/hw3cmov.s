/* OPTIMIZED  VERSION (FASTER)
 * PROGRAMMER: Alayne Anderson

 
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


/*OPTIMIZED THE IF-ELSE BLOCK: we first move the remainder variable into a temporary register and keep the else block remainder in the rbx register
  the test compares the two remainders with the 0x80 immediate accessible and chooses the one that matches, this makes the process much faster which can be seen at 
  at the bottom when you run it to see the time and number of cycles it runs in comparison to the crca1.S original file version	
*/

.if:
	mov %rdi, %rbx			/*move remainder into temporary register */
	test $0x80, %rdi		/*if(remainder & 0x80)			*/				

	mov %rdi, %r15			/*move remainder into temporary register */
	sal %rdi			/*remainder = (remainder << 1) ~~~~ 	*/
	xor $0xD5, %rdi			/*remainder = remainder ^ POLYNOMIAL 	*/

.else:
	sal %rbx			/*remainder = (remainder << 1) 		*/

.test:
	test $0x80, %r15		/*testing remainder */
	cmove %rbx, %rdi			/*updating remainder with the one we want*/

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
					/*char remainder = { %rax } -> { %r14 }	*/
					/*long byte = { %r12 } 			*/
					/*char * message = { %r8 } 		*/
					/*long nBytes = { %r9 }			*/ 
					/*message[byte] = { %r11 }		*/
					/*crcTable[data] = { %r13 }		*/

.L3:		
	leaq crcTable, %rcx 		/*crcTable -> %rcx 			*/	
	movq $0, %r12			/*for(long byte = 0; ~~~~ ; ~~~~) */
	movq $0, %rax			/*remainder = 0	*/
	movq $0, %rbx			/*data = 0*/


.forfast:		
	
	leaq (%rdi, %r12,1), %r11		/* ~~~~ = message[byte] ~~~~~~		*/	
	mov %rax, %r14  			/*move remainder into separate register -> %r14*/

	movb (%r11), %bl					/*data = message[byte] */ /*bl is same reg, smaller slice*/
	xorb %r14b, %bl 							/*data = data^remainder */

	leaq (%rcx, %rbx,1), %r13		/*~~~~ = crcTable[data]			*/		
	movb (%r13), %al				/*remainder = crcTable[data]		*/ 	
	

.forfastcheck:	
	
	inc %r12			/*for(~~~~; ~~~~ ; ++byte)		*/
	cmp %rsi, %r12			/*for(~~~~; byte < nBytes; ~~~~~) 	*/

	jl .forfast	
					/*return remainder;			*/
	ret		
	

