.data
nombre:	    .byte 997
primaire:   .byte

.text
.global     main

main:		mov r0, #1
            mov r4, #1
            mov r5, #0
            mov r6, #0
		    ldr r1, =nombre
            ldr r2, [r1]

			b racinebis

racine:		add r4, r4, #2
racinebis:	add r5, r5, r4
			add r6, r6, #1
			cmp r5, r2
            blt racine

loop:		add r0, r0, #1
            udiv r3, r2, r0 		// N/x = y
            mls r3, r3, r0, r2		// M = N-(x*y)
            cmp r3, #0
            beq primair
            cmp r0, r6
            bne loop
            b fin


primair:	ldr r1, =primaire
            str r0, [r1]

fin:        bx lr
.end
