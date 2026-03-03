

;%IFE A .AND. B .AND.((C.OR.D).AND.E)

%INSERT Header/define.h
  
 REGISTER
			XMAX=0xffffff;
			YMAX=0xffffff;
			ZMAX=0xff;
			XOS=0x0;
			YOS=0x0;	
ENDREGISTER		

			START XY_SCR_00;
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP			XOS<0 YOS<0x0													;                                      
			NOP			XC<0 YC<0x8000	Z<0												;                                      
			NOP		 		      		    				 		TP<0x0 TP2<0		;                                                 
			NOP			W R       			 X<0x000000 Y<0x000000  TP<0x0 TP2<0	 	; //1                                                
			NOP			W R       			 X<0x000001 Y<0x000000 	TP<0x0 TP2<0	 	; //2                                                
			NOP			W R       			 X<0x000000 Y<0x000001 	TP<0x0 TP2<0	 	; //3                                                
			NOP			W R       			 X<0x000001 Y<0x000001 	TP<0x0 TP2<0	 	; //4                                                
			NOP			W R       			 X<0x000000 Y<0x000000  TP<0x0 TP2<0	 	; //1                                                
			NOP			W R       			 X<0x000001 Y<0x000000 	TP<0x0 TP2<0	 	; //2                                                
			NOP			W R       			 X<0x000000 Y<0x000001 	TP<0x0 TP2<0	 	; //3                                                
			NOP			W R       			 X<0x000001 Y<0x000001 	TP<0x0 TP2<0	 	; //4    		
