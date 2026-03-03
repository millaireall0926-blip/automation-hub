

%IFE A .AND. B .AND.((C.OR.D).AND.E)

%INSERT Header/define.h

 REGISTER
			XMAX=0xffffff;                                                                                                         
			YMAX=0xffffff;                                                                                                           
			ZMAX=0xff;                                                                                                          
			LMAX=0xffffff;                                                                                                           
			HMAX=0xfffffe;                                                                                                           
			XOS=0x1;                                                                                                           
			IDX0=0x1;                                                                                                           
			IDX1=0x2;                                                                                                        
			IDX2=0x3;                                                                                                        
			IDX3=0x1;                                                                                                        
			IDX4=0x2;                                                                                                        
			IDX5=0x3;	                                                                                                    	
			IDX6=0x1;	                                                                                                          	
			IDX7=0x2;                                                                                                           
			IDX8=0x1;                                                                                                         
            IDX9=0x2;                                                                                                           
            IDX10=0x3;                                                                                                           
            IDX11=0x1;                                                                                                         
            IDX12=0x2;                                                                                                              
            IDX13=0x3;                                                                                                         
            IDX14=0x1;                                                                                                       
            IDX15=0x2;               
			XH=0x1fffff;   
            XT=0x2fffff;   
            YH=0x3fffff;   
            YT=0x4fffff;   
            ZH=0xff;       
            D1A=0xfffff1;  
            D1B=0xfffff2;  
            D1C=0xfffff3;  
            D1D=0xfffff4;  
            D1E=0xfffff5;  
            D1F=0xfffff6;  
            D1G=0xfffff7;  
            D1H=0xfffff8;  
            D2A=0xfffff9;  
            D2B=0xffff1f;  
            D2C=0xffff2f;  
            D2D=0xffff3f;  
            D3B=0xffff4f;  
            D4B=0xffff5f;  
            TPH1=0x111111; 
            TPH1A=0x222222;
            TPH1B=0x333333;
			TPH1C=0x444444;   
            TPH1D=0x555555;   
            TPH2=0x666666;    
            TPH2A=0x777777;   
            TPH2B=0x888888;   
            TPH2C=0x999999;   
            TPH2D=0xAAAAAA;   
            D5=0xffffff;      
            D6=0xeeeeee;      
            D7=0xdddddd;      
            D8=0xcccccc;      
            YOS=0x1;          
            DCMR=0x111111;    
            DCMR2=0x222222;   
            DBMAP=0x3333;     
            ;TIMER=0x444444;   
            PWAIT=0x000001;   
            ;STISP=0x6666;     
            ;REFTIMER=0x777777;
            CFLG=0x8888;      
            ;MOD=0x99;         
            SCAN=0xAAAAAA;    
            VMPA=0xBBBBBB;  
			STATUS=0xCCCCCC;
			XMASK=0x111111;
			YMASK=0x222222;
			ZMASK=0x33;
			CCD=0x444444;
			RCD=0x555555;
			DSD=0x666666;
			DCMRH=0x777777;
			DCMRL=0x888888;
			BL=0x99;
			BT=0xaa;
			WL=0xbb;
			RL=0xcc;
			XMIN=0xdddddd;
			YMIN=0xeeeeee;
			ZMIN=0xff;
			S1A=0x11;
			;MLOOP=0xff;
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
			NOP					TP<0xFFFF TP2<0x0009   									;                                                 
			NOP			W R		TP<0xEEEE TP2<0x0099   									;                                                 
			NOP					TP<0xDDDD TP2<0x0999   									;                                                 
			NOP			W R		TP<0xCCCC TP2<0x9999   									;                                                 
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP			W R																;                                     
			NOP																			;                                     
			NOP																			;                                     
			RTN			
