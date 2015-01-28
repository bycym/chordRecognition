; ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Taken from The Alternative Csound Reference Manual
; and AUTOCOMP
; ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Standard stereo output header
sr	=	44100
kr	=	4410
ksmps	=	10
nchnls	=	2


; Rhodes keyboard-like thing
	instr 1 
i1	=	cpspch(p5)
a1 	fmrhode	p4, i1, 16, 0, 0.01, 3, 7, 7, 7, 8, 7
	outs	a1, a1
	endin

; Hammond keyboard-like thing
	instr 3
i1	=	cpspch(p5-1)
a1 	fmb3	p4, i1, 5, 5, 0.0005, 3, 7, 7, 7, 7, 7
	outs	a1, a1
	endin

; Wurlitzer keyboard-like thing
	instr 4 
i1	=	cpspch(p5-1)
a1 	fmb3	p4, i1, 6, 1, 0.005, 6, 7, 7, 7, 8, 7
	outs	a1, a1
	endin

; Vibes-ish thing
	instr 5
i1	=	cpspch(p5)
a1	wgpluck2 .2, p4, i1, .5, .5
	outs	a1, a1
	endin
