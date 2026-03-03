

;%IFE A .AND. B .AND.((C.OR.D).AND.E)

%INSERT Header/define.h
 
REGISTER
			XMAX=0xffffff;
			YMAX=0xffffff;
			ZMAX=0xff;
			XOS=0x0;
			IDX0=0x1;
			IDX1=0x2;
			IDX2=0x1;
			IDX3=0x2;
			IDX4=0x1;
			IDX5=0x2;		
			IDX6=0x1;		
			IDX7=0x2;		

			IDX8=0x4;		
			IDX9=0x1;		
			IDX10=0x4;	
			IDX11=0x1;		
			IDX12=0x4;		
			IDX13=0x1;		
			IDX14=0x4;		
			IDX15=0x1;		
            TIME = 3 US;
      //      REFTIME = 10 US;
            ISP=RFAA;
      //      MODE REFRESHA
ENDREGISTER		

FUNCTION RTNCHK0
			NOP																			;                                      
			NOP																			;                                                 
			NOP																			;                                                 
			NOP					TP<0x0001 TP2<0x000A   									;
			NOP			W R		TP<0x0002 TP2<0x000B   									;
			NOP			W R		TP<0x0004 TP2<0x000C   									;                                                 
			NOP			W R		TP<0x0008 TP2<0x000D   									;                                                 
			NOP			W R																;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			RTN			
ENDFUNCTION	

FUNCTION RTNCHK1
			NOP																			;                                      
			NOP																			;                                                 
			NOP																			;                                                 
			NOP					TP<0x0011 TP2<0x00AA   									;                                                 
			NOP			W R		TP<0x0022 TP2<0x00BB   									;                                                 
			NOP			W R		TP<0x0044 TP2<0x00CC   									;                                                 
			NOP			W R		TP<0x0088 TP2<0x00DD   									;                                                 
			NOP			W R																;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			RTN			
ENDFUNCTION	

FUNCTION RTNCHK2
			NOP																			;                                      
			NOP																			;                                                 
			NOP																			;                                                 
			NOP					TP<0x0111 TP2<0x0AAA   									;                                                 
			NOP			W R		TP<0x0222 TP2<0x0BBB   									;                                                 
			NOP			W R		TP<0x0444 TP2<0x0CCC   									;                                                 
			NOP			W R		TP<0x0888 TP2<0x0DDD   									;                                                 
			NOP			W R																;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			RTN			
ENDFUNCTION	

FUNCTION RTNCHK3
			NOP																			;                                      
			NOP																			;                                                 
			NOP																			;                                                 
			NOP					TP<0x1111 TP2<0xAAAA   									;                                                 
			NOP			W R		TP<0x2222 TP2<0xBBBB   									;                                                 
			NOP			W R		TP<0x4444 TP2<0xCCCC   									;                                                 
			NOP			W R		TP<0x8888 TP2<0xDDDD   									;                                                 
			NOP			W R																;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			RTN			
ENDFUNCTION	

FUNCTION RFAA
           
            NOP                           TP<0    TP2<0
			NOP         W  R              TP<TP+1 TP2<TP2+1									;                                                        
			NOP         W  R              TP<TP+1 TP2<TP2+1									;                                                        
			NOP         W  R              TP<TP+1 TP2<TP2+1									;                                                        
			NOP         W  R              TP<TP+1 TP2<TP2+1									;                                                        
			NOP         W  R              TP<TP+1 TP2<TP2+1									;                                                        
			NOP         W  R              TP<TP+1 TP2<TP2+1									;                                                        
			NOP         W  R              TP<TP+1 TP2<TP2+1									;                                                        
			NOP         W  R              TP<TP+1 TP2<TP2+1									;                                                        
			NOP         W  R              TP<TP+1 TP2<TP2+1									;                                                        
			NOP         W  R              TP<TP+1 TP2<TP2+1									;                                                        
			NOP         W  R              TP<TP+1 TP2<TP2+1									;                                                        
			NOP         W  R              TP<TP+1 TP2<TP2+1									;                                                        
			NOP         W  R              TP<TP+1 TP2<TP2+1									;                                                        
			NOP         W  R              TP<TP+1 TP2<TP2+1									;                                                        
	//		NOP         W  R              TP<TP+1 TP2<TP2+1									;                                                                                                                                                                       
			NOP         W  R                                                                ;
            NOP          	                               									;
            NOP          	                               						        	;
            REPEAT 75          	                               								;
            NOP          	                               								    ;
          	RTN																				;
