

;%IFE A .AND. B .AND.((C.OR.D).AND.E)

%INSERT Header/define.h
  
 REGISTER
			XMAX=0xffffff;
			YMAX=0xffffff;
			ZMAX=0xff;
			XOS=0x0;
			YOS=0x0;	
ENDREGISTER		


 			START XB_ALU_XB_00;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
 
			NOP									  									;                                      
			NOP							      										;                                                 
			NOP							      										;                                                 
			NOP							    				XB<0		YB<0		;                                                 
			NOP			W R				    XBYB			XB</XB		YB</YB		;                                                 
			NOP			W R				    XBYB			XB<0		YB<0		;                                                 
			NOP			W R				    XBYB			XB<XB+1		YB<YB+1		;                                                 
			NOP			W R				    XBYB			XB<XB-1		YB<YB-1		;                                                 
			NOP			W R				    XBYB			XB<0x1		YB<0x1		;                                                 
			NOP			W R				    XBYB			XB<XB*2		YB<YB*2		;                                                 
			NOP			W R				    XBYB			XB<XB*2		YB<YB*2		;                                                 
			NOP			W R				    XBYB			XB<XB/2		YB<YB/2		;                                                 
			NOP			W R				    XBYB			XB<XB/2		YB<YB/2		;                                                 
			NOP			W R				    XBYB			XB<0x8000	YB<0xffff	;                                     
			NOP			W R				    XBYB  									;                                                 
			NOP							    XBYB			XB<XB-1		YB<YB-1		;                                                 
			NOP			W R				    XBYB			 						;                                                 
			NOP							    XBYB			XB<XB+1		YB<YB+1		;                                                 
			NOP			W R				    XBYB			 						;                                                 
			NOP							    XBYB			XB<XB-1		YB<YB-1		;                                                 
			NOP			W R				    XBYB			 						;                                                 
			NOP							    XBYB			XB<XB+1		YB<YB+1		;                                                 
			NOP			W R				    XBYB			 						;                                                 
			NOP							    XBYB			XB<XB-1		YB<YB-1		;                                                 
			NOP			W R				    XBYB			 						;         

            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;	
			STPS							         ;
	END


	START XB_ALU_A_00;
		    NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
 		
 			NOP			XH=0x1																	;                                      
    		NOP			XB=0x0																	;                                      
 			NOP			D1A=0x0																	;                                     
 			NOP			D1B=0x1																	;                                     
 			NOP			D1C=0x2																	;                                     
 			NOP			D1D=0x3																	;                                     
 			NOP			D1E=0x4																	;                                     
 			NOP			D1F=0x5																	;                                     
 			NOP			D1G=0x6																	;                                     
 			NOP			D2A=0x7																	;                                     
 			NOP			D2B=0x8																	;                                     
 			NOP			D2C=0xffff																;                                     
 			NOP			D2D=0x1111																;                                     
 			NOP			XOS<0x0																	;                                      
		
			NOP																					;                                     
			NOP																					;                                     
			NOP																					;                                     
			NOP																					;                                     
			NOP																					;                                     
			NOP																					;                                     
			NOP		 				            XBYB        		XB<XH	YB<D2A				;                                                 
			NOP			W R						XBYB  				XB<D1A	YB<D2B				;                                                 
			NOP			W R						XBYB  				XB<D1B	YB<D2C				;                                                 
			NOP			W R						XBYB  				XB<D1C	YB<D2D				;                                                 
			NOP			W R						XBYB  				XB<D1D	YB<D1D				;                                                 
			NOP			W R						XBYB  				XB<D1E	YB<D1E				;                                                 
			NOP			W R						XBYB  				XB<D1F	YB<D1F				;                                                 
			NOP			W R						XBYB  				XB<D1G	YB<D1G				;                                                 
			NOP			W R						XBYB  				XB<D2A	YB<D2A				;                                                 
			NOP			W R						XBYB  				XB<D2B	YB<D2B				;                                                 
			NOP			W R						XBYB  				XB<D2C	YB<D2C				;                                                 
			NOP			W R						XBYB  				XB<D2D	YB<D2D				;                                                 
			NOP			W R						XBYB  											;                                     
			NOP									XBYB				XB<D1A	YB<D2B				;                                     
			NOP			W R						XBYB  				XB<D1B	YB<D2C				;                                                 
			NOP			W R						XBYB  											;                                                 
			NOP									XBYB											;                                     
			NOP									XBYB				XB<D1A	YB<D2B				;                                     
			NOP			W R						XBYB  				XB<D1B	YB<D2C				;                                                 
			NOP			W R						XBYB  											;                                                 

            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;  		
			STPS																				;
	END 
	

			START XB_ALU_plus_A_00;
		 	NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
 
			NOP																					;                                     
			NOP																					;                                      
			NOP																					;                                      
			NOP			XH=0x1000   YH=0x8000													;                                      
			NOP			D1A=0x1																	;                                      
			NOP			D1B=0x2																	;                                      
			NOP			D1C=0x4																	;                                      
			NOP			D1D=0x8																	;                                      
			NOP			D1E=0x10																;                                      
			NOP			D1F=0x20																;                                      
			NOP			D1G=0x40																;                                      
			NOP			D2A=0x100																;                                      
			NOP			D2B=0x200																;                                      
			NOP			D2C=0x400																;                                      
			NOP			D2D=0x800																;                                      
			NOP																					;                                     
			NOP																					;                                     
			NOP																					;                                     
			NOP			   				                			XB<XB+XH	YB<YB+D2A		;                                                 
 			NOP			W R						XBYB  				XB<XB+D1A	YB<YB+D2B		;                                                 
 			NOP			W R						XBYB  				XB<XB+D1B	YB<YB+D2C		;                                                 
 			NOP			W R						XBYB  				XB<XB+D1C	YB<YB+D2D		;                                                 
 			NOP			W R						XBYB  				XB<XB+D1D	YB<YB+YH		;                                                 
 			NOP			W R						XBYB  				XB<XB+D1E	YB<YB+D1A		;                                                 
 			NOP			W R						XBYB  				XB<XB+D1F	YB<YB+D1B		;                                                 
 			NOP			W R						XBYB  				XB<XB+D1G	YB<YB+D1C		;                                                 
			NOP			W R						XBYB  				XB<XB+D2A	YB<YB+D1D		;                                                 
			NOP			W R						XBYB  				XB<XB+D2B	YB<YB+D1E		;                                                 
			NOP			W R						XBYB  				XB<XB+D2C	YB<YB+D1F		;                                                 
			NOP			W R						XBYB  				XB<XB+D2D	YB<YB+D1G		;                                                 
			NOP			W R						XBYB  				XB<0	    YB<0xffff		;                                                 
			NOP			W R						XBYB  				XB<0xffff	YB<0    		;                                                 
			NOP			W R						XBYB  											;                                     
			NOP																					;                                     
			NOP																					;                                     
			NOP																					;                                     
			NOP			 							  				XB<0xffff   YB<0xffff		;                                                 
			NOP			W R						XBYB  				XB<0x0		YB<0    		;                                                 
			NOP			W R						XBYB  											;                                     

 
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;	
			NOP																					;                                     
			STPS																				;
	END
			START XB_ALU_minus_A_00;
		 	NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;

			NOP																					;                                      
			NOP			XB<0xffff	YB<0x8000													;                                      
			NOP			XH=0x8000	YH=0x4000												;                                      
			NOP			D1A=0x0001																;                                      
			NOP			D1B=0x0010																;                                      
			NOP			D1C=0x0100																;                                      
			NOP			D1D=0x1000																;                                      
			NOP			D1E=0x0000																;                                      
			NOP			D1F=0x0001																;                                      
			NOP			D1G=0x0010																;                                      
			NOP			D2A=0x0100																;                                      
			NOP			D2B=0x1000																;                                      
			NOP			D2C=0x0000																;                                      
			NOP			D2D=0x0001																;                                      
			NOP			   				                			XB<XB-XH	YB<YB-YH		;                                                 
			NOP			W R						XBYB  				XB<XB-D1A	YB<YB-D1A		;                                                 
			NOP			W R						XBYB  				XB<XB-D1B	YB<YB-D1B		;                                                 
			NOP			W R						XBYB  				XB<XB-D1C	YB<YB-D1C		;                                                 
			NOP			W R						XBYB  				XB<XB-D1D	YB<YB-D1D		;                                                 
			NOP			W R						XBYB  				XB<XB-D1E	YB<YB-D1E		;                                                 
			NOP			W R						XBYB  				XB<XB-D1F	YB<YB-D1F		;                                                 
			NOP			W R						XBYB  				XB<XB-D1G	YB<YB-D1G		;                                                 
			NOP			W R						XBYB  				XB<XB-D2A	YB<YB-D2A		;                                                 
			NOP			W R						XBYB  				XB<XB-D2B	YB<YB-D2B		;                                                 
			NOP			W R						XBYB  				XB<XB-D2C	YB<YB-D2C		;                                                 
			NOP			W R						XBYB  				XB<XB-D2D	YB<YB-D2D		;                                                 
			NOP			W R						XBYB  				XB<0	    YB<0xffff		;                                                 
			NOP			W R						XBYB  				XB<0xffff	YB<0    		;                                                 
			NOP			W R						XBYB  											;                                     
			NOP			 							  				XB<0xffff   YB<0xffff		;                                                 
			NOP			W R						XBYB  				XB<0x0		YB<0    		;                                                 
			NOP			W R						XBYB  											;                                     
			NOP																					;                                     
			NOP																					;                                     
			NOP																					;                                     
			NOP																					;                                     
			NOP																					;                                     
			NOP																					;                                     
    		STPS																				;
	END  	
			START XB_ALU_XB_plus_Imm;
		 	NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;

			NOP									        XB<0x8000   YB<0x4000			;                                      
			NOP																			;                                                 
			NOP											 								;                                                 
			NOP											XB<XB+0x1	YB<YB+0x1			;                                                 
			NOP			W R						XBYB  	XB<XB+0x2	YB<YB+0x2			;                                                 
			NOP			W R						XBYB  	XB<XB+0x3	YB<YB+0x3			;                                                 
			NOP			W R						XBYB  	XB<XB+0x4	YB<YB+0x4			;                                                 
			NOP			W R						XBYB  	XB<XB+0x5	YB<YB+0x5			;                                                 
			NOP			W R						XBYB  	XB<XB+0x6	YB<YB+0x6			;                                                 
			NOP			W R						XBYB  	XB<XB+0x7	YB<YB+0x7			;                                                 
			NOP			W R						XBYB  	XB<XB+0x8	YB<YB+0x8			;                                                 
			NOP			W R						XBYB  	XB<XB+0x9	YB<YB+0x9			;                                                 
			NOP			W R						XBYB  	XB<XB+0xA	YB<YB+0xA			;                                                 
			NOP			W R						XBYB  	XB<XB+0xB	YB<YB+0xB			;                                                 
			NOP			W R						XBYB  	XB<XB+0xC	YB<YB+0xC			;                                                 
			NOP			W R						XBYB  	XB<XB+0xD	YB<YB+0xD			;                                                 
			NOP			W R						XBYB  	XB<XB+0xE	YB<YB+0xE			;                                                 
			NOP			W R						XBYB  	XB<XB+0xF	YB<YB+0xF			;                                                 
			NOP			W R						XBYB  	XB<XB+0x10	YB<YB+0x10			;                                                 
			NOP			W R						XBYB  									;                                     

		 	NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
    		STPS																		;
	END	
	
			START XB_ALU_XOS_00;
		 	NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;

			NOP																			;                                      
			NOP																			;                                                 
			NOP											XB<0x0		YB<0x0				;                                                 
			NOP											XOS<0x80	YOS<0x40			;                                                 
			NOP			W R						XBYB  	XB<XB+1		YB<YB+1				;                                                 
			NOP			W R						XBYB  	XB<XB+1		YB<YB+1				;                                                 
			NOP			W R						XBYB  	XB<XB+1		YB<YB+1				;                                                 
			NOP			W R						XBYB  	XB<XB+1		YB<YB+1				;                                                 
			NOP			W R						XBYB  	XB<XB-1		YB<YB-1				;                                                 
			NOP			W R						XBYB  	XB<XB-1		YB<YB-1				;                                                 
			NOP			W R						XBYB  	XB<XB-1		YB<YB-1				;                                                 
			NOP			W R						XBYB  	XB<XB-1		YB<YB-1				;                                                 
			NOP			W R						XBYB  	XB<XB-1		YB<YB-1				;                                                 
			NOP			W R						XBYB  	XB<XB+1		YB<YB+1				;                                                 
			NOP			W R						XBYB  	XB<XB-1		YB<YB-1				;                                                 
			NOP			W R						XBYB  	XB<XB-1		YB<YB-1				;                                                 
			NOP			W R						XBYB  	XB<XB-1		YB<YB-1				;                                                 
			NOP			W R						XBYB  	XB<XB-1		YB<YB-1				;                                                 
			NOP			W R						XBYB  	XB<XB-1		YB<YB-1				;                                                 
			NOP			W R						XBYB  									;                                     

		 	NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
    		STPS																		;
	END


			START XB_ALU_Imm_00;
		 	NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;

			NOP																			;                                      
			NOP																			;                                                 
			NOP											XB<0x0			YB<0x0				;                                                 
			NOP											XOS<0x00		YOS<0x00			;                                                 
			NOP			W R						XBYB  	XB<XB+0x1		YB<YB+0x1				;                                                 
			NOP			W R						XBYB  	XB<XB+0x2		YB<YB+0x2				;                                                 
			NOP			W R						XBYB  	XB<XB+0x4		YB<YB+0x4				;                                                 
			NOP			W R						XBYB  	XB<XB+0x8		YB<YB+0x8				;                                                 
			NOP			W R						XBYB  	XB<XB-0x1		YB<YB-0x1				;                                                 
			NOP			W R						XBYB  	XB<XB-0x2		YB<YB-0x2				;                                                 
			NOP			W R						XBYB  	XB<XB-0x4		YB<YB-0x4				;                                                 
			NOP			W R						XBYB  	XB<XB-0x8		YB<YB-0x8				;                                                 
			NOP			W R						XBYB  	XB<XB+0xFFFF		YB<YB+0xFFFF				;                                                 
			NOP			W R						XBYB  	XB<XB-0xFFFF		YB<YB-0xFFFF				;                                                 
			NOP			W R						XBYB  	XB<XB+0xAAAA		YB<YB+0xBBBB				;                                                 
			NOP			W R						XBYB  	XB<XB-0xAAAA		YB<YB-0xBBBB				;                                                 
			NOP			W R						XBYB  	XB<XB+0xBBBB		YB<YB+0xCCCC				;                                                 
			NOP			W R						XBYB  	XB<XB-0xBBBB		YB<YB-0xCCCC				;                                                 
			NOP			W R						XBYB  	XB<0		YB<0				;                                                 
			NOP			W R						XBYB  									;                                     

		 	NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
    		STPS																		;
	END
	

;%ENDC 