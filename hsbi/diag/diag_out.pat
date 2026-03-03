

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
            REPEAT 998                               ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP			      	 	              	  TP<0x0    TP2<0x0	   	                 ;
            NOP		 	      	 X<XC Y<YC            TP<0xf0f0 TP2<0x0f0f   XC<XB+1 YC<YB+1 ;  
            NOP		W	  R 	 X<XC Y<YC            TP<0x0002 TP2<0x000B   XC<XB+1 YC<YB+1 ;  
            NOP		W	  R 	 X<XC Y<YC            TP<0x0004 TP2<0x000C   XC<XB+1 YC<YB+1 ;  
            NOP		W	  R 	 X<XC Y<YC            TP<0x0008 TP2<0x000D	 XC<XB+1 YC<YB+1 ;  
            NOP		W	  R 	 X<XC Y<YC            TP<0x0010 TP2<0x00A0	 XC<XB+1 YC<YB+1 ;  
            NOP		W	  R 	 X<XC Y<YC            TP<0x0020 TP2<0x00B0	 XC<XB+1 YC<YB+1 ;  
            NOP		W	  R 	 X<XC Y<YC            TP<0x0040 TP2<0x00C0	 XC<XB+1 YC<YB+1 ;  
            NOP		W	  R 	 X<XC Y<YC            TP<0x0080 TP2<0x00D0	 XC<XB+1 YC<YB+1 ;  
            NOP		W	  R 	 X<XC Y<YC            TP<0x0100 TP2<0x0A00	 XC<XB+1 YC<YB+1 ;  
            NOP		W	  R 	 X<XC Y<YC            TP<0x0f0f TP2<0xf0f0	 XC<XB+1 YC<YB+1 ;  
            NOP		W	  R 	 X<XC Y<YC            TP<0x0    TP2<0x0						 ;  
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            REPEAT 998
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			
 
			STPS									 ;
END

        START SHMOO_NOP_01;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            REPEAT 998                               ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP		         	 	              	      TP<0xFFFF TP2<0xAAAA
            NOP		W 	      	     X<XC Y<YC            TP<0x1234 TP2<0x4567 	   	                 ;
            NOP		W	R 	         X<XC Y<YC            TP<0xCCCC TP2<0xDDDD         XC<XB+1 YC<YB+1     ;  
            NOP     W   R             			     ;    TP<0x0    TP2<0x0					       ;  
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            REPEAT 998
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
            NOP			      	 X<XC Y<YC            TP<0x0000 TP2<0x0000				;  
            NOP		W        	 X<XC Y<YC            TP<0xFFFF TP2<0x0000  			;  
            NOP		W	  R 	 X<XC Y<YC            TP<0x0000 TP2<0x0000    		  	;  
            NOP		W			 X<XC Y<YC             									;  
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