ENDFUNCTION

        START OPCODE_NOP_00;
            NOP          	  	   		 			                                     ;
            NOP          	  	   		 			                                     ;
            NOP          	  	   		 			                                     ;
            NOP          	  	   		 			                                     ;
            NOP          	  	   		 			                                     ;
            NOP          	  	   		 			                                     ;
            NOP          	  	   		 			                                     ;
            NOP          	  	   		 			                                     ;
AAA:        NOP          	  	   		 			                                     ;
            NOP          	  	   		 			                                     ;

 			NOP	 		   X<XC Y<YC            TP<0x0001 TP2<0x000A   //XC<0xFFFFFFFF YC<0xFFFFFFFF ;  
            NOP    W R     X<XC Y<YC            TP<0x0002 TP2<0x000B      ;  
            NOP    W R     X<XC Y<YC            TP<0x0004 TP2<0x000C      ;  
            NOP    W R     X<XC Y<YC            TP<0x0008 TP2<0x000D	   ;  
            NOP    W R     X<XC Y<YC            TP<0x0010 TP2<0x00A0	   ;  
            NOP    W R     X<XC Y<YC            TP<0x0020 TP2<0x00B0	   ;  
            NOP    W R     X<XC Y<YC            TP<0x0040 TP2<0x00C0	   ;  
            NOP    W R     X<XC Y<YC            TP<0x0080 TP2<0x00D0	   ;  
            NOP    W R     X<XC Y<YC            TP<0x0100 TP2<0x0A00	   ;  
            NOP    W R     X<XC Y<YC            TP<0x0200 TP2<0x0B00	   ;  
            NOP    W R     X<XC Y<YC            TP<0x0400 TP2<0x0C00      ;  
            NOP    W R     X<XC Y<YC            TP<0x0800 TP2<0x0D00	   ;  
            NOP    W R     X<XC Y<YC            TP<0x1000 TP2<0xA000	   ;  
            NOP    W R     X<XC Y<YC            TP<0x2000 TP2<0xB000	   ;  
            NOP    W R     X<XC Y<YC            TP<0x4000 TP2<0xC000	   ;  
            NOP    W R     X<XC Y<YC            TP<0x8000 TP2<0xD000	   ;  
            NOP    W R     X<XC Y<YC             										 ;  
   		    NOP	  																         ;				 
            NOP	 																         ;

			NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
      //      JMP AAA          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			
 
			STPS									 ;