//			NOP			W R       			 X<0x000010 Y<0x000010 	TP<0x0 TP2<0	 	; //5                                               
// 			NOP			W R       			 X<0x000020 Y<0x000020 	TP<0x0 TP2<0	 	; //6                                                
// 			NOP			W R       			 X<0x000040 Y<0x000040 	TP<0x0 TP2<0	 	; //7 //read ffff                                               
// 			NOP			W R       			 X<0x000080 Y<0x000080 	TP<0x0 TP2<0	 	; //8                                                
// 			NOP			W R       			 X<0x000100 Y<0x000100 	TP<0x0 TP2<0	 	; //9                                                
// 			NOP			W R       			 X<0x000200 Y<0x000200 	TP<0x0 TP2<0	 	; //10                                                
// 			NOP			W R       			 X<0x000400 Y<0x000400 	TP<0x0 TP2<0	 	; //11                                                
// 			NOP			W R       			 X<0x000800 Y<0x000800 	TP<0x0 TP2<0	 	; //12                                                
// 			NOP			W R       			 X<0x001000 Y<0x001000 	TP<0x0 TP2<0	 	; //13 //read ffff                                               
// 			NOP			W R       			 X<0x002000 Y<0x002000 	TP<0x0 TP2<0	 	; //14                                                
// 			NOP			W R       			 X<0x004000 Y<0x004000 	TP<0x0 TP2<0	 	; //15                                                
// 			NOP			W R       			 X<0x008000 Y<0x008000 	TP<0x0 TP2<0	 	; //16                                                
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
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

			START XY_SCR_01;
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
	
			NOP			XOS<0 YOS<0x0													;                                      
			NOP			XC<0 YC<0x8000	Z<0												;                                      
			NOP		 		      		    				 		  					;                                                 
		
			NOP			W R       			 X<0x000000  Y<0x000000    					;                                                 
			NOP			W R       			 X<0x000000  Y<0x000001    					;                                                 
			NOP			W R       			 X<0x000001  Y<0x000000    					;                                                 
			NOP			W R       			 X<0x000001  Y<0x000001    					;                                                 
			NOP			W R       			 X<0x000000  Y<0x000000  					;                                                 
			NOP			W R       			 X<0x000000  Y<0x000002  					;                                                 
			NOP			W R       			 X<0x000002  Y<0x000000  					;                                                 
			NOP			W R       			 X<0x000002  Y<0x000002  					;                                                 
			NOP			W R       			 X<0x000000  Y<0x000000  					;                                                 
			NOP			W R       			 X<0x000000  Y<0x000004  					;                                                 
			NOP			W R       			 X<0x000004  Y<0x000000  					;                                                 
			NOP			W R       			 X<0x000004  Y<0x000004  					;                                                 
			NOP			W R       			 X<0x000000  Y<0x000000  					;                                                 
			NOP			W R       			 X<0x000000  Y<0x000008  					;                                                 
			NOP			W R       			 X<0x000008  Y<0x000000  					;                                                 
			NOP			W R       			 X<0x000008  Y<0x000008  					;                                                 
		
                                          
			NOP																			;                                     
			NOP																			;                                     
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
			START XY_SCR_02;
                                          
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP			XOS<0 YOS<0x0													;                                      
			NOP			XC<0 YC<0x8000	Z<0												;                                      
			NOP		 		      		    				 		  					;                                                 
			NOP			W R       			 X<0x000000 Y<0x000000   					;                                                 
			NOP			W R       			 X<0x000000 Y<0x000010   					;                                                 
			NOP			W R       			 X<0x000010 Y<0x000000   					;                                                 
			NOP			W R       			 X<0x000010 Y<0x000010   					;                                                 
			NOP			W R       			 X<0x000000 Y<0x000000 						;                                                 
			NOP			W R       			 X<0x000000 Y<0x000020 						;                                                 
			NOP			W R       			 X<0x000020 Y<0x000000 						;                                                 
			NOP			W R       			 X<0x000020 Y<0x000020 						;                                                 
			NOP			W R       			 X<0x000000 Y<0x000000 						;                                                 
			NOP			W R       			 X<0x000000 Y<0x000040 						;                                                 
			NOP			W R       			 X<0x000040 Y<0x000000 						;                                                 
			NOP			W R       			 X<0x000040 Y<0x000040 						;                                                 
			NOP			W R       			 X<0x000000 Y<0x000000 						;                                                 
			NOP			W R       			 X<0x000000 Y<0x000080 						;                                                 
			NOP			W R       			 X<0x000080 Y<0x000000 						;                                                 
			NOP			W R       			 X<0x000080 Y<0x000080 						;  

			NOP																			;                                     
			NOP																			;                                     
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

			START XY_SCR_03;
                                          
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP			XOS<0 YOS<0x0													;                                      
			NOP			XC<0 YC<0x8000	Z<0												;                                      
			NOP		 		      		    				 		  					;                                                 
			NOP			W R       			 X<0x000000 Y<0x000000   					;                                                 
			NOP			W R       			 X<0x000000 Y<0x000100   					;                                                 
			NOP			W R       			 X<0x000100 Y<0x000000   					;                                                 
			NOP			W R       			 X<0x000100 Y<0x000100   					;                                                 
			NOP			W R       			 X<0x000000 Y<0x000000 						;                                                 
			NOP			W R       			 X<0x000000 Y<0x000200 						;                                                 
			NOP			W R       			 X<0x000200 Y<0x000000 						;                                                 
			NOP			W R       			 X<0x000200 Y<0x000200 						;                                                 
			NOP			W R       			 X<0x000000 Y<0x000000 						;                                                 
			NOP			W R       			 X<0x000000 Y<0x000400 						;                                                 
			NOP			W R       			 X<0x000400 Y<0x000000 						;                                                 
			NOP			W R       			 X<0x000400 Y<0x000400 						;                                                 
			NOP			W R       			 X<0x000000 Y<0x000000 						;                                                 
			NOP			W R       			 X<0x000000 Y<0x000800 						;                                                 
			NOP			W R       			 X<0x000800 Y<0x000000 						;                                                 
			NOP			W R       			 X<0x000800 Y<0x000800 						;        

			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP												   							;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
    		STPS																		;
	END			

			START XY_SCR_04;
                                          
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP			XOS<0 YOS<0x0													;                                      
			NOP			XC<0 YC<0x8000	Z<0												;                                      
			NOP		 		      		    				 		  					;                                                 
			NOP			W R       			 X<0x000000 Y<0x000000   					;                                                 
			NOP			W R       			 X<0x000000 Y<0x001000   					;                                                 
			NOP			W R       			 X<0x001000 Y<0x000000   					;                                                 
			NOP			W R       			 X<0x001000 Y<0x001000   					;                                                 
			NOP			W R       			 X<0x000000 Y<0x000000 						;                                                 
			NOP			W R       			 X<0x000000 Y<0x002000 						;                                                 
			NOP			W R       			 X<0x002000 Y<0x000000 						;                                                 
			NOP			W R       			 X<0x002000 Y<0x002000 						;                                                 
			NOP			W R       			 X<0x000000 Y<0x000000 						;                                                 
			NOP			W R       			 X<0x000000 Y<0x004000 						;                                                 
			NOP			W R       			 X<0x004000 Y<0x000000 						;                                                 
			NOP			W R       			 X<0x004000 Y<0x004000 						;                                                 
			NOP			W R       			 X<0x000000 Y<0x000000 						;                                                 
			NOP			W R       			 X<0x000000 Y<0x008000 						;                                                 
			NOP			W R       			 X<0x008000 Y<0x000000 						;                                                 
			NOP			W R       			 X<0x008000 Y<0x008000 						;        

			NOP																			;                                     
			NOP																			;                                     
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
     	
;%ENDC 

