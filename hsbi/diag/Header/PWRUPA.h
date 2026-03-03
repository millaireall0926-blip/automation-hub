
FUNCTION PWRUPA1M
       NOP         NOP                              XCYC CLK
       NOP         NOP                              XCYC CLK

       NOP         NOP                              XCYC CLK   INIT
       NOP         NOP                              XCYC BLK
       NOP         NOP                              XCYC BLK
       NOP         NOP                              XCYC BLK
LL1:   NOP         NOP                              XCYC BLK
       JNI1 LL1    NOP                              XCYC BLK                                     ; /RESET LO
       NOP         NOP                              XCYC CLK                                     ;
       NOP         NOP                              XCYC CLK                                     ;
       NOP         NOP                              XCYC CLK                                     ;
       NOP         NOP                              XCYC CLK                                     ;
       NOP         NOP                              XCYC DLK                                     ;/RESET HI
       NOP         NOP                              XCYC DLK                                     ;/RESET HI
       NOP         NOP                              XCYC DLK                                     ;
       NOP         NOP                              XCYC DLK                                     ;
       NOP         NOP                              XCYC DLK                                     ;
LL2:   NOP         NOP                              XCYC DLK                                     ;
       NOP         NOP                              XCYC DLK                                     ;
       JNI1 LL2    NOP                              XCYC DLK                                     ;CKE LOW R
       NOP         NOP                              XCYC DLK                                     ;
       NOP         NOP                              XCYC DLK                                     ;
       NOP         NOP                              XCYC DLK                                     ;
       NOP         NOP                              XCYC DLK                                     ;
       NOP         NOP                              XCYC DLK                                     ;
       NOP         NOP                              XCYC CLK            INIT                     ;
       NOP         NOP                              XCYC CLK                                     ;
       NOP         NOP                              XCYC CLK                                     ;
       NOP         NOP                              XCYC CLK                                     ;
       NOP         NOP                              XCYC CLK                      XT<#0400       ;A14 A15
       NOP         NOP                              XCYC CLK                                     ;
       NOP         PREA                             X<XT CLK                                     ;
       NOP         NOP                              XCYC CLK                                     ;
       NOP         NOP                              XCYC CLK                                     ;
       NOP         NOP                              XCYC CLK                                     ;
       NOP         NOP                              XCYC CLK                                     ;
       NOP         NOP                              XCYC CLK                                     ;
       NOP         NOP                              XCYC CLK                                     ;
       NOP         NOP                              XCYC CLK                                     ;
;----------------------------------------------------------------------
       NOP         NOP                              XCYC CLK
       NOP         NOP                              XCYC CLK   XT<#0020     ;; MR3    
       NOP         NOP                              XCYC CLK
       NOP         MRSJ               C12 C13       X<XT CLK
       NOP         NOP                              XCYC CLK
       NOP         NOP                              XCYC CLK
       NOP         NOP                              XCYC CLK
       NOP         NOP                              XCYC CLK  XT<#00C8      ; MR6 
       NOP         NOP                              XCYC CLK
       NOP         MRSJ                   C13 C1    X<XT CLK
       NOP         NOP                              XCYC CLK
       NOP         NOP                              XCYC CLK
       NOP         NOP                              XCYC CLK
       NOP         NOP                              XCYC CLK  XT<#0000      ; MR5   
       NOP         NOP                              XCYC CLK
       NOP         MRSJ               C12     C1    X<XT CLK
       NOP         NOP                              XCYC CLK
       NOP         NOP                              XCYC CLK
       NOP         NOP                              XCYC CLK
       NOP         NOP                              XCYC CLK
       NOP         NOP                              XCYC CLK  XT<#0000      ; MR4    
       NOP         NOP                              XCYC CLK
       NOP         MRSJ                       C1    X<XT CLK
       NOP         NOP                              XCYC CLK
       NOP         NOP                              XCYC CLK
       NOP         NOP                              XCYC CLK
       NOP         NOP                              XCYC CLK
       NOP         NOP                              XCYC CLK  XT<#0000      ; MR2 
       NOP         NOP                              XCYC CLK
       NOP         MRSJ                  C13        X<XT CLK                     
       NOP         NOP                              XCYC CLK
       NOP         NOP                              XCYC CLK
       NOP         NOP                              XCYC CLK
       NOP         NOP                              XCYC CLK
       NOP         NOP                              XCYC CLK  XT<#0002      ; MR1 
       NOP         NOP                              XCYC CLK
       NOP         MRSJ            C12              X<XT CLK                                                       
       NOP         NOP                              XCYC CLK
       NOP         NOP                              XCYC CLK
       NOP         NOP                              XCYC CLK
       NOP         NOP                              XCYC CLK
       NOP         NOP                              XCYC CLK  XT<#0310      ; MR0
       NOP         NOP                              XCYC CLK
       NOP         MRSS                             X<XT CLK                                           
       NOP         NOP                              XCYC CLK
LL3:   NOP         NOP                              XCYC CLK                                     ;
       NOP         NOP                              XCYC CLK                                     ;
       JNI1 LL3    NOP                              XCYC CLK                                     ;
;--------------------------------------------------------------------------------------------------------------------------------------------------------
;--------------------------------------------------------------------------------------------------------------------------------------------------------
;--------------------------------------------------------------------------------------------------------------------------------------------------------
       RTN         NOP                              XCYC CLK
ENDFUNCTION