END


			START OPCODE_JMP_00;
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
 
                                                   
			NOP					TP<0x0000 TP2<0xFFFF   									;                                                 
			NOP		  	W R		TP<TP+1 TP2<TP2-1   									;                                                 
            JMP A2    	W R		TP<TP+1 TP2<TP2-1   									;                                                 
		A2: JMP A3      W R		TP<TP+1 TP2<TP2-1   									;                                                 
			NOP        	W R		TP<0xAAAA TP2<0xAAAA   									;                                                 
		A3: JMP A4 	  	W R		TP<TP+1 TP2<TP2-1   									;                                                 
			NOP   	  	W R		TP<0xAAAA TP2<0xAAAA   									;                                                 
			NOP 	    W R		TP<0xBBBB TP2<0xBBBB   									;                                                 
		A4: JMP A5    	W R		TP<TP+1 TP2<TP2-1   									;                                                 
			NOP        	W R		TP<0xAAAA TP2<0xAAAA 									;                                                 
			NOP		  	W R		TP<0xBBBB TP2<0xBBBB 									;                                     
			NOP		  	W R		TP<0xCCCC TP2<0xCCCC 									;                                                 
		A5:	JMP A6     	W R		TP<TP+1 TP2<TP2-1   									;                                     
			NOP		  	W R		TP<0xAAAA TP2<0xAAAA 									;                                                 
			NOP		  	W R		TP<0xBBBB TP2<0xBBBB   									;                                     
			NOP		  	W R		TP<0xCCCC TP2<0xCCCC 									;                                                 
			NOP		  	W R		TP<0xDDDD TP2<0xDDDD 									;                                     
		A6:	NOP		  	W R		TP<TP+1 TP2<TP2-1 										;                                     
			NOP		  	W R		TP<TP+1 TP2<TP2-1 										;                                     
			NOP		  	W R		TP<TP+1 TP2<TP2-1 										;                                     
			NOP		  	W R		TP<TP+1 TP2<TP2-1 										;                                     
			NOP		  	W R		TP<TP+1 TP2<TP2-1 										;                                     
			NOP		  	W R		TP<TP+1 TP2<TP2-1 										;                                     
			NOP		  	W R		TP<TP+1 TP2<TP2-1 										;                                     
			NOP		  	W R		TP<TP+1 TP2<TP2-1 										;                                     
			NOP		  	W R		TP<TP+1 TP2<TP2-1 										;                                     
			NOP		  	W R		TP<TP+1 TP2<TP2-1 										;                                     
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


			START OPCODE_JSR_00;
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
 
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;                                              
			JSR  RTNCHK0					         ;          
			JSR  RTNCHK1	                         ;          
			JSR  RTNCHK2                             ;          
    		JSR  RTNCHK3                            ;          
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
 
 
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;                                   
			STPS						   			 ;                                     
                                         
END


			START OPCODE_JNC_00;
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
			JNC0 LA																		;                                                 
			NOP					TP<0x0000 TP2<0xA000   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1  										;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1  										;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1  										;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1  										;                                                 
LA:			JNC1 LB																		;                                                 
			NOP					TP<0x1000 TP2<0xB000   	 								;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                            
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                            
LB:			JNC2 LC																		;                                      
			NOP					TP<0x2000 TP2<0xC000   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
LC:			JNC3 LD																		;                                                 
			NOP					TP<0x4000 TP2<0xD000   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                     
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                     
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
LD:			JNC4 LE																		;                                                 
			NOP					TP<0x8000 TP2<0xE000   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                     
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                     
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
LE:			NOP																			;                                     
			
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
            STPS								     ;

END

			START OPCODE_JNC_01;
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
 
			JNC5 LA																		;                                                 
			NOP					TP<0x0000 TP2<0x0000   									;                                                                                               
			NOP			W R		TP<TP+1 TP2<TP2+1  										;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1  										;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1                                       ;
            NOP         W R  										                    ;                                                 
LA:			JNC6 LB																		;                                                 
			NOP					TP<0x1000 TP2<0x1000   	 								;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;
            NOP         W R  										                    ;                                                                        
LB:			JNC7 LC																		;                                      
			NOP					TP<0x2000 TP2<0x2000   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;
            NOP         W R  										                    ;                                                             
LC:			JNC8 LD																		;                                                 
			NOP					TP<0x4000 TP2<0x4000   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                     
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;
            NOP         W R  										                    ;                                                 
LD:			JNC9 LE																		;                                                 
			NOP					TP<0x8000 TP2<0x8000   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                     
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;
            NOP         W R  										                    ;                                                 
LE:			NOP																			;                                                 
			NOP			                               									;                                                 
			
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

			START OPCODE_JNC_02; // JNC10 ~ JNC14
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
 
            JNC10 LA																	;                                                 
			NOP					TP<0x0000 TP2<0x0000   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1  										;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1  										;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1  										;                                                 
            NOP         W R                                                             ;
