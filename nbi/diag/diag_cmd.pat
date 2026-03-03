

;%IFE A .AND. B .AND.((C.OR.D).AND.E)

%INSERT Header/define.h
  
 REGISTER
			XMAX=0xffffff;
			YMAX=0xffffff;
			ZMAX=0xffff;
			XOS=0x0;
			YOS=0x0;	
			S1A=0xA;
			ZH=0xAAAA;
ENDREGISTER		

			START XY_CMD_00;
			NOP					 														;                                                 
			NOP					 														;                                                 
			NOP					 														;                                                 
			NOP					 														;                                                 
			NOP					 														;                                                 
			NOP					 														;                                                 
			NOP					 														;                                                 
			NOP					 														;                                                 
			NOP					 														;                                                 
			NOP					 														;                                                 

			NOP		XC<0x0001		YC<0x8001											;                                      
			NOP		XS<0x0002		YS<0x8002											;                                      
			NOP		XK<0x0003		YK<0x8003											;                                      
			NOP		XA<0x0004		YA<0x8004											;                                      
			NOP		XD<0x0005		YD<0x8005											;                                      
			NOP		XE<0x0006		YE<0x8006											;                                      
			NOP		XF<0x0007		YF<0x8007											;                                      
			NOP		XB<0x0008		YB<0x8008											;                                      
			NOP		XT<0x0011		YT<0x8011											;                                      
			NOP		XOS<0x0		YOS<0x0													;                                      
			NOP		ZH<0x12																;                                      
			NOP		Z<ZH																;                                      
			NOP		RF<0																;                                      
			NOP		RF<RF+1																;                                      
			NOP		RF<RF+1																;                                      
			NOP																			;                                      
			NOP		 		      		    											;                                                 
			NOP			W R       X<XC		Y<YC	/X			    					;                                                 
			NOP			W R       X<XS		Y<YS    /Y				    				;                                                 
			NOP			W R       X<XK		Y<YK				    					;                                                 
			NOP			W R       X<XA		Y<YA				    					;                                                 
			NOP			W R       X<XD		Y<YD				    					;                                                 
 			NOP			W R       X<XE		Y<YE	/X  /Y			    				;                                                 
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
 			NOP			W R       X<XT		Y<YT  				    					;                                               
 			NOP			W R       X<XOS		Y<YOS   	 	  							;                                              
 			NOP			W R       X<Z		Y<Z						    				;                                                 
 			NOP			W R       X<0xAAAA	Y<0xBBBB	/X 								;                                                 
 			NOP			W R		  X<RF		 			/X								;                                                 

			NOP					 														;                                                 
			NOP					 														;                                                 
			NOP					 														;                                                 
			NOP					 														;                                                 
			NOP					 														;                                                 
			NOP					 														;                                                 
			NOP					 														;                                                 
			NOP					 														;                                                 
			NOP					 														;                                                 
			NOP					 														;                                                 
    		STPS																		;
	END	
	
			START TP_CMD_DBDB2_00;
			NOP					 														;                                                 
			NOP					 														;                                                 
			NOP					 														;                                                 
			NOP					 														;                                                 
			NOP					 														;                                                 
			NOP					 														;                                                 
			NOP					 														;                                                 
			NOP					 														;                                                 
			NOP					 														;                                                 
			NOP					 														;                                                 

			NOP						TP<0x0 TP2<0x8000									;                                      
			NOP																			;                                      
			NOP																			;                                      
			NOP																			;                                      
			NOP																			;                                      
			NOP																			;                                      
			NOP																			;                                      
			NOP																			;                                                 
			NOP																			;                                                 
			NOP						TP<TP+1		TP2<TP2-1								;                                                 
			NOP			W R			TP<TP+1		TP2<TP2-1								;                                                 
			NOP			W R	/D		TPH1<TP		TPH2<TP2 								;                                                
			NOP			W R		/D2	TP<TP+1		TP2<TP2-1								;                                                 
			NOP			W R		/D2	TP<TP+1		TP2<TP2-1								;                                                 
			NOP			W R	/D	/D2	TP<TPH1  	TP2<TPH2								;                                                 
			NOP			W R	/D		TP<TP+1     TP2<TP2-1                               ;
			NOP			W R			 													;                                                 
			NOP																			;                                     
			NOP																			;                                     
			NOP						TP<0x0 TP2<0x8000									;                                      
			NOP						TP<TP+1		TP2<TP2-1								;                                                 
			NOP			W R			TP<TP+1		TP2<TP2-1								;                                                 
			NOP			W R	/D		TPH1<TP		TPH2<TP2 								;                                                
			NOP			W R		/D2	TP<TP+1		TP2<TP2-1								;                                                 
			NOP			W R		/D2	TP<TP+1		TP2<TP2-1								;                                                 
			NOP			W R	/D	/D2	TP<TPH1  	TP2<TPH2								;                                                 
			NOP			W R	/D		TP<TP+1     TP2<TP2-1                               ;
			NOP			W R			 													;                                                 
			NOP						TP<0x0000   TP2<0x0000								;                                     
			NOP			W R	 /D		 													;                                                 
			NOP																			;                                     
			NOP			W R		/D2	 													;                                                 

			NOP					 														;                                                 
			NOP					 														;                                                 
			NOP					 														;                                                 
			NOP					 														;                                                 
			NOP					 														;                                                 
			NOP					 														;                                                 
			NOP					 														;                                                 
			NOP					 														;                                                 
			NOP					 														;                                                 
			NOP					 														;                                                 
    		STPS																		;
	END
	
;%ENDC 