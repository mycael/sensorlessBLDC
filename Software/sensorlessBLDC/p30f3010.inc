; p30F3010.inc  Standard Include/Header File, Version 5
; (c) Copyright 2005-2006 Microchip Technology, All rights reserved

.NOLIST

;******************************************************************************
;
; Important:
; ==========
; The processor must be selected before this file is included. The processor
; may  be selected in the following ways:
;
;       1. Command line switch:
;               C:\> AS -p30F3010
;       2. Placing a ".equ" directive before the ".include":
;               .equ __30F3010, 1
;               .include "p30F3010.inc"
;       3. Setting the processor in the MPLAB Project Dialog
;
; Notes:
; =========================
; 1. This include file defines useful bits of information for the
;    PIC30Fxxxx Family of Digital Signal Controllers (dsPIC).
; 2. The register and bit names used in this file are taken to match the
;    PIC30Fxxxx data sheets as closely as possible.
; 3. This file does not map special function register (SFR) names used in the
;    datasheet to memory locations in the PIC30Fxxxx device. The address
;    mapping for SFR names is done in the device linker script only.
; 4. It does however, provide names for the lower(even address) and
;    higher(odd address) bytes of a special function register. For e.g.,
;    the TMR2 register is 16 bits long and its memory address is provided in
;    the device linker script. This SFR can be alternately accessed as two
;    separate bytes, TMR2L and TMR2H.In such a case, TMR2L maps to the lower
;    byte and TMR2H maps to the higher byte. This mapping is performed in this
;    include file.
; 5. This file maps bit names used in the datasheet to bit offsets/positions
;    in a word [0x000F(15) for the MS bit down to 0x0000(0) for the LS bit].
; 6. SFR bit definitions are listed in the ascending order of SFR memory
;    addresses and are grouped based on the module they belong to. For e.g.,the
;    Core SFR  bits are grouped separately from the Interrupt Controller SFR
;    bits or the  General Purpose Timer SFR bits.
;
; File Structure/Organization:
; ============================
; The sections in this file are organized as shown below:
;       1. Core section
;          a. Byte High and Byte Low definitions for SFRs in the section
;          b. Bit Position Definitions for some SFRS in the section
;       2. Interrupt section
;          a. Byte High and Byte Low definitions for SFRs in the section
;          b. Bit Position Definitions for some SFRS in the section
;       :
;       :
;       13. Peripheral x section
;          a. Byte High and Byte Low definitions for SFRs in the section
;          b. Bit Position Definitions for some SFRS in the section
;       :
;       :
;       N. Useful constants that ease assembly-level programming
;       N+1. Configuration Fuses and Macros
;
; Revision History:
; =================
; --------------------------------------------------------------------------
; Rev:   Date:        Details:                                      Who:
; --------------------------------------------------------------------------
; 1.0   27 Nov 2001   Device-specific .inc files from superset      H Vasuki
; 1.1   13 Dec 2001   Added PMD, Changed SPI, UART, ADC + changes      -do-
; 1.2   14 Dec 2001   Modified CORCON bits to reflect Rev C Core       -do-
; 1.3   20 Feb 2002   PMD, CORCON, A/D, RCON, INTCONx, NVMCON          -do-
; 1.4   18 Mar 2002   Renumbered bits for extended support to
;                     Bit-instructions+changes to configuration        -do-
;                     and QEI sections
; 1.5   12 Apr 2002   MCPWM and CAN updates + 1 SPI change             -do-
; 1.6   23 Apr 2002   Reinserted SFR byte (high and low) defines       -do-
; 1.7   06 May 2002   Changed SZ bit to Z, removed SIZESEL1            -do-
; 1.8   13 May 2002   Added PROGOP bits, Added DL2 and moved EDT       -do-
; 1.9   13 May 2002   Added Config Fuse bits and e.g.s in FPORBOR      -do-
; 2.0   31 May 2002   Added DISI bit                                   -do-
; 2.1   13 Aug 2002   Changed I2C interrupt bit to match Datasheet     -do-
; 2.2   23 Aug 2002   Removed ADSTBY bit                               -do-
; 2.3   17 Sep 2002   Added TWRI bit / Removed OFFCAL bit              -do-
; 2.3.1 28 Oct 2002   Changed UPDN_CNT bit to UPDN_SRC                 -do-
; 2.4   02 Dec 2002   Updated Config Macro                             -do-
; 2.5   22 Jan 2003   Corrected ADDRERR bit name                       -do-
; 2.6   27 Jan 2003   Corrected FWDT Fuse Definition                   -do-
; 2.7   07 Mar 2003   Added the descriptive .error directive           -do-
; 2.8   10 Apr 2003   Added Miscellaneous Rev A2 device files          -do-
; 2.9   01 May 2003   This is the first release of Rev B Si files      -do-
;                     Both Rev A2 and Rev B Si files will be
;                     maintained until Rev B Si is available.
;                     Initial changes in Rev B Si include CAN, QEI
;                     and CORCON
; 3.0   02 May 2003   Corrected CAN bit TXB0(zero) to TXBO('o')        -do-
; 3.1   02 June 2003  Added CANCAP bit in CiCTRL                       -do-
; 3.2   23 Jun 2003   Correcetd CANCAP/Changed CONV to DONE            -do-
; 3.3   05 Sept 2003  Added TUNx bits in OSCCON for MSS1 Rev B Si      -do-
; 3.4   17 Sept 2003  Added IMVx bits in DFLTCON/Removed INDOUTx       -do-
; 3.5   12 Nov  2003  Added extra NOLIST directive for enhancing LST   -do-
; 3.6   25 Nov  2003  Renamed TUN4-TUN1 to TUN3 to TUN0                -do-
; 3.7   08 Apr 2004   Enhanced Config macro examples for FOSC fuse     -do-
; 3.7a  21 May 2004   Enhancements to FOSC, OSCCON and added OSCTUN    -do-
; 3.7b  23 Aug 2004   More changes to FOSC bits for SIB v3             -do-
; 3.8   25 Oct  2004  Added "A" devices, OSCTUNL and changed ADCBUF    -do-
; 3.9   23 Jan 2005   Changed Config Macro for C30 v1.30 compliance    -do-
; 3.9a  23 Jan 2005   Removed LVD bits for Motor Ctrl family devices   -do-
; 4.0   01 Apr 2005   Global change "EC_IO" to "ECIO"/Fixed "__FOSC"   -do-
;                     Global change "ERC_IO" to "ERCIO"
; 4.1   28 Jun 2005   Added IUE bit in PWMCON2                         -do-
; 4.2   12 Aug 2005   Changed config FGS to match silicon              -do-
; 5.0   07 Nov 2005   Removed SECUREFLT bit                            -do-
; --------------------------------------------------------------------------
;
;******************************************************************************


;==============================================================================
;
; Verify Processor Type
;
;==============================================================================

.IFNDEF __30F3010
   .error "Include file does not match processor setting."
.ENDIF
.NOLIST

;==============================================================================
;
; 1. Core Bit Position Definitions for SFRs
;     & SFR High/Low byte definitions.
;==============================================================================
; 1a. SFR Definitions
;------------------------------------------------------------------------------
         .equiv WREG0L, _WREG0
         .equiv WREG0H, _WREG0+1
         .equiv WREG1L, _WREG1
         .equiv WREG1H, _WREG1+1
         .equiv WREG2L, _WREG2
         .equiv WREG2H, _WREG2+1
         .equiv WREG3L, _WREG3
         .equiv WREG3H, _WREG3+1
         .equiv WREG4L, _WREG4
         .equiv WREG4H, _WREG4+1
         .equiv WREG5L, _WREG5
         .equiv WREG5H, _WREG5+1
         .equiv WREG6L, _WREG6
         .equiv WREG6H, _WREG6+1
         .equiv WREG7L, _WREG7
         .equiv WREG7H, _WREG7+1
         .equiv WREG8L, _WREG8
         .equiv WREG8H, _WREG8+1
         .equiv WREG9L, _WREG9
         .equiv WREG9H, _WREG9+1
         .equiv WREG10L, _WREG10
         .equiv WREG10H, _WREG10+1
         .equiv WREG11L, _WREG11
         .equiv WREG11H, _WREG11+1
         .equiv WREG12L, _WREG12
         .equiv WREG12H, _WREG12+1
         .equiv WREG13L, _WREG13
         .equiv WREG13H, _WREG13+1
         .equiv WREG14L, _WREG14
         .equiv WREG14H, _WREG14+1
         .equiv WREG15L, _WREG15
         .equiv WREG15H, _WREG15+1
         .equiv SPLIML, _SPLIM
         .equiv SPLIMH, _SPLIM+1
         .equiv ACCALL, _ACCAL
         .equiv ACCALH, _ACCAL+1
         .equiv ACCAHL, _ACCAH
         .equiv ACCAHH, _ACCAH+1
         .equiv ACCAUL, _ACCAU
         .equiv ACCBLL, _ACCBL
         .equiv ACCBLH, _ACCBL+1
         .equiv ACCBHL, _ACCBH
         .equiv ACCBHH, _ACCBH+1
         .equiv ACCBUL, _ACCBU
         .equiv PCLL, _PCL
         .equiv PCLH, _PCL+1
         .equiv PCHL, _PCH
         .equiv PCHH, _PCH+1
         .equiv TBLPAGL, _TBLPAG
         .equiv TBLPAGH, _TBLPAG+1
         .equiv PSVPAGL, _PSVPAG
         .equiv PSVPAGH, _PSVPAG+1
         .equiv RCOUNTL, _RCOUNT
         .equiv RCOUNTH, _RCOUNT+1
         .equiv DCOUNTL, _DCOUNT
         .equiv DCOUNTH, _DCOUNT+1
         .equiv DOSTARTLL, _DOSTARTL
         .equiv DOSTARTLH, _DOSTARTL+1
         .equiv DOSTARTHL, _DOSTARTH
         .equiv DOSTARTHH, _DOSTARTH+1
         .equiv DOENDLL, _DOENDL
         .equiv DOENDLH, _DOENDL+1
         .equiv DOENDHL, _DOENDH
         .equiv DOENDHH, _DOENDH+1
         .equiv SRL, _SR                        ; See Description in section 1b
         .equiv SRH, _SR+1                      ; See Description in section 1b
         .equiv CORCONL, _CORCON                ; See Description in section 1b
         .equiv MODCONL, _MODCON                ; See Description in section 1b
         .equiv MODCONH, _MODCON+1              ; See Description in section 1b
         .equiv XMODSRTL, _XMODSRT
         .equiv XMODSRTH, _XMODSRT+1
         .equiv XMODENDL, _XMODEND
         .equiv XMODENDH, _XMODEND+1
         .equiv YMODSRTL, _YMODSRT
         .equiv YMODSRTH, _YMODSRT+1
         .equiv YMODENDL, _YMODEND
         .equiv YMODENDH, _YMODEND+1
         .equiv XBREVL, _XBREV                  ; See Description in section 1b
         .equiv XBREVH, _XBREV+1
         .equiv DISICNTL, _DISICNT
         .equiv DISICNTH, _DISICNT+1

;------------------------------------------------------------------------------
; 1b. Bit Position Definitions for some SFRs
;------------------------------------------------------------------------------
;     SR - Status Register
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv OA, 0x000F
         .equiv OB, 0x000E
         .equiv SA, 0x000D
         .equiv SB, 0x000C
         .equiv OAB, 0x000B
         .equiv SAB, 0x000A
         .equiv DA, 0x0009
         .equiv DC, 0x0008

; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv IPL2, 0x0007
         .equiv IPL1, 0x0006
         .equiv IPL0, 0x0005
         .equiv RA, 0x0004
         .equiv N, 0x0003
         .equiv OV, 0x0002
         .equiv Z, 0x0001
         .equiv C, 0x0000

;------------------------------------------------------------------------------
;     CORCON - Core Configuration Register
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv US, 0x000C
         .equiv EDT, 0x000B
         .equiv DL2, 0x000A
         .equiv DL1, 0x0009
         .equiv DL0, 0x0008

; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv SATA, 0x0007
         .equiv SATB, 0x0006
         .equiv SATDW, 0x0005
         .equiv ACCSAT, 0x0004
         .equiv IPL3, 0x0003
         .equiv PSV, 0x0002
         .equiv RND, 0x0001
         .equiv IF, 0x0000

;------------------------------------------------------------------------------
;     MODCON - Modulo Addressing Mode Control Register
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv XMODEN, 0x000F
         .equiv YMODEN, 0x000E

;------------------------------------------------------------------------------
;     XBREV - Bit Reversed Addressing Mode Control Register
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv BREN, 0x000F