LA:			JNC11 LB																	;                                                 
			NOP					TP<0x1000 TP2<0x1000   	 								;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                            
            NOP         W R                                                             ;
LB:			JNC12 LC																	;                                      
			NOP					TP<0x2000 TP2<0x2000   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;
            NOP         W R                                                             ;                                                 
LC:			JNC13 LD																	;                                                 
			NOP					TP<0x4000 TP2<0x4000   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                     
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;
            NOP         W R                                                             ;                                     
LD:			JNC14 LE																	;                                                 
			NOP					TP<0x8000 TP2<0x8000   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                     
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;
            NOP         W R                                                             ;                                     
LE:																				        ;                                                                                    
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

			START OPCODE_JNC_03; // JNC15
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
 
            JNC15 LA																	;                                                 
			NOP					TP<0x0000 TP2<0x0000   									;                                                                                               
			NOP			W R		TP<TP+1 TP2<TP2+1  										;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1  										;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1                                       ;
            NOP         W R  										                    ;                                                 
LA:			NOP					TP<0x1000 TP2<0x1000   	 								;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;
            NOP         W R  										                    ;                                                                        
	        NOP					TP<0x2000 TP2<0x2000   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;
            NOP         W R  										                    ;                                                             
	        NOP					TP<0x4000 TP2<0x4000   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                     
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;
            NOP         W R  										                    ;                                                 
            NOP					TP<0x8000 TP2<0x8000   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                     
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;
            NOP         W R  										                    ;
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

			START OPCODE_JET_00;
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
                                                                                                       		
    		NOP  T1                                                                     ;                          
A0:         NOP          	  	   		TP<TP+1 TP2<TP2+1 			  	    	
            JET A0       	  W R                                                        ;
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
         	STPS                                                                        ;
END

			START OPCODE_OUT_00;
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
                                        
			OUT  2									;                                                 
			NOP										   									;                                                 
			NOP										   									;                                     
			NOP										   									;                                                 
			NOP										   									;                                     
			NOP										   									;                                                 
			NOP										   									;                                     
			NOP										   									;                                                 
			OUT  1																	;                                     
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
 
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
                                       
    		STPS									 ;
	END		


			START OPCODE_REF_00;
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
                             
			NOP    T0 	 I							;                                                                                           
			NOP          I 							;                                                                                           
			NOP          I 							;                                                                                           
			REPEAT 100   I 							;                                                                                           
            NOP          	       		 			;
            NOP            	  	TP<0x1111 TP2<0x2222     ;
            NOP         W  R 	    		 			 ; //1111
            NOP     I		 			                 ;//2
            NOP     I		 			                 ;//3
            NOP     I		 			                 ;//4
            NOP     I		 			                 ;
            NOP     I		 			                 ;
            NOP     			                         ;
            NOP     I     	  	   		 			 ;

            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;                                                                                         																														
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP          	  	   		 			 ;                                                                                         																														
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;                                                                                         																														
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;                                                                                         																														
			STPS   I							  	 ;			
END
     

			START OPCODE_REF_01;
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
                             
			NOP    T0 	 I							;                                                                                           
			NOP          I 							;                                                                                           
			NOP          I 							;                                                                                           
			REPEAT 100   I 							;                                                                                           
            NOP          	       		 			;
            NOP            	                        ;
            NOP         	    		 		    ; //1111
            NOP     I		 			                 ;//2
            NOP     I		 			                 ;//3
            NOP     I		 			                 ;//4
            NOP     I		 			                 ;
            NOP     I		 			                 ;
            NOP     			                         ;
            NOP     I     	  	   		 			 ;

            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;                                                                                         																														
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP          	  	   		 			 ;                                                                                         																														
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;                                                                                         																														
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;
            NOP    I      	  	   		 			 ;                                                                                         																														
			STPS   I							  	 ;		       												  
END

		START OPCODE_JZD_00;
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
 
	    NOP                                      	                  					;
	    NOP                                      	                  					;									
        NOP                                      TP<0x1111 TP2<0x1111  					;
        JZD AA        W R                                              					; //1111                      
