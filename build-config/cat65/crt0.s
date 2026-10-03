.setcpu "65c02"

.segment "HEADER"
    .byte "c65", $15    ; Identification string[cite: 1]
    .byte $07           ; ExPort 4 (Vgc7)[cite: 1]
    .byte $00           ; ExPort 7 (Disconnected)[cite: 1]
    .byte 1             ; Amount of 8KiB tilesets[cite: 1]

; -------------------------------------------------------------------------
; C Runtime Setup
; -------------------------------------------------------------------------

.import _main
.export __STARTUP__ : absolute = 1
.importzp   c_sp
.import _NMI
.import _IRQ


.segment "CODE"
Startup:
    sei                 ; Disable interrupts
    cld                 ; Clear decimal mode

    ; Reset Hardware Stack Pointer (Matching your #$ef)
    ldx #$ef
    txs

    ; Initialize the cc65 C software stack pointer safely to $0100
    lda #$00
    sta c_sp
    lda #$01
    sta c_sp+1

    jsr _main           ; Jump to our C main() function

infinite_loop:
    jmp infinite_loop   ; Safety catch-all



.segment "TILES"
    .incbin "tileset.chr"

.segment "VECTORS"
    .word _NMI
    .word Startup
    .word _IRQ


.import   __BSS_RUN__
.import   __BSS_SIZE__

.export     __end = __BSS_RUN__ + __BSS_SIZE__