;==============================================================================
;
; 2.  Interrupt Controller Bit Position Definitions for SFRs
;     & SFR High/Low byte definitions.
;==============================================================================
; 2a. SFR Definitions
;------------------------------------------------------------------------------
         .equiv INTCON1H, _INTCON1+1           ; See description for all
         .equiv INTCON1L, _INTCON1       ;  registers in sub-section below
         .equiv INTCON2H, _INTCON2+1
         .equiv INTCON2L, _INTCON2
         .equiv IFS0H, _IFS0+1
         .equiv IFS0L, _IFS0
         .equiv IFS1L, _IFS1
         .equiv IFS2H, _IFS2+1
         .equiv IFS2L, _IFS2
         .equiv IEC0H, _IEC0+1
         .equiv IEC0L, _IEC0
         .equiv IEC1L, _IEC1
         .equiv IEC2H, _IEC2+1
         .equiv IEC2L, _IEC2
         .equiv IPC0H, _IPC0+1
         .equiv IPC0L, _IPC0
         .equiv IPC1H, _IPC1+1
         .equiv IPC1L, _IPC1
         .equiv IPC2H, _IPC2+1
         .equiv IPC2L, _IPC2
         .equiv IPC3H, _IPC3+1
         .equiv IPC3L, _IPC3
         .equiv IPC4H, _IPC4+1
         .equiv IPC4L, _IPC4
         .equiv IPC5H, _IPC5+1
         .equiv IPC5L, _IPC5
         .equiv IPC9H, _IPC9+1
         .equiv IPC10H, _IPC10+1
         .equiv IPC10L, _IPC10
	 .equiv INTTREGH, _INTTREG+1
         .equiv INTTREGL, _INTTREG

;------------------------------------------------------------------------------
; 2b. Bit Position Definitions for some SFRs
;------------------------------------------------------------------------------
;     INTCON1 : Interrupt Control Register 1
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv NSTDIS, 0x000F
         .equiv OVATE, 0x000A
         .equiv OVBTE, 0x0009
         .equiv COVTE, 0x0008

; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv MATHERR, 0x0004
         .equiv ADDRERR, 0x0003
         .equiv STKERR, 0x0002
         .equiv OSCFAIL, 0x0001

;------------------------------------------------------------------------------
;     INTCON2 : Interrupt Control Register 2
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv ALTIVT, 0x000F
         .equiv DISI, 0x000E


; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv INT2EP, 0x0002
         .equiv INT1EP, 0x0001
         .equiv INT0EP, 0x0000

;------------------------------------------------------------------------------
;     IFS0 : Interrupt Flag Status Register 0
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv CNIF, 0x000F
         .equiv MI2CIF, 0x000E
         .equiv SI2CIF, 0x000D
         .equiv NVMIF, 0x000C
         .equiv ADIF, 0x000B
         .equiv U1TXIF, 0x000A
         .equiv U1RXIF, 0x0009
         .equiv SPI1IF, 0x0008

; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv T3IF, 0x0007
         .equiv T2IF, 0x0006
         .equiv OC2IF, 0x0005
         .equiv IC2IF, 0x0004
         .equiv T1IF, 0x0003
         .equiv OC1IF, 0x0002
         .equiv IC1IF, 0x0001
         .equiv INT0IF, 0x0000

;------------------------------------------------------------------------------
;     IFS1 : Interrupt Flag Status Register 1
;------------------------------------------------------------------------------
; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv INT2IF, 0x0007
         .equiv T5IF, 0x0006
         .equiv T4IF, 0x0005
         .equiv OC4IF, 0x0004
         .equiv OC3IF, 0x0003
         .equiv IC8IF, 0x0002
         .equiv IC7IF, 0x0001
         .equiv INT1IF, 0x0000

;------------------------------------------------------------------------------
;     IFS2 : Interrupt Flag Status Register 2
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv FLTAIF, 0x000B
         .equiv QEIIF, 0x0008

; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv PWMIF, 0x0007

;------------------------------------------------------------------------------
;     IEC0 : Interrupt Enable Control Register 0
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv CNIE, 0x000F
         .equiv MI2CIE, 0x000E
         .equiv SI2CIE, 0x000D
         .equiv NVMIE, 0x000C
         .equiv ADIE, 0x000B
         .equiv U1TXIE, 0x000A
         .equiv U1RXIE, 0x0009
         .equiv SPI1IE, 0x0008

; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv T3IE, 0x0007
         .equiv T2IE, 0x0006
         .equiv OC2IE, 0x0005
         .equiv IC2IE, 0x0004
         .equiv T1IE, 0x0003
         .equiv OC1IE, 0x0002
         .equiv IC1IE, 0x0001
         .equiv INT0IE, 0x0000

;------------------------------------------------------------------------------
;     IEC1 : Interrupt Enable Control Register 1
;------------------------------------------------------------------------------
; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv INT2IE, 0x0007
         .equiv T5IE, 0x0006
         .equiv T4IE, 0x0005
         .equiv IC8IE, 0x0002
         .equiv IC7IE, 0x0001
         .equiv INT1IE, 0x0000

;------------------------------------------------------------------------------
;     IEC2 : Interrupt Enable Control Register 2
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv FLTAIE, 0x000B
         .equiv QEIIE, 0x0008

; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv PWMIE, 0x0007

;------------------------------------------------------------------------------
;     IPC0 : Interrupt Priority Control Register 0
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv T1IP2, 0x000E
         .equiv T1IP1, 0x000D
         .equiv T1IP0, 0x000C
         .equiv OC1IP2, 0x000A
         .equiv OC1IP1, 0x0009
         .equiv OC1IP0, 0x0008

; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv IC1IP2, 0x0006
         .equiv IC1IP1, 0x0005
         .equiv IC1IP0, 0x0004
         .equiv INT0IP2, 0x0002
         .equiv INT0IP1, 0x0001
         .equiv INT0IP0, 0x0000

;------------------------------------------------------------------------------
;     IPC1 : Interrupt Priority Control Register 1
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv T3IP2, 0x000E
         .equiv T3IP1, 0x000D
         .equiv T3IP0, 0x000C
         .equiv T2IP2, 0x000A
         .equiv T2IP1, 0x0009
         .equiv T2IP0, 0x0008

; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv OC2IP2, 0x0006
         .equiv OC2IP1, 0x0005
         .equiv OC2IP0, 0x0004
         .equiv IC2IP2, 0x0002
         .equiv IC2IP1, 0x0001
         .equiv IC2IP0, 0x0000

;------------------------------------------------------------------------------
;     IPC2 : Interrupt Priority Control Register 2
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv ADIP2, 0x000E
         .equiv ADIP1, 0x000D
         .equiv ADIP0, 0x000C
         .equiv U1TXIP2, 0x000A
         .equiv U1TXIP1, 0x0009
         .equiv U1TXIP0, 0x0008

; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv U1RXIP2, 0x0006
         .equiv U1RXIP1, 0x0005
         .equiv U1RXIP0, 0x0004
         .equiv SPI1IP2, 0x0002
         .equiv SPI1IP1, 0x0001
         .equiv SPI1IP0, 0x0000

;------------------------------------------------------------------------------
;     IPC3 : Interrupt Priority Control Register 3
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv CNIP2, 0x000E
         .equiv CNIP1, 0x000D
         .equiv CNIP0, 0x000C
         .equiv MI2CIP2, 0x000A
         .equiv MI2CIP1, 0x0009
         .equiv MI2CIP0, 0x0008

; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv SI2CIP2, 0x0006
         .equiv SI2CIP1, 0x0005
         .equiv SI2CIP0, 0x0004
         .equiv NVMIP2, 0x0002
         .equiv NVMIP1, 0x0001
         .equiv NVMIP0, 0x0000

;------------------------------------------------------------------------------
;     IPC4 : Interrupt Priority Control Register 4
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv IC8IP2, 0x000A
         .equiv IC8IP1, 0x0009
         .equiv IC8IP0, 0x0008

; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv IC7IP2, 0x0006
         .equiv IC7IP1, 0x0005
         .equiv IC7IP0, 0x0004
         .equiv INT1IP2, 0x0002
         .equiv INT1IP1, 0x0001
         .equiv INT1IP0, 0x0000

;------------------------------------------------------------------------------
;     IPC5 : Interrupt Priority Control Register 5
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv INT2IP2, 0x000E
         .equiv INT2IP1, 0x000D
         .equiv INT2IP0, 0x000C
         .equiv T5IP2, 0x000A
         .equiv T5IP1, 0x0009
         .equiv T5IP0, 0x0008

; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv T4IP2, 0x0006
         .equiv T4IP1, 0x0005
         .equiv T4IP0, 0x0004


;------------------------------------------------------------------------------
;     IPC9 : Interrupt Priority Control Register 9
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv PWMIP2, 0x000E
         .equiv PWMIP1, 0x000D
         .equiv PWMIP0, 0x000C

;------------------------------------------------------------------------------
;     IPC10 : Interrupt Priority Control Register 10
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv FLTAIP2, 0x000E
         .equiv FLTAIP1, 0x000D
         .equiv FLTAIP0, 0x000C

; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv QEIIP2, 0x0002
         .equiv QEIIP1, 0x0001
         .equiv QEIIP0, 0x0000

;------------------------------------------------------------------------------
;     INTTREG : Interrupt Controller Test Register
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv IRQTOCPU,0x000F
	 .equiv TMODE, 0x000E
	 .equiv VHOLD, 0x000D
 	 .equiv ILR3, 0x000B
         .equiv ILR2, 0x000A
         .equiv ILR1, 0x0009
         .equiv ILR0, 0x0008

; Low Byte (Even Address)
; Bit Positions defined below:

         .equiv VECNUM5, 0x0005	
         .equiv VECNUM4, 0x0004
         .equiv VECNUM3, 0x0003
         .equiv VECNUM2, 0x0002
         .equiv VECNUM1, 0x0001
         .equiv VECNUM0, 0x0000



;==============================================================================
;
; 3.  Input Change Notification Module Bit Position Definitions for SFRs
;     & SFR High/Low byte definitions.
;==============================================================================
; 3a. SFR Definitions
;------------------------------------------------------------------------------
         .equiv CNEN1L, _CNEN1                  ; See description for all
         .equiv CNEN2L, _CNEN2                  ;  registers in sub-section below
         .equiv CNPU1L, _CNPU1
	 .equiv CNPU2L, _CNPU2

;------------------------------------------------------------------------------
; 3b. Bit Position Definitions for some SFRs
;------------------------------------------------------------------------------
;     CNEN1 : Input Change Notification Interrupt Enable Register 1
;------------------------------------------------------------------------------
; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv CN7IE, 0x0007
         .equiv CN6IE, 0x0006
         .equiv CN5IE, 0x0005
         .equiv CN4IE, 0x0004
         .equiv CN3IE, 0x0003
         .equiv CN2IE, 0x0002
         .equiv CN1IE, 0x0001
         .equiv CN0IE, 0x0000
;------------------------------------------------------------------------------
;     CNEN2 : Input Change Notification Interrupt Enable Register 2
;------------------------------------------------------------------------------
; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv CN18IE, 0x0002
         .equiv CN17IE, 0x0001

;------------------------------------------------------------------------------
;     CNPU1 : Input Change Notification Pullup Enable Register 1
;------------------------------------------------------------------------------
; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv CN7PUE, 0x0007
         .equiv CN6PUE, 0x0006
         .equiv CN5PUE, 0x0005
         .equiv CN4PUE, 0x0004
         .equiv CN3PUE, 0x0003
         .equiv CN2PUE, 0x0002
         .equiv CN1PUE, 0x0001
         .equiv CN0PUE, 0x0000
;------------------------------------------------------------------------------
;     CNPU2 : Input Change Notification Pullup Enable Register 2
;------------------------------------------------------------------------------
; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv CN18PUE, 0x0002
         .equiv CN17PUE, 0x0001

;==============================================================================
;
; 4.  Timer Module Bit Position Definitions for SFRs
;     & SFR High/Low byte definitions.
;==============================================================================
; 4a. SFR Definitions
;------------------------------------------------------------------------------

;---------------Timer 1 Module-------------------------------------------------
         .equiv TMR1L, _TMR1
         .equiv TMR1H, _TMR1+1
         .equiv PR1L, _PR1
         .equiv PR1H, _PR1+1
         .equiv T1CONL, _T1CON                  ; See TxCON description in
         .equiv T1CONH, _T1CON+1                ;  sub-section below

;---------------Timer2/3 Module------------------------------------------------
         .equiv TMR2L, _TMR2
         .equiv TMR2H, _TMR2+1
         .equiv TMR3HLDL, _TMR3HLD
         .equiv TMR3HLDH, _TMR3HLD+1
         .equiv TMR3L, _TMR3
         .equiv TMR3H, _TMR3+1
         .equiv PR2L, _PR2
         .equiv PR2H, _PR2+1
         .equiv PR3L, _PR3
         .equiv PR3H, _PR3+1
         .equiv T2CONL, _T2CON                  ; See TxCON description in
         .equiv T2CONH, _T2CON+1                ;  sub-section below
         .equiv T3CONL, _T3CON                  ; - do -
         .equiv T3CONH, _T3CON+1

