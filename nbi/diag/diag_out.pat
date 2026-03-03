

;%IFE A .AND. B .AND.((C.OR.D).AND.E)

%INSERT Header/define.h
  
 REGISTER
			XMAX=0xffffff;
			YMAX=0xffffff;
			ZMAX=0xff;
			XOS=0x0;
			YOS=0x0;	
ENDREGISTER		

        START SHMOO_NOP_00;
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
            NOP			      	 	              	  TP<0x0    TP2<0x0	   	                 ;
            NOP		 	      	 X<XC Y<YC            TP<0xf0f0 TP2<0x0f0f   XB<XB+1 YC<YB+1 ;  
            NOP		C19	  C17 	 X<XC Y<YC            TP<0x0002 TP2<0x000B   XB<XB+1 YC<YB+1 ;  
            NOP		C19	  C17 	 X<XC Y<YC            TP<0x0004 TP2<0x000C   XB<XB+1 YC<YB+1 ;  
            NOP		C19	  C17 	 X<XC Y<YC            TP<0x0008 TP2<0x000D	 XB<XB+1 YC<YB+1 ;  
            NOP		C19	  C17 	 X<XC Y<YC            TP<0x0010 TP2<0x00A0	 XB<XB+1 YC<YB+1 ;  
            NOP		C19	  C17 	 X<XC Y<YC            TP<0x0020 TP2<0x00B0	 XB<XB+1 YC<YB+1 ;  
            NOP		C19	  C17 	 X<XC Y<YC            TP<0x0040 TP2<0x00C0	 XB<XB+1 YC<YB+1 ;  
            NOP		C19	  C17 	 X<XC Y<YC            TP<0x0080 TP2<0x00D0	 XB<XB+1 YC<YB+1 ;  
            NOP		C19	  C17 	 X<XC Y<YC            TP<0x0100 TP2<0x0A00	 XB<XB+1 YC<YB+1 ;  
            NOP		C19	  C17 	 X<XC Y<YC            TP<0x0f0f TP2<0xf0f0	 XB<XB+1 YC<YB+1 ;  
            NOP		C19	  C17 	 X<XC Y<YC            TP<0x0    TP2<0x0						 ;  
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			
 
			STPS									 ;
END


      START SHMOO_TDR_00;
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
            NOP		     	     	           	       	  								;
 LL:        NOP			      	                 	        	        				;
            NOP			      	 	              	   	 	 	   			 			;
            NOP			      	 	              	  	   		 			 			;
            NOP			      	 X<XC Y<YC           									;  
            NOP		        	 X<XC Y<YC            TP<0xffff TP2<0x0000  			;  
            NOP		C19	  C17 	 X<XC Y<YC            TP<0x0000 TP2<0x0000    		  	;  
            NOP					 X<XC Y<YC             									;  
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
            NOP          	  	   		 			
 
			STPS									 ;
END


      START SHMOO_TDR_01;
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
            NOP		     	     	           	       	  								;
 LL:        NOP			      	                 	        	        				;
            NOP			      	 	              	   	 	 	   			 			;
            NOP			      	 	              	  	   		 			 			;
            NOP			      	 X<XC Y<YC            TP<0xFFFF TP2<0xFFFF       									;  
            NOP		C19       	 X<XC Y<YC            TP<0x0000 TP2<0xFFFF  			;  
            NOP		C19	  C17 	 X<XC Y<YC            TP<0xFFFF TP2<0xFFFF    		  	;  
            NOP					 X<XC Y<YC             									;  
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
            NOP          	  	   		 			
 
			STPS									 ;
END



	
;%ENDC 