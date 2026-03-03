%INSERT sample_def.h
 /*************************
    Resgister Setting
*************************/
REGISTER
			XMAX=0xffffff
			YMAX=0xffffff
			ZMAX=0xff
;--------------------------------------------------------------------------------------------------
            IDX0=0xFF 
            IDX1=0x3FE      
            IDX2=0x3FE     
;--------------------------------------------------------------------------------------------------
            D1A=0x000000   
            D1B=0x000000   
ENDREGISTER	  	
/*************************
    Contact Test Sample   
*************************/
START STEP_A;
		NOP         DUMMY       	  	   		 			 
		NOP         DUMMY 	  	   		 			  
		NOP         DUMMY 	  	   		 			  
		NOP         DUMMY 	      	   		 		  
		NOP         DUMMY                               SA<0  		   		 			  
;--------------------------------------------------------------------------------------------------
SA1:    NOP         DUMMY                               S<SA  	  	   		 			  
        NOP         DUMMY       TP<0x100  TP2<0x000		        	    		 			  
        NOP         DUMMY   W
        NOP         READ    R	  	   		 			  
        NOP         DUMMY 	  	   		 			  
        JNI1 SA1    DUMMY                               SA<SA+0x1     	  	   		 		
;--------------------------------------------------------------------------------------------------  
        NOP         DUMMY 	  	   		 			  
        NOP         DUMMY 	  	   		 			              
        STPS                        			 	                  
END 
/*************************
    Run Test Sample   
*************************/
START STEP_B;
		NOP         DUMMY       	  	   		 			 
		NOP         DUMMY 	  	   		 			  
		NOP         DUMMY 	  	   		 			  
		NOP         DUMMY 	      	   		 		  
		NOP         DUMMY                               SA<0  		   		 			  
;--------------------------------------------------------------------------------------------------
SA2:    NOP         DUMMY                               S<SA  	  	   		 			  
        NOP         DUMMY       TP<0x100  TP2<0x000		        	    		 			  
        NOP         DUMMY   W
        NOP         READ    R	  	   		 			  
        NOP         DUMMY 	  	   		 			  
        JNI1 SA2    DUMMY                               SA<SA+0x1     	  	   		 		
;--------------------------------------------------------------------------------------------------  
        NOP         DUMMY 	  	   		 			  
        NOP         DUMMY 	  	   		 			              
        STPS                        			 	                  
END