ENDFUNCTION	

            START OPCODE_NOP_00;
            REPEAT 30																			 ;
            NOP																					 ;
            NOP		     			 X<XC Y<YC 	 												 ;
            NOP		     			 X<XC Y<YC													 ;
            NOP		     			 X<XC Y<YC 													 ;
            NOP		     	    	 X<XC Y<YC													 ;
            NOP		     	   	 	 X<XC Y<YC                   	  			    			 ;
            NOP		     	     	 X<XC Y<YC                   	       		Z<0x0       	 ;
            NOP			      	 	 X<XC Y<YC                     	 XC<0x80          			 ;
            NOP			      	 	 X<XC Y<YC                      	   	 	 	   			 ;
            NOP			      	 	 X<XC Y<YC                      	  	   		 			 ;
            NOP			      	 	 X<XC Y<YC            TP<0x0001 TP2<0x000A   XC<XC+1 YC<YC+1 ;  
            NOP			C19  C17 	 X<XC Y<YC            TP<0x0002 TP2<0x000B   XC<XC+1 YC<YC+1 ;  
            NOP			C19  C17 	 X<XC Y<YC            TP<0x0004 TP2<0x000C   XC<XC+1 YC<YC+1 ;  
            NOP			C19  C17 	 X<XC Y<YC            TP<0x0008 TP2<0x000D	 XC<XC+1 YC<YC+1 ;  
            NOP			C19  C17 	 X<XC Y<YC            TP<0x0010 TP2<0x00A0	 XC<XC+1 YC<YC+1 ;  
            NOP			C19  C17 	 X<XC Y<YC            TP<0x0020 TP2<0x00B0	 XC<XC+1 YC<YC+1 ;  
            NOP			C19  C17 	 X<XC Y<YC            TP<0x0040 TP2<0x00C0	 XC<XC+1 YC<YC+1 ;  
            NOP			C19  C17 	 X<XC Y<YC            TP<0x0080 TP2<0x00D0	 XC<XC+1 YC<YC+1 ;  
            NOP			C19  C17 	 X<XC Y<YC            TP<0x0100 TP2<0x0A00	 XC<XC+1 YC<YC+1 ;  
            NOP			C19  C17 	 X<XC Y<YC            TP<0x0200 TP2<0x0B00	 XC<XC+1 YC<YC+1 ;  
            NOP			C19  C17 	 X<XC Y<YC             										 ;  
   		    NOP																					 ;
            NOP																					 ;
            NOP																					 ;
            NOP																					 ;
            NOP																					 ;
            NOP																					 ;
            NOP																					 ;
            NOP											   ZH<0x1 								 ;
            STPS																				 ;
END  

START OPCODE_NOP_01;
            REPEAT 33																			 ;
            NOP																					 ;
            NOP		     		 X<XC Y<YC 	 												 	 ;
            NOP		     		 X<XC Y<YC													     ;
            NOP		     		 X<XC Y<YC 													     ;
            NOP		     	     X<XC Y<YC													     ;
            NOP		     	   	 X<XC Y<YC                   	  			    			     ;
            NOP		     	     X<XC Y<YC                   	       		Z<0x0       	     ;
            NOP			      	 X<XC Y<YC                     	 XC<0x40          			     ;
            NOP			      	 X<XC Y<YC                      	   	 	 	   			     ;
            NOP			      	 X<XC Y<YC                      	  	   		 			     ;
            NOP			      	 X<XC Y<YC            TP<0xFFFF TP2<0x0055   XC<XC+1 YC<YC+1     ;  
            NOP			W  R 	 																 ;
			NOP  				 X<XC Y<YC            TP<0x7FFF TP2<0x00AA   XC<XC+1 YC<YC+1     ;
            NOP			W  R 	 																 ;
			NOP					 X<XC Y<YC            TP<0x3FFF TP2<0x0155   XC<XC+1 YC<YC+1     ;
            NOP			W  R 	   																 ;
			NOP					 X<XC Y<YC            TP<0x1FFF TP2<0x02AA	 XC<XC+1 YC<YC+1     ;
            NOP			W  R 	  																 ;
			NOP					 X<XC Y<YC            TP<0x0FFF TP2<0x0555	 XC<XC+1 YC<YC+1     ; 
            NOP			W  R 	   																 ;
			NOP					 X<XC Y<YC            TP<0x07FF TP2<0x0AAA	 XC<XC+1 YC<YC+1     ;
            NOP			W  R 	   																 ;
			NOP					 X<XC Y<YC            TP<0x03FF TP2<0x1555	 XC<XC+1 YC<YC+1     ;
            NOP			W  R 	  																 ;
			NOP					 X<XC Y<YC            TP<0x01FF TP2<0x2AAA 	 XC<XC+1 YC<YC+1     ; 
            NOP			W  R 	 																 ;
			NOP					 X<XC Y<YC            TP<0x00FF TP2<0x5555	 XC<XC+1 YC<YC+1     ;  
            NOP			W  R 	  																 ;
			NOP					 X<XC Y<YC            TP<0x007F TP2<0xAAAA	 XC<XC+1 YC<YC+1     ; 
            NOP			W  R 	 X<XC Y<YC             										     ;  
   		    NOP																					 ;
            NOP																					 ;
            NOP																					 ;
            NOP																					 ;
            NOP																					 ;
            NOP																					 ;
            NOP																					 ;
            NOP											   ZH<0x1 								 ;
            STPS																				 ;