AA:     NOP           W R                                              					; //EEEE          /D  					 	  															 
        NOP                                      TP<0x2222 TP2<0x2222  					; 
        JZD BB        W R                                              					; //DDDD          /D
BB:     NOP           W R                                              					; //2222					 	  															 
CC:     NOP           W R                        TP<0x3333 TP2<0x3333  					; //2222    CCCC  /D          
        JZD CC        W R                                              					; //3333    CCCC  /D					 	  															 
DD:     NOP           W R                        TP<0x4444 TP2<0x4444  					; //3333    BBBB  /D
        JZD DD        W R                                              					; //4444    BBBB  /D					  
        NOP                                      TP<0x5555 TP2<0x5555  					; //5555    AAAA  /D
EE:     NOP           W R                                              					; //5555    AAAA  /D
        JZD EE        W R                                                               ;                                                                                        
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
 	
        STPS																			;
END			


			START OPCODE_RPT_00; // Read & Comp vlaue diff
            NOP          	  	   		 			                                    ;
            NOP          	  	   		 			                                    ;
            NOP          	  	   		 			                                    ;
            NOP          	  	   		 			                                    ;
            NOP          	  	   		 			                                    ;
            NOP          	  	   		 			                                    ;
            NOP          	  	   		 			                                    ;
            NOP          	  	   		 			                                    ;
            NOP          	  	   		 			                                    ;
            NOP          	  	   		 			                                    ;
              
            NOP                                             TP<0x1111 TP2<0xAAAA        ;
			REPEAT 3       	 	 W R                                        			;
			NOP                                             TP<0x2222 TP2<0xBBBB        ;                                              
			REPEAT 4             W R                                        			;
			NOP				                             	TP<0x3333 TP2<0xCCCC        ;                     
			REPEAT 3             W R                                       				;
            NOP          	  	   		 			                                    ;
            NOP          	  	   		 			                                    ;
            NOP          	  	   		 			                                    ;
            NOP          	  	   		 			                                    ;
            NOP          	  	   		 			                                    ;
            NOP          	  	   		 			                                    ;
            NOP          	  	   		 			                                    ;
            NOP          	  	   		 			                                    ;
            NOP          	  	   		 			                                    ;
            NOP          	  	   		 			                                    ;
 																														
			STPS																		;
END

		START OPCODE_STI_00;  
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
 
                                               
 LA:	NOP										TP<0x1000 TP2<0x5000					;                                
        JNI0 LA      W R            													;  																;
 LB:	NOP										TP<0x2000 TP2<0x6000					;                                
        JNI1 LB      W R                         										;  
 		NOP																				;                                
		NOP																				;                         
        NOP																				;                                     
        NOP																				;                                     
        STI0 0x3                               TP<0x3000 TP2<0x7000	      			    ;
        NOP																				;                                     
        NOP																				;                                     
        NOP																				;                                     
 LA1:   NOP																				;                                     
        JNI0 LA1      W R                       										;  
        NOP																				;                                     
        NOP																				;                                     
        STI1 0x2                               TP<0x4000 TP2<0x8000	  				 	;
        NOP																				;                                     
        NOP																				;                                     
        NOP																				;                                     
 LB1:   NOP																				;                     
        JNI1 LB1      W R                        										;
        NOP																				;                                     
        NOP																				;                                     
        NOP																				;                                     
 
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
    	STPS                                                                            ;
