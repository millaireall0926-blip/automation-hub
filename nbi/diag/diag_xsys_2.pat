
%IFE A .AND. B .AND.((C.OR.D).AND.E)
%INSERT Header/define.h
  
 REGISTER
			XMAX=0xffffff;
			YMAX=0xffffff;
			ZMAX=0xff;
			XOS=0x0;
			YOS=0x0;	
ENDREGISTER	
				
			START XS_ALU_AORB_00;
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
			NOP														XS<XC|D1A	YS<YC|D1A;                                                 
			NOP			W R						XSYS  				XS<XC|D1B	YS<YC|D1B; //Write : X 0x0000 Y 0x8000 Read : X 0x0000 Y 0x8000                                                 
			NOP			W R						XSYS  				XS<XC|D1C	YS<YC|D1C; //Write : X 0x0001 Y 0x8001 Read : X 0x0001 Y 0x8001                                                 
			NOP			W R						XSYS  				XS<XC|D1D	YS<YC|D1D; //Write : X 0x0003 Y 0x8003 Read : X 0x0003 Y 0x8003                                                 
			NOP			W R						XSYS  				XS<XC|D1E	YS<YC|D1E; //Write : X 0x0007 Y 0x8007 Read : X 0x0007 Y 0x8007                                                 
			NOP			W R						XSYS  				XS<XC|D1F	YS<YC|D1F; //Write : X 0x000F Y 0x800F Read : X 0x000F Y 0x800F                                                 
			NOP			W R						XSYS  				XS<XC|D1G	YS<YC|D1G; //Write : X 0x001F Y 0x801F Read : X 0x001F Y 0x801F                                                 
			NOP			W R						XSYS  				XS<XC|D1H	YS<YC|D1H; //Write : X 0x003F Y 0x803F Read : X 0x003F Y 0x803F                                                 
			NOP			W R						XSYS  				XS<XC|D2A	YS<YC|D2A; //Write : X 0x007F Y 0x807F Read : X 0x007F Y 0x807F                                                 
			NOP			W R						XSYS  				XS<XC|D2B	YS<YC|D2B; //Write : X 0x00FF Y 0x80FF Read : X 0x00FF Y 0x80FF                                                 
			NOP			W R						XSYS  				XS<XC|D2C	YS<YC|D2C; //Write : X 0x01FF Y 0x81FF Read : X 0x01FF Y 0x81FF                                                 
			NOP			W R						XSYS  				XS<XC|D2D	YS<YC|D2D; //Write : X 0x03FF Y 0x83FF Read : X 0x03FF Y 0x83FF                                                 
			NOP			W R						XSYS  				XS<XC|D3	YS<YC|D3 ; //Write : X 0x07FF Y 0x87FF Read : X 0x07FF Y 0x87FF                                                 
			NOP			W R						XSYS  				XS<XC|D4	YS<YC|D4 ; //Write : X 0x0FFF Y 0x8FFF Read : X 0x0FFF Y 0x8FFF                                                 
			NOP			W R						XSYS  									 ; //Write : X 0x1FFF Y 0x9FFF Read : X 0x1FFF Y 0x9FFF                                                 
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
						
			START XS_ALU_AORB_01;
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
			NOP														XS<XS|D1A	YS<YS|D1A;                                                 
			NOP			W R						XSYS  				XS<XS|D1B	YS<YS|D1B; //Write : X 0x1000 Y 0x9000 Read : X 0x1000 Y 0x9000                                                 
			NOP			W R						XSYS  				XS<XS|D1C	YS<YS|D1C; //Write : X 0x1001 Y 0x9001 Read : X 0x1001 Y 0x9001                                                 
			NOP			W R						XSYS  				XS<XS|D1D	YS<YS|D1D; //Write : X 0x1003 Y 0x9003 Read : X 0x1003 Y 0x9003                                                 
			NOP			W R						XSYS  				XS<XS|D1E	YS<YS|D1E; //Write : X 0x1007 Y 0x9007 Read : X 0x1007 Y 0x9007                                                 
			NOP			W R						XSYS  				XS<XS|D1F	YS<YS|D1F; //Write : X 0x100F Y 0x900F Read : X 0x100F Y 0x900F                                                 
			NOP			W R						XSYS  				XS<XS|D1G	YS<YS|D1G; //Write : X 0x101F Y 0x901F Read : X 0x101F Y 0x901F                                                 
			NOP			W R						XSYS  				XS<XS|D1H	YS<YS|D1H; //Write : X 0x103F Y 0x903F Read : X 0x103F Y 0x903F                                                 
			NOP			W R						XSYS  				XS<XS|D2A	YS<YS|D2A; //Write : X 0x107F Y 0x907F Read : X 0x107F Y 0x907F                                                 
			NOP			W R						XSYS  				XS<XS|D2B	YS<YS|D2B; //Write : X 0x10FF Y 0x90FF Read : X 0x10FF Y 0x90FF                                                 
			NOP			W R						XSYS  				XS<XS|D2C	YS<YS|D2C; //Write : X 0x11FF Y 0x91FF Read : X 0x11FF Y 0x91FF                                                 
			NOP			W R						XSYS  				XS<XS|D2D	YS<YS|D2D; //Write : X 0x13FF Y 0x93FF Read : X 0x13FF Y 0x93FF                                                 
			NOP			W R						XSYS  				XS<XS|D3	YS<YS|D3 ; //Write : X 0x17FF Y 0x97FF Read : X 0x17FF Y 0x97FF                                                 
			NOP			W R						XSYS  				XS<XS|D4	YS<YS|D4 ; //Write : X 0x1FFF Y 0x9FFF Read : X 0x1FFF Y 0x9FFF                                                 
			NOP			W R						XSYS  									 ; //Write : X 0x1FFF Y 0x9FFF Read : X 0x1FFF Y 0x9FFF                                                 
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
						
			START XS_ALU_AORB_02;
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
			NOP														XS<XK|D1A	YS<YK|D1A;                                                 
			NOP			W R						XSYS  				XS<XK|D1B	YS<YK|D1B; //Write : X 0x2000 Y 0xA000 Read : X 0x2000 Y 0xA000                                                 
			NOP			W R						XSYS  				XS<XK|D1C	YS<YK|D1C; //Write : X 0x2001 Y 0xA001 Read : X 0x2001 Y 0xA001                                                 
			NOP			W R						XSYS  				XS<XK|D1D	YS<YK|D1D; //Write : X 0x2003 Y 0xA003 Read : X 0x2003 Y 0xA003                                                 
			NOP			W R						XSYS  				XS<XK|D1E	YS<YK|D1E; //Write : X 0x2007 Y 0xA007 Read : X 0x2007 Y 0xA007                                                 
			NOP			W R						XSYS  				XS<XK|D1F	YS<YK|D1F; //Write : X 0x200F Y 0xA00F Read : X 0x200F Y 0xA00F                                                 
			NOP			W R						XSYS  				XS<XK|D1G	YS<YK|D1G; //Write : X 0x201F Y 0xA01F Read : X 0x201F Y 0xA01F                                                 
			NOP			W R						XSYS  				XS<XK|D1H	YS<YK|D1H; //Write : X 0x203F Y 0xA03F Read : X 0x203F Y 0xA03F                                                 
			NOP			W R						XSYS  				XS<XK|D2A	YS<YK|D2A; //Write : X 0x207F Y 0xA07F Read : X 0x207F Y 0xA07F                                                 
			NOP			W R						XSYS  				XS<XK|D2B	YS<YK|D2B; //Write : X 0x20FF Y 0xA0FF Read : X 0x20FF Y 0xA0FF                                                 
			NOP			W R						XSYS  				XS<XK|D2C	YS<YK|D2C; //Write : X 0x21FF Y 0xA1FF Read : X 0x21FF Y 0xA1FF                                                 
			NOP			W R						XSYS  				XS<XK|D2D	YS<YK|D2D; //Write : X 0x23FF Y 0xA3FF Read : X 0x23FF Y 0xA3FF                                                 
			NOP			W R						XSYS  				XS<XK|D3	YS<YK|D3 ; //Write : X 0x27FF Y 0xA7FF Read : X 0x27FF Y 0xA7FF                                                 
			NOP			W R						XSYS  				XS<XK|D4	YS<YK|D4 ; //Write : X 0x2FFF Y 0xAFFF Read : X 0x2FFF Y 0xAFFF                                                 
			NOP			W R						XSYS  									 ; //Write : X 0x3FFF Y 0xBFFF Read : X 0x3FFF Y 0xBFFF                                                 
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
						
			START XS_ALU_AORB_03;
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
			NOP														XS<XA|D1A	YS<YA|D1A;                                                 
			NOP			W R						XSYS  				XS<XA|D1B	YS<YA|D1B; //Write : X 0x3000 Y 0xB000 Read : X 0x3000 Y 0xB000                                                 
			NOP			W R						XSYS  				XS<XA|D1C	YS<YA|D1C; //Write : X 0x3001 Y 0xB001 Read : X 0x3001 Y 0xB001                                                 
			NOP			W R						XSYS  				XS<XA|D1D	YS<YA|D1D; //Write : X 0x3003 Y 0xB003 Read : X 0x3003 Y 0xB003                                                 
			NOP			W R						XSYS  				XS<XA|D1E	YS<YA|D1E; //Write : X 0x3007 Y 0xB007 Read : X 0x3007 Y 0xB007                                                 
			NOP			W R						XSYS  				XS<XA|D1F	YS<YA|D1F; //Write : X 0x300F Y 0xB00F Read : X 0x300F Y 0xB00F                                                 
			NOP			W R						XSYS  				XS<XA|D1G	YS<YA|D1G; //Write : X 0x301F Y 0xB01F Read : X 0x301F Y 0xB01F                                                 
			NOP			W R						XSYS  				XS<XA|D1H	YS<YA|D1H; //Write : X 0x303F Y 0xB03F Read : X 0x303F Y 0xB03F                                                 
			NOP			W R						XSYS  				XS<XA|D2A	YS<YA|D2A; //Write : X 0x307F Y 0xB07F Read : X 0x307F Y 0xB07F                                                 
			NOP			W R						XSYS  				XS<XA|D2B	YS<YA|D2B; //Write : X 0x30FF Y 0xB0FF Read : X 0x30FF Y 0xB0FF                                                 
			NOP			W R						XSYS  				XS<XA|D2C	YS<YA|D2C; //Write : X 0x31FF Y 0xB1FF Read : X 0x31FF Y 0xB1FF                                                 
			NOP			W R						XSYS  				XS<XA|D2D	YS<YA|D2D; //Write : X 0x33FF Y 0xB3FF Read : X 0x33FF Y 0xB3FF                                                 
			NOP			W R						XSYS  				XS<XA|D3	YS<YA|D3 ; //Write : X 0x37FF Y 0xB7FF Read : X 0x37FF Y 0xB7FF                                                 
			NOP			W R						XSYS  				XS<XA|D4	YS<YA|D4 ; //Write : X 0x3FFF Y 0xBFFF Read : X 0x3FFF Y 0xBFFF                                                 
			NOP			W R						XSYS  									 ; //Write : X 0x3FFF Y 0xBFFF Read : X 0x3FFF Y 0xBFFF                                              
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
						
			START XS_ALU_AORB_04;
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

			NOP		   XC<0x0000	YC<0x8000														;                                                                  
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
			NOP														XS<XD|D1A	YS<YD|D1A			;                                                 
			NOP			W R						XSYS  				XS<XD|D1B	YS<YD|D1B			; //Write : X 0x4000 Y 0xC000 Read : X 0x4000 Y 0xC000                                                 
			NOP			W R						XSYS  				XS<XD|D1C	YS<YD|D1C			; //Write : X 0x4001 Y 0xC001 Read : X 0x4001 Y 0xC001                                                 
			NOP			W R						XSYS  				XS<XD|D1D	YS<YD|D1D			; //Write : X 0x4003 Y 0xC003 Read : X 0x4003 Y 0xC003                                                 
			NOP			W R						XSYS  				XS<XD|D1E	YS<YD|D1E			; //Write : X 0x4007 Y 0xC007 Read : X 0x4007 Y 0xC007                                                 
			NOP			W R						XSYS  				XS<XD|D1F	YS<YD|D1F			; //Write : X 0x400F Y 0xC00F Read : X 0x400F Y 0xC00F                                                 
			NOP			W R						XSYS  				XS<XD|D1G	YS<YD|D1G			; //Write : X 0x401F Y 0xC01F Read : X 0x401F Y 0xC01F                                                 
			NOP			W R						XSYS  				XS<XD|D1H	YS<YD|D1H			; //Write : X 0x403F Y 0xC03F Read : X 0x403F Y 0xC03F                                                 
			NOP			W R						XSYS  				XS<XD|D2A	YS<YD|D2A			; //Write : X 0x407F Y 0xC07F Read : X 0x407F Y 0xC07F                                                 
			NOP			W R						XSYS  				XS<XD|D2B	YS<YD|D2B			; //Write : X 0x40FF Y 0xC0FF Read : X 0x40FF Y 0xC0FF                                                 
			NOP			W R						XSYS  				XS<XD|D2C	YS<YD|D2C			; //Write : X 0x41FF Y 0xC1FF Read : X 0x41FF Y 0xC1FF                                                 
			NOP			W R						XSYS  				XS<XD|D2D	YS<YD|D2D			; //Write : X 0x43FF Y 0xC3FF Read : X 0x43FF Y 0xC3FF                                                 
			NOP			W R						XSYS  				XS<XD|D3	YS<YD|D3 			; //Write : X 0x47FF Y 0xC7FF Read : X 0x47FF Y 0xC7FF                                                 
			NOP			W R						XSYS  				XS<XD|D4	YS<YD|D4 			; //Write : X 0x4FFF Y 0xCFFF Read : X 0x4FFF Y 0xCFFF                                                 
			NOP			W R						XSYS  									 			; //Write : X 0x5FFF Y 0xDFFF Read : X 0x5FFF Y 0xDFFF                                                 
			NOP																						;                                     
			NOP																						;                                     
			NOP																						;                                     

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
    		STPS																					;
	END
						
			START XS_ALU_AORB_05;
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

			NOP		   XC<0x0000	YC<0x8000														;                                                                  
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
			NOP														XS<XE|D1A	YS<YE|D1A			;                                                 
			NOP			W R						XSYS  				XS<XE|D1B	YS<YE|D1B			; //Write : X 0x5000 Y 0xD000 Read : X 0x5000 Y 0xD000                                                 
			NOP			W R						XSYS  				XS<XE|D1C	YS<YE|D1C			; //Write : X 0x5001 Y 0xD001 Read : X 0x5001 Y 0xD001                                                 
			NOP			W R						XSYS  				XS<XE|D1D	YS<YE|D1D			; //Write : X 0x5003 Y 0xD003 Read : X 0x5003 Y 0xD003                                                 
			NOP			W R						XSYS  				XS<XE|D1E	YS<YE|D1E			; //Write : X 0x5007 Y 0xD007 Read : X 0x5007 Y 0xD007                                                 
			NOP			W R						XSYS  				XS<XE|D1F	YS<YE|D1F			; //Write : X 0x500F Y 0xD00F Read : X 0x500F Y 0xD00F                                                 
			NOP			W R						XSYS  				XS<XE|D1G	YS<YE|D1G			; //Write : X 0x501F Y 0xD01F Read : X 0x501F Y 0xD01F                                                 
			NOP			W R						XSYS  				XS<XE|D1H	YS<YE|D1H			; //Write : X 0x503F Y 0xD03F Read : X 0x503F Y 0xD03F                                                 
			NOP			W R						XSYS  				XS<XE|D2A	YS<YE|D2A			; //Write : X 0x507F Y 0xD07F Read : X 0x507F Y 0xD07F                                                 
			NOP			W R						XSYS  				XS<XE|D2B	YS<YE|D2B			; //Write : X 0x50FF Y 0xD0FF Read : X 0x50FF Y 0xD0FF                                                 
			NOP			W R						XSYS  				XS<XE|D2C	YS<YE|D2C			; //Write : X 0x51FF Y 0xD1FF Read : X 0x51FF Y 0xD1FF                                                 
			NOP			W R						XSYS  				XS<XE|D2D	YS<YE|D2D			; //Write : X 0x53FF Y 0xD3FF Read : X 0x53FF Y 0xD3FF                                                 
			NOP			W R						XSYS  				XS<XE|D3	YS<YE|D3 			; //Write : X 0x57FF Y 0xD7FF Read : X 0x57FF Y 0xD7FF                                                 
			NOP			W R						XSYS  				XS<XE|D4	YS<YE|D4 			; //Write : X 0x5FFF Y 0xDFFF Read : X 0x5FFF Y 0xDFFF                                                 
			NOP			W R						XSYS  									 			; //Write : X 0x5FFF Y 0xDFFF Read : X 0x5FFF Y 0xDFFF                                                 
			NOP																			 			;                                     
			NOP																			 			;                                     

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
    		STPS																		 			;
	END
						
			START XS_ALU_AORB_06;
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

			NOP		   XC<0x0000	YC<0x8000											 			;                                                                  
			NOP		   XS<0x1000	YS<0x9000											 			;                                                                  
			NOP		   XK<0x2000	YK<0xA000											 			;                                                                  
			NOP		   XA<0x3000	YA<0xB000											 			;                                                                  
			NOP		   XD<0x4000	YD<0xC000											 			;                                                                  
			NOP		   XE<0x5000	YE<0xD000		 									 			;                                                                  
			NOP		   XF<0x6000	YF<0xE000											 			;                                                                  
			NOP		   XB<0x7000	YB<0xF000											 			;                                                                  
			NOP		  D1A<0x0000	D4B<0x1000											 			;                                                                  
			NOP		  D1B<0x0001														 			;                                                                  
			NOP		  D1C<0x0002														 			;                                                                  
			NOP		  D1D<0x0004														 			;                                                                  
			NOP		  D1E<0x0008														 			;                                                                  
			NOP		  D1F<0x0010			 											 			;                                                                  
			NOP		  D1G<0x0020														 			;                                                                  
			NOP		  D1H<0x0040														 			;                                                                  
			NOP		  D2A<0x0080	D3B<0x0800											 			;                                                                  
			NOP		  D2B<0x0100														 			;                                                                  
			NOP		  D2C<0x0200														 			;                                                                  
			NOP		  D2D<0x0400														 			;                                                                  
			NOP		   D3<D3B															 			;                                                                  
			NOP		   D4<D4B															 			;                                                                  
			NOP																			 			;                                                 
			NOP														XS<XF|D1A	YS<YF|D1A			;                                                 
			NOP			W R						XSYS  				XS<XF|D1B	YS<YF|D1B			; //Write : X 0x6000 Y 0xE000 Read : X 0x6000 Y 0xE000                                                 
			NOP			W R						XSYS  				XS<XF|D1C	YS<YF|D1C			; //Write : X 0x6001 Y 0xE001 Read : X 0x6001 Y 0xE001                                                 
			NOP			W R						XSYS  				XS<XF|D1D	YS<YF|D1D			; //Write : X 0x6003 Y 0xE003 Read : X 0x6003 Y 0xE003                                                 
			NOP			W R						XSYS  				XS<XF|D1E	YS<YF|D1E			; //Write : X 0x6007 Y 0xE007 Read : X 0x6007 Y 0xE007                                                 
			NOP			W R						XSYS  				XS<XF|D1F	YS<YF|D1F			; //Write : X 0x600F Y 0xE00F Read : X 0x600F Y 0xE00F                                                 
			NOP			W R						XSYS  				XS<XF|D1G	YS<YF|D1G			; //Write : X 0x601F Y 0xE01F Read : X 0x601F Y 0xE01F                                                 
			NOP			W R						XSYS  				XS<XF|D1H	YS<YF|D1H			; //Write : X 0x603F Y 0xE03F Read : X 0x603F Y 0xE03F                                                 
			NOP			W R						XSYS  				XS<XF|D2A	YS<YF|D2A			; //Write : X 0x607F Y 0xE07F Read : X 0x607F Y 0xE07F                                                 
			NOP			W R						XSYS  				XS<XF|D2B	YS<YF|D2B			; //Write : X 0x60FF Y 0xE0FF Read : X 0x60FF Y 0xE0FF                                                 
			NOP			W R						XSYS  				XS<XF|D2C	YS<YF|D2C			; //Write : X 0x61FF Y 0xE1FF Read : X 0x61FF Y 0xE1FF                                                 
			NOP			W R						XSYS  				XS<XF|D2D	YS<YF|D2D			; //Write : X 0x63FF Y 0xE3FF Read : X 0x63FF Y 0xE3FF                                                 
			NOP			W R						XSYS  				XS<XF|D3	YS<YF|D3 			; //Write : X 0x67FF Y 0xE7FF Read : X 0x67FF Y 0xE7FF                                                 
			NOP			W R						XSYS  				XS<XF|D4	YS<YF|D4 			; //Write : X 0x6FFF Y 0xEFFF Read : X 0x6FFF Y 0xEFFF                                                 
			NOP			W R						XSYS  									 			; //Write : X 0x7FFF Y 0xFFFF Read : X 0x7FFF Y 0xFFFF                                                 
			NOP																			 			;                                     
			NOP																			 			;                                     
			NOP																			 			;                                     

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
    		STPS																		 			;
	END
						
			START XS_ALU_AORB_07;
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

			NOP		   XC<0x0000	YC<0x8000											 			;                                                                  
			NOP		   XS<0x1000	YS<0x9000											 			;                                                                  
			NOP		   XK<0x2000	YK<0xA000											 			;                                                                  
			NOP		   XA<0x3000	YA<0xB000											 			;                                                                  
			NOP		   XD<0x4000	YD<0xC000											 			;                                                                  
			NOP		   XE<0x5000	YE<0xD000		 									 			;                                                                  
			NOP		   XF<0x6000	YF<0xE000											 			;                                                                  
			NOP		   XB<0x7000	YB<0xF000											 			;                                                                  
			NOP		  D1A<0x0000	D4B<0x1000											 			;                                                                  
			NOP		  D1B<0x0001														 			;                                                                  
			NOP		  D1C<0x0002														 			;                                                                  
			NOP		  D1D<0x0004														 			;                                                                  
			NOP		  D1E<0x0008														 			;                                                                  
			NOP		  D1F<0x0010			 											 			;                                                                  
			NOP		  D1G<0x0020														 			;                                                                  
			NOP		  D1H<0x0040														 			;                                                                  
			NOP		  D2A<0x0080	D3B<0x0800											 			;                                                                  
			NOP		  D2B<0x0100														 			;                                                                  
			NOP		  D2C<0x0200														 			;                                                                  
			NOP		  D2D<0x0400														 			;                                                                  
			NOP		   D3<D3B															 			;                                                                  
			NOP		   D4<D4B															 			;                                                                  
			NOP																			 			;                                                 
			NOP														XS<XB|D1A	YS<YB|D1A			;                                                 
			NOP			W R						XSYS  				XS<XB|D1B	YS<YB|D1B			; //Write : X 0x7000 Y 0xF000 Read : X 0x7000 Y 0xF000                                                 
			NOP			W R						XSYS  				XS<XB|D1C	YS<YB|D1C			; //Write : X 0x7001 Y 0xF001 Read : X 0x7001 Y 0xF001                                                 
			NOP			W R						XSYS  				XS<XB|D1D	YS<YB|D1D			; //Write : X 0x7003 Y 0xF003 Read : X 0x7003 Y 0xF003                                                 
			NOP			W R						XSYS  				XS<XB|D1E	YS<YB|D1E			; //Write : X 0x7007 Y 0xF007 Read : X 0x7007 Y 0xF007                                                 
			NOP			W R						XSYS  				XS<XB|D1F	YS<YB|D1F			; //Write : X 0x700F Y 0xF00F Read : X 0x700F Y 0xF00F                                                 
			NOP			W R						XSYS  				XS<XB|D1G	YS<YB|D1G			; //Write : X 0x701F Y 0xF01F Read : X 0x701F Y 0xF01F                                                 
			NOP			W R						XSYS  				XS<XB|D1H	YS<YB|D1H			; //Write : X 0x703F Y 0xF03F Read : X 0x703F Y 0xF03F                                                 
			NOP			W R						XSYS  				XS<XB|D2A	YS<YB|D2A			; //Write : X 0x707F Y 0xF07F Read : X 0x707F Y 0xF07F                                                 
			NOP			W R						XSYS  				XS<XB|D2B	YS<YB|D2B			; //Write : X 0x70FF Y 0xF0FF Read : X 0x70FF Y 0xF0FF                                                 
			NOP			W R						XSYS  				XS<XB|D2C	YS<YB|D2C			; //Write : X 0x71FF Y 0xF1FF Read : X 0x71FF Y 0xF1FF                                                 
			NOP			W R						XSYS  				XS<XB|D2D	YS<YB|D2D			; //Write : X 0x73FF Y 0xF3FF Read : X 0x73FF Y 0xF3FF                                                 
			NOP			W R						XSYS  				XS<XB|D3	YS<YB|D3 			; //Write : X 0x77FF Y 0xF7FF Read : X 0x77FF Y 0xF7FF                                                 
			NOP			W R						XSYS  				XS<XB|D4	YS<YB|D4 			; //Write : X 0x7FFF Y 0xFFFF Read : X 0x7FFF Y 0xFFFF                                                 
			NOP			W R						XSYS  									 			; //Write : X 0x7FFF Y 0xFFFF Read : X 0x7FFF Y 0xFFFF                                                 
			NOP																						;                                     
			NOP																						;                                     

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
    		STPS																					;
	END

			START XS_ALU_AXORB_00;
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
			NOP														XS<XC^D1A	YS<YC^D1A			;                                                 
			NOP			W R						XSYS  				XS<XC^D1B	YS<YC^D1B			; //Write : X 0xFFFF Y 0x0000 Read : X 0xFFFF Y 0x0000                                                 
			NOP			W R						XSYS  				XS<XC^D1C	YS<YC^D1C			; //Write : X 0xFFFE Y 0x0001 Read : X 0xFFFE Y 0x0001                                                 
			NOP			W R						XSYS  				XS<XC^D1D	YS<YC^D1D			; //Write : X 0xFFFC Y 0x0003 Read : X 0xFFFC Y 0x0003                                                 
			NOP			W R						XSYS  				XS<XC^D1E	YS<YC^D1E			; //Write : X 0xFFF8 Y 0x0007 Read : X 0xFFF8 Y 0x0007                                                 
			NOP			W R						XSYS  				XS<XC^D1F	YS<YC^D1F			; //Write : X 0xFFF0 Y 0x000F Read : X 0xFFF0 Y 0x000F                                                 
			NOP			W R						XSYS  				XS<XC^D1G	YS<YC^D1G			; //Write : X 0xFFE0 Y 0x001F Read : X 0xFFE0 Y 0x001F                                                 
			NOP			W R						XSYS  				XS<XC^D1H	YS<YC^D1H			; //Write : X 0xFFC0 Y 0x003F Read : X 0xFFC0 Y 0x003F                                                 
			NOP			W R						XSYS  				XS<XC^D2A	YS<YC^D2A			; //Write : X 0xFF80 Y 0x007F Read : X 0xFF80 Y 0x007F                                                 
			NOP			W R						XSYS  				XS<XC^D2B	YS<YC^D2B			; //Write : X 0xFF00 Y 0x00FF Read : X 0xFF00 Y 0x00FF                                                 
			NOP			W R						XSYS  				XS<XC^D2C	YS<YC^D2C			; //Write : X 0xFE00 Y 0x01FF Read : X 0xFE00 Y 0x01FF                                                 
			NOP			W R						XSYS  				XS<XC^D2D	YS<YC^D2D			; //Write : X 0xFC00 Y 0x03FF Read : X 0xFC00 Y 0x03FF                                                 
			NOP			W R						XSYS  				XS<XC^D3	YS<YC^D3 			; //Write : X 0xF800 Y 0x07FF Read : X 0xF800 Y 0x07FF                                                 
			NOP			W R						XSYS  				XS<XC^D4	YS<YC^D4 			; //Write : X 0xF000 Y 0x0FFF Read : X 0xF000 Y 0x0FFF                                                 
			NOP			W R						XSYS  									 			; //Write : X 0xE000 Y 0x1FFF Read : X 0xE000 Y 0x1FFF                                                 
			NOP																						;                                     
			NOP																						;                                     

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
    		STPS																					;
	END
																		
			START XS_ALU_AXORB_01;
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

			NOP		   XC<0xFFFF	YC<0x0000											 			;                                                                  
			NOP		   XS<0x1000	YS<0x9000											 			;                                                                  
			NOP		   XK<0x2000	YK<0xA000											 			;                                                                  
			NOP		   XA<0x3000	YA<0xB000											 			;                                                                  
			NOP		   XD<0x4000	YD<0xC000											 			;                                                                  
			NOP		   XE<0x5000	YE<0xD000		 									 			;                                                                  
			NOP		   XF<0x6000	YF<0xE000											 			;                                                                  
			NOP		   XB<0x7000	YB<0xF000											 			;                                                                  
			NOP		  D1A<0x0000	D4B<0x1000											 			;                                                                  
			NOP		  D1B<0x0001														 			;                                                                  
			NOP		  D1C<0x0002														 			;                                                                  
			NOP		  D1D<0x0004														 			;                                                                  
			NOP		  D1E<0x0008														 			;                                                                  
			NOP		  D1F<0x0010			 											 			;                                                                  
			NOP		  D1G<0x0020														 			;                                                                  
			NOP		  D1H<0x0040														 			;                                                                  
			NOP		  D2A<0x0080	D3B<0x0800											 			;                                                                  
			NOP		  D2B<0x0100														 			;                                                                  
			NOP		  D2C<0x0200														 			;                                                                  
			NOP		  D2D<0x0400														 			;                                                                  
			NOP		   D3<D3B															 			;                                                                  
			NOP		   D4<D4B															 			;                                                                  
			NOP																			 			;                                                 
			NOP														XS<XS^D1A	YS<YS^D1A			;                                                 
			NOP			W R						XSYS  				XS<XS^D1B	YS<YS^D1B			; //Write : X 0x1000 Y 0x9000 Read : X 0x1000 Y 0x9000                                                 
			NOP			W R						XSYS  				XS<XS^D1C	YS<YS^D1C			; //Write : X 0x1001 Y 0x9001 Read : X 0x1001 Y 0x9001                                                 
			NOP			W R						XSYS  				XS<XS^D1D	YS<YS^D1D			; //Write : X 0x1003 Y 0x9003 Read : X 0x1003 Y 0x9003                                                 
			NOP			W R						XSYS  				XS<XS^D1E	YS<YS^D1E			; //Write : X 0x1007 Y 0x9007 Read : X 0x1007 Y 0x9007                                                 
			NOP			W R						XSYS  				XS<XS^D1F	YS<YS^D1F			; //Write : X 0x100F Y 0x900F Read : X 0x100F Y 0x900F                                                 
			NOP			W R						XSYS  				XS<XS^D1G	YS<YS^D1G			; //Write : X 0x101F Y 0x901F Read : X 0x101F Y 0x901F                                                 
			NOP			W R						XSYS  				XS<XS^D1H	YS<YS^D1H			; //Write : X 0x103F Y 0x903F Read : X 0x103F Y 0x903F                                                 
			NOP			W R						XSYS  				XS<XS^D2A	YS<YS^D2A			; //Write : X 0x107F Y 0x907F Read : X 0x107F Y 0x907F                                                 
			NOP			W R						XSYS  				XS<XS^D2B	YS<YS^D2B			; //Write : X 0x10FF Y 0x90FF Read : X 0x10FF Y 0x90FF                                                 
			NOP			W R						XSYS  				XS<XS^D2C	YS<YS^D2C			; //Write : X 0x11FF Y 0x91FF Read : X 0x11FF Y 0x91FF                                                 
			NOP			W R						XSYS  				XS<XS^D2D	YS<YS^D2D			; //Write : X 0x13FF Y 0x93FF Read : X 0x13FF Y 0x93FF                                                 
			NOP			W R						XSYS  				XS<XS^D3	YS<YS^D3 			; //Write : X 0x17FF Y 0x97FF Read : X 0x17FF Y 0x97FF                                                 
			NOP			W R						XSYS  				XS<XS^D4	YS<YS^D4 			; //Write : X 0x1FFF Y 0x9FFF Read : X 0x1FFF Y 0x9FFF                                                 
			NOP			W R						XSYS  									 			; //Write : X 0x0FFF Y 0x8FFF Read : X 0x0FFF Y 0x8FFF                                                 
			NOP																						;                                     
			NOP																						;                                     
			NOP																						;                                     

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
    		STPS																					;
	END		
																		
			START XS_ALU_AXORB_02;
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
			NOP														XS<XK^D1A	YS<YK^D1A			;                                                 
			NOP			W R						XSYS  				XS<XK^D1B	YS<YK^D1B			; //Write : X 0x2000 Y 0xA000 Read : X 0x2000 Y 0xA000                                                 
			NOP			W R						XSYS  				XS<XK^D1C	YS<YK^D1C			; //Write : X 0x2001 Y 0xA001 Read : X 0x2001 Y 0xA001                                                 
			NOP			W R						XSYS  				XS<XK^D1D	YS<YK^D1D			; //Write : X 0x2003 Y 0xA003 Read : X 0x2003 Y 0xA003                                                 
			NOP			W R						XSYS  				XS<XK^D1E	YS<YK^D1E			; //Write : X 0x2007 Y 0xA007 Read : X 0x2007 Y 0xA007                                                 
			NOP			W R						XSYS  				XS<XK^D1F	YS<YK^D1F			; //Write : X 0x200F Y 0xA00F Read : X 0x200F Y 0xA00F                                                 
			NOP			W R						XSYS  				XS<XK^D1G	YS<YK^D1G			; //Write : X 0x201F Y 0xA01F Read : X 0x201F Y 0xA01F                                                 
			NOP			W R						XSYS  				XS<XK^D1H	YS<YK^D1H			; //Write : X 0x203F Y 0xA03F Read : X 0x203F Y 0xA03F                                                 
			NOP			W R						XSYS  				XS<XK^D2A	YS<YK^D2A			; //Write : X 0x207F Y 0xA07F Read : X 0x207F Y 0xA07F                                                 
			NOP			W R						XSYS  				XS<XK^D2B	YS<YK^D2B			; //Write : X 0x20FF Y 0xA0FF Read : X 0x20FF Y 0xA0FF                                                 
			NOP			W R						XSYS  				XS<XK^D2C	YS<YK^D2C			; //Write : X 0x21FF Y 0xA1FF Read : X 0x21FF Y 0xA1FF                                                 
			NOP			W R						XSYS  				XS<XK^D2D	YS<YK^D2D			; //Write : X 0x23FF Y 0xA3FF Read : X 0x23FF Y 0xA3FF                                                 
			NOP			W R						XSYS  				XS<XK^D3	YS<YK^D3 			; //Write : X 0x27FF Y 0xA7FF Read : X 0x27FF Y 0xA7FF                                                 
			NOP			W R						XSYS  				XS<XK^D4	YS<YK^D4 			; //Write : X 0x2FFF Y 0xAFFF Read : X 0x2FFF Y 0xAFFF                                                 
			NOP			W R						XSYS  									 			; //Write : X 0x3FFF Y 0xBFFF Read : X 0x3FFF Y 0xBFFF                                                 
			NOP																						;                                     
			NOP																						;                                     
			NOP																						;                                     

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
    		STPS																					;
	END	
																			
			START XS_ALU_AXORB_03;
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

			NOP		   XC<0xFFFF	YC<0x0000											 			;                                                                  
			NOP		   XS<0x1000	YS<0x9000											 			;                                                                  
			NOP		   XK<0x2000	YK<0xA000											 			;                                                                  
			NOP		   XA<0x3000	YA<0xB000											 			;                                                                  
			NOP		   XD<0x4000	YD<0xC000											 			;                                                                  
			NOP		   XE<0x5000	YE<0xD000		 									 			;                                                                  
			NOP		   XF<0x6000	YF<0xE000											 			;                                                                  
			NOP		   XB<0x7000	YB<0xF000											 			;                                                                  
			NOP		  D1A<0x0000	D4B<0x1000											 			;                                                                  
			NOP		  D1B<0x0001														 			;                                                                  
			NOP		  D1C<0x0002														 			;                                                                  
			NOP		  D1D<0x0004														 			;                                                                  
			NOP		  D1E<0x0008														 			;                                                                  
			NOP		  D1F<0x0010			 											 			;                                                                  
			NOP		  D1G<0x0020														 			;                                                                  
			NOP		  D1H<0x0040														 			;                                                                  
			NOP		  D2A<0x0080	D3B<0x0800											 			;                                                                  
			NOP		  D2B<0x0100														 			;                                                                  
			NOP		  D2C<0x0200														 			;                                                                  
			NOP		  D2D<0x0400														 			;                                                                  
			NOP		   D3<D3B															 			;                                                                  
			NOP		   D4<D4B															 			;                                                                  
			NOP																			 			;                                                 
			NOP																			 			;                                                 
			NOP														XS<XA^D1A	YS<YA^D1A			;                                                 
			NOP			W R						XSYS  				XS<XA^D1B	YS<YA^D1B			; //Write : X 0x3000 Y 0xB000 Read : X 0x3000 Y 0xB000                                                 
			NOP			W R						XSYS  				XS<XA^D1C	YS<YA^D1C			; //Write : X 0x3001 Y 0xB001 Read : X 0x3001 Y 0xB001                                                 
			NOP			W R						XSYS  				XS<XA^D1D	YS<YA^D1D			; //Write : X 0x3003 Y 0xB003 Read : X 0x3003 Y 0xB003                                                 
			NOP			W R						XSYS  				XS<XA^D1E	YS<YA^D1E			; //Write : X 0x3007 Y 0xB007 Read : X 0x3007 Y 0xB007                                                 
			NOP			W R						XSYS  				XS<XA^D1F	YS<YA^D1F			; //Write : X 0x300F Y 0xB00F Read : X 0x300F Y 0xB00F                                                 
			NOP			W R						XSYS  				XS<XA^D1G	YS<YA^D1G			; //Write : X 0x301F Y 0xB01F Read : X 0x301F Y 0xB01F                                                 
			NOP			W R						XSYS  				XS<XA^D1H	YS<YA^D1H			; //Write : X 0x303F Y 0xB03F Read : X 0x303F Y 0xB03F                                                 
			NOP			W R						XSYS  				XS<XA^D2A	YS<YA^D2A			; //Write : X 0x307F Y 0xB07F Read : X 0x307F Y 0xB07F                                                 
			NOP			W R						XSYS  				XS<XA^D2B	YS<YA^D2B			; //Write : X 0x30FF Y 0xB0FF Read : X 0x30FF Y 0xB0FF                                                 
			NOP			W R						XSYS  				XS<XA^D2C	YS<YA^D2C			; //Write : X 0x31FF Y 0xB1FF Read : X 0x31FF Y 0xB1FF                                                 
			NOP			W R						XSYS  				XS<XA^D2D	YS<YA^D2D			; //Write : X 0x33FF Y 0xB3FF Read : X 0x33FF Y 0xB3FF                                                 
			NOP			W R						XSYS  				XS<XA^D3	YS<YA^D3 			; //Write : X 0x37FF Y 0xB7FF Read : X 0x37FF Y 0xB7FF                                                 
			NOP			W R						XSYS  				XS<XA^D4	YS<YA^D4 			; //Write : X 0x3FFF Y 0xBFFF Read : X 0x3FFF Y 0xBFFF                                                 
			NOP			W R						XSYS  									 			; //Write : X 0x2FFF Y 0xAFFF Read : X 0x2FFF Y 0xAFFF                                                 
			NOP																						;                                     
			NOP																						;                                     
			NOP																						;                                     

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
    		STPS																					;
	END	
																			
			START XS_ALU_AXORB_04;
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

			NOP		   XC<0xFFFF	YC<0x0000											 			;                                                                  
			NOP		   XS<0x1000	YS<0x9000											 			;                                                                  
			NOP		   XK<0x2000	YK<0xA000											 			;                                                                  
			NOP		   XA<0x3000	YA<0xB000											 			;                                                                  
			NOP		   XD<0x4000	YD<0xC000											 			;                                                                  
			NOP		   XE<0x5000	YE<0xD000		 									 			;                                                                  
			NOP		   XF<0x6000	YF<0xE000											 			;                                                                  
			NOP		   XB<0x7000	YB<0xF000											 			;                                                                  
			NOP		  D1A<0x0000	D4B<0x1000											 			;                                                                  
			NOP		  D1B<0x0001														 			;                                                                  
			NOP		  D1C<0x0002														 			;                                                                  
			NOP		  D1D<0x0004														 			;                                                                  
			NOP		  D1E<0x0008														 			;                                                                  
			NOP		  D1F<0x0010			 											 			;                                                                  
			NOP		  D1G<0x0020														 			;                                                                  
			NOP		  D1H<0x0040														 			;                                                                  
			NOP		  D2A<0x0080	D3B<0x0800											 			;                                                                  
			NOP		  D2B<0x0100														 			;                                                                  
			NOP		  D2C<0x0200														 			;                                                                  
			NOP		  D2D<0x0400														 			;                                                                  
			NOP		   D3<D3B															 			;                                                                  
			NOP		   D4<D4B															 			;                                                                  
			NOP																			 			;                                                 
			NOP																			 			;                                                 
			NOP														XS<XD^D1A	YS<YD^D1A			;                                                 
			NOP			W R						XSYS  				XS<XD^D1B	YS<YD^D1B			; //Write : X 0x4000 Y 0xC000 Read : X 0x4000 Y 0xC000                                                 
			NOP			W R						XSYS  				XS<XD^D1C	YS<YD^D1C			; //Write : X 0x4001 Y 0xC001 Read : X 0x4001 Y 0xC001                                                 
			NOP			W R						XSYS  				XS<XD^D1D	YS<YD^D1D			; //Write : X 0x4003 Y 0xC003 Read : X 0x4003 Y 0xC003                                                 
			NOP			W R						XSYS  				XS<XD^D1E	YS<YD^D1E			; //Write : X 0x4007 Y 0xC007 Read : X 0x4007 Y 0xC007                                                 
			NOP			W R						XSYS  				XS<XD^D1F	YS<YD^D1F			; //Write : X 0x400F Y 0xC00F Read : X 0x400F Y 0xC00F                                                 
			NOP			W R						XSYS  				XS<XD^D1G	YS<YD^D1G			; //Write : X 0x401F Y 0xC01F Read : X 0x401F Y 0xC01F                                                 
			NOP			W R						XSYS  				XS<XD^D1H	YS<YD^D1H			; //Write : X 0x403F Y 0xC03F Read : X 0x403F Y 0xC03F                                                 
			NOP			W R						XSYS  				XS<XD^D2A	YS<YD^D2A			; //Write : X 0x407F Y 0xC07F Read : X 0x407F Y 0xC07F                                                 
			NOP			W R						XSYS  				XS<XD^D2B	YS<YD^D2B			; //Write : X 0x40FF Y 0xC0FF Read : X 0x40FF Y 0xC0FF                                                 
			NOP			W R						XSYS  				XS<XD^D2C	YS<YD^D2C			; //Write : X 0x41FF Y 0xC1FF Read : X 0x41FF Y 0xC1FF                                                 
			NOP			W R						XSYS  				XS<XD^D2D	YS<YD^D2D			; //Write : X 0x43FF Y 0xC3FF Read : X 0x43FF Y 0xC3FF                                                 
			NOP			W R						XSYS  				XS<XD^D3	YS<YD^D3 			; //Write : X 0x47FF Y 0xC7FF Read : X 0x47FF Y 0xC7FF                                                 
			NOP			W R						XSYS  				XS<XD^D4	YS<YD^D4 			; //Write : X 0x4FFF Y 0xCFFF Read : X 0x4FFF Y 0xCFFF                                                 
			NOP			W R						XSYS  									 			; //Write : X 0x5FFF Y 0xDFFF Read : X 0x5FFF Y 0xDFFF                                                 
			NOP																						;                                     
			NOP																						;                                     
			NOP																						;                                     
			NOP																						;                                     

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
    		STPS																					;
	END	
																		
			START XS_ALU_AXORB_05;
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

			NOP		   XC<0xFFFF	YC<0x0000											 			;                                                                  
			NOP		   XS<0x1000	YS<0x9000											 			;                                                                  
			NOP		   XK<0x2000	YK<0xA000											 			;                                                                  
			NOP		   XA<0x3000	YA<0xB000											 			;                                                                  
			NOP		   XD<0x4000	YD<0xC000											 			;                                                                  
			NOP		   XE<0x5000	YE<0xD000		 									 			;                                                                  
			NOP		   XF<0x6000	YF<0xE000											 			;                                                                  
			NOP		   XB<0x7000	YB<0xF000											 			;                                                                  
			NOP		  D1A<0x0000	D4B<0x1000											 			;                                                                  
			NOP		  D1B<0x0001														 			;                                                                  
			NOP		  D1C<0x0002														 			;                                                                  
			NOP		  D1D<0x0004														 			;                                                                  
			NOP		  D1E<0x0008														 			;                                                                  
			NOP		  D1F<0x0010			 											 			;                                                                  
			NOP		  D1G<0x0020														 			;                                                                  
			NOP		  D1H<0x0040														 			;                                                                  
			NOP		  D2A<0x0080	D3B<0x0800											 			;                                                                  
			NOP		  D2B<0x0100														 			;                                                                  
			NOP		  D2C<0x0200														 			;                                                                  
			NOP		  D2D<0x0400														 			;                                                                  
			NOP		   D3<D3B															 			;                                                                  
			NOP		   D4<D4B															 			;                                                                  
			NOP																			 			;                                                                  
			NOP																			 			;                                                 
			NOP														XS<XE^D1A	YS<YE^D1A			;                                                 
			NOP			W R						XSYS  				XS<XE^D1B	YS<YE^D1B			; //Write : X 0x5000 Y 0xD000 Read : X 0x5000 Y 0xD000                                                 
			NOP			W R						XSYS  				XS<XE^D1C	YS<YE^D1C			; //Write : X 0x5001 Y 0xD001 Read : X 0x5001 Y 0xD001                                                 
			NOP			W R						XSYS  				XS<XE^D1D	YS<YE^D1D			; //Write : X 0x5003 Y 0xD003 Read : X 0x5003 Y 0xD003                                                 
			NOP			W R						XSYS  				XS<XE^D1E	YS<YE^D1E			; //Write : X 0x5007 Y 0xD007 Read : X 0x5007 Y 0xD007                                                 
			NOP			W R						XSYS  				XS<XE^D1F	YS<YE^D1F			; //Write : X 0x500F Y 0xD00F Read : X 0x500F Y 0xD00F                                                 
			NOP			W R						XSYS  				XS<XE^D1G	YS<YE^D1G			; //Write : X 0x501F Y 0xD01F Read : X 0x501F Y 0xD01F                                                 
			NOP			W R						XSYS  				XS<XE^D1H	YS<YE^D1H			; //Write : X 0x503F Y 0xD03F Read : X 0x503F Y 0xD03F                                                 
			NOP			W R						XSYS  				XS<XE^D2A	YS<YE^D2A			; //Write : X 0x507F Y 0xD07F Read : X 0x507F Y 0xD07F                                                 
			NOP			W R						XSYS  				XS<XE^D2B	YS<YE^D2B			; //Write : X 0x50FF Y 0xD0FF Read : X 0x50FF Y 0xD0FF                                                 
			NOP			W R						XSYS  				XS<XE^D2C	YS<YE^D2C			; //Write : X 0x51FF Y 0xD1FF Read : X 0x51FF Y 0xD1FF                                                 
			NOP			W R						XSYS  				XS<XE^D2D	YS<YE^D2D			; //Write : X 0x53FF Y 0xD3FF Read : X 0x53FF Y 0xD3FF                                                 
			NOP			W R						XSYS  				XS<XE^D3	YS<YE^D3 			; //Write : X 0x57FF Y 0xD7FF Read : X 0x57FF Y 0xD7FF                                                 
			NOP			W R						XSYS  				XS<XE^D4	YS<YE^D4 			; //Write : X 0x5FFF Y 0xDFFF Read : X 0x5FFF Y 0xDFFF                                                 
			NOP			W R						XSYS  									 			; //Write : X 0x4FFF Y 0xCFFF Read : X 0x4FFF Y 0xCFFF                                                 
			NOP																						;                                     
			NOP																						;                                     

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
    		STPS																					;
	END		
																			
			START XS_ALU_AXORB_06;
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

			NOP		   XC<0xFFFF	YC<0x0000											 			;                                                                  
			NOP		   XS<0x1000	YS<0x9000											 			;                                                                  
			NOP		   XK<0x2000	YK<0xA000											 			;                                                                  
			NOP		   XA<0x3000	YA<0xB000											 			;                                                                  
			NOP		   XD<0x4000	YD<0xC000											 			;                                                                  
			NOP		   XE<0x5000	YE<0xD000		 									 			;                                                                  
			NOP		   XF<0x6000	YF<0xE000											 			;                                                                  
			NOP		   XB<0x7000	YB<0xF000											 			;                                                                  
			NOP		  D1A<0x0000	D4B<0x1000											 			;                                                                  
			NOP		  D1B<0x0001														 			;                                                                  
			NOP		  D1C<0x0002														 			;                                                                  
			NOP		  D1D<0x0004														 			;                                                                  
			NOP		  D1E<0x0008														 			;                                                                  
			NOP		  D1F<0x0010			 											 			;                                                                  
			NOP		  D1G<0x0020														 			;                                                                  
			NOP		  D1H<0x0040														 			;                                                                  
			NOP		  D2A<0x0080	D3B<0x0800											 			;                                                                  
			NOP		  D2B<0x0100														 			;                                                                  
			NOP		  D2C<0x0200														 			;                                                                  
			NOP		  D2D<0x0400														 			;                                                                  
			NOP		   D3<D3B															 			;                                                                  
			NOP		   D4<D4B															 			;                                                                  
			NOP																			 			;                                                 
			NOP														XS<XF^D1A	YS<YF^D1A			;                                                 
			NOP			W R						XSYS  				XS<XF^D1B	YS<YF^D1B			; //Write : X 0x6000 Y 0xE000 Read : X 0x6000 Y 0xE000                                                 
			NOP			W R						XSYS  				XS<XF^D1C	YS<YF^D1C			; //Write : X 0x6001 Y 0xE001 Read : X 0x6001 Y 0xE001                                                 
			NOP			W R						XSYS  				XS<XF^D1D	YS<YF^D1D			; //Write : X 0x6003 Y 0xE003 Read : X 0x6003 Y 0xE003                                                 
			NOP			W R						XSYS  				XS<XF^D1E	YS<YF^D1E			; //Write : X 0x6007 Y 0xE007 Read : X 0x6007 Y 0xE007                                                 
			NOP			W R						XSYS  				XS<XF^D1F	YS<YF^D1F			; //Write : X 0x600F Y 0xE00F Read : X 0x600F Y 0xE00F                                                 
			NOP			W R						XSYS  				XS<XF^D1G	YS<YF^D1G			; //Write : X 0x601F Y 0xE01F Read : X 0x601F Y 0xE01F                                                 
			NOP			W R						XSYS  				XS<XF^D1H	YS<YF^D1H			; //Write : X 0x603F Y 0xE03F Read : X 0x603F Y 0xE03F                                                 
			NOP			W R						XSYS  				XS<XF^D2A	YS<YF^D2A			; //Write : X 0x607F Y 0xE07F Read : X 0x607F Y 0xE07F                                                 
			NOP			W R						XSYS  				XS<XF^D2B	YS<YF^D2B			; //Write : X 0x60FF Y 0xE0FF Read : X 0x60FF Y 0xE0FF                                                 
			NOP			W R						XSYS  				XS<XF^D2C	YS<YF^D2C			; //Write : X 0x61FF Y 0xE1FF Read : X 0x61FF Y 0xE1FF                                                 
			NOP			W R						XSYS  				XS<XF^D2D	YS<YF^D2D			; //Write : X 0x63FF Y 0xE3FF Read : X 0x63FF Y 0xE3FF                                                 
			NOP			W R						XSYS  				XS<XF^D3	YS<YF^D3 			; //Write : X 0x67FF Y 0xE7FF Read : X 0x67FF Y 0xE7FF                                                 
			NOP			W R						XSYS  				XS<XF^D4	YS<YF^D4 			; //Write : X 0x6FFF Y 0xEFFF Read : X 0x6FFF Y 0xEFFF                                                 
			NOP			W R						XSYS  									 			; //Write : X 0x7FFF Y 0xFFFF Read : X 0x7FFF Y 0xFFFF                                                 
			NOP																						;                                     
			NOP																						;                                     
			NOP																						;                                     
			NOP																						;                                     

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
    		STPS																					;
	END	
																			
			START XS_ALU_AXORB_07;
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

			NOP		   XC<0xFFFF	YC<0x0000											 			;                                                                  
			NOP		   XS<0x1000	YS<0x9000											 			;                                                                  
			NOP		   XK<0x2000	YK<0xA000											 			;                                                                  
			NOP		   XA<0x3000	YA<0xB000											 			;                                                                  
			NOP		   XD<0x4000	YD<0xC000											 			;                                                                  
			NOP		   XE<0x5000	YE<0xD000		 									 			;                                                                  
			NOP		   XF<0x6000	YF<0xE000											 			;                                                                  
			NOP		   XB<0x7000	YB<0xF000											 			;                                                                  
			NOP		  D1A<0x0000	D4B<0x1000											 			;                                                                  
			NOP		  D1B<0x0001														 			;                                                                  
			NOP		  D1C<0x0002														 			;                                                                  
			NOP		  D1D<0x0004														 			;                                                                  
			NOP		  D1E<0x0008														 			;                                                                  
			NOP		  D1F<0x0010			 											 			;                                                                  
			NOP		  D1G<0x0020														 			;                                                                  
			NOP		  D1H<0x0040														 			;                                                                  
			NOP		  D2A<0x0080	D3B<0x0800											 			;                                                                  
			NOP		  D2B<0x0100														 			;                                                                  
			NOP		  D2C<0x0200														 			;                                                                  
			NOP		  D2D<0x0400														 			;                                                                  
			NOP		   D3<D3B															 			;                                                                  
			NOP		   D4<D4B															 			;                                                                  
			NOP																			 			;                                                 
			NOP														XS<XB^D1A	YS<YB^D1A			;                                                 
			NOP			W R						XSYS  				XS<XB^D1B	YS<YB^D1B			; //Write : X 0x7000 Y 0xF000 Read : X 0x7000 Y 0xF000                                                 
			NOP			W R						XSYS  				XS<XB^D1C	YS<YB^D1C			; //Write : X 0x7001 Y 0xF001 Read : X 0x7001 Y 0xF001                                                 
			NOP			W R						XSYS  				XS<XB^D1D	YS<YB^D1D			; //Write : X 0x7003 Y 0xF003 Read : X 0x7003 Y 0xF003                                                 
			NOP			W R						XSYS  				XS<XB^D1E	YS<YB^D1E			; //Write : X 0x7007 Y 0xF007 Read : X 0x7007 Y 0xF007                                                 
			NOP			W R						XSYS  				XS<XB^D1F	YS<YB^D1F			; //Write : X 0x700F Y 0xF00F Read : X 0x700F Y 0xF00F                                                 
			NOP			W R						XSYS  				XS<XB^D1G	YS<YB^D1G			; //Write : X 0x701F Y 0xF01F Read : X 0x701F Y 0xF01F                                                 
			NOP			W R						XSYS  				XS<XB^D1H	YS<YB^D1H			; //Write : X 0x703F Y 0xF03F Read : X 0x703F Y 0xF03F                                                 
			NOP			W R						XSYS  				XS<XB^D2A	YS<YB^D2A			; //Write : X 0x707F Y 0xF07F Read : X 0x707F Y 0xF07F                                                 
			NOP			W R						XSYS  				XS<XB^D2B	YS<YB^D2B			; //Write : X 0x70FF Y 0xF0FF Read : X 0x70FF Y 0xF0FF                                                 
			NOP			W R						XSYS  				XS<XB^D2C	YS<YB^D2C			; //Write : X 0x71FF Y 0xF1FF Read : X 0x71FF Y 0xF1FF                                                 
			NOP			W R						XSYS  				XS<XB^D2D	YS<YB^D2D			; //Write : X 0x73FF Y 0xF3FF Read : X 0x73FF Y 0xF3FF                                                 
			NOP			W R						XSYS  				XS<XB^D3	YS<YB^D3 			; //Write : X 0x77FF Y 0xF7FF Read : X 0x77FF Y 0xF7FF                                                 
			NOP			W R						XSYS  				XS<XB^D4	YS<YB^D4 			; //Write : X 0x7FFF Y 0xFFFF Read : X 0x7FFF Y 0xFFFF                                                 
			NOP			W R						XSYS  									 			; //Write : X 0x6FFF Y 0xEFFF Read : X 0x6FFF Y 0xEFFF                                                 
			NOP																						;                                     
			NOP																						;                                     
			NOP																						;                                     
			NOP																						;                                     

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
    		STPS																					;
	END
	
			START XS_ALU_D3D4_00;
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

			NOP																						;                                                 
			NOP														 								;                                                 
			NOP				        D3B<0x1111 D4B<0xAAAA											;                                                 
			NOP		   D3<D3B  D4<D4B												    			;                                                                  
			NOP														XS<D3 YS<D4						;                                                 
			NOP			W R						XSYS  				XS<D3 YS<D4	D3<0     D4<D4+1	; //Write : X 0x1111 Y 0xAAAA Read : X 0x1111 Y 0xAAAA                                                
			NOP			W R						XSYS  				XS<D3 YS<D4	D3<D3+1	 D4<0		; //Write : X 0x0000 Y 0xAAAB Read : X 0x0000 Y 0xAAAB                                                  
			NOP			W R						XSYS  				XS<D3 YS<D4	D3<D3+1	 D4<D4+1	; //Write : X 0x0001 Y 0x0000 Read : X 0x0001 Y 0x0000                                                 
			NOP			W R						XSYS  				XS<D3 YS<D4	D3<D3-1	 D4<D4+1	; //Write : X 0x0002 Y 0x0001 Read : X 0x0002 Y 0x0001                                                
			NOP			W R						XSYS  				XS<D3 YS<D4	D3<D3+1	 D4<D4-1	; //Write : X 0x0001 Y 0x0002 Read : X 0x0001 Y 0x0002                                                
			NOP			W R						XSYS  				XS<D3 YS<D4	D3<D3*2	 D4<D4*2	; //Write : X 0x0002 Y 0x0001 Read : X 0x0002 Y 0x0001                                                
			NOP			W R						XSYS  				XS<D3 YS<D4	D3<D3*2	 D4<D4/2	; //Write : X 0x0004 Y 0x0002 Read : X 0x0004 Y 0x0002                                                 
			NOP			W R						XSYS  				XS<D3 YS<D4	D3<D3/2	 D4<D4*2	; //Write : X 0x0008 Y 0x0001 Read : X 0x0008 Y 0x0001                                                
			NOP			W R						XSYS  				XS<D3 YS<D4	D3<D3*2	 D4<D4*2	; //Write : X 0x0004 Y 0x0002 Read : X 0x0004 Y 0x0002                                                  
			NOP			W R						XSYS  				XS<D3 YS<D4	D3<D3B/2 D4<D4B/2	; //Write : X 0x0008 Y 0x0004 Read : X 0x0008 Y 0x0004                                                 
			NOP			W R						XSYS  				XS<D3 YS<D4						; //Write : X 0x0888 Y 0x5555 Read : X 0x0888 Y 0x5555                                                
			NOP			W R						XSYS  				XS<D3 YS<D4						; //Write : X 0x1111 Y 0xAAAA Read : X 0x1111 Y 0xAAAA                                                 
			NOP			W R						XSYS  				XS<D3 YS<D4						; //Write : X 0x1111 Y 0xAAAA Read : X 0x1111 Y 0xAAAA                                     
			NOP			W R                     XSYS																			; //Write : X 0x1111 Y 0xAAAA Read : X 0x1111 Y 0xAAAA                                    
			NOP																						;                                     
			NOP																						;                                     

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
    		STPS																					;
	END

			START XS_ALU_APImm_00;
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

			NOP		   XC<0x0000	 YC<0x8000													;                                                                  
			NOP																					;                                                 
			NOP														XS<XC+0x0001	YS<YC+0x0001;                                                 
			NOP			W R						XSYS  				XS<XC+0x0002	YS<YC+0x0002; //Write : X 0x0001 Y 0x8001 Read : X 0x0001 Y 0x8001                                                 
			NOP			W R						XSYS  				XS<XC+0x0004	YS<YC+0x0004; //Write : X 0x0003 Y 0x8003 Read : X 0x0003 Y 0x8003                                                 
			NOP			W R						XSYS  				XS<XC+0x0008	YS<YC+0x0008; //Write : X 0x0007 Y 0x8007 Read : X 0x0007 Y 0x8007                                                 
			NOP			W R						XSYS  				 							; //Write : X 0x000F Y 0x800F Read : X 0x000F Y 0x800F                                                 
			NOP																					;                                     

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
    		STPS																				;
	END	

			START XS_ALU_APImm_01;
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

			NOP		   XS<0x0000	 YS<0x8000													;                                                                  
			NOP																					;                                                 
			NOP														XS<XS+0x0010	YS<YS+0x0010;                                                 
			NOP			W R						XSYS  				XS<XS+0x0020	YS<YS+0x0020; //Write : X 0x0010 Y 0x8010 Read : X 0x0010 Y 0x8010                                                 
			NOP			W R						XSYS  				XS<XS+0x0040	YS<YS+0x0040; //Write : X 0x0030 Y 0x8030 Read : X 0x0030 Y 0x8030                                                 
			NOP			W R						XSYS  				XS<XS+0x0080	YS<YS+0x0080; //Write : X 0x0070 Y 0x8070 Read : X 0x0070 Y 0x8070                                                 
			NOP			W R						XSYS  				 							; //Write : X 0x00F0 Y 0x80F0 Read : X 0x00F0 Y 0x80F0                                                 
			NOP																					;                                     

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
    		STPS																				;
	END	

			START XS_ALU_APImm_02;
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

			NOP		   XK<0x0000	 YK<0x8000													;                                                                  
			NOP																					;                                                 
			NOP														XS<XK+0x0100	YS<YK+0x0100;                                                 
			NOP			W R						XSYS  				XS<XK+0x0200	YS<YK+0x0200; //Write : X 0x0100 Y 0x8100 Read : X 0x0100 Y 0x8100                                                 
			NOP			W R						XSYS  				XS<XK+0x0400	YS<YK+0x0400; //Write : X 0x0200 Y 0x8200 Read : X 0x0200 Y 0x8200                                                 
			NOP			W R						XSYS  				XS<XK+0x0800	YS<YK+0x0800; //Write : X 0x0400 Y 0x8400 Read : X 0x0400 Y 0x8400                                                 
			NOP			W R						XSYS  				 							; //Write : X 0x0800 Y 0x8800 Read : X 0x0800 Y 0x8800                                                 
			NOP																					;                                     

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
    		STPS																				;
	END	

			START XS_ALU_APImm_03;
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

			NOP		   XA<0x0000	 YA<0x0001													;                                                                  
			NOP																					;                                                 
			NOP														XS<XA+0x1000	YS<YA+0x1000;                                                 
			NOP			W R						XSYS  				XS<XA+0x2000	YS<YA+0x2000; //Write : X 0x1000 Y 0x1001 Read : X 0x1000 Y 0x1001                                                 
			NOP			W R						XSYS  				XS<XA+0x4000	YS<YA+0x4000; //Write : X 0x2000 Y 0x2001 Read : X 0x2000 Y 0x2001                                                 
			NOP			W R						XSYS  				XS<XA+0x8000	YS<YA+0x8000; //Write : X 0x4000 Y 0x4001 Read : X 0x4000 Y 0x4001                                                 
			NOP			W R						XSYS  				 							; //Write : X 0x8000 Y 0x8001 Read : X 0x8000 Y 0x8001                                                 
			NOP																					;                                     

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
    		STPS																				;
	END	

			START XS_ALU_APImm_04;
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

			NOP		   XD<0x0000	 YD<0x8000													;                                                                  
			NOP																					;                                                 
			NOP														XS<XD+0x000A	YS<YD+0x000A;                                                 
			NOP			W R						XSYS  				XS<XD+0x000B	YS<YD+0x000B; //Write : X 0x000A Y 0x800A Read : X 0x000A Y 0x800A                                                  
			NOP			W R						XSYS  				XS<XD+0x000C	YS<YD+0x000C; //Write : X 0x000B Y 0x800B Read : X 0x000B Y 0x800B                                                  
			NOP			W R						XSYS  				XS<XD+0x000D	YS<YD+0x000D; //Write : X 0x000C Y 0x800C Read : X 0x000C Y 0x800C                                                  
			NOP			W R						XSYS  				 							; //Write : X 0x000D Y 0x800D Read : X 0x000D Y 0x800D                                                  
			NOP																					;                                     

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
    		STPS																				;
	END	

			START XS_ALU_APImm_05;
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

			NOP		   XE<0x0000	 YE<0x8000													;                                                                  
			NOP																					;                                                 
			NOP														XS<XE+0x00A0	YS<YE+0x00A0;                                                 
			NOP			W R						XSYS  				XS<XE+0x00B0	YS<YE+0x00B0; //Write : X 0x00A0 Y 0x80A0 Read : X 0x00A0 Y 0x80A0                                                 
			NOP			W R						XSYS  				XS<XE+0x00C0	YS<YE+0x00C0; //Write : X 0x00B0 Y 0x80B0 Read : X 0x00B0 Y 0x80B0                                                 
			NOP			W R						XSYS  				XS<XE+0x00D0	YS<YE+0x00D0; //Write : X 0x00C0 Y 0x80C0 Read : X 0x00C0 Y 0x80C0                                                 
			NOP			W R						XSYS  				 							; //Write : X 0x00D0 Y 0x80D0 Read : X 0x00D0 Y 0x80D0                                                 
			NOP																					;                                     

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
    		STPS																				;
	END	

			START XS_ALU_APImm_06;
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

			NOP		   XF<0x0000	 YF<0x0001													;                                                                  
			NOP																					;                                                 
			NOP														XS<XF+0x0A00	YS<YF+0x0A00;                                                 
			NOP			W R						XSYS  				XS<XF+0x0B00	YS<YF+0x0B00; //Write : X 0x0A00 Y 0x0A01 Read : X 0x0A00 Y 0x0A01                                                 
			NOP			W R						XSYS  				XS<XF+0x0C00	YS<YF+0x0C00; //Write : X 0x0B00 Y 0x0B01 Read : X 0x0B00 Y 0x0B01                                                 
			NOP			W R						XSYS  				XS<XF+0x0D00	YS<YF+0x0D00; //Write : X 0x0C00 Y 0x0C01 Read : X 0x0C00 Y 0x0C01                                                 
			NOP			W R						XSYS  				 							; //Write : X 0x0D00 Y 0x0D01 Read : X 0x0D00 Y 0x0D01                                                 
			NOP																					;                                     

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
    		STPS																				;
	END	

			START XS_ALU_APImm_07;
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

			NOP		   XB<0x0000	 YB<0x1000													;                                                                  
			NOP																					;                                                 
			NOP														XS<XB+0xA000	YS<YB+0xA000;                                                 
			NOP			W R						XSYS  				XS<XB+0x0B00	YS<YB+0x0B00; //Write : X 0xA000 Y 0xB000 Read : X 0xA000 Y 0xB000                                                 
			NOP			W R						XSYS  				XS<XB+0x00C0	YS<YB+0x00C0; //Write : X 0x0B00 Y 0x1B00 Read : X 0x0B00 Y 0x1B00                                                 
			NOP			W R						XSYS  				XS<XB+0x000D	YS<YB+0x000D; //Write : X 0x00C0 Y 0x10C0 Read : X 0x00C0 Y 0x10C0                                                 
			NOP			W R						XSYS  				 							; //Write : X 0x000D Y 0x100D Read : X 0x000D Y 0x100D                                                 
			NOP																					;                                     

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
    		STPS																				;
	END	
	
			START XS_ALU_AMImm_00;
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

			NOP		   XC<0xF000	 YC<0x8000													;                                                                  
			NOP																					;                                                 
			NOP														XS<XC-0x0001	YS<YC-0x0001;                                                 
			NOP			W R						XSYS  				XS<XC-0x0002	YS<YC-0x0002; //Write : X 0xEFFF Y 0x7FFF Read : X 0xEFFF Y 0x7FFF                                                 
			NOP			W R						XSYS  				XS<XC-0x0004	YS<YC-0x0004; //Write : X 0xEFFE Y 0x7FFE Read : X 0xEFFE Y 0x7FFE                                                 
			NOP			W R						XSYS  				XS<XC-0x0008	YS<YC-0x0008; //Write : X 0xEFFC Y 0x7FFC Read : X 0xEFFC Y 0x7FFC                                                 
			NOP			W R						XSYS  				 							; //Write : X 0xEFF8 Y 0x7FF8 Read : X 0xEFF8 Y 0x7FF8                                                 
			NOP																					;                                     

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
    		STPS																				;
	END	

			START XS_ALU_AMImm_01;
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

			NOP		   XS<0xF000	 YS<0x8000													;                                                                  
			NOP																					;                                                 
			NOP														XS<XS-0x0010	YS<YS-0x0010;                                                 
			NOP			W R						XSYS  				XS<XS-0x0020	YS<YS-0x0020; //Write : X 0xEFF0 Y 0x7FF0 Read : X 0xEFF0 Y 0x7FF0                                                 
			NOP			W R						XSYS  				XS<XS-0x0040	YS<YS-0x0040; //Write : X 0xEFD0 Y 0x7FD0 Read : X 0xEFE0 Y 0x7FE0                                                 
			NOP			W R						XSYS  				XS<XS-0x0080	YS<YS-0x0080; //Write : X 0xEF90 Y 0x7F90 Read : X 0xEFC0 Y 0x7FC0                                                 
			NOP			W R						XSYS  				 							; //Write : X 0xEF10 Y 0x7F10 Read : X 0xEF80 Y 0x7F80                                                 
			NOP																					;                                     

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
    		STPS																				;
	END	

			START XS_ALU_AMImm_02;
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

			NOP		   XK<0xF000	 YK<0x8000													;                                                                  
			NOP																					;                                                 
			NOP														XS<XK-0x0100	YS<YK-0x0100;                                                 
			NOP			W R						XSYS  				XS<XK-0x0200	YS<YK-0x0200; //Write : X 0xEF00 Y 0x7F00 Read : X 0xEF00 Y 0x7F00                                                 
			NOP			W R						XSYS  				XS<XK-0x0400	YS<YK-0x0400; //Write : X 0xEE00 Y 0x7E00 Read : X 0xEE00 Y 0x7E00                                                 
			NOP			W R						XSYS  				XS<XK-0x0800	YS<YK-0x0800; //Write : X 0xEC00 Y 0x7C00 Read : X 0xEC00 Y 0x7C00                                                 
			NOP			W R						XSYS  				 							; //Write : X 0xE800 Y 0x7800 Read : X 0xE800 Y 0x7800                                                 
			NOP																					;                                     

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
    		STPS																				;
	END	

			START XS_ALU_AMImm_03;
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

			NOP		   XA<0xF000	 YA<0x8000													;                                                                  
			NOP																					;                                                 
			NOP														XS<XA-0x1000	YS<YA-0x1000;                                                 
			NOP			W R						XSYS  				XS<XA-0x0200	YS<YA-0x0200; //Write : X 0xE000 Y 0x7000 Read : X 0xE000 Y 0x7000                                                 
			NOP			W R						XSYS  				XS<XA-0x0040	YS<YA-0x0040; //Write : X 0xEE00 Y 0x7E00 Read : X 0xEE00 Y 0x7E00                                                 
			NOP			W R						XSYS  				XS<XA-0x0008	YS<YA-0x0008; //Write : X 0xEFC0 Y 0x7FC0 Read : X 0xEFC0 Y 0x7FC0                                                 
			NOP			W R						XSYS  				 							; //Write : X 0xEFF8 Y 0x7FF8 Read : X 0xEFF8 Y 0x7FF8                                                 
			NOP																					;                                     

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
    		STPS																				;
	END	

			START XS_ALU_AMImm_04;
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

			NOP		   XD<0xF000	 YD<0x8000													;                                                                  
			NOP																					;                                                 
			NOP														XS<XD-0x000A	YS<YD-0x000A;                                                 
			NOP			W R						XSYS  				XS<XD-0x000B	YS<YD-0x000B; //Write : X 0xEFF6 Y 0x7FF6 Read : X 0xEFF6 Y 0x7FF6                                                 
			NOP			W R						XSYS  				XS<XD-0x000C	YS<YD-0x000C; //Write : X 0xEFF5 Y 0x7FF5 Read : X 0xEFF5 Y 0x7FF5                                                 
			NOP			W R						XSYS  				XS<XD-0x000D	YS<YD-0x000D; //Write : X 0xEFF4 Y 0x7FF4 Read : X 0xEFF4 Y 0x7FF4                                                 
			NOP			W R						XSYS  				 							; //Write : X 0xEFF3 Y 0x7FF3 Read : X 0xEFF3 Y 0x7FF3                                                 
			NOP																					;                                     

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
    		STPS																				;
	END	

			START XS_ALU_AMImm_05;
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

			NOP		   XE<0xF000	 YE<0x8000													;                                                                  
			NOP																					;                                                 
			NOP														XS<XE-0x00A0	YS<YE-0x00A0;                                                 
			NOP			W R						XSYS  				XS<XE-0x00B0	YS<YE-0x00B0; //Write : X 0xEF60 Y 0x7F60 Read : X 0xEF60 Y 0x7F60                                                 
			NOP			W R						XSYS  				XS<XE-0x00C0	YS<YE-0x00C0; //Write : X 0xEF50 Y 0x7F50 Read : X 0xEF50 Y 0x7F50                                                 
			NOP			W R						XSYS  				XS<XE-0x00D0	YS<YE-0x00D0; //Write : X 0xEF40 Y 0x7F40 Read : X 0xEF40 Y 0x7F40                                                 
			NOP			W R						XSYS  				 							; //Write : X 0xEF30 Y 0x7F30 Read : X 0xEF30 Y 0x7F30                                                 
			NOP																					;                                     

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
    		STPS																				;
	END	

			START XS_ALU_AMImm_06;
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

			NOP		   XF<0xF000	 YF<0x8000													;                                                                  
			NOP																					;                                                 
			NOP														XS<XF-0x0A00	YS<YF-0x0A00;                                                 
			NOP			W R						XSYS  				XS<XF-0x0B00	YS<YF-0x0B00; //Write : X 0xE600 Y 0x7600 Read : X 0xE600 Y 0x7600                                                 
			NOP			W R						XSYS  				XS<XF-0x0C00	YS<YF-0x0C00; //Write : X 0xE500 Y 0x7500 Read : X 0xE500 Y 0x7500                                                 
			NOP			W R						XSYS  				XS<XF-0x0D00	YS<YF-0x0D00; //Write : X 0xE400 Y 0x7400 Read : X 0xE400 Y 0x7400                                                 
			NOP			W R						XSYS  				 							; //Write : X 0xE300 Y 0x7300 Read : X 0xE300 Y 0x7300                                                 
			NOP																					;                                     

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
    		STPS																				;
	END	

			START XS_ALU_AMImm_07;
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

			NOP		   XB<0xF000	 YB<0xE000													;                                                                  
			NOP																					;                                                 
			NOP														XS<XB-0xA000	YS<YB-0xA000;                                                 
			NOP			W R						XSYS  				XS<XB-0x0B00	YS<YB-0x0B00; //Write : X 0x5000 Y 0x4000 Read : X 0x5000 Y 0x4000                                                 
			NOP			W R						XSYS  				XS<XB-0x00C0	YS<YB-0x00C0; //Write : X 0xE500 Y 0xD500 Read : X 0xE500 Y 0xD500                                                 
			NOP			W R						XSYS  				XS<XB-0x000D	YS<YB-0x000D; //Write : X 0xEF40 Y 0xDF40 Read : X 0xEF40 Y 0xDF40                                                 
			NOP			W R						XSYS  				 							; //Write : X 0xEFF3 Y 0xDFF3 Read : X 0xEFF3 Y 0xDFF3                                                 
			NOP																					;                                     

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
    		STPS																				;
	END

%ENDC 