;-------------- Timer4/5 Module------------------------------------------------
         .equiv TMR4L, _TMR4
         .equiv TMR4H, _TMR4+1
         .equiv TMR5HLDL, _TMR5HLD
         .equiv TMR5HLDH, _TMR5HLD+1
         .equiv TMR5L, _TMR5
         .equiv TMR5H, _TMR5+1
         .equiv PR4L, _PR4
         .equiv PR4H, _PR4+1
         .equiv PR5L, _PR5
         .equiv PR5H, _PR5+1
         .equiv T4CONL, _T4CON                  ; See TxCON description in
         .equiv T4CONH, _T4CON+1                ;  sub-section below
         .equiv T5CONL, _T5CON                  ; - do -
         .equiv T5CONH, _T5CON+1

;------------------------------------------------------------------------------
; 4b. Bit Position Definitions for some SFRs
;------------------------------------------------------------------------------
;     TxCON : Timer x Control Register
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv TON, 0x000F
         .equiv TSIDL, 0x000D

; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv TGATE, 0x0006
         .equiv TCKPS1, 0x0005
         .equiv TCKPS0, 0x0004
         .equiv T32, 0x0003       ;T32 present only in T2CON and T4CON
         .equiv TSYNC, 0x0002
         .equiv TCS, 0x0001

;==============================================================================
;
; 5.  Quadrature Encoder Interface Module Bit Position Definitions for SFRs
;     & SFR High/Low byte definitions.
;==============================================================================
; 5a. SFR Definitions
;------------------------------------------------------------------------------
         .equiv QEICONL, _QEICON                ; See QEICON description in
         .equiv QEICONH, _QEICON+1              ;  sub-section below
         .equiv DFLTCONL, _DFLTCON              ; See DFLTCON description in
         .equiv DFLTCONH, _DFLTCON+1            ;  sub-section below
         .equiv POSCNTL, _POSCNT
         .equiv POSCNTH, _POSCNT+1
         .equiv MAXCNTL, _MAXCNT
         .equiv MAXCNTH, _MAXCNT+1

;------------------------------------------------------------------------------
; 5b. Bit Position Definitions for some SFRs
;------------------------------------------------------------------------------
;     QEICON : Quadrature Encoder Interface Control Register
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv CNTERR, 0x000F
         .equiv QEISIDL, 0x000D
         .equiv INDX, 0x000C
         .equiv UPDN, 0x000B
         .equiv QEIM2, 0x000A
         .equiv QEIM1, 0x0009
         .equiv QEIM0, 0x0008

; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv SWPAB, 0x0007
         .equiv PCDOUT, 0x0006
         .equiv TQGATE, 0x0005
         .equiv TQCKPS1, 0x0004
         .equiv TQCKPS0, 0x0003
         .equiv POSRES, 0x0002
         .equiv TQCS, 0x0001
         .equiv UPDN_SRC, 0x0000

;------------------------------------------------------------------------------
;     DFLTCON : Digital Filter Control Register
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv IMV1, 0x000A
         .equiv IMV0, 0x0009
         .equiv CEID, 0x0008

; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv QEOUT, 0x0007
         .equiv QECK2, 0x0006
         .equiv QECK1, 0x0005
         .equiv QECK0, 0x0004


;==============================================================================
;
; 6.  Input Capture Module Bit Position Definitions for SFRs
;     & SFR High/Low byte definitions.
;==============================================================================
; 6a. SFR Definitions
;------------------------------------------------------------------------------
         .equiv IC1BUFL, _IC1BUF
         .equiv IC1BUFH, _IC1BUF+1
         .equiv IC1CONL, _IC1CON                ; See ICxCON description in
         .equiv IC1CONH, _IC1CON+1              ;  sub-section below
         .equiv IC2BUFL, _IC2BUF
         .equiv IC2BUFH, _IC2BUF+1
         .equiv IC2CONL, _IC2CON                ; See ICxCON description in
         .equiv IC2CONH, _IC2CON+1              ;  sub-section below
         .equiv IC7BUFL, _IC7BUF
         .equiv IC7BUFH, _IC7BUF+1
         .equiv IC7CONL, _IC7CON                ; See ICxCON description in
         .equiv IC7CONH, _IC7CON+1              ;  sub-section below
         .equiv IC8BUFL, _IC8BUF
         .equiv IC8BUFH, _IC8BUF+1
         .equiv IC8CONL, _IC8CON                ; See ICxCON description in
         .equiv IC8CONH, _IC8CON+1              ;  sub-section below
;------------------------------------------------------------------------------
; 6b. Bit Position Definitions for some SFRs
;------------------------------------------------------------------------------
;     ICxCON : Input Capture x Control Register
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv ICSIDL, 0x000D

; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv ICTMR, 0x0007
         .equiv ICI1, 0x0006
         .equiv ICI0, 0x0005
         .equiv ICOV, 0x0004
         .equiv ICBNE, 0x0003
         .equiv ICM2, 0x0002
         .equiv ICM1, 0x0001
         .equiv ICM0, 0x0000

;==============================================================================
;
; 7.  Output Compare Module Bit Position Definitions for SFRs
;     & SFR High/Low byte definitions.
;==============================================================================
; 7a. SFR Definitions
;------------------------------------------------------------------------------
         .equiv OC1RSL, _OC1RS
         .equiv OC1RSH, _OC1RS+1
         .equiv OC1RL, _OC1R
         .equiv OC1RH, _OC1R+1
         .equiv OC1CONL, _OC1CON                ; See OCxCON description in
         .equiv OC1CONH, _OC1CON+1              ;  sub-section below
         .equiv OC2RSL, _OC2RS
         .equiv OC2RSH, _OC2RS+1
         .equiv OC2RL, _OC2R
         .equiv OC2RH, _OC2R+1
         .equiv OC2CONL, _OC2CON                ; See OCxCON description in
         .equiv OC2CONH, _OC2CON+1              ;  sub-section below


;------------------------------------------------------------------------------
; 7b. Bit Position Definitions for some SFRs
;------------------------------------------------------------------------------
;     OCxCON : Output Compare x Control Register
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv OCSIDL, 0x000D

; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv OCFLT, 0x0004
         .equiv OCTSEL, 0x0003
         .equiv OCM2, 0x0002
         .equiv OCM1, 0x0001
         .equiv OCM0, 0x0000

;==============================================================================
;
; 8. Motor Control PWM Module Bit Position Definitions for SFRs
;     & SFR High/Low byte definitions.
;==============================================================================
; 8a. SFR Definitions
;------------------------------------------------------------------------------
         .equiv PTCONL, _PTCON              ; See description for all registers
         .equiv PTCONH, _PTCON+1            ;  except PDCx registers, in section 8b
         .equiv PTMRL, _PTMR
         .equiv PTMRH, _PTMR+1
         .equiv PTPERL, _PTPER
         .equiv PTPERH, _PTPER+1
         .equiv SEVTCMPL, _SEVTCMP
         .equiv SEVTCMPH, _SEVTCMP+1
         .equiv PWMCON1L, _PWMCON1
         .equiv PWMCON1H, _PWMCON1+1
         .equiv PWMCON2L, _PWMCON2
         .equiv PWMCON2H, _PWMCON2+1
         .equiv DTCON1L, _DTCON1
         .equiv DTCON1H, _DTCON1+1
         .equiv FLTACONL, _FLTACON
         .equiv FLTACONH, _FLTACON+1
         .equiv OVDCONL, _OVDCON
         .equiv OVDCONH, _OVDCON+1
         .equiv PDC1L, _PDC1
         .equiv PDC1H, _PDC1+1
         .equiv PDC2L, _PDC2
         .equiv PDC2H, _PDC2+1
         .equiv PDC3L, _PDC3
         .equiv PDC3H, _PDC3+1

;------------------------------------------------------------------------------
; 8b. Bit Position Definitions for some SFRs
;------------------------------------------------------------------------------
;     PTCON : PWM Timebase Control Register
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv PTEN, 0x000F
         .equiv PTSIDL, 0x000D

; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv PTOPS3, 0x0007
         .equiv PTOPS2, 0x0006
         .equiv PTOPS1, 0x0005
         .equiv PTOPS0, 0x0004
         .equiv PTCKPS1, 0x0003
         .equiv PTCKPS0, 0x0002
         .equiv PTMOD1, 0x0001
         .equiv PTMOD0, 0x0000

;------------------------------------------------------------------------------
;     PTMR : PWM Timebase Count Register
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv PTDIR, 0x000F

;------------------------------------------------------------------------------
;     SEVTCMP : Special Event Compare Register
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv SEVTDIR, 0x000F

;------------------------------------------------------------------------------
;     PWMCON1 : PWM Control Register 1
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv PMOD3, 0x000A
         .equiv PMOD2, 0x0009
         .equiv PMOD1, 0x0008

; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv PEN3H, 0x0006
         .equiv PEN2H, 0x0005
         .equiv PEN1H, 0x0004
         .equiv PEN3L, 0x0002
         .equiv PEN2L, 0x0001
         .equiv PEN1L, 0x0000

;------------------------------------------------------------------------------
;     PWMCON2 : PWM Control Register 2
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv SEVOPS3, 0x000B
         .equiv SEVOPS2, 0x000A
         .equiv SEVOPS1, 0x0009
         .equiv SEVOPS0, 0x0008

; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv IUE, 0x0002
         .equiv OSYNC, 0x0001
         .equiv UDIS, 0x0000

;------------------------------------------------------------------------------
;     DTCON1 : Dead Time Control Register 1
;------------------------------------------------------------------------------
; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv DTAPS1, 0x0007
         .equiv DTAPS0, 0x0006
         .equiv DTA5, 0x0005
         .equiv DTA4, 0x0004
         .equiv DTA3, 0x0003
         .equiv DTA2, 0x0002
         .equiv DTA1, 0x0001
         .equiv DTA0, 0x0000

;------------------------------------------------------------------------------
;     FLTACON : Fault Input A Control Register
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv FAOV3H, 0x000D
         .equiv FAOV3L, 0x000C
         .equiv FAOV2H, 0x000B
         .equiv FAOV2L, 0x000A
         .equiv FAOV1H, 0x0009
         .equiv FAOV1L, 0x0008

; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv FLTAM, 0x0007
         .equiv FAEN3, 0x0002
         .equiv FAEN2, 0x0001
         .equiv FAEN1, 0x0000

;------------------------------------------------------------------------------
;     OVDCON : PWM Output Override Control Register
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv POVD3H, 0x000D
         .equiv POVD3L, 0x000C
         .equiv POVD2H, 0x000B
         .equiv POVD2L, 0x000A
         .equiv POVD1H, 0x0009
         .equiv POVD1L, 0x0008

; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv POUT3H, 0x0005
         .equiv POUT3L, 0x0004
         .equiv POUT2H, 0x0003
         .equiv POUT2L, 0x0002
         .equiv POUT1H, 0x0001
         .equiv POUT1L, 0x0000

;==============================================================================
;
; 9. Inter-Integrated Circuit(I2C) Module Bit Position Definitions for SFRs
;     & SFR High/Low byte definitions.
;==============================================================================
; 9a. SFR Definitions
;------------------------------------------------------------------------------
         .equiv I2CRCVL, _I2CRCV
         .equiv I2CRCVH, _I2CRCV+1
         .equiv I2CTRNL, _I2CTRN
         .equiv I2CTRNH, _I2CTRN+1
         .equiv I2CBRGL, _I2CBRG
         .equiv I2CBRGH, _I2CBRG+1
         .equiv I2CCONL, _I2CCON                ; See I2CCON description in
         .equiv I2CCONH, _I2CCON+1              ;  section 9b
         .equiv I2CSTATL, _I2CSTAT              ; See I2CSTAT description in
         .equiv I2CSTATH, _I2CSTAT+1            ;  section 9b
         .equiv I2CADDL, _I2CADD
         .equiv I2CADDH, _I2CADD+1

;------------------------------------------------------------------------------
; 9b. Bit Position Definitions for some SFRs
;------------------------------------------------------------------------------
;     I2CCON : I2C Control Register
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv I2CEN, 0x000F
         .equiv I2CSIDL, 0x000D
         .equiv SCLREL, 0x000C
         .equiv IPMIEN, 0x000B
         .equiv A10M, 0x000A
         .equiv DISSLW, 0x0009
         .equiv SMEN, 0x0008

; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv GCEN, 0x0007
         .equiv STREN, 0x0006
         .equiv ACKDT, 0x0005
         .equiv ACKEN, 0x0004
         .equiv RCEN, 0x0003
         .equiv PEN, 0x0002
         .equiv RSEN, 0x0001
         .equiv SEN, 0x0000

;------------------------------------------------------------------------------
;     I2CSTAT : I2C Status Register
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv ACKSTAT, 0x000F
         .equiv TRSTAT, 0x000E
         .equiv BCL, 0x000A
         .equiv GCSTAT, 0x0009
         .equiv ADD10, 0x0008

; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv IWCOL, 0x0007
         .equiv I2COV, 0x0006
         .equiv D_A, 0x0005
         .equiv P, 0x0004
         .equiv S, 0x0003
         .equiv R_W, 0x0002
         .equiv RBF, 0x0001
         .equiv TBF, 0x0000

