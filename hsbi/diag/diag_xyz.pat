

;%IFE A .AND. B .AND.((C.OR.D).AND.E)

%INSERT Header/define.h
  
 REGISTER
			XMAX=0xffffff;
			YMAX=0xffffff;
			ZMAX=0xffff;
			XOS=0x0;
			YOS=0x0;	
ENDREGISTER		

			START XY_OUT_00;
			NOP																			;                                      
			NOP		 		      		    											;                                                 
			NOP																			;                                      
			NOP		 		      		    											;                                                 
			NOP																			;                                      
			NOP		 		      		    											;                                                 
			NOP																			;                                      
			NOP		 		      		    											;                                                 
			NOP																			;                                      
			NOP		 		      		    											;                                                 
			NOP																			;                                      
			NOP		 		      		    											;                                                 
			NOP																			;                                      
			NOP		 		      		    											;                                                 
			NOP																			;                                      
			NOP		 		      		    											;                                                 
			NOP																			;                                      
			NOP		 		      		    											;                                                 
			NOP																			;                                      
			NOP		 		      		    											;                                                 
			NOP																			;                                      
			NOP		 		      		    											;                                                 
			NOP																			;                                      
			NOP		 		      		    											;                                                 
			NOP																			;                                      
			NOP		 		      		    											;                                                 
			NOP																			;                                      
			NOP		 		      		    											;                                                 
			NOP																			;                                      
			NOP		 		      		    											;                                                 

			NOP		XC<0x0001		YC<0x8001											;                                      
			NOP		XS<0x0002		YS<0x8002											;                                      
			NOP		XK<0x0003		YK<0x8003											;                                      
			NOP		XA<0x0004		YA<0x8004											;                                      
			NOP		XD<0x0005		YD<0x8005											;                                      
			NOP		XE<0x0006		YE<0x8006											;                                      
			NOP		XF<0x0007		YF<0x8007											;                                      
			NOP		XB<0x0008		YB<0x8008											;                                      
			NOP		XT<0x0011 		YT<0x8011											;                                    
			NOP		XOS<0x0			YOS<0x0												;                                      
			NOP		ZH<0x1212															;                                      
			NOP		Z<ZH																;                                      
			NOP		RF<0																;                                      
			NOP		RF<RF+1																;                                      
			NOP		RF<RF+1																;                                      
			NOP																			;                                      
	    	NOP																			;                                      
			NOP		 		      		    											;                                                 
			NOP																			;                                      
			NOP		 		      		    											;                                                 
			NOP																			;                                      
			NOP		 		      		    											;                                                 
			NOP																			;                                      
			NOP		 		      		    											;                                                 
			NOP																			;                                      
			NOP		 		      		    											;                                                 
			NOP																			;                                      
			NOP		 		      		    											;                                                 
			NOP		 		      		    											;                                                 
			NOP			W R       X<XC		Y<YC				    					;                                                 
			NOP			W R       X<XS		Y<YS				    					;                                                 
			NOP			W R       X<XK		Y<YK				    					;                                                 
			NOP			W R       X<XA		Y<YA				    					;                                                 
			NOP			W R       X<XD		Y<YD				    					;                                                 
 			NOP			W R       X<XE		Y<YE				    					;                                                 
 			NOP			W R       X<XF		Y<YF				    					;                                                 
 			NOP			W R       X<XB		Y<YB				    					;                                                 
 			NOP			W R       X<YC		Y<XC				    					;                                                 
 			NOP			W R       X<YS		Y<XS				    					;                                                 
 			NOP			W R       X<YK		Y<XK				    					;                                                 
 			NOP			W R       X<YA		Y<XA				    					;                                                 
 			NOP			W R       X<YD		Y<XD				    					;                                                 
 			NOP			W R       X<YE		Y<XE				    					;                                                 
 			NOP			W R       X<YF		Y<XF				    					;                                                 
 			NOP			W R       X<YB		Y<XB				    					;                                                 
			NOP																			;                                      
			NOP		 		      		    											;                                                 
			NOP																			;                                      
			NOP		 		      		    											;                                                 
			NOP																			;                                      
			NOP		 		      		    											;                                                 
			NOP																			;                                      
			NOP		 		      		    											;                                                 
			NOP																			;                                      
			NOP		 		      		    											;                                                 
			NOP																			;                                      
			NOP		 		      		    											;                                                 
			NOP																			;                                      
			NOP		 		      		    											;                                                 
			NOP																			;                                      
			NOP		 		      		    											;                                                 
			NOP																			;                                      
			NOP		 		      		    											;                                                 
			NOP																			;                                      
			NOP		 		      		    											;                                                 
			NOP																			;                                      
			NOP		 		      		    											;                                                 
			NOP																			;                                      
			NOP		 		      		    											;                                                 
			NOP																			;                                      
			NOP		 		      		    											;                                                 
			NOP																			;                                      
			NOP		 		      		    											;                                                 
			NOP																			;                                      
			NOP		 		      		    											;                                                 
				STPS																		;
	END	

		START XY_OUT_01;
			%INSERT DUMMY.asc
			NOP		XC<0x0001		YC<0x8001											;                                      
			NOP		XS<0x0002		YS<0x8002											;                                      
			NOP		XK<0x0003		YK<0x8003											;                                      
			NOP		XA<0x0004		YA<0x8004											;                                      
			NOP		XD<0x0005		YD<0x8005											;                                      
			NOP		XE<0x0006		YE<0x8006											;                                      
			NOP		XF<0x0007		YF<0x8007											;                                      
			NOP		XB<0x0008		YB<0x8008											;                                      
			NOP		XT<0x0011 		YT<0x8011											;                                    
			NOP		XOS<0x0			YOS<0x0												;                                      
			NOP		ZH<0x1212															;                                      
			NOP		Z<ZH																;                                      
			NOP		RF<0																;                                      
			NOP		RF<RF+1																;                                      
			NOP		RF<RF+1																;                                      
			NOP																			;                                      
			NOP		 		      		    											;                                                 
			NOP			W R       X<XE		Y<YE				    					;                                                 
 			NOP			W R       X<XF		Y<YF				    					;                                                 
 			NOP			W R       X<XB		Y<YB				    					;                                                 
 			NOP			W R       X<YC		Y<XC				    					;                                                 
 			NOP			W R       X<YS		Y<XS				    					;                                                 
 			NOP			W R       X<YK		Y<XK				    					;                                                 
 			NOP			W R       X<YA		Y<XA				    					;                                                 
 			NOP			W R       X<YD		Y<XD				    					;                                                 
 			NOP			W R       X<YE		Y<XE				    					;                                                 
 			NOP			W R       X<YF		Y<XF				    					;                                                 
 			NOP			W R       X<YB		Y<XB				    					;                                                 
 			NOP			W R       X<XT      Y<YT       					    			;                                         
 			NOP			W R       X<XOS     Y<YOS	  					  				;                                               
 			NOP			W R       X<Z		Y<Z						    				;                                                 
 			NOP			W R       X<0xAAAA	Y<0xBBBB									;                                                 
 			NOP			W R		  X<RF		 											;                                                 
			%INSERT DUMMY.asc
    		STPS																		;
	END	


			START XB_ALU_XT_00;
			%INSERT DUMMY.asc
			NOP																			;                                      
			NOP				XOS<0xff		YOS<0xff									;                                                 
			NOP												XT<0x0000	YT<0x0000 								;                                                 
			NOP			W R			X<XT  	Y<YT		  	XT<0x0001	YT<0x000A	 		;                                                 
			NOP			W R			X<XT	Y<YT		  	XT<0x0002	YT<0x000B	 		;                                                 
			NOP			W R			X<XT	Y<YT		  	XT<0x0001	YT<0x000A	 		;                                                 
			NOP			W R			X<XT	Y<YT		  	XT<0x0002	YT<0x000B	 		;                                                 
			NOP			W R			X<XT	Y<YT		  	XT<0x0001	YT<0x000A	 		;                                                 
			NOP			W R			X<XT	Y<YT		  	XT<0x0002	YT<0x000B	 		;                                                 
			NOP									    		XT<0x0000	YT<0x0000			;                                                 
			NOP			W R			X<XT    Y<YT		  	XT<0x0001	YT<0x000A	 		;                                                 
			NOP			W R			X<XT	Y<YT		  	XT<0x0002	YT<0x000B	 		;                                                 
			NOP			W R			X<XT	Y<YT		  	XT<0x0004	YT<0x000C	 		;                                                 
			NOP			W R			X<XT	Y<YT		  	XT<0x0008	YT<0x000D	 		;                                                 
			NOP			W R			X<XT	Y<YT		  	XT<0x0010	YT<0x00A0	 		;                                                 
			NOP			W R			X<XT	Y<YT		  	XT<0x0020	YT<0x00B0	 		;                                                 
			NOP			W R			X<XT	Y<YT		  	XT<0x0040	YT<0x00C0	 		;                                                 
			NOP			W R			X<XT	Y<YT		  	XT<0x0080	YT<0x00D0			;                                                 
			NOP			W R			X<XT	Y<YT		  	XT<0x0100	YT<0x0A00			;                                                 
			NOP			W R			X<XT	Y<YT		  									;                                     
			%INSERT DUMMY.asc
    		STPS																		;
	END	

			START Z_ALU_Z_00;
			%INSERT DUMMY.asc
			NOP																			;                                                 
			NOP														 					;                                                 
			NOP			 											Z<0					;                                                 
			NOP			W R						X<Z 				Z</Z				;                                                 
			NOP			W R						X<Z 				Z<0					;                                                 
			NOP			W R						X<Z 				Z<Z+1				;                                                 
			NOP			W R						X<Z 				Z<Z+1				;                                                 
			NOP			W R						X<Z 				Z<Z-1				;                                                 
			NOP			W R						X<Z 				Z<Z+1				;                                                 
			NOP			W R						X<Z 				Z<Z*2				;                                                 
			NOP			W R						X<Z 				Z<Z/2				;                                                 
			NOP			W R						X<Z 				Z<Z					;                                                 
			NOP			W R						X<Z 				Z<Z/2				;                                                 
			NOP			W R						X<Z 				Z<Z-1				;                                                 
			NOP			W R						X<Z 				Z<Z					;                                                 
			NOP			W R						X<Z 				Z</Z				;                                                 
			NOP			W R						X<Z 				Z<0					;                                                 
			NOP			W R						X<Z 				Z</Z				;                                                 
			NOP			W R						X<Z 									;                                                 
			%INSERT DUMMY.asc
    		STPS																		;
	END	
	
			START Z_ALU_A_00;
			%INSERT DUMMY.asc

			NOP			ZH<0xAA 														;                                      
			NOP																		    ;  ZMAX<0xF                                               
			NOP																			;                                                 
			NOP														Z<Z					;                                                 
			NOP			W R						X<Z 				Z<ZH				;                                                 
			NOP			W R						X<Z 				Z<0xbb 				;                                                 
			NOP			W R						X<Z 				Z<ZMAX				;                                                 
			NOP			W R						X<Z 				Z<ZH				;                                                 
			NOP			W R						X<Z 				Z<Z+1				;                                                 
			NOP			W R						X<Z 				Z<Z+1				;                                                 
			NOP			W R						X<Z 				Z<Z+1				;                                                 
			NOP			W R						X<Z 				Z<Z+1				;                                                 
			NOP			W R						X<Z 				Z<Z+1				;                                                 
			NOP			W R						X<Z 				Z<Z+1				;                                                 
			NOP			W R						X<Z 				Z<Z+1				;                                                 
			NOP			W R						X<Z 				Z<Z+1				;                                                 
			NOP			W R						X<Z 				Z<Z+1				;                                                 
			NOP			W R						X<Z 									;                                                 
			NOP																			;                                     
			NOP									X<Z 				Z<Z+1    			;                                                 
			NOP			W R						X<Z 				Z<Z+1				;                                                 
			NOP			W R						X<Z 									;                                                 
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			%INSERT DUMMY.asc
			STPS																		;
	END

		START Z_ALU_A_01;
			%INSERT DUMMY.asc

			NOP			ZH<0x1234 														;                                      
			NOP																		    ;  ZMAX<0xFFFF                                               
			NOP																			;                                                 
			NOP														Z<Z					;                                                 
			NOP			W R						X<Z 				Z<ZH				;                                                 
			NOP			W R						X<Z 				Z<0x3456			;                                                 
			NOP			W R						X<Z 				Z<ZMAX				;                                                 
			NOP			W R						X<Z 				Z<ZH				;                                                 
			NOP			W R						X<Z 				Z<Z+1				;                                                 
			NOP			W R						X<Z 				Z<Z+1				;                                                 
			NOP			W R						X<Z 				Z<Z+1				;                                                 
			NOP			W R						X<Z 				Z<Z+1				;                                                 
			NOP			W R						X<Z 				Z<Z+1				;                                                 
			NOP			W R						X<Z 				Z<Z+1				;                                                 
			NOP			W R						X<Z 				Z<Z+1				;                                                 
			NOP			W R						X<Z 				Z<Z+1				;                                                 
			NOP			W R						X<Z 				Z<Z+1				;                                                 
			NOP			W R						X<Z 									;                                                 
			NOP																			;                                     
			NOP									X<Z 				Z<Z+1    			;                                                 
			NOP			W R						X<Z 				Z<Z+1				;                                                 
			NOP			W R						X<Z 									;                                                 
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			NOP																			;                                     
			%INSERT DUMMY.asc
			STPS																		;
	END


			START Z_ALU_ZPA_00;
			%INSERT DUMMY.asc
			NOP			ZH<0x11															;                                                                  
			NOP																			;  ZMAX<0xFF                                    
			NOP																			;                                                 
			NOP																			;                                                 
			NOP														 					;                                                 
			NOP			 				 							Z<Z+ZH				;                                                 
			NOP			W R						X<Z 				Z<Z+0x0				;                                                 
			NOP			W R						X<Z 				Z<Z+0x1				;                                                 
			NOP			W R						X<Z 				Z<Z+0x2				;                                                 
			NOP			W R						X<Z 				Z<Z+0x4				;                                                 
			NOP			W R	 				 	X<Z 				Z<Z+ZH				;                                                 
			NOP			W R						X<Z 				Z<Z+ZMAX			;                                                 
			NOP			W R						X<Z 				Z<Z+0x0				;                                                 
			NOP			W R						X<Z 				Z<Z+0x1				;                                                 
			NOP			W R						X<Z 				Z<Z+0x2				;                                                 
			NOP			W R						X<Z 				Z<Z+0x4				;                                                 
			NOP			W R	 				 	X<Z 				Z<Z+ZH				;                                                 
			NOP			W R						X<Z 				Z<Z+ZMAX			;                                                 
			NOP			W R						X<Z 				Z<Z+0x0				;                                                 
			NOP			W R						X<Z 				Z<Z+0x1				;                                                 
			NOP			W R						X<Z 				Z<0    				;                                                 
			NOP			W R						X<Z 									;                                                 
			NOP																			;                                     
			NOP																			;                                     
			%INSERT DUMMY.asc
    		STPS																		;
	END			

			START Z_ALU_ZMA_00;
			%INSERT DUMMY.asc
			NOP			ZH<0x11															;	                                                                  
			NOP																			;  ZMAX<0xF                                    
			NOP																			;                                                 
			NOP																			;                                                 
			NOP														Z<0xFF 				;                                                 
			NOP		 	W R						X<Z					Z<Z-ZH				;                                                 
			NOP			W R						X<Z 				Z<Z-0x1				;                                                 
			NOP			W R						X<Z 				Z<Z-ZMAX			;                                                 
			NOP			W R						X<Z 				Z<0					;                                                 
			NOP																			;                                     
			NOP														Z<0xFF 				;                                                 
			NOP		 	W R						X<Z					Z<Z-ZH				;                                                 
			NOP			W R						X<Z 				Z<Z-0x1				;                                                 
			NOP			W R						X<Z 				Z<Z-ZMAX			;                                                 
			NOP			W R						X<Z 				Z<0					;                                                 
			NOP																			;                                     
			NOP																			;                                     
			NOP														Z<0xFF 				;                                                 
			NOP		 	W R						X<Z					Z<Z-ZH				;                                                 
			NOP			W R						X<Z 				Z<Z-0x1				;                                                 
			NOP			W R						X<Z 				Z<Z-ZMAX			;                                                 
			NOP			W R						X<Z 				Z<0					;                                                 
			NOP																			;                                     
			NOP																			;                                     
			NOP														Z<0xFF 				;                                                 
			NOP		 	W R						X<Z					Z<Z-ZH				;                                                 
			NOP			W R						X<Z 				Z<Z-0x1				;                                                 
			NOP			W R						X<Z 				Z<Z-ZMAX			;                                                 
			NOP			W R						X<Z 				Z<0					;                                                 
			%INSERT DUMMY.asc

    		STPS																		;
	END	
;%ENDC 

