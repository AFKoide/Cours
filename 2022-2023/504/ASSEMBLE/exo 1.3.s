.equ 	N, 10	/* number of elements of the vector */

.data

vector: 	.int 1,236,3,154,32,11,236,153,254,35
max: 		.int 0
mul_by_17: 	.int 0


.text
.global main

main:		ldr r0,=vector
			mov r1, #0
			mov r2, #0
			mov r3, #0

loop:		add r1, r1, #1 /* i */
			ldr r2, [r0]

			cmp r2, r3 /* < */
			blt faux
			mov r3, r2
faux:		add r0, r0, #4
			cmp r1, #10 /* Sortir LOOP */
			blt loop


/*			mov r1, #0
			mov r4, #0
mult:		add r1, r1, #1
			add r4, r3, r4
			cmp r1, #17
			blt mult
*/
			lsl r4, r3, #4 //R3*4
			add r4, r3, r4 // R4+R3

			ldr r0,=max
			str r3,[r0]
			ldr r0,=mul_by_17
			str r4,[r0]

			bx lr
.end