END  


		START OPCODE_STI_01;  
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
 
                                               
 LA:	NOP										TP<0x1000 TP2<0x5000					;                                
        JNI2 LA      W R            													;  																;
 LB:	NOP										TP<0x2000 TP2<0x6000					;                                
        JNI3 LB      W R                         										;  
 		NOP																				;                                
		NOP																				;                         
        NOP																				;                                     
        NOP																				;                                     
        STI2 0x3                               TP<0x3000 TP2<0x7000	      			    ;
        NOP																				;                                     
        NOP																				;                                     
        NOP																				;                                     
 LA1:   NOP																				;                                     
        JNI2 LA1      W R                       										;  
        NOP																				;                                     
        NOP																				;                                     
        STI3 0x2                               TP<0x4000 TP2<0x8000	  				 	;
        NOP																				;                                     
        NOP																				;                                     
        NOP																				;                                     
 LB1:   NOP																				;                     
        JNI3 LB1      W R                        										;
        NOP																				;                                     
        NOP																				;                                     
        NOP																				;                                     
 
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
    	STPS                                                                            ;
END                                                                                     ;
 		START OPCODE_STI_02;                                        
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
 
                                               
 LA:	NOP										TP<0x1000 TP2<0x5000					;                                
        JNI4 LA      W R            													;  																;
 LB:	NOP										TP<0x2000 TP2<0x6000					;                                
        JNI5 LB      W R                         										;  
 		NOP																				;                                
		NOP																				;                         
        NOP																				;                                     
        NOP																				;                                     
        STI4 0x3                               TP<0x3000 TP2<0x7000	      			    ;
        NOP																				;                                     
        NOP																				;                                     
        NOP																				;                                     
 LA1:   NOP																				;                                     
        JNI4 LA1      W R                       										;  
        NOP																				;                                     
        NOP																				;                                     
        STI5 0x2                               TP<0x4000 TP2<0x8000	  				 	;
        NOP																				;                                     
        NOP																				;                                     
        NOP																				;                                     
 LB1:   NOP																				;                     
        JNI5 LB1      W R                        										;
        NOP																				;                                     
        NOP																				;                                     
        NOP																				;                                     
 
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
    	STPS                                                                            ;
END                                                                                     ;
 
		START OPCODE_STI_03;  
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
        NOP          	  	   		 			                                        ;
 
                                               
 LA:	NOP										TP<0x1000 TP2<0x5000					;                                
        JNI6 LA      W R            													;  																;
 LB:	NOP										TP<0x2000 TP2<0x6000					;                                
        JNI7 LB      W R                         										;  
 		NOP																				;                                
		NOP																				;                         
        NOP																				;                                     
        NOP																				;                                     
        STI6 0x3                               TP<0x3000 TP2<0x7000	      			    ;
        NOP																				;                                     
        NOP																				;                                     
        NOP																				;                                     
 LA1:   NOP																				;                                     
        JNI6 LA1      W R                       										;  
        NOP																				;                                     
        NOP																				;                                     
        STI7 0x2                               TP<0x4000 TP2<0x8000	  				 	;
        NOP																				;                                     
        NOP																				;                                     
        NOP																				;                                     
 LB1:   NOP																				;                     
        JNI7 LB1      W R                        										;
        NOP																				;                                     
        NOP																				;                                     
        NOP																				;                                     
 
        NOP          	  	   		 			 ;
        NOP          	  	   		 			 ;
        NOP          	  	   		 			 ;
        NOP          	  	   		 			 ;
        NOP          	  	   		 			 ;
        NOP          	  	   		 			 ;
        NOP          	  	   		 			 ;
        NOP          	  	   		 			 ;
        NOP          	  	   		 			 ;
    	STPS                                                                            ;
END                                                                          ;
 

		START OPCODE_LDI_00;  
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

                                     
A0:     NOP                        	         TP<0x1000 TP2<0xA000						;                   
        JNI0 A0          W  R       													;              
A1:     NOP                         		 TP<0x2000 TP2<0xB000						;                             
        JNI1 A1          W  R       													;              
        NOP                         													;             
        LDI0  8                     													;             
        LDI1  9                     													;             
        NOP                         													;             
        NOP                         													;             
A3:     NOP                         		 TP<0x4000 TP2<0xC000						;             
        JNI0 A3          W  R       													;//loop 10    7        
A4:     NOP                         		 TP<0x5000 TP2<0xD000						;         
        JNI1 A4          W  R       													;//loop 11    11         
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
        NOP          	  	   		 			 ;

    	STPS                        													;
