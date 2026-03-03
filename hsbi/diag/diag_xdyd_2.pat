%INSERT Header/define.h
  
 REGISTER
			XMAX=0xffffff;
			YMAX=0xffffff;
			ZMAX=0xff;
			XOS=0x0;
			YOS=0x0;	
ENDREGISTER		

					
			START XD_ALU_AORB_00;
			NOP          	  	   		 			 							 
            NOP          	  	   		 			 							 
            NOP          	  	   		 			 							 
            NOP          	  	   		 			 							 
            NOP          	  	   		 			 							 
            NOP          	  	   		 			 							 
            NOP          	  	   		 			 							 
            NOP          	  	   		 			 							 
            NOP          	  	   		 			 							 
			NOP          	  	   		 			 							                                                        
			NOP		   XC<0x0000	YC<0x8000									                                                                
			NOP		   XS<0x1000	YS<0x9000									                                                                
			NOP		   XK<0x2000	YK<0xA000									                                                                
			NOP		   XA<0x3000	YA<0xB000									                                                                
			NOP		   XD<0x4000	YD<0xC000									                                                                
			NOP		   XE<0x5000	YE<0xD000		 							                                                                
			NOP		   XF<0x6000	YF<0xE000									                                                                
			NOP		   XB<0x7000	YB<0xF000									                                                                
			NOP		  D1A<0x0000	D4B<0x1000									                                                                
			NOP		  D1B<0x0001												                                                        
			NOP		  D1C<0x0002												                                                        
			NOP		  D1D<0x0004												                                                        
			NOP		  D1E<0x0008												                                                        
			NOP		  D1F<0x0010			 									                                                        
			NOP		  D1G<0x0020												                                                        
			NOP		  D1H<0x0040												                                                        
			NOP		  D2A<0x0080	D3B<0x0800									                                                                
			NOP		  D2B<0x0100												                                                        
			NOP		  D2C<0x0200												                                                        
			NOP		  D2D<0x0400												                                                        
			NOP		   D3<D3B													                                                    
			NOP		   D4<D4B													                                                    
			NOP																	                                       
			NOP														XD<XC|D1A	YD<YC|D1A;                                                 
			NOP			W R						XDYD  				XD<XC|D1B	YD<YC|D1B; //Write : X 0x0000 Y 0x8000 Read : X 0x9FFE Y 0x8FFE                                                 
			NOP			W R						XDYD  				XD<XC|D1C	YD<YC|D1C; //Write : X 0x0001 Y 0x8000 Read : X 0x9FFC Y 0x8FFC                                                 
			NOP			W R						XDYD  				XD<XC|D1D	YD<YC|D1D; //Write : X 0x0003 Y 0x8000 Read : X 0x9FF8 Y 0x8FF8                                                 
			NOP			W R						XDYD  				XD<XC|D1E	YD<YC|D1E; //Write : X 0x0007 Y 0x8000 Read : X 0x9FF0 Y 0x8FF0                                                 
			NOP			W R						XDYD  				XD<XC|D1F	YD<YC|D1F; //Write : X 0x000F Y 0x8000 Read : X 0x9FE0 Y 0x8FE0                                                 
			NOP			W R						XDYD  				XD<XC|D1G	YD<YC|D1G; //Write : X 0x001F Y 0x8000 Read : X 0x9FC0 Y 0x8FC0                                                 
			NOP			W R						XDYD  				XD<XC|D1H	YD<YC|D1H; //Write : X 0x003F Y 0x8000 Read : X 0x9F80 Y 0x8F80                                                 
			NOP			W R						XDYD  				XD<XC|D2A	YD<YC|D2A; //Write : X 0x007F Y 0x8000 Read : X 0x9F00 Y 0x8F00                                                 
			NOP			W R						XDYD  				XD<XC|D2B	YD<YC|D2B; //Write : X 0x00FF Y 0x8000 Read : X 0x9E00 Y 0x8E00                                                 
			NOP			W R						XDYD  				XD<XC|D2C	YD<YC|D2C; //Write : X 0x01FF Y 0x8000 Read : X 0x9C00 Y 0x8C00                                                 
			NOP			W R						XDYD  				XD<XC|D2D	YD<YC|D2D; //Write : X 0x03FF Y 0x8000 Read : X 0x9800 Y 0x8800                                                 
			NOP			W R						XDYD  				XD<XC|D3	YD<YC|D3 ; //Write : X 0x07FF Y 0x8000 Read : X 0x9000 Y 0x8000                                                 
			NOP			W R						XDYD  				XD<XC|D4	YD<YC|D4 ; //Write : X 0x0FFF Y 0x8000 Read : X 0x8000 Y 0x8000                                                 
			NOP			W R						XDYD  									 ; //Write : X 0x1FFF Y 0x8000 Read : X 0x8000 Y 0x8000                                                 
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
    		STPS																		;
	END
						
			START XD_ALU_AORB_01;
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
			NOP		   XC<0x0000	YC<0x8000													;                                                                  
			NOP		   XS<0x1000	YS<0x9000													;                                                                  
			NOP		   XK<0x2000	YK<0xA000													;                                                                  
			NOP		   XA<0x3000	YA<0xB000													;                                                                  
			NOP		   XD<0x4000	YD<0xC000													;                                                                  
			NOP		   XE<0x5000	YE<0xD000		 											;                                                                  
			NOP		   XF<0x6000	YF<0xE000													;                                                                  
			NOP		   XB<0x7000	YB<0xF000													;                                                                  
			NOP		  D1A<0x0000	D4B<0x1000													;                                                                  
			NOP		  D1B<0x0001														;                                                                  
			NOP		  D1C<0x0002														;                                                                  
			NOP		  D1D<0x0004														;                                                                  
			NOP		  D1E<0x0008														;                                                                  
			NOP		  D1F<0x0010			 											;                                                                  
			NOP		  D1G<0x0020														;                                                                  
			NOP		  D1H<0x0040														;                                                                  
			NOP		  D2A<0x0080	D3B<0x0800													;                                                                  
			NOP		  D2B<0x0100														;                                                                  
			NOP		  D2C<0x0200														;                                                                  
			NOP		  D2D<0x0400														;                                                                  
			NOP		   D3<D3B														;                                                                  
			NOP		   D4<D4B														;                                                                  
			NOP																			;                                                 
			NOP														XD<XS|D1A	YD<YS|D1A;                                                 
			NOP			W R						XDYD  				XD<XS|D1B	YD<YS|D1B; //Write : X 0x1000 Y 0x9000 Read : X 0x1000 Y 0x9000                                                 
			NOP			W R						XDYD  				XD<XS|D1C	YD<YS|D1C; //Write : X 0x1001 Y 0x9001 Read : X 0x1001 Y 0x9001                                                 
			NOP			W R						XDYD  				XD<XS|D1D	YD<YS|D1D; //Write : X 0x1003 Y 0x9003 Read : X 0x1003 Y 0x9003                                                 
			NOP			W R						XDYD  				XD<XS|D1E	YD<YS|D1E; //Write : X 0x1007 Y 0x9007 Read : X 0x1007 Y 0x9007                                                 
			NOP			W R						XDYD  				XD<XS|D1F	YD<YS|D1F; //Write : X 0x100F Y 0x900F Read : X 0x100F Y 0x900F                                                 
			NOP			W R						XDYD  				XD<XS|D1G	YD<YS|D1G; //Write : X 0x101F Y 0x901F Read : X 0x101F Y 0x901F                                                 
			NOP			W R						XDYD  				XD<XS|D1H	YD<YS|D1H; //Write : X 0x103F Y 0x903F Read : X 0x103F Y 0x903F                                                 
			NOP			W R						XDYD  				XD<XS|D2A	YD<YS|D2A; //Write : X 0x107F Y 0x907F Read : X 0x107F Y 0x907F                                                 
			NOP			W R						XDYD  				XD<XS|D2B	YD<YS|D2B; //Write : X 0x10FF Y 0x90FF Read : X 0x10FF Y 0x90FF                                                 
			NOP			W R						XDYD  				XD<XS|D2C	YD<YS|D2C; //Write : X 0x11FF Y 0x91FF Read : X 0x11FF Y 0x91FF                                                 
			NOP			W R						XDYD  				XD<XS|D2D	YD<YS|D2D; //Write : X 0x13FF Y 0x93FF Read : X 0x13FF Y 0x93FF                                                 
			NOP			W R						XDYD  				XD<XS|D3	YD<YS|D3 ; //Write : X 0x17FF Y 0x97FF Read : X 0x17FF Y 0x97FF                                                 
			NOP			W R						XDYD  				XD<XS|D4	YD<YS|D4 ; //Write : X 0x1FFF Y 0x9FFF Read : X 0x1FFF Y 0x9FFF                                                 
			NOP			W R						XDYD  									 ; //Write : X 0x1FFF Y 0x9FFF Read : X 0x1FFF Y 0x9FFF                                                 
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
    		STPS																		;
	END
						
			START XD_ALU_AORB_02;
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
			NOP		   XC<0x0000	YC<0x8000													;                                                                  
			NOP		   XS<0x1000	YS<0x9000													;                                                                  
			NOP		   XK<0x2000	YK<0xA000													;                                                                  
			NOP		   XA<0x3000	YA<0xB000													;                                                                  
			NOP		   XD<0x4000	YD<0xC000													;                                                                  
			NOP		   XE<0x5000	YE<0xD000		 											;                                                                  
			NOP		   XF<0x6000	YF<0xE000													;                                                                  
			NOP		   XB<0x7000	YB<0xF000													;                                                                  
			NOP		  D1A<0x0000	D4B<0x1000													;                                                                  
			NOP		  D1B<0x0001														;                                                                  
			NOP		  D1C<0x0002														;                                                                  
			NOP		  D1D<0x0004														;                                                                  
			NOP		  D1E<0x0008														;                                                                  
			NOP		  D1F<0x0010			 											;                                                                  
			NOP		  D1G<0x0020														;                                                                  
			NOP		  D1H<0x0040														;                                                                  
			NOP		  D2A<0x0080	D3B<0x0800													;                                                                  
			NOP		  D2B<0x0100														;                                                                  
			NOP		  D2C<0x0200														;                                                                  
			NOP		  D2D<0x0400														;                                                                  
			NOP		   D3<D3B														;                                                                  
			NOP		   D4<D4B														;                                                                  
			NOP																			;                                                 
			NOP														XD<XK|D1A	YD<YK|D1A;                                                 
			NOP			W R						XDYD  				XD<XK|D1B	YD<YK|D1B; //Write : X 0x2000 Y 0xA000 Read : X 0x2000 Y 0xA000                                                 
			NOP			W R						XDYD  				XD<XK|D1C	YD<YK|D1C; //Write : X 0x2001 Y 0xA001 Read : X 0x2001 Y 0xA001                                                 
			NOP			W R						XDYD  				XD<XK|D1D	YD<YK|D1D; //Write : X 0x2003 Y 0xA003 Read : X 0x2003 Y 0xA003                                                 
			NOP			W R						XDYD  				XD<XK|D1E	YD<YK|D1E; //Write : X 0x2007 Y 0xA007 Read : X 0x2007 Y 0xA007                                                 
			NOP			W R						XDYD  				XD<XK|D1F	YD<YK|D1F; //Write : X 0x200F Y 0xA00F Read : X 0x200F Y 0xA00F                                                 
			NOP			W R						XDYD  				XD<XK|D1G	YD<YK|D1G; //Write : X 0x201F Y 0xA01F Read : X 0x201F Y 0xA01F                                                 
			NOP			W R						XDYD  				XD<XK|D1H	YD<YK|D1H; //Write : X 0x203F Y 0xA03F Read : X 0x203F Y 0xA03F                                                 
			NOP			W R						XDYD  				XD<XK|D2A	YD<YK|D2A; //Write : X 0x207F Y 0xA07F Read : X 0x207F Y 0xA07F                                                 
			NOP			W R						XDYD  				XD<XK|D2B	YD<YK|D2B; //Write : X 0x20FF Y 0xA0FF Read : X 0x20FF Y 0xA0FF                                                 
			NOP			W R						XDYD  				XD<XK|D2C	YD<YK|D2C; //Write : X 0x21FF Y 0xA1FF Read : X 0x21FF Y 0xA1FF                                                 
			NOP			W R						XDYD  				XD<XK|D2D	YD<YK|D2D; //Write : X 0x23FF Y 0xA3FF Read : X 0x23FF Y 0xA3FF                                                 
			NOP			W R						XDYD  				XD<XK|D3	YD<YK|D3 ; //Write : X 0x27FF Y 0xA7FF Read : X 0x27FF Y 0xA7FF                                                 
			NOP			W R						XDYD  				XD<XK|D4	YD<YK|D4 ; //Write : X 0x2FFF Y 0xAFFF Read : X 0x2FFF Y 0xAFFF                                                 
			NOP			W R						XDYD  									 ; //Write : X 0x3FFF Y 0xBFFF Read : X 0x3FFF Y 0xBFFF                                                 
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
   		STPS																		;
	END
						
			START XD_ALU_AORB_03;
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
			NOP		   XC<0x0000	YC<0x8000													;                                                                  
			NOP		   XS<0x1000	YS<0x9000													;                                                                  
			NOP		   XK<0x2000	YK<0xA000													;                                                                  
			NOP		   XA<0x3000	YA<0xB000													;                                                                  
			NOP		   XD<0x4000	YD<0xC000													;                                                                  
			NOP		   XE<0x5000	YE<0xD000		 											;                                                                  
			NOP		   XF<0x6000	YF<0xE000													;                                                                  
			NOP		   XB<0x7000	YB<0xF000													;                                                                  
			NOP		  D1A<0x0000	D4B<0x1000													;                                                                  
			NOP		  D1B<0x0001														;                                                                  
			NOP		  D1C<0x0002														;                                                                  
			NOP		  D1D<0x0004														;                                                                  
			NOP		  D1E<0x0008														;                                                                  
			NOP		  D1F<0x0010			 											;                                                                  
			NOP		  D1G<0x0020														;                                                                  
			NOP		  D1H<0x0040														;                                                                  
			NOP		  D2A<0x0080	D3B<0x0800													;                                                                  
			NOP		  D2B<0x0100														;                                                                  
			NOP		  D2C<0x0200														;                                                                  
			NOP		  D2D<0x0400														;                                                                  
			NOP		   D3<D3B														;                                                                  
			NOP		   D4<D4B														;                                                                  
			NOP																			;                                                               
			NOP														XD<XA|D1A	YD<YA|D1A;                                                 
			NOP			W R						XDYD  				XD<XA|D1B	YD<YA|D1B; //Write : X 0x3000 Y 0xB000 Read : X 0x3000 Y 0xB000                                                 
			NOP			W R						XDYD  				XD<XA|D1C	YD<YA|D1C; //Write : X 0x3001 Y 0xB001 Read : X 0x3001 Y 0xB001                                                 
			NOP			W R						XDYD  				XD<XA|D1D	YD<YA|D1D; //Write : X 0x3003 Y 0xB003 Read : X 0x3003 Y 0xB003                                                 
			NOP			W R						XDYD  				XD<XA|D1E	YD<YA|D1E; //Write : X 0x3007 Y 0xB007 Read : X 0x3007 Y 0xB007                                                 
			NOP			W R						XDYD  				XD<XA|D1F	YD<YA|D1F; //Write : X 0x300F Y 0xB00F Read : X 0x300F Y 0xB00F                                                 
			NOP			W R						XDYD  				XD<XA|D1G	YD<YA|D1G; //Write : X 0x301F Y 0xB01F Read : X 0x301F Y 0xB01F                                                 
			NOP			W R						XDYD  				XD<XA|D1H	YD<YA|D1H; //Write : X 0x303F Y 0xB03F Read : X 0x303F Y 0xB03F                                                 
			NOP			W R						XDYD  				XD<XA|D2A	YD<YA|D2A; //Write : X 0x307F Y 0xB07F Read : X 0x307F Y 0xB07F                                                 
			NOP			W R						XDYD  				XD<XA|D2B	YD<YA|D2B; //Write : X 0x30FF Y 0xB0FF Read : X 0x30FF Y 0xB0FF                                                 
			NOP			W R						XDYD  				XD<XA|D2C	YD<YA|D2C; //Write : X 0x31FF Y 0xB1FF Read : X 0x31FF Y 0xB1FF                                                 
			NOP			W R						XDYD  				XD<XA|D2D	YD<YA|D2D; //Write : X 0x33FF Y 0xB3FF Read : X 0x33FF Y 0xB3FF                                                 
			NOP			W R						XDYD  				XD<XA|D3	YD<YA|D3 ; //Write : X 0x37FF Y 0xB7FF Read : X 0x37FF Y 0xB7FF                                                 
			NOP			W R						XDYD  				XD<XA|D4	YD<YA|D4 ; //Write : X 0x3FFF Y 0xBFFF Read : X 0x3FFF Y 0xBFFF                                                 
			NOP			W R						XDYD  				XD<XA|D1A	YD<YA|D1A; //Write : X 0x3000 Y 0xB000 Read : X 0x3000 Y 0xB000                                                 
			NOP			W R						XDYD  				XD<XA|D1B	YD<YA|D1B; //Write : X 0x3001 Y 0xB001 Read : X 0x3001 Y 0xB001                                                 
			NOP			W R						XDYD  									 ; //Write : X 0x3FFF Y 0xBFFF Read : X 0x3FFF Y 0xBFFF                                              
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
   		STPS																		;
	END
						
			START XD_ALU_AORB_04;
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
			NOP		   XC<0x0000	YC<0x8000													;                                                                  
			NOP		   XS<0x1000	YS<0x9000													;                                                                  
			NOP		   XK<0x2000	YK<0xA000													;                                                                  
			NOP		   XA<0x3000	YA<0xB000													;                                                                  
			NOP		   XD<0x4000	YD<0xC000													;                                                                  
			NOP		   XE<0x5000	YE<0xD000		 											;                                                                  
			NOP		   XF<0x6000	YF<0xE000													;                                                                  
			NOP		   XB<0x7000	YB<0xF000													;                                                                  
			NOP		  D1A<0x0000	D4B<0x1000													;                                                                  
			NOP		  D1B<0x0001														;                                                                  
			NOP		  D1C<0x0002														;                                                                  
			NOP		  D1D<0x0004														;                                                                  
			NOP		  D1E<0x0008														;                                                                  
			NOP		  D1F<0x0010			 											;                                                                  
			NOP		  D1G<0x0020														;                                                                  
			NOP		  D1H<0x0040														;                                                                  
			NOP		  D2A<0x0080	D3B<0x0800													;                                                                  
			NOP		  D2B<0x0100														;                                                                  
			NOP		  D2C<0x0200														;                                                                  
			NOP		  D2D<0x0400														;                                                                  
			NOP		   D3<D3B														;                                                                  
			NOP		   D4<D4B														;                                                                  
			NOP																			;                                                 
			NOP														XD<XD|D1A	YD<YD|D1A;                                                 
			NOP			W R						XDYD  				XD<XD|D1B	YD<YD|D1B; //Write : X 0x4000 Y 0xC000 Read : X 0x4000 Y 0xC000                                                 
			NOP			W R						XDYD  				XD<XD|D1C	YD<YD|D1C; //Write : X 0x4001 Y 0xC001 Read : X 0x4001 Y 0xC001                                                 
			NOP			W R						XDYD  				XD<XD|D1D	YD<YD|D1D; //Write : X 0x4003 Y 0xC003 Read : X 0x4003 Y 0xC003                                                 
			NOP			W R						XDYD  				XD<XD|D1E	YD<YD|D1E; //Write : X 0x4007 Y 0xC007 Read : X 0x4007 Y 0xC007                                                 
			NOP			W R						XDYD  				XD<XD|D1F	YD<YD|D1F; //Write : X 0x400F Y 0xC00F Read : X 0x400F Y 0xC00F                                                 
			NOP			W R						XDYD  				XD<XD|D1G	YD<YD|D1G; //Write : X 0x401F Y 0xC01F Read : X 0x401F Y 0xC01F                                                 
			NOP			W R						XDYD  				XD<XD|D1H	YD<YD|D1H; //Write : X 0x403F Y 0xC03F Read : X 0x403F Y 0xC03F                                                 
			NOP			W R						XDYD  				XD<XD|D2A	YD<YD|D2A; //Write : X 0x407F Y 0xC07F Read : X 0x407F Y 0xC07F                                                 
			NOP			W R						XDYD  				XD<XD|D2B	YD<YD|D2B; //Write : X 0x40FF Y 0xC0FF Read : X 0x40FF Y 0xC0FF                                                 
			NOP			W R						XDYD  				XD<XD|D2C	YD<YD|D2C; //Write : X 0x41FF Y 0xC1FF Read : X 0x41FF Y 0xC1FF                                                 
			NOP			W R						XDYD  				XD<XD|D2D	YD<YD|D2D; //Write : X 0x43FF Y 0xC3FF Read : X 0x43FF Y 0xC3FF                                                 
			NOP			W R						XDYD  				XD<XD|D3	YD<YD|D3 ; //Write : X 0x47FF Y 0xC7FF Read : X 0x47FF Y 0xC7FF                                                 
			NOP			W R						XDYD  				XD<XD|D4	YD<YD|D4 ; //Write : X 0x4FFF Y 0xCFFF Read : X 0x4FFF Y 0xCFFF                                                 
			NOP			W R						XDYD  				XD<XD|D1A	YD<YD|D1A; //Write : X 0x4000 Y 0xC000 Read : X 0x4000 Y 0xC000                                                 
			NOP			W R						XDYD  				XD<XD|D1B	YD<YD|D1B; //Write : X 0x4001 Y 0xC001 Read : X 0x4001 Y 0xC001                                                 
			NOP			W R						XDYD  									 ; //Write : X 0x5FFF Y 0xDFFF Read : X 0x5FFF Y 0xDFFF                                                 
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
    		STPS																		;
	END
						
			START XD_ALU_AORB_05;
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
			NOP		   XC<0x0000	YC<0x8000													;                                                                  
			NOP		   XS<0x1000	YS<0x9000													;                                                                  
			NOP		   XK<0x2000	YK<0xA000													;                                                                  
			NOP		   XA<0x3000	YA<0xB000													;                                                                  
			NOP		   XD<0x4000	YD<0xC000													;                                                                  
			NOP		   XE<0x5000	YE<0xD000		 											;                                                                  
			NOP		   XF<0x6000	YF<0xE000													;                                                                  
			NOP		   XB<0x7000	YB<0xF000													;                                                                  
			NOP		  D1A<0x0000	D4B<0x1000													;                                                                  
			NOP		  D1B<0x0001														;                                                                  
			NOP		  D1C<0x0002														;                                                                  
			NOP		  D1D<0x0004														;                                                                  
			NOP		  D1E<0x0008														;                                                                  
			NOP		  D1F<0x0010			 											;                                                                  
			NOP		  D1G<0x0020														;                                                                  
			NOP		  D1H<0x0040														;                                                                  
			NOP		  D2A<0x0080	D3B<0x0800													;                                                                  
			NOP		  D2B<0x0100														;                                                                  
			NOP		  D2C<0x0200														;                                                                  
			NOP		  D2D<0x0400														;                                                                  
			NOP		   D3<D3B														;                                                                  
			NOP		   D4<D4B														;                                                                  
			NOP																			;                                                 
			NOP														XD<XE|D1A	YD<YE|D1A;                                                 
			NOP			W R						XDYD  				XD<XE|D1B	YD<YE|D1B; //Write : X 0x5000 Y 0xD000 Read : X 0x5000 Y 0xD000                                                 
			NOP			W R						XDYD  				XD<XE|D1C	YD<YE|D1C; //Write : X 0x5001 Y 0xD001 Read : X 0x5001 Y 0xD001                                                 
			NOP			W R						XDYD  				XD<XE|D1D	YD<YE|D1D; //Write : X 0x5003 Y 0xD003 Read : X 0x5003 Y 0xD003                                                 
			NOP			W R						XDYD  				XD<XE|D1E	YD<YE|D1E; //Write : X 0x5007 Y 0xD007 Read : X 0x5007 Y 0xD007                                                 
			NOP			W R						XDYD  				XD<XE|D1F	YD<YE|D1F; //Write : X 0x500F Y 0xD00F Read : X 0x500F Y 0xD00F                                                 
			NOP			W R						XDYD  				XD<XE|D1G	YD<YE|D1G; //Write : X 0x501F Y 0xD01F Read : X 0x501F Y 0xD01F                                                 
			NOP			W R						XDYD  				XD<XE|D1H	YD<YE|D1H; //Write : X 0x503F Y 0xD03F Read : X 0x503F Y 0xD03F                                                 
			NOP			W R						XDYD  				XD<XE|D2A	YD<YE|D2A; //Write : X 0x507F Y 0xD07F Read : X 0x507F Y 0xD07F                                                 
			NOP			W R						XDYD  				XD<XE|D2B	YD<YE|D2B; //Write : X 0x50FF Y 0xD0FF Read : X 0x50FF Y 0xD0FF                                                 
			NOP			W R						XDYD  				XD<XE|D2C	YD<YE|D2C; //Write : X 0x51FF Y 0xD1FF Read : X 0x51FF Y 0xD1FF                                                 
			NOP			W R						XDYD  				XD<XE|D2D	YD<YE|D2D; //Write : X 0x53FF Y 0xD3FF Read : X 0x53FF Y 0xD3FF                                                 
			NOP			W R						XDYD  				XD<XE|D3	YD<YE|D3 ; //Write : X 0x57FF Y 0xD7FF Read : X 0x57FF Y 0xD7FF                                                 
			NOP			W R						XDYD  				XD<XE|D4	YD<YE|D4 ; //Write : X 0x5FFF Y 0xDFFF Read : X 0x5FFF Y 0xDFFF                                                 
			NOP			W R						XDYD  									 ; //Write : X 0x5FFF Y 0xDFFF Read : X 0x5FFF Y 0xDFFF                                                 
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
    		STPS																		 ;
	END
						
			START XD_ALU_AORB_06;
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
			NOP		   XC<0x0000	YC<0x8000											 ;                                                                  
			NOP		   XS<0x1000	YS<0x9000											 ;                                                                  
			NOP		   XK<0x2000	YK<0xA000											 ;                                                                  
			NOP		   XA<0x3000	YA<0xB000											 ;                                                                  
			NOP		   XD<0x4000	YD<0xC000											 ;                                                                  
			NOP		   XE<0x5000	YE<0xD000		 									 ;                                                                  
			NOP		   XF<0x6000	YF<0xE000											 ;                                                                  
			NOP		   XB<0x7000	YB<0xF000											 ;                                                                  
			NOP		  D1A<0x0000	D4B<0x1000											 ;                                                                  
			NOP		  D1B<0x0001														 ;                                                                  
			NOP		  D1C<0x0002														 ;                                                                  
			NOP		  D1D<0x0004														 ;                                                                  
			NOP		  D1E<0x0008														 ;                                                                  
			NOP		  D1F<0x0010			 											 ;                                                                  
			NOP		  D1G<0x0020														 ;                                                                  
			NOP		  D1H<0x0040														 ;                                                                  
			NOP		  D2A<0x0080	D3B<0x0800											 ;                                                                  
			NOP		  D2B<0x0100														 ;                                                                  
			NOP		  D2C<0x0200														 ;                                                                  
			NOP		  D2D<0x0400														 ;                                                                  
			NOP		   D3<D3B															 ;                                                                  
			NOP		   D4<D4B															 ;                                                                  
			NOP																			 ;                                                 
			NOP														XD<XF|D1A	YD<YF|D1A;                                                 
			NOP			W R						XDYD  				XD<XF|D1B	YD<YF|D1B; //Write : X 0x6000 Y 0xE000 Read : X 0x6000 Y 0xE000                                                 
			NOP			W R						XDYD  				XD<XF|D1C	YD<YF|D1C; //Write : X 0x6001 Y 0xE001 Read : X 0x6001 Y 0xE001                                                 
			NOP			W R						XDYD  				XD<XF|D1D	YD<YF|D1D; //Write : X 0x6003 Y 0xE003 Read : X 0x6003 Y 0xE003                                                 
			NOP			W R						XDYD  				XD<XF|D1E	YD<YF|D1E; //Write : X 0x6007 Y 0xE007 Read : X 0x6007 Y 0xE007                                                 
			NOP			W R						XDYD  				XD<XF|D1F	YD<YF|D1F; //Write : X 0x600F Y 0xE00F Read : X 0x600F Y 0xE00F                                                 
			NOP			W R						XDYD  				XD<XF|D1G	YD<YF|D1G; //Write : X 0x601F Y 0xE01F Read : X 0x601F Y 0xE01F                                                 
			NOP			W R						XDYD  				XD<XF|D1H	YD<YF|D1H; //Write : X 0x603F Y 0xE03F Read : X 0x603F Y 0xE03F                                                 
			NOP			W R						XDYD  				XD<XF|D2A	YD<YF|D2A; //Write : X 0x607F Y 0xE07F Read : X 0x607F Y 0xE07F                                                 
			NOP			W R						XDYD  				XD<XF|D2B	YD<YF|D2B; //Write : X 0x60FF Y 0xE0FF Read : X 0x60FF Y 0xE0FF                                                 
			NOP			W R						XDYD  				XD<XF|D2C	YD<YF|D2C; //Write : X 0x61FF Y 0xE1FF Read : X 0x61FF Y 0xE1FF                                                 
			NOP			W R						XDYD  				XD<XF|D2D	YD<YF|D2D; //Write : X 0x63FF Y 0xE3FF Read : X 0x63FF Y 0xE3FF                                                 
			NOP			W R						XDYD  				XD<XF|D3	YD<YF|D3 ; //Write : X 0x67FF Y 0xE7FF Read : X 0x67FF Y 0xE7FF                                                 
			NOP			W R						XDYD  				XD<XF|D4	YD<YF|D4 ; //Write : X 0x6FFF Y 0xEFFF Read : X 0x6FFF Y 0xEFFF                                                 
			NOP			W R						XDYD  									 ; //Write : X 0x7FFF Y 0xFFFF Read : X 0x7FFF Y 0xFFFF                                                 
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
    		STPS																		 ;
	END
						
			START XD_ALU_AORB_07;
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
			NOP		   XC<0x0000	YC<0x8000											 ;                                                                  
			NOP		   XS<0x1000	YS<0x9000											 ;                                                                  
			NOP		   XK<0x2000	YK<0xA000											 ;                                                                  
			NOP		   XA<0x3000	YA<0xB000											 ;                                                                  
			NOP		   XD<0x4000	YD<0xC000											 ;                                                                  
			NOP		   XE<0x5000	YE<0xD000		 									 ;                                                                  
			NOP		   XF<0x6000	YF<0xE000											 ;                                                                  
			NOP		   XB<0x7000	YB<0xF000											 ;                                                                  
			NOP		  D1A<0x0000	D4B<0x1000											 ;                                                                  
			NOP		  D1B<0x0001														 ;                                                                  
			NOP		  D1C<0x0002														 ;                                                                  
			NOP		  D1D<0x0004														 ;                                                                  
			NOP		  D1E<0x0008														 ;                                                                  
			NOP		  D1F<0x0010			 											 ;                                                                  
			NOP		  D1G<0x0020														 ;                                                                  
			NOP		  D1H<0x0040														 ;                                                                  
			NOP		  D2A<0x0080	D3B<0x0800											 ;                                                                  
			NOP		  D2B<0x0100														 ;                                                                  
			NOP		  D2C<0x0200														 ;                                                                  
			NOP		  D2D<0x0400														 ;                                                                  
			NOP		   D3<D3B															 ;                                                                  
			NOP		   D4<D4B															 ;                                                                  
			NOP																			 ;                                                 
			NOP														XD<XB|D1A	YD<YB|D1A;                                                 
			NOP			W R						XDYD  				XD<XB|D1B	YD<YB|D1B; //Write : X 0x7000 Y 0xF000 Read : X 0x7000 Y 0xF000                                                 
			NOP			W R						XDYD  				XD<XB|D1C	YD<YB|D1C; //Write : X 0x7001 Y 0xF001 Read : X 0x7001 Y 0xF001                                                 
			NOP			W R						XDYD  				XD<XB|D1D	YD<YB|D1D; //Write : X 0x7003 Y 0xF003 Read : X 0x7003 Y 0xF003                                                 
			NOP			W R						XDYD  				XD<XB|D1E	YD<YB|D1E; //Write : X 0x7007 Y 0xF007 Read : X 0x7007 Y 0xF007                                                 
			NOP			W R						XDYD  				XD<XB|D1F	YD<YB|D1F; //Write : X 0x700F Y 0xF00F Read : X 0x700F Y 0xF00F                                                 
			NOP			W R						XDYD  				XD<XB|D1G	YD<YB|D1G; //Write : X 0x701F Y 0xF01F Read : X 0x701F Y 0xF01F                                                 
			NOP			W R						XDYD  				XD<XB|D1H	YD<YB|D1H; //Write : X 0x703F Y 0xF03F Read : X 0x703F Y 0xF03F                                                 
			NOP			W R						XDYD  				XD<XB|D2A	YD<YB|D2A; //Write : X 0x707F Y 0xF07F Read : X 0x707F Y 0xF07F                                                 
			NOP			W R						XDYD  				XD<XB|D2B	YD<YB|D2B; //Write : X 0x70FF Y 0xF0FF Read : X 0x70FF Y 0xF0FF                                                 
			NOP			W R						XDYD  				XD<XB|D2C	YD<YB|D2C; //Write : X 0x71FF Y 0xF1FF Read : X 0x71FF Y 0xF1FF                                                 
			NOP			W R						XDYD  				XD<XB|D2D	YD<YB|D2D; //Write : X 0x73FF Y 0xF3FF Read : X 0x73FF Y 0xF3FF                                                 
			NOP			W R						XDYD  				XD<XB|D3	YD<YB|D3 ; //Write : X 0x77FF Y 0xF7FF Read : X 0x77FF Y 0xF7FF                                                 
			NOP			W R						XDYD  				XD<XB|D4	YD<YB|D4 ; //Write : X 0x7FFF Y 0xFFFF Read : X 0x7FFF Y 0xFFFF                                                 
			NOP			W R						XDYD  									 ; //Write : X 0x7FFF Y 0xFFFF Read : X 0x7FFF Y 0xFFFF                                                 
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
    		STPS																		;
	END
																		
			START XD_ALU_AXORB_00;
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
			NOP		   XC<0xFFFF	YC<0x0000														;                                                                  
			NOP		   XS<0x8001	YS<0x8001														;                                                                  
			NOP		   XK<0x8002	YK<0x8002														;                                                                  
			NOP		   XA<0x8004	YA<0x8004														;                                                                  
			NOP		   XD<0x8008	YD<0x8008														;                                                                  
			NOP		   XE<0x8010	YE<0x8010		 												;                                                                  
			NOP		   XF<0x8020	YF<0x8020														;                                                                  
			NOP		   XB<0x8040	YB<0x8040														;                                                                  
			NOP		  D1A<0x0000	D4B<0x1000														;                                                                  
			NOP		  D1B<0x0001																	;                                                                  
			NOP		  D1C<0x0002																	;                                                                  
			NOP		  D1D<0x0004																	;                                                                  
			NOP		  D1E<0x0008																	;                                                                  
			NOP		  D1F<0x0010			 														;                                                                  
			NOP		  D1G<0x0020																	;                                                                  
			NOP		  D1H<0x0040																	;                                                                  
			NOP		  D2A<0x0080	D3B<0x0800														;                                                                  
			NOP		  D2B<0x0100																	;                                                                  
			NOP		  D2C<0x0200																	;                                                                  
			NOP		  D2D<0x0400																	;                                                                  
			NOP		   D3<D3B																		;                                                                  
			NOP		   D4<D4B																		;                                                                  
			NOP																						;                                                 
			NOP														XD<XC^D1A	YD<YC^D1A;                                                 
			NOP			W R						XDYD  				XD<XC^D1B	YD<YC^D1B; //Write : X 0xFFFF Y 0x0000 Read : X 0xFFFF Y 0x0000                                                 
			NOP			W R						XDYD  				XD<XC^D1C	YD<YC^D1C; //Write : X 0xFFFE Y 0x0001 Read : X 0xFFFE Y 0x0001                                                 
			NOP			W R						XDYD  				XD<XC^D1D	YD<YC^D1D; //Write : X 0xFFFC Y 0x0003 Read : X 0xFFFC Y 0x0003                                                 
			NOP			W R						XDYD  				XD<XC^D1E	YD<YC^D1E; //Write : X 0xFFF8 Y 0x0007 Read : X 0xFFF8 Y 0x0007                                                 
			NOP			W R						XDYD  				XD<XC^D1F	YD<YC^D1F; //Write : X 0xFFF0 Y 0x000F Read : X 0xFFF0 Y 0x000F                                                 
			NOP			W R						XDYD  				XD<XC^D1G	YD<YC^D1G; //Write : X 0xFFE0 Y 0x001F Read : X 0xFFE0 Y 0x001F                                                 
			NOP			W R						XDYD  				XD<XC^D1H	YD<YC^D1H; //Write : X 0xFFC0 Y 0x003F Read : X 0xFFC0 Y 0x003F                                                 
			NOP			W R						XDYD  				XD<XC^D2A	YD<YC^D2A; //Write : X 0xFF80 Y 0x007F Read : X 0xFF80 Y 0x007F                                                 
			NOP			W R						XDYD  				XD<XC^D2B	YD<YC^D2B; //Write : X 0xFF00 Y 0x00FF Read : X 0xFF00 Y 0x00FF                                                 
			NOP			W R						XDYD  				XD<XC^D2C	YD<YC^D2C; //Write : X 0xFE00 Y 0x01FF Read : X 0xFE00 Y 0x01FF                                                 
			NOP			W R						XDYD  				XD<XC^D2D	YD<YC^D2D; //Write : X 0xFC00 Y 0x03FF Read : X 0xFC00 Y 0x03FF                                                 
			NOP			W R						XDYD  				XD<XC^D3	YD<YC^D3 ; //Write : X 0xF800 Y 0x07FF Read : X 0xF800 Y 0x07FF                                                 
			NOP			W R						XDYD  				XD<XC^D4	YD<YC^D4 ; //Write : X 0xF000 Y 0x0FFF Read : X 0xF000 Y 0x0FFF                                                 
			NOP			W R						XDYD  									 ; //Write : X 0xE000 Y 0x1FFF Read : X 0xE000 Y 0x1FFF                                                 
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
    		STPS																					;
	END
																		
			START XD_ALU_AXORB_01;
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
			NOP		   XC<0xFFFF	YC<0x0000											 ;                                                                  
			NOP		   XS<0x1000	YS<0x9000											 ;                                                                  
			NOP		   XK<0x2000	YK<0xA000											 ;                                                                  
			NOP		   XA<0x3000	YA<0xB000											 ;                                                                  
			NOP		   XD<0x4000	YD<0xC000											 ;                                                                  
			NOP		   XE<0x5000	YE<0xD000		 									 ;                                                                  
			NOP		   XF<0x6000	YF<0xE000											 ;                                                                  
			NOP		   XB<0x7000	YB<0xF000											 ;                                                                  
			NOP		  D1A<0x0000	D4B<0x1000											 ;                                                                  
			NOP		  D1B<0x0001														 ;                                                                  
			NOP		  D1C<0x0002														 ;                                                                  
			NOP		  D1D<0x0004														 ;                                                                  
			NOP		  D1E<0x0008														 ;                                                                  
			NOP		  D1F<0x0010			 											 ;                                                                  
			NOP		  D1G<0x0020														 ;                                                                  
			NOP		  D1H<0x0040														 ;                                                                  
			NOP		  D2A<0x0080	D3B<0x0800											 ;                                                                  
			NOP		  D2B<0x0100														 ;                                                                  
			NOP		  D2C<0x0200														 ;                                                                  
			NOP		  D2D<0x0400														 ;                                                                  
			NOP		   D3<D3B															 ;                                                                  
			NOP		   D4<D4B															 ;                                                                  
			NOP																			 ;                                                 
			NOP														XD<XS^D1A	YD<YS^D1A;                                                 
			NOP			W R						XDYD  				XD<XS^D1B	YD<YS^D1B; //Write : X 0x1000 Y 0x9000 Read : X 0x1000 Y 0x9000                                                 
			NOP			W R						XDYD  				XD<XS^D1C	YD<YS^D1C; //Write : X 0x1001 Y 0x9001 Read : X 0x1001 Y 0x9001                                                 
			NOP			W R						XDYD  				XD<XS^D1D	YD<YS^D1D; //Write : X 0x1003 Y 0x9003 Read : X 0x1003 Y 0x9003                                                 
			NOP			W R						XDYD  				XD<XS^D1E	YD<YS^D1E; //Write : X 0x1007 Y 0x9007 Read : X 0x1007 Y 0x9007                                                 
			NOP			W R						XDYD  				XD<XS^D1F	YD<YS^D1F; //Write : X 0x100F Y 0x900F Read : X 0x100F Y 0x900F                                                 
			NOP			W R						XDYD  				XD<XS^D1G	YD<YS^D1G; //Write : X 0x101F Y 0x901F Read : X 0x101F Y 0x901F                                                 
			NOP			W R						XDYD  				XD<XS^D1H	YD<YS^D1H; //Write : X 0x103F Y 0x903F Read : X 0x103F Y 0x903F                                                 
			NOP			W R						XDYD  				XD<XS^D2A	YD<YS^D2A; //Write : X 0x107F Y 0x907F Read : X 0x107F Y 0x907F                                                 
			NOP			W R						XDYD  				XD<XS^D2B	YD<YS^D2B; //Write : X 0x10FF Y 0x90FF Read : X 0x10FF Y 0x90FF                                                 
			NOP			W R						XDYD  				XD<XS^D2C	YD<YS^D2C; //Write : X 0x11FF Y 0x91FF Read : X 0x11FF Y 0x91FF                                                 
			NOP			W R						XDYD  				XD<XS^D2D	YD<YS^D2D; //Write : X 0x13FF Y 0x93FF Read : X 0x13FF Y 0x93FF                                                 
			NOP			W R						XDYD  				XD<XS^D3	YD<YS^D3 ; //Write : X 0x17FF Y 0x97FF Read : X 0x17FF Y 0x97FF                                                 
			NOP			W R						XDYD  				XD<XS^D4	YD<YS^D4 ; //Write : X 0x1FFF Y 0x9FFF Read : X 0x1FFF Y 0x9FFF                                                 
			NOP			W R						XDYD  									 ; //Write : X 0x0FFF Y 0x8FFF Read : X 0x0FFF Y 0x8FFF                                                 
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
    		STPS																		;
	END		
																		
			START XD_ALU_AXORB_02;
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
			NOP		   XC<0xFFFF	YC<0x0000														;                                                                  
			NOP		   XS<0x1000	YS<0x9000														;                                                                  
			NOP		   XK<0x2000	YK<0xA000														;                                                                  
			NOP		   XA<0x3000	YA<0xB000														;                                                                  
			NOP		   XD<0x4000	YD<0xC000														;                                                                  
			NOP		   XE<0x5000	YE<0xD000		 												;                                                                  
			NOP		   XF<0x6000	YF<0xE000														;                                                                  
			NOP		   XB<0x7000	YB<0xF000														;                                                                  
			NOP		  D1A<0x0000	D4B<0x1000														;                                                                  
			NOP		  D1B<0x0001																	;                                                                  
			NOP		  D1C<0x0002																	;                                                                  
			NOP		  D1D<0x0004																	;                                                                  
			NOP		  D1E<0x0008																	;                                                                  
			NOP		  D1F<0x0010			 														;                                                                  
			NOP		  D1G<0x0020																	;                                                                  
			NOP		  D1H<0x0040																	;                                                                  
			NOP		  D2A<0x0080	D3B<0x0800														;                                                                  
			NOP		  D2B<0x0100																	;                                                                  
			NOP		  D2C<0x0200																	;                                                                  
			NOP		  D2D<0x0400																	;                                                                  
			NOP		   D3<D3B																		;                                                                  
			NOP		   D4<D4B																		;                                                                  
			NOP																						;                                                 
			NOP														XD<XK^D1A	YD<YK^D1A;                                                 
			NOP			W R						XDYD  				XD<XK^D1B	YD<YK^D1B; //Write : X 0x2000 Y 0xA000 Read : X 0x2000 Y 0xA000                                                 
			NOP			W R						XDYD  				XD<XK^D1C	YD<YK^D1C; //Write : X 0x2001 Y 0xA001 Read : X 0x2001 Y 0xA001                                                 
			NOP			W R						XDYD  				XD<XK^D1D	YD<YK^D1D; //Write : X 0x2003 Y 0xA003 Read : X 0x2003 Y 0xA003                                                 
			NOP			W R						XDYD  				XD<XK^D1E	YD<YK^D1E; //Write : X 0x2007 Y 0xA007 Read : X 0x2007 Y 0xA007                                                 
			NOP			W R						XDYD  				XD<XK^D1F	YD<YK^D1F; //Write : X 0x200F Y 0xA00F Read : X 0x200F Y 0xA00F                                                 
			NOP			W R						XDYD  				XD<XK^D1G	YD<YK^D1G; //Write : X 0x201F Y 0xA01F Read : X 0x201F Y 0xA01F                                                 
			NOP			W R						XDYD  				XD<XK^D1H	YD<YK^D1H; //Write : X 0x203F Y 0xA03F Read : X 0x203F Y 0xA03F                                                 
			NOP			W R						XDYD  				XD<XK^D2A	YD<YK^D2A; //Write : X 0x207F Y 0xA07F Read : X 0x207F Y 0xA07F                                                 
			NOP			W R						XDYD  				XD<XK^D2B	YD<YK^D2B; //Write : X 0x20FF Y 0xA0FF Read : X 0x20FF Y 0xA0FF                                                 
			NOP			W R						XDYD  				XD<XK^D2C	YD<YK^D2C; //Write : X 0x21FF Y 0xA1FF Read : X 0x21FF Y 0xA1FF                                                 
			NOP			W R						XDYD  				XD<XK^D2D	YD<YK^D2D; //Write : X 0x23FF Y 0xA3FF Read : X 0x23FF Y 0xA3FF                                                 
			NOP			W R						XDYD  				XD<XK^D3	YD<YK^D3 ; //Write : X 0x27FF Y 0xA7FF Read : X 0x27FF Y 0xA7FF                                                 
			NOP			W R						XDYD  				XD<XK^D4	YD<YK^D4 ; //Write : X 0x2FFF Y 0xAFFF Read : X 0x2FFF Y 0xAFFF                                                 
			NOP			W R						XDYD  									 ; //Write : X 0x3FFF Y 0xBFFF Read : X 0x3FFF Y 0xBFFF                                                 
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
    		STPS																		;
	END	
																			
			START XD_ALU_AXORB_03;
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
			NOP		   XC<0xFFFF	YC<0x0000											 ;                                                                  
			NOP		   XS<0x1000	YS<0x9000											 ;                                                                  
			NOP		   XK<0x2000	YK<0xA000											 ;                                                                  
			NOP		   XA<0x3000	YA<0xB000											 ;                                                                  
			NOP		   XD<0x4000	YD<0xC000											 ;                                                                  
			NOP		   XE<0x5000	YE<0xD000		 									 ;                                                                  
			NOP		   XF<0x6000	YF<0xE000											 ;                                                                  
			NOP		   XB<0x7000	YB<0xF000											 ;                                                                  
			NOP		  D1A<0x0000	D4B<0x1000											 ;                                                                  
			NOP		  D1B<0x0001														 ;                                                                  
			NOP		  D1C<0x0002														 ;                                                                  
			NOP		  D1D<0x0004														 ;                                                                  
			NOP		  D1E<0x0008														 ;                                                                  
			NOP		  D1F<0x0010			 											 ;                                                                  
			NOP		  D1G<0x0020														 ;                                                                  
			NOP		  D1H<0x0040														 ;                                                                  
			NOP		  D2A<0x0080	D3B<0x0800											 ;                                                                  
			NOP		  D2B<0x0100														 ;                                                                  
			NOP		  D2C<0x0200														 ;                                                                  
			NOP		  D2D<0x0400														 ;                                                                  
			NOP		   D3<D3B															 ;                                                                  
			NOP		   D4<D4B															 ;                                                                  
			NOP																			 ;                                                 
			NOP																			 ;                                                 
			NOP														XD<XA^D1A	YD<YA^D1A;                                                 
			NOP			W R						XDYD  				XD<XA^D1B	YD<YA^D1B; //Write : X 0x3000 Y 0xB000 Read : X 0x3000 Y 0xB000                                                 
			NOP			W R						XDYD  				XD<XA^D1C	YD<YA^D1C; //Write : X 0x3001 Y 0xB001 Read : X 0x3001 Y 0xB001                                                 
			NOP			W R						XDYD  				XD<XA^D1D	YD<YA^D1D; //Write : X 0x3003 Y 0xB003 Read : X 0x3003 Y 0xB003                                                 
			NOP			W R						XDYD  				XD<XA^D1E	YD<YA^D1E; //Write : X 0x3007 Y 0xB007 Read : X 0x3007 Y 0xB007                                                 
			NOP			W R						XDYD  				XD<XA^D1F	YD<YA^D1F; //Write : X 0x300F Y 0xB00F Read : X 0x300F Y 0xB00F                                                 
			NOP			W R						XDYD  				XD<XA^D1G	YD<YA^D1G; //Write : X 0x301F Y 0xB01F Read : X 0x301F Y 0xB01F                                                 
			NOP			W R						XDYD  				XD<XA^D1H	YD<YA^D1H; //Write : X 0x303F Y 0xB03F Read : X 0x303F Y 0xB03F                                                 
			NOP			W R						XDYD  				XD<XA^D2A	YD<YA^D2A; //Write : X 0x307F Y 0xB07F Read : X 0x307F Y 0xB07F                                                 
			NOP			W R						XDYD  				XD<XA^D2B	YD<YA^D2B; //Write : X 0x30FF Y 0xB0FF Read : X 0x30FF Y 0xB0FF                                                 
			NOP			W R						XDYD  				XD<XA^D2C	YD<YA^D2C; //Write : X 0x31FF Y 0xB1FF Read : X 0x31FF Y 0xB1FF                                                 
			NOP			W R						XDYD  				XD<XA^D2D	YD<YA^D2D; //Write : X 0x33FF Y 0xB3FF Read : X 0x33FF Y 0xB3FF                                                 
			NOP			W R						XDYD  				XD<XA^D3	YD<YA^D3 ; //Write : X 0x37FF Y 0xB7FF Read : X 0x37FF Y 0xB7FF                                                 
			NOP			W R						XDYD  				XD<XA^D4	YD<YA^D4 ; //Write : X 0x3FFF Y 0xBFFF Read : X 0x3FFF Y 0xBFFF                                                 
			NOP			W R						XDYD  									 ; //Write : X 0x2FFF Y 0xAFFF Read : X 0x2FFF Y 0xAFFF                                                 
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
    		STPS																		;
	END	
																			
			START XD_ALU_AXORB_04;
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
			NOP		   XC<0xFFFF	YC<0x0000											 ;                                                                  
			NOP		   XS<0x1000	YS<0x9000											 ;                                                                  
			NOP		   XK<0x2000	YK<0xA000											 ;                                                                  
			NOP		   XA<0x3000	YA<0xB000											 ;                                                                  
			NOP		   XD<0x4000	YD<0xC000											 ;                                                                  
			NOP		   XE<0x5000	YE<0xD000		 									 ;                                                                  
			NOP		   XF<0x6000	YF<0xE000											 ;                                                                  
			NOP		   XB<0x7000	YB<0xF000											 ;                                                                  
			NOP		  D1A<0x0000	D4B<0x1000											 ;                                                                  
			NOP		  D1B<0x0001														 ;                                                                  
			NOP		  D1C<0x0002														 ;                                                                  
			NOP		  D1D<0x0004														 ;                                                                  
			NOP		  D1E<0x0008														 ;                                                                  
			NOP		  D1F<0x0010			 											 ;                                                                  
			NOP		  D1G<0x0020														 ;                                                                  
			NOP		  D1H<0x0040														 ;                                                                  
			NOP		  D2A<0x0080	D3B<0x0800											 ;                                                                  
			NOP		  D2B<0x0100														 ;                                                                  
			NOP		  D2C<0x0200														 ;                                                                  
			NOP		  D2D<0x0400														 ;                                                                  
			NOP		   D3<D3B															 ;                                                                  
			NOP		   D4<D4B															 ;                                                                  
			NOP																			 ;                                                 
			NOP																			 ;                                                 
			NOP														XD<XD^D1A	YD<YD^D1A;                                                 
			NOP			W R						XDYD  				XD<XD^D1B	YD<YD^D1B; //Write : X 0x4000 Y 0xC000 Read : X 0x4000 Y 0xC000                                                 
			NOP			W R						XDYD  				XD<XD^D1C	YD<YD^D1C; //Write : X 0x4001 Y 0xC001 Read : X 0x4001 Y 0xC001                                                 
			NOP			W R						XDYD  				XD<XD^D1D	YD<YD^D1D; //Write : X 0x4003 Y 0xC003 Read : X 0x4003 Y 0xC003                                                 
			NOP			W R						XDYD  				XD<XD^D1E	YD<YD^D1E; //Write : X 0x4007 Y 0xC007 Read : X 0x4007 Y 0xC007                                                 
			NOP			W R						XDYD  				XD<XD^D1F	YD<YD^D1F; //Write : X 0x400F Y 0xC00F Read : X 0x400F Y 0xC00F                                                 
			NOP			W R						XDYD  				XD<XD^D1G	YD<YD^D1G; //Write : X 0x401F Y 0xC01F Read : X 0x401F Y 0xC01F                                                 
			NOP			W R						XDYD  				XD<XD^D1H	YD<YD^D1H; //Write : X 0x403F Y 0xC03F Read : X 0x403F Y 0xC03F                                                 
			NOP			W R						XDYD  				XD<XD^D2A	YD<YD^D2A; //Write : X 0x407F Y 0xC07F Read : X 0x407F Y 0xC07F                                                 
			NOP			W R						XDYD  				XD<XD^D2B	YD<YD^D2B; //Write : X 0x40FF Y 0xC0FF Read : X 0x40FF Y 0xC0FF                                                 
			NOP			W R						XDYD  				XD<XD^D2C	YD<YD^D2C; //Write : X 0x41FF Y 0xC1FF Read : X 0x41FF Y 0xC1FF                                                 
			NOP			W R						XDYD  				XD<XD^D2D	YD<YD^D2D; //Write : X 0x43FF Y 0xC3FF Read : X 0x43FF Y 0xC3FF                                                 
			NOP			W R						XDYD  				XD<XD^D3	YD<YD^D3 ; //Write : X 0x47FF Y 0xC7FF Read : X 0x47FF Y 0xC7FF                                                 
			NOP			W R						XDYD  				XD<XD^D4	YD<YD^D4 ; //Write : X 0x4FFF Y 0xCFFF Read : X 0x4FFF Y 0xCFFF                                                 
			NOP			W R						XDYD  									 ; //Write : X 0x5FFF Y 0xDFFF Read : X 0x5FFF Y 0xDFFF                                                 
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
    		STPS																		;
	END	
																		
			START XD_ALU_AXORB_05;
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
			NOP		   XC<0xFFFF	YC<0x0000											 ;                                                                  
			NOP		   XS<0x1000	YS<0x9000											 ;                                                                  
			NOP		   XK<0x2000	YK<0xA000											 ;                                                                  
			NOP		   XA<0x3000	YA<0xB000											 ;                                                                  
			NOP		   XD<0x4000	YD<0xC000											 ;                                                                  
			NOP		   XE<0x5000	YE<0xD000		 									 ;                                                                  
			NOP		   XF<0x6000	YF<0xE000											 ;                                                                  
			NOP		   XB<0x7000	YB<0xF000											 ;                                                                  
			NOP		  D1A<0x0000	D4B<0x1000											 ;                                                                  
			NOP		  D1B<0x0001														 ;                                                                  
			NOP		  D1C<0x0002														 ;                                                                  
			NOP		  D1D<0x0004														 ;                                                                  
			NOP		  D1E<0x0008														 ;                                                                  
			NOP		  D1F<0x0010			 											 ;                                                                  
			NOP		  D1G<0x0020														 ;                                                                  
			NOP		  D1H<0x0040														 ;                                                                  
			NOP		  D2A<0x0080	D3B<0x0800											 ;                                                                  
			NOP		  D2B<0x0100														 ;                                                                  
			NOP		  D2C<0x0200														 ;                                                                  
			NOP		  D2D<0x0400														 ;                                                                  
			NOP		   D3<D3B															 ;                                                                  
			NOP		   D4<D4B															 ;                                                                  
			NOP																			 ;                                                                  
			NOP																			 ;                                                 
			NOP														XD<XE^D1A	YD<YE^D1A;                                                 
			NOP			W R						XDYD  				XD<XE^D1B	YD<YE^D1B; //Write : X 0x5000 Y 0xD000 Read : X 0x5000 Y 0xD000                                                 
			NOP			W R						XDYD  				XD<XE^D1C	YD<YE^D1C; //Write : X 0x5001 Y 0xD001 Read : X 0x5001 Y 0xD001                                                 
			NOP			W R						XDYD  				XD<XE^D1D	YD<YE^D1D; //Write : X 0x5003 Y 0xD003 Read : X 0x5003 Y 0xD003                                                 
			NOP			W R						XDYD  				XD<XE^D1E	YD<YE^D1E; //Write : X 0x5007 Y 0xD007 Read : X 0x5007 Y 0xD007                                                 
			NOP			W R						XDYD  				XD<XE^D1F	YD<YE^D1F; //Write : X 0x500F Y 0xD00F Read : X 0x500F Y 0xD00F                                                 
			NOP			W R						XDYD  				XD<XE^D1G	YD<YE^D1G; //Write : X 0x501F Y 0xD01F Read : X 0x501F Y 0xD01F                                                 
			NOP			W R						XDYD  				XD<XE^D1H	YD<YE^D1H; //Write : X 0x503F Y 0xD03F Read : X 0x503F Y 0xD03F                                                 
			NOP			W R						XDYD  				XD<XE^D2A	YD<YE^D2A; //Write : X 0x507F Y 0xD07F Read : X 0x507F Y 0xD07F                                                 
			NOP			W R						XDYD  				XD<XE^D2B	YD<YE^D2B; //Write : X 0x50FF Y 0xD0FF Read : X 0x50FF Y 0xD0FF                                                 
			NOP			W R						XDYD  				XD<XE^D2C	YD<YE^D2C; //Write : X 0x51FF Y 0xD1FF Read : X 0x51FF Y 0xD1FF                                                 
			NOP			W R						XDYD  				XD<XE^D2D	YD<YE^D2D; //Write : X 0x53FF Y 0xD3FF Read : X 0x53FF Y 0xD3FF                                                 
			NOP			W R						XDYD  				XD<XE^D3	YD<YE^D3 ; //Write : X 0x57FF Y 0xD7FF Read : X 0x57FF Y 0xD7FF                                                 
			NOP			W R						XDYD  				XD<XE^D4	YD<YE^D4 ; //Write : X 0x5FFF Y 0xDFFF Read : X 0x5FFF Y 0xDFFF                                                 
			NOP			W R						XDYD  									 ; //Write : X 0x4FFF Y 0xCFFF Read : X 0x4FFF Y 0xCFFF                                                 
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
    		STPS																		;
	END		
																			
			START XD_ALU_AXORB_06;
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
			NOP		   XC<0xFFFF	YC<0x0000											 ;                                                                  
			NOP		   XS<0x1000	YS<0x9000											 ;                                                                  
			NOP		   XK<0x2000	YK<0xA000											 ;                                                                  
			NOP		   XA<0x3000	YA<0xB000											 ;                                                                  
			NOP		   XD<0x4000	YD<0xC000											 ;                                                                  
			NOP		   XE<0x5000	YE<0xD000		 									 ;                                                                  
			NOP		   XF<0x6000	YF<0xE000											 ;                                                                  
			NOP		   XB<0x7000	YB<0xF000											 ;                                                                  
			NOP		  D1A<0x0000	D4B<0x1000											 ;                                                                  
			NOP		  D1B<0x0001														 ;                                                                  
			NOP		  D1C<0x0002														 ;                                                                  
			NOP		  D1D<0x0004														 ;                                                                  
			NOP		  D1E<0x0008														 ;                                                                  
			NOP		  D1F<0x0010			 											 ;                                                                  
			NOP		  D1G<0x0020														 ;                                                                  
			NOP		  D1H<0x0040														 ;                                                                  
			NOP		  D2A<0x0080	D3B<0x0800											 ;                                                                  
			NOP		  D2B<0x0100														 ;                                                                  
			NOP		  D2C<0x0200														 ;                                                                  
			NOP		  D2D<0x0400														 ;                                                                  
			NOP		   D3<D3B															 ;                                                                  
			NOP		   D4<D4B															 ;                                                                  
			NOP																			 ;                                                 
			NOP														XD<XF^D1A	YD<YF^D1A;                                                 
			NOP			W R						XDYD  				XD<XF^D1B	YD<YF^D1B; //Write : X 0x6000 Y 0xE000 Read : X 0x6000 Y 0xE000                                                 
			NOP			W R						XDYD  				XD<XF^D1C	YD<YF^D1C; //Write : X 0x6001 Y 0xE001 Read : X 0x6001 Y 0xE001                                                 
			NOP			W R						XDYD  				XD<XF^D1D	YD<YF^D1D; //Write : X 0x6003 Y 0xE003 Read : X 0x6003 Y 0xE003                                                 
			NOP			W R						XDYD  				XD<XF^D1E	YD<YF^D1E; //Write : X 0x6007 Y 0xE007 Read : X 0x6007 Y 0xE007                                                 
			NOP			W R						XDYD  				XD<XF^D1F	YD<YF^D1F; //Write : X 0x600F Y 0xE00F Read : X 0x600F Y 0xE00F                                                 
			NOP			W R						XDYD  				XD<XF^D1G	YD<YF^D1G; //Write : X 0x601F Y 0xE01F Read : X 0x601F Y 0xE01F                                                 
			NOP			W R						XDYD  				XD<XF^D1H	YD<YF^D1H; //Write : X 0x603F Y 0xE03F Read : X 0x603F Y 0xE03F                                                 
			NOP			W R						XDYD  				XD<XF^D2A	YD<YF^D2A; //Write : X 0x607F Y 0xE07F Read : X 0x607F Y 0xE07F                                                 
			NOP			W R						XDYD  				XD<XF^D2B	YD<YF^D2B; //Write : X 0x60FF Y 0xE0FF Read : X 0x60FF Y 0xE0FF                                                 
			NOP			W R						XDYD  				XD<XF^D2C	YD<YF^D2C; //Write : X 0x61FF Y 0xE1FF Read : X 0x61FF Y 0xE1FF                                                 
			NOP			W R						XDYD  				XD<XF^D2D	YD<YF^D2D; //Write : X 0x63FF Y 0xE3FF Read : X 0x63FF Y 0xE3FF                                                 
			NOP			W R						XDYD  				XD<XF^D3	YD<YF^D3 ; //Write : X 0x67FF Y 0xE7FF Read : X 0x67FF Y 0xE7FF                                                 
			NOP			W R						XDYD  				XD<XF^D4	YD<YF^D4 ; //Write : X 0x6FFF Y 0xEFFF Read : X 0x6FFF Y 0xEFFF                                                 
			NOP			W R						XDYD  									 ; //Write : X 0x7FFF Y 0xFFFF Read : X 0x7FFF Y 0xFFFF                                                 
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
    		STPS																		;
	END	
																			
			START XD_ALU_AXORB_07;
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
			NOP		   XC<0xFFFF	YC<0x0000											 ;                                                                  
			NOP		   XS<0x1000	YS<0x9000											 ;                                                                  
			NOP		   XK<0x2000	YK<0xA000											 ;                                                                  
			NOP		   XA<0x3000	YA<0xB000											 ;                                                                  
			NOP		   XD<0x4000	YD<0xC000											 ;                                                                  
			NOP		   XE<0x5000	YE<0xD000		 									 ;                                                                  
			NOP		   XF<0x6000	YF<0xE000											 ;                                                                  
			NOP		   XB<0x7000	YB<0xF000											 ;                                                                  
			NOP		  D1A<0x0000	D4B<0x1000											 ;                                                                  
			NOP		  D1B<0x0001														 ;                                                                  
			NOP		  D1C<0x0002														 ;                                                                  
			NOP		  D1D<0x0004														 ;                                                                  
			NOP		  D1E<0x0008														 ;                                                                  
			NOP		  D1F<0x0010			 											 ;                                                                  
			NOP		  D1G<0x0020														 ;                                                                  
			NOP		  D1H<0x0040														 ;                                                                  
			NOP		  D2A<0x0080	D3B<0x0800											 ;                                                                  
			NOP		  D2B<0x0100														 ;                                                                  
			NOP		  D2C<0x0200														 ;                                                                  
			NOP		  D2D<0x0400														 ;                                                                  
			NOP		   D3<D3B															 ;                                                                  
			NOP		   D4<D4B															 ;                                                                  
			NOP																			 ;                                                 
			NOP														XD<XB^D1A	YD<YB^D1A;                                                 
			NOP			W R						XDYD  				XD<XB^D1B	YD<YB^D1B; //Write : X 0x7000 Y 0xF000 Read : X 0x7000 Y 0xF000                                                 
			NOP			W R						XDYD  				XD<XB^D1C	YD<YB^D1C; //Write : X 0x7001 Y 0xF001 Read : X 0x7001 Y 0xF001                                                 
			NOP			W R						XDYD  				XD<XB^D1D	YD<YB^D1D; //Write : X 0x7003 Y 0xF003 Read : X 0x7003 Y 0xF003                                                 
			NOP			W R						XDYD  				XD<XB^D1E	YD<YB^D1E; //Write : X 0x7007 Y 0xF007 Read : X 0x7007 Y 0xF007                                                 
			NOP			W R						XDYD  				XD<XB^D1F	YD<YB^D1F; //Write : X 0x700F Y 0xF00F Read : X 0x700F Y 0xF00F                                                 
			NOP			W R						XDYD  				XD<XB^D1G	YD<YB^D1G; //Write : X 0x701F Y 0xF01F Read : X 0x701F Y 0xF01F                                                 
			NOP			W R						XDYD  				XD<XB^D1H	YD<YB^D1H; //Write : X 0x703F Y 0xF03F Read : X 0x703F Y 0xF03F                                                 
			NOP			W R						XDYD  				XD<XB^D2A	YD<YB^D2A; //Write : X 0x707F Y 0xF07F Read : X 0x707F Y 0xF07F                                                 
			NOP			W R						XDYD  				XD<XB^D2B	YD<YB^D2B; //Write : X 0x70FF Y 0xF0FF Read : X 0x70FF Y 0xF0FF                                                 
			NOP			W R						XDYD  				XD<XB^D2C	YD<YB^D2C; //Write : X 0x71FF Y 0xF1FF Read : X 0x71FF Y 0xF1FF                                                 
			NOP			W R						XDYD  				XD<XB^D2D	YD<YB^D2D; //Write : X 0x73FF Y 0xF3FF Read : X 0x73FF Y 0xF3FF                                                 
			NOP			W R						XDYD  				XD<XB^D3	YD<YB^D3 ; //Write : X 0x77FF Y 0xF7FF Read : X 0x77FF Y 0xF7FF                                                 
			NOP			W R						XDYD  				XD<XB^D4	YD<YB^D4 ; //Write : X 0x7FFF Y 0xFFFF Read : X 0x7FFF Y 0xFFFF                                                 
			NOP			W R						XDYD  									 ; //Write : X 0x6FFF Y 0xEFFF Read : X 0x6FFF Y 0xEFFF                                                 
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
    		STPS																		;
	END
	
			START XD_ALU_APImm_00;
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
			NOP		   XC<0x0000	 YC<0x8000													;                                                                  
			NOP																					;                                                 
			NOP														XD<XC+0x0001	YD<YC+0x0001;                                                 
			NOP			W R						XDYD  				XD<XC+0x0002	YD<YC+0x0002; //Write : X 0x0001 Y 0x8001 Read : X 0x0001 Y 0x8001                                                 
			NOP			W R						XDYD  				XD<XC+0x0004	YD<YC+0x0004; //Write : X 0x0003 Y 0x8003 Read : X 0x0003 Y 0x8003                                                 
			NOP			W R						XDYD  				XD<XC+0x0008	YD<YC+0x0008; //Write : X 0x0007 Y 0x8007 Read : X 0x0007 Y 0x8007                                                 
			NOP			W R						XDYD  				 							; //Write : X 0x000F Y 0x800F Read : X 0x000F Y 0x800F                                                 
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
    		STPS																				;
	END	

			START XD_ALU_APImm_01;
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
			NOP		   XS<0x0000	 YS<0x8000													;                                                                  
			NOP																					;                                                 
			NOP														XD<XS+0x0010	YD<YS+0x0010;                                                 
			NOP			W R						XDYD  				XD<XS+0x0020	YD<YS+0x0020; //Write : X 0x0010 Y 0x8010 Read : X 0x0010 Y 0x8010                                                 
			NOP			W R						XDYD  				XD<XS+0x0040	YD<YS+0x0040; //Write : X 0x0030 Y 0x8030 Read : X 0x0030 Y 0x8030                                                 
			NOP			W R						XDYD  				XD<XS+0x0080	YD<YS+0x0080; //Write : X 0x0070 Y 0x8070 Read : X 0x0070 Y 0x8070                                                 
			NOP			W R						XDYD  				 							; //Write : X 0x00F0 Y 0x80F0 Read : X 0x00F0 Y 0x80F0                                                 
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
    		STPS																				;
	END	

			START XD_ALU_APImm_02;
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
			NOP		   XK<0x0000	 YK<0x8000													;                                                                  
			NOP																					;                                                 
			NOP														XD<XK+0x0100	YD<YK+0x0100;                                                 
			NOP			W R						XDYD  				XD<XK+0x0200	YD<YK+0x0200; //Write : X 0x0100 Y 0x8100 Read : X 0x0100 Y 0x8100                                                 
			NOP			W R						XDYD  				XD<XK+0x0400	YD<YK+0x0400; //Write : X 0x0300 Y 0x8300 Read : X 0x0300 Y 0x8300                                                 
			NOP			W R						XDYD  				XD<XK+0x0800	YD<YK+0x0800; //Write : X 0x0700 Y 0x8700 Read : X 0x0700 Y 0x8700                                                 
			NOP			W R						XDYD  				 							; //Write : X 0x0F00 Y 0x8F00 Read : X 0x0F00 Y 0x8F00                                                 
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
    		STPS																				;
	END	

			START XD_ALU_APImm_03;
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
			NOP		   XA<0x0000	 YA<0x0001													;                                                                  
			NOP																					;                                                 
			NOP														XD<XA+0x1000	YD<YA+0x1000;                                                 
			NOP			W R						XDYD  				XD<XA+0x2000	YD<YA+0x2000; //Write : X 0x1000 Y 0x1001 Read : X 0x1000 Y 0x1001                                                 
			NOP			W R						XDYD  				XD<XA+0x4000	YD<YA+0x4000; //Write : X 0x3000 Y 0x3001 Read : X 0x3000 Y 0x3001                                                 
			NOP			W R						XDYD  				XD<XA+0x8000	YD<YA+0x8000; //Write : X 0x7000 Y 0x7001 Read : X 0x7000 Y 0x7001                                                 
			NOP			W R						XDYD  				 							; //Write : X 0xF000 Y 0xF001 Read : X 0xF000 Y 0xF001                                                 
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
    		STPS																				;
	END	

			START XD_ALU_APImm_04;
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
			NOP		   XD<0x0000	 YD<0x8000													;                                                                  
			NOP																					;                                                 
			NOP														XD<XD+0x000A	YD<YD+0x000A;                                                 
			NOP			W R						XDYD  				XD<XD+0x000B	YD<YD+0x000B; //Write : X 0x000A Y 0x800A Read : X 0x000A Y 0x800A                                                  
			NOP			W R						XDYD  				XD<XD+0x000C	YD<YD+0x000C; //Write : X 0x0015 Y 0x8015 Read : X 0x0015 Y 0x8015                                                  
			NOP			W R						XDYD  				XD<XD+0x000D	YD<YD+0x000D; //Write : X 0x0021 Y 0x8021 Read : X 0x0021 Y 0x8021                                                  
			NOP			W R						XDYD  				 							; //Write : X 0x002E Y 0x802E Read : X 0x002E Y 0x802E                                                  
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
    		STPS																				;
	END	

			START XD_ALU_APImm_05;
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
			NOP		   XE<0x0000	 YE<0x8000													;                                                                  
			NOP																					;                                                 
			NOP														XD<XE+0x00A0	YD<YE+0x00A0;                                                 
			NOP			W R						XDYD  				XD<XE+0x00B0	YD<YE+0x00B0; //Write : X 0x00A0 Y 0x80A0 Read : X 0x00A0 Y 0x80A0                                                 
			NOP			W R						XDYD  				XD<XE+0x00C0	YD<YE+0x00C0; //Write : X 0x0150 Y 0x8150 Read : X 0x0150 Y 0x8150                                                 
			NOP			W R						XDYD  				XD<XE+0x00D0	YD<YE+0x00D0; //Write : X 0x0210 Y 0x8210 Read : X 0x0210 Y 0x8210                                                 
			NOP			W R						XDYD  				 							; //Write : X 0x02E0 Y 0x82E0 Read : X 0x02E0 Y 0x82E0                                                 
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
    		STPS																				;
	END	

			START XD_ALU_APImm_06;
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
			NOP		   XF<0x0000	 YF<0x0001													;                                                                  
			NOP																					;                                                 
			NOP														XD<XF+0x0A00	YD<YF+0x0A00;                                                 
			NOP			W R						XDYD  				XD<XF+0x0B00	YD<YF+0x0B00; //Write : X 0x0A00 Y 0x0A01 Read : X 0x0A00 Y 0x0A01                                                 
			NOP			W R						XDYD  				XD<XF+0x0C00	YD<YF+0x0C00; //Write : X 0x1500 Y 0x1501 Read : X 0x1500 Y 0x1501                                                 
			NOP			W R						XDYD  				XD<XF+0x0D00	YD<YF+0x0D00; //Write : X 0x2100 Y 0x2101 Read : X 0x2100 Y 0x2101                                                 
			NOP			W R						XDYD  				 							; //Write : X 0x2E00 Y 0x2E01 Read : X 0x2E00 Y 0x2E01                                                 
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
    		STPS																				;
	END	

			START XD_ALU_APImm_07;
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
			NOP		   XB<0x0000	 YB<0x0000													;                                                                  
			NOP																					;                                                 
			NOP														XD<XB+0xA000	YD<YB+0xA000;                                                 
			NOP			W R						XDYD  				XD<XB+0x0B00	YD<YB+0x0B00; //Write : X 0xA000 Y 0xA000 Read : X 0xA000 Y 0xA000                                                 
			NOP			W R						XDYD  				XD<XB+0x00C0	YD<YB+0x00C0; //Write : X 0xAB00 Y 0xAB00 Read : X 0xAB00 Y 0xAB00                                                 
			NOP			W R						XDYD  				XD<XB+0x000D	YD<YB+0x000D; //Write : X 0xABC0 Y 0xABC0 Read : X 0xABC0 Y 0xABC0                                                 
			NOP			W R						XDYD  				 							; //Write : X 0xABCD Y 0xABCD Read : X 0xABCD Y 0xABCD                                                 
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
    		STPS																				;
	END	
	
			START XD_ALU_AMImm_00;
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
			NOP		   XC<0xF000	 YC<0x8000													;                                                                  
			NOP																					;                                                 
			NOP														XD<XC-0x0001	YD<YC-0x0001;                                                 
			NOP			W R						XDYD  				XD<XC-0x0002	YD<YC-0x0002; //Write : X 0xEFFF Y 0x7FFF Read : X 0xEFFF Y 0x7FFF                                                 
			NOP			W R						XDYD  				XD<XC-0x0004	YD<YC-0x0004; //Write : X 0xEFFD Y 0x7FFD Read : X 0xEFFD Y 0x7FFD                                                 
			NOP			W R						XDYD  				XD<XC-0x0008	YD<YC-0x0008; //Write : X 0xEFF9 Y 0x7FF9 Read : X 0xEFF9 Y 0x7FF9                                                 
			NOP			W R						XDYD  				 							; //Write : X 0xEFF1 Y 0x7FF1 Read : X 0xEFF1 Y 0x7FF1                                                 
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
    		STPS																				;
	END	

			START XD_ALU_AMImm_01;
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
			NOP		   XS<0xF000	 YS<0x8000													;                                                                  
			NOP																					;                                                 
			NOP														XD<XS-0x0010	YD<YS-0x0010;                                                 
			NOP			W R						XDYD  				XD<XS-0x0020	YD<YS-0x0020; //Write : X 0xEFF0 Y 0x7FF0 Read : X 0xEFF0 Y 0x7FF0                                                 
			NOP			W R						XDYD  				XD<XS-0x0040	YD<YS-0x0040; //Write : X 0xEFD0 Y 0x7FD0 Read : X 0xEFD0 Y 0x7FD0                                                 
			NOP			W R						XDYD  				XD<XS-0x0080	YD<YS-0x0080; //Write : X 0xEF90 Y 0x7F90 Read : X 0xEF90 Y 0x7F90                                                 
			NOP			W R						XDYD  				 							; //Write : X 0xEF10 Y 0x7F10 Read : X 0xEF10 Y 0x7F10                                                 
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
    		STPS																				;
	END	

			START XD_ALU_AMImm_02;
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
			NOP		   XK<0xF000	 YK<0x8000													;                                                                  
			NOP																					;                                                 
			NOP														XD<XK-0x0100	YD<YK-0x0100;                                                 
			NOP			W R						XDYD  				XD<XK-0x0200	YD<YK-0x0200; //Write : X 0xEF00 Y 0x7F00 Read : X 0xEF00 Y 0x7F00                                                 
			NOP			W R						XDYD  				XD<XK-0x0400	YD<YK-0x0400; //Write : X 0xED00 Y 0x7D00 Read : X 0xED00 Y 0x7D00                                                 
			NOP			W R						XDYD  				XD<XK-0x0800	YD<YK-0x0800; //Write : X 0xE900 Y 0x7900 Read : X 0xE900 Y 0x7900                                                 
			NOP			W R						XDYD  				 							; //Write : X 0xE100 Y 0x7100 Read : X 0xE100 Y 0x7100                                                 
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
    		STPS																				;
	END	

			START XD_ALU_AMImm_03;
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
			NOP		   XA<0xF000	 YA<0x8000													;                                                                  
			NOP																					;                                                 
			NOP														XD<XA-0x1000	YD<YA-0x1000;                                                 
			NOP			W R						XDYD  				XD<XA-0x0200	YD<YA-0x0200; //Write : X 0xE000 Y 0x7000 Read : X 0xE000 Y 0x7000                                                 
			NOP			W R						XDYD  				XD<XA-0x0040	YD<YA-0x0040; //Write : X 0xDE00 Y 0x6E00 Read : X 0xDE00 Y 0x6E00                                                 
			NOP			W R						XDYD  				XD<XA-0x0008	YD<YA-0x0008; //Write : X 0xDDC0 Y 0x6DC0 Read : X 0xDDC0 Y 0x6DC0                                                 
			NOP			W R						XDYD  				 							; //Write : X 0xDDB8 Y 0x6DB8 Read : X 0xDDB8 Y 0x6DB8                                                 
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
    		STPS																				;
	END	

			START XD_ALU_AMImm_04;
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
			NOP		   XD<0xF000	 YD<0x8000													;                                                                  
			NOP																					;                                                 
			NOP														XD<XD-0x000A	YD<YD-0x000A;                                                 
			NOP			W R						XDYD  				XD<XD-0x000B	YD<YD-0x000B; //Write : X 0xEFF6 Y 0x7FF6 Read : X 0xEFF6 Y 0x7FF6                                                 
			NOP			W R						XDYD  				XD<XD-0x000C	YD<YD-0x000C; //Write : X 0xEFEB Y 0x7FEB Read : X 0xEFEB Y 0x7FEB                                                 
			NOP			W R						XDYD  				XD<XD-0x000D	YD<YD-0x000D; //Write : X 0xEFDF Y 0x7FDF Read : X 0xEFDF Y 0x7FDF                                                 
			NOP			W R						XDYD  				 							; //Write : X 0xEFD2 Y 0x7FD2 Read : X 0xEFD2 Y 0x7FD2                                                 
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
    		STPS																				;
	END	

			START XD_ALU_AMImm_05;
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
			NOP		   XE<0xF000	 YE<0x8000													;                                                                  
			NOP																					;                                                 
			NOP														XD<XE-0x00A0	YD<YE-0x00A0;                                                 
			NOP			W R						XDYD  				XD<XE-0x00B0	YD<YE-0x00B0; //Write : X 0xEF60 Y 0x7F60 Read : X 0xEF60 Y 0x7F60                                                 
			NOP			W R						XDYD  				XD<XE-0x00C0	YD<YE-0x00C0; //Write : X 0xEEB0 Y 0x7EB0 Read : X 0xEEB0 Y 0x7EB0                                                 
			NOP			W R						XDYD  				XD<XE-0x00D0	YD<YE-0x00D0; //Write : X 0xEDF0 Y 0x7DF0 Read : X 0xEDF0 Y 0x7DF0                                                 
			NOP			W R						XDYD  				 							; //Write : X 0xED20 Y 0x7D20 Read : X 0xED20 Y 0x7D20                                                 
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
    		STPS																				;
	END	

			START XD_ALU_AMImm_06;
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
			NOP		   XF<0xF000	 YF<0x8000													;                                                                  
			NOP																					;                                                 
			NOP														XD<XF-0x0A00	YD<YF-0x0A00;                                                 
			NOP			W R						XDYD  				XD<XF-0x0B00	YD<YF-0x0B00; //Write : X 0xE600 Y 0x7600 Read : X 0xE600 Y 0x7600                                                 
			NOP			W R						XDYD  				XD<XF-0x0C00	YD<YF-0x0C00; //Write : X 0xDB00 Y 0x6B00 Read : X 0xDB00 Y 0x6B00                                                 
			NOP			W R						XDYD  				XD<XF-0x0D00	YD<YF-0x0D00; //Write : X 0xCF00 Y 0x5F00 Read : X 0xCF00 Y 0x5F00                                                 
			NOP			W R						XDYD  				 							; //Write : X 0xC200 Y 0x5200 Read : X 0xC200 Y 0x5200                                                 
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
    		STPS																				;
	END	

			START XD_ALU_AMImm_07;
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
			NOP		   XB<0xF000	 YB<0xE000													;                                                                  
			NOP																					;                                                 
			NOP														XD<XB-0xA000	YD<YB-0xA000;                                                 
			NOP			W R						XDYD  				XD<XB-0x0B00	YD<YB-0x0B00; //Write : X 0x5000 Y 0x4000 Read : X 0x5000 Y 0x4000                                                 
			NOP			W R						XDYD  				XD<XB-0x00C0	YD<YB-0x00C0; //Write : X 0x4500 Y 0x3500 Read : X 0x4500 Y 0x3500                                                 
			NOP			W R						XDYD  				XD<XB-0x000D	YD<YB-0x000D; //Write : X 0x4440 Y 0x3440 Read : X 0x4440 Y 0x3440                                                 
			NOP			W R						XDYD  				 							; //Write : X 0x4433 Y 0x3433 Read : X 0x4433 Y 0x3433                                                 
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
    		STPS																				;
	END																	
			START XD_ALU_D3D4_00;
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
			NOP																					 ;                                                 
			NOP														 							 ;                                                 
			NOP				        D3B<0x1111 D4B<0xAAAA										 ;                                                 
			NOP		   D3<D3B  D4<D4B												    		 ;                                                                  
			NOP														XD<D3 YD<D4					 ;                                                 
			NOP			W R						XDYD  				XD<D3 YD<D4	D3<0     D4<D4+1 ; //Write : X 0x1111 Y 0xAAAA Read : X 0x1111 Y 0xAAAA                                                 
			NOP			W R						XDYD  				XD<D3 YD<D4	D3<D3+1	 D4<0	 ; //Write : X 0x0000 Y 0xAAAb Read : X 0x0000 Y 0xAAAb                                                 
			NOP			W R						XDYD  				XD<D3 YD<D4	D3<D3+1	 D4<D4+1 ; //Write : X 0x0001 Y 0x0000 Read : X 0x0001 Y 0x0000                                                 
			NOP			W R						XDYD  				XD<D3 YD<D4	D3<D3-1	 D4<D4+1 ; //Write : X 0x0002 Y 0x0001 Read : X 0x0002 Y 0x0001                                                 
			NOP			W R						XDYD  				XD<D3 YD<D4	D3<D3+1	 D4<D4-1 ; //Write : X 0x0001 Y 0x0002 Read : X 0x0001 Y 0x0002                                                 
			NOP			W R						XDYD  				XD<D3 YD<D4	D3<D3*2	 D4<D4*2 ; //Write : X 0x0002 Y 0x0001 Read : X 0x0002 Y 0x0001                                                 
			NOP			W R						XDYD  				XD<D3 YD<D4	D3<D3*2	 D4<D4/2 ; //Write : X 0x0004 Y 0x0002 Read : X 0x0004 Y 0x0002                                                 
			NOP			W R						XDYD  				XD<D3 YD<D4	D3<D3/2	 D4<D4*2 ; //Write : X 0x0008 Y 0x0001 Read : X 0x0008 Y 0x0001                                                 
			NOP			W R						XDYD  				XD<D3 YD<D4	D3<D3*2	 D4<D4*2 ; //Write : X 0x0004 Y 0x0002 Read : X 0x0004 Y 0x0002                                                 
			NOP			W R						XDYD  				XD<D3 YD<D4	D3<D3B/2 D4<D4B/2; //Write : X 0x0008 Y 0x0004 Read : X 0x0008 Y 0x0004                                                 
			NOP			W R						XDYD  				XD<D3 YD<D4					 ; //Write : X 0x0888 Y 0x5555 Read : X 0x0888 Y 0x5555                                                 
			NOP			W R						XDYD  				XC<D3 YC<D4						; //0x0800 0x5000
			NOP			W R						XDYD  				XC<D3 YC<D4						; //0x0800 0x5000                                               
			NOP			W R						XDYD  				XC<D3 YC<D4						; //0x0800 0x5000
			NOP			W R						XDYD  				XC<D4 YC<D3						; //0x0800 0x5000                                               
			NOP			W R						XDYD											 ; //Write : X 0xAAAA Y 0x1111 Read : X 0xAAAA Y 0x1111                                     
			NOP																					 ;                                     
			NOP																					 ;                                     
			NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
            NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                  
   		STPS																				 ;
	END 

