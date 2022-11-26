.data
nombre:	    .byte 11
resultat:   .bool

.global     main

main:
pas_beau:	mov r0, #0
		    ldr r1, =nombre
            ldr r2, [r1]
            ldr r3, =resultat

loop:		add r0, r0, #1  // R0 compteur
            udiv r2, r2, #2 // divise la valeur par deux

            cmp r0, #4   // Si on a fait un bit entier.
            beq fin
            cmp r2, #1 // Si R2 = 0, loop.
            ble loop

fin:
            str r0, [r3]
			bx lr
.end