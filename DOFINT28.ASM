;------------------------------------------------------------------------------;
;                                                                              ;
;  DOF int 28 ( DOS idle ) handler.                                            ;
;                                                                              ;
;------------------------------------------------------------------------------;

        .MODEL SMALL

public  _StealInt28
public  _RestoreInt28
extrn   _DoTime:far    ; called by ISR28 to do time processing
extrn   _Beep:near

.DATA
InDosAddr       DD      0
old28           DD      0
inISR28         DW      0

.CODE
;
; Set int 28 vector to point to my routine
;
_StealInt28     PROC
        push    ds
        push    es

        mov     ah , 34H
        int     21H
        mov     word ptr InDosAddr , bx
        mov     word ptr InDosAddr +2 , es

        mov     ax , 3528H      ; get vector
        int     21H

        mov     word ptr old28 , bx     ; save it
        mov     word ptr old28 + 2 , es

        push    cs
        pop     ds
        mov     dx , offset _TEXT:ISR28
        mov     ax , 2528H      ; change vector
        int     21H

        pop     es
        pop     ds
        ret
_StealInt28     ENDP
;
; Restore original int 28 vector
;
_RestoreInt28   PROC
        push    ds
        lds     dx , [old28]
        mov     ax , 2528H
        int     21H

        pop     ds
        ret
_RestoreInt28   ENDP
;
; Int 28 service routine ( DOS idle handler )
;
ISR28           PROC   FAR
        pushf
        call    cs:old28        ; chain to original routine
        sti                     ; restore interupts

        cmp     cs:inISR28 , 0  ; are we already running
        jne     short isr28exit ; yes then exit

        inc     cs:inISR28

;        lds     bx , cs:InDosAddr
;        mov     al , [bx]       ; al = InDos flag
;        or      al , al         ; are we in DOS?
;        jnz     short notindos  ; jump if where not


        push    ax              ; save all registers
        push    bx
        push    cx
        push    dx
        push    si
        push    di
        push    ds
        push    es

;        mov     ax , 1
;        push    ax
;        mov     ax , 600
;        push    ax
;        call    near ptr _Beep
;        add     sp , 4

        lds     bx , cs:InDosAddr
        dec     byte ptr [bx]

        call    far ptr _DoTime  ; update clock

        lds     bx , cs:InDosAddr
        inc     byte ptr [bx]

        pop     es              ; restore all registers
        pop     ds
        pop     di
        pop     si
        pop     dx
        pop     cx
        pop     bx
        pop     ax

notindos:
        dec     cs:inISR28
isr28exit:
        iret                    ; return from interupt
ISR28           ENDP
                END