END   

		START OPCODE_LDI_01;  
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

                                     
A0:     NOP                        	         TP<0x1000 TP2<0xA000						;                   
        JNI2 A0          W  R       													;              
A1:     NOP                         		 TP<0x2000 TP2<0xB000						;                             
        JNI3 A1          W  R       													;              
        NOP                         													;             
        LDI2  10                     													;             
        LDI3  11                     													;             
        NOP                         													;             
        NOP                         													;             
A3:     NOP                         		 TP<0x4000 TP2<0xC000						;             
        JNI2 A3          W  R       													;//loop 10    7        
A4:     NOP                         		 TP<0x5000 TP2<0xD000						;         
        JNI3 A4          W  R       													;//loop 11    11         
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
        NOP          	  	   		 			 ;

    	STPS                        													;
END 
		START OPCODE_LDI_02;  
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

                                     
A0:     NOP                        	         TP<0x1000 TP2<0xA000						 ;                   
        JNI4 A0          W  R       													 ;              
A1:     NOP                         		 TP<0x2000 TP2<0xB000						 ;                             
        JNI5 A1          W  R       													 ;              
        NOP                         													 ;             
        LDI4  12                     													 ;             
        LDI5  13                     													 ;             
        NOP                         													 ;             
        NOP                         													 ;             
A3:     NOP                         		 TP<0x4000 TP2<0xC000						 ;             
        JNI4 A3          W  R       													 ;//loop 10    7        
A4:     NOP                         		 TP<0x5000 TP2<0xD000						 ;         
        JNI5 A4          W  R       													 ;//loop 11    11         
        NOP          	  	   		 			                                         ;
        NOP          	  	   		 			                                         ;
        NOP          	  	   		 			                                         ;
        NOP          	  	   		 			                                         ;
        NOP          	  	   		 			                                         ;
        NOP          	  	   		 			                                         ;
        NOP          	  	   		 			                                         ;
        NOP          	  	   		 			                                         ;
        NOP          	  	   		 			                                         ;
        NOP          	  	   		 			                                         ;
        NOP          	  	   		 			                                         ;

    	STPS                        												 	 ;
END  
		START OPCODE_LDI_03;  
        NOP          	  	   		 			                                         ;
        NOP          	  	   		 			                                         ;
        NOP          	  	   		 			                                         ;
        NOP          	  	   		 			                                         ;
        NOP          	  	   		 			                                         ;
        NOP          	  	   		 			                                         ;
        NOP          	  	   		 			                                         ;
        NOP          	  	   		 			                                         ;
        NOP          	  	   		 			                                         ;
        NOP          	  	   		 			                                         ;

                                     
A0:     NOP                        	         TP<0x1000 TP2<0xA000						;                   
        JNI6 A0          W  R       													;              
A1:     NOP                         		 TP<0x2000 TP2<0xB000						;                             
        JNI7 A1          W  R       													;              
        NOP                         													;             
        LDI6  14                     													;             
        LDI7  15                     													;             
        NOP                         													;             
        NOP                         													;             
A3:     NOP                         		 TP<0x4000 TP2<0xC000						;             
        JNI6 A3          W  R       													;//loop 10    7        
A4:     NOP                         		 TP<0x5000 TP2<0xD000						;         
        JNI7 A4          W  R       													;//loop 11    11         
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
        NOP          	  	   		 			 ;

    	STPS                        													;
END 

       START OPCODE_JNI_00;  
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;                                  
A0:     NOP                        	         TP<0x1111 TP2<0xAAAA   ;                   
        JNI0 A0          W  R       							    ;              
A1:     NOP                         		 TP<0x2222 TP2<0xBBBB   ;                             
        JNI1 A1          W  R       							    ;              
        NOP                         							    ;             
        NOP                         							    ;             
        NOP                         							    ;             
A3:     NOP                         		 TP<0x3333 TP2<0xCCCC   ;             
        JNI2 A3          W  R       							    ;     
