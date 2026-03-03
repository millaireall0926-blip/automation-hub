

;%IFE A .AND. B .AND.((C.OR.D).AND.E)

%INSERT Header/define.h
  
 REGISTER
			XMAX=0xffffff;
			YMAX=0xffffff;
			ZMAX=0xff;
			XOS=0x0;
			YOS=0x0;	
ENDREGISTER		

START INFINITY_H;

        NOP        XB=0xFFFFFFF   YB=0xFFFFFFF	SA=0xFFFFFFFF 	;
        NOP        TPH1=0xFFFFFF  TPH2=0xFFFFFF		            ;
        NOP     									            ;
LB1:    NOP        X<XB Y<YB    S=SA  	  	   		 			;
        JMP LB1    X<XB Y<YB	S=SA 			                ;
		STPS									                ;
END

START INFINITY_L;

        NOP        XB=0  YB=0    		           ;
        NOP        TPH1=0  TPH2=0		           ;
        NOP 									   ;
LB1:    NOP        X<XB Y<YB      	  			   ;
        JMP LB1    X<XB Y<YB	 			       ;
		STPS									   ;
END
	
;%ENDC 