;==============================================================================
;
; 10. UARTx Module Bit Position Definitions for SFRs
;     & SFR High/Low byte definitions.
;==============================================================================
; 10a. SFR Definitions
;------------------------------------------------------------------------------
;------------------UART 1 Module-----------------------------------------------
         .equiv U1MODEL, _U1MODE                ; See U1MODE description in
         .equiv U1MODEH, _U1MODE+1              ;  sub-section below
         .equiv U1STAL, _U1STA                  ; See U1STA description in
         .equiv U1STAH, _U1STA+1                ;  sub-section below
         .equiv U1TXREGL, _U1TXREG
         .equiv U1TXREGH, _U1TXREG+1
         .equiv U1RXREGL, _U1RXREG
         .equiv U1RXREGH, _U1RXREG+1
         .equiv U1BRGL, _U1BRG
         .equiv U1BRGH, _U1BRG+1

;------------------------------------------------------------------------------
; 10b. Bit Position Definitions for some SFRs
;------------------------------------------------------------------------------
;     UxMODE : UART x Mode Register
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv UARTEN, 0x000F
         .equiv USIDL, 0x000D
         .equiv ALTIO, 0x000A


; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv WAKE, 0x0007
         .equiv LPBACK, 0x0006
         .equiv ABAUD, 0x0005
         .equiv PDSEL1, 0x0002
         .equiv PDSEL0, 0x0001
         .equiv STSEL, 0x0000

;------------------------------------------------------------------------------
;     U1STA : UART 1 Status and Control Register
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv UTXISEL, 0x000F
         .equiv UTXBRK, 0x000B
         .equiv UTXEN, 0x000A
         .equiv UTXBF, 0x0009
         .equiv TRMT, 0x0008

; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv URXISEL1, 0x0007
         .equiv URXISEL0, 0x0006
         .equiv ADDEN, 0x0005
         .equiv RIDLE, 0x0004
         .equiv PERR, 0x0003
         .equiv FERR, 0x0002
         .equiv OERR, 0x0001
         .equiv URXDA, 0x0000

;==============================================================================
;
; 11. Serial Peripheral Interface Module Bit Position Definitions for SFRs
;     & SFR High/Low byte definitions.      (SPI1)
;==============================================================================
; 11a. SFR Definitions
;------------------------------------------------------------------------------
;------------------SPI 1 Module------------------------------------------------
         .equiv SPI1STATL, _SPI1STAT    ; See SPIxSTAT description in
         .equiv SPI1STATH, _SPI1STAT+1  ;  sub-section below
         .equiv SPI1CONL, _SPI1CON      ; See SPIxCON description in
         .equiv SPI1CONH, _SPI1CON+1    ;  sub-section below
         .equiv SPI1BUFL, _SPI1BUF
         .equiv SPI1BUFH, _SPI1BUF+1

;------------------------------------------------------------------------------
; 11b. Bit Position Definitions for some SFRs
;------------------------------------------------------------------------------
;     SPI1STAT : SPI 1 Status register
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv SPIEN, 0x000F
         .equiv SPISIDL, 0x000D

; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv SPIROV, 0x0006
         .equiv SPITBF, 0x0001
         .equiv SPIRBF, 0x0000


;------------------------------------------------------------------------------
;     SPIxCON : SPI 1 Control Register
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv FRMEN, 0x000E
         .equiv SPIFSD, 0x000D
         .equiv DISSDO, 0x000B
         .equiv MODE16, 0x000A
         .equiv SMP, 0x0009
         .equiv CKE, 0x0008

; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv SSEN, 0x0007
         .equiv CKP, 0x0006
         .equiv MSTEN, 0x0005
         .equiv SPRE2, 0x0004
         .equiv SPRE1, 0x0003
         .equiv SPRE0, 0x0002
         .equiv PPRE1, 0x0001
         .equiv PPRE0, 0x0000



;==============================================================================
;
; 12.  A/D Module Bit Position Definitions for SFRs
;     & SFR High/Low byte definitions.
;==============================================================================
; 12a. SFR Definitions
;------------------------------------------------------------------------------
         .equiv ADCBUF0L, _ADCBUF0
         .equiv ADCBUF0H, _ADCBUF0+1
         .equiv ADCBUF1L, _ADCBUF1
         .equiv ADCBUF1H, _ADCBUF1+1
         .equiv ADCBUF2L, _ADCBUF2
         .equiv ADCBUF2H, _ADCBUF2+1
         .equiv ADCBUF3L, _ADCBUF3
         .equiv ADCBUF3H, _ADCBUF3+1
         .equiv ADCBUF4L, _ADCBUF4
         .equiv ADCBUF4H, _ADCBUF4+1
         .equiv ADCBUF5L, _ADCBUF5
         .equiv ADCBUF5H, _ADCBUF5+1
         .equiv ADCBUF6L, _ADCBUF6
         .equiv ADCBUF6H, _ADCBUF6+1
         .equiv ADCBUF7L, _ADCBUF7
         .equiv ADCBUF7H, _ADCBUF7+1
         .equiv ADCBUF8L, _ADCBUF8
         .equiv ADCBUF8H, _ADCBUF8+1
         .equiv ADCBUF9L, _ADCBUF9
         .equiv ADCBUF9H, _ADCBUF9+1
         .equiv ADCBUFAL, _ADCBUFA
         .equiv ADCBUFAH, _ADCBUFA+1
         .equiv ADCBUFBL, _ADCBUFB
         .equiv ADCBUFBH, _ADCBUFB+1
         .equiv ADCBUFCL, _ADCBUFC
         .equiv ADCBUFCH, _ADCBUFC+1
         .equiv ADCBUFDL, _ADCBUFD
         .equiv ADCBUFDH, _ADCBUFD+1
         .equiv ADCBUFEL, _ADCBUFE
         .equiv ADCBUFEH, _ADCBUFE+1
         .equiv ADCBUFFL, _ADCBUFF
         .equiv ADCBUFFH, _ADCBUFF+1
         .equiv ADCON1L, _ADCON1                ; See ADCON1L through ADPCFGH
         .equiv ADCON1H, _ADCON1+1              ;  description in sub-section below
         .equiv ADCON2L, _ADCON2
         .equiv ADCON2H, _ADCON2+1
         .equiv ADCON3L, _ADCON3
         .equiv ADCON3H, _ADCON3+1
         .equiv ADCHSL, _ADCHS
         .equiv ADCHSH, _ADCHS+1
         .equiv ADPCFGL, _ADPCFG
         .equiv ADPCFGH, _ADPCFG+1
         .equiv ADCSSLL, _ADCSSL
         .equiv ADCSSLH, _ADCSSL+1

;------------------------------------------------------------------------------
; 12b. Bit Position Definitions for some SFRs
;------------------------------------------------------------------------------
;     ADCON1 : A/D Control Register 1
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv ADON, 0x000F
         .equiv ADSIDL, 0x000D
         .equiv FORM1, 0x0009
         .equiv FORM0, 0x0008

; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv SSRC2, 0x0007
         .equiv SSRC1, 0x0006
         .equiv SSRC0, 0x0005
         .equiv SIMSAM, 0x0003
         .equiv ASAM, 0x0002
         .equiv SAMP, 0x0001
         .equiv DONE, 0x0000

;------------------------------------------------------------------------------
;     ADCON2 : A/D Control Register 2
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv VCFG2, 0x000F
         .equiv VCFG1, 0x000E
         .equiv VCFG0, 0x000D
         .equiv CSCNA, 0x000A
         .equiv CHPS1, 0x0009
         .equiv CHPS0, 0x0008

; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv BUFS, 0x0007
         .equiv SMPI3, 0x0005
         .equiv SMPI2, 0x0004
         .equiv SMPI1, 0x0003
         .equiv SMPI0, 0x0002
         .equiv BUFM, 0x0001
         .equiv ALTS, 0x0000

;------------------------------------------------------------------------------
;     ADCON3 : A/D Control Register 3
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv SAMC4, 0x000C
         .equiv SAMC3, 0x000B
         .equiv SAMC2, 0x000A
         .equiv SAMC1, 0x0009
         .equiv SAMC0, 0x0008

; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv ADRC, 0x0007
         .equiv ADCS5, 0x0005
         .equiv ADCS4, 0x0004
         .equiv ADCS3, 0x0003
         .equiv ADCS2, 0x0002
         .equiv ADCS1, 0x0001
         .equiv ADCS0, 0x0000

;------------------------------------------------------------------------------
;     ADCHS : A/D Input Channel Select Register
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv CH123NB1, 0x000F
         .equiv CH123NB0, 0x000E
         .equiv CH123SB, 0x000D
         .equiv CH0NB, 0x000C
         .equiv CH0SB3, 0x000B
         .equiv CH0SB2, 0x000A
         .equiv CH0SB1, 0x0009
         .equiv CH0SB0, 0x0008

; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv CH123NA1, 0x0007
         .equiv CH123NA0, 0x0006
         .equiv CH123SA, 0x0005
         .equiv CH0NA, 0x0004
         .equiv CH0SA3, 0x0003
         .equiv CH0SA2, 0x0002
         .equiv CH0SA1, 0x0001
         .equiv CH0SA0, 0x0000

;------------------------------------------------------------------------------
;     ADPCFG : A/D Port Configuration Register
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv PCFG15, 0x000F
         .equiv PCFG14, 0x000E
         .equiv PCFG13, 0x000D
         .equiv PCFG12, 0x000C
         .equiv PCFG11, 0x000B
         .equiv PCFG10, 0x000A
         .equiv PCFG9, 0x0009
         .equiv PCFG8, 0x0008

; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv PCFG7, 0x0007
         .equiv PCFG6, 0x0006
         .equiv PCFG5, 0x0005
         .equiv PCFG4, 0x0004
         .equiv PCFG3, 0x0003
         .equiv PCFG2, 0x0002
         .equiv PCFG1, 0x0001
         .equiv PCFG0, 0x0000

;------------------------------------------------------------------------------
;     ADCSSL : A/D Input Scan Select Register ;SFR present only in 12-bit ADC
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv CSSL15, 0x000F
         .equiv CSSL14, 0x000E
         .equiv CSSL13, 0x000D
         .equiv CSSL12, 0x000C
         .equiv CSSL11, 0x000B
         .equiv CSSL10, 0x000A
         .equiv CSSL9, 0x0009
         .equiv CSSL8, 0x0008

; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv CSSL7, 0x0007
         .equiv CSSL6, 0x0006
         .equiv CSSL5, 0x0005
         .equiv CSSL4, 0x0004
         .equiv CSSL3, 0x0003
         .equiv CSSL2, 0x0002
         .equiv CSSL1, 0x0001
         .equiv CSSL0, 0x0000


;==============================================================================
;
; 13.  Port B: General Purpose I/O Bit Position Definitions for SFRs
;     & SFR High/Low byte definitions.
;==============================================================================
; 13a. SFR Definitions
;------------------------------------------------------------------------------
         .equiv TRISBL, _TRISB                  ; See all SFR descriptions in
                                                ;  sub-section below
         .equiv PORTBL, _PORTB
         .equiv LATBL, _LATB


;------------------------------------------------------------------------------
; 13b. Bit Position Definitions for some SFRs
;------------------------------------------------------------------------------
;     TRISB : Port B Data Direction Control Register
;------------------------------------------------------------------------------
; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv TRISB5, 0x0005
         .equiv TRISB4, 0x0004
         .equiv TRISB3, 0x0003
         .equiv TRISB2, 0x0002
         .equiv TRISB1, 0x0001
         .equiv TRISB0, 0x0000

;------------------------------------------------------------------------------
;     PORTB : Read Port B Pin / Write Port B Latch Register
;------------------------------------------------------------------------------
; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv RB5, 0x0005
         .equiv RB4, 0x0004
         .equiv RB3, 0x0003
         .equiv RB2, 0x0002
         .equiv RB1, 0x0001
         .equiv RB0, 0x0000

;------------------------------------------------------------------------------
;     LATB : Read / Write Port B Latch Register
;------------------------------------------------------------------------------
; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv LATB5, 0x0005
         .equiv LATB4, 0x0004
         .equiv LATB3, 0x0003
         .equiv LATB2, 0x0002
         .equiv LATB1, 0x0001
         .equiv LATB0, 0x0000

;==============================================================================
;
; 14.  Port C: General Purpose I/O Bit Position Definitions for SFRs
;     & SFR High/Low byte definitions.
;==============================================================================
; 14a. SFR Definitions
;------------------------------------------------------------------------------
                                                ; See all SFR descriptions in
         .equiv TRISCH, _TRISC+1                ;  sub-section below
         .equiv PORTCH, _PORTC+1
         .equiv LATCH, _LATC+1

;------------------------------------------------------------------------------
; 14b. Bit Position Definitions for some SFRs
;------------------------------------------------------------------------------
;     TRISC : Port C Data Direction Control Register
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv TRISC15, 0x000F
         .equiv TRISC14, 0x000E
         .equiv TRISC13, 0x000D