END  


  
			START OPCODE_JMP_00;
			REPEAT 30																	;                                                                  
			NOP																			;                                      
			NOP																			;                                                 
			NOP																			;                                                 
			NOP					TP<0x0000 TP2<0xFFFF   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2-1   									;                                                 
            JMP A2  	W R		TP<TP+1 TP2<TP2-1   									;                                                 
A2:         JMP A3    	W R		TP<TP+1 TP2<TP2-1   									;                                                 
            NOP      	W R		TP<0xAAAA TP2<0xAAAA   									;                                                 
A3:         JMP A4 		W R		TP<TP+1 TP2<TP2-1   									;                                                 
            NOP   		W R		TP<0xAAAA TP2<0xAAAA   									;                                                 
            NOP 	  	W R		TP<0xBBBB TP2<0xBBBB   									;                                                 
A4:         JMP A5  	W R		TP<TP+1 TP2<TP2-1   									;                                                 
		    NOP      	W R		TP<0xAAAA TP2<0xAAAA 									;                                                 
			NOP			W R		TP<0xBBBB TP2<0xBBBB 									;                                     
			NOP			W R		TP<0xCCCC TP2<0xCCCC 									;                                                 
A5:			JMP A6   	W R		TP<TP+1 TP2<TP2-1   									;                                     
			NOP			W R		TP<0xAAAA TP2<0xAAAA 									;                                                 
			NOP			W R		TP<0xBBBB TP2<0xBBBB   									;                                     
			NOP			W R		TP<0xCCCC TP2<0xCCCC 									;                                                 
			NOP			W R		TP<0xDDDD TP2<0xDDDD 									;                                     
A6:			NOP			W R		TP<TP+1 TP2<TP2-1 										;                                     
			NOP			W R		TP<TP+1 TP2<TP2-1 										;                                     
			NOP			W R		TP<TP+1 TP2<TP2-1 										;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;
			NOP																			;                                     
			NOP																			;                                     
    		STPS																		;
END  	

			START OPCODE_JSR_00;
			REPEAT 30																	;                                                                  
			NOP																			;                                                                  
			NOP																			;                                      
			NOP																			;                                                 
			NOP																			;                                                 
			NOP																			;                                                 
			JSR  OPCODERTNA	                             								;          
			JSR  OPCODERTNB	                                    					 	;          
			JSR  OPCODERTNC                                  					 		;          
    		NOP	 																		;                                                 
			NOP																			;                                                 
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			STPS																		;                                     
END 

FUNCTION OPCODERTNA                                                                   
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
																						;                                     
FUNCTION OPCODERTNB 
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
					
FUNCTION OPCODERTNC
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
			RTN																			; // RTN 제외해도 진행됌.Error 발생 안함. JSR-RTN                                       
ENDFUNCTION 

			START OPCODE_JNC_00;
			REPEAT 30																	;                                                                  
			NOP																			;                                      
			NOP																			;                                                 
			JNC0 LA																		;                                                 
			NOP					TP<0x0000 TP2<0x0000   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1  										;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1  										;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1  										;                                                 
LA:			JNC1 LB																		;                                                 
			NOP					TP<0x1000 TP2<0x1000   	 								;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                            
