; ..............................................................................
;    File   BEMF_filter.s
; ..............................................................................

		.equ BEMF_filterNumSections, 3

; ..............................................................................
;
; Allocate and initialize filter coefficients
;
; These coefficients have been designed for use in the Transpose filter only

		.section data, xmemory
		.global _BEMF_filterCoefs_81940Hz
		.global _BEMF_filterCoefs_49152Hz

; BEMF_filterCoefs_81940Hz
; 	Butterworth 10th order lowpass
; 	81.94 kHz sampling freq
; 	4kHz passband freq
; 	8kHz stopband freq
; 	0.1db passband ripple
; 	60db stopband ripple

_BEMF_filterCoefs_81940Hz:
.hword	0x024B	; b( 1,0)/2
.hword	0x0496	; b( 1,1)/2
.hword	0x55DE	; a( 1,1)/2
.hword	0x024B	; b( 1,2)/2
.hword	0xE0F6	; a( 1,2)/2
.hword	0x023D	; b( 2,0)/2
.hword	0x047A	; b( 2,1)/2
.hword	0x661E	; a( 2,1)/2
.hword	0x023D	; b( 2,2)/2
.hword	0xCEF9	; a( 2,2)/2
.hword	0x0E54	; b( 3,0)/2
.hword	0x0E54	; b( 3,1)/2
.hword	0x287A	; a( 3,1)/2
.hword	0x0000	; b( 3,2)/2
.hword	0x0000	; a( 3,2)/2

; BEMF_filterCoefs_49152Hz
; 	Butterworth 10th order lowpass
; 	49152 kHz sampling freq
; 	4kHz passband freq
; 	8kHz stopband freq
; 	0.1db passband ripple
; 	60db stopband ripple

_BEMF_filterCoefs_49152Hz:
.hword	0x0544	; b( 1,0)/2
.hword	0x0A89	; b( 1,1)/2
.hword	0x3E25	; a( 1,1)/2
.hword	0x0544	; b( 1,2)/2
.hword	0xECC8	; a( 1,2)/2
.hword	0x0584	; b( 2,0)/2
.hword	0x0B08	; b( 2,1)/2
.hword	0x4F4A	; a( 2,1)/2
.hword	0x0584	; b( 2,2)/2
.hword	0xD5D3	; a( 2,2)/2
.hword	0x1581	; b( 3,0)/2
.hword	0x1581	; b( 3,1)/2
.hword	0x1CB4	; a( 3,1)/2
.hword	0x0000	; b( 3,2)/2
.hword	0x0000	; a( 3,2)/2

; ..............................................................................
; Allocate states buffers in (uninitialized) Y data space

		.section b,bss,ymemory

BEMF_PhaseA_States1:
		.space BEMF_filterNumSections*2

BEMF_PhaseA_States2:
		.space BEMF_filterNumSections*2

BEMF_PhaseB_States1:
		.space BEMF_filterNumSections*2

BEMF_PhaseB_States2:
		.space BEMF_filterNumSections*2

BEMF_PhaseC_States1:
		.space BEMF_filterNumSections*2

BEMF_PhaseC_States2:
		.space BEMF_filterNumSections*2

; ..............................................................................
; Allocate and intialize filter structure

		.section .data
		.global _BEMF_phaseA_Filter
		.global _BEMF_phaseB_Filter
		.global _BEMF_phaseC_Filter

_BEMF_phaseA_Filter:
.hword BEMF_filterNumSections-1
.hword _BEMF_filterCoefs_49152Hz
.hword 0xFF00
.hword BEMF_PhaseA_States1
.hword BEMF_PhaseA_States2
.hword 0x0000

_BEMF_phaseB_Filter:
.hword BEMF_filterNumSections-1
.hword _BEMF_filterCoefs_49152Hz
.hword 0xFF00
.hword BEMF_PhaseB_States1
.hword BEMF_PhaseB_States2
.hword 0x0000

_BEMF_phaseC_Filter:
.hword BEMF_filterNumSections-1
.hword _BEMF_filterCoefs_49152Hz
.hword 0xFF00
.hword BEMF_PhaseC_States1
.hword BEMF_PhaseC_States2
.hword 0x0000
; ..............................................................................
; Sample assembly language calling program
;  The following declarations can be cut and pasted as needed into a program
;		.extern	_IIRTransposeFilterInit
;		.extern	_BlockIIRTransposeFilter
;		.extern	_BEMF_filterFilter
;
;		.section	.bss
;
;	 The input and output buffers can be made any desired size
;	   the value 40 is just an example - however, one must ensure
;	   that the output buffer is at least as long as the number of samples
;	   to be filtered (parameter 4)
;input:		.space	40
;output:	.space	40
;		.text
;
;
;  This code can be copied and pasted as needed into a program
;
;
; Set up pointers to access input samples, filter taps, delay line and
; output samples.
;		mov	#_BEMF_filterFilter, W0	; Initalize W0 to filter structure
;		call	_IIRTransposeFilterInit	; call this function once
;
; The next 4 instructions are required prior to each subroutine call
; to _BlockIIRTransposeFilter
;		mov	#_BEMF_filterFilter, W0	; Initalize W0 to filter structure
;		mov	#input, W1	; Initalize W1 to input buffer 
;		mov	#output, W2	; Initalize W2 to output buffer
;		mov	#20, W3	; Initialize W3 with number of required output samples
;		call	_BlockIIRTransposeFilter	; call as many times as needed
