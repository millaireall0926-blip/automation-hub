

;%IFE A .AND. B .AND.((C.OR.D).AND.E)

%INSERT Header/define.h
  
 REGISTER
			XMAX=0xffffff;
			YMAX=0xffffff;
			ZMAX=0xffff;
			XOS=0x0;
			YOS=0x0;	
ENDREGISTER		

			START XY_CYPN_00;
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     

			NOP			XOS<0 YOS<0x0														;                                      
			NOP			XC<0 YC<0x8000	Z<0													;                                      
			NOP		 		      		    				 		   						;                                                 
			NOP			W R       			 X<0x000001	Y<0x001000       	   CYP0 		;                                                 
			NOP			W R       			 X<0x000002 Y<0x002000	     	   CYP1   		;                                                 
			NOP			W R       			 X<0x000004	Y<0x004000		       CYP2 		;                                                 
			NOP			W R       			 X<0x000008	Y<0x008000		       CYP3 		;                                                 
			NOP			W R       			 X<0x000010	Y<0x000001		       CYP4 		;                                                 
 			NOP			W R       			 X<0x000020	Y<0x000002		       CYP5    		;                                                 
 			NOP			W R       			 X<0x000040	Y<0x000004		       CYP6 		;                                                 
 			NOP			W R       			 X<0x000080	Y<0x000008		       CYP7 		;                                                 
 			NOP			W R       			 X<0x000100	Y<0x000010		       CYP8 		;                                                 
 			NOP			W R       			 X<0x000200	Y<0x000020		       CYP9 		;                                                 
 			NOP			W R       			 X<0x000400	Y<0x000040		       CYP10 		;                                                 
 			NOP			W R       			 X<0x000800	Y<0x000080		       CYP11 		;                                                 
 			NOP			W R       			 X<0x001000	Y<0x000100		       CYP12 		;                                                 
 			NOP			W R       			 X<0x002000	Y<0x000200		       CYP13 		;

 			NOP			W R       			 X<0x004000	Y<0x000400		       CYP14 		;                                                 
 
 			NOP			W R       			 X<0x008000	Y<0x000800		       CYP15		;                                                 
			NOP			W R       			 X<0x010000	Y<0x100000		       CYP8 		;                                                 
			NOP			W R       			 X<0x020000 Y<0x200000		       CYP9   		;                                                 
			NOP			W R       			 X<0x040000 Y<0x400000		       CYP10 		;                                                 
			NOP			W R       			 X<0x080000 Y<0x800000		       CYP11		;                                                 
			NOP			W R       			 X<0x100000 Y<0x010000	Z<0x1      CYP12 		;                                                 
 			NOP			W R       			 X<0x200000 Y<0x020000	Z<0x2  	   CYP13    	;                                                 
 			NOP			W R       			 X<0x400000 Y<0x040000		       CYP14 		;                                                 
 			NOP			W R       			 X<0x800000 Y<0x080000		       CYP15 		;                                                 

			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
    		STPS																			;
	END	


			START XY_CYPN_02;
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     

			NOP			XOS<0 YOS<0x0														;                                      
			NOP			XC<0 YC<0x8000														;                                      
			NOP		 		      		    TP<0x00001	TP2<0x01000				 			;                                                 
			NOP			W R       			TP<0x00002	TP2<0x02000 		   CYP0 		;                                                 
			NOP			W R       			TP<0x00004	TP2<0x04000 		   CYP1   		;                                                 
			NOP			W R       			TP<0x00008	TP2<0x08000 		   CYP2 		;                                                 
			NOP			W R       			TP<0x00010	TP2<0x00001 		   CYP3 		;                                                 
			NOP			W R       			TP<0x00020	TP2<0x00002 		   CYP4 		;                                                 
 			NOP			W R       			TP<0x00040	TP2<0x00004 		   CYP5    		;                                                 
 			NOP			W R       			TP<0x00080	TP2<0x00008 		   CYP6 		;                                                 
 			NOP			W R       			TP<0x00100	TP2<0x00010 		   CYP7 		;                                                 
 			NOP			W R       			TP<0x00200	TP2<0x00020 		   CYP8 		;                                                 
 			NOP			W R       			TP<0x00400	TP2<0x00040 		   CYP9 		;                                                 
 			NOP			W R       			TP<0x00800	TP2<0x00080 		   CYP10 		;                                                 
 			NOP			W R       			TP<0x01000	TP2<0x00100 		   CYP11 		;                                                 
 			NOP			W R       			TP<0x02000	TP2<0x00200 		   CYP12 		;                                                 
 			NOP			W R       			TP<0x04000	TP2<0x00400 		   CYP13 		;                                                 

 			NOP			W R       			TP<0x08000	TP2<0x00800 		   CYP14 		;                                                 

 			NOP			W R       			TP<0x10000	TP2<0x20000 		   CYP15 		;                                                 
 			NOP			W R       			TP<0x20000	TP2<0x10000 		   CYP3 		;                                                 
 			NOP			W R       			 		   					       CYP4			;                                                 

			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
    		STPS																			;
	END		

			START XY_CYPN_04;
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     

			NOP			XOS<0 YOS<0x0													;                                      
			NOP			XC<3 YC<0x0														;                                      
			NOP		 		      		     				 							;                                                 
			NOP			W R   X<XC    			C0 			C16				   CYP0 		;                                                 
			NOP			W R        			C1 			C17				   CYP1	  		;                                                 
			NOP			W R       			C2 			C18				   CYP2			;                                                 
			NOP			W R       			C3 			C19				   CYP3			;                                                 
			NOP			W R       			C4 			C20				   CYP4			;                                                 
 			NOP			W R       			C5 			C21				   CYP5	   		;                                                 
 			NOP			W R      			C6 			C22				   CYP6			;                                                 
 			NOP			W R       			C7 			C23				   CYP7			;                                                 
 			NOP			W R       			C8 			C24				   CYP8			;                                                 
 			NOP			W R       			C9 			C25				   CYP9			;                                                 
 			NOP			W R       			C10			C26				   CYP10	 	;                                                 
 			NOP			W R       			C11			C27				   CYP11	 	;                                                 
 			NOP			W R       			C12			C28				   CYP12	 	;                                                 
 			NOP			W R       			C13			C29				   CYP13	 	;                                                 
 			NOP			W R       			C14			C30				   CYP14	 	;                                                 
 			NOP			W R       			C15			C31				   CYP15		;                                                 

			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
    		STPS																		;
	END		


			START Z_CYPN_00;
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     

			NOP			XOS<0 YOS<0x0														;                                      
			NOP			XC<0 YC<0x8000	Z<0													;                                      
			NOP		 		      		    				 		   						;                                                 
			NOP			W R       			 Z<0x0001	       	       CYP0 				;                                                 
			NOP			W R       			 Z<0x0002   	     	   CYP1   				;                                                 
			NOP			W R       			 Z<0x0004			       CYP2 				;                                                 
			NOP			W R       			 Z<0x0008			       CYP3 				;                                                 
			NOP			W R       			 Z<0x0010			       CYP4 				;                                                 
 			NOP			W R       			 Z<0x0020			       CYP5    				;                                                 
 			NOP			W R       			 Z<0x0040			       CYP6 				;                                                 
 			NOP			W R       			 Z<0x0080			       CYP7 				;                                                 
 			NOP			W R       			 Z<0x0100			       CYP8 				;                                                 
 			NOP			W R       			 Z<0x0200			       CYP9 				;                                                 
 			NOP			W R       			 Z<0x0400			       CYP10 				;                                                 
 			NOP			W R       			 Z<0x0800			       CYP11 				;                                                 
 			NOP			W R       			 Z<0x1000			       CYP12 				;                                                 
 			NOP			W R       			 Z<0x2000			       CYP13 				;
 			NOP			W R       			 Z<0x4000			       CYP14 				;                                                 
 			NOP			W R       			 Z<0x8000			       CYP15 				;

			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
    		STPS																			;
	END	

			START Z_CYPN_01;
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     

			NOP			XOS<0 YOS<0x0														;                                      
			NOP			XC<0 YC<0x8000	Z<0													;                                      
			NOP		 		      		    				 		   						;                                                 
			NOP			W R       			 Z<0x0001	       	       CYP0 				;                                                 
			NOP			W R       			 Z<0x0002   	     	   CYP1   				;                                                 
			NOP			W R       			 Z<0x0004			       CYP2 				;                                                 
			NOP			W R       			 Z<0x0008			       CYP3 				;                                                 
			NOP			W R       			 Z<0x0010			       CYP4 				;                                                 
 			NOP			W R       			 Z<0x0020			       CYP5    				;                                                 
 			NOP			W R       			 Z<0x0040			       CYP6 				;                                                 
 			NOP			W R       			 Z<0x0080			       CYP7 				;                                                 
 			NOP			W R       			 Z<0x0100			       CYP8 				;                                                 
 			NOP			W R       			 Z<0x0200			       CYP9 				;                                                 
 			NOP			W R       			 Z<0x0400			       CYP10 				;                                                 
 			NOP			W R       			 Z<0x0800			       CYP11 				;                                                 
 			NOP			W R       			 Z<0x1000			       CYP12 				;                                                 
 			NOP			W R       			 Z<0x2000			       CYP13 				;
 			NOP			W R       			 Z<0x4000			       CYP14 				;                                                 
 			NOP			W R       			 Z<0x8000			       CYP15 				;

			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
			NOP																				;                                     
    		STPS																			;
	END	

;%ENDC 