;Co

IF 0
Y: EXTERN	;c� � � � � s � 

MAT: EQU 0

IF MAT
Y: EXTERN	;c� � � � � s � 

SECTION TEXT
INPUT Y
ROT2:

	LOAD VAL

ADD Y; loko demais
STORE Y+2
JMPP MOD_B
L1: STOP
L3: COPY Aaaa, Beeech

SECTION DATA
VAL1: CONST 0x19

SECTION BSS
VAL: SPACE