;------------------------------------------------------------------------------
;     PORTC : Read Port C Pin / Write Port C Latch Register
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv RC15, 0x000F
         .equiv RC14, 0x000E
         .equiv RC13, 0x000D

;------------------------------------------------------------------------------
;     LATC : Read / Write Port C Latch Register
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv LATC15, 0x000F
         .equiv LATC14, 0x000E
         .equiv LATC13, 0x000D

;==============================================================================
;
; 15.  Port D: General Purpose I/O Bit Position Definitions for SFRs
;     & SFR High/Low byte definitions.
;==============================================================================
; 15a. SFR Definitions
;------------------------------------------------------------------------------
         .equiv TRISDL, _TRISD                  ; See all SFR descriptions in
                                                ;  sub-section below
         .equiv PORTDL, _PORTD
         .equiv LATDL, _LATD


;------------------------------------------------------------------------------
; 15b. Bit Position Definitions for some SFRs
;------------------------------------------------------------------------------
;     TRISD : Port D Data Direction Control Register
;------------------------------------------------------------------------------
; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv TRISD1, 0x0001
         .equiv TRISD0, 0x0000

;------------------------------------------------------------------------------
;     PORTD : Read Port D Pin / Write Port D Latch Register
;------------------------------------------------------------------------------
; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv RD1, 0x0001
         .equiv RD0, 0x0000

;------------------------------------------------------------------------------
;     LATD : Read / Write Port D Latch Register
;------------------------------------------------------------------------------
; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv LATD1, 0x0001
         .equiv LATD0, 0x0000

;==============================================================================
;
; 16.  Port E: General Purpose I/O Bit Position Definitions for SFRs
;     & SFR High/Low byte definitions.
;==============================================================================
; 16a. SFR Definitions
;------------------------------------------------------------------------------
         .equiv TRISEL, _TRISE                  ; See all SFR descriptions in
         .equiv TRISEH, _TRISE+1                ;  sub-section below
         .equiv PORTEL, _PORTE
         .equiv PORTEH, _PORTE+1
         .equiv LATEL, _LATE
         .equiv LATEH, _LATE+1

;------------------------------------------------------------------------------
; 16b. Bit Position Definitions for some SFRs
;------------------------------------------------------------------------------
;     TRISE : Port E Data Direction Control Register
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv TRISE8, 0x0008

; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv TRISE5, 0x0005
         .equiv TRISE4, 0x0004
         .equiv TRISE3, 0x0003
         .equiv TRISE2, 0x0002
         .equiv TRISE1, 0x0001
         .equiv TRISE0, 0x0000

;------------------------------------------------------------------------------
;     PORTE : Read Port E Pin / Write Port E Latch Register
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv RE8, 0x0008

; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv RE5, 0x0005
         .equiv RE4, 0x0004
         .equiv RE3, 0x0003
         .equiv RE2, 0x0002
         .equiv RE1, 0x0001
         .equiv RE0, 0x0000

;------------------------------------------------------------------------------
;     LATE : Read / Write Port E Latch Register
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv LATE8, 0x0008

; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv LATE5, 0x0005
         .equiv LATE4, 0x0004
         .equiv LATE3, 0x0003
         .equiv LATE2, 0x0002
         .equiv LATE1, 0x0001
         .equiv LATE0, 0x0000

;==============================================================================
;
; 17.  Port F: General Purpose I/O Bit Position Definitions for SFRs
;     & SFR High/Low byte definitions.
;==============================================================================
; 17a. SFR Definitions
;------------------------------------------------------------------------------
         .equiv TRISFL, _TRISF                  ; See all SFR descriptions in
                                                ;  sub-section below
         .equiv PORTFL, _PORTF
         .equiv LATFL, _LATF


;------------------------------------------------------------------------------
; 17b. Bit Position Definitions for some SFRs
;------------------------------------------------------------------------------
;     TRISF : Port F Data Direction Control Register
;------------------------------------------------------------------------------
; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv TRISF3, 0x0003
         .equiv TRISF2, 0x0002

;------------------------------------------------------------------------------
;     PORTF : Read Port F Pin / Write Port F Latch Register
;------------------------------------------------------------------------------
; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv RF3, 0x0003
         .equiv RF2, 0x0002

;------------------------------------------------------------------------------
;     LATF : Read / Write Port F Latch Register
;------------------------------------------------------------------------------
; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv LATF3, 0x0003
         .equiv LATF2, 0x0002


;==============================================================================
;
; 18.  System Integration Block Bit Position Definitions for SFRs
;     & SFR High/Low byte definitions.
;==============================================================================
; 18a. SFR Definitions
;------------------------------------------------------------------------------
         .equiv RCONL, _RCON                    ; See all SFR descriptions in
         .equiv RCONH, _RCON+1                  ;  section 22b
         .equiv OSCCONL, _OSCCON
         .equiv OSCCONH, _OSCCON+1
         .equiv OSCTUNL, _OSCTUN

;------------------------------------------------------------------------------
; 18b. Bit Position Definitions for some SFRs
;------------------------------------------------------------------------------
;     RCON : Reset and System Control Register
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv TRAPR, 0x000F
         .equiv IOPUWR, 0x000E
         .equiv BGST, 0x000D

; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv EXTR, 0x0007
         .equiv SWR, 0x0006
         .equiv SWDTEN, 0x0005
         .equiv WDTO, 0x0004
         .equiv SLEEP, 0x0003
         .equiv IDLE, 0x0002
         .equiv BOR, 0x0001
         .equiv POR, 0x0000

;------------------------------------------------------------------------------
;     OSCCON : Oscillator Control Register
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv COSC2, 0x000E
         .equiv COSC1, 0x000D
         .equiv COSC0, 0x000C
         .equiv NOSC2, 0x000A
         .equiv NOSC1, 0x0009
         .equiv NOSC0, 0x0008

; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv POST1, 0x0007
         .equiv POST0, 0x0006
         .equiv LOCK, 0x0005
         .equiv CF, 0x0003
         .equiv LPOSCEN, 0x0001
         .equiv OSWEN, 0x0000

;------------------------------------------------------------------------------
;     OSCTUN : Internal FRC Oscillator Tuning Register
;------------------------------------------------------------------------------
; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv TUN3, 0x0003
         .equiv TUN2, 0x0002
         .equiv TUN1, 0x0001
         .equiv TUN0, 0x0000

;==============================================================================
;
; 19.  Non Volatile Memory Bit Position Definitions for SFRs
;     & SFR High/Low byte definitions.
;==============================================================================
; 19a. SFR Definitions
;------------------------------------------------------------------------------
         .equiv NVMCONL, _NVMCON                ; See NVMCON description in
         .equiv NVMCONH, _NVMCON+1              ;  sub-section below
         .equiv NVMADRL, _NVMADR
         .equiv NVMADRH, _NVMADR+1
         .equiv NVMKEYL, _NVMKEY
         .equiv NVMKEYH, _NVMKEY+1

;------------------------------------------------------------------------------
; 19b. Bit Position Definitions for some SFRs
;------------------------------------------------------------------------------
;     NVMCON : Non Volatile Memory Control Register
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv WR, 0x000F
         .equiv WREN, 0x000E
         .equiv WRERR, 0x000D
         .equiv TWRI, 0x0008

; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv PROGOP6, 0x0006
         .equiv PROGOP5, 0x0005
         .equiv PROGOP4, 0x0004
         .equiv PROGOP3, 0x0003
         .equiv PROGOP2, 0x0002
         .equiv PROGOP1, 0x0001
         .equiv PROGOP0, 0x0000


;==============================================================================
;
; 20. Peripheral Module Disable Bit Position Definitions for SFRs
;     & SFR High/Low byte definitions.
;==============================================================================
; 20a. SFR Definitions
;------------------------------------------------------------------------------
         .equiv PMD1L, _PMD1                    ; See bit descrpitions below
         .equiv PMD1H, _PMD1+1                  ;  for all files
         .equiv PMD2L, _PMD2
         .equiv PMD2H, _PMD2+1

;------------------------------------------------------------------------------
; 20b. Bit Position Definitions for some SFRs
;------------------------------------------------------------------------------
;  PMD1: Peripheral Module Disable 1 Register
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv T5MD, 0x000F
         .equiv T4MD, 0x000E
         .equiv T3MD, 0x000D
         .equiv T2MD, 0x000C
         .equiv T1MD, 0x000B
         .equiv QEIMD, 0x000A
         .equiv PWMMD, 0x0009

; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv I2CMD, 0x0007
         .equiv U1MD, 0x0005
         .equiv SPI1MD, 0x0003
         .equiv ADCMD, 0x0000

;------------------------------------------------------------------------------
;  PMD2: Peripheral Module Disable 2 Register
;------------------------------------------------------------------------------
; High Byte (Odd Address)
; Bit Positions defined below:
         .equiv IC8MD, 0x000F
         .equiv IC7MD, 0x000E
         .equiv IC2MD, 0x0009
         .equiv IC1MD, 0x0008

; Low Byte (Even Address)
; Bit Positions defined below:
         .equiv OC2MD, 0x0001
         .equiv OC1MD, 0x0000

;==============================================================================
;
; 21. Useful constants that ease assembly-level programming
;
;==============================================================================
         .equiv SLEEP_MODE, 0x0000
         .equiv IDLE_MODE, 0x0001


;==============================================================================
;
; 22. Macros and Fuse Configuration Definitions
;
;==============================================================================
; Notes:
;    1. Use the following fuse configuration settings by calling the "config"
;       macro(defined at the end of this section)or directly in assembly source
;       code.
;    2. Configuration Fuses exist in Program Space and their locations are
;       defined in the device linker script.
;==============================================================================
;------------------------------------------------------------------------------
; FOSC: Oscillator Selection Fuse
;------------------------------------------------------------------------------
        .equiv    CSW_FSCM_OFF, 0xFFFF
        .equiv    CSW_ON_FSCM_OFF, 0x7FFF
        .equiv    CSW_FSCM_ON, 0x3FFF
        .equiv    LP, 0xF8FF
        .equiv    FRC, 0xF9FF
        .equiv    LPRC, 0xFAFF
        .equiv    EXT, 0xFBFF
        .equiv    ECIO_PLL4, 0xFFED
        .equiv    ECIO_PLL8, 0xFFEE
        .equiv    ECIO_PLL16, 0xFFEF
        .equiv    FRC_PLL4, 0xFFE1
        .equiv    FRC_PLL8, 0xFFEA
        .equiv    FRC_PLL16, 0xFFE3
        .equiv    XT_PLL4, 0xFFE5
        .equiv    XT_PLL8, 0xFFE6
        .equiv    XT_PLL16, 0xFFE7
        .equiv    HS2_PLL4, 0xFFF1
        .equiv    HS2_PLL8, 0xFFF2
        .equiv    HS2_PLL16, 0xFFF3
        .equiv    HS3_PLL4, 0xFFF5
        .equiv    HS3_PLL8, 0xFFF6
        .equiv    HS3_PLL16, 0xFFF7
        .equiv    ECIO, 0xFBEC
        .equiv    XT, 0xFBE4
        .equiv    HS, 0xFBE2
        .equiv    EC, 0xFBEB
        .equiv    ERC, 0xFBE9
        .equiv    ERCIO, 0xFBE8
        .equiv    XTL, 0xFBE0


;------------------------------------------------------------------------------
; FWDT: Watch Dog Timer Configuration Fuse
;------------------------------------------------------------------------------
         .equiv WDT_ON, 0xFFFF
         .equiv WDT_OFF, 0x7FFF
         .equiv WDTPSA_1, 0xFFCF
         .equiv WDTPSA_8, 0xFFDF
         .equiv WDTPSA_64, 0xFFEF
         .equiv WDTPSA_512, 0xFFFF
         .equiv WDTPSB_1, 0xFFF0
         .equiv WDTPSB_2, 0xFFF1
         .equiv WDTPSB_3, 0xFFF2
         .equiv WDTPSB_4, 0xFFF3
         .equiv WDTPSB_5, 0xFFF4
         .equiv WDTPSB_6, 0xFFF5
         .equiv WDTPSB_7, 0xFFF6
         .equiv WDTPSB_8, 0xFFF7
         .equiv WDTPSB_9, 0xFFF8
         .equiv WDTPSB_10, 0xFFF9
         .equiv WDTPSB_11, 0xFFFA
         .equiv WDTPSB_12, 0xFFFB
         .equiv WDTPSB_13, 0xFFFC
         .equiv WDTPSB_14, 0xFFFD
         .equiv WDTPSB_15, 0xFFFE
         .equiv WDTPSB_16, 0xFFFF