LB:			JNC2 LC																		;                                      
			NOP					TP<0x2000 TP2<0x2000   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
LC:			JNC3 LD																		;                                                 
			NOP					TP<0x4000 TP2<0x4000   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                     
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                     
LD:			JNC4 LE																		;                                                 
			NOP					TP<0x8000 TP2<0x8000   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                     
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                     
LE:			NOP																			;                                                 
			NOP					TP<0xA000 TP2<0xA000   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                     
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R																;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
    		STPS																		;
END

			START OPCODE_JNC_01;
			REPEAT 30																	;                                                                  
			NOP																			;                                      
			NOP																			;                                                 
			JNC5 LA																		;                                                 
			NOP					TP<0x0000 TP2<0x0000   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1  										;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1  										;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1  										;                                                 
LA:			JNC6 LB																		;                                                 
			NOP					TP<0x1000 TP2<0x1000   	 								;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                            
LB:			JNC7 LC																		;                                      
			NOP					TP<0x2000 TP2<0x2000   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
LC:			JNC8 LD																		;                                                 
			NOP					TP<0x4000 TP2<0x4000   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                     
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                     
LD:			JNC9 LE																		;                                                 
			NOP					TP<0x8000 TP2<0x8000   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                     
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                     
LE:			NOP																			;                                                 
			NOP					TP<0xA000 TP2<0xA000   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                     
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R																;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
    		STPS																		;
END

			START OPCODE_JNC_02; // JNC10 ~ JNC15
			REPEAT 50																	;                                                                  
			NOP																			;                                      
			NOP																			;                                                 
			JNC10 LA																	;                                                 
			NOP					TP<0x0000 TP2<0x0000   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1  										;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1  										;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1  										;                                                 
LA:			JNC11 LB																	;                                                 
			NOP					TP<0x1000 TP2<0x1000   	 								;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                            
LB:			JNC12 LC																	;                                      
			NOP					TP<0x2000 TP2<0x2000   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
LC:			JNC13 LD																	;                                                 
			NOP					TP<0x4000 TP2<0x4000   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                     
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                     
LD:			JNC14 LE																	;                                                 
			NOP					TP<0x8000 TP2<0x8000   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                     
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                     
LE:			JNC15 LF																	;                                                 
			NOP					TP<0xA000 TP2<0xA000   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                     
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R																;                                     
LF:			NOP																			;                                     
			NOP					TP<0xF000 TP2<0xF000   									;                                                 
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                     
			NOP			W R		TP<TP+1 TP2<TP2+1   									;                                                 
			NOP			W R																;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
    		STPS																		;
END

		START OPCODE_JET_00;
        REPEAT 30;                                                                                                              		
    	NOP  T1                                                                         ; //Timer1 Start                          
A0:		NOP                                      TP<TP+1 TP2<TP2+1                      ;
        JET A0       	  W R                                                           ;
    	NOP                                                                             ;                          
    	NOP                                                                             ;                          
    	NOP                                                                             ;                                                                                                                                              
        STPS                                                                            ;
END

			START OPCODE_OUT_00;
			NOP																			;                                                                  
			NOP																			;                                      
			NOP																			;                                                 
			NOP																			;                                                 
			NOP										   									;                                                 
			NOP										   									;                                                 
			NOP										   									;                                                 
			NOP										   									;                                                 
			NOP										   									;                                                 
			NOP										   									;                                                 
			NOP										   									;                                                 
			NOP	 									   									;                                                 
			OUT  154										   							; // BURST 에 154 실핼 후 다음 Clock 감                                                 
			NOP										   									;                                                 
			NOP										   									;                                     
			NOP										   									;                                                 
			NOP										   									;                                     
			NOP										   									;                                                 
			NOP										   									;                                     
			NOP										   									;                                                 
			OUT  234																	;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
    		STPS
	END		


			START OPCODE_REFRESH_00;
			//STISP=RFAA	//STISP 실행 X			
    		REPEAT 30  																		;                                       
			NOP    T0 																		; //Timer0 Start                                                                                          
			NOP         I 																	;                                                                                           
			NOP         I 																	;                                                                                           
			REPEAT 30   I 																	;                                                                                           
			NOP         I 																	;                                                                                                                    
			NOP       																		;                                   	
			NOP         I 																	;  	               					
			NOP         I 																	;                                                                                           
			NOP         I 																	;                                                                                           
			NOP         I 																	;                                                                                           																														
			STPS																			;															  
