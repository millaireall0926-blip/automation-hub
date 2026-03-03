

;%IFE A .AND. B .AND.((C.OR.D).AND.E)

%INSERT Header/define.h
 
REGISTER
			XMAX=0xffffff;
			YMAX=0xffffff;
			ZMAX=0xff;
			XOS=0x0;
            YOS=0x0;
ENDREGISTER		
 
  
        START CONTACT_A;
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

 			NOP	 	   	 	 X<XC Y<YC            TP<0x0001 TP2<0x000A   XC<XC+1 YC<YC+1 ;  
            NOP  C19   C17	 X<XC Y<YC            TP<0x0002 TP2<0x000B   XC<XC+1 YC<YC+1 ;  
            NOP  C19   C17	 X<XC Y<YC            TP<0x0004 TP2<0x000C   XC<XC+1 YC<YC+1 ;  
            NOP  C19   C17	 X<XC Y<YC            TP<0x0008 TP2<0x000D	 XC<XC+1 YC<YC+1 ;  
            NOP  C19   C17	 X<XC Y<YC            TP<0x0010 TP2<0x00A0	 XC<XC+1 YC<YC+1 ;  
            NOP  C19   C17	 X<XC Y<YC            TP<0x0020 TP2<0x00B0	 XC<XC+1 YC<YC+1 ;  
            NOP  C19   C17	 X<XC Y<YC            TP<0x0040 TP2<0x00C0	 XC<XC+1 YC<YC+1 ;  
            NOP  C19   C17	 X<XC Y<YC            TP<0x0080 TP2<0x00D0	 XC<XC+1 YC<YC+1 ;  
            NOP  C19   C17	 X<XC Y<YC            TP<0x0100 TP2<0x0A00	 XC<XC+1 YC<YC+1 ;  
            NOP  C19   C17	 X<XC Y<YC            TP<0x0200 TP2<0x0B00	 XC<XC+1 YC<YC+1 ;  
            NOP  C19   C17	 X<XC Y<YC            TP<0x0400 TP2<0x0C00   XC<XC+1 YC<YC+1 ;  
            NOP  C19   C17	 X<XC Y<YC            TP<0x0800 TP2<0x0D00	 XC<XC+1 YC<YC+1 ;  
            NOP  C19   C17	 X<XC Y<YC            TP<0x1000 TP2<0xA000	 XC<XC+1 YC<YC+1 ;  
            NOP  C19   C17	 X<XC Y<YC            TP<0x2000 TP2<0xB000	 XC<XC+1 YC<YC+1 ;  
            NOP  C19   C17	 X<XC Y<YC            TP<0x4000 TP2<0xC000	 XC<XC+1 YC<YC+1 ;  
            NOP  C19   C17	 X<XC Y<YC            TP<0x8000 TP2<0xD000	 XC<XC+1 YC<YC+1 ;  
            NOP  C19   C17	 X<XC Y<YC             										 ;  
   		    NOP	  																         ;				 
            NOP	 																         ;
  

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


        START RUN_A;
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
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
    	STPS                        			 	 ;
END 
        START RUN_B;
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
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
    	STPS                        			 	 ;
END 
        START RUN_C;
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
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
            NOP          	  	   		 			 ;
    	STPS                        			 	 ;
END 

;%ENDC 