;------------------------------------------------------------------------------
; FBORPOR: Brown-out and Power-on Reset Fuse
;------------------------------------------------------------------------------
         .equiv MCLR_EN, 0xFFFF
         .equiv MCLR_DIS, 0x7FFF
         .equiv RST_IOPIN, 0xFFFF
         .equiv RST_PWMPIN, 0xFBFF
         .equiv PWMxH_ACT_HI, 0xFFFF
         .equiv PWMxH_ACT_LO, 0xFDFF
         .equiv PWMxL_ACT_HI, 0xFFFF
         .equiv PWMxL_ACT_LO, 0xFEFF
         .equiv PBOR_ON, 0xFFFF
         .equiv PBOR_OFF, 0xFF7F
         .equiv BORV_20, 0xFFFF
         .equiv BORV_27, 0xFFEF
         .equiv BORV_42, 0xFFDF
         .equiv BORV_45, 0xFFCF
         .equiv PWRT_OFF, 0xFFFC
         .equiv PWRT_4, 0xFFFD
         .equiv PWRT_16, 0xFFFE
         .equiv PWRT_64, 0xFFFF

;------------------------------------------------------------------------------
; FGS: Genneral (Code) Segment Fuse
;------------------------------------------------------------------------------
         .equiv CODE_PROT_OFF, 0xFFFF
         .equiv CODE_PROT_ON, 0xFFF9

;------------------------------------------------------------------------------
; Setting configuration fuses
;------------------------------------------------------------------------------
; Setting configuration fuses using macros:
;==========================================
; The following macro named "config" can be used to set configuration fuses:
        .macro config REG, VALUE
        .section \REG.sec, code
        .global \REG
\REG:   .pword \VALUE
        .endm