RFAA:
			NOP         W  R              TP<TP+1 TP2<TP2+1									;                                                        
			NOP         W  R              TP<TP+1 TP2<TP2+1									;                                                        
			NOP         W  R              TP<TP+1 TP2<TP2+1									;                                                        
			NOP         W  R              TP<TP+1 TP2<TP2+1									;                                                        
			NOP         W  R              TP<TP+1 TP2<TP2+1									;                                                        
			NOP         W  R              TP<TP+1 TP2<TP2+1									;                                                        
			NOP         W  R              TP<TP+1 TP2<TP2+1									;                                                                                                                                                                       
			RTN																				;
RFAB:
			NOP         W  R              TP<TP+1 TP2<TP2+1									;                                                        
			NOP         W  R              TP<TP+1 TP2<TP2+1									;                                                        
			NOP         W  R              TP<TP+1 TP2<TP2+1									;                                                        
			NOP         W  R              TP<TP+1 TP2<TP2+1									;                                                        
			NOP         W  R              TP<TP+1 TP2<TP2+1									;                                                        
			NOP         W  R              TP<TP+1 TP2<TP2+1									;                                                        
			NOP         W  R              TP<TP+1 TP2<TP2+1									;                                                                                                                                                                       
			RTN																				;
END

		START OPCODE_JZD_00;
        REPEAT 30                                      	                  				;
	    NOP                                      	                  					;
	    NOP                                      	                  					;									
        NOP                                      TP<0x1111 TP2<0x1111  					;
        JZD AA        W R                                              					; //1111                      
AA:     NOP           W R                                              					; //EEEE          /D  					 	  															;
        NOP                                      TP<0x2222 TP2<0x2222  					; 
        JZD BB        W R                                              					; //DDDD          /D
BB:     NOP           W R                                              					; //2222					 	  															;
CC:     NOP           W R                        TP<0x3333 TP2<0x3333  					; //                                         //2222    CCCC  /D          
        JZD CC        W R                                              					;                                          //3333    CCCC  /D					 	  															;
DD:     NOP           W R                        TP<0x4444 TP2<0x4444  					;                                          //3333    BBBB  /D
        JZD DD        W R                                              					;                                          //4444    BBBB  /D					  
        NOP                                      TP<0x5555 TP2<0x5555  					;                                          //5555    AAAA  /D
EE:     NOP           W R                                              					;                                          //5555    AAAA  /D
        JZD EE        W R                                                               ;                                                                                        
        STPS																			;
END			


		START OPCODE_RPT_00;
		REPEAT 30                                                                          ;                        
		NOP                                             TP<0x1111 TP2<0xAAAA            ;
		REPEAT 3       	 	 W R                                        					;
		NOP                                             TP<0x2222 TP2<0xBBBB            ;                                              
		REPEAT 4             W R                                        					;
		NOP				                             	TP<0x3333 TP2<0xCCCC            ;                     
		REPEAT 5             W R                                       					;
		NOP				                           		TP<0x4444 TP2<0xDDDDD           ;                     
		REPEAT 4             W R                                        					;
		NOP				                                                                ; 																														
		STPS																			;
