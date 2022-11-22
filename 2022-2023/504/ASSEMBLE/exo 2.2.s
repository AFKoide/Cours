.equ R, 4
.equ C, 4
.equ N, R*C

.data

matrix1:	.byte 165,67,107,67//,236,93,20,80,80,77,59,158,51,164,69,20
matrix2:	.byte 195,65,224,205//,136,83,217,130,23,27,20,10,61,166,39,30
result:		.long 0

.text
.global main

main:
pas_beau:	mov r0, #0
		 	ldr r1, =matrix1
		  	ldr r2, =matrix2
		  	ldr r3, =result
			mov r4, #0
			mov r7, #0

loop:		add r0, r0, #1
			ldrb r5, [r1, r7]
			ldrb r6, [r2, r7]
			add r4, r5, r6
			str r4, [r3]
			add r7, r7, #1
			add r3, r3, #4
			cmp r0, #16
			blt loop
fin:
			bx lr
.end
