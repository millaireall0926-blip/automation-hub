


%INSERT Header/define.h
  
 REGISTER
			XMAX=0xffffff;
			YMAX=0xffffff;
			ZMAX=0xff;
			XOS=0x0;
			YOS=0x0;	
ENDREGISTER		

			START XA_ALU_A_00;
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
			NOP		  XC<0x0000			YC<0x8000										;                                                                  
			NOP		  XS<0x1000			YS<0x9000										;                                                                  
			NOP		  XK<0x2000			YK<0xA000										;                                                                  
			NOP		  XA<0x3000			YA<0xB000										;                                                                  
			NOP		  XD<0x4000			YD<0xC000										;                                                                  
			NOP		  XE<0x5000			YE<0xD000		 								;                                                                  
			NOP		  XF<0x6000			YF<0xE000										;                                                                  
			NOP		  XB<0x7000			YB<0xF000										;                                                                  
			NOP																			;                                                 
			NOP														XA<XC	YA<YC		;                                                 
			NOP			W R						XAYA  				XA<XS	YA<YS		; //Write : X 0x0000 Y 0x8000 Read : X 0x0000 Y 0x8000                                                                                                 
			NOP			W R						XAYA  				XA<XK	YA<YK		; //Write : X 0x1000 Y 0x9000 Read : X 0x1000 Y 0x9000                                                                                                 
			NOP			W R						XAYA  				XA<XA	YA<YA 		; //Write : X 0x2000 Y 0xA000 Read : X 0x2000 Y 0xA000                                                                                                 
			NOP			W R						XAYA  				XA<XD	YA<YD 		; //Write : X 0x2000 Y 0xA000 Read : X 0x2000 Y 0xA000                                                                                                 
			NOP			W R						XAYA  				XA<XE	YA<YE		; //Write : X 0x4000 Y 0xC000 Read : X 0x4000 Y 0xC000                                                                                                 
			NOP			W R						XAYA  				XA<XF	YA<YF		; //Write : X 0x5000 Y 0xD000 Read : X 0x5000 Y 0xD000                                                                                                 
			NOP			W R						XAYA  				XA<XB	YA<YB		; //Write : X 0x6000 Y 0xE000 Read : X 0x6000 Y 0xE000                                                                                                 
			NOP			W R						XAYA  									; //Write : X 0x7000 Y 0xF000 Read : X 0x7000 Y 0xF000                                                 
			NOP																			;                                     
			NOP																			;
			NOP		  XC<0x8000			YC<0x0000										;                                                                  
			NOP		  XS<0x9000			YS<0x1000										;                                                                  
			NOP		  XK<0xA000			YK<0x2000										;                                                                  
			NOP		  XA<0xB000			YA<0x3000										;                                                                  
			NOP		  XD<0xC000			YD<0x4000										;                                                                  
			NOP		  XE<0xD000			YE<0x5000		 								;                                                                  
			NOP		  XF<0xE000			YF<0x6000										;                                                                  
			NOP		  XB<0xF000			YB<0x7000										;                                                                  
			NOP																			;                                                 
			NOP														XA<XC	YA<YC		;                                                 
			NOP			W R						XAYA  				XA<XS	YA<YS		; //Write : X 0x8000 Y 0x0000 Read : X 0x8000 Y 0x0000                                                                                                 
			NOP			W R						XAYA  				XA<XK	YA<YK		; //Write : X 0x9000 Y 0x1000 Read : X 0x9000 Y 0x1000                                                                                                 
			NOP			W R						XAYA  				XA<XA	YA<YA 		; //Write : X 0xA000 Y 0x2000 Read : X 0xA000 Y 0x2000                                                                                                 
			NOP			W R						XAYA  				XA<XD	YA<YD 		; //Write : X 0xA000 Y 0x2000 Read : X 0xA000 Y 0x2000                                                                                                 
			NOP			W R						XAYA  				XA<XE	YA<YE		; //Write : X 0xC000 Y 0x4000 Read : X 0xC000 Y 0x4000                                                                                                 
			NOP			W R						XAYA  				XA<XF	YA<YF		; //Write : X 0xD000 Y 0x5000 Read : X 0xD000 Y 0x5000                                                                                                 
			NOP			W R						XAYA  				XA<XB	YA<YB		; //Write : X 0xE000 Y 0x6000 Read : X 0xE000 Y 0x6000                                                                                                 
			NOP			W R						XAYA  									; //Write : X 0xF000 Y 0x7000 Read : X 0xF000 Y 0x7000 
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

			START XA_ALU_NA_00;
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
			NOP		  XC<0x0000			YC<0x0000										;                                                                  
			NOP		  XS<0x0001			YS<0x000A										;                                                                 
			NOP		  XK<0x0002			YK<0x000B										;                                                                 
			NOP		  XA<0x0004			YA<0x000C										;                                                                 
			NOP		  XD<0x0008			YD<0x000D										;                                                                 
			NOP		  XE<0x0010			YE<0x00A0 										;                                                                 
			NOP		  XF<0x0020			YF<0x00B0										;                                                                 
			NOP		  XB<0x0040			YB<0x00C0										;                                                                 
			NOP																			;                                                 
			NOP														XA</XC	YA</YC		;                                                 
			NOP			W R						XAYA  				XA</XS	YA</YS		; //Write : X 0xFFFF Y 0xFFFF Read : X 0xFFFF Y 0xFFFF                                                  
			NOP			W R						XAYA  				XA</XK	YA</YK		; //Write : X 0xFFFE Y 0xFFF5 Read : X 0xFFFE Y 0xFFF5                                                  
			NOP			W R						XAYA  				XA</XA	YA</YA		; //Write : X 0xFFFD Y 0xFFF4 Read : X 0xFFFD Y 0xFFF4                                                  
			NOP			W R						XAYA  				XA</XD	YA</YD		; //Write : X 0x0002 Y 0x000B Read : X 0x0002 Y 0x000B                                                  
			NOP			W R						XAYA  				XA</XE	YA</YE		; //Write : X 0xFFF7 Y 0xFFF2 Read : X 0xFFF7 Y 0xFFF2                                                  
			NOP			W R						XAYA  				XA</XF	YA</YF		; //Write : X 0xFFEF Y 0xFF5F Read : X 0xFFEF Y 0xFF5F                                                  
			NOP			W R						XAYA  				XA</XB	YA</YB		; //Write : X 0xFFDF Y 0xFF4F Read : X 0xFFDF Y 0xFF4F                                                  
			NOP			W R						XAYA  									; //Write : X 0xFFBF Y 0xFF3F Read : X 0xFFBF Y 0xFF3F                                                  
			NOP																			;                                     
			NOP																			;
			NOP		  XC<0x0000			YC<0x0000										;                                                                  
			NOP		  XS<0x000A			YS<0x0001										;                                                                 
			NOP		  XK<0x000B			YK<0x0002										;                                                                 
			NOP		  XA<0x000C			YA<0x0004										;                                                                 
			NOP		  XD<0x000D			YD<0x0008										;                                                                 
			NOP		  XE<0x00A0			YE<0x0010 										;                                                                 
			NOP		  XF<0x00B0			YF<0x0020										;                                                                 
			NOP		  XB<0x00C0			YB<0x0040										;                                                                 
			NOP																			;                                                 
			NOP														XA</XC	YA</YC		;                                                 
			NOP			W R						XAYA  				XA</XS	YA</YS		; //Write : X 0xFFFF Y 0xFFFF Read : X 0xFFFF Y 0xFFFF                                                  
			NOP			W R						XAYA  				XA</XK	YA</YK		; //Write : X 0xFFF5 Y 0xFFFE Read : X 0xFFF5 Y 0xFFFE                                                  
			NOP			W R						XAYA  				XA</XA	YA</YA		; //Write : X 0xFFF4 Y 0xFFFD Read : X 0xFFF4 Y 0xFFFD                                                  
			NOP			W R						XAYA  				XA</XD	YA</YD		; //Write : X 0x000B Y 0x0002 Read : X 0x000B Y 0x0002                                                  
			NOP			W R						XAYA  				XA</XE	YA</YE		; //Write : X 0xFFF2 Y 0xFFF7 Read : X 0xFFF2 Y 0xFFF7                                                  
			NOP			W R						XAYA  				XA</XF	YA</YF		; //Write : X 0xFF5F Y 0xFFEF Read : X 0xFF5F Y 0xFFEF                                                  
			NOP			W R						XAYA  				XA</XB	YA</YB		; //Write : X 0xFF4F Y 0xFFDF Read : X 0xFF4F Y 0xFFDF                                                  
			NOP			W R						XAYA  									; //Write : X 0xFF3F Y 0xFFBF Read : X 0xFF3F Y 0xFFBF
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
	
			START XA_ALU_AP1_00;
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
			NOP		  XC<0x0000			YC<0xFFFE										;                                                                  
			NOP		  XS<0x0001			YS<0x7FFF										;                                                                  
			NOP		  XK<0x0002			YK<0x3FFF										;                                                                  
			NOP		  XA<0x0004			YA<0x1FFF										;                                                                  
			NOP		  XD<0x0008			YD<0x0FFF										;                                                                  
			NOP		  XE<0x0010			YE<0x07FF	 									;                                                                  
			NOP		  XF<0x0020			YF<0x03FF										;                                                                  
			NOP		  XB<0x0040			YB<0x01FF										;                                                                  
			NOP																			;                                                                  
			NOP														XA<XC+1	YA<YC+1		; 
			NOP			W R						XAYA  				XA<XS+1	YA<YS+1		; //Write : X 0x0001 Y 0xFFFF Read : X 0x0001 Y 0xFFFF                                                                                       
			NOP			W R						XAYA  				XA<XK+1	YA<YK+1		; //Write : X 0x0002 Y 0x8000 Read : X 0x0002 Y 0x8000                                                                                       
			NOP			W R						XAYA  				XA<XA+1	YA<YA+1		; //Write : X 0x0003 Y 0x4000 Read : X 0x0003 Y 0x4000                                                                                      
			NOP			W R						XAYA  				XA<XD+1	YA<YD+1		; //Write : X 0x0004 Y 0x4001 Read : X 0x0004 Y 0x4001                                                                                      
			NOP			W R						XAYA  				XA<XE+1	YA<YE+1		; //Write : X 0x0009 Y 0x1000 Read : X 0x0009 Y 0x1000                                                                                      
			NOP			W R						XAYA  				XA<XF+1	YA<YF+1		; //Write : X 0x0011 Y 0x0800 Read : X 0x0011 Y 0x0800                                                                                      
			NOP			W R						XAYA  				XA<XB+1	YA<YB+1		; //Write : X 0x0021 Y 0x0400 Read : X 0x0021 Y 0x0400                                                                                      
			NOP			W R						XAYA  									; //Write : X 0x0041 Y 0x0200 Read : X 0x0041 Y 0x0200 
			NOP          	  	   		 			 									;
			NOP          	  	   		 			 									;                                                                 
			NOP		  XC<0xFFFE			YC<0x0000										;                                                                  
			NOP		  XS<0x7FFF			YS<0x0001										;                                                                  
			NOP		  XK<0x3FFF			YK<0x0002										;                                                                  
			NOP		  XA<0x1FFF			YA<0x0004										;                                                                  
			NOP		  XD<0x0FFF			YD<0x0008										;                                                                  
			NOP		  XE<0x07FF			YE<0x0010	 									;                                                                  
			NOP		  XF<0x03FF			YF<0x0020										;                                                                  
			NOP		  XB<0x01FF			YB<0x0040										;                                                                  
			NOP																			;                                                                  
			NOP														XA<XC+1	YA<YC+1		; 
			NOP			W R						XAYA  				XA<XS+1	YA<YS+1		; //Write : X 0xFFFF Y 0x0001 Read : X 0xFFFF Y 0x0001                                                                                       
			NOP			W R						XAYA  				XA<XK+1	YA<YK+1		; //Write : X 0x8000 Y 0x0002 Read : X 0x8000 Y 0x0002                                                                                       
			NOP			W R						XAYA  				XA<XA+1	YA<YA+1		; //Write : X 0x4000 Y 0x0003 Read : X 0x4000 Y 0x0003                                                                                      
			NOP			W R						XAYA  				XA<XD+1	YA<YD+1		; //Write : X 0x4001 Y 0x0004 Read : X 0x4001 Y 0x0004                                                                                      
			NOP			W R						XAYA  				XA<XE+1	YA<YE+1		; //Write : X 0x1000 Y 0x0009 Read : X 0x1000 Y 0x0009                                                                                      
			NOP			W R						XAYA  				XA<XF+1	YA<YF+1		; //Write : X 0x0800 Y 0x0011 Read : X 0x0800 Y 0x0011                                                                                      
			NOP			W R						XAYA  				XA<XB+1	YA<YB+1		; //Write : X 0x0400 Y 0x0021 Read : X 0x0400 Y 0x0021                                                                                      
			NOP			W R						XAYA  									; //Write : X 0x0200 Y 0x0041 Read : X 0x0200 Y 0x0041                                                 
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
	
			START XA_ALU_AM1_00;
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
			NOP		  XC<0x0001		YC<0xFFFF											;                                                                  
			NOP		  XS<0x0002		YS<0x7FFF											;                                                                  
			NOP		  XK<0x0004		YK<0x3FFF											;                                                                  
			NOP		  XA<0x0008		YA<0x1FFF											;                                                                  
			NOP		  XD<0x0010		YD<0x0FFF											;                                                                  
			NOP		  XE<0x0020		YE<0x07FF	 										;                                                                  
			NOP		  XF<0x0040		YF<0x03FF											;                                                                  
			NOP		  XB<0x0080		YB<0x01FF											;                                                                  
			NOP																			;                                                 
			NOP														XA<XC-1	YA<YC-1		;                                                 
			NOP			W R						XAYA  				XA<XS-1	YA<YS-1		; //Write : X 0x0000 Y 0xFFFE Read : X 0x0000 Y 0xFFFE                                                  
			NOP			W R						XAYA  				XA<XK-1	YA<YK-1		; //Write : X 0x0001 Y 0x7FFE Read : X 0x0001 Y 0x7FFE                                                  
			NOP			W R						XAYA  				XA<XA-1	YA<YA-1		; //Write : X 0x0003 Y 0x3FFE Read : X 0x0003 Y 0x3FFE                                                  
			NOP			W R						XAYA  				XA<XD-1	YA<YD-1		; //Write : X 0x0002 Y 0x3FFD Read : X 0x0002 Y 0x3FFD                                                  
			NOP			W R						XAYA  				XA<XE-1	YA<YE-1		; //Write : X 0x000F Y 0x0FFE Read : X 0x000F Y 0x0FFE                                                  
			NOP			W R						XAYA  				XA<XF-1	YA<YF-1		; //Write : X 0x001F Y 0x07FE Read : X 0x001F Y 0x07FE                                                  
			NOP			W R						XAYA  				XA<XB-1	YA<YB-1		; //Write : X 0x003F Y 0x03FE Read : X 0x003F Y 0x03FE                                                  
			NOP			W R						XAYA  									; //Write : X 0x007F Y 0x01FE Read : X 0x007F Y 0x01FE                                              
			NOP																			;                                     
			NOP																			;
			NOP		  XC<0xFFFF		YC<0x0001											;                                                                  
			NOP		  XS<0x7FFF		YS<0x0002											;                                                                  
			NOP		  XK<0x3FFF		YK<0x0004											;                                                                  
			NOP		  XA<0x1FFF		YA<0x0008											;                                                                  
			NOP		  XD<0x0FFF		YD<0x0010											;                                                                  
			NOP		  XE<0x07FF		YE<0x0020	 										;                                                                  
			NOP		  XF<0x03FF		YF<0x0040											;                                                                  
			NOP		  XB<0x01FF		YB<0x0080											;                                                                  
			NOP																			;                                                 
			NOP														XA<XC-1	YA<YC-1		;                                                 
			NOP			W R						XAYA  				XA<XS-1	YA<YS-1		; //Write : X 0x0000 Y 0xFFFE Read : X 0x0000 Y 0xFFFE                                                  
			NOP			W R						XAYA  				XA<XK-1	YA<YK-1		; //Write : X 0x0001 Y 0x7FFE Read : X 0x0001 Y 0x7FFE                                                  
			NOP			W R						XAYA  				XA<XA-1	YA<YA-1		; //Write : X 0x0003 Y 0x3FFE Read : X 0x0003 Y 0x3FFE                                                  
			NOP			W R						XAYA  				XA<XD-1	YA<YD-1		; //Write : X 0x0007 Y 0x1FFE Read : X 0x0007 Y 0x1FFE                                                  
			NOP			W R						XAYA  				XA<XE-1	YA<YE-1		; //Write : X 0x000F Y 0x0FFE Read : X 0x000F Y 0x0FFE                                                  
			NOP			W R						XAYA  				XA<XF-1	YA<YF-1		; //Write : X 0x001F Y 0x07FE Read : X 0x001F Y 0x07FE                                                  
			NOP			W R						XAYA  				XA<XB-1	YA<YB-1		; //Write : X 0x003F Y 0x03FE Read : X 0x003F Y 0x03FE                                                  
			NOP			W R						XAYA  									; //Write : X 0x007F Y 0x01FE Read : X 0x007F Y 0x01FE 
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
	
			START XA_ALU_AM2_00;
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
			NOP		  XC<0x0000		YC<0x0000											;                                                                  
			NOP		  XS<0x0001		YS<0x000A											;                                                                  
			NOP		  XK<0x0002		YK<0x000B											;                                                                  
			NOP		  XA<0x0004		YA<0x000C											;                                                                  
			NOP		  XD<0x0008		YD<0x000D											;                                                                  
			NOP		  XE<0x0010		YE<0x00A0	 										;                                                                  
			NOP		  XF<0x0020		YF<0x00B0											;                                                                  
			NOP		  XB<0x0040		YB<0x00C0											;                                                                  
			NOP																			;                                                 
			NOP														XA<XC*2	YA<YC*2		;                                                 
			NOP			W R						XAYA  				XA<XS*2	YA<YS*2		; //Write : X 0x0000 Y 0x0000 Read : X 0x0000 Y 0x0000                                                 
			NOP			W R						XAYA  				XA<XK*2	YA<YK*2		; //Write : X 0x0002 Y 0x0014 Read : X 0x0002 Y 0x0014                                                 
			NOP			W R						XAYA  				XA<XA*2	YA<YA*2		; //Write : X 0x0004 Y 0x0016 Read : X 0x0004 Y 0x0016                                                 
			NOP			W R						XAYA  				XA<XD*2	YA<YD*2		; //Write : X 0x0008 Y 0x002C Read : X 0x0008 Y 0x002C                                                 
			NOP			W R						XAYA  				XA<XE*2	YA<YE*2		; //Write : X 0x0010 Y 0x001A Read : X 0x0010 Y 0x001A                                                 
			NOP			W R						XAYA  				XA<XF*2	YA<YF*2		; //Write : X 0x0020 Y 0x0140 Read : X 0x0020 Y 0x0140                                                 
			NOP			W R						XAYA  				XA<XB*2	YA<YB*2		; //Write : X 0x0040 Y 0x0160 Read : X 0x0040 Y 0x0160                                                 
			NOP			W R						XAYA  									; //Write : X 0x0080 Y 0x0180 Read : X 0x0080 Y 0x0180                                                 
			NOP																			;                                     
			NOP																			;
			NOP		  XC<0x0000		YC<0x0000											;                                                                  
			NOP		  XS<0x000A		YS<0x0001											;                                                                  
			NOP		  XK<0x000B		YK<0x0002											;                                                                  
			NOP		  XA<0x000C		YA<0x0004											;                                                                  
			NOP		  XD<0x000D		YD<0x0008											;                                                                  
			NOP		  XE<0x00A0		YE<0x0010	 										;                                                                  
			NOP		  XF<0x00B0		YF<0x0020											;                                                                  
			NOP		  XB<0x00C0		YB<0x0040											;                                                                  
			NOP																			;                                                 
			NOP														XA<XC*2	YA<YC*2		;                                                 
			NOP			W R						XAYA  				XA<XS*2	YA<YS*2		; //Write : X 0x0000 Y 0x0000 Read : X 0x0000 Y 0x0000                                                 
			NOP			W R						XAYA  				XA<XK*2	YA<YK*2		; //Write : X 0x0002 Y 0x0014 Read : X 0x0002 Y 0x0014                                                 
			NOP			W R						XAYA  				XA<XA*2	YA<YA*2		; //Write : X 0x0004 Y 0x0016 Read : X 0x0004 Y 0x0016                                                 
			NOP			W R						XAYA  				XA<XD*2	YA<YD*2		; //Write : X 0x0008 Y 0x0018 Read : X 0x0008 Y 0x0018                                                 
			NOP			W R						XAYA  				XA<XE*2	YA<YE*2		; //Write : X 0x0010 Y 0x001A Read : X 0x0010 Y 0x001A                                                 
			NOP			W R						XAYA  				XA<XF*2	YA<YF*2		; //Write : X 0x0020 Y 0x0140 Read : X 0x0020 Y 0x0140                                                 
			NOP			W R						XAYA  				XA<XB*2	YA<YB*2		; //Write : X 0x0040 Y 0x0160 Read : X 0x0040 Y 0x0160                                                 
			NOP			W R						XAYA  									; //Write : X 0x0080 Y 0x0180 Read : X 0x0080 Y 0x0180  
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
		

			START XA_ALU_AD2_00;
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
			NOP		  XC<0x0000		YC<0x0000											;                                                                  
			NOP		  XS<0x0001		YS<0x000A											;                                                                  
			NOP		  XK<0x0002		YK<0x00A0											;                                                                  
			NOP		  XA<0x0004		YA<0x0A00											;                                                                  
			NOP		  XD<0x0008		YD<0xA000											;                                                                  
			NOP		  XE<0x0010		YE<0xAAAA	 										;                                                                  
			NOP		  XF<0x0020		YF<0x0AAA											;                                                                  
			NOP		  XB<0x0040		YB<0x00AA											;                                                                  
			NOP																			;                                                 
			NOP														XA<XC/2	YA<YC/2		;                                                 
			NOP			W R						XAYA  				XA<XS/2	YA<YS/2		; //Write : X 0x0000 Y 0x0000 Read : X 0x0000 Y 0x0000                                                 
			NOP			W R						XAYA  				XA<XK/2	YA<YK/2		; //Write : X 0x0000 Y 0x0005 Read : X 0x0000 Y 0x0005                                                 
			NOP			W R						XAYA  				XA<XA/2	YA<YA/2		; //Write : X 0x0001 Y 0x0050 Read : X 0x0001 Y 0x0050                                                 
			NOP			W R						XAYA  				XA<XD/2	YA<YD/2		; //Write : X 0x0000 Y 0x0028 Read : X 0x0002 Y 0x0500                                                 
			NOP			W R						XAYA  				XA<XE/2	YA<YE/2		; //Write : X 0x0004 Y 0x5000 Read : X 0x0004 Y 0xD000                                                 
			NOP			W R						XAYA  				XA<XF/2	YA<YF/2		; //Write : X 0x0008 Y 0x5555 Read : X 0x0008 Y 0xD555                                                 
			NOP			W R						XAYA  				XA<XB/2	YA<YB/2		; //Write : X 0x0010 Y 0x0555 Read : X 0x0010 Y 0x0555                                                 
			NOP			W R						XAYA  									; //Write : X 0x0020 Y 0x0055 Read : X 0x0020 Y 0x0055                                                 
			NOP																			;                                     
			NOP																			;                                     
			NOP		  XC<0x0000		YC<0x0000											;                                                                  
			NOP		  XS<0x000A		YS<0x0001											;                                                                  
			NOP		  XK<0x00A0		YK<0x0002											;                                                                  
			NOP		  XA<0x0A00		YA<0x0004											;                                                                  
			NOP		  XD<0xA000		YD<0x0008											;                                                                  
			NOP		  XE<0xAAAA		YE<0x0010	 										;                                                                  
			NOP		  XF<0x0AAA		YF<0x0020											;                                                                  
			NOP		  XB<0x00AA		YB<0x0040											;                                                                  
			NOP																			;                                                 
			NOP														XA<XC/2	YA<YC/2		;                                                 
			NOP			W R						XAYA  				XA<XS/2	YA<YS/2		; //Write : X 0x0000 Y 0x0000 Read : X 0x0000 Y 0x0000                                                 
			NOP			W R						XAYA  				XA<XK/2	YA<YK/2		; //Write : X 0x0000 Y 0x0005 Read : X 0x0000 Y 0x0005                                                 
			NOP			W R						XAYA  				XA<XA/2	YA<YA/2		; //Write : X 0x0001 Y 0x0050 Read : X 0x0001 Y 0x0050                                                 
			NOP			W R						XAYA  				XA<XD/2	YA<YD/2		; //Write : X 0x0002 Y 0x0500 Read : X 0x0002 Y 0x0500                                                 
			NOP			W R						XAYA  				XA<XE/2	YA<YE/2		; //Write : X 0x0004 Y 0xD000 Read : X 0x0004 Y 0xD000                                                 
			NOP			W R						XAYA  				XA<XF/2	YA<YF/2		; //Write : X 0x0008 Y 0xD555 Read : X 0x0008 Y 0xD555                                                 
			NOP			W R						XAYA  				XA<XB/2	YA<YB/2		; //Write : X 0x0010 Y 0x0555 Read : X 0x0010 Y 0x0555                                                 
			NOP			W R						XAYA  									; //Write : X 0x0020 Y 0x0055 Read : X 0x0020 Y 0x0055 
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
		
			START XA_ALU_B_00;
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
			NOP		  D3<D3B															;                                                                  
			NOP		  D4<D4B															;                                                                                                                                    
			NOP																			;                                                 
			NOP														XA<D1A	 YA<D2A		;                                                 
			NOP			W R						XAYA  				XA<D1B	 YA<D2B		; //Write : X 0x0000 Y 0x0080 Read : X 0x0000 Y 0x0080                                                   
			NOP			W R						XAYA  				XA<D1C	 YA<D2C		; //Write : X 0x0001 Y 0x0100 Read : X 0x0001 Y 0x0100                                                 
			NOP			W R						XAYA  				XA<D1D	 YA<D2D		; //Write : X 0x0002 Y 0x0200 Read : X 0x0002 Y 0x0200                                                 
			NOP			W R						XAYA  				XA<D1E	 YA<D1A		; //Write : X 0x0004 Y 0x0400 Read : X 0x0004 Y 0x0400                                                 
			NOP			W R						XAYA  				XA<D1F	 YA<D1B		; //Write : X 0x0008 Y 0x0000 Read : X 0x0008 Y 0x0000                                                 
			NOP			W R						XAYA  				XA<D1G	 YA<D1C		; //Write : X 0x0010 Y 0x0001 Read : X 0x0010 Y 0x0001                                                 
			NOP			W R						XAYA  				XA<D1H	 YA<D1D		; //Write : X 0x0020 Y 0x0002 Read : X 0x0020 Y 0x0002                                                 
			NOP			W R						XAYA  				XA<D2A	 YA<D1E		; //Write : X 0x0040 Y 0x0004 Read : X 0x0040 Y 0x0004                                                 
			NOP			W R						XAYA  				XA<D2B	 YA<D1F		; //Write : X 0x0080 Y 0x0008 Read : X 0x0080 Y 0x0008                                                 
			NOP			W R						XAYA  				XA<D2C	 YA<D1G		; //Write : X 0x0100 Y 0x0010 Read : X 0x0100 Y 0x0010                                                 
			NOP			W R						XAYA  				XA<D2D	 YA<D1H		; //Write : X 0x0200 Y 0x0020 Read : X 0x0200 Y 0x0020                                                 
			NOP			W R						XAYA  				XA<D3	 YA<D4		; //Write : X 0x0400 Y 0x0040 Read : X 0x0400 Y 0x0040                                                 
			NOP			W R						XAYA  				XA<D4	 YA<D3		; //Write : X 0x0800 Y 0x1000 Read : X 0x0800 Y 0x1000                                                 
			NOP			W R						XAYA  				XA<XMAX	 YA<YMAX	; //Write : X 0x1000 Y 0x0800 Read : X 0x1000 Y 0x0800                                                 
			NOP			W R						XAYA  				XA<D1A	 YA<D2A		; //Write : X 0xFFFF Y 0xFFFF Read : X 0xFFFF Y 0xFFFF                                                 
			NOP			W R						XAYA									; //Write : X 0x0000 Y 0x0080 Read : X 0x0000 Y 0x0080                                    
			NOP																			;
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
	   
			START XA_ALU_APB_00;
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
			NOP		   XC<0x0000	YC<0x8000											;                                                                  
			NOP		   XS<0x1000	YS<0x9000											;                                                                  
			NOP		   XK<0x2000	YK<0xA000											;                                                                  
			NOP		   XA<0x3000	YA<0xB000											;                                                                  
			NOP		   XD<0x4000	YD<0xC000											;                                                                  
			NOP		   XE<0x5000	YE<0xD000		 									;                                                                  
			NOP		   XF<0x6000	YF<0xE000											;                                                                  
			NOP		   XB<0x7000	YB<0xF000											;                                                                  
			NOP		  D1A<0x0001   D4B<0x0001											;                                                                  
			NOP		  D1B<0x0010   													    ;                                                                  
			NOP		  D1C<0x0100														;                                                                  
			NOP		  D1D<0x0001														;                                                                  
			NOP		  D1E<0x0010														;                                                                  
			NOP		  D1F<0x0100			 											;                                                                  
			NOP		  D1G<0x0001														;                                                                  
			NOP		  D1H<0x0010														;                                                                  
			NOP		  D2A<0x0100	D3B<0x0010											;                                                                  
			NOP		  D2B<0x0001														;                                                                  
			NOP		  D2C<0x0010														;                                                                  
			NOP		  D2D<0x0100														;                                                                  
			NOP		   D3<D3B															;                                                                  
			NOP		   D4<D4B															;                                                                  
			NOP																			;                                                 
			NOP													  XA<XC+D1A   YA<YC+D2A ;                                                 
			NOP			W R						XAYA  			  XA<XC+D1B   YA<YC+D2B ; //Write : X 0x0001 Y 0x8100 Read : X 0x0001 Y 0x8100                                                 
			NOP			W R						XAYA  			  XA<XC+D1C   YA<YC+D2C ; //Write : X 0x0010 Y 0x8001 Read : X 0x0010 Y 0x8001                                                 
			NOP			W R						XAYA  			  XA<XC+D1D   YA<YC+D2D ; //Write : X 0x0100 Y 0x8010 Read : X 0x0100 Y 0x8010                                                 
			NOP			W R						XAYA  			  XA<XC+D1E   YA<YC+D1A ; //Write : X 0x0001 Y 0x8100 Read : X 0x0001 Y 0x8100                                                 
			NOP			W R						XAYA  			  XA<XC+D1F   YA<YC+D1B ; //Write : X 0x0010 Y 0x8001 Read : X 0x0010 Y 0x8001                                                
			NOP			W R						XAYA  			  XA<XC+D1G   YA<YC+D1C ; //Write : X 0x0100 Y 0x8010 Read : X 0x0001 Y 0x8010                                                 
			NOP			W R						XAYA  			  XA<XC+D1H   YA<YC+D1D ; //Write : X 0x0001 Y 0x8100 Read : X 0x0100 Y 0x8100                                                 
			NOP			W R						XAYA  			  XA<XC+D2A   YA<YC+D1E ; //Write : X 0x0010 Y 0x8001 Read : X 0x0010 Y 0x8001                                                 
			NOP			W R						XAYA  			  XA<XC+D2B   YA<YC+D1F ; //Write : X 0x0100 Y 0x8010 Read : X 0x0100 Y 0x8010                                                 
			NOP			W R						XAYA  			  XA<XC+D2C   YA<YC+D1G ; //Write : X 0x0001 Y 0x8100 Read : X 0x0001 Y 0x8100                                                 
			NOP			W R						XAYA  			  XA<XC+D2D   YA<YC+D1H ; //Write : X 0x0010 Y 0x8001 Read : X 0x0010 Y 0x8001                                                 
			NOP			W R						XAYA  			  XA<XC+D3	  YA<YC+D4  ; //Write : X 0x0100 Y 0x8010 Read : X 0x0001 Y 0x8100                                                 
			NOP			W R						XAYA  			  XA<XC+D4	  YA<YC+D3  ; //Write : X 0x0010 Y 0x8001 Read : X 0x0010 Y 0x8001                                                 
			NOP			W R						XAYA  			  XA<XC+D1A   YA<YC+D2A ; //Write : X 0x0001 Y 0x8010 Read : X 0x0001 Y 0x8010                                                
			NOP			W R						XAYA 			  XA<XC+D1B   YA<YC+D2B ; //Write : X 0x0001 Y 0x8100 Read : X 0x0001 Y 0x8100                                    
			NOP			W R						XAYA 									; //Write : X 0x0010 Y 0x8001 Read : X 0x0010 Y 0x8001                                    
			NOP         							                                    ;
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
	
			START XA_ALU_APB_01;
			NOP         							                                    ;
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
			NOP														XA<XS+D1A	YA<YS+D2A;                                                 
			NOP			W R						XAYA  				XA<XS+D1B	YA<YS+D2B; //Write : X 0x1001 Y 0x9100 Read : X 0x1001 Y 0x9100                                                 
			NOP			W R						XAYA  				XA<XS+D1C	YA<YS+D2C; //Write : X 0x1011 Y 0x9101 Read : X 0x1011 Y 0x9101                                                 
			NOP			W R						XAYA  				XA<XS+D1D	YA<YS+D2D; //Write : X 0x1111 Y 0x9111 Read : X 0x1111 Y 0x9111                                                 
			NOP			W R						XAYA  				XA<XS+D1E	YA<YS+D1A; //Write : X 0x1112 Y 0x9211 Read : X 0x1012 Y 0x9211                                                 
			NOP			W R						XAYA  				XA<XS+D1F	YA<YS+D1B; //Write : X 0x1122 Y 0x9212 Read : X 0x1022 Y 0x9212                                                 
			NOP			W R						XAYA  				XA<XS+D1G	YA<YS+D1C; //Write : X 0x1222 Y 0x9222 Read : X 0x1222 Y 0x9222                                                 
			NOP			W R						XAYA  				XA<XS+D1H	YA<YS+D1D; //Write : X 0x1223 Y 0x9322 Read : X 0x1223 Y 0x9322                                                 
			NOP			W R						XAYA  				XA<XS+D2A	YA<YS+D1E; //Write : X 0x1233 Y 0x9323 Read : X 0x1233 Y 0x9323                                                 
			NOP			W R						XAYA  				XA<XS+D2B	YA<YS+D1F; //Write : X 0x1333 Y 0x9333 Read : X 0x1333 Y 0x9333                                                 
			NOP			W R						XAYA  				XA<XS+D2C	YA<YS+D1G; //Write : X 0x1334 Y 0x9433 Read : X 0x1334 Y 0x9433                                                 
			NOP			W R						XAYA  				XA<XS+D2D	YA<YS+D1H; //Write : X 0x1344 Y 0x9434 Read : X 0x1344 Y 0x9434                                                 
			NOP			W R						XAYA  				XA<XS+D3	YA<YS+D4 ; //Write : X 0x1444 Y 0x9444 Read : X 0x1444 Y 0x9444                                                 
			NOP			W R						XAYA  				XA<XS+D4	YA<YS+D3 ; //Write : X 0x1454 Y 0x9445 Read : X 0x1454 Y 0x9445                                                 
			NOP			W R						XAYA  				XA<XS+D1A	YA<YS+D2A; //Write : X 0x1455 Y 0x9455 Read : X 0x1455 Y 0x9455                                                 
			NOP			W R						XAYA				XA<XS+D1B	YA<YS+D2B; //Write : X 0x1001 Y 0x9100 Read : X 0x1001 Y 0x9100                                    
			NOP			W R						XAYA									;  //Write : X 0x1011 Y 0x9101 Read : X 0x1011 Y 0x9101
			NOP         							                                    ;
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
	
			START XA_ALU_APB_02;
			NOP         							                                    ;
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
			NOP														XA<XK+D1A	YA<YK+D2A;                                                 
			NOP			W R						XAYA  				XA<XK+D1B	YA<YK+D2B; //Write : X 0x2001 Y 0xA100 Read : X 0x2001 Y 0xA100                                                 
			NOP			W R						XAYA  				XA<XK+D1C	YA<YK+D2C; //Write : X 0x2011 Y 0xA101 Read : X 0x2011 Y 0xA101                                                 
			NOP			W R						XAYA  				XA<XK+D1D	YA<YK+D2D; //Write : X 0x2111 Y 0xA111 Read : X 0x2111 Y 0xA111                                                  
			NOP			W R						XAYA  				XA<XK+D1E	YA<YK+D1A; //Write : X 0x2112 Y 0xA211 Read : X 0x2112 Y 0xA211                                                 
			NOP			W R						XAYA  				XA<XK+D1F	YA<YK+D1B; //Write : X 0x2122 Y 0xA212 Read : X 0x2122 Y 0xA212                                                 
			NOP			W R						XAYA  				XA<XK+D1G	YA<YK+D1C; //Write : X 0x2222 Y 0xA222 Read : X 0x2222 Y 0xA222                                                 
			NOP			W R						XAYA  				XA<XK+D1H	YA<YK+D1D; //Write : X 0x2223 Y 0xA322 Read : X 0x2223 Y 0xA322                                                 
			NOP			W R						XAYA  				XA<XK+D2A	YA<YK+D1E; //Write : X 0x2233 Y 0xA323 Read : X 0x2233 Y 0xA323                                                   
			NOP			W R						XAYA  				XA<XK+D2B	YA<YK+D1F; //Write : X 0x2333 Y 0xA333 Read : X 0x2333 Y 0xA333                                                  
			NOP			W R						XAYA  				XA<XK+D2C	YA<YK+D1G; //Write : X 0x2334 Y 0xA433 Read : X 0x2334 Y 0xA433                                                 
			NOP			W R						XAYA  				XA<XK+D2D	YA<YK+D1H; //Write : X 0x2344 Y 0xA434 Read : X 0x2344 Y 0xA434                                                  
			NOP			W R						XAYA  				XA<XK+D3	YA<YK+D4 ; //Write : X 0x2444 Y 0xA444 Read : X 0x2444 Y 0xA444                                                 
			NOP			W R						XAYA  				XA<XK+D4	YA<YK+D3 ; //Write : X 0x2454 Y 0xA445 Read : X 0x2454 Y 0xA445                                                 
			NOP			W R						XAYA  				XA<XK+D1A	YA<YK+D2A; //Write : X 0x2455 Y 0xA455 Read : X 0x2455 Y 0xA455                                                 
			NOP			W R						XAYA				XA<XK+D1B	YA<YK+D2B; //Write : X 0x2001 Y 0xA100 Read : X 0x2001 Y 0xA100                                    
			NOP			W R						XAYA									;  //Write : X 0x2011 Y 0xA101 Read : X 0x2011 Y 0xA101
			NOP         							                                    ;
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
	

			START XA_ALU_APB_03;
			NOP         							                                    ;
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
			NOP														XA<XA+D1A	YA<YA+D2A;                                                 
			NOP			W R						XAYA  				XA<XA+D1B	YA<YA+D2B; //Write : X 0x3001 Y 0xB100 Read : X 0x3001 Y 0xB100                                                  
			NOP			W R						XAYA  				XA<XA+D1C	YA<YA+D2C; //Write : X 0x3011 Y 0xB101 Read : X 0x3011 Y 0xB101                                                 
			NOP			W R						XAYA  				XA<XA+D1D	YA<YA+D2D; //Write : X 0x3111 Y 0xB111 Read : X 0x3111 Y 0xB111                                                 
			NOP			W R						XAYA  				XA<XA+D1E	YA<YA+D1A; //Write : X 0x3112 Y 0xB211 Read : X 0x3112 Y 0xB211                                                 
			NOP			W R						XAYA  				XA<XA+D1F	YA<YA+D1B; //Write : X 0x3122 Y 0xB212 Read : X 0x3122 Y 0xB212                                                 
			NOP			W R						XAYA  				XA<XA+D1G	YA<YA+D1C; //Write : X 0x3222 Y 0xB222 Read : X 0x3222 Y 0xB222                                                 
			NOP			W R						XAYA  				XA<XA+D1H	YA<YA+D1D; //Write : X 0x3223 Y 0xB322 Read : X 0x3223 Y 0xB322                                                 
			NOP			W R						XAYA  				XA<XA+D2A	YA<YA+D1E; //Write : X 0x3233 Y 0xB323 Read : X 0x3233 Y 0xB323                                                 
			NOP			W R						XAYA  				XA<XA+D2B	YA<YA+D1F; //Write : X 0x3333 Y 0xB333 Read : X 0x3333 Y 0xB333                                                 
			NOP			W R						XAYA  				XA<XA+D2C	YA<YA+D1G; //Write : X 0x3334 Y 0xB433 Read : X 0x3334 Y 0xB433                                                 
			NOP			W R						XAYA  				XA<XA+D2D	YA<YA+D1H; //Write : X 0x3344 Y 0xB434 Read : X 0x3344 Y 0xB434                                                 
			NOP			W R						XAYA  				XA<XA+D3	YA<YA+D4 ; //Write : X 0x3444 Y 0xB444 Read : X 0x3444 Y 0xB444                                                 
			NOP			W R						XAYA  				XA<XA+D4	YA<YA+D3 ; //Write : X 0x3454 Y 0xB445 Read : X 0x3454 Y 0xB445                                                 
			NOP			W R						XAYA  				XA<XA+D1A	YA<YA+D2A; //Write : X 0x3455 Y 0xB455 Read : X 0x3455 Y 0xB455                                                 
			NOP			W R						XAYA				XA<XA+D1B	YA<YA+D2B; //Write : X 0x3555 Y 0xB456 Read : X 0x3001 Y 0xB100                                    
			NOP			W R						XAYA									;  //Write : X 0x3556 Y 0xB466 Read : X 0x3011 Y 0xB101
			NOP         							                                    ;
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


			START XA_ALU_APB_04;
			NOP         							                                    ;
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
			NOP														XA<XD+D1A	YA<YD+D2A;                                                 
			NOP			W R						XAYA  				XA<XD+D1B	YA<YD+D2B; //Write : X 0x4001 Y 0xC100 Read : X 0x4001 Y 0xC100                                                 
			NOP			W R						XAYA  				XA<XD+D1C	YA<YD+D2C; //Write : X 0x4011 Y 0xC101 Read : X 0x4011 Y 0xC101                                                 
			NOP			W R						XAYA  				XA<XD+D1D	YA<YD+D2D; //Write : X 0x4111 Y 0xC111 Read : X 0x4111 Y 0xC111                                                 
			NOP			W R						XAYA  				XA<XD+D1E	YA<YD+D1A; //Write : X 0x4112 Y 0xC211 Read : X 0x4112 Y 0xC211                                                 
			NOP			W R						XAYA  				XA<XD+D1F	YA<YD+D1B; //Write : X 0x4122 Y 0xC212 Read : X 0x4122 Y 0xC212                                                 
			NOP			W R						XAYA  				XA<XD+D1G	YA<YD+D1C; //Write : X 0x4222 Y 0xC222 Read : X 0x4222 Y 0xC222                                                 
			NOP			W R						XAYA  				XA<XD+D1H	YA<YD+D1D; //Write : X 0x4223 Y 0xC322 Read : X 0x4223 Y 0xC322                                                 
			NOP			W R						XAYA  				XA<XD+D2A	YA<YD+D1E; //Write : X 0x4233 Y 0xC323 Read : X 0x4233 Y 0xC323                                                 
			NOP			W R						XAYA  				XA<XD+D2B	YA<YD+D1F; //Write : X 0x4333 Y 0xC333 Read : X 0x4333 Y 0xC333                                                 
			NOP			W R						XAYA  				XA<XD+D2C	YA<YD+D1G; //Write : X 0x4334 Y 0xC433 Read : X 0x4334 Y 0xC433                                                 
			NOP			W R						XAYA  				XA<XD+D2D	YA<YD+D1H; //Write : X 0x4344 Y 0xC434 Read : X 0x4344 Y 0xC434                                                 
			NOP			W R						XAYA  				XA<XD+D3	YA<YD+D4 ; //Write : X 0x4444 Y 0xC444 Read : X 0x4444 Y 0xC444                                                 
			NOP			W R						XAYA  				XA<XD+D4	YA<YD+D3 ; //Write : X 0x4454 Y 0xC445 Read : X 0x4454 Y 0xC445                                                 
			NOP			W R						XAYA  				XA<XD+D1A	YA<YD+D2A; //Write : X 0x4455 Y 0xC455 Read : X 0x4455 Y 0xC455                                                 
			NOP			W R						XAYA				XA<XD+D1B	YA<YD+D2B; //Write : X 0x4001 Y 0xC100 Read : X 0x4001 Y 0xC100                                    
			NOP			W R						XAYA									 ; //Write : X 0x4011 Y 0xC101 Read : X 0x4011 Y 0xC101                                    
			NOP																			;
			NOP         							                                    ;
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
	
			START XA_ALU_APB_05;
			NOP         							                                    ;
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
			NOP														XA<XE+D1A	YA<YE+D2A;                                                 
			NOP			W R						XAYA  				XA<XE+D1B	YA<YE+D2B; //Write : X 0x5001 Y 0xD100 Read : X 0x5001 Y 0xD100                                                 
			NOP			W R						XAYA  				XA<XE+D1C	YA<YE+D2C; //Write : X 0x5011 Y 0xD101 Read : X 0x5011 Y 0xD101                                                 
			NOP			W R						XAYA  				XA<XE+D1D	YA<YE+D2D; //Write : X 0x5111 Y 0xD111 Read : X 0x5111 Y 0xD111                                                 
			NOP			W R						XAYA  				XA<XE+D1E	YA<YE+D1A; //Write : X 0x5112 Y 0xD211 Read : X 0x5112 Y 0xD211                                                 
			NOP			W R						XAYA  				XA<XE+D1F	YA<YE+D1B; //Write : X 0x5122 Y 0xD212 Read : X 0x5122 Y 0xD212                                                 
			NOP			W R						XAYA  				XA<XE+D1G	YA<YE+D1C; //Write : X 0x5222 Y 0xD222 Read : X 0x5222 Y 0xD222                                                 
			NOP			W R						XAYA  				XA<XE+D1H	YA<YE+D1D; //Write : X 0x5223 Y 0xD322 Read : X 0x5223 Y 0xD322                                                 
			NOP			W R						XAYA  				XA<XE+D2A	YA<YE+D1E; //Write : X 0x5233 Y 0xD323 Read : X 0x5233 Y 0xD323                                                 
			NOP			W R						XAYA  				XA<XE+D2B	YA<YE+D1F; //Write : X 0x5333 Y 0xD333 Read : X 0x5333 Y 0xD333                                                 
			NOP			W R						XAYA  				XA<XE+D2C	YA<YE+D1G; //Write : X 0x5334 Y 0xD433 Read : X 0x5334 Y 0xD433                                                 
			NOP			W R						XAYA  				XA<XE+D2D	YA<YE+D1H; //Write : X 0x5344 Y 0xD434 Read : X 0x5344 Y 0xD434                                                 
			NOP			W R						XAYA  				XA<XE+D3	YA<YE+D4 ; //Write : X 0x5444 Y 0xD444 Read : X 0x5444 Y 0xD444                                                 
			NOP			W R						XAYA  				XA<XE+D4	YA<YE+D3 ; //Write : X 0x5454 Y 0xD445 Read : X 0x5454 Y 0xD445                                                 
			NOP			W R						XAYA  				XA<XE+D1A	YA<YE+D2A; //Write : X 0x5455 Y 0xD455 Read : X 0x5455 Y 0xD455                                                 
			NOP			W R						XAYA				XA<XE+D1B	YA<YE+D2B; //Write : X 0x5001 Y 0xD100 Read : X 0x5001 Y 0xD100
			NOP         W R						XAYA				                     ; //Write : X 0x5011 Y 0xD101 Read : X 0x5011 Y 0xD101
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
	
			START XA_ALU_APB_06;
			NOP         							                                    ;
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
			NOP														XA<XF+D1A	YA<YF+D2A;                                                 
			NOP			W R						XAYA  				XA<XF+D1B	YA<YF+D2B; //Write : X 0x6001 Y 0xE100 Read : X 0x6001 Y 0xE100                                                 
			NOP			W R						XAYA  				XA<XF+D1C	YA<YF+D2C; //Write : X 0x6011 Y 0xE101 Read : X 0x6011 Y 0xE101                                                 
			NOP			W R						XAYA  				XA<XF+D1D	YA<YF+D2D; //Write : X 0x6111 Y 0xE111 Read : X 0x6111 Y 0xE111                                                 
			NOP			W R						XAYA  				XA<XF+D1E	YA<YF+D1A; //Write : X 0x6112 Y 0xE211 Read : X 0x6112 Y 0xE211                                                 
			NOP			W R						XAYA  				XA<XF+D1F	YA<YF+D1B; //Write : X 0x6122 Y 0xE212 Read : X 0x6122 Y 0xE212                                                 
			NOP			W R						XAYA  				XA<XF+D1G	YA<YF+D1C; //Write : X 0x6222 Y 0xE222 Read : X 0x6222 Y 0xE222                                                 
			NOP			W R						XAYA  				XA<XF+D1H	YA<YF+D1D; //Write : X 0x6223 Y 0xE322 Read : X 0x6223 Y 0xE322                                                 
			NOP			W R						XAYA  				XA<XF+D2A	YA<YF+D1E; //Write : X 0x6233 Y 0xE323 Read : X 0x6233 Y 0xE323                                                 
			NOP			W R						XAYA  				XA<XF+D2B	YA<YF+D1F; //Write : X 0x6333 Y 0xE333 Read : X 0x6333 Y 0xE333                                                 
			NOP			W R						XAYA  				XA<XF+D2C	YA<YF+D1G; //Write : X 0x6334 Y 0xE433 Read : X 0x6334 Y 0xE433                                                 
			NOP			W R						XAYA  				XA<XF+D2D	YA<YF+D1H; //Write : X 0x6344 Y 0xE434 Read : X 0x6344 Y 0xE434                                                 
			NOP			W R						XAYA  				XA<XF+D3	YA<YF+D4 ; //Write : X 0x6444 Y 0xE444 Read : X 0x6444 Y 0xE444                                                 
			NOP			W R						XAYA  				XA<XF+D4	YA<YF+D3 ; //Write : X 0x6454 Y 0xE445 Read : X 0x6454 Y 0xE445                                                 
			NOP			W R						XAYA  				XA<XF+D1A	YA<YF+D2A; //Write : X 0x6455 Y 0xE455 Read : X 0x6455 Y 0xE455                                                 
			NOP			W R						XAYA				XA<XF+D1B	YA<YF+D2B; //Write : X 0x6001 Y 0xE100 Read : X 0x6001 Y 0xE100                                     
			NOP			W R						XAYA									 ; //Write : X 0x6011 Y 0xE101 Read : X 0x6011 Y 0xE101
			NOP         							                                    ;
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
	
			START XA_ALU_APB_07;
			NOP         							                                    ;
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
			NOP														XA<XB+D1A	YA<YB+D2A;                                                 
			NOP			W R						XAYA  				XA<XB+D1B	YA<YB+D2B; //Write : X 0x7001 Y 0xF100 Read : X 0x7001 Y 0xF100                                                 
			NOP			W R						XAYA  				XA<XB+D1C	YA<YB+D2C; //Write : X 0x7011 Y 0xF101 Read : X 0x7011 Y 0xF101                                                 
			NOP			W R						XAYA  				XA<XB+D1D	YA<YB+D2D; //Write : X 0x7111 Y 0xF111 Read : X 0x7111 Y 0xF111                                                 
			NOP			W R						XAYA  				XA<XB+D1E	YA<YB+D1A; //Write : X 0x7112 Y 0xF211 Read : X 0x7112 Y 0xF211                                                 
			NOP			W R						XAYA  				XA<XB+D1F	YA<YB+D1B; //Write : X 0x7122 Y 0xF212 Read : X 0x7122 Y 0xF212                                                 
			NOP			W R						XAYA  				XA<XB+D1G	YA<YB+D1C; //Write : X 0x7222 Y 0xF222 Read : X 0x7222 Y 0xF222                                                 
			NOP			W R						XAYA  				XA<XB+D1H	YA<YB+D1D; //Write : X 0x7223 Y 0xF322 Read : X 0x7223 Y 0xF322                                                 
			NOP			W R						XAYA  				XA<XB+D2A	YA<YB+D1E; //Write : X 0x7233 Y 0xF323 Read : X 0x7233 Y 0xF323                                                 
			NOP			W R						XAYA  				XA<XB+D2B	YA<YB+D1F; //Write : X 0x7333 Y 0xF333 Read : X 0x7333 Y 0xF333                                                 
			NOP			W R						XAYA  				XA<XB+D2C	YA<YB+D1G; //Write : X 0x7334 Y 0xF433 Read : X 0x7334 Y 0xF433                                                 
			NOP			W R						XAYA  				XA<XB+D2D	YA<YB+D1H; //Write : X 0x7344 Y 0xF434 Read : X 0x7344 Y 0xF434                                                 
			NOP			W R						XAYA  				XA<XB+D3	YA<YB+D4 ; //Write : X 0x7444 Y 0xF444 Read : X 0x7444 Y 0xF444                                                 
			NOP			W R						XAYA  				XA<XB+D4	YA<YB+D3 ; //Write : X 0x7454 Y 0xF445 Read : X 0x7454 Y 0xF445                                                 
			NOP			W R						XAYA  				XA<XB+D1A	YA<YB+D2A; //Write : X 0x7455 Y 0xF455 Read : X 0x7455 Y 0xF455                                                 
			NOP			W R						XAYA				XA<XB+D1B	YA<YB+D2B; //Write : X 0x7001 Y 0xF100 Read : X 0x7001 Y 0xF100                                     
			NOP			W R						XAYA									 ; //Write : X 0x7011 Y 0xF101 Read : X 0x7011 Y 0xF101                                     
			NOP         							                                    ;
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
	
			START XA_ALU_AMB_00;
			NOP         							                                    ;
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
			NOP														XA<XC-D1A	YA<YC-D1A;                                                 
			NOP			W R						XAYA  				XA<XC-D1B	YA<YC-D1B; //Write : X 0x0665 Y 0x0554 Read : X 0x0665 Y 0x0554                                                 
			NOP			W R						XAYA  				XA<XC-D1C	YA<YC-D1C; //Write : X 0x0656 Y 0x0545 Read : X 0x0655 Y 0x0544                                                 
			NOP			W R						XAYA  				XA<XC-D1D	YA<YC-D1D; //Write : X 0x0566 Y 0x0455 Read : X 0x0555 Y 0x0444                                                 
			NOP			W R						XAYA  				XA<XC-D1E	YA<YC-D1E; //Write : X 0x0665 Y 0x0554 Read : X 0x0554 Y 0x0443                                                 
			NOP			W R						XAYA  				XA<XC-D1F	YA<YC-D1F; //Write : X 0x0656 Y 0x0545 Read : X 0x0544 Y 0x0433                                                 
			NOP			W R						XAYA  				XA<XC-D1G	YA<YC-D1G; //Write : X 0x0566 Y 0x0455 Read : X 0x0444 Y 0x0333                                                 
			NOP			W R						XAYA  				XA<XC-D1H	YA<YC-D1H; //Write : X 0x0665 Y 0x0554 Read : X 0x0443 Y 0x0332                                                 
			NOP			W R						XAYA  				XA<XC-D2A	YA<YC-D2A; //Write : X 0x0656 Y 0x0545 Read : X 0x0433 Y 0x0322                                                 
			NOP			W R						XAYA  				XA<XC-D2B	YA<YC-D2B; //Write : X 0x0566 Y 0x0455 Read : X 0x0333 Y 0x0222                                                 
			NOP			W R						XAYA  				XA<XC-D2C	YA<YC-D2C; //Write : X 0x0665 Y 0x0554 Read : X 0x0332 Y 0x0221                                                 
			NOP			W R						XAYA  				XA<XC-D2D	YA<YC-D2D; //Write : X 0x0656 Y 0x0545 Read : X 0x0322 Y 0x0211                                                 
			NOP			W R						XAYA  				XA<XC-D3	YA<YC-D3 ; //Write : X 0x0566 Y 0x0455 Read : X 0x0222 Y 0xF444                                                 
			NOP			W R						XAYA  				XA<XC-D4	YA<YC-D4 ; //Write : X 0x0656 Y 0x0545 Read : X 0x0212 Y 0x0101                                                 
			NOP			W R						XAYA  				XA<XC-D1A	YA<YC-D1A; //Write : X 0x0665 Y 0x0554 Read : X 0x0211 Y 0x0101                                                 
			NOP         W R						XAYA				XA<XC-D1B	YA<YC-D1B; //Write : X 0x0665 Y 0x0554 Read : X 0x0665 Y 0x0554
			NOP         W R						XAYA 	  	   							;  //Write : X 0x0656 Y 0x0545 Read : X 0x0655 Y 0x0544
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

			START XA_ALU_AMB_01;
			NOP         							                                    ;
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
			NOP														XA<XS-D1A	YA<YS-D1A;                                                 
			NOP			W R						XAYA  				XA<XS-D1B	YA<YS-D1B; //Write : X 0x0776 Y 0x0665 Read : X 0x0776 Y 0x0665                                                 
			NOP			W R						XAYA  				XA<XS-D1C	YA<YS-D1C; //Write : X 0x0767 Y 0x0656 Read : X 0x0766 Y 0x0655                                                 
			NOP			W R						XAYA  				XA<XS-D1D	YA<YS-D1D; //Write : X 0x0677 Y 0x0566 Read : X 0x0666 Y 0x0555                                                 
			NOP			W R						XAYA  				XA<XS-D1E	YA<YS-D1E; //Write : X 0x0776 Y 0x0665 Read : X 0x0665 Y 0x0554                                                 
			NOP			W R						XAYA  				XA<XS-D1F	YA<YS-D1F; //Write : X 0x0767 Y 0x0656 Read : X 0x0655 Y 0x0544                                                 
			NOP			W R						XAYA  				XA<XS-D1G	YA<YS-D1G; //Write : X 0x0677 Y 0x0566 Read : X 0x0555 Y 0x0444                                                 
			NOP			W R						XAYA  				XA<XS-D1H	YA<YS-D1H; //Write : X 0x0776 Y 0x0665 Read : X 0x0554 Y 0x0443                                                 
			NOP			W R						XAYA  				XA<XS-D2A	YA<YS-D2A; //Write : X 0x0767 Y 0x0656 Read : X 0x0544 Y 0x0433                                                 
			NOP			W R						XAYA  				XA<XS-D2B	YA<YS-D2B; //Write : X 0x0677 Y 0x0566 Read : X 0x0444 Y 0x0333                                                 
			NOP			W R						XAYA  				XA<XS-D2C	YA<YS-D2C; //Write : X 0x0776 Y 0x0665 Read : X 0x0443 Y 0x0332                                                 
			NOP			W R						XAYA  				XA<XS-D2D	YA<YS-D2D; //Write : X 0x0767 Y 0x0656 Read : X 0x0433 Y 0x0322                                                 
			NOP			W R						XAYA  				XA<XS-D3	YA<YS-D3 ; //Write : X 0x0677 Y 0x0566 Read : X 0x0333 Y 0x0222                                                 
			NOP			W R						XAYA  				XA<XS-D4	YA<YS-D4 ; //Write : X 0x0767 Y 0x0656 Read : X 0x0323 Y 0x0212                                                 
			NOP			W R						XAYA  				XA<XS-D1A	YA<YS-D1A; //Write : X 0x0776 Y 0x0665 Read : X 0x0322 Y 0x0211                                                 
			NOP         W R						XAYA				XA<XS-D1B	YA<YS-D1B; //Write : X 0x0776 Y 0x0665 Read : X 0x0776 Y 0x0665
			NOP         W R						XAYA 	  	   		 			 		 ; //Write : X 0x0767 Y 0x0656 Read : X 0x0766 Y 0x0655
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
	
			START XA_ALU_AMB_02;
			NOP         							                                    ;
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
			NOP														XA<XK-D1A	YA<YK-D1A;                                                 
			NOP			W R						XAYA  				XA<XK-D1B	YA<YK-D1B; //Write : X 0x0887 Y 0x0776 Read : X 0x0887 Y 0x0776                                                 
			NOP			W R						XAYA  				XA<XK-D1C	YA<YK-D1C; //Write : X 0x0878 Y 0x0767 Read : X 0x0877 Y 0x0766                                                 
			NOP			W R						XAYA  				XA<XK-D1D	YA<YK-D1D; //Write : X 0x0788 Y 0x0677 Read : X 0x0777 Y 0x0666                                                 
			NOP			W R						XAYA  				XA<XK-D1E	YA<YK-D1E; //Write : X 0x0887 Y 0x0776 Read : X 0x0776 Y 0x0665                                                 
			NOP			W R						XAYA  				XA<XK-D1F	YA<YK-D1F; //Write : X 0x0878 Y 0x0767 Read : X 0x0766 Y 0x0655                                                 
			NOP			W R						XAYA  				XA<XK-D1G	YA<YK-D1G; //Write : X 0x0788 Y 0x0677 Read : X 0x0777 Y 0x0555                                                 
			NOP			W R						XAYA  				XA<XK-D1H	YA<YK-D1H; //Write : X 0x0887 Y 0x0776 Read : X 0x0776 Y 0x0554                                                 
			NOP			W R						XAYA  				XA<XK-D2A	YA<YK-D2A; //Write : X 0x0878 Y 0x0767 Read : X 0x0766 Y 0x0544                                                 
			NOP			W R						XAYA  				XA<XK-D2B	YA<YK-D2B; //Write : X 0x0788 Y 0x0677 Read : X 0x0666 Y 0x0444                                                 
			NOP			W R						XAYA  				XA<XK-D2C	YA<YK-D2C; //Write : X 0x0887 Y 0x0776 Read : X 0x0665 Y 0x0443                                                 
			NOP			W R						XAYA  				XA<XK-D2D	YA<YK-D2D; //Write : X 0x0878 Y 0x0767 Read : X 0x0655 Y 0x0433                                                 
			NOP			W R						XAYA  				XA<XK-D3	YA<YK-D3 ; //Write : X 0x0788 Y 0x0677 Read : X 0x0555 Y 0x0333                                                 
			NOP			W R						XAYA  				XA<XK-D4	YA<YK-D4 ; //Write : X 0x0878 Y 0x0767 Read : X 0x0545 Y 0x0323                                                 
			NOP			W R						XAYA  				XA<XK-D1A	YA<YK-D1A; //Write : X 0x0887 Y 0x0776 Read : X 0x0544 Y 0x0322                                                 
			NOP         W R						XAYA				XA<XK-D1B	YA<YK-D1B; //Write : X 0x0887 Y 0x0776 Read : X 0x0887 Y 0x0776
			NOP         W R						XAYA 	  	   		 			 		 ; //Write : X 0x0878 Y 0x0767 Read : X 0x0877 Y 0x0766
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
	
			START XA_ALU_AMB_03;
			NOP         							                                    ;
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
			NOP														XA<XA-D1A	YA<YA-D1A;                                                 
			NOP			W R						XAYA  				XA<XA-D1B	YA<YA-D1B; //Write : X 0x0998 Y 0x0887 Read : X 0x0998 Y 0x0887                                                 
			NOP			W R						XAYA  				XA<XA-D1C	YA<YA-D1C; //Write : X 0x0988 Y 0x0877 Read : X 0x0988 Y 0x0877                                                 
			NOP			W R						XAYA  				XA<XA-D1D	YA<YA-D1D; //Write : X 0x0888 Y 0x0777 Read : X 0x0888 Y 0x0777                                                 
			NOP			W R						XAYA  				XA<XA-D1E	YA<YA-D1E; //Write : X 0x0887 Y 0x0776 Read : X 0x0887 Y 0x0776                                                 
			NOP			W R						XAYA  				XA<XA-D1F	YA<YA-D1F; //Write : X 0x0877 Y 0x0766 Read : X 0x0877 Y 0x0766                                                 
			NOP			W R						XAYA  				XA<XA-D1G	YA<YA-D1G; //Write : X 0x0777 Y 0x0666 Read : X 0x0777 Y 0x0666                                                 
			NOP			W R						XAYA  				XA<XA-D1H	YA<YA-D1H; //Write : X 0x0776 Y 0x0665 Read : X 0x0776 Y 0x0665                                                 
			NOP			W R						XAYA  				XA<XA-D2A	YA<YA-D2A; //Write : X 0x0766 Y 0x0655 Read : X 0x0766 Y 0x0655                                                 
			NOP			W R						XAYA  				XA<XA-D2B	YA<YA-D2B; //Write : X 0x0666 Y 0x0555 Read : X 0x0666 Y 0x0555                                                 
			NOP			W R						XAYA  				XA<XA-D2C	YA<YA-D2C; //Write : X 0x0665 Y 0x0554 Read : X 0x0665 Y 0x0554                                                 
			NOP			W R						XAYA  				XA<XA-D2D	YA<YA-D2D; //Write : X 0x0655 Y 0x0544 Read : X 0x0655 Y 0x0544                                                 
			NOP			W R						XAYA  				XA<XA-D3	YA<YA-D3 ; //Write : X 0x0555 Y 0x0444 Read : X 0x0555 Y 0x0444                                                 
			NOP			W R						XAYA  				XA<XA-D4	YA<YA-D4 ; //Write : X 0x0545 Y 0x0434 Read : X 0x0545 Y 0x0434                                                 
			NOP			W R						XAYA  				XA<XA-D1A	YA<YA-D1A; //Write : X 0x0544 Y 0x0433 Read : X 0x0544 Y 0x0433                                                 
			NOP         W R						XAYA				XA<XA-D1B	YA<YA-D1B; //Write : X 0x0543 Y 0x0432 Read : X 0x0998 Y 0x0887
			NOP         W R						XAYA 	  	   		 			 		 ; //Write : X 0x0533 Y 0x0422 Read : X 0x0988 Y 0x0877
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
	
			START XA_ALU_AMB_04;
			NOP         							                                    ;
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
			NOP														XA<XD-D1A	YA<YD-D1A;                                                 
			NOP			W R						XAYA  				XA<XD-D1B	YA<YD-D1B; //Write : X 0x0AA9 Y 0x0998 Read : X 0x0AA9 Y 0x0998                                                 
			NOP			W R						XAYA  				XA<XD-D1C	YA<YD-D1C; //Write : X 0x0A9A Y 0x0989 Read : X 0x0A99 Y 0x0988                                                 
			NOP			W R						XAYA  				XA<XD-D1D	YA<YD-D1D; //Write : X 0x09AA Y 0x0899 Read : X 0x0999 Y 0x0888                                                 
			NOP			W R						XAYA  				XA<XD-D1E	YA<YD-D1E; //Write : X 0x0AA9 Y 0x0998 Read : X 0x0998 Y 0x0887                                                 
			NOP			W R						XAYA  				XA<XD-D1F	YA<YD-D1F; //Write : X 0x0A9A Y 0x0989 Read : X 0x0988 Y 0x0877                                                 
			NOP			W R						XAYA  				XA<XD-D1G	YA<YD-D1G; //Write : X 0x09AA Y 0x0899 Read : X 0x0888 Y 0x0777                                                 
			NOP			W R						XAYA  				XA<XD-D1H	YA<YD-D1H; //Write : X 0x0AA9 Y 0x0998 Read : X 0x0887 Y 0x0776                                                 
			NOP			W R						XAYA  				XA<XD-D2A	YA<YD-D2A; //Write : X 0x0A9A Y 0x0989 Read : X 0x0877 Y 0x0766                                                 
			NOP			W R						XAYA  				XA<XD-D2B	YA<YD-D2B; //Write : X 0x09AA Y 0x0899 Read : X 0x0777 Y 0x0666                                                 
			NOP			W R						XAYA  				XA<XD-D2C	YA<YD-D2C; //Write : X 0x0AA9 Y 0x0998 Read : X 0x0776 Y 0x0665                                                 
			NOP			W R						XAYA  				XA<XD-D2D	YA<YD-D2D; //Write : X 0x0A9A Y 0x0989 Read : X 0x0766 Y 0x0655                                                 
			NOP			W R						XAYA  				XA<XD-D3	YA<YD-D3 ; //Write : X 0x09AA Y 0x0899 Read : X 0x0666 Y 0x0555                                                 
			NOP			W R						XAYA  				XA<XD-D4	YA<YD-D4 ; //Write : X 0x0A9A Y 0x0989 Read : X 0x0656 Y 0x0545                                                 
			NOP			W R						XAYA  				XA<XD-D1A	YA<YD-D1A; //Write : X 0x0AA9 Y 0x0998 Read : X 0x0655 Y 0x0544                                                 
			NOP         W R						XAYA				XA<XD-D1B	YA<YD-D1B; //Write : X 0x0AA9 Y 0x0998 Read : X 0x0AA9 Y 0x0998
			NOP         W R						XAYA 	  	   		 			 		 ; //Write : X 0x0A9A Y 0x0989 Read : X 0x0A99 Y 0x0988
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
		
			START XA_ALU_AMB_05;
			NOP         							                                    ;
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
			NOP														XA<XE-D1A	YA<YE-D1A;                                                 
			NOP			W R						XAYA  				XA<XE-D1B	YA<YE-D1B; //Write : X 0x0BBA Y 0x0AA9 Read : X 0x0BBA Y 0x0AA9                                                 
			NOP			W R						XAYA  				XA<XE-D1C	YA<YE-D1C; //Write : X 0x0BAB Y 0x0A9A Read : X 0x0BAA Y 0x0A99                                                 
			NOP			W R						XAYA  				XA<XE-D1D	YA<YE-D1D; //Write : X 0x0ABB Y 0x09AA Read : X 0x0AAA Y 0x0999                                                 
			NOP			W R						XAYA  				XA<XE-D1E	YA<YE-D1E; //Write : X 0x0BBA Y 0x0AA9 Read : X 0x0AA9 Y 0x0998                                                 
			NOP			W R						XAYA  				XA<XE-D1F	YA<YE-D1F; //Write : X 0x0BAB Y 0x0A9A Read : X 0x0A99 Y 0x0988                                                 
			NOP			W R						XAYA  				XA<XE-D1G	YA<YE-D1G; //Write : X 0x0ABB Y 0x09AA Read : X 0x0999 Y 0x0888                                                 
			NOP			W R						XAYA  				XA<XE-D1H	YA<YE-D1H; //Write : X 0x0BBA Y 0x0AA9 Read : X 0x0998 Y 0x0887                                                 
			NOP			W R						XAYA  				XA<XE-D2A	YA<YE-D2A; //Write : X 0x0BAB Y 0x0A9A Read : X 0x0988 Y 0x0877                                                 
			NOP			W R						XAYA  				XA<XE-D2B	YA<YE-D2B; //Write : X 0x0ABB Y 0x09AA Read : X 0x0888 Y 0x0777                                                 
			NOP			W R						XAYA  				XA<XE-D2C	YA<YE-D2C; //Write : X 0x0BBA Y 0x0AA9 Read : X 0x0887 Y 0x0776                                                 
			NOP			W R						XAYA  				XA<XE-D2D	YA<YE-D2D; //Write : X 0x0BAB Y 0x0A9A Read : X 0x0877 Y 0x0766                                                 
			NOP			W R						XAYA  				XA<XE-D3	YA<YE-D3 ; //Write : X 0x0ABB Y 0x09AA Read : X 0x0777 Y 0x0666                                                 
			NOP			W R						XAYA  				XA<XE-D4	YA<YE-D4 ; //Write : X 0x0BAB Y 0x0A9A Read : X 0x0767 Y 0x0656                                                 
			NOP			W R						XAYA  				XA<XE-D1A	YA<YE-D1A; //Write : X 0x0BBA Y 0x0AA9 Read : X 0x0766 Y 0x0655                                                 
			NOP         W R						XAYA				XA<XE-D1B	YA<YE-D1B; //Write : X 0x0BBA Y 0x0AA9 Read : X 0x0BBA Y 0x0AA9
			NOP         W R						XAYA 	  	   		 			 		 ; //Write : X 0x0BAB Y 0x0A9A Read : X 0x0BAA Y 0x0A99
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
		
			START XA_ALU_AMB_06;
			NOP         							                                    ;
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
			NOP														XA<XF-D1A	YA<YF-D1A;                                                 
			NOP			W R						XAYA  				XA<XF-D1B	YA<YF-D1B; //Write : X 0x0CCB Y 0x0BBA Read : X 0x0CCB Y 0x0BBA                                                 
			NOP			W R						XAYA  				XA<XF-D1C	YA<YF-D1C; //Write : X 0x0CBC Y 0x0BAB Read : X 0x0CBB Y 0x0BAA                                                 
			NOP			W R						XAYA  				XA<XF-D1D	YA<YF-D1D; //Write : X 0x0BCC Y 0x0ABB Read : X 0x0BBB Y 0x0AAA                                                 
			NOP			W R						XAYA  				XA<XF-D1E	YA<YF-D1E; //Write : X 0x0CCB Y 0x0BBA Read : X 0x0BBA Y 0x0AA9                                                 
			NOP			W R						XAYA  				XA<XF-D1F	YA<YF-D1F; //Write : X 0x0CBC Y 0x0BAB Read : X 0x0BAA Y 0x0A99                                                 
			NOP			W R						XAYA  				XA<XF-D1G	YA<YF-D1G; //Write : X 0x0BCC Y 0x0ABB Read : X 0x0AAA Y 0x0999                                                 
			NOP			W R						XAYA  				XA<XF-D1H	YA<YF-D1H; //Write : X 0x0CCB Y 0x0BBA Read : X 0x0AA9 Y 0x0998                                                 
			NOP			W R						XAYA  				XA<XF-D2A	YA<YF-D2A; //Write : X 0x0CBC Y 0x0BAB Read : X 0x0A99 Y 0x0988                                                 
			NOP			W R						XAYA  				XA<XF-D2B	YA<YF-D2B; //Write : X 0x0BCC Y 0x0ABB Read : X 0x0999 Y 0x0888                                                 
			NOP			W R						XAYA  				XA<XF-D2C	YA<YF-D2C; //Write : X 0x0CCB Y 0x0BBA Read : X 0x0998 Y 0x0887                                                 
			NOP			W R						XAYA  				XA<XF-D2D	YA<YF-D2D; //Write : X 0x0CBC Y 0x0BAB Read : X 0x0988 Y 0x0877                                                 
			NOP			W R						XAYA  				XA<XF-D3	YA<YF-D3 ; //Write : X 0x0BCC Y 0x0ABB Read : X 0x0888 Y 0x0777                                                 
			NOP			W R						XAYA  				XA<XF-D4	YA<YF-D4 ; //Write : X 0x0CBC Y 0x0BAB Read : X 0x0878 Y 0x0767                                                 
			NOP			W R						XAYA  				XA<XF-D1A	YA<YF-D1A; //Write : X 0x0CCB Y 0x0BBA Read : X 0x0877 Y 0x0766                                                 
			NOP         W R						XAYA				XA<XF-D1B	YA<YF-D1B; //Write : X 0x0CCB Y 0x0BBA Read : X 0x0CCB Y 0x0BBA
			NOP         W R						XAYA 	  	   		 			 		 ; //Write : X 0x0CBC Y 0x0BAB Read : X 0x0CBB Y 0x0BAA
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
	
			START XA_ALU_AMB_07;
			NOP         							                                    ;
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
			NOP														XA<XB-D1A	YA<YB-D1A;                                                 
			NOP			W R						XAYA  				XA<XB-D1B	YA<YB-D1B; //Write : X 0x0DDC Y 0x0CCB Read : X 0x0DDC Y 0x0CCB                                                 
			NOP			W R						XAYA  				XA<XB-D1C	YA<YB-D1C; //Write : X 0x0DCD Y 0x0CBC Read : X 0x0DCC Y 0x0CBB                                                 
			NOP			W R						XAYA  				XA<XB-D1D	YA<YB-D1D; //Write : X 0x0CDD Y 0x0BCC Read : X 0x0CCC Y 0x0BBB                                                 
			NOP			W R						XAYA  				XA<XB-D1E	YA<YB-D1E; //Write : X 0x0DDC Y 0x0CCB Read : X 0x0CCB Y 0x0BBA                                                 
			NOP			W R						XAYA  				XA<XB-D1F	YA<YB-D1F; //Write : X 0x0DCD Y 0x0CBC Read : X 0x0CBB Y 0x0BAA                                                 
			NOP			W R						XAYA  				XA<XB-D1G	YA<YB-D1G; //Write : X 0x0CDD Y 0x0BCC Read : X 0x0BBB Y 0x0AAA                                                 
			NOP			W R						XAYA  				XA<XB-D1H	YA<YB-D1H; //Write : X 0x0DDC Y 0x0CCB Read : X 0x0BBA Y 0x0AA9                                                 
			NOP			W R						XAYA  				XA<XB-D2A	YA<YB-D2A; //Write : X 0x0DCD Y 0x0CBC Read : X 0x0BAA Y 0x0A99                                                 
			NOP			W R						XAYA  				XA<XB-D2B	YA<YB-D2B; //Write : X 0x0CDD Y 0x0BCC Read : X 0x0AAA Y 0x0999                                                 
			NOP			W R						XAYA  				XA<XB-D2C	YA<YB-D2C; //Write : X 0x0DDC Y 0x0CCB Read : X 0x0AA9 Y 0x0998                                                 
			NOP			W R						XAYA  				XA<XB-D2D	YA<YB-D2D; //Write : X 0x0DCD Y 0x0CBC Read : X 0x0A99 Y 0x0988                                                 
			NOP			W R						XAYA  				XA<XB-D3	YA<YB-D3 ; //Write : X 0x0CDD Y 0x0BCC Read : X 0x0999 Y 0x0888                                                 
			NOP			W R						XAYA  				XA<XB-D4	YA<YB-D4 ; //Write : X 0x0DCD Y 0x0CBC Read : X 0x0989 Y 0x0878                                                 
			NOP			W R						XAYA  				XA<XB-D1A	YA<YB-D1A; //Write : X 0x0DDC Y 0x0CCB Read : X 0x0988 Y 0x0877                                                 
			NOP         W R						XAYA				XA<XB-D1B	YA<YB-D1B; //Write : X 0x0DDC Y 0x0CCB Read : X 0x0DDC Y 0x0CCB
			NOP         W R						XAYA 	  	   		 			 		 ; //Write : X 0x0DCD Y 0x0CBC Read : X 0x0DCC Y 0x0CBB
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
					
		
			START XA_ALU_AANDB_00;
			NOP         							                                    ;
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
			NOP		   XC<0xFFFF	YC<0xEFFF													;                                                                  
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
			NOP														XA<XC&D1A	YA<YC&D1A;                                                 
			NOP			W R						XAYA  				XA<XC&D1B	YA<YC&D1B; //Write : X 0xFFFE Y 0xEFFE Read : X 0xFFFE Y 0xEFFE                                                  
			NOP			W R						XAYA  				XA<XC&D1C	YA<YC&D1C; //Write : X 0xFFFD Y 0xEFFD Read : X 0xFFFC Y 0xEFFC                                                 
			NOP			W R						XAYA  				XA<XC&D1D	YA<YC&D1D; //Write : X 0xFFFB Y 0xEFFB Read : X 0xFFF8 Y 0xEFF8                                                 
			NOP			W R						XAYA  				XA<XC&D1E	YA<YC&D1E; //Write : X 0xFFF7 Y 0xEFF7 Read : X 0xFFF0 Y 0xEFF0                                                 
			NOP			W R						XAYA  				XA<XC&D1F	YA<YC&D1F; //Write : X 0xFFEF Y 0xEFEF Read : X 0xFFE0 Y 0xEFE0                                                 
			NOP			W R						XAYA  				XA<XC&D1G	YA<YC&D1G; //Write : X 0xFFDF Y 0xEFDF Read : X 0xFFC0 Y 0xEFC0                                                 
			NOP			W R						XAYA  				XA<XC&D1H	YA<YC&D1H; //Write : X 0xFFBF Y 0xEFBF Read : X 0xFF80 Y 0xEF80                                                 
			NOP			W R						XAYA  				XA<XC&D2A	YA<YC&D2A; //Write : X 0xFF7F Y 0xEF7F Read : X 0xFF00 Y 0xEF00                                                 
			NOP			W R						XAYA  				XA<XC&D2B	YA<YC&D2B; //Write : X 0xFEFF Y 0xEEFF Read : X 0xFE00 Y 0xEE00                                                 
			NOP			W R						XAYA  				XA<XC&D2C	YA<YC&D2C; //Write : X 0xFDFF Y 0xEDFF Read : X 0xFC00 Y 0xEC00                                                 
			NOP			W R						XAYA  				XA<XC&D2D	YA<YC&D2D; //Write : X 0xFBFF Y 0xEBFF Read : X 0xF800 Y 0xE800                                                 
			NOP			W R						XAYA  				XA<XC&D3	YA<YC&D3 ; //Write : X 0xF7FF Y 0xE7FF Read : X 0xF000 Y 0xE000                                                 
			NOP			W R						XAYA  				XA<XC&D4	YA<YC&D4 ; //Write : X 0xEFFF Y 0xEFFF Read : X 0xE000 Y 0xE000                                                 
			NOP			W R						XAYA  									 ; //Write : X 0xDFFF Y 0xCFFF Read : X 0xC000 Y 0xC000                                                 
			NOP         							                                    ;
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
			
			START XA_ALU_AANDB_01;
			NOP         							                                    ;
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
			NOP														XA<XS&D1A	YA<YS&D1A;                                                 
			NOP			W R						XAYA  				XA<XS&D1B	YA<YS&D1B; //Write : X 0xFFFE Y 0xEFFE Read : X 0xFFFE Y 0xEFFE                                                 
			NOP			W R						XAYA  				XA<XS&D1C	YA<YS&D1C; //Write : X 0xFFFC Y 0xEFFC Read : X 0xFFFC Y 0xEFFC                                                 
			NOP			W R						XAYA  				XA<XS&D1D	YA<YS&D1D; //Write : X 0xFFF8 Y 0xEFF8 Read : X 0xFFF8 Y 0xEFF8                                                 
			NOP			W R						XAYA  				XA<XS&D1E	YA<YS&D1E; //Write : X 0xFFF0 Y 0xEFF0 Read : X 0xFFF0 Y 0xEFF0                                                 
			NOP			W R						XAYA  				XA<XS&D1F	YA<YS&D1F; //Write : X 0xFFE0 Y 0xEFE0 Read : X 0xFFE0 Y 0xEFE0                                                 
			NOP			W R						XAYA  				XA<XS&D1G	YA<YS&D1G; //Write : X 0xFFC0 Y 0xEFC0 Read : X 0xFFC0 Y 0xEFC0                                                 
			NOP			W R						XAYA  				XA<XS&D1H	YA<YS&D1H; //Write : X 0xFF80 Y 0xEF80 Read : X 0xFF80 Y 0xEF80                                                 
			NOP			W R						XAYA  				XA<XS&D2A	YA<YS&D2A; //Write : X 0xFF00 Y 0xEF00 Read : X 0xFF00 Y 0xEF00                                                 
			NOP			W R						XAYA  				XA<XS&D2B	YA<YS&D2B; //Write : X 0xFE00 Y 0xEE00 Read : X 0xFE00 Y 0xEE00                                                 
			NOP			W R						XAYA  				XA<XS&D2C	YA<YS&D2C; //Write : X 0xFC00 Y 0xEC00 Read : X 0xFC00 Y 0xEC00                                                 
			NOP			W R						XAYA  				XA<XS&D2D	YA<YS&D2D; //Write : X 0xF800 Y 0xE800 Read : X 0xF800 Y 0xE800                                                 
			NOP			W R						XAYA  				XA<XS&D3	YA<YS&D3 ; //Write : X 0xF000 Y 0xE000 Read : X 0xF000 Y 0xE000                                                 
			NOP			W R						XAYA  				XA<XS&D4	YA<YS&D4 ; //Write : X 0xE000 Y 0xE000 Read : X 0xE000 Y 0xE000                                                 
			NOP			W R						XAYA  									 ; //Write : X 0xC000 Y 0xC000 Read : X 0xC000 Y 0xC000                                                 
			NOP         							                                    ;
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
		
			START XA_ALU_AANDB_02;
			NOP         							                                    ;
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
			NOP		   XC<0xFFFF	YC<0xEFFF													;                                                                  
			NOP		   XS<0xFFFF	YS<0xEFFF													;                                                                  
			NOP		   XK<0xFFFF	YK<0xEFFF													;                                                                  
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
			NOP														XA<XK&D1A	YA<YK&D1A;                                                 
			NOP			W R						XAYA  				XA<XK&D1B	YA<YK&D1B; //Write : X 0xFFFE Y 0xEFFE Read : X 0xFFFE Y 0xEFFE                                                 
			NOP			W R						XAYA  				XA<XK&D1C	YA<YK&D1C; //Write : X 0xFFFC Y 0xEFFC Read : X 0xFFFC Y 0xEFFC                                                 
			NOP			W R						XAYA  				XA<XK&D1D	YA<YK&D1D; //Write : X 0xFFF8 Y 0xEFF8 Read : X 0xFFF8 Y 0xEFF8                                                 
			NOP			W R						XAYA  				XA<XK&D1E	YA<YK&D1E; //Write : X 0xFFF0 Y 0xEFF0 Read : X 0xFFF0 Y 0xEFF0                                                 
			NOP			W R						XAYA  				XA<XK&D1F	YA<YK&D1F; //Write : X 0xFFE0 Y 0xEFE0 Read : X 0xFFE0 Y 0xEFE0                                                 
			NOP			W R						XAYA  				XA<XK&D1G	YA<YK&D1G; //Write : X 0xFFC0 Y 0xEFC0 Read : X 0xFFC0 Y 0xEFC0                                                 
			NOP			W R						XAYA  				XA<XK&D1H	YA<YK&D1H; //Write : X 0xFF80 Y 0xEF80 Read : X 0xFF80 Y 0xEF80                                                 
			NOP			W R						XAYA  				XA<XK&D2A	YA<YK&D2A; //Write : X 0xFF00 Y 0xEF00 Read : X 0xFF00 Y 0xEF00                                                 
			NOP			W R						XAYA  				XA<XK&D2B	YA<YK&D2B; //Write : X 0xFE00 Y 0xEE00 Read : X 0xFE00 Y 0xEE00                                                 
			NOP			W R						XAYA  				XA<XK&D2C	YA<YK&D2C; //Write : X 0xFC00 Y 0xEC00 Read : X 0xFC00 Y 0xEC00                                                 
			NOP			W R						XAYA  				XA<XK&D2D	YA<YK&D2D; //Write : X 0xF800 Y 0xE800 Read : X 0xF800 Y 0xE800                                                 
			NOP			W R						XAYA  				XA<XK&D3	YA<YK&D3 ; //Write : X 0xF000 Y 0xE000 Read : X 0xF000 Y 0xE000                                                 
			NOP			W R						XAYA  				XA<XK&D4	YA<YK&D4 ; //Write : X 0xE000 Y 0xE000 Read : X 0xE000 Y 0xE000                                                 
			NOP			W R						XAYA  									 ; //Write : X 0xC000 Y 0xC000 Read : X 0xC000 Y 0xC000                                                 
			NOP         							                                    ;
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
		
			START XA_ALU_AANDB_03;
			NOP         							                                    ;
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
			NOP														XA<XA&D1A	YA<YA&D1A;                                                 
			NOP			W R						XAYA  				XA<XA&D1B	YA<YA&D1B; //Write : X 0xDFFE Y 0xCFFE Read : X 0xDFFE Y 0xCFFE                                                 
			NOP			W R						XAYA  				XA<XA&D1C	YA<YA&D1C; //Write : X 0xDFFC Y 0xCFFC Read : X 0xDFFC Y 0xCFFC                                                 
			NOP			W R						XAYA  				XA<XA&D1D	YA<YA&D1D; //Write : X 0xDFF8 Y 0xCFF8 Read : X 0xDFF8 Y 0xCFF8                                                 
			NOP			W R						XAYA  				XA<XA&D1E	YA<YA&D1E; //Write : X 0xDFF0 Y 0xCFF0 Read : X 0xDFF0 Y 0xCFF0                                                 
			NOP			W R						XAYA  				XA<XA&D1F	YA<YA&D1F; //Write : X 0xDFE0 Y 0xCFE0 Read : X 0xDFE0 Y 0xCFE0                                                  
			NOP			W R						XAYA  				XA<XA&D1G	YA<YA&D1G; //Write : X 0xDFC0 Y 0xCFC0 Read : X 0xDFC0 Y 0xCFC0                                                  
			NOP			W R						XAYA  				XA<XA&D1H	YA<YA&D1H; //Write : X 0xDF80 Y 0xCF80 Read : X 0xDF80 Y 0xCF80                                                  
			NOP			W R						XAYA  				XA<XA&D2A	YA<YA&D2A; //Write : X 0xDF00 Y 0xCF00 Read : X 0xDF00 Y 0xCF00                                                   
			NOP			W R						XAYA  				XA<XA&D2B	YA<YA&D2B; //Write : X 0xDE00 Y 0xCE00 Read : X 0xDE00 Y 0xCE00                                                  
			NOP			W R						XAYA  				XA<XA&D2C	YA<YA&D2C; //Write : X 0xDC00 Y 0xCC00 Read : X 0xDC00 Y 0xCC00                                                  
			NOP			W R						XAYA  				XA<XA&D2D	YA<YA&D2D; //Write : X 0xD800 Y 0xC800 Read : X 0xD800 Y 0xC800                                                  
			NOP			W R						XAYA  				XA<XA&D3	YA<YA&D3 ; //Write : X 0xD000 Y 0xC000 Read : X 0xD000 Y 0xC000                                                  
			NOP			W R						XAYA  				XA<XA&D4	YA<YA&D4 ; //Write : X 0xC000 Y 0xC000 Read : X 0xC000 Y 0xC000                                                  
			NOP			W R						XAYA  									 ; //Write : X 0xC000 Y 0xC000 Read : X 0xC000 Y 0xC000                                                  
			NOP         							                                    ;
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
		
			START XA_ALU_AANDB_04;
			NOP         							                                    ;
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
			NOP														XA<XD&D1A	YA<YD&D1A;                                                 
			NOP			W R						XAYA  				XA<XD&D1B	YA<YD&D1B; //Write : X 0xCFFE Y 0xBFFE Read : X 0xCFFE Y 0xBFFE                                                 
			NOP			W R						XAYA  				XA<XD&D1C	YA<YD&D1C; //Write : X 0xCFFC Y 0xBFFC Read : X 0xCFFC Y 0xBFFC                                                 
			NOP			W R						XAYA  				XA<XD&D1D	YA<YD&D1D; //Write : X 0xCFF8 Y 0xBFF8 Read : X 0xCFF8 Y 0xBFF8                                                 
			NOP			W R						XAYA  				XA<XD&D1E	YA<YD&D1E; //Write : X 0xCFF0 Y 0xBFF0 Read : X 0xCFF0 Y 0xBFF0                                                 
			NOP			W R						XAYA  				XA<XD&D1F	YA<YD&D1F; //Write : X 0xCFE0 Y 0xBFE0 Read : X 0xCFE0 Y 0xBFE0                                                 
			NOP			W R						XAYA  				XA<XD&D1G	YA<YD&D1G; //Write : X 0xCFC0 Y 0xBFC0 Read : X 0xCFC0 Y 0xBFC0                                                 
			NOP			W R						XAYA  				XA<XD&D1H	YA<YD&D1H; //Write : X 0xCF80 Y 0xBF80 Read : X 0xCF80 Y 0xBF80                                                 
			NOP			W R						XAYA  				XA<XD&D2A	YA<YD&D2A; //Write : X 0xCF00 Y 0xBF00 Read : X 0xCF00 Y 0xBF00                                                 
			NOP			W R						XAYA  				XA<XD&D2B	YA<YD&D2B; //Write : X 0xCE00 Y 0xBE00 Read : X 0xCE00 Y 0xBE00                                                 
			NOP			W R						XAYA  				XA<XD&D2C	YA<YD&D2C; //Write : X 0xCC00 Y 0xBC00 Read : X 0xCC00 Y 0xBC00                                                 
			NOP			W R						XAYA  				XA<XD&D2D	YA<YD&D2D; //Write : X 0xC800 Y 0xB800 Read : X 0xC800 Y 0xB800                                                 
			NOP			W R						XAYA  				XA<XD&D3	YA<YD&D3 ; //Write : X 0xC000 Y 0xB000 Read : X 0xC000 Y 0xB000                                                 
			NOP			W R						XAYA  				XA<XD&D4	YA<YD&D4 ; //Write : X 0xC000 Y 0xA000 Read : X 0xC000 Y 0xA000                                                 
			NOP			W R						XAYA  									 ; //Write : X 0xC000 Y 0x8000 Read : X 0xC000 Y 0x8000                                                 
			NOP         							                                    ;
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
			
			START XA_ALU_AANDB_05;
			NOP         							                                    ;
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
			NOP														XA<XE&D1A	YA<YE&D1A;                                                 
			NOP			W R						XAYA  				XA<XE&D1B	YA<YE&D1B; //Write : X 0xBFFE Y 0xAFFE Read : X 0xBFFE Y 0xAFFE                                                 
			NOP			W R						XAYA  				XA<XE&D1C	YA<YE&D1C; //Write : X 0xBFFC Y 0xAFFC Read : X 0xBFFC Y 0xAFFC                                                 
			NOP			W R						XAYA  				XA<XE&D1D	YA<YE&D1D; //Write : X 0xBFF8 Y 0xAFF8 Read : X 0xBFF8 Y 0xAFF8                                                 
			NOP			W R						XAYA  				XA<XE&D1E	YA<YE&D1E; //Write : X 0xBFF0 Y 0xAFF0 Read : X 0xBFF0 Y 0xAFF0                                                 
			NOP			W R						XAYA  				XA<XE&D1F	YA<YE&D1F; //Write : X 0xBFE0 Y 0xAFE0 Read : X 0xBFE0 Y 0xAFE0                                                 
			NOP			W R						XAYA  				XA<XE&D1G	YA<YE&D1G; //Write : X 0xBFC0 Y 0xAFC0 Read : X 0xBFC0 Y 0xAFC0                                                 
			NOP			W R						XAYA  				XA<XE&D1H	YA<YE&D1H; //Write : X 0xBF80 Y 0xAF80 Read : X 0xBF80 Y 0xAF80                                                 
			NOP			W R						XAYA  				XA<XE&D2A	YA<YE&D2A; //Write : X 0xBF00 Y 0xAF00 Read : X 0xBF00 Y 0xAF00                                                 
			NOP			W R						XAYA  				XA<XE&D2B	YA<YE&D2B; //Write : X 0xBE00 Y 0xAE00 Read : X 0xBE00 Y 0xAE00                                                 
			NOP			W R						XAYA  				XA<XE&D2C	YA<YE&D2C; //Write : X 0xBC00 Y 0xAC00 Read : X 0xBC00 Y 0xAC00                                                 
			NOP			W R						XAYA  				XA<XE&D2D	YA<YE&D2D; //Write : X 0xB800 Y 0xA800 Read : X 0xB800 Y 0xA800                                                 
			NOP			W R						XAYA  				XA<XE&D3	YA<YE&D3 ; //Write : X 0xB000 Y 0xA000 Read : X 0xB000 Y 0xA000                                                 
			NOP			W R						XAYA  				XA<XE&D4	YA<YE&D4 ; //Write : X 0xA000 Y 0xA000 Read : X 0xA000 Y 0xA000                                                 
			NOP			W R						XAYA  									 ; //Write : X 0x8000 Y 0x8000 Read : X 0x8000 Y 0x8000                                                 
			NOP         							                                    ;
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
		
			START XA_ALU_AANDB_06;
			NOP         							                                    ;
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
			NOP														XA<XF&D1A	YA<YF&D1A;                                                 
			NOP			W R						XAYA  				XA<XF&D1B	YA<YF&D1B; //Write : X 0xAFFE Y 0x9FFE Read : X 0xAFFE Y 0x9FFE                                                 
			NOP			W R						XAYA  				XA<XF&D1C	YA<YF&D1C; //Write : X 0xAFFC Y 0x9FFC Read : X 0xAFFC Y 0x9FFC                                                 
			NOP			W R						XAYA  				XA<XF&D1D	YA<YF&D1D; //Write : X 0xAFF8 Y 0x9FF8 Read : X 0xAFF8 Y 0x9FF8                                                 
			NOP			W R						XAYA  				XA<XF&D1E	YA<YF&D1E; //Write : X 0xAFF0 Y 0x9FF0 Read : X 0xAFF0 Y 0x9FF0                                                 
			NOP			W R						XAYA  				XA<XF&D1F	YA<YF&D1F; //Write : X 0xAFE0 Y 0x9FE0 Read : X 0xAFE0 Y 0x9FE0                                                 
			NOP			W R						XAYA  				XA<XF&D1G	YA<YF&D1G; //Write : X 0xAFC0 Y 0x9FC0 Read : X 0xAFC0 Y 0x9FC0                                                 
			NOP			W R						XAYA  				XA<XF&D1H	YA<YF&D1H; //Write : X 0xAF80 Y 0x9F80 Read : X 0xAF80 Y 0x9F80                                                 
			NOP			W R						XAYA  				XA<XF&D2A	YA<YF&D2A; //Write : X 0xAF00 Y 0x9F00 Read : X 0xAF00 Y 0x9F00                                                 
			NOP			W R						XAYA  				XA<XF&D2B	YA<YF&D2B; //Write : X 0xAE00 Y 0x9E00 Read : X 0xAE00 Y 0x9E00                                                 
			NOP			W R						XAYA  				XA<XF&D2C	YA<YF&D2C; //Write : X 0xAC00 Y 0x9C00 Read : X 0xAC00 Y 0x9C00                                                 
			NOP			W R						XAYA  				XA<XF&D2D	YA<YF&D2D; //Write : X 0xA800 Y 0x9800 Read : X 0xA800 Y 0x9800                                                 
			NOP			W R						XAYA  				XA<XF&D3	YA<YF&D3 ; //Write : X 0xA000 Y 0x9000 Read : X 0xA000 Y 0x9000                                                 
			NOP			W R						XAYA  				XA<XF&D4	YA<YF&D4 ; //Write : X 0xA000 Y 0x8000 Read : X 0xA000 Y 0x8000                                                 
			NOP			W R						XAYA  									 ; //Write : X 0x8000 Y 0x8000 Read : X 0x8000 Y 0x8000                                                 
			NOP         							                                    ;
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
			
			START XA_ALU_AANDB_07;
			NOP         							                                    ;
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
			NOP														XA<XB&D1A	YA<YB&D1A;                                                 
			NOP			W R						XAYA  				XA<XB&D1B	YA<YB&D1B; //Write : X 0x9FFE Y 0x8FFE Read : X 0x9FFE Y 0x8FFE                                                 
			NOP			W R						XAYA  				XA<XB&D1C	YA<YB&D1C; //Write : X 0x9FFC Y 0x8FFC Read : X 0x9FFC Y 0x8FFC                                                 
			NOP			W R						XAYA  				XA<XB&D1D	YA<YB&D1D; //Write : X 0x9FF8 Y 0x8FF8 Read : X 0x9FF8 Y 0x8FF8                                                 
			NOP			W R						XAYA  				XA<XB&D1E	YA<YB&D1E; //Write : X 0x9FF0 Y 0x8FF0 Read : X 0x9FF0 Y 0x8FF0                                                 
			NOP			W R						XAYA  				XA<XB&D1F	YA<YB&D1F; //Write : X 0x9FE0 Y 0x8FE0 Read : X 0x9FE0 Y 0x8FE0                                                 
			NOP			W R						XAYA  				XA<XB&D1G	YA<YB&D1G; //Write : X 0x9FC0 Y 0x8FC0 Read : X 0x9FC0 Y 0x8FC0                                                 
			NOP			W R						XAYA  				XA<XB&D1H	YA<YB&D1H; //Write : X 0x9F80 Y 0x8F80 Read : X 0x9F80 Y 0x8F80                                                 
			NOP			W R						XAYA  				XA<XB&D2A	YA<YB&D2A; //Write : X 0x9F00 Y 0x8F00 Read : X 0x9F00 Y 0x8F00                                                 
			NOP			W R						XAYA  				XA<XB&D2B	YA<YB&D2B; //Write : X 0x9E00 Y 0x8E00 Read : X 0x9E00 Y 0x8E00                                                 
			NOP			W R						XAYA  				XA<XB&D2C	YA<YB&D2C; //Write : X 0x9C00 Y 0x8C00 Read : X 0x9C00 Y 0x8C00                                                 
			NOP			W R						XAYA  				XA<XB&D2D	YA<YB&D2D; //Write : X 0x9800 Y 0x8800 Read : X 0x9800 Y 0x8800                                                 
			NOP			W R						XAYA  				XA<XB&D3	YA<YB&D3 ; //Write : X 0x9000 Y 0x8000 Read : X 0x9000 Y 0x8000                                                 
			NOP			W R						XAYA  				XA<XB&D4	YA<YB&D4 ; //Write : X 0x8000 Y 0x8000 Read : X 0x8000 Y 0x8000                                                 
			NOP			W R						XAYA  									 ; //Write : X 0x8000 Y 0x8000 Read : X 0x8000 Y 0x8000                                                 
			NOP         							                                    ;
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
						
			
																		
	
 