END

		START OPCODE_STI_00;
		REPEAT 30																			;                                                     
		NOP																				;                         
		NOP																				;                                
		NOP																				;                                
 LA:	NOP										TP<0x1111 TP2<0x1111					;                                
        JNI0 LA      W R            
 LB:	NOP										TP<0x2222 TP2<0x2222					;                                
        JNI1 LB      W R                         										;
 LC:	NOP										TP<0x3333 TP2<0x3333 					;                                
        JNI2 LC      W R                         										;
		NOP																				;                                
		NOP																				;                         
        NOP																				;                                     
        NOP																				;                                     
        STI0 0x3                               TP<0x1111 TP2<0x1111	      			    ;
        NOP																				;                                     
 LA1:   NOP																				;                                     
        JNI0 LA1      W R                       										;
        NOP																				;                                     
        NOP																				;                                     
        STI1 0x2                               TP<0x2222 TP2<0x2222	  				 	;
        NOP																				;                                     
 LB1:   NOP																				;                                     
        JNI1 LB1      W R                        										;
        NOP																				;                                     
        NOP																				;                                     
        NOP																				;                                     
        STI2 0x1                               TP<0x3333 TP2<0x3333   					;
        NOP																				;                                     
 LC1:   NOP																				;                                     
        JNI2 LC1      W R                         										;
      	NOP                                  											;											
      	NOP                                  											;
      	NOP                                  											;
      	NOP                                  											;
      	NOP                                                                             ;
    	STPS                                                                            ;
END   

		START OPCODE_STI_01;
		REPEAT 30																			;                                                     
		NOP																				;                         
		NOP																				;                                
		NOP																				;                                
 LA:	NOP										TP<0x1111 TP2<0x1111					;                                
        JNI3 LA      W R            
 LB:	NOP										TP<0x2222 TP2<0x2222					;                                
        JNI4 LB      W R                         										;
 LC:	NOP										TP<0x3333 TP2<0x3333 					;                                
        JNI5 LC      W R                         										;
		NOP																				;                                
		NOP																				;                         
        NOP																				;                                     
        NOP																				;                                     
        STI3 0x3                               TP<0x1111 TP2<0x1111	      			    ;
        NOP																				;                                     
 LA1:   NOP																				;                                     
        JNI3 LA1      W R                       										;
        NOP																				;                                     
        NOP																				;                                     
        STI4 0x2                               TP<0x2222 TP2<0x2222	  				 	;
        NOP																				;                                     
 LB1:   NOP																				;                                     
        JNI4 LB1      W R                        										;
        NOP																				;                                     
        NOP																				;                                     
        NOP																				;                                     
        STI5 0x1                               TP<0x3333 TP2<0x3333   					;
        NOP																				;                                     
 LC1:   NOP																				;                                     
        JNI5 LC1      W R                         										;
      	NOP                                  											;											
      	NOP                                  											;
      	NOP                                  											;
      	NOP                                  											;
      	NOP                                                                             ;
    	STPS                                                                            ;
END   

		START OPCODE_STI_02;
		REPEAT 30																			;                                                     
		NOP																				;                         
		NOP																				;                                
		NOP																				;                                
 LA:	NOP										TP<0x1111 TP2<0x1111					;                                
        JNI6 LA      W R            
 LB:	NOP										TP<0x2222 TP2<0x2222					;                                
        JNI7 LB      W R                         										;
 LC:	NOP										TP<0x3333 TP2<0x3333 					;                                
        JNI8 LC      W R                         										;
		NOP																				;                                
		NOP																				;                         
        NOP																				;                                     
        NOP																				;                                     
        STI6 0x3                               TP<0x1111 TP2<0x1111	      			    ;
        NOP																				;                                     
 LA1:   NOP																				;                                     
        JNI6 LA1      W R                       										;//loop 5
        NOP																				;                                     
        NOP																				;                                     
        STI7 0x2                               TP<0x2222 TP2<0x2222	  				 	;
        NOP																				;                                     
 LB1:   NOP																				;                                     
        JNI7 LB1      W R                        										;//loop 4
        NOP																				;                                     
        NOP																				;                                     
        NOP																				;                                     
        STI8 0x1                               TP<0x3333 TP2<0x3333   					;
        NOP																				;                                     
 LC1:   NOP																				;                                     
        JNI8 LC1      W R                         										;//loop 3
      	NOP                                  											;											
      	NOP                                  											;
      	NOP                                  											;
      	NOP                                  											;
      	NOP                                                                             ;
    	STPS                                                                            ;
END  
	   
		START OPCODE_LDI_00;
		REPEAT 30                        													;                                           
		NOP                        														;  
		NOP                        														;         
		NOP                        														;         
		NOP                        														;         
		NOP                        														;    
A0:     NOP                        	         TP<0x1111 TP2<0xAAAA						;                   
        JNI0 A0          W  R       													;              