; For e.g., to set the FOSC fuse using the macro above, the following line of
; code can be pasted at the only at the beginning of the assembly source code,
; immediately below the ".include" directive.
;
;        config __FOSC, CSW_FSCM_ON & EC_PLL16
;
; This would enable the external clock with the PLL set to 16x and further,
; enable clock switching and failsafe clock monitoring.
;
; Similarly, to set the FBORPOR fuse, paste the following :
;
;        config __FBORPOR, PBOR_ON & BORV_27 & PWRT_ON_64 & MCLR_DIS
;
; This would enable Brown-out Reset at 2.7 Volts and initialize the Power-up
; timer to 64 milliseconds and configure the use of the MCLR pin for I/O.
; Given below, is a complete list of settings valid to each of the fuses:
; (Paste the ones relevant to your application at the beginning of the assembly
; source, immediately below the ".include" directive.)
;
;                FOSC:
;               ======
;               config __FOSC, CSW_FSCM_OFF & EC
;               config __FOSC, CSW_FSCM_OFF & ECIO
;               config __FOSC, CSW_FSCM_OFF & ECIO_PLL4
;               config __FOSC, CSW_FSCM_OFF & ECIO_PLL8
;               config __FOSC, CSW_FSCM_OFF & ECIO_PLL16
;               config __FOSC, CSW_FSCM_OFF & ERC
;               config __FOSC, CSW_FSCM_OFF & ERCIO
;               config __FOSC, CSW_FSCM_OFF & XT
;               config __FOSC, CSW_FSCM_OFF & XT_PLL4
;               config __FOSC, CSW_FSCM_OFF & XT_PLL8
;               config __FOSC, CSW_FSCM_OFF & XT_PLL16
;               config __FOSC, CSW_FSCM_OFF & XTL
;               config __FOSC, CSW_FSCM_OFF & FRC_PLL4
;               config __FOSC, CSW_FSCM_OFF & FRC_PLL8
;               config __FOSC, CSW_FSCM_OFF & FRC_PLL16
;               config __FOSC, CSW_FSCM_OFF & HS
;               config __FOSC, CSW_FSCM_OFF & HS2_PLL4
;               config __FOSC, CSW_FSCM_OFF & HS2_PLL8
;               config __FOSC, CSW_FSCM_OFF & HS2_PLL16
;               config __FOSC, CSW_FSCM_OFF & HS3_PLL4
;               config __FOSC, CSW_FSCM_OFF & HS3_PLL8
;               config __FOSC, CSW_FSCM_OFF & HS3_PLL16
;               config __FOSC, CSW_FSCM_OFF & LP & EC
;               config __FOSC, CSW_FSCM_OFF & LP & ECIO
;               config __FOSC, CSW_FSCM_OFF & LP & ECIO_PLL4
;               config __FOSC, CSW_FSCM_OFF & LP & ECIO_PLL8
;               config __FOSC, CSW_FSCM_OFF & LP & ECIO_PLL16
;               config __FOSC, CSW_FSCM_OFF & LP & ERC
;               config __FOSC, CSW_FSCM_OFF & LP & ERCIO
;               config __FOSC, CSW_FSCM_OFF & LP & XT
;               config __FOSC, CSW_FSCM_OFF & LP & XT_PLL4
;               config __FOSC, CSW_FSCM_OFF & LP & XT_PLL8
;               config __FOSC, CSW_FSCM_OFF & LP & XT_PLL16
;               config __FOSC, CSW_FSCM_OFF & LP & XTL
;               config __FOSC, CSW_FSCM_OFF & LP & FRC_PLL4
;               config __FOSC, CSW_FSCM_OFF & LP & FRC_PLL8
;               config __FOSC, CSW_FSCM_OFF & LP & FRC_PLL16
;               config __FOSC, CSW_FSCM_OFF & LP & HS
;               config __FOSC, CSW_FSCM_OFF & LP & HS2_PLL4
;               config __FOSC, CSW_FSCM_OFF & LP & HS2_PLL8
;               config __FOSC, CSW_FSCM_OFF & LP & HS2_PLL16
;               config __FOSC, CSW_FSCM_OFF & LP & HS3_PLL4
;               config __FOSC, CSW_FSCM_OFF & LP & HS3_PLL8
;               config __FOSC, CSW_FSCM_OFF & LP & HS3_PLL16
;               config __FOSC, CSW_FSCM_OFF & FRC & EC
;               config __FOSC, CSW_FSCM_OFF & FRC & ECIO
;               config __FOSC, CSW_FSCM_OFF & FRC & ECIO_PLL4
;               config __FOSC, CSW_FSCM_OFF & FRC & ECIO_PLL8
;               config __FOSC, CSW_FSCM_OFF & FRC & ECIO_PLL16
;               config __FOSC, CSW_FSCM_OFF & FRC & ERC
;               config __FOSC, CSW_FSCM_OFF & FRC & ERCIO
;               config __FOSC, CSW_FSCM_OFF & FRC & XT
;               config __FOSC, CSW_FSCM_OFF & FRC & XT_PLL4
;               config __FOSC, CSW_FSCM_OFF & FRC & XT_PLL8
;               config __FOSC, CSW_FSCM_OFF & FRC & XT_PLL16
;               config __FOSC, CSW_FSCM_OFF & FRC & XTL
;               config __FOSC, CSW_FSCM_OFF & FRC & FRC_PLL4
;               config __FOSC, CSW_FSCM_OFF & FRC & FRC_PLL8
;               config __FOSC, CSW_FSCM_OFF & FRC & FRC_PLL16
;               config __FOSC, CSW_FSCM_OFF & FRC & HS
;               config __FOSC, CSW_FSCM_OFF & FRC & HS2_PLL4
;               config __FOSC, CSW_FSCM_OFF & FRC & HS2_PLL8
;               config __FOSC, CSW_FSCM_OFF & FRC & HS2_PLL16
;               config __FOSC, CSW_FSCM_OFF & FRC & HS3_PLL4
;               config __FOSC, CSW_FSCM_OFF & FRC & HS3_PLL8
;               config __FOSC, CSW_FSCM_OFF & FRC & HS3_PLL16
;               config __FOSC, CSW_FSCM_OFF & LPRC & EC
;               config __FOSC, CSW_FSCM_OFF & LPRC & ECIO
;               config __FOSC, CSW_FSCM_OFF & LPRC & ECIO_PLL4
;               config __FOSC, CSW_FSCM_OFF & LPRC & ECIO_PLL8
;               config __FOSC, CSW_FSCM_OFF & LPRC & ECIO_PLL16
;               config __FOSC, CSW_FSCM_OFF & LPRC & ERC
;               config __FOSC, CSW_FSCM_OFF & LPRC & ERCIO
;               config __FOSC, CSW_FSCM_OFF & LPRC & XT
;               config __FOSC, CSW_FSCM_OFF & LPRC & XT_PLL4
;               config __FOSC, CSW_FSCM_OFF & LPRC & XT_PLL8
;               config __FOSC, CSW_FSCM_OFF & LPRC & XT_PLL16
;               config __FOSC, CSW_FSCM_OFF & LPRC & XTL
;               config __FOSC, CSW_FSCM_OFF & LPRC & FRC_PLL4
;               config __FOSC, CSW_FSCM_OFF & LPRC & FRC_PLL8
;               config __FOSC, CSW_FSCM_OFF & LPRC & FRC_PLL16
;               config __FOSC, CSW_FSCM_OFF & LPRC & HS
;               config __FOSC, CSW_FSCM_OFF & LPRC & HS2_PLL4
;               config __FOSC, CSW_FSCM_OFF & LPRC & HS2_PLL8
;               config __FOSC, CSW_FSCM_OFF & LPRC & HS2_PLL16
;               config __FOSC, CSW_FSCM_OFF & LPRC & HS3_PLL4
;               config __FOSC, CSW_FSCM_OFF & LPRC & HS3_PLL8
;               config __FOSC, CSW_FSCM_OFF & LPRC & HS3_PLL16
;               config __FOSC, CSW_FSCM_OFF & EXT & EC
;               config __FOSC, CSW_FSCM_OFF & EXT & ECIO
;               config __FOSC, CSW_FSCM_OFF & EXT & ECIO_PLL4
;               config __FOSC, CSW_FSCM_OFF & EXT & ECIO_PLL8
;               config __FOSC, CSW_FSCM_OFF & EXT & ECIO_PLL16
;               config __FOSC, CSW_FSCM_OFF & EXT & ERC
;               config __FOSC, CSW_FSCM_OFF & EXT & ERCIO
;               config __FOSC, CSW_FSCM_OFF & EXT & XT
;               config __FOSC, CSW_FSCM_OFF & EXT & XT_PLL4
;               config __FOSC, CSW_FSCM_OFF & EXT & XT_PLL8
;               config __FOSC, CSW_FSCM_OFF & EXT & XT_PLL16
;               config __FOSC, CSW_FSCM_OFF & EXT & XTL
;               config __FOSC, CSW_FSCM_OFF & EXT & FRC_PLL4
;               config __FOSC, CSW_FSCM_OFF & EXT & FRC_PLL8
;               config __FOSC, CSW_FSCM_OFF & EXT & FRC_PLL16
;               config __FOSC, CSW_FSCM_OFF & EXT & HS
;               config __FOSC, CSW_FSCM_OFF & EXT & HS2_PLL4
;               config __FOSC, CSW_FSCM_OFF & EXT & HS2_PLL8
;               config __FOSC, CSW_FSCM_OFF & EXT & HS2_PLL16
;               config __FOSC, CSW_FSCM_OFF & EXT & HS3_PLL4
;               config __FOSC, CSW_FSCM_OFF & EXT & HS3_PLL8
;               config __FOSC, CSW_FSCM_OFF & EXT & HS3_PLL16
;               config __FOSC, CSW_ON_FSCM_OFF & EC
;               config __FOSC, CSW_ON_FSCM_OFF & ECIO
;               config __FOSC, CSW_ON_FSCM_OFF & ECIO_PLL4
;               config __FOSC, CSW_ON_FSCM_OFF & ECIO_PLL8
;               config __FOSC, CSW_ON_FSCM_OFF & ECIO_PLL16
;               config __FOSC, CSW_ON_FSCM_OFF & ERC
;               config __FOSC, CSW_ON_FSCM_OFF & ERCIO
;               config __FOSC, CSW_ON_FSCM_OFF & XT
;               config __FOSC, CSW_ON_FSCM_OFF & XT_PLL4
;               config __FOSC, CSW_ON_FSCM_OFF & XT_PLL8
;               config __FOSC, CSW_ON_FSCM_OFF & XT_PLL16
;               config __FOSC, CSW_ON_FSCM_OFF & XTL
;               config __FOSC, CSW_ON_FSCM_OFF & FRC_PLL4
;               config __FOSC, CSW_ON_FSCM_OFF & FRC_PLL8
;               config __FOSC, CSW_ON_FSCM_OFF & FRC_PLL16
;               config __FOSC, CSW_ON_FSCM_OFF & HS
;               config __FOSC, CSW_ON_FSCM_OFF & HS2_PLL4
;               config __FOSC, CSW_ON_FSCM_OFF & HS2_PLL8
;               config __FOSC, CSW_ON_FSCM_OFF & HS2_PLL16
;               config __FOSC, CSW_ON_FSCM_OFF & HS3_PLL4
;               config __FOSC, CSW_ON_FSCM_OFF & HS3_PLL8
;               config __FOSC, CSW_ON_FSCM_OFF & HS3_PLL16
;               config __FOSC, CSW_ON_FSCM_OFF & LP & EC
;               config __FOSC, CSW_ON_FSCM_OFF & LP & ECIO
;               config __FOSC, CSW_ON_FSCM_OFF & LP & ECIO_PLL4
;               config __FOSC, CSW_ON_FSCM_OFF & LP & ECIO_PLL8
;               config __FOSC, CSW_ON_FSCM_OFF & LP & ECIO_PLL16
;               config __FOSC, CSW_ON_FSCM_OFF & LP & ERC
;               config __FOSC, CSW_ON_FSCM_OFF & LP & ERCIO
;               config __FOSC, CSW_ON_FSCM_OFF & LP & XT
;               config __FOSC, CSW_ON_FSCM_OFF & LP & XT_PLL4
;               config __FOSC, CSW_ON_FSCM_OFF & LP & XT_PLL8
;               config __FOSC, CSW_ON_FSCM_OFF & LP & XT_PLL16
;               config __FOSC, CSW_ON_FSCM_OFF & LP & XTL
;               config __FOSC, CSW_ON_FSCM_OFF & LP & FRC_PLL4
;               config __FOSC, CSW_ON_FSCM_OFF & LP & FRC_PLL8
;               config __FOSC, CSW_ON_FSCM_OFF & LP & FRC_PLL16
;               config __FOSC, CSW_ON_FSCM_OFF & LP & HS
;               config __FOSC, CSW_ON_FSCM_OFF & LP & HS2_PLL4
;               config __FOSC, CSW_ON_FSCM_OFF & LP & HS2_PLL8
;               config __FOSC, CSW_ON_FSCM_OFF & LP & HS2_PLL16
;               config __FOSC, CSW_ON_FSCM_OFF & LP & HS3_PLL4
;               config __FOSC, CSW_ON_FSCM_OFF & LP & HS3_PLL8
;               config __FOSC, CSW_ON_FSCM_OFF & LP & HS3_PLL16
;               config __FOSC, CSW_ON_FSCM_OFF & FRC & EC
;               config __FOSC, CSW_ON_FSCM_OFF & FRC & ECIO
;               config __FOSC, CSW_ON_FSCM_OFF & FRC & ECIO_PLL4
;               config __FOSC, CSW_ON_FSCM_OFF & FRC & ECIO_PLL8
;               config __FOSC, CSW_ON_FSCM_OFF & FRC & ECIO_PLL16
;               config __FOSC, CSW_ON_FSCM_OFF & FRC & ERC
;               config __FOSC, CSW_ON_FSCM_OFF & FRC & ERCIO
;               config __FOSC, CSW_ON_FSCM_OFF & FRC & XT
;               config __FOSC, CSW_ON_FSCM_OFF & FRC & XT_PLL4
;               config __FOSC, CSW_ON_FSCM_OFF & FRC & XT_PLL8
;               config __FOSC, CSW_ON_FSCM_OFF & FRC & XT_PLL16
;               config __FOSC, CSW_ON_FSCM_OFF & FRC & XTL
;               config __FOSC, CSW_ON_FSCM_OFF & FRC & FRC_PLL4
;               config __FOSC, CSW_ON_FSCM_OFF & FRC & FRC_PLL8
;               config __FOSC, CSW_ON_FSCM_OFF & FRC & FRC_PLL16
;               config __FOSC, CSW_ON_FSCM_OFF & FRC & HS
;               config __FOSC, CSW_ON_FSCM_OFF & FRC & HS2_PLL4
;               config __FOSC, CSW_ON_FSCM_OFF & FRC & HS2_PLL8
;               config __FOSC, CSW_ON_FSCM_OFF & FRC & HS2_PLL16
;               config __FOSC, CSW_ON_FSCM_OFF & FRC & HS3_PLL4
;               config __FOSC, CSW_ON_FSCM_OFF & FRC & HS3_PLL8
;               config __FOSC, CSW_ON_FSCM_OFF & FRC & HS3_PLL16
;               config __FOSC, CSW_ON_FSCM_OFF & LPRC & EC
;               config __FOSC, CSW_ON_FSCM_OFF & LPRC & ECIO
;               config __FOSC, CSW_ON_FSCM_OFF & LPRC & ECIO_PLL4
;               config __FOSC, CSW_ON_FSCM_OFF & LPRC & ECIO_PLL8
;               config __FOSC, CSW_ON_FSCM_OFF & LPRC & ECIO_PLL16
;               config __FOSC, CSW_ON_FSCM_OFF & LPRC & ERC
;               config __FOSC, CSW_ON_FSCM_OFF & LPRC & ERCIO
;               config __FOSC, CSW_ON_FSCM_OFF & LPRC & XT
;               config __FOSC, CSW_ON_FSCM_OFF & LPRC & XT_PLL4
;               config __FOSC, CSW_ON_FSCM_OFF & LPRC & XT_PLL8
;               config __FOSC, CSW_ON_FSCM_OFF & LPRC & XT_PLL16
;               config __FOSC, CSW_ON_FSCM_OFF & LPRC & XTL
;               config __FOSC, CSW_ON_FSCM_OFF & LPRC & FRC_PLL4
;               config __FOSC, CSW_ON_FSCM_OFF & LPRC & FRC_PLL8
;               config __FOSC, CSW_ON_FSCM_OFF & LPRC & FRC_PLL16
;               config __FOSC, CSW_ON_FSCM_OFF & LPRC & HS
;               config __FOSC, CSW_ON_FSCM_OFF & LPRC & HS2_PLL4
;               config __FOSC, CSW_ON_FSCM_OFF & LPRC & HS2_PLL8
;               config __FOSC, CSW_ON_FSCM_OFF & LPRC & HS2_PLL16
;               config __FOSC, CSW_ON_FSCM_OFF & LPRC & HS3_PLL4
;               config __FOSC, CSW_ON_FSCM_OFF & LPRC & HS3_PLL8
;               config __FOSC, CSW_ON_FSCM_OFF & LPRC & HS3_PLL16
;               config __FOSC, CSW_ON_FSCM_OFF & EXT & EC
;               config __FOSC, CSW_ON_FSCM_OFF & EXT & ECIO
;               config __FOSC, CSW_ON_FSCM_OFF & EXT & ECIO_PLL4
;               config __FOSC, CSW_ON_FSCM_OFF & EXT & ECIO_PLL8
;               config __FOSC, CSW_ON_FSCM_OFF & EXT & ECIO_PLL16
;               config __FOSC, CSW_ON_FSCM_OFF & EXT & ERC
;               config __FOSC, CSW_ON_FSCM_OFF & EXT & ERCIO
;               config __FOSC, CSW_ON_FSCM_OFF & EXT & XT
;               config __FOSC, CSW_ON_FSCM_OFF & EXT & XT_PLL4
;               config __FOSC, CSW_ON_FSCM_OFF & EXT & XT_PLL8
;               config __FOSC, CSW_ON_FSCM_OFF & EXT & XT_PLL16
;               config __FOSC, CSW_ON_FSCM_OFF & EXT & XTL
;               config __FOSC, CSW_ON_FSCM_OFF & EXT & FRC_PLL4
;               config __FOSC, CSW_ON_FSCM_OFF & EXT & FRC_PLL8
;               config __FOSC, CSW_ON_FSCM_OFF & EXT & FRC_PLL16
;               config __FOSC, CSW_ON_FSCM_OFF & EXT & HS
;               config __FOSC, CSW_ON_FSCM_OFF & EXT & HS2_PLL4
;               config __FOSC, CSW_ON_FSCM_OFF & EXT & HS2_PLL8
;               config __FOSC, CSW_ON_FSCM_OFF & EXT & HS2_PLL16
;               config __FOSC, CSW_ON_FSCM_OFF & EXT & HS3_PLL4
;               config __FOSC, CSW_ON_FSCM_OFF & EXT & HS3_PLL8
;               config __FOSC, CSW_ON_FSCM_OFF & EXT & HS3_PLL16
;               config __FOSC, CSW_FSCM_ON & EC
;               config __FOSC, CSW_FSCM_ON & ECIO
;               config __FOSC, CSW_FSCM_ON & ECIO_PLL4
;               config __FOSC, CSW_FSCM_ON & ECIO_PLL8
;               config __FOSC, CSW_FSCM_ON & ECIO_PLL16
;               config __FOSC, CSW_FSCM_ON & ERC
;               config __FOSC, CSW_FSCM_ON & ERCIO
;               config __FOSC, CSW_FSCM_ON & XT
;               config __FOSC, CSW_FSCM_ON & XT_PLL4
;               config __FOSC, CSW_FSCM_ON & XT_PLL8
;               config __FOSC, CSW_FSCM_ON & XT_PLL16
;               config __FOSC, CSW_FSCM_ON & XTL
;               config __FOSC, CSW_FSCM_ON & FRC_PLL4
;               config __FOSC, CSW_FSCM_ON & FRC_PLL8
;               config __FOSC, CSW_FSCM_ON & FRC_PLL16
;               config __FOSC, CSW_FSCM_ON & HS
;               config __FOSC, CSW_FSCM_ON & HS2_PLL4
;               config __FOSC, CSW_FSCM_ON & HS2_PLL8
;               config __FOSC, CSW_FSCM_ON & HS2_PLL16
;               config __FOSC, CSW_FSCM_ON & HS3_PLL4
;               config __FOSC, CSW_FSCM_ON & HS3_PLL8
;               config __FOSC, CSW_FSCM_ON & HS3_PLL16
;               config __FOSC, CSW_FSCM_ON & LP & EC
;               config __FOSC, CSW_FSCM_ON & LP & ECIO
;               config __FOSC, CSW_FSCM_ON & LP & ECIO_PLL4
;               config __FOSC, CSW_FSCM_ON & LP & ECIO_PLL8
;               config __FOSC, CSW_FSCM_ON & LP & ECIO_PLL16
;               config __FOSC, CSW_FSCM_ON & LP & ERC
;               config __FOSC, CSW_FSCM_ON & LP & ERCIO
;               config __FOSC, CSW_FSCM_ON & LP & XT
;               config __FOSC, CSW_FSCM_ON & LP & XT_PLL4
;               config __FOSC, CSW_FSCM_ON & LP & XT_PLL8
;               config __FOSC, CSW_FSCM_ON & LP & XT_PLL16
;               config __FOSC, CSW_FSCM_ON & LP & XTL
;               config __FOSC, CSW_FSCM_ON & LP & FRC_PLL4
;               config __FOSC, CSW_FSCM_ON & LP & FRC_PLL8
;               config __FOSC, CSW_FSCM_ON & LP & FRC_PLL16
;               config __FOSC, CSW_FSCM_ON & LP & HS
;               config __FOSC, CSW_FSCM_ON & LP & HS2_PLL4
;               config __FOSC, CSW_FSCM_ON & LP & HS2_PLL8
;               config __FOSC, CSW_FSCM_ON & LP & HS2_PLL16
;               config __FOSC, CSW_FSCM_ON & LP & HS3_PLL4
;               config __FOSC, CSW_FSCM_ON & LP & HS3_PLL8
;               config __FOSC, CSW_FSCM_ON & LP & HS3_PLL16
;               config __FOSC, CSW_FSCM_ON & FRC & EC
;               config __FOSC, CSW_FSCM_ON & FRC & ECIO
;               config __FOSC, CSW_FSCM_ON & FRC & ECIO_PLL4
;               config __FOSC, CSW_FSCM_ON & FRC & ECIO_PLL8
;               config __FOSC, CSW_FSCM_ON & FRC & ECIO_PLL16
;               config __FOSC, CSW_FSCM_ON & FRC & ERC
;               config __FOSC, CSW_FSCM_ON & FRC & ERCIO
;               config __FOSC, CSW_FSCM_ON & FRC & XT
;               config __FOSC, CSW_FSCM_ON & FRC & XT_PLL4
;               config __FOSC, CSW_FSCM_ON & FRC & XT_PLL8
;               config __FOSC, CSW_FSCM_ON & FRC & XT_PLL16
;               config __FOSC, CSW_FSCM_ON & FRC & XTL
;               config __FOSC, CSW_FSCM_ON & FRC & FRC_PLL4
;               config __FOSC, CSW_FSCM_ON & FRC & FRC_PLL8
;               config __FOSC, CSW_FSCM_ON & FRC & FRC_PLL16
;               config __FOSC, CSW_FSCM_ON & FRC & HS
;               config __FOSC, CSW_FSCM_ON & FRC & HS2_PLL4
;               config __FOSC, CSW_FSCM_ON & FRC & HS2_PLL8
;               config __FOSC, CSW_FSCM_ON & FRC & HS2_PLL16
;               config __FOSC, CSW_FSCM_ON & FRC & HS3_PLL4
;               config __FOSC, CSW_FSCM_ON & FRC & HS3_PLL8
;               config __FOSC, CSW_FSCM_ON & FRC & HS3_PLL16
;               config __FOSC, CSW_FSCM_ON & LPRC & EC
;               config __FOSC, CSW_FSCM_ON & LPRC & ECIO
;               config __FOSC, CSW_FSCM_ON & LPRC & ECIO_PLL4
;               config __FOSC, CSW_FSCM_ON & LPRC & ECIO_PLL8
;               config __FOSC, CSW_FSCM_ON & LPRC & ECIO_PLL16
;               config __FOSC, CSW_FSCM_ON & LPRC & ERC
;               config __FOSC, CSW_FSCM_ON & LPRC & ERCIO
;               config __FOSC, CSW_FSCM_ON & LPRC & XT
;               config __FOSC, CSW_FSCM_ON & LPRC & XT_PLL4
;               config __FOSC, CSW_FSCM_ON & LPRC & XT_PLL8
;               config __FOSC, CSW_FSCM_ON & LPRC & XT_PLL16
;               config __FOSC, CSW_FSCM_ON & LPRC & XTL
;               config __FOSC, CSW_FSCM_ON & LPRC & FRC_PLL4
;               config __FOSC, CSW_FSCM_ON & LPRC & FRC_PLL8
;               config __FOSC, CSW_FSCM_ON & LPRC & FRC_PLL16
;               config __FOSC, CSW_FSCM_ON & LPRC & HS
;               config __FOSC, CSW_FSCM_ON & LPRC & HS2_PLL4
;               config __FOSC, CSW_FSCM_ON & LPRC & HS2_PLL8
;               config __FOSC, CSW_FSCM_ON & LPRC & HS2_PLL16
;               config __FOSC, CSW_FSCM_ON & LPRC & HS3_PLL4
;               config __FOSC, CSW_FSCM_ON & LPRC & HS3_PLL8
;               config __FOSC, CSW_FSCM_ON & LPRC & HS3_PLL16
;               config __FOSC, CSW_FSCM_ON & EXT & EC
;               config __FOSC, CSW_FSCM_ON & EXT & ECIO
;               config __FOSC, CSW_FSCM_ON & EXT & ECIO_PLL4
;               config __FOSC, CSW_FSCM_ON & EXT & ECIO_PLL8
;               config __FOSC, CSW_FSCM_ON & EXT & ECIO_PLL16
;               config __FOSC, CSW_FSCM_ON & EXT & ERC
;               config __FOSC, CSW_FSCM_ON & EXT & ERCIO
;               config __FOSC, CSW_FSCM_ON & EXT & XT
;               config __FOSC, CSW_FSCM_ON & EXT & XT_PLL4
;               config __FOSC, CSW_FSCM_ON & EXT & XT_PLL8
;               config __FOSC, CSW_FSCM_ON & EXT & XT_PLL16
;               config __FOSC, CSW_FSCM_ON & EXT & XTL
;               config __FOSC, CSW_FSCM_ON & EXT & FRC_PLL4
;               config __FOSC, CSW_FSCM_ON & EXT & FRC_PLL8
;               config __FOSC, CSW_FSCM_ON & EXT & FRC_PLL16
;               config __FOSC, CSW_FSCM_ON & EXT & HS
;               config __FOSC, CSW_FSCM_ON & EXT & HS2_PLL4
;               config __FOSC, CSW_FSCM_ON & EXT & HS2_PLL8
;               config __FOSC, CSW_FSCM_ON & EXT & HS2_PLL16
;               config __FOSC, CSW_FSCM_ON & EXT & HS3_PLL4
;               config __FOSC, CSW_FSCM_ON & EXT & HS3_PLL8
;               config __FOSC, CSW_FSCM_ON & EXT & HS3_PLL16
;
;               FWDT
;               =====
;               config __FWDT, WDT_OFF
;               config __FWDT, WDT_ON & WDTPSA_1 & WDTPSB_1
;               config __FWDT, WDT_ON & WDTPSA_1 & WDTPSB_2
;               config __FWDT, WDT_ON & WDTPSA_1 & WDTPSB_3
;               config __FWDT, WDT_ON & WDTPSA_1 & WDTPSB_4
;               config __FWDT, WDT_ON & WDTPSA_1 & WDTPSB_5
;               config __FWDT, WDT_ON & WDTPSA_1 & WDTPSB_6
;               config __FWDT, WDT_ON & WDTPSA_1 & WDTPSB_7
;               config __FWDT, WDT_ON & WDTPSA_1 & WDTPSB_8
;               config __FWDT, WDT_ON & WDTPSA_1 & WDTPSB_9
;               config __FWDT, WDT_ON & WDTPSA_1 & WDTPSB_10
;               config __FWDT, WDT_ON & WDTPSA_1 & WDTPSB_11
;               config __FWDT, WDT_ON & WDTPSA_1 & WDTPSB_12
;               config __FWDT, WDT_ON & WDTPSA_1 & WDTPSB_13
;               config __FWDT, WDT_ON & WDTPSA_1 & WDTPSB_14
;               config __FWDT, WDT_ON & WDTPSA_1 & WDTPSB_15
;               config __FWDT, WDT_ON & WDTPSA_1 & WDTPSB_16
;               config __FWDT, WDT_ON & WDTPSA_8 & WDTPSB_1
;               config __FWDT, WDT_ON & WDTPSA_8 & WDTPSB_2
;               config __FWDT, WDT_ON & WDTPSA_8 & WDTPSB_3
;               config __FWDT, WDT_ON & WDTPSA_8 & WDTPSB_4
;               config __FWDT, WDT_ON & WDTPSA_8 & WDTPSB_5
;               config __FWDT, WDT_ON & WDTPSA_8 & WDTPSB_6
;               config __FWDT, WDT_ON & WDTPSA_8 & WDTPSB_7
;               config __FWDT, WDT_ON & WDTPSA_8 & WDTPSB_8
;               config __FWDT, WDT_ON & WDTPSA_8 & WDTPSB_9
;               config __FWDT, WDT_ON & WDTPSA_8 & WDTPSB_10
;               config __FWDT, WDT_ON & WDTPSA_8 & WDTPSB_11
;               config __FWDT, WDT_ON & WDTPSA_8 & WDTPSB_12
;               config __FWDT, WDT_ON & WDTPSA_8 & WDTPSB_13
;               config __FWDT, WDT_ON & WDTPSA_8 & WDTPSB_14
;               config __FWDT, WDT_ON & WDTPSA_8 & WDTPSB_15
;               config __FWDT, WDT_ON & WDTPSA_8 & WDTPSB_16
;               config __FWDT, WDT_ON & WDTPSA_64 & WDTPSB_1
;               config __FWDT, WDT_ON & WDTPSA_64 & WDTPSB_2
;               config __FWDT, WDT_ON & WDTPSA_64 & WDTPSB_3
;               config __FWDT, WDT_ON & WDTPSA_64 & WDTPSB_4
;               config __FWDT, WDT_ON & WDTPSA_64 & WDTPSB_5
;               config __FWDT, WDT_ON & WDTPSA_64 & WDTPSB_6
;               config __FWDT, WDT_ON & WDTPSA_64 & WDTPSB_7
;               config __FWDT, WDT_ON & WDTPSA_64 & WDTPSB_8
;               config __FWDT, WDT_ON & WDTPSA_64 & WDTPSB_9
;               config __FWDT, WDT_ON & WDTPSA_64 & WDTPSB_10
;               config __FWDT, WDT_ON & WDTPSA_64 & WDTPSB_11
;               config __FWDT, WDT_ON & WDTPSA_64 & WDTPSB_12
;               config __FWDT, WDT_ON & WDTPSA_64 & WDTPSB_13
;               config __FWDT, WDT_ON & WDTPSA_64 & WDTPSB_14
;               config __FWDT, WDT_ON & WDTPSA_64 & WDTPSB_15
;               config __FWDT, WDT_ON & WDTPSA_64 & WDTPSB_16
;               config __FWDT, WDT_ON & WDTPSA_512 & WDTPSB_1
;               config __FWDT, WDT_ON & WDTPSA_512 & WDTPSB_2
;               config __FWDT, WDT_ON & WDTPSA_512 & WDTPSB_3
;               config __FWDT, WDT_ON & WDTPSA_512 & WDTPSB_4
;               config __FWDT, WDT_ON & WDTPSA_512 & WDTPSB_5
;               config __FWDT, WDT_ON & WDTPSA_512 & WDTPSB_6
;               config __FWDT, WDT_ON & WDTPSA_512 & WDTPSB_7
;               config __FWDT, WDT_ON & WDTPSA_512 & WDTPSB_8
;               config __FWDT, WDT_ON & WDTPSA_512 & WDTPSB_9
;               config __FWDT, WDT_ON & WDTPSA_512 & WDTPSB_10
;               config __FWDT, WDT_ON & WDTPSA_512 & WDTPSB_11
;               config __FWDT, WDT_ON & WDTPSA_512 & WDTPSB_12
;               config __FWDT, WDT_ON & WDTPSA_512 & WDTPSB_13
;               config __FWDT, WDT_ON & WDTPSA_512 & WDTPSB_14
;               config __FWDT, WDT_ON & WDTPSA_512 & WDTPSB_15
;               config __FWDT, WDT_ON & WDTPSA_512 & WDTPSB_16
;
;               FBORPOR (Examples on Motor Control PWM fuses are not provided)
;               ========
;               config __FBORPOR, PBOR_OFF & MCLR_DIS
;               config __FBORPOR, PBOR_OFF & MCLR_EN
;               config __FBORPOR, PBOR_ON & BORV_20 & PWRT_OFF & MCLR_DIS
;               config __FBORPOR, PBOR_ON & BORV_27 & PWRT_OFF & MCLR_DIS
;               config __FBORPOR, PBOR_ON & BORV_42 & PWRT_OFF & MCLR_DIS
;               config __FBORPOR, PBOR_ON & BORV_45 & PWRT_OFF & MCLR_DIS
;               config __FBORPOR, PBOR_ON & BORV_20 & PWRT_OFF & MCLR_EN
;               config __FBORPOR, PBOR_ON & BORV_27 & PWRT_OFF & MCLR_EN
;               config __FBORPOR, PBOR_ON & BORV_42 & PWRT_OFF & MCLR_EN
;               config __FBORPOR, PBOR_ON & BORV_45 & PWRT_OFF & MCLR_EN
;               config __FBORPOR, PBOR_ON & BORV_20 & PWRT_4 & MCLR_DIS
;               config __FBORPOR, PBOR_ON & BORV_27 & PWRT_4 & MCLR_DIS
;               config __FBORPOR, PBOR_ON & BORV_42 & PWRT_4 & MCLR_DIS
;               config __FBORPOR, PBOR_ON & BORV_45 & PWRT_4 & MCLR_DIS
;               config __FBORPOR, PBOR_ON & BORV_20 & PWRT_4 & MCLR_EN
;               config __FBORPOR, PBOR_ON & BORV_27 & PWRT_4 & MCLR_EN
;               config __FBORPOR, PBOR_ON & BORV_42 & PWRT_4 & MCLR_EN
;               config __FBORPOR, PBOR_ON & BORV_45 & PWRT_4 & MCLR_EN
;               config __FBORPOR, PBOR_ON & BORV_20 & PWRT_16 & MCLR_DIS
;               config __FBORPOR, PBOR_ON & BORV_27 & PWRT_16 & MCLR_DIS
;               config __FBORPOR, PBOR_ON & BORV_42 & PWRT_16 & MCLR_DIS
;               config __FBORPOR, PBOR_ON & BORV_45 & PWRT_16 & MCLR_DIS
;               config __FBORPOR, PBOR_ON & BORV_20 & PWRT_16 & MCLR_EN
;               config __FBORPOR, PBOR_ON & BORV_27 & PWRT_16 & MCLR_EN
;               config __FBORPOR, PBOR_ON & BORV_42 & PWRT_16 & MCLR_EN
;               config __FBORPOR, PBOR_ON & BORV_45 & PWRT_16 & MCLR_EN
;               config __FBORPOR, PBOR_ON & BORV_20 & PWRT_64 & MCLR_DIS
;               config __FBORPOR, PBOR_ON & BORV_27 & PWRT_64 & MCLR_DIS
;               config __FBORPOR, PBOR_ON & BORV_42 & PWRT_64 & MCLR_DIS
;               config __FBORPOR, PBOR_ON & BORV_45 & PWRT_64 & MCLR_DIS
;               config __FBORPOR, PBOR_ON & BORV_20 & PWRT_64 & MCLR_EN
;               config __FBORPOR, PBOR_ON & BORV_27 & PWRT_64 & MCLR_EN
;               config __FBORPOR, PBOR_ON & BORV_42 & PWRT_64 & MCLR_EN
;               config __FBORPOR, PBOR_ON & BORV_45 & PWRT_64 & MCLR_EN
;
;               FGS
;               ====
;               config __FGS, CODE_PROT_OFF
;               config __FGS, CODE_PROT_ON
;
;Setting configuration fuses without using macros:
;=================================================
; To set configuration fuses without using the "config" macro, use the following
; format:
;        .section __FOSC.sec, code
;        .global __FOSC
;__FOSC: .pword CSW_FSCM_ON & EC_PLL16
;
;------------------------------------------------------------------------------

.LIST

