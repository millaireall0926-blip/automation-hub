
SDEF W C19
SDEF R C17
SDEF XBYB X<XB Y<YB
SDEF XCYC X<XC Y<YC
SDEF XAYA X<XA Y<YA


//                      C	C	C	 C	 O	  R		C	C	A	A	B	W	C	 R	 P	 T	 A
//						K	K	K	 K	 D	  E		S	S	C	P	C	E	A	 A	 A	 E	 L
//						T	C	E	 E	 T	  S			1	T				S	 S	 N	 N	 E
//									 1		  E				N						 R		 R
//											  T										 T	 	 T
// CONTROL SIGNAL		C3	C4	C29	 C1	 C31  C30	S	S	C2	C10	C12	C5	C28	 C0	          
//                                                                      A14 A15  A16   

SDEF RESET              C3  C4  C29  C1       C30                                        CYP0          
SDEF RESETA             C3  C4  C29  C1                                                  CYP0
SDEF RESETB             C3  C4                                                           CYP0

SDEF DUMMY              C3  C4                                                           CYP3
SDEF MRS                C3  C4                                                           CYP0 

SDEF ACTIV              C3  C4                              C2                           CYP0
SDEF WRITE              C3  C4                                          C5  C28          CYP1
SDEF READ               C3  C4                                              C28          CYP1
SDEF PREC               C3  C4                                          C5       C0      CYP1