A1:     NOP                         		 TP<0x2222 TP2<0xBBBB						;                             
        JNI1 A1          W  R       													;              
A2:     NOP                         		 TP<0x3333 TP2<0xCCCC						;         
        JNI2 A2          W  R       													;              
        NOP                         													;             
        LDI0  8                     													;             
        NOP                         													;             
        NOP                         													;             
        NOP                         													;             
        NOP                         													;             
        LDI1  9                     													;             
        NOP                         													;             
        NOP                         													;             
        NOP                         													;             
        NOP                         													;             
        LDI2  10                     													;             
        NOP                         													;             
        NOP                         													;             
        NOP                         													;             
        NOP                         													;             
A3:     NOP                         		 TP<0x4444 TP2<0xDDDD						;             
        JNI0 A3          W  R       													;//loop 10           
A4:     NOP                         		 TP<0x5555 TP2<0xEEEE						;         
        JNI1 A4          W  R       													;//loop 11           
A5:     NOP                         		 TP<0x6666 TP2<0xFFFF						;         
        JNI2 A5          W  R       													;//loop 12           
        NOP                         													;             
        NOP                         													;             
        NOP                         													;             
        NOP                         													;             
        NOP                         													;              
    	STPS                        													;
END   

		START OPCODE_LDI_01;
		REPEAT 30                        													;                                           
		NOP                        														;  
		NOP                        														;         
		NOP                        														;         
		NOP                        														;         
		NOP                        														;    
A0:     NOP                        	         TP<0x1111 TP2<0xAAAA						;                   
        JNI3 A0          W  R       													;              
A1:     NOP                         		 TP<0x2222 TP2<0xBBBB						;                             
        JNI4 A1          W  R       													;              
A2:     NOP                         		 TP<0x3333 TP2<0xCCCC						;         
        JNI5 A2          W  R       													;              
        NOP                         													;             
        LDI3  8                     													;             
        NOP                         													;             
        NOP                         													;             
        NOP                         													;             
        NOP                         													;             
        LDI4  9                     													;             
        NOP                         													;             
        NOP                         													;             
        NOP                         													;             
        NOP                         													;             
        LDI5  10                     													;             
        NOP                         													;             
        NOP                         													;             
        NOP                         													;             
        NOP                         													;             
A3:     NOP                         		 TP<0x4444 TP2<0xDDDD						;             
        JNI3 A3          W  R       													;//loop 10           
A4:     NOP                         		 TP<0x5555 TP2<0xEEEE						;         
        JNI4 A4          W  R       													;//loop 11           
A5:     NOP                         		 TP<0x6666 TP2<0xFFFF						;         
        JNI5 A5          W  R       													;//loop 12           
        NOP                         													;             
        NOP                         													;             
        NOP                         													;             
        NOP                         													;             
        NOP                         													;              
    	STPS                        													;
END 

		START OPCODE_LDI_02;
		REPEAT 30                        													;                                           
		NOP                        														;  
		NOP                        														;         
		NOP                        														;         
		NOP                        														;         
		NOP                        														;    
A0:     NOP                        	         TP<0x1111 TP2<0xAAAA						;                   
        JNI6 A0          W  R       													;              
A1:     NOP                         		 TP<0x2222 TP2<0xBBBB						;                             
        JNI7 A1          W  R       													;                           
        NOP                         													;             
        LDI6  8                     													;             
        NOP                         													;             
        NOP                         													;             
        NOP                         													;             
        NOP                         													;             
        LDI7  9                     													;             
        NOP                         													;                       
        NOP                         													;             
        NOP                         													;             
        NOP                         													;             
        NOP                         													;             
A3:     NOP                         		 TP<0x4444 TP2<0xDDDD						;             
        JNI6 A3          W  R       													;//loop 10           
A4:     NOP                         		 TP<0x5555 TP2<0xEEEE						;         
        JNI7 A4          W  R       													;//loop 11                   
        NOP                         													;             
        NOP                         													;             
        NOP                         													;             
        NOP                         													;             
        NOP                         													;              
    	STPS                        													;
END 

%ENDC 