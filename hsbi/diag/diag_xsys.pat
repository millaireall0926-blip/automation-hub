

%IFE A .AND. B .AND.((C.OR.D).AND.E)
%INSERT Header/define.h
  
 REGISTER
			XMAX=0xffffff;
			YMAX=0xffffff;
			ZMAX=0xff;
			XOS=0x0;
			YOS=0x0;	
ENDREGISTER		

			START XS_ALU_A_00;
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

			NOP		  XS<0x0000			YS<0xFFFF														;                                                                  
			NOP		  XC<0x0001			YC<0x7FFF														;                                                                  
			NOP		  XK<0x0002			YK<0x3FFF														;                                                                  
			NOP		  XA<0x0004			YA<0x1FFF														;                                                                  
			NOP		  XD<0x0008			YD<0x0FFF														;                                                                  
			NOP		  XE<0x0010			YE<0x07FF		 												;                                                                  
			NOP		  XF<0x0020			YF<0x03FF														;                                                                  
			NOP		  XB<0x0040			YB<0x01FF														;                                                                  
			NOP																			;                                                 
			NOP														XS<XS	YS<YS			; 
			NOP			W R						XSYS  				XS<XC	YS<YC			; //Write : XS 0x0000 YC 0xFFFF Read : XS 0x0000 YC 0xFFFF                                                 
			NOP			W R						XSYS  				XS<XK	YS<YK			; //Write : XS 0x0000 YC 0xFFFF Read : XS 0x0000 YC 0xFFFF                                                                                                 
			NOP			W R						XSYS  				XS<XA	YS<YA 			; //Write : XS 0x0002 YC 0x3FFF Read : XS 0x0002 YC 0x3FFF                                                                                                 
			NOP			W R						XSYS  				XS<XD	YS<YD 			; //Write : XS 0x0004 YC 0x1FFF Read : XS 0x0004 YC 0x1FFF                                                                                                 
			NOP			W R						XSYS  				XS<XE	YS<YE			; //Write : XS 0x0008 YC 0x0FFF Read : XS 0x0008 YC 0x0FFF                                                                                                 
			NOP			W R						XSYS  				XS<XF	YS<YF			; //Write : XS 0x0010 YC 0x07FF Read : XS 0x0010 YC 0x07FF                                                                                                 
			NOP			W R						XSYS  				XS<XB	YS<YB			; //Write : XS 0x0020 YC 0x03FF Read : XS 0x0020 YC 0x03FF                                                                                                 
			NOP			W R						XSYS  										; //Write : XS 0x0040 YC 0x01FF Read : XS 0x0040 YC 0x01FF                                                                                                                                                 
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

			START XS_ALU_NA_00;
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

			NOP		  XS<0x0000			YS<0xFFFF												;                                                                  
			NOP		  XC<0x0001			YC<0x7FFF												;                                                                 
			NOP		  XK<0x0002			YK<0x3FFF												;                                                                 
			NOP		  XA<0x0004			YA<0x1FFF												;                                                                 
			NOP		  XD<0x0008			YD<0x0FFF												;                                                                 
			NOP		  XE<0x0010			YE<0x07FF 												;                                                                 
			NOP		  XF<0x0020			YF<0x03FF												;                                                                 
			NOP		  XB<0x0040			YB<0x01FF												;                                                                 
			NOP																			;                                                 
			NOP														XS</XS	YS</YS		;                                                 
			NOP			W R						XSYS  				XS</XC	YS</YC		; //Write : XS 0xFFFF YC 0x0000 Read : XS 0xFFFF YC 0x0000                                                  
			NOP			W R						XSYS  				XS</XK	YS</YK		; //Write : XS 0xFFFE YC 0x8000 Read : XS 0xFFFE YC 0x8000                                                  
			NOP			W R						XSYS  				XS</XA	YS</YA		; //Write : XS 0xFFFD YC 0xC000 Read : XS 0xFFFD YC 0xC000                                                  
			NOP			W R						XSYS  				XS</XD	YS</YD		; //Write : XS 0xFFFB YC 0xE000 Read : XS 0xFFFB YC 0xE000                                                  
			NOP			W R						XSYS  				XS</XE	YS</YE		; //Write : XS 0xFFF7 YC 0xF000 Read : XS 0xFFF7 YC 0xF000                                                  
			NOP			W R						XSYS  				XS</XF	YS</YF		; //Write : XS 0xFFEF YC 0xF800 Read : XS 0xFFEF YC 0xF800                                                  
			NOP			W R						XSYS  				XS</XB	YS</YB		; //Write : XS 0xFFDF YC 0xFC00 Read : XS 0xFFDF YC 0xFC00                                                  
			NOP			W R						XSYS  									; //Write : XS 0xFFBF YC 0xFE00 Read : XS 0xFFBF YC 0xFE00                                                  
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
	
			START XS_ALU_AP1_00;
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
																			;                                                                  
			NOP		  XS<0x0000			YS<0xFFFD													;                                                                  
			NOP		  XC<0x0001			YC<0x7FFF													;                                                                  
			NOP		  XK<0x0002			YK<0x3FFF													;                                                                  
			NOP		  XA<0x0004			YA<0x1FFF													;                                                                  
			NOP		  XD<0x0008			YD<0x0FFF													;                                                                  
			NOP		  XE<0x0010			YE<0x07FF	 												;                                                                  
			NOP		  XF<0x0020			YF<0x03FF													;                                                                  
			NOP		  XB<0x0040			YB<0x01FF													;                                                                  
			NOP																			;                                                                  
			NOP														XS<XS+1	YS<YS+1		;                                               
			NOP			W R						XSYS  				XS<XC+1	YS<YC+1		; //Write : XS 0x0001 YC 0xFFFE Read : XS 0x0001 YC 0xFFFE                                                                                       
			NOP			W R						XSYS  				XS<XK+1	YS<YK+1		; //Write : XS 0x0002 YC 0xFFFF Read : XS 0x0002 YC 0xFFFF                                                                                       
			NOP			W R						XSYS  				XS<XA+1	YS<YA+1		; //Write : XS 0x0003 YC 0x4000 Read : XS 0x0003 YC 0x4000                                                                                      
			NOP			W R						XSYS  				XS<XD+1	YS<YD+1		; //Write : XS 0x0005 YC 0x2000 Read : XS 0x0005 YC 0x2000                                                                                      
			NOP			W R						XSYS  				XS<XE+1	YS<YE+1		; //Write : XS 0x0009 YC 0x1000 Read : XS 0x0009 YC 0x1000                                                                                      
			NOP			W R						XSYS  				XS<XF+1	YS<YF+1		; //Write : XS 0x0011 YC 0x0800 Read : XS 0x0011 YC 0x0800                                                                                      
			NOP			W R						XSYS  				XS<XB+1	YS<YB+1		; //Write : XS 0x0021 YC 0x0400 Read : XS 0x0021 YC 0x0400                                                                                      
			NOP			W R						XSYS  									; //Write : XS 0x0041 YC 0x0200 Read : XS 0x0041 YC 0x0200                                                 
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
	
			START XS_ALU_AM1_00;
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
																			;                                                                  
			NOP		  XS<0x0002		YS<0xFFFF													;                                                                  
			NOP		  XC<0x0003		YC<0x7FFF													;                                                                  
			NOP		  XK<0x0004		YK<0x3FFF													;                                                                  
			NOP		  XA<0x0008		YA<0x1FFF													;                                                                  
			NOP		  XD<0x0010		YD<0x0FFF													;                                                                  
			NOP		  XE<0x0020		YE<0x07FF	 												;                                                                  
			NOP		  XF<0x0040		YF<0x03FF													;                                                                  
			NOP		  XB<0x0080		YB<0x01FF													;                                                                  
			NOP																			;                                                 
			NOP														XS<XS-1	YS<YS-1		;                                                 
			NOP			W R						XSYS  				XS<XC-1	YS<YC-1		; //Write : XS 0x0001 YC 0xFFFE Read : XS 0x0001 YC 0xFFFE                                                  
			NOP			W R						XSYS  				XS<XK-1	YS<YK-1		; //Write : XS 0x0000 YC 0xFFFD Read : XS 0x0000 YC 0xFFFD                                                  
			NOP			W R						XSYS  				XS<XA-1	YS<YA-1		; //Write : XS 0x0003 YC 0x3FFE Read : XS 0x0003 YC 0x3FFE                                                  
			NOP			W R						XSYS  				XS<XD-1	YS<YD-1		; //Write : XS 0x0007 YC 0x1FFE Read : XS 0x0007 YC 0x1FFE                                                  
			NOP			W R						XSYS  				XS<XE-1	YS<YE-1		; //Write : XS 0x000F YC 0x0FFE Read : XS 0x000F YC 0x0FFE                                                  
			NOP			W R						XSYS  				XS<XF-1	YS<YF-1		; //Write : XS 0x001F YC 0x07FE Read : XS 0x001F YC 0x07FE                                                  
			NOP			W R						XSYS  				XS<XB-1	YS<YB-1		; //Write : XS 0x003F YC 0x03FE Read : XS 0x003F YC 0x03FE                                                  
			NOP			W R						XSYS  									; //Write : XS 0x007F YC 0x01FE Read : XS 0x007F YC 0x01FE                                              
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
	
			START XS_ALU_AM2_00;
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

			NOP		  XS<0x0000		YS<0x000A													;                                                                  
			NOP		  XC<0x0001		YC<0x000B													;                                                                  
			NOP		  XK<0x0002		YK<0x000C													;                                                                  
			NOP		  XA<0x0004		YA<0x000D													;                                                                  
			NOP		  XD<0x0008		YD<0x000E													;                                                                  
			NOP		  XE<0x0010		YE<0x000F	 												;                                                                  
			NOP		  XF<0x0020		YF<0x00A0													;                                                                  
			NOP		  XB<0x0040		YB<0x00B0													;                                                                  
			NOP																			;                                                 
			NOP														XS<XS*2	YS<YS*2		;                                                 
			NOP			W R						XSYS  				XS<XC*2	YS<YC*2		; //Write : XS 0x0000 YC 0x0014 Read : XS 0x0000 YC 0x0014                                                 
			NOP			W R						XSYS  				XS<XK*2	YS<YK*2		; //Write : XS 0x0000 YC 0x0016 Read : XS 0x0000 YC 0x0016                                                 
			NOP			W R						XSYS  				XS<XA*2	YS<YA*2		; //Write : XS 0x0004 YC 0x0018 Read : XS 0x0004 YC 0x0018                                                 
			NOP			W R						XSYS  				XS<XD*2	YS<YD*2		; //Write : XS 0x0008 YC 0x001A Read : XS 0x0008 YC 0x001A                                                 
			NOP			W R						XSYS  				XS<XE*2	YS<YE*2		; //Write : XS 0x0010 YC 0x001C Read : XS 0x0010 YC 0x001C                                                 
			NOP			W R						XSYS  				XS<XF*2	YS<YF*2		; //Write : XS 0x0020 YC 0x001F Read : XS 0x0020 YC 0x001F                                                 
			NOP			W R						XSYS  				XS<XB*2	YS<YB*2		; //Write : XS 0x0040 YC 0x0140 Read : XS 0x0040 YC 0x0140                                                 
			NOP			W R						XSYS  									; //Write : XS 0x0080 YC 0x0180 Read : XS 0x0080 YC 0x0180                                                 
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
		
			START XS_ALU_AD2_00;
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

			NOP		  XS<0x0002		YS<0x000A													;                                                                  
			NOP		  XC<0x0004		YC<0x00A0													;                                                                  
			NOP		  XK<0x0008		YK<0x0A00													;                                                                  
			NOP		  XA<0x0010		YA<0xA000													;                                                                  
			NOP		  XD<0x0020		YD<0xAAAA													;                                                                  
			NOP		  XE<0x0040		YE<0xAAA0	 												;                                                                  
			NOP		  XF<0x0080		YF<0xAA0A													;                                                                  
			NOP		  XB<0x0100		YB<0xA0AA													;                                                                  
			NOP																			;                                                 
			NOP														XS<XS/2	YS<YS/2		;                                                 
			NOP			W R						XSYS  				XS<XC/2	YS<YC/2		; //Write : XS 0x0001 YC 0x0005 Read : XS 0x0001 YC 0x0005                                                 
			NOP			W R						XSYS  				XS<XK/2	YS<YK/2		; //Write : XS 0x0001 YC 0x0005 Read : XS 0x0001 YC 0x0005                                                 
			NOP			W R						XSYS  				XS<XA/2	YS<YA/2		; //Write : XS 0x0004 YC 0x0050 Read : XS 0x0004 YC 0x0050                                                 
			NOP			W R						XSYS  				XS<XD/2	YS<YD/2		; //Write : XS 0x0008 YC 0x0500 Read : XS 0x0008 YC 0x0500                                                 
			NOP			W R						XSYS  				XS<XE/2	YS<YE/2		; //Write : XS 0x0010 YC 0x5555 Read : XS 0x0010 YC 0x5555                                                 
			NOP			W R						XSYS  				XS<XF/2	YS<YF/2		; //Write : XS 0x0020 YC 0x5550 Read : XS 0x0020 YC 0x5550                                                 
			NOP			W R						XSYS  				XS<XB/2	YS<YB/2		; //Write : XS 0x0040 YC 0x5505 Read : XS 0x0040 YC 0x5505                                                 
			NOP			W R						XSYS  									; //Write : XS 0x0080 YC 0x5055 Read : XS 0x0080 YC 0x5055                                                 
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
		
			START XS_ALU_B_00;
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

			NOP		  D1A<0x0000														;                                                                  
			NOP		  D1B<0x0001														;                                                                  
			NOP		  D1C<0x0002														;                                                                  
			NOP		  D1D<0x0004														;                                                                  
			NOP		  D1E<0x0008														;                                                                  
			NOP		  D1F<0x0010	D4B<0x1000		 									;                                                                  
			NOP		  D1G<0x0020														;                                                                  
			NOP		  D1H<0x0040														;                                                                  
			NOP		  D2A<0x0080														;                                                                  
			NOP		  D2B<0x0100														;                                                                  
			NOP		  D2C<0x0200														;                                                                  
			NOP		  D2D<0x0400	D3B<0x0800											;                                                                  
			NOP		   D3<D3B															;                                                                  
			NOP		   D4<D4B															;                                                                  
			;NOP		 XMAX<0xFFFF	YMAX<0xFFFF 										;                                                                  
			NOP																			;                                                 
			NOP														XS<D1A	 YS<D2A		;                                                 
			NOP			W R						XSYS  				XS<D1B	 YS<D2B		; //Write : XS 0x0000 YC 0x0080 Read : XS 0x0000 YC 0x0080                                                   
			NOP			W R						XSYS  				XS<D1C	 YS<D2C		; //Write : XS 0x0001 YC 0x0100 Read : XS 0x0001 YC 0x0100                                                 
			NOP			W R						XSYS  				XS<D1D	 YS<D2D		; //Write : XS 0x0002 YC 0x0200 Read : XS 0x0002 YC 0x0200                                                 
			NOP			W R						XSYS  				XS<D1E	 YS<D1A		; //Write : XS 0x0004 YC 0x0400 Read : XS 0x0004 YC 0x0400                                                 
			NOP			W R						XSYS  				XS<D1F	 YS<D1B		; //Write : XS 0x0008 YC 0x0000 Read : XS 0x0008 YC 0x0000                                                 
			NOP			W R						XSYS  				XS<D1G	 YS<D1C		; //Write : XS 0x0010 YC 0x0001 Read : XS 0x0010 YC 0x0001                                                 
			NOP			W R						XSYS  				XS<D1H	 YS<D1D		; //Write : XS 0x0020 YC 0x0002 Read : XS 0x0020 YC 0x0002                                                 
			NOP			W R						XSYS  				XS<D2A	 YS<D1E		; //Write : XS 0x0040 YC 0x0004 Read : XS 0x0040 YC 0x0004                                                 
			NOP			W R						XSYS  				XS<D2B	 YS<D1F		; //Write : XS 0x0080 YC 0x0008 Read : XS 0x0080 YC 0x0008                                                 
			NOP			W R						XSYS  				XS<D2C	 YS<D1G		; //Write : XS 0x0100 YC 0x0010 Read : XS 0x0100 YC 0x0010                                                 
			NOP			W R						XSYS  				XS<D2D	 YS<D1H		; //Write : XS 0x0200 YC 0x0020 Read : XS 0x0200 YC 0x0020                                                 
			NOP			W R						XSYS  				XS<D3	 YS<D4		; //Write : XS 0x0400 YC 0x0040 Read : XS 0x0400 YC 0x0040                                                 
			NOP			W R						XSYS  				XS<D4	 YS<D3		; //Write : XS 0x0800 YC 0x1000 Read : XS 0x0800 YC 0x1000                                                 
			NOP			W R						XSYS  				XS<XMAX	 YS<YMAX	; //Write : XS 0x1000 YC 0x0800 Read : XS 0x1000 YC 0x0800                                                 
			NOP			W R						XSYS  									; //Write : XS 0xFFFF YC 0xFFFF Read : XS 0xFFFF YC 0xFFFF                                                 
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
	   
			START XS_ALU_APB_00;
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
			NOP		  D1A<0x0001   D4B<0x0001														;                                                                  
			NOP		  D1B<0x0010   													;                                                                  
			NOP		  D1C<0x0100														;                                                                  
			NOP		  D1D<0x0001														;                                                                  
			NOP		  D1E<0x0010														;                                                                  
			NOP		  D1F<0x0100			 											;                                                                  
			NOP		  D1G<0x0001														;                                                                  
			NOP		  D1H<0x0010														;                                                                  
			NOP		  D2A<0x0100	D3B<0x0010													;                                                                  
			NOP		  D2B<0x0001														;                                                                  
			NOP		  D2C<0x0010														;                                                                  
			NOP		  D2D<0x0100														;                                                                  
			NOP		   D3<D3B														;                                                                  
			NOP		   D4<D4B														;                                                                  
			NOP																			;                                                 
			NOP														XS<XC+D1A	YS<YC+D2A;                                                 
			NOP			W R						XSYS  				XS<XC+D1B	YS<YC+D2B; //Write : X 0x0001 Y 0x8100 Read : X 0x0001 Y 0x8100                                                 
			NOP			W R						XSYS  				XS<XC+D1C	YS<YC+D2C; //Write : X 0x0010 Y 0x8001 Read : X 0x0010 Y 0x8001                                                 
			NOP			W R						XSYS  				XS<XC+D1D	YS<YC+D2D; //Write : X 0x0100 Y 0x8010 Read : X 0x0100 Y 0x8010                                                 
			NOP			W R						XSYS  				XS<XC+D1E	YS<YC+D1A; //Write : X 0x0001 Y 0x8100 Read : X 0x0001 Y 0x8100                                                 
			NOP			W R						XSYS  				XS<XC+D1F	YS<YC+D1B; //Write : X 0x0010 Y 0x8001 Read : X 0x0010 Y 0x8001                                                
			NOP			W R						XSYS  				XS<XC+D1G	YS<YC+D1C; //Write : X 0x0001 Y 0x8010 Read : X 0x0001 Y 0x8010                                                 
			NOP			W R						XSYS  				XS<XC+D1H	YS<YC+D1D; //Write : X 0x0100 Y 0x8100 Read : X 0x0100 Y 0x8100                                                 
			NOP			W R						XSYS  				XS<XC+D2A	YS<YC+D1E; //Write : X 0x0010 Y 0x8001 Read : X 0x0010 Y 0x8001                                                 
			NOP			W R						XSYS  				XS<XC+D2B	YS<YC+D1F; //Write : X 0x0100 Y 0x8010 Read : X 0x0100 Y 0x8010                                                 
			NOP			W R						XSYS  				XS<XC+D2C	YS<YC+D1G; //Write : X 0x0001 Y 0x8100 Read : X 0x0001 Y 0x8100                                                 
			NOP			W R						XSYS  				XS<XC+D2D	YS<YC+D1H; //Write : X 0x0010 Y 0x8001 Read : X 0x0010 Y 0x8001                                                 
			NOP			W R						XSYS  				XS<XC+D3	YS<YC+D4 ; //Write : X 0x0001 Y 0x8100 Read : X 0x0001 Y 0x8100                                                 
			NOP			W R						XSYS  				XS<XC+D4	YS<YC+D3 ; //Write : X 0x0010 Y 0x8001 Read : X 0x0010 Y 0x8001                                                 
			NOP			W R						XSYS  									 ; //Write : X 0x0001 Y 0x8010 Read : X 0x0001 Y 0x8010                                                
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
	
	
			START XS_ALU_APB_01;
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
			NOP		  D1A<0x0001   D4B<0x0001														;                                                                  
			NOP		  D1B<0x0010   													;                                                                  
			NOP		  D1C<0x0100														;                                                                  
			NOP		  D1D<0x0001														;                                                                  
			NOP		  D1E<0x0010														;                                                                  
			NOP		  D1F<0x0100			 											;                                                                  
			NOP		  D1G<0x0001														;                                                                  
			NOP		  D1H<0x0010														;                                                                  
			NOP		  D2A<0x0100   D3B<0x0010														;                                                                  
			NOP		  D2B<0x0001														;                                                                  
			NOP		  D2C<0x0010														;                                                                  
			NOP		  D2D<0x0100														;                                                                  
			NOP		   D3<D3B														;                                                                  
			NOP		   D4<D4B														;                                                                  
			NOP																			;                                                 
			NOP														XS<XS+D1A	YS<YS+D2A;                                                 
			NOP			W R						XSYS  				XS<XS+D1B	YS<YS+D2B; //Write : X 0x1001 Y 0x9100 Read : X 0x1001 Y 0x9100                                                 
			NOP			W R						XSYS  				XS<XS+D1C	YS<YS+D2C; //Write : X 0x1011 Y 0x9101 Read : X 0x1011 Y 0x9101                                                 
			NOP			W R						XSYS  				XS<XS+D1D	YS<YS+D2D; //Write : X 0x1111 Y 0x9111 Read : X 0x1111 Y 0x9111                                                 
			NOP			W R						XSYS  				XS<XS+D1E	YS<YS+D1A; //Write : X 0x1112 Y 0x9211 Read : X 0x1012 Y 0x9211                                                 
			NOP			W R						XSYS  				XS<XS+D1F	YS<YS+D1B; //Write : X 0x1122 Y 0x9212 Read : X 0x1022 Y 0x9212                                                 
			NOP			W R						XSYS  				XS<XS+D1G	YS<YS+D1C; //Write : X 0x1222 Y 0x9222 Read : X 0x1222 Y 0x9222                                                 
			NOP			W R						XSYS  				XS<XS+D1H	YS<YS+D1D; //Write : X 0x1223 Y 0x9322 Read : X 0x1223 Y 0x9322                                                 
			NOP			W R						XSYS  				XS<XS+D2A	YS<YS+D1E; //Write : X 0x1233 Y 0x9323 Read : X 0x1233 Y 0x9323                                                 
			NOP			W R						XSYS  				XS<XS+D2B	YS<YS+D1F; //Write : X 0x1333 Y 0x9333 Read : X 0x1333 Y 0x9333                                                 
			NOP			W R						XSYS  				XS<XS+D2C	YS<YS+D1G; //Write : X 0x1334 Y 0x9433 Read : X 0x1334 Y 0x9433                                                 
			NOP			W R						XSYS  				XS<XS+D2D	YS<YS+D1H; //Write : X 0x1344 Y 0x9434 Read : X 0x1344 Y 0x9434                                                 
			NOP			W R						XSYS  				XS<XS+D3	YS<YS+D4 ; //Write : X 0x1444 Y 0x9444 Read : X 0x1444 Y 0x9444                                                 
			NOP			W R						XSYS  				XS<XS+D4	YS<YS+D3 ; //Write : X 0x1454 Y 0x9445 Read : X 0x1454 Y 0x9445                                                 
			NOP			W R						XSYS  									 ; //Write : X 0x1455 Y 0x9455 Read : X 0x1455 Y 0x9455                                                 
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
	
			START XS_ALU_APB_02;
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
			NOP		  D1A<0x0001   D4B<0x0001														;                                                                  
			NOP		  D1B<0x0010   													;                                                                  
			NOP		  D1C<0x0100														;                                                                  
			NOP		  D1D<0x0001														;                                                                  
			NOP		  D1E<0x0010														;                                                                  
			NOP		  D1F<0x0100			 											;                                                                  
			NOP		  D1G<0x0001														;                                                                  
			NOP		  D1H<0x0010														;                                                                  
			NOP		  D2A<0x0100   D3B<0x0010													;                                                                  
			NOP		  D2B<0x0001														;                                                                  
			NOP		  D2C<0x0010														;                                                                  
			NOP		  D2D<0x0100														;                                                                  
			NOP		   D3<D3B														;                                                                  
			NOP		   D4<D4B														;                                                                  
			NOP																			;                                                 
			NOP																			;                                                 
			NOP														XS<XK+D1A	YS<YK+D2A;                                                 
			NOP			W R						XSYS  				XS<XK+D1B	YS<YK+D2B; //Write : X 0x2001 Y 0xA100 Read : X 0x2001 Y 0xA100                                                 
			NOP			W R						XSYS  				XS<XK+D1C	YS<YK+D2C; //Write : X 0x2011 Y 0xA101 Read : X 0x2011 Y 0xA101                                                 
			NOP			W R						XSYS  				XS<XK+D1D	YS<YK+D2D; //Write : X 0x2111 Y 0xA111 Read : X 0x2111 Y 0xA111                                                  
			NOP			W R						XSYS  				XS<XK+D1E	YS<YK+D1A; //Write : X 0x2112 Y 0xA211 Read : X 0x2112 Y 0xA211                                                 
			NOP			W R						XSYS  				XS<XK+D1F	YS<YK+D1B; //Write : X 0x2122 Y 0xA212 Read : X 0x2122 Y 0xA212                                                 
			NOP			W R						XSYS  				XS<XK+D1G	YS<YK+D1C; //Write : X 0x2222 Y 0xA222 Read : X 0x2222 Y 0xA222                                                 
			NOP			W R						XSYS  				XS<XK+D1H	YS<YK+D1D; //Write : X 0x2223 Y 0xA322 Read : X 0x2223 Y 0xA322                                                 
			NOP			W R						XSYS  				XS<XK+D2A	YS<YK+D1E; //Write : X 0x2233 Y 0xA323 Read : X 0x2233 Y 0xA323                                                   
			NOP			W R						XSYS  				XS<XK+D2B	YS<YK+D1F; //Write : X 0x2333 Y 0xA333 Read : X 0x2333 Y 0xA333                                                  
			NOP			W R						XSYS  				XS<XK+D2C	YS<YK+D1G; //Write : X 0x2334 Y 0xA433 Read : X 0x2334 Y 0xA433                                                 
			NOP			W R						XSYS  				XS<XK+D2D	YS<YK+D1H; //Write : X 0x2344 Y 0xA434 Read : X 0x2344 Y 0xA434                                                  
			NOP			W R						XSYS  				XS<XK+D3	YS<YK+D4 ; //Write : X 0x2444 Y 0xA444 Read : X 0x2444 Y 0xA444                                                 
			NOP			W R						XSYS  				XS<XK+D4	YS<YK+D3 ; //Write : X 0x2454 Y 0xA445 Read : X 0x2454 Y 0xA445                                                 
			NOP			W R						XSYS  									 ; //Write : X 0x2455 Y 0xA455 Read : X 0x2455 Y 0xA455                                                 
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
	

			START XS_ALU_APB_03;
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
			NOP		  D1A<0x0001   D4B<0x0001														;                                                                  
			NOP		  D1B<0x0010   													;                                                                  
			NOP		  D1C<0x0100														;                                                                  
			NOP		  D1D<0x0001														;                                                                  
			NOP		  D1E<0x0010														;                                                                  
			NOP		  D1F<0x0100			 											;                                                                  
			NOP		  D1G<0x0001														;                                                                  
			NOP		  D1H<0x0010														;                                                                  
			NOP		  D2A<0x0100   D3B<0x0010													;                                                                  
			NOP		  D2B<0x0001														;                                                                  
			NOP		  D2C<0x0010														;                                                                  
			NOP		  D2D<0x0100														;                                                                  
			NOP		   D3<D3B														;                                                                  
			NOP		   D4<D4B														;                                                                  
			NOP																			;                                                 
			NOP														XS<XA+D1A	YS<YA+D2A;                                                 
			NOP			W R						XSYS  				XS<XA+D1B	YS<YA+D2B; //Write : X 0x3001 Y 0xB100 Read : X 0x3001 Y 0xB100                                                  
			NOP			W R						XSYS  				XS<XA+D1C	YS<YA+D2C; //Write : X 0x3011 Y 0xB101 Read : X 0x3011 Y 0xB101                                                 
			NOP			W R						XSYS  				XS<XA+D1D	YS<YA+D2D; //Write : X 0x3111 Y 0xB111 Read : X 0x3111 Y 0xB111                                                 
			NOP			W R						XSYS  				XS<XA+D1E	YS<YA+D1A; //Write : X 0x3112 Y 0xB211 Read : X 0x3112 Y 0xB211                                                 
			NOP			W R						XSYS  				XS<XA+D1F	YS<YA+D1B; //Write : X 0x3122 Y 0xB212 Read : X 0x3122 Y 0xB212                                                 
			NOP			W R						XSYS  				XS<XA+D1G	YS<YA+D1C; //Write : X 0x3222 Y 0xB222 Read : X 0x3222 Y 0xB222                                                 
			NOP			W R						XSYS  				XS<XA+D1H	YS<YA+D1D; //Write : X 0x3223 Y 0xB322 Read : X 0x3223 Y 0xB322                                                 
			NOP			W R						XSYS  				XS<XA+D2A	YS<YA+D1E; //Write : X 0x3233 Y 0xB323 Read : X 0x3233 Y 0xB323                                                 
			NOP			W R						XSYS  				XS<XA+D2B	YS<YA+D1F; //Write : X 0x3333 Y 0xB333 Read : X 0x3333 Y 0xB333                                                 
			NOP			W R						XSYS  				XS<XA+D2C	YS<YA+D1G; //Write : X 0x3334 Y 0xB433 Read : X 0x3334 Y 0xB433                                                 
			NOP			W R						XSYS  				XS<XA+D2D	YS<YA+D1H; //Write : X 0x3344 Y 0xB434 Read : X 0x3344 Y 0xB434                                                 
			NOP			W R						XSYS  				XS<XA+D3	YS<YA+D4 ; //Write : X 0x3444 Y 0xB444 Read : X 0x3444 Y 0xB444                                                 
			NOP			W R						XSYS  				XS<XA+D4	YS<YA+D3 ; //Write : X 0x3454 Y 0xB445 Read : X 0x3454 Y 0xB445                                                 
			NOP			W R						XSYS  									 ; //Write : X 0x3455 Y 0xB455 Read : X 0x3455 Y 0xB455                                                 
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


			START XS_ALU_APB_04;
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
			NOP		  D1A<0x0001   D4B<0x0001														;                                                                  
			NOP		  D1B<0x0010   													;                                                                  
			NOP		  D1C<0x0100														;                                                                  
			NOP		  D1D<0x0001														;                                                                  
			NOP		  D1E<0x0010														;                                                                  
			NOP		  D1F<0x0100			 											;                                                                  
			NOP		  D1G<0x0001														;                                                                  
			NOP		  D1H<0x0010														;                                                                  
			NOP		  D2A<0x0100	D3B<0x0010													;                                                                  
			NOP		  D2B<0x0001														;                                                                  
			NOP		  D2C<0x0010														;                                                                  
			NOP		  D2D<0x0100														;                                                                  
			NOP		   D3<D3B														;                                                                  
			NOP		   D4<D4B														;                                                                  
			NOP																			;                                                 
			NOP														XS<XD+D1A	YS<YD+D2A;                                                 
			NOP			W R						XSYS  				XS<XD+D1B	YS<YD+D2B; //Write : X 0x4001 Y 0xC100 Read : X 0x4001 Y 0xC100                                                 
			NOP			W R						XSYS  				XS<XD+D1C	YS<YD+D2C; //Write : X 0x4011 Y 0xC101 Read : X 0x4011 Y 0xC101                                                 
			NOP			W R						XSYS  				XS<XD+D1D	YS<YD+D2D; //Write : X 0x4111 Y 0xC111 Read : X 0x4111 Y 0xC111                                                 
			NOP			W R						XSYS  				XS<XD+D1E	YS<YD+D1A; //Write : X 0x4112 Y 0xC211 Read : X 0x4112 Y 0xC211                                                 
			NOP			W R						XSYS  				XS<XD+D1F	YS<YD+D1B; //Write : X 0x4122 Y 0xC212 Read : X 0x4122 Y 0xC212                                                 
			NOP			W R						XSYS  				XS<XD+D1G	YS<YD+D1C; //Write : X 0x4222 Y 0xC222 Read : X 0x4222 Y 0xC222                                                 
			NOP			W R						XSYS  				XS<XD+D1H	YS<YD+D1D; //Write : X 0x4223 Y 0xC322 Read : X 0x4223 Y 0xC322                                                 
			NOP			W R						XSYS  				XS<XD+D2A	YS<YD+D1E; //Write : X 0x4233 Y 0xC323 Read : X 0x4233 Y 0xC323                                                 
			NOP			W R						XSYS  				XS<XD+D2B	YS<YD+D1F; //Write : X 0x4333 Y 0xC333 Read : X 0x4333 Y 0xC333                                                 
			NOP			W R						XSYS  				XS<XD+D2C	YS<YD+D1G; //Write : X 0x4334 Y 0xC433 Read : X 0x4334 Y 0xC433                                                 
			NOP			W R						XSYS  				XS<XD+D2D	YS<YD+D1H; //Write : X 0x4344 Y 0xC434 Read : X 0x4344 Y 0xC434                                                 
			NOP			W R						XSYS  				XS<XD+D3	YS<YD+D4 ; //Write : X 0x4444 Y 0xC444 Read : X 0x4444 Y 0xC444                                                 
			NOP			W R						XSYS  				XS<XD+D4	YS<YD+D3 ; //Write : X 0x4454 Y 0xC445 Read : X 0x4454 Y 0xC445                                                 
			NOP			W R						XSYS  									 ; //Write : X 0x4455 Y 0xC455 Read : X 0x4455 Y 0xC455                                                 
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
	
			START XS_ALU_APB_05;
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
			NOP		  D1A<0x0001   D4B<0x0001														;                                                                  
			NOP		  D1B<0x0010   													;                                                                  
			NOP		  D1C<0x0100														;                                                                  
			NOP		  D1D<0x0001														;                                                                  
			NOP		  D1E<0x0010														;                                                                  
			NOP		  D1F<0x0100			 											;                                                                  
			NOP		  D1G<0x0001														;                                                                  
			NOP		  D1H<0x0010														;                                                                  
			NOP		  D2A<0x0100   D3B<0x0010													;                                                                  
			NOP		  D2B<0x0001														;                                                                  
			NOP		  D2C<0x0010														;                                                                  
			NOP		  D2D<0x0100														;                                                                  
			NOP		   D3<D3B														;                                                                  
			NOP		   D4<D4B														;                                                                  
			NOP																			;                                                 
			NOP														XS<XE+D1A	YS<YE+D2A;                                                 
			NOP			W R						XSYS  				XS<XE+D1B	YS<YE+D2B; //Write : X 0x5001 Y 0xD100 Read : X 0x5001 Y 0xD100                                                 
			NOP			W R						XSYS  				XS<XE+D1C	YS<YE+D2C; //Write : X 0x5011 Y 0xD101 Read : X 0x5011 Y 0xD101                                                 
			NOP			W R						XSYS  				XS<XE+D1D	YS<YE+D2D; //Write : X 0x5111 Y 0xD111 Read : X 0x5111 Y 0xD111                                                 
			NOP			W R						XSYS  				XS<XE+D1E	YS<YE+D1A; //Write : X 0x5112 Y 0xD211 Read : X 0x5112 Y 0xD211                                                 
			NOP			W R						XSYS  				XS<XE+D1F	YS<YE+D1B; //Write : X 0x5122 Y 0xD212 Read : X 0x5122 Y 0xD212                                                 
			NOP			W R						XSYS  				XS<XE+D1G	YS<YE+D1C; //Write : X 0x5222 Y 0xD222 Read : X 0x5222 Y 0xD222                                                 
			NOP			W R						XSYS  				XS<XE+D1H	YS<YE+D1D; //Write : X 0x5223 Y 0xD322 Read : X 0x5223 Y 0xD322                                                 
			NOP			W R						XSYS  				XS<XE+D2A	YS<YE+D1E; //Write : X 0x5233 Y 0xD323 Read : X 0x5233 Y 0xD323                                                 
			NOP			W R						XSYS  				XS<XE+D2B	YS<YE+D1F; //Write : X 0x5333 Y 0xD333 Read : X 0x5333 Y 0xD333                                                 
			NOP			W R						XSYS  				XS<XE+D2C	YS<YE+D1G; //Write : X 0x5334 Y 0xD433 Read : X 0x5334 Y 0xD433                                                 
			NOP			W R						XSYS  				XS<XE+D2D	YS<YE+D1H; //Write : X 0x5344 Y 0xD434 Read : X 0x5344 Y 0xD434                                                 
			NOP			W R						XSYS  				XS<XE+D3	YS<YE+D4 ; //Write : X 0x5444 Y 0xD444 Read : X 0x5444 Y 0xD444                                                 
			NOP			W R						XSYS  				XS<XE+D4	YS<YE+D3 ; //Write : X 0x5454 Y 0xD445 Read : X 0x5454 Y 0xD445                                                 
			NOP			W R						XSYS  									 ; //Write : X 0x5455 Y 0xD455 Read : X 0x5455 Y 0xD455                                                 
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
	
			START XS_ALU_APB_06;
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
			NOP		  D1A<0x0001   D4B<0x0001														;                                                                  
			NOP		  D1B<0x0010   													;                                                                  
			NOP		  D1C<0x0100														;                                                                  
			NOP		  D1D<0x0001														;                                                                  
			NOP		  D1E<0x0010														;                                                                  
			NOP		  D1F<0x0100			 											;                                                                  
			NOP		  D1G<0x0001														;                                                                  
			NOP		  D1H<0x0010														;                                                                  
			NOP		  D2A<0x0100   D3B<0x0010														;                                                                  
			NOP		  D2B<0x0001														;                                                                  
			NOP		  D2C<0x0010														;                                                                  
			NOP		  D2D<0x0100														;                                                                  
			NOP		   D3<D3B														;                                                                  
			NOP		   D4<D4B														;                                                                  
			NOP																			;                                                 
			NOP														XS<XF+D1A	YS<YF+D2A;                                                 
			NOP			W R						XSYS  				XS<XF+D1B	YS<YF+D2B; //Write : X 0x6001 Y 0xE100 Read : X 0x6001 Y 0xE100                                                 
			NOP			W R						XSYS  				XS<XF+D1C	YS<YF+D2C; //Write : X 0x6011 Y 0xE101 Read : X 0x6011 Y 0xE101                                                 
			NOP			W R						XSYS  				XS<XF+D1D	YS<YF+D2D; //Write : X 0x6111 Y 0xE111 Read : X 0x6111 Y 0xE111                                                 
			NOP			W R						XSYS  				XS<XF+D1E	YS<YF+D1A; //Write : X 0x6112 Y 0xE211 Read : X 0x6112 Y 0xE211                                                 
			NOP			W R						XSYS  				XS<XF+D1F	YS<YF+D1B; //Write : X 0x6122 Y 0xE212 Read : X 0x6122 Y 0xE212                                                 
			NOP			W R						XSYS  				XS<XF+D1G	YS<YF+D1C; //Write : X 0x6222 Y 0xE222 Read : X 0x6222 Y 0xE222                                                 
			NOP			W R						XSYS  				XS<XF+D1H	YS<YF+D1D; //Write : X 0x6223 Y 0xE322 Read : X 0x6223 Y 0xE322                                                 
			NOP			W R						XSYS  				XS<XF+D2A	YS<YF+D1E; //Write : X 0x6233 Y 0xE323 Read : X 0x6233 Y 0xE323                                                 
			NOP			W R						XSYS  				XS<XF+D2B	YS<YF+D1F; //Write : X 0x6333 Y 0xE333 Read : X 0x6333 Y 0xE333                                                 
			NOP			W R						XSYS  				XS<XF+D2C	YS<YF+D1G; //Write : X 0x6334 Y 0xE433 Read : X 0x6334 Y 0xE433                                                 
			NOP			W R						XSYS  				XS<XF+D2D	YS<YF+D1H; //Write : X 0x6344 Y 0xE434 Read : X 0x6344 Y 0xE434                                                 
			NOP			W R						XSYS  				XS<XF+D3	YS<YF+D4 ; //Write : X 0x6444 Y 0xE444 Read : X 0x6444 Y 0xE444                                                 
			NOP			W R						XSYS  				XS<XF+D4	YS<YF+D3 ; //Write : X 0x6454 Y 0xE445 Read : X 0x6454 Y 0xE445                                                 
			NOP			W R						XSYS  									 ; //Write : X 0x6455 Y 0xE455 Read : X 0x6455 Y 0xE455                                                 
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
	
			START XS_ALU_APB_07;
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
			NOP		  D1A<0x0001   D4B<0x0001														;                                                                  
			NOP		  D1B<0x0010   													;                                                                  
			NOP		  D1C<0x0100														;                                                                  
			NOP		  D1D<0x0001														;                                                                  
			NOP		  D1E<0x0010														;                                                                  
			NOP		  D1F<0x0100			 											;                                                                  
			NOP		  D1G<0x0001														;                                                                  
			NOP		  D1H<0x0010														;                                                                  
			NOP		  D2A<0x0100   D3B<0x0010														;                                                                  
			NOP		  D2B<0x0001														;                                                                  
			NOP		  D2C<0x0010														;                                                                  
			NOP		  D2D<0x0100														;                                                                  
			NOP		   D3<D3B														;                                                                  
			NOP		   D4<D4B														;                                                                  
			NOP																			;                                                 
			NOP														XS<XB+D1A	YS<YB+D2A;                                                 
			NOP			W R						XSYS  				XS<XB+D1B	YS<YB+D2B; //Write : X 0x7001 Y 0xF100 Read : X 0x7001 Y 0xF100                                                 
			NOP			W R						XSYS  				XS<XB+D1C	YS<YB+D2C; //Write : X 0x7011 Y 0xF101 Read : X 0x7011 Y 0xF101                                                 
			NOP			W R						XSYS  				XS<XB+D1D	YS<YB+D2D; //Write : X 0x7111 Y 0xF111 Read : X 0x7111 Y 0xF111                                                 
			NOP			W R						XSYS  				XS<XB+D1E	YS<YB+D1A; //Write : X 0x7112 Y 0xF211 Read : X 0x7112 Y 0xF211                                                 
			NOP			W R						XSYS  				XS<XB+D1F	YS<YB+D1B; //Write : X 0x7122 Y 0xF212 Read : X 0x7122 Y 0xF212                                                 
			NOP			W R						XSYS  				XS<XB+D1G	YS<YB+D1C; //Write : X 0x7222 Y 0xF222 Read : X 0x7222 Y 0xF222                                                 
			NOP			W R						XSYS  				XS<XB+D1H	YS<YB+D1D; //Write : X 0x7223 Y 0xF322 Read : X 0x7223 Y 0xF322                                                 
			NOP			W R						XSYS  				XS<XB+D2A	YS<YB+D1E; //Write : X 0x7233 Y 0xF323 Read : X 0x7233 Y 0xF323                                                 
			NOP			W R						XSYS  				XS<XB+D2B	YS<YB+D1F; //Write : X 0x7333 Y 0xF333 Read : X 0x7333 Y 0xF333                                                 
			NOP			W R						XSYS  				XS<XB+D2C	YS<YB+D1G; //Write : X 0x7334 Y 0xF433 Read : X 0x7334 Y 0xF433                                                 
			NOP			W R						XSYS  				XS<XB+D2D	YS<YB+D1H; //Write : X 0x7344 Y 0xF434 Read : X 0x7344 Y 0xF434                                                 
			NOP			W R						XSYS  				XS<XB+D3	YS<YB+D4 ; //Write : X 0x7444 Y 0xF444 Read : X 0x7444 Y 0xF444                                                 
			NOP			W R						XSYS  				XS<XB+D4	YS<YB+D3 ; //Write : X 0x7454 Y 0xF445 Read : X 0x7454 Y 0xF445                                                 
			NOP			W R						XSYS  									 ; //Write : X 0x7455 Y 0xF455 Read : X 0x7455 Y 0xF455                                                 
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
	
			START XS_ALU_AMB_00;
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

			NOP		   XC<0x0666	YC<0x0555													;                                                                  
			NOP		   XS<0x0777	YS<0x0666													;                                                                  
			NOP		   XK<0x0888	YK<0x0777													;                                                                  
			NOP		   XA<0x0999	YA<0x0888													;                                                                  
			NOP		   XD<0x0AAA	YD<0x0999													;                                                                  
			NOP		   XE<0x0BBB	YE<0x0AAA		 											;                                                                  
			NOP		   XF<0x0CCC	YF<0x0BBB													;                                                                  
			NOP		   XB<0x0DDD	YB<0x0CCC													;                                                                  
			NOP		  D1A<0x0001   D4B<0x0001														;                                                                  
			NOP		  D1B<0x0010   													;                                                                  
			NOP		  D1C<0x0100														;                                                                  
			NOP		  D1D<0x0001														;                                                                  
			NOP		  D1E<0x0010														;                                                                  
			NOP		  D1F<0x0100			 											;                                                                  
			NOP		  D1G<0x0001														;                                                                  
			NOP		  D1H<0x0010														;                                                                  
			NOP		  D2A<0x0100   D3B<0x0010														;                                                                  
			NOP		  D2B<0x0001														;                                                                  
			NOP		  D2C<0x0010														;                                                                  
			NOP		  D2D<0x0100														;                                                                  
			NOP		   D3<D3B														;                                                                  
			NOP		   D4<D4B														;                                                                  
			NOP																			;                                                 
			NOP														XS<XC-D1A	YS<YC-D1A;                                                 
			NOP			W R						XSYS  				XS<XC-D1B	YS<YC-D1B; //Write : X 0x0665 Y 0x0554 Read : X 0x0665 Y 0x0554                                                 
			NOP			W R						XSYS  				XS<XC-D1C	YS<YC-D1C; //Write : X 0x0655 Y 0x0544 Read : X 0x0655 Y 0x0544                                                 
			NOP			W R						XSYS  				XS<XC-D1D	YS<YC-D1D; //Write : X 0x0555 Y 0x0444 Read : X 0x0555 Y 0x0444                                                 
			NOP			W R						XSYS  				XS<XC-D1E	YS<YC-D1E; //Write : X 0x0554 Y 0x0443 Read : X 0x0554 Y 0x0443                                                 
			NOP			W R						XSYS  				XS<XC-D1F	YS<YC-D1F; //Write : X 0x0544 Y 0x0433 Read : X 0x0544 Y 0x0433                                                 
			NOP			W R						XSYS  				XS<XC-D1G	YS<YC-D1G; //Write : X 0x0444 Y 0x0333 Read : X 0x0444 Y 0x0333                                                 
			NOP			W R						XSYS  				XS<XC-D1H	YS<YC-D1H; //Write : X 0x0443 Y 0x0332 Read : X 0x0443 Y 0x0332                                                 
			NOP			W R						XSYS  				XS<XC-D2A	YS<YC-D2A; //Write : X 0x0433 Y 0x0322 Read : X 0x0433 Y 0x0322                                                 
			NOP			W R						XSYS  				XS<XC-D2B	YS<YC-D2B; //Write : X 0x0333 Y 0x0222 Read : X 0x0333 Y 0x0222                                                 
			NOP			W R						XSYS  				XS<XC-D2C	YS<YC-D2C; //Write : X 0x0332 Y 0x0221 Read : X 0x0332 Y 0x0221                                                 
			NOP			W R						XSYS  				XS<XC-D2D	YS<YC-D2D; //Write : X 0x0322 Y 0x0211 Read : X 0x0322 Y 0x0211                                                 
			NOP			W R						XSYS  				XS<XC-D3	YS<YC-D3 ; //Write : X 0x0222 Y 0x0111 Read : X 0x0222 Y 0xF444                                                 
			NOP			W R						XSYS  				XS<XC-D4	YS<YC-D4 ; //Write : X 0x0212 Y 0x0101 Read : X 0x0212 Y 0x0101                                                 
			NOP			W R						XSYS  									 ; //Write : X 0x0211 Y 0x0101 Read : X 0x0211 Y 0x0101                                                 
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

			START XS_ALU_AMB_01;
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

			NOP		   XC<0x0666	YC<0x0555													;                                                                  
			NOP		   XS<0x0777	YS<0x0666													;                                                                  
			NOP		   XK<0x0888	YK<0x0777													;                                                                  
			NOP		   XA<0x0999	YA<0x0888													;                                                                  
			NOP		   XD<0x0AAA	YD<0x0999													;                                                                  
			NOP		   XE<0x0BBB	YE<0x0AAA		 											;                                                                  
			NOP		   XF<0x0CCC	YF<0x0BBB													;                                                                  
			NOP		   XB<0x0DDD	YB<0x0CCC													;                                                                  
			NOP		  D1A<0x0001   D4B<0x0001														;                                                                  
			NOP		  D1B<0x0010   													;                                                                  
			NOP		  D1C<0x0100														;                                                                  
			NOP		  D1D<0x0001														;                                                                  
			NOP		  D1E<0x0010														;                                                                  
			NOP		  D1F<0x0100			 											;                                                                  
			NOP		  D1G<0x0001														;                                                                  
			NOP		  D1H<0x0010														;                                                                  
			NOP		  D2A<0x0100   D3B<0x0010														;                                                                  
			NOP		  D2B<0x0001														;                                                                  
			NOP		  D2C<0x0010														;                                                                  
			NOP		  D2D<0x0100														;                                                                  
			NOP		   D3<D3B														;                                                                  
			NOP		   D4<D4B														;                                                                  
			NOP																			;                                                 
			NOP														XS<XS-D1A	YS<YS-D1A;                                                 
			NOP			W R						XSYS  				XS<XS-D1B	YS<YS-D1B; //Write : X 0x0776 Y 0x0665 Read : X 0x0776 Y 0x0665                                                 
			NOP			W R						XSYS  				XS<XS-D1C	YS<YS-D1C; //Write : X 0x0766 Y 0x0655 Read : X 0x0766 Y 0x0655                                                 
			NOP			W R						XSYS  				XS<XS-D1D	YS<YS-D1D; //Write : X 0x0666 Y 0x0555 Read : X 0x0666 Y 0x0555                                                 
			NOP			W R						XSYS  				XS<XS-D1E	YS<YS-D1E; //Write : X 0x0665 Y 0x0554 Read : X 0x0665 Y 0x0554                                                 
			NOP			W R						XSYS  				XS<XS-D1F	YS<YS-D1F; //Write : X 0x0655 Y 0x0544 Read : X 0x0655 Y 0x0544                                                 
			NOP			W R						XSYS  				XS<XS-D1G	YS<YS-D1G; //Write : X 0x0555 Y 0x0444 Read : X 0x0555 Y 0x0444                                                 
			NOP			W R						XSYS  				XS<XS-D1H	YS<YS-D1H; //Write : X 0x0554 Y 0x0443 Read : X 0x0554 Y 0x0443                                                 
			NOP			W R						XSYS  				XS<XS-D2A	YS<YS-D2A; //Write : X 0x0544 Y 0x0433 Read : X 0x0544 Y 0x0433                                                 
			NOP			W R						XSYS  				XS<XS-D2B	YS<YS-D2B; //Write : X 0x0444 Y 0x0333 Read : X 0x0444 Y 0x0333                                                 
			NOP			W R						XSYS  				XS<XS-D2C	YS<YS-D2C; //Write : X 0x0443 Y 0x0332 Read : X 0x0443 Y 0x0332                                                 
			NOP			W R						XSYS  				XS<XS-D2D	YS<YS-D2D; //Write : X 0x0433 Y 0x0322 Read : X 0x0433 Y 0x0322                                                 
			NOP			W R						XSYS  				XS<XS-D3	YS<YS-D3 ; //Write : X 0x0333 Y 0x0222 Read : X 0x0333 Y 0x0222                                                 
			NOP			W R						XSYS  				XS<XS-D4	YS<YS-D4 ; //Write : X 0x0323 Y 0x0212 Read : X 0x0323 Y 0x0212                                                 
			NOP			W R						XSYS  									 ; //Write : X 0x0322 Y 0x0211 Read : X 0x0322 Y 0x0211                                                 
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
	
			START XS_ALU_AMB_02;
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

			NOP		   XC<0x0666	YC<0x0555													;                                                                  
			NOP		   XS<0x0777	YS<0x0666													;                                                                  
			NOP		   XK<0x0888	YK<0x0777													;                                                                  
			NOP		   XA<0x0999	YA<0x0888													;                                                                  
			NOP		   XD<0x0AAA	YD<0x0999													;                                                                  
			NOP		   XE<0x0BBB	YE<0x0AAA		 											;                                                                  
			NOP		   XF<0x0CCC	YF<0x0BBB													;                                                                  
			NOP		   XB<0x0DDD	YB<0x0CCC													;                                                                  
			NOP		  D1A<0x0001   D4B<0x0001														;                                                                  
			NOP		  D1B<0x0010   													;                                                                  
			NOP		  D1C<0x0100														;                                                                  
			NOP		  D1D<0x0001														;                                                                  
			NOP		  D1E<0x0010														;                                                                  
			NOP		  D1F<0x0100			 											;                                                                  
			NOP		  D1G<0x0001														;                                                                  
			NOP		  D1H<0x0010														;                                                                  
			NOP		  D2A<0x0100   D3B<0x0010														;                                                                  
			NOP		  D2B<0x0001														;                                                                  
			NOP		  D2C<0x0010														;                                                                  
			NOP		  D2D<0x0100														;                                                                  
			NOP		   D3<D3B														;                                                                  
			NOP		   D4<D4B														;                                                                  
			NOP																			;                                                                  
			NOP														XS<XK-D1A	YS<YK-D1A;                                                 
			NOP			W R						XSYS  				XS<XK-D1B	YS<YK-D1B; //Write : X 0x0887 Y 0x0776 Read : X 0x0887 Y 0x0776                                                 
			NOP			W R						XSYS  				XS<XK-D1C	YS<YK-D1C; //Write : X 0x0877 Y 0x0766 Read : X 0x0877 Y 0x0766                                                 
			NOP			W R						XSYS  				XS<XK-D1D	YS<YK-D1D; //Write : X 0x0777 Y 0x0666 Read : X 0x0777 Y 0x0666                                                 
			NOP			W R						XSYS  				XS<XK-D1E	YS<YK-D1E; //Write : X 0x0776 Y 0x0665 Read : X 0x0776 Y 0x0665                                                 
			NOP			W R						XSYS  				XS<XK-D1F	YS<YK-D1F; //Write : X 0x0766 Y 0x0655 Read : X 0x0766 Y 0x0655                                                 
			NOP			W R						XSYS  				XS<XK-D1G	YS<YK-D1G; //Write : X 0x0777 Y 0x0555 Read : X 0x0777 Y 0x0555                                                 
			NOP			W R						XSYS  				XS<XK-D1H	YS<YK-D1H; //Write : X 0x0776 Y 0x0554 Read : X 0x0776 Y 0x0554                                                 
			NOP			W R						XSYS  				XS<XK-D2A	YS<YK-D2A; //Write : X 0x0766 Y 0x0544 Read : X 0x0766 Y 0x0544                                                 
			NOP			W R						XSYS  				XS<XK-D2B	YS<YK-D2B; //Write : X 0x0666 Y 0x0444 Read : X 0x0666 Y 0x0444                                                 
			NOP			W R						XSYS  				XS<XK-D2C	YS<YK-D2C; //Write : X 0x0665 Y 0x0443 Read : X 0x0665 Y 0x0443                                                 
			NOP			W R						XSYS  				XS<XK-D2D	YS<YK-D2D; //Write : X 0x0655 Y 0x0433 Read : X 0x0655 Y 0x0433                                                 
			NOP			W R						XSYS  				XS<XK-D3	YS<YK-D3 ; //Write : X 0x0555 Y 0x0333 Read : X 0x0555 Y 0x0333                                                 
			NOP			W R						XSYS  				XS<XK-D4	YS<YK-D4 ; //Write : X 0x0545 Y 0x0323 Read : X 0x0545 Y 0x0323                                                 
			NOP			W R						XSYS  									 ; //Write : X 0x0544 Y 0x0322 Read : X 0x0544 Y 0x0322                                                 
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
	
			START XS_ALU_AMB_03;
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

			NOP		   XC<0x0666	YC<0x0555													;                                                                  
			NOP		   XS<0x0777	YS<0x0666													;                                                                  
			NOP		   XK<0x0888	YK<0x0777													;                                                                  
			NOP		   XA<0x0999	YA<0x0888													;                                                                  
			NOP		   XD<0x0AAA	YD<0x0999													;                                                                  
			NOP		   XE<0x0BBB	YE<0x0AAA		 											;                                                                  
			NOP		   XF<0x0CCC	YF<0x0BBB													;                                                                  
			NOP		   XB<0x0DDD	YB<0x0CCC													;                                                                  
			NOP		  D1A<0x0001   D4B<0x0001														;                                                                  
			NOP		  D1B<0x0010   													;                                                                  
			NOP		  D1C<0x0100														;                                                                  
			NOP		  D1D<0x0001														;                                                                  
			NOP		  D1E<0x0010														;                                                                  
			NOP		  D1F<0x0100			 											;                                                                  
			NOP		  D1G<0x0001														;                                                                  
			NOP		  D1H<0x0010														;                                                                  
			NOP		  D2A<0x0100   D3B<0x0010														;                                                                  
			NOP		  D2B<0x0001														;                                                                  
			NOP		  D2C<0x0010														;                                                                  
			NOP		  D2D<0x0100														;                                                                  
			NOP		   D3<D3B														;                                                                  
			NOP		   D4<D4B														;                                                                  
			NOP																			;                                                                  
			NOP														XS<XA-D1A	YS<YA-D1A;                                                 
			NOP			W R						XSYS  				XS<XA-D1B	YS<YA-D1B; //Write : X 0x0998 Y 0x0887 Read : X 0x0998 Y 0x0887                                                 
			NOP			W R						XSYS  				XS<XA-D1C	YS<YA-D1C; //Write : X 0x0988 Y 0x0877 Read : X 0x0988 Y 0x0877                                                 
			NOP			W R						XSYS  				XS<XA-D1D	YS<YA-D1D; //Write : X 0x0888 Y 0x0777 Read : X 0x0888 Y 0x0777                                                 
			NOP			W R						XSYS  				XS<XA-D1E	YS<YA-D1E; //Write : X 0x0887 Y 0x0776 Read : X 0x0887 Y 0x0776                                                 
			NOP			W R						XSYS  				XS<XA-D1F	YS<YA-D1F; //Write : X 0x0877 Y 0x0766 Read : X 0x0877 Y 0x0766                                                 
			NOP			W R						XSYS  				XS<XA-D1G	YS<YA-D1G; //Write : X 0x0777 Y 0x0666 Read : X 0x0777 Y 0x0666                                                 
			NOP			W R						XSYS  				XS<XA-D1H	YS<YA-D1H; //Write : X 0x0776 Y 0x0665 Read : X 0x0776 Y 0x0665                                                 
			NOP			W R						XSYS  				XS<XA-D2A	YS<YA-D2A; //Write : X 0x0766 Y 0x0655 Read : X 0x0766 Y 0x0655                                                 
			NOP			W R						XSYS  				XS<XA-D2B	YS<YA-D2B; //Write : X 0x0666 Y 0x0555 Read : X 0x0666 Y 0x0555                                                 
			NOP			W R						XSYS  				XS<XA-D2C	YS<YA-D2C; //Write : X 0x0665 Y 0x0554 Read : X 0x0665 Y 0x0554                                                 
			NOP			W R						XSYS  				XS<XA-D2D	YS<YA-D2D; //Write : X 0x0655 Y 0x0544 Read : X 0x0655 Y 0x0544                                                 
			NOP			W R						XSYS  				XS<XA-D3	YS<YA-D3 ; //Write : X 0x0555 Y 0x0444 Read : X 0x0555 Y 0x0444                                                 
			NOP			W R						XSYS  				XS<XA-D4	YS<YA-D4 ; //Write : X 0x0545 Y 0x0434 Read : X 0x0545 Y 0x0434                                                 
			NOP			W R						XSYS  									 ; //Write : X 0x0544 Y 0x0433 Read : X 0x0544 Y 0x0433                                                 
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
	
			START XS_ALU_AMB_04;
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

			NOP		   XC<0x0666	YC<0x0555													;                                                                  
			NOP		   XS<0x0777	YS<0x0666													;                                                                  
			NOP		   XK<0x0888	YK<0x0777													;                                                                  
			NOP		   XA<0x0999	YA<0x0888													;                                                                  
			NOP		   XD<0x0AAA	YD<0x0999													;                                                                  
			NOP		   XE<0x0BBB	YE<0x0AAA		 											;                                                                  
			NOP		   XF<0x0CCC	YF<0x0BBB													;                                                                  
			NOP		   XB<0x0DDD	YB<0x0CCC													;                                                                  
			NOP		  D1A<0x0001   D4B<0x0001														;                                                                  
			NOP		  D1B<0x0010   													;                                                                  
			NOP		  D1C<0x0100														;                                                                  
			NOP		  D1D<0x0001														;                                                                  
			NOP		  D1E<0x0010														;                                                                  
			NOP		  D1F<0x0100			 											;                                                                  
			NOP		  D1G<0x0001														;                                                                  
			NOP		  D1H<0x0010														;                                                                  
			NOP		  D2A<0x0100   D3B<0x0010														;                                                                  
			NOP		  D2B<0x0001														;                                                                  
			NOP		  D2C<0x0010														;                                                                  
			NOP		  D2D<0x0100														;                                                                  
			NOP		   D3<D3B														;                                                                  
			NOP		   D4<D4B														;                                                                  
			NOP																			;                                                 
			NOP														XS<XD-D1A	YS<YD-D1A;                                                 
			NOP			W R						XSYS  				XS<XD-D1B	YS<YD-D1B; //Write : X 0x0AA9 Y 0x0998 Read : X 0x0AA9 Y 0x0998                                                 
			NOP			W R						XSYS  				XS<XD-D1C	YS<YD-D1C; //Write : X 0x0A99 Y 0x0988 Read : X 0x0A99 Y 0x0988                                                 
			NOP			W R						XSYS  				XS<XD-D1D	YS<YD-D1D; //Write : X 0x0999 Y 0x0888 Read : X 0x0999 Y 0x0888                                                 
			NOP			W R						XSYS  				XS<XD-D1E	YS<YD-D1E; //Write : X 0x0998 Y 0x0887 Read : X 0x0998 Y 0x0887                                                 
			NOP			W R						XSYS  				XS<XD-D1F	YS<YD-D1F; //Write : X 0x0988 Y 0x0877 Read : X 0x0988 Y 0x0877                                                 
			NOP			W R						XSYS  				XS<XD-D1G	YS<YD-D1G; //Write : X 0x0888 Y 0x0777 Read : X 0x0888 Y 0x0777                                                 
			NOP			W R						XSYS  				XS<XD-D1H	YS<YD-D1H; //Write : X 0x0887 Y 0x0776 Read : X 0x0887 Y 0x0776                                                 
			NOP			W R						XSYS  				XS<XD-D2A	YS<YD-D2A; //Write : X 0x0877 Y 0x0766 Read : X 0x0877 Y 0x0766                                                 
			NOP			W R						XSYS  				XS<XD-D2B	YS<YD-D2B; //Write : X 0x0777 Y 0x0666 Read : X 0x0777 Y 0x0666                                                 
			NOP			W R						XSYS  				XS<XD-D2C	YS<YD-D2C; //Write : X 0x0776 Y 0x0665 Read : X 0x0776 Y 0x0665                                                 
			NOP			W R						XSYS  				XS<XD-D2D	YS<YD-D2D; //Write : X 0x0766 Y 0x0655 Read : X 0x0766 Y 0x0655                                                 
			NOP			W R						XSYS  				XS<XD-D3	YS<YD-D3 ; //Write : X 0x0666 Y 0x0555 Read : X 0x0666 Y 0x0555                                                 
			NOP			W R						XSYS  				XS<XD-D4	YS<YD-D4 ; //Write : X 0x0656 Y 0x0545 Read : X 0x0656 Y 0x0545                                                 
			NOP			W R						XSYS  									 ; //Write : X 0x0655 Y 0x0544 Read : X 0x0655 Y 0x0544                                                 
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
		
			START XS_ALU_AMB_05;
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

			NOP		   XC<0x0666	YC<0x0555													;                                                                  
			NOP		   XS<0x0777	YS<0x0666													;                                                                  
			NOP		   XK<0x0888	YK<0x0777													;                                                                  
			NOP		   XA<0x0999	YA<0x0888													;                                                                  
			NOP		   XD<0x0AAA	YD<0x0999													;                                                                  
			NOP		   XE<0x0BBB	YE<0x0AAA		 											;                                                                  
			NOP		   XF<0x0CCC	YF<0x0BBB													;                                                                  
			NOP		   XB<0x0DDD	YB<0x0CCC													;                                                                  
			NOP		  D1A<0x0001   D4B<0x0001														;                                                                  
			NOP		  D1B<0x0010   													;                                                                  
			NOP		  D1C<0x0100														;                                                                  
			NOP		  D1D<0x0001														;                                                                  
			NOP		  D1E<0x0010														;                                                                  
			NOP		  D1F<0x0100			 											;                                                                  
			NOP		  D1G<0x0001														;                                                                  
			NOP		  D1H<0x0010														;                                                                  
			NOP		  D2A<0x0100   D3B<0x0010														;                                                                  
			NOP		  D2B<0x0001														;                                                                  
			NOP		  D2C<0x0010														;                                                                  
			NOP		  D2D<0x0100														;                                                                  
			NOP		   D3<D3B														;                                                                  
			NOP		   D4<D4B														;                                                                  
			NOP																			;                                                 
			NOP														XS<XE-D1A	YS<YE-D1A;                                                 
			NOP			W R						XSYS  				XS<XE-D1B	YS<YE-D1B; //Write : X 0x0BBA Y 0x0AA9 Read : X 0x0BBA Y 0x0AA9                                                 
			NOP			W R						XSYS  				XS<XE-D1C	YS<YE-D1C; //Write : X 0x0BAA Y 0x0A99 Read : X 0x0BAA Y 0x0A99                                                 
			NOP			W R						XSYS  				XS<XE-D1D	YS<YE-D1D; //Write : X 0x0AAA Y 0x0999 Read : X 0x0AAA Y 0x0999                                                 
			NOP			W R						XSYS  				XS<XE-D1E	YS<YE-D1E; //Write : X 0x0AA9 Y 0x0998 Read : X 0x0AA9 Y 0x0998                                                 
			NOP			W R						XSYS  				XS<XE-D1F	YS<YE-D1F; //Write : X 0x0A99 Y 0x0988 Read : X 0x0A99 Y 0x0988                                                 
			NOP			W R						XSYS  				XS<XE-D1G	YS<YE-D1G; //Write : X 0x0999 Y 0x0888 Read : X 0x0999 Y 0x0888                                                 
			NOP			W R						XSYS  				XS<XE-D1H	YS<YE-D1H; //Write : X 0x0998 Y 0x0887 Read : X 0x0998 Y 0x0887                                                 
			NOP			W R						XSYS  				XS<XE-D2A	YS<YE-D2A; //Write : X 0x0988 Y 0x0877 Read : X 0x0988 Y 0x0877                                                 
			NOP			W R						XSYS  				XS<XE-D2B	YS<YE-D2B; //Write : X 0x0888 Y 0x0777 Read : X 0x0888 Y 0x0777                                                 
			NOP			W R						XSYS  				XS<XE-D2C	YS<YE-D2C; //Write : X 0x0887 Y 0x0776 Read : X 0x0887 Y 0x0776                                                 
			NOP			W R						XSYS  				XS<XE-D2D	YS<YE-D2D; //Write : X 0x0877 Y 0x0766 Read : X 0x0877 Y 0x0766                                                 
			NOP			W R						XSYS  				XS<XE-D3	YS<YE-D3 ; //Write : X 0x0777 Y 0x0666 Read : X 0x0777 Y 0x0666                                                 
			NOP			W R						XSYS  				XS<XE-D4	YS<YE-D4 ; //Write : X 0x0767 Y 0x0656 Read : X 0x0767 Y 0x0656                                                 
			NOP			W R						XSYS  									 ; //Write : X 0x0766 Y 0x0655 Read : X 0x0766 Y 0x0655                                                 
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
		
			START XS_ALU_AMB_06;
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

			NOP		   XC<0x0666	YC<0x0555													;                                                                  
			NOP		   XS<0x0777	YS<0x0666													;                                                                  
			NOP		   XK<0x0888	YK<0x0777													;                                                                  
			NOP		   XA<0x0999	YA<0x0888													;                                                                  
			NOP		   XD<0x0AAA	YD<0x0999													;                                                                  
			NOP		   XE<0x0BBB	YE<0x0AAA		 											;                                                                  
			NOP		   XF<0x0CCC	YF<0x0BBB													;                                                                  
			NOP		   XB<0x0DDD	YB<0x0CCC													;                                                                  
			NOP		  D1A<0x0001   D4B<0x0001														;                                                                  
			NOP		  D1B<0x0010   													;                                                                  
			NOP		  D1C<0x0100														;                                                                  
			NOP		  D1D<0x0001														;                                                                  
			NOP		  D1E<0x0010														;                                                                  
			NOP		  D1F<0x0100			 											;                                                                  
			NOP		  D1G<0x0001														;                                                                  
			NOP		  D1H<0x0010														;                                                                  
			NOP		  D2A<0x0100   D3B<0x0010														;                                                                  
			NOP		  D2B<0x0001														;                                                                  
			NOP		  D2C<0x0010														;                                                                  
			NOP		  D2D<0x0100														;                                                                  
			NOP		   D3<D3B														;                                                                  
			NOP		   D4<D4B														;                                                                  
			NOP																			;                                                 
			NOP														XS<XF-D1A	YS<YF-D1A;                                                 
			NOP			W R						XSYS  				XS<XF-D1B	YS<YF-D1B; //Write : X 0x0CCB Y 0x0BBA Read : X 0x0CCB Y 0x0BBA                                                 
			NOP			W R						XSYS  				XS<XF-D1C	YS<YF-D1C; //Write : X 0x0CBB Y 0x0BAA Read : X 0x0CBB Y 0x0BAA                                                 
			NOP			W R						XSYS  				XS<XF-D1D	YS<YF-D1D; //Write : X 0x0BBB Y 0x0AAA Read : X 0x0BBB Y 0x0AAA                                                 
			NOP			W R						XSYS  				XS<XF-D1E	YS<YF-D1E; //Write : X 0x0BBA Y 0x0AA9 Read : X 0x0BBA Y 0x0AA9                                                 
			NOP			W R						XSYS  				XS<XF-D1F	YS<YF-D1F; //Write : X 0x0BAA Y 0x0A99 Read : X 0x0BAA Y 0x0A99                                                 
			NOP			W R						XSYS  				XS<XF-D1G	YS<YF-D1G; //Write : X 0x0AAA Y 0x0999 Read : X 0x0AAA Y 0x0999                                                 
			NOP			W R						XSYS  				XS<XF-D1H	YS<YF-D1H; //Write : X 0x0AA9 Y 0x0998 Read : X 0x0AA9 Y 0x0998                                                 
			NOP			W R						XSYS  				XS<XF-D2A	YS<YF-D2A; //Write : X 0x0A99 Y 0x0988 Read : X 0x0A99 Y 0x0988                                                 
			NOP			W R						XSYS  				XS<XF-D2B	YS<YF-D2B; //Write : X 0x0999 Y 0x0888 Read : X 0x0999 Y 0x0888                                                 
			NOP			W R						XSYS  				XS<XF-D2C	YS<YF-D2C; //Write : X 0x0998 Y 0x0887 Read : X 0x0998 Y 0x0887                                                 
			NOP			W R						XSYS  				XS<XF-D2D	YS<YF-D2D; //Write : X 0x0988 Y 0x0877 Read : X 0x0988 Y 0x0877                                                 
			NOP			W R						XSYS  				XS<XF-D3	YS<YF-D3 ; //Write : X 0x0888 Y 0x0777 Read : X 0x0888 Y 0x0777                                                 
			NOP			W R						XSYS  				XS<XF-D4	YS<YF-D4 ; //Write : X 0x0878 Y 0x0767 Read : X 0x0878 Y 0x0767                                                 
			NOP			W R						XSYS  									 ; //Write : X 0x0877 Y 0x0766 Read : X 0x0877 Y 0x0766                                                 
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
	
			START XS_ALU_AMB_07;
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

			NOP		   XC<0x0666	YC<0x0555													;                                                                  
			NOP		   XS<0x0777	YS<0x0666													;                                                                  
			NOP		   XK<0x0888	YK<0x0777													;                                                                  
			NOP		   XA<0x0999	YA<0x0888													;                                                                  
			NOP		   XD<0x0AAA	YD<0x0999													;                                                                  
			NOP		   XE<0x0BBB	YE<0x0AAA		 											;                                                                  
			NOP		   XF<0x0CCC	YF<0x0BBB													;                                                                  
			NOP		   XB<0x0DDD	YB<0x0CCC													;                                                                  
			NOP		  D1A<0x0001   D4B<0x0001														;                                                                  
			NOP		  D1B<0x0010   													;                                                                  
			NOP		  D1C<0x0100														;                                                                  
			NOP		  D1D<0x0001														;                                                                  
			NOP		  D1E<0x0010														;                                                                  
			NOP		  D1F<0x0100			 											;                                                                  
			NOP		  D1G<0x0001														;                                                                  
			NOP		  D1H<0x0010														;                                                                  
			NOP		  D2A<0x0100   D3B<0x0010														;                                                                  
			NOP		  D2B<0x0001														;                                                                  
			NOP		  D2C<0x0010														;                                                                  
			NOP		  D2D<0x0100														;                                                                  
			NOP		   D3<D3B														;                                                                  
			NOP		   D4<D4B														;                                                                  
			NOP																			;                                                 
			NOP														XS<XB-D1A	YS<YB-D1A;                                                 
			NOP			W R						XSYS  				XS<XB-D1B	YS<YB-D1B; //Write : X 0x0DDC Y 0x0CCB Read : X 0x0DDC Y 0x0CCB                                                 
			NOP			W R						XSYS  				XS<XB-D1C	YS<YB-D1C; //Write : X 0x0DCC Y 0x0CBB Read : X 0x0DCC Y 0x0CBB                                                 
			NOP			W R						XSYS  				XS<XB-D1D	YS<YB-D1D; //Write : X 0x0CCC Y 0x0BBB Read : X 0x0CCC Y 0x0BBB                                                 
			NOP			W R						XSYS  				XS<XB-D1E	YS<YB-D1E; //Write : X 0x0CCB Y 0x0BBA Read : X 0x0CCB Y 0x0BBA                                                 
			NOP			W R						XSYS  				XS<XB-D1F	YS<YB-D1F; //Write : X 0x0CBB Y 0x0BAA Read : X 0x0CBB Y 0x0BAA                                                 
			NOP			W R						XSYS  				XS<XB-D1G	YS<YB-D1G; //Write : X 0x0BBB Y 0x0AAA Read : X 0x0BBB Y 0x0AAA                                                 
			NOP			W R						XSYS  				XS<XB-D1H	YS<YB-D1H; //Write : X 0x0BBA Y 0x0AA9 Read : X 0x0BBA Y 0x0AA9                                                 
			NOP			W R						XSYS  				XS<XB-D2A	YS<YB-D2A; //Write : X 0x0BAA Y 0x0A99 Read : X 0x0BAA Y 0x0A99                                                 
			NOP			W R						XSYS  				XS<XB-D2B	YS<YB-D2B; //Write : X 0x0AAA Y 0x0999 Read : X 0x0AAA Y 0x0999                                                 
			NOP			W R						XSYS  				XS<XB-D2C	YS<YB-D2C; //Write : X 0x0AA9 Y 0x0998 Read : X 0x0AA9 Y 0x0998                                                 
			NOP			W R						XSYS  				XS<XB-D2D	YS<YB-D2D; //Write : X 0x0A99 Y 0x0988 Read : X 0x0A99 Y 0x0988                                                 
			NOP			W R						XSYS  				XS<XB-D3	YS<YB-D3 ; //Write : X 0x0999 Y 0x0888 Read : X 0x0999 Y 0x0888                                                 
			NOP			W R						XSYS  				XS<XB-D4	YS<YB-D4 ; //Write : X 0x0989 Y 0x0878 Read : X 0x0989 Y 0x0878                                                 
			NOP			W R						XSYS  									 ; //Write : X 0x0988 Y 0x0877 Read : X 0x0988 Y 0x0877                                                 
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
		
			START XS_ALU_AANDB_00;
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

			NOP		   XC<0xFFFF	YC<0x8FFF													;                                                                  
			NOP		   XS<0x8001	YS<0x8001													;                                                                  
			NOP		   XK<0x8002	YK<0x8002													;                                                                  
			NOP		   XA<0x8004	YA<0x8004													;                                                                  
			NOP		   XD<0x8008	YD<0x8008													;                                                                  
			NOP		   XE<0x8010	YE<0x8010		 											;                                                                  
			NOP		   XF<0x8020	YF<0x8020													;                                                                  
			NOP		   XB<0x8040	YB<0x8040													;                                                                  
			NOP		  D1A<0xFFFE														;                                                                  
			NOP		  D1B<0xFFFD	D4B<0xDFFF													;                                                                  
			NOP		  D1C<0xFFFB														;                                                                  
			NOP		  D1D<0xFFF7														;                                                                  
			NOP		  D1E<0xFFEF														;                                                                  
			NOP		  D1F<0xFFDF			 											;                                                                  
			NOP		  D1G<0xFFBF														;                                                                  
			NOP		  D1H<0xFF7F														;                                                                  
			NOP		  D2A<0xFEFF	D3B<0xEFFF													;                                                                  
			NOP		  D2B<0xFDFF														;                                                                  
			NOP		  D2C<0xFBFF														;                                                                  
			NOP		  D2D<0xF7FF														;                                                                  
			NOP		   D3<D3B														;                                                                  
			NOP		   D4<D4B														;                                                                  
			NOP																			;                                                 
			NOP														XS<XC&D1A	YS<YC&D1A;                                                 
			NOP			W R						XSYS  				XS<XC&D1B	YS<YC&D1B; //Write : X 0xFFFE Y 0x8FFE Read : X 0xFFFE Y 0x8FFE                                                  
			NOP			W R						XSYS  				XS<XC&D1C	YS<YC&D1C; //Write : X 0xFFFC Y 0x8FFC Read : X 0xFFFC Y 0x8FFC                                                 
			NOP			W R						XSYS  				XS<XC&D1D	YS<YC&D1D; //Write : X 0xFFF8 Y 0x8FF8 Read : X 0xFFF8 Y 0x8FF8                                                 
			NOP			W R						XSYS  				XS<XC&D1E	YS<YC&D1E; //Write : X 0xFFF0 Y 0x8FF0 Read : X 0xFFF0 Y 0x8FF0                                                 
			NOP			W R						XSYS  				XS<XC&D1F	YS<YC&D1F; //Write : X 0xFFE0 Y 0x8FE0 Read : X 0xFFE0 Y 0x8FE0                                                 
			NOP			W R						XSYS  				XS<XC&D1G	YS<YC&D1G; //Write : X 0xFFC0 Y 0x8FC0 Read : X 0xFFC0 Y 0x8FC0                                                 
			NOP			W R						XSYS  				XS<XC&D1H	YS<YC&D1H; //Write : X 0xFF80 Y 0x8F80 Read : X 0xFF80 Y 0x8F80                                                 
			NOP			W R						XSYS  				XS<XC&D2A	YS<YC&D2A; //Write : X 0xFF00 Y 0x8F00 Read : X 0xFF00 Y 0x8F00                                                 
			NOP			W R						XSYS  				XS<XC&D2B	YS<YC&D2B; //Write : X 0xFE00 Y 0x8E00 Read : X 0xFE00 Y 0x8E00                                                 
			NOP			W R						XSYS  				XS<XC&D2C	YS<YC&D2C; //Write : X 0xFC00 Y 0x8C00 Read : X 0xFC00 Y 0x8C00                                                 
			NOP			W R						XSYS  				XS<XC&D2D	YS<YC&D2D; //Write : X 0xF800 Y 0x8800 Read : X 0xF800 Y 0x8800                                                 
			NOP			W R						XSYS  				XS<XC&D3	YS<YC&D3 ; //Write : X 0xF000 Y 0x8000 Read : X 0xF000 Y 0x8000                                                 
			NOP			W R						XSYS  				XS<XC&D4	YS<YC&D4 ; //Write : X 0xE000 Y 0x8000 Read : X 0xE000 Y 0x8000                                                 
			NOP			W R						XSYS  									 ; //Write : X 0xC000 Y 0x8000 Read : X 0xC000 Y 0x8000                                                 
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
			
			START XS_ALU_AANDB_01;
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

			NOP		   XC<0xFFFF	YC<0xEFFF													;                                                                  
			NOP		   XS<0xFFFF	YS<0x8FFF													;                                                                  
			NOP		   XK<0xEFFF	YK<0xDFFF													;                                                                  
			NOP		   XA<0xDFFF	YA<0xCFFF													;                                                                  
			NOP		   XD<0xCFFF	YD<0xBFFF													;                                                                  
			NOP		   XE<0xBFFF	YE<0xAFFF		 											;                                                                  
			NOP		   XF<0xAFFF	YF<0x9FFF													;                                                                  
			NOP		   XB<0x9FFF	YB<0x8FFF													;                                                                  
			NOP		  D1A<0xFFFE														;                                                                  
			NOP		  D1B<0xFFFD	D4B<0xDFFF													;                                                                  
			NOP		  D1C<0xFFFB														;                                                                  
			NOP		  D1D<0xFFF7														;                                                                  
			NOP		  D1E<0xFFEF														;                                                                  
			NOP		  D1F<0xFFDF			 											;                                                                  
			NOP		  D1G<0xFFBF														;                                                                  
			NOP		  D1H<0xFF7F														;                                                                  
			NOP		  D2A<0xFEFF	D3B<0xEFFF													;                                                                  
			NOP		  D2B<0xFDFF														;                                                                  
			NOP		  D2C<0xFBFF														;                                                                  
			NOP		  D2D<0xF7FF														;                                                                  
			NOP		   D3<D3B														;                                                                  
			NOP		   D4<D4B														;                                                                  
			NOP																			;                                                 
			NOP														XS<XS&D1A	YS<YS&D1A;                                                 
			NOP			W R						XSYS  				XS<XS&D1B	YS<YS&D1B; //Write : X 0xFFFE Y 0x8FFE Read : X 0xFFFE Y 0x8FFE                                                 
			NOP			W R						XSYS  				XS<XS&D1C	YS<YS&D1C; //Write : X 0xFFFC Y 0x8FFC Read : X 0xFFFC Y 0x8FFC                                                 
			NOP			W R						XSYS  				XS<XS&D1D	YS<YS&D1D; //Write : X 0xFFF8 Y 0x8FF8 Read : X 0xFFF8 Y 0x8FF8                                                 
			NOP			W R						XSYS  				XS<XS&D1E	YS<YS&D1E; //Write : X 0xFFF0 Y 0x8FF0 Read : X 0xFFF0 Y 0x8FF0                                                 
			NOP			W R						XSYS  				XS<XS&D1F	YS<YS&D1F; //Write : X 0xFFE0 Y 0x8FE0 Read : X 0xFFE0 Y 0x8FE0                                                 
			NOP			W R						XSYS  				XS<XS&D1G	YS<YS&D1G; //Write : X 0xFFC0 Y 0x8FC0 Read : X 0xFFC0 Y 0x8FC0                                                 
			NOP			W R						XSYS  				XS<XS&D1H	YS<YS&D1H; //Write : X 0xFF80 Y 0x8F80 Read : X 0xFF80 Y 0x8F80                                                 
			NOP			W R						XSYS  				XS<XS&D2A	YS<YS&D2A; //Write : X 0xFF00 Y 0x8F00 Read : X 0xFF00 Y 0x8F00                                                 
			NOP			W R						XSYS  				XS<XS&D2B	YS<YS&D2B; //Write : X 0xFE00 Y 0x8E00 Read : X 0xFE00 Y 0x8E00                                                 
			NOP			W R						XSYS  				XS<XS&D2C	YS<YS&D2C; //Write : X 0xFC00 Y 0x8C00 Read : X 0xFC00 Y 0x8C00                                                 
			NOP			W R						XSYS  				XS<XS&D2D	YS<YS&D2D; //Write : X 0xF800 Y 0x8800 Read : X 0xF800 Y 0x8800                                                 
			NOP			W R						XSYS  				XS<XS&D3	YS<YS&D3 ; //Write : X 0xF000 Y 0x8000 Read : X 0xF000 Y 0x8000                                                 
			NOP			W R						XSYS  				XS<XS&D4	YS<YS&D4 ; //Write : X 0xE000 Y 0x8000 Read : X 0xE000 Y 0x8000                                                 
			NOP			W R						XSYS  									 ; //Write : X 0xC000 Y 0x8000 Read : X 0xC000 Y 0x8000                                                 
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
		
			START XS_ALU_AANDB_02;
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

			NOP		   XC<0xFFFF	YC<0xEFFF													;                                                                  
			NOP		   XS<0xFFFF	YS<0xEFFF													;                                                                  
			NOP		   XK<0xFFFF	YK<0x8FFF													;                                                                  
			NOP		   XA<0xDFFF	YA<0xCFFF													;                                                                  
			NOP		   XD<0xCFFF	YD<0xBFFF													;                                                                  
			NOP		   XE<0xBFFF	YE<0xAFFF		 											;                                                                  
			NOP		   XF<0xAFFF	YF<0x9FFF													;                                                                  
			NOP		   XB<0x9FFF	YB<0x8FFF													;                                                                  
			NOP		  D1A<0xFFFE														;                                                                  
			NOP		  D1B<0xFFFD	D4B<0xDFFF													;                                                                  
			NOP		  D1C<0xFFFB														;                                                                  
			NOP		  D1D<0xFFF7														;                                                                  
			NOP		  D1E<0xFFEF														;                                                                  
			NOP		  D1F<0xFFDF			 											;                                                                  
			NOP		  D1G<0xFFBF														;                                                                  
			NOP		  D1H<0xFF7F														;                                                                  
			NOP		  D2A<0xFEFF	D3B<0xEFFF													;                                                                  
			NOP		  D2B<0xFDFF														;                                                                  
			NOP		  D2C<0xFBFF														;                                                                  
			NOP		  D2D<0xF7FF														;                                                                  
			NOP		   D3<D3B														;                                                                  
			NOP		   D4<D4B														;                                                                  
			NOP																			;                                                 
			NOP														XS<XK&D1A	YS<YK&D1A;                                                 
			NOP			W R						XSYS  				XS<XK&D1B	YS<YK&D1B; //Write : X 0xFFFE Y 0x8FFE Read : X 0xFFFE Y 0x8FFE                                                 
			NOP			W R						XSYS  				XS<XK&D1C	YS<YK&D1C; //Write : X 0xFFFC Y 0x8FFC Read : X 0xFFFC Y 0x8FFC                                                 
			NOP			W R						XSYS  				XS<XK&D1D	YS<YK&D1D; //Write : X 0xFFF8 Y 0x8FF8 Read : X 0xFFF8 Y 0x8FF8                                                 
			NOP			W R						XSYS  				XS<XK&D1E	YS<YK&D1E; //Write : X 0xFFF0 Y 0x8FF0 Read : X 0xFFF0 Y 0x8FF0                                                 
			NOP			W R						XSYS  				XS<XK&D1F	YS<YK&D1F; //Write : X 0xFFE0 Y 0x8FE0 Read : X 0xFFE0 Y 0x8FE0                                                 
			NOP			W R						XSYS  				XS<XK&D1G	YS<YK&D1G; //Write : X 0xFFC0 Y 0x8FC0 Read : X 0xFFC0 Y 0x8FC0                                                 
			NOP			W R						XSYS  				XS<XK&D1H	YS<YK&D1H; //Write : X 0xFF80 Y 0x8F80 Read : X 0xFF80 Y 0x8F80                                                 
			NOP			W R						XSYS  				XS<XK&D2A	YS<YK&D2A; //Write : X 0xFF00 Y 0x8F00 Read : X 0xFF00 Y 0x8F00                                                 
			NOP			W R						XSYS  				XS<XK&D2B	YS<YK&D2B; //Write : X 0xFE00 Y 0x8E00 Read : X 0xFE00 Y 0x8E00                                                 
			NOP			W R						XSYS  				XS<XK&D2C	YS<YK&D2C; //Write : X 0xFC00 Y 0x8C00 Read : X 0xFC00 Y 0x8C00                                                 
			NOP			W R						XSYS  				XS<XK&D2D	YS<YK&D2D; //Write : X 0xF800 Y 0x8800 Read : X 0xF800 Y 0x8800                                                 
			NOP			W R						XSYS  				XS<XK&D3	YS<YK&D3 ; //Write : X 0xF000 Y 0x8000 Read : X 0xF000 Y 0x8000                                                 
			NOP			W R						XSYS  				XS<XK&D4	YS<YK&D4 ; //Write : X 0xE000 Y 0x8000 Read : X 0xE000 Y 0x8000                                                 
			NOP			W R						XSYS  									 ; //Write : X 0xC000 Y 0x8000 Read : X 0xC000 Y 0x8000                                                 
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
		
			START XS_ALU_AANDB_03;
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

			NOP		   XC<0xFFFF	YC<0xEFFF													;                                                                  
			NOP		   XS<0xFFFF	YS<0xEFFF													;                                                                  
			NOP		   XK<0xEFFF	YK<0xDFFF													;                                                                  
			NOP		   XA<0xDFFF	YA<0xCFFF													;                                                                  
			NOP		   XD<0xCFFF	YD<0xBFFF													;                                                                  
			NOP		   XE<0xBFFF	YE<0xAFFF		 											;                                                                  
			NOP		   XF<0xAFFF	YF<0x9FFF													;                                                                  
			NOP		   XB<0x9FFF	YB<0x8FFF													;                                                                  
			NOP		  D1A<0xFFFE														;                                                                  
			NOP		  D1B<0xFFFD	D4B<0xDFFF													;                                                                  
			NOP		  D1C<0xFFFB														;                                                                  
			NOP		  D1D<0xFFF7														;                                                                  
			NOP		  D1E<0xFFEF														;                                                                  
			NOP		  D1F<0xFFDF			 											;                                                                  
			NOP		  D1G<0xFFBF														;                                                                  
			NOP		  D1H<0xFF7F														;                                                                  
			NOP		  D2A<0xFEFF	D3B<0xEFFF													;                                                                  
			NOP		  D2B<0xFDFF														;                                                                  
			NOP		  D2C<0xFBFF														;                                                                  
			NOP		  D2D<0xF7FF														;                                                                  
			NOP		   D3<D3B														;                                                                  
			NOP		   D4<D4B														;                                                                  
			NOP																			;                                                                  
			NOP																			;                                                 
			NOP														XS<XA&D1A	YS<YA&D1A;                                                 
			NOP			W R						XSYS  				XS<XA&D1B	YS<YA&D1B; //Write : X 0xDFFE Y 0xCFFE Read : X 0xDFFE Y 0xCFFE                                                 
			NOP			W R						XSYS  				XS<XA&D1C	YS<YA&D1C; //Write : X 0xDFFC Y 0xCFFC Read : X 0xDFFC Y 0xCFFC                                                 
			NOP			W R						XSYS  				XS<XA&D1D	YS<YA&D1D; //Write : X 0xDFF8 Y 0xCFF8 Read : X 0xDFF8 Y 0xCFF8                                                 
			NOP			W R						XSYS  				XS<XA&D1E	YS<YA&D1E; //Write : X 0xDFF0 Y 0xCFF0 Read : X 0xDFF0 Y 0xCFF0                                                 
			NOP			W R						XSYS  				XS<XA&D1F	YS<YA&D1F; //Write : X 0xDFE0 Y 0xCFE0 Read : X 0xDFE0 Y 0xCFE0                                                  
			NOP			W R						XSYS  				XS<XA&D1G	YS<YA&D1G; //Write : X 0xDFC0 Y 0xCFC0 Read : X 0xDFC0 Y 0xCFC0                                                  
			NOP			W R						XSYS  				XS<XA&D1H	YS<YA&D1H; //Write : X 0xDF80 Y 0xCF80 Read : X 0xDF80 Y 0xCF80                                                  
			NOP			W R						XSYS  				XS<XA&D2A	YS<YA&D2A; //Write : X 0xDF00 Y 0xCF00 Read : X 0xDF00 Y 0xCF00                                                   
			NOP			W R						XSYS  				XS<XA&D2B	YS<YA&D2B; //Write : X 0xDE00 Y 0xCE00 Read : X 0xDE00 Y 0xCE00                                                  
			NOP			W R						XSYS  				XS<XA&D2C	YS<YA&D2C; //Write : X 0xDC00 Y 0xCC00 Read : X 0xDC00 Y 0xCC00                                                  
			NOP			W R						XSYS  				XS<XA&D2D	YS<YA&D2D; //Write : X 0xD800 Y 0xC800 Read : X 0xD800 Y 0xC800                                                  
			NOP			W R						XSYS  				XS<XA&D3	YS<YA&D3 ; //Write : X 0xD000 Y 0xC000 Read : X 0xD000 Y 0xC000                                                  
			NOP			W R						XSYS  				XS<XA&D4	YS<YA&D4 ; //Write : X 0xC000 Y 0xC000 Read : X 0xC000 Y 0xC000                                                  
			NOP			W R						XSYS  									 ; //Write : X 0xC000 Y 0xC000 Read : X 0xC000 Y 0xC000                                                  
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
		
			START XS_ALU_AANDB_04;
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

			NOP		   XC<0xFFFF	YC<0xEFFF													;                                                                  
			NOP		   XS<0xFFFF	YS<0xEFFF													;                                                                  
			NOP		   XK<0xEFFF	YK<0xDFFF													;                                                                  
			NOP		   XA<0xDFFF	YA<0xCFFF													;                                                                  
			NOP		   XD<0xCFFF	YD<0xBFFF													;                                                                  
			NOP		   XE<0xBFFF	YE<0xAFFF		 											;                                                                  
			NOP		   XF<0xAFFF	YF<0x9FFF													;                                                                  
			NOP		   XB<0x9FFF	YB<0x8FFF													;                                                                  
			NOP		  D1A<0xFFFE														;                                                                  
			NOP		  D1B<0xFFFD	D4B<0xDFFF													;                                                                  
			NOP		  D1C<0xFFFB														;                                                                  
			NOP		  D1D<0xFFF7														;                                                                  
			NOP		  D1E<0xFFEF														;                                                                  
			NOP		  D1F<0xFFDF			 											;                                                                  
			NOP		  D1G<0xFFBF														;                                                                  
			NOP		  D1H<0xFF7F														;                                                                  
			NOP		  D2A<0xFEFF	D3B<0xEFFF													;                                                                  
			NOP		  D2B<0xFDFF														;                                                                  
			NOP		  D2C<0xFBFF														;                                                                  
			NOP		  D2D<0xF7FF														;                                                                  
			NOP		   D3<D3B														;                                                                  
			NOP		   D4<D4B														;                                                                  
			NOP																			;                                                 
			NOP														XS<XD&D1A	YS<YD&D1A;                                                 
			NOP			W R						XSYS  				XS<XD&D1B	YS<YD&D1B; //Write : X 0xCFFE Y 0xBFFE Read : X 0xCFFE Y 0xBFFE                                                 
			NOP			W R						XSYS  				XS<XD&D1C	YS<YD&D1C; //Write : X 0xCFFC Y 0xBFFC Read : X 0xCFFC Y 0xBFFC                                                 
			NOP			W R						XSYS  				XS<XD&D1D	YS<YD&D1D; //Write : X 0xCFF8 Y 0xBFF8 Read : X 0xCFF8 Y 0xBFF8                                                 
			NOP			W R						XSYS  				XS<XD&D1E	YS<YD&D1E; //Write : X 0xCFF0 Y 0xBFF0 Read : X 0xCFF0 Y 0xBFF0                                                 
			NOP			W R						XSYS  				XS<XD&D1F	YS<YD&D1F; //Write : X 0xCFE0 Y 0xBFE0 Read : X 0xCFE0 Y 0xBFE0                                                 
			NOP			W R						XSYS  				XS<XD&D1G	YS<YD&D1G; //Write : X 0xCFC0 Y 0xBFC0 Read : X 0xCFC0 Y 0xBFC0                                                 
			NOP			W R						XSYS  				XS<XD&D1H	YS<YD&D1H; //Write : X 0xCF80 Y 0xBF80 Read : X 0xCF80 Y 0xBF80                                                 
			NOP			W R						XSYS  				XS<XD&D2A	YS<YD&D2A; //Write : X 0xCF00 Y 0xBF00 Read : X 0xCF00 Y 0xBF00                                                 
			NOP			W R						XSYS  				XS<XD&D2B	YS<YD&D2B; //Write : X 0xCE00 Y 0xBE00 Read : X 0xCE00 Y 0xBE00                                                 
			NOP			W R						XSYS  				XS<XD&D2C	YS<YD&D2C; //Write : X 0xCC00 Y 0xBC00 Read : X 0xCC00 Y 0xBC00                                                 
			NOP			W R						XSYS  				XS<XD&D2D	YS<YD&D2D; //Write : X 0xC800 Y 0xB800 Read : X 0xC800 Y 0xB800                                                 
			NOP			W R						XSYS  				XS<XD&D3	YS<YD&D3 ; //Write : X 0xC000 Y 0xB000 Read : X 0xC000 Y 0xB000                                                 
			NOP			W R						XSYS  				XS<XD&D4	YS<YD&D4 ; //Write : X 0xC000 Y 0xA000 Read : X 0xC000 Y 0xA000                                                 
			NOP			W R						XSYS  									 ; //Write : X 0xC000 Y 0x8000 Read : X 0xC000 Y 0x8000                                                 
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
			
			START XS_ALU_AANDB_05;
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

			NOP		   XC<0xFFFF	YC<0xEFFF													;                                                                  
			NOP		   XS<0xFFFF	YS<0xEFFF													;                                                                  
			NOP		   XK<0xEFFF	YK<0xDFFF													;                                                                  
			NOP		   XA<0xDFFF	YA<0xCFFF													;                                                                  
			NOP		   XD<0xCFFF	YD<0xBFFF													;                                                                  
			NOP		   XE<0xBFFF	YE<0xAFFF		 											;                                                                  
			NOP		   XF<0xAFFF	YF<0x9FFF													;                                                                  
			NOP		   XB<0x9FFF	YB<0x8FFF													;                                                                  
			NOP		  D1A<0xFFFE														;                                                                  
			NOP		  D1B<0xFFFD	D4B<0xDFFF													;                                                                  
			NOP		  D1C<0xFFFB														;                                                                  
			NOP		  D1D<0xFFF7														;                                                                  
			NOP		  D1E<0xFFEF														;                                                                  
			NOP		  D1F<0xFFDF			 											;                                                                  
			NOP		  D1G<0xFFBF														;                                                                  
			NOP		  D1H<0xFF7F														;                                                                  
			NOP		  D2A<0xFEFF	D3B<0xEFFF													;                                                                  
			NOP		  D2B<0xFDFF														;                                                                  
			NOP		  D2C<0xFBFF														;                                                                  
			NOP		  D2D<0xF7FF														;                                                                  
			NOP		   D3<D3B														;                                                                  
			NOP		   D4<D4B														;                                                                  
			NOP																			;                                                 
			NOP														XS<XE&D1A	YS<YE&D1A;                                                 
			NOP			W R						XSYS  				XS<XE&D1B	YS<YE&D1B; //Write : X 0xBFFE Y 0xAFFE Read : X 0xBFFE Y 0xAFFE                                                 
			NOP			W R						XSYS  				XS<XE&D1C	YS<YE&D1C; //Write : X 0xBFFC Y 0xAFFC Read : X 0xBFFC Y 0xAFFC                                                 
			NOP			W R						XSYS  				XS<XE&D1D	YS<YE&D1D; //Write : X 0xBFF8 Y 0xAFF8 Read : X 0xBFF8 Y 0xAFF8                                                 
			NOP			W R						XSYS  				XS<XE&D1E	YS<YE&D1E; //Write : X 0xBFF0 Y 0xAFF0 Read : X 0xBFF0 Y 0xAFF0                                                 
			NOP			W R						XSYS  				XS<XE&D1F	YS<YE&D1F; //Write : X 0xBFE0 Y 0xAFE0 Read : X 0xBFE0 Y 0xAFE0                                                 
			NOP			W R						XSYS  				XS<XE&D1G	YS<YE&D1G; //Write : X 0xBFC0 Y 0xAFC0 Read : X 0xBFC0 Y 0xAFC0                                                 
			NOP			W R						XSYS  				XS<XE&D1H	YS<YE&D1H; //Write : X 0xBF80 Y 0xAF80 Read : X 0xBF80 Y 0xAF80                                                 
			NOP			W R						XSYS  				XS<XE&D2A	YS<YE&D2A; //Write : X 0xBF00 Y 0xAF00 Read : X 0xBF00 Y 0xAF00                                                 
			NOP			W R						XSYS  				XS<XE&D2B	YS<YE&D2B; //Write : X 0xBE00 Y 0xAE00 Read : X 0xBE00 Y 0xAE00                                                 
			NOP			W R						XSYS  				XS<XE&D2C	YS<YE&D2C; //Write : X 0xBC00 Y 0xAC00 Read : X 0xBC00 Y 0xAC00                                                 
			NOP			W R						XSYS  				XS<XE&D2D	YS<YE&D2D; //Write : X 0xB800 Y 0xA800 Read : X 0xB800 Y 0xA800                                                 
			NOP			W R						XSYS  				XS<XE&D3	YS<YE&D3 ; //Write : X 0xB000 Y 0xA000 Read : X 0xB000 Y 0xA000                                                 
			NOP			W R						XSYS  				XS<XE&D4	YS<YE&D4 ; //Write : X 0xA000 Y 0xA000 Read : X 0xA000 Y 0xA000                                                 
			NOP			W R						XSYS  									 ; //Write : X 0x8000 Y 0x8000 Read : X 0x8000 Y 0x8000                                                 
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
		
			START XS_ALU_AANDB_06;
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

			NOP		   XC<0xFFFF	YC<0xEFFF													;                                                                  
			NOP		   XS<0xFFFF	YS<0xEFFF													;                                                                  
			NOP		   XK<0xEFFF	YK<0xDFFF													;                                                                  
			NOP		   XA<0xDFFF	YA<0xCFFF													;                                                                  
			NOP		   XD<0xCFFF	YD<0xBFFF													;                                                                  
			NOP		   XE<0xBFFF	YE<0xAFFF		 											;                                                                  
			NOP		   XF<0xAFFF	YF<0x9FFF													;                                                                  
			NOP		   XB<0x9FFF	YB<0x8FFF													;                                                                  
			NOP		  D1A<0xFFFE														;                                                                  
			NOP		  D1B<0xFFFD	D4B<0xDFFF													;                                                                  
			NOP		  D1C<0xFFFB														;                                                                  
			NOP		  D1D<0xFFF7														;                                                                  
			NOP		  D1E<0xFFEF														;                                                                  
			NOP		  D1F<0xFFDF			 											;                                                                  
			NOP		  D1G<0xFFBF														;                                                                  
			NOP		  D1H<0xFF7F														;                                                                  
			NOP		  D2A<0xFEFF	D3B<0xEFFF													;                                                                  
			NOP		  D2B<0xFDFF														;                                                                  
			NOP		  D2C<0xFBFF														;                                                                  
			NOP		  D2D<0xF7FF														;                                                                  
			NOP		   D3<D3B														;                                                                  
			NOP		   D4<D4B														;                                                                  
			NOP																			;                                                 
			NOP																			;                                                 
			NOP														XS<XF&D1A	YS<YF&D1A;                                                 
			NOP			W R						XSYS  				XS<XF&D1B	YS<YF&D1B; //Write : X 0xAFFE Y 0x9FFE Read : X 0xAFFE Y 0x9FFE                                                 
			NOP			W R						XSYS  				XS<XF&D1C	YS<YF&D1C; //Write : X 0xAFFC Y 0x9FFC Read : X 0xAFFC Y 0x9FFC                                                 
			NOP			W R						XSYS  				XS<XF&D1D	YS<YF&D1D; //Write : X 0xAFF8 Y 0x9FF8 Read : X 0xAFF8 Y 0x9FF8                                                 
			NOP			W R						XSYS  				XS<XF&D1E	YS<YF&D1E; //Write : X 0xAFF0 Y 0x9FF0 Read : X 0xAFF0 Y 0x9FF0                                                 
			NOP			W R						XSYS  				XS<XF&D1F	YS<YF&D1F; //Write : X 0xAFE0 Y 0x9FE0 Read : X 0xAFE0 Y 0x9FE0                                                 
			NOP			W R						XSYS  				XS<XF&D1G	YS<YF&D1G; //Write : X 0xAFC0 Y 0x9FC0 Read : X 0xAFC0 Y 0x9FC0                                                 
			NOP			W R						XSYS  				XS<XF&D1H	YS<YF&D1H; //Write : X 0xAF80 Y 0x9F80 Read : X 0xAF80 Y 0x9F80                                                 
			NOP			W R						XSYS  				XS<XF&D2A	YS<YF&D2A; //Write : X 0xAF00 Y 0x9F00 Read : X 0xAF00 Y 0x9F00                                                 
			NOP			W R						XSYS  				XS<XF&D2B	YS<YF&D2B; //Write : X 0xAE00 Y 0x9E00 Read : X 0xAE00 Y 0x9E00                                                 
			NOP			W R						XSYS  				XS<XF&D2C	YS<YF&D2C; //Write : X 0xAC00 Y 0x9C00 Read : X 0xAC00 Y 0x9C00                                                 
			NOP			W R						XSYS  				XS<XF&D2D	YS<YF&D2D; //Write : X 0xA800 Y 0x9800 Read : X 0xA800 Y 0x9800                                                 
			NOP			W R						XSYS  				XS<XF&D3	YS<YF&D3 ; //Write : X 0xA000 Y 0x9000 Read : X 0xA000 Y 0x9000                                                 
			NOP			W R						XSYS  				XS<XF&D4	YS<YF&D4 ; //Write : X 0xA000 Y 0x8000 Read : X 0xA000 Y 0x8000                                                 
			NOP			W R						XSYS  									 ; //Write : X 0x8000 Y 0x8000 Read : X 0x8000 Y 0x8000                                                 
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
			
			START XS_ALU_AANDB_07;
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

			NOP		   XC<0xFFFF	YC<0xEFFF													;                                                                  
			NOP		   XS<0xFFFF	YS<0xEFFF													;                                                                  
			NOP		   XK<0xEFFF	YK<0xDFFF													;                                                                  
			NOP		   XA<0xDFFF	YA<0xCFFF													;                                                                  
			NOP		   XD<0xCFFF	YD<0xBFFF													;                                                                  
			NOP		   XE<0xBFFF	YE<0xAFFF		 											;                                                                  
			NOP		   XF<0xAFFF	YF<0x9FFF													;                                                                  
			NOP		   XB<0x9FFF	YB<0x8FFF													;                                                                  
			NOP		  D1A<0xFFFE														;                                                                  
			NOP		  D1B<0xFFFD	D4B<0xDFFF													;                                                                  
			NOP		  D1C<0xFFFB														;                                                                  
			NOP		  D1D<0xFFF7														;                                                                  
			NOP		  D1E<0xFFEF														;                                                                  
			NOP		  D1F<0xFFDF			 											;                                                                  
			NOP		  D1G<0xFFBF														;                                                                  
			NOP		  D1H<0xFF7F														;                                                                  
			NOP		  D2A<0xFEFF	D3B<0xEFFF													;                                                                  
			NOP		  D2B<0xFDFF														;                                                                  
			NOP		  D2C<0xFBFF														;                                                                  
			NOP		  D2D<0xF7FF														;                                                                  
			NOP		   D3<D3B														;                                                                  
			NOP		   D4<D4B														;                                                                  
			NOP																			;                                                 
			NOP														XS<XB&D1A	YS<YB&D1A;                                                 
			NOP			W R						XSYS  				XS<XB&D1B	YS<YB&D1B; //Write : X 0x9FFE Y 0x8FFE Read : X 0x9FFE Y 0x8FFE                                                 
			NOP			W R						XSYS  				XS<XB&D1C	YS<YB&D1C; //Write : X 0x9FFC Y 0x8FFC Read : X 0x9FFC Y 0x8FFC                                                 
			NOP			W R						XSYS  				XS<XB&D1D	YS<YB&D1D; //Write : X 0x9FF8 Y 0x8FF8 Read : X 0x9FF8 Y 0x8FF8                                                 
			NOP			W R						XSYS  				XS<XB&D1E	YS<YB&D1E; //Write : X 0x9FF0 Y 0x8FF0 Read : X 0x9FF0 Y 0x8FF0                                                 
			NOP			W R						XSYS  				XS<XB&D1F	YS<YB&D1F; //Write : X 0x9FE0 Y 0x8FE0 Read : X 0x9FE0 Y 0x8FE0                                                 
			NOP			W R						XSYS  				XS<XB&D1G	YS<YB&D1G; //Write : X 0x9FC0 Y 0x8FC0 Read : X 0x9FC0 Y 0x8FC0                                                 
			NOP			W R						XSYS  				XS<XB&D1H	YS<YB&D1H; //Write : X 0x9F80 Y 0x8F80 Read : X 0x9F80 Y 0x8F80                                                 
			NOP			W R						XSYS  				XS<XB&D2A	YS<YB&D2A; //Write : X 0x9F00 Y 0x8F00 Read : X 0x9F00 Y 0x8F00                                                 
			NOP			W R						XSYS  				XS<XB&D2B	YS<YB&D2B; //Write : X 0x9E00 Y 0x8E00 Read : X 0x9E00 Y 0x8E00                                                 
			NOP			W R						XSYS  				XS<XB&D2C	YS<YB&D2C; //Write : X 0x9C00 Y 0x8C00 Read : X 0x9C00 Y 0x8C00                                                 
			NOP			W R						XSYS  				XS<XB&D2D	YS<YB&D2D; //Write : X 0x9800 Y 0x8800 Read : X 0x9800 Y 0x8800                                                 
			NOP			W R						XSYS  				XS<XB&D3	YS<YB&D3 ; //Write : X 0x9000 Y 0x8000 Read : X 0x9000 Y 0x8000                                                 
			NOP			W R						XSYS  				XS<XB&D4	YS<YB&D4 ; //Write : X 0x8000 Y 0x8000 Read : X 0x8000 Y 0x8000                                                 
			NOP			W R						XSYS  									 ; //Write : X 0x8000 Y 0x8000 Read : X 0x8000 Y 0x8000                                                 
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
																		
%ENDC 