A4:     NOP                         		 TP<0x4444 TP2<0xDDDD   ;         
        JNI3 A4          W  R       							    ;        
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
    	STPS                        								;
END

        START OPCODE_JNI_01;  
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;                                  
A0:     NOP                        	         TP<0x1111 TP2<0xAAAA   ;                   
        JNI4 A0          W  R       							    ;              
A1:     NOP                         		 TP<0x2222 TP2<0xBBBB   ;                             
        JNI5 A1          W  R       							    ;              
        NOP                         							    ;             
        NOP                         							    ;             
        NOP                         							    ;             
A3:     NOP                         		 TP<0x3333 TP2<0xCCCC   ;             
        JNI6 A3          W  R       							    ;      
A4:     NOP                         		 TP<0x4444 TP2<0xDDDD   ;         
        JNI7 A4          W  R       							    ;        
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
    	STPS                        								;
END

        START OPCODE_PAS_00;  
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;                                  
        NOP          	  	   	    TP<0    TP2<0x8000              ;
        PAUSE 1000NS    W  R    	TP<TP+1 TP2<TP2+1               ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
    	STPS                        								;
END

        START OPCODE_PAS_01;  
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;                                  
        NOP          	  	   	TP<0    TP2<0x8000                  ;
        PAUSE 1.0US    W  R    	TP<TP+1 TP2<TP2+1                   ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
    	STPS                        								;
END

        START IO_MASK;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
		NOP					         TP<0x0000 TP2<0x0000  	   		;
		NOP		W	 R		         TP<0x1111 TP2<0x2222 	   		;
		NOP		W	 R		         TP<0x0000 TP2<0x0000 	   		;
		NOP		W	 R		         TP<0x3333 TP2<0x4444 	   		;        
		NOP		W	 R		         TP<0x0000 TP2<0x0000 	   		;
		NOP		W	 R		         TP<0x5555 TP2<0x6666 	   		;                
		NOP		W	 R		         TP<0x0000 TP2<0x0000 	   		;
		NOP		W	 R		         TP<0x7777 TP2<0x8888 	   		;
		NOP		W	 R		         TP<0x0000 TP2<0x0000 	   		;
		NOP		W	 R		         TP<0x9999 TP2<0xaaaa 	   		;
		NOP		W	 R		         TP<0x0000 TP2<0x0000 	   		;
		NOP		W	 R		         TP<0xbbbb TP2<0xcccc 	   		;
		NOP		W	 R		         TP<0x0000 TP2<0x0000 	   		;
		NOP		W	 R		         TP<0xdddd TP2<0xeeee 	   		;
		NOP		W	 R		         TP<0x0000 TP2<0x0000 	   		;
		NOP		W	 R		         TP<0xffff TP2<0xffff 	   		;
        NOP     W    R 	  	   		 TP<0x0000 TP2<0x0000           ;
        NOP     W     	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
		STPS								                        ;
END

        START OPCODE_JRB_00;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 TP<0 TP2<0 	         	    ;
        JRB 0               	   	 TP<TP+1 TP2<TP2+1	 	        ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
		NOP					                              	   		;
		NOP		W	 R		                             	   		;
		NOP		W	 R		                             	   		;
		NOP		W	 R		                             	   		;        
		NOP		W	 R		                             	   		;
		NOP		W	 R		                             	   		;                
		NOP		W	 R		                             	   		;
		NOP		W	 R		                             	   		;
		NOP		W	 R		                             	   		;
		NOP		W	 R		                             	   		;
		NOP		W	 R		                             	   		;
		NOP		W	 R		                             	   		;
		NOP		W	 R		                             	   		;
		NOP		W	 R		                             	   		;
		NOP		W	 R		                             	   		;
		NOP		W	 R		                             	   		;
        NOP     W    R 	  	   		                                ;
        NOP           	  	   		 			                    ;
   
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
        NOP          	  	   		 			                    ;
		STPS								                        ;
END


;%ENDC 