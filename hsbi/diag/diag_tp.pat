

;%IFE A .AND. B .AND.((C.OR.D).AND.E)

%INSERT Header/define.h
  
 REGISTER
			XMAX=0xffffff;
			YMAX=0xffffff;
			ZMAX=0xff;
			XOS=0x0;
			YOS=0x0;	
ENDREGISTER		


 			START TP_ALU_A_00;
			%INSERT DUMMY.asc
			NOP				TPH1<0x1111			TPH2<0xAAAA								;                                      
			NOP				TPH1A<0x2222		TPH2A<0xBBBB							;                                      
			NOP				TPH1B<0x3333		TPH2B<0xCCCC							;                                      
			NOP				TPH1C<0x4444		TPH2C<0xDDDD							;                                      
			NOP				TPH1D<0x5555		TPH2D<0xEEEE							;                                      
			NOP				D5<0x6666			D6<0xFFFF								;                                      
			NOP				 D7<0x7777			D8<0xA5A5								;                                     
			NOP																			;                                                 
			NOP																			;                                                 
			NOP						TP<TPH1		TP2<TPH2	                            ;                                           
			NOP			W R			TP<TPH1A	TP2<TPH2A								;                                                 
			NOP			W R			TP<TPH1B	TP2<TPH2B								;                                                 
			NOP			W R			TP<TPH1C	TP2<TPH2C								;                                                 
			NOP			W R			TP<TPH1D	TP2<TPH2D								;                                                 
			NOP			W R			TP<D5		TP2<D6									;                                                 
			NOP			W R			TP<D7		TP2<D8									;                                                 
			NOP			W R																;                                                 
			NOP																			;                                     
			NOP						TP<TPH1		TP2<TPH2								;                                                 
			NOP			W R			TP<TPH1A	TP2<TPH2A								;                                                 
			NOP			W R			TP<TPH1B	TP2<TPH2B								;                                                 
			NOP			W R			TP<TPH1C	TP2<TPH2C								;                                        -         
			NOP			W R			TP<TPH1D	TP2<TPH2D								;                                                 
			NOP			W R			TP<D5		TP2<D6									;                                                 
			NOP			W R			TP<D7		TP2<D8									;                                                 
			NOP			W R																;                                                 
			NOP																			;                                     
			NOP			 			TP<D5		TP2<D6									;                                                 
			NOP			W R			TP<D7		TP2<D8									;                                                 
			NOP			W R																;                                                 
	
			%INSERT DUMMY.asc
			STPS																		;
	END


			START TP_ALU_TPA_00;
			%INSERT DUMMY.asc

			NOP				TPH1<0x0001			TPH2<0x0002								;                                      
			NOP				TPH1A<0x0010		TPH2A<0x0020							;                                      
			NOP				TPH1B<0x0100		TPH2B<0x0200							;                                      
			NOP				TPH1C<0x1000		TPH2C<0x2000							;                                      
			NOP				TPH1D<0x0001		TPH2D<0x0002							;                                      
			NOP				D5<0x0010			D6<0x0020								;                                      
			NOP				D7<0x0100			D8<0x0200     							;                                 
			NOP																			;                                                 
			NOP																			;                                                 
			NOP						TP<TP+TPH1		TP2<TP2+TPH2						;                                                 
			NOP			W R			TP<TP+TPH1A		TP2<TP2+TPH2A						;                                                 
			NOP			W R			TP<TP+TPH1B		TP2<TP2+TPH2B						;                                                 
			NOP			W R			TP<TP+TPH1C		TP2<TP2+TPH2C						;                                                 
			NOP			W R			TP<TP+TPH1D		TP2<TP2+TPH2D						;                                                 
			NOP			W R			TP<TP+D5		TP2<TP2+D6							;                                                 
			NOP			W R			TP<TP+D7		TP2<TP2+D8							;                                                 
			NOP			W R			TP<0            TP2<0								;                                                 
			NOP						TP<TP+TPH1		TP2<TP2+TPH2						;                                                 
			NOP			W R			TP<TP+TPH1A		TP2<TP2+TPH2A						;                                                 
			NOP			W R			TP<TP+TPH1B		TP2<TP2+TPH2B						;                                                 
			NOP			W R			TP<TP+TPH1C		TP2<TP2+TPH2C						;                                                 
			NOP			W R			TP<TP+TPH1D		TP2<TP2+TPH2D						;                                                 
			NOP			W R			TP<TP+D5		TP2<TP2+D6							;                                                 
			NOP			W R			TP<TP+D7		TP2<TP2+D8							;                                                 
			NOP			W R																;                                                 
			NOP																			;                                     
			NOP			 			TP<TP+D5		TP2<TP2+D6							;                                                 
			NOP			W R			TP<TP+D7		TP2<TP2+D8							;                                                 
			NOP			W R																;                                                 

			%INSERT DUMMY.asc
    		STPS																		;
	END      	
			START TP_ALU_TMA_00;
			%INSERT DUMMY.asc
			NOP				TP<0xFFFF           TP2<0xFFFF								;                                                 
			NOP				TPH1<0x0001			TPH2<0x0002								;                                      
			NOP				TPH1A<0x0010		TPH2A<0x0020							;                                      
			NOP				TPH1B<0x0100		TPH2B<0x0200							;                                      
			NOP				TPH1C<0x1000		TPH2C<0x2000							;                                      
			NOP				TPH1D<0x0001		TPH2D<0x0002							;                                      
			NOP				D5<0x0010			D6<0x0020								;                                      
			NOP				D7<0x0100			D8<0x0200 								;                                     
			NOP																			;                                                 
			NOP																			;                                                 
			NOP																			;                                                 
			NOP						TP<TP-TPH1		TP2<TP2-TPH2						;                                                 
			NOP			W R			TP<TP-TPH1A		TP2<TP2-TPH2A						;                                                 
			NOP			W R			TP<TP-TPH1B		TP2<TP2-TPH2B						;                                                 
			NOP			W R			TP<TP-TPH1C		TP2<TP2-TPH2C						;                                                 
			NOP			W R			TP<TP-TPH1D		TP2<TP2-TPH2D						;                                                 
			NOP			W R			TP<TP-D5		TP2<TP2-D6							;                                                 
			NOP			W R			TP<TP-D7		TP2<TP2-D8							;                                                 
			NOP			W R			TP<0xffff       TP2<0xffff							;                                                 
			NOP																			;                                     
			NOP																			;                                     
			NOP						TP<TP-TPH1		TP2<TP2-TPH2						;                                                 
			NOP			W R			TP<TP-TPH1A		TP2<TP2-TPH2A						;                                                 
			NOP			W R			TP<TP-TPH1B		TP2<TP2-TPH2B						;                                                 
			NOP			W R			TP<TP-TPH1C		TP2<TP2-TPH2C						;                                                 
			NOP			W R			TP<TP-TPH1D		TP2<TP2-TPH2D						;                                                 
			NOP			W R			TP<TP-D5		TP2<TP2-D6							;                                                 
			NOP			W R			TP<TP-D7		TP2<TP2-D8							;                                                 
			NOP			W R																;                                                 
			NOP																			;                                     
			NOP			 			TP<0			TP2<0								;                                                 
			NOP			W R			TP<0xffff		TP2<0xffff							;                                                 
			NOP			W R																;                                                 
			%INSERT DUMMY.asc
  		STPS																		;
	END
			START TP_ALU_TANDA_00;
			%INSERT DUMMY.asc
			NOP				TP<0xFFFF           TP2<0xFFFF								;                                                 
			NOP				TPH1<0xFFFE			TPH2<0xFEFF								;                                      
			NOP				TPH1A<0xFFFD		TPH2A<0xFDFF							;                                      
			NOP				TPH1B<0xFFFB 		TPH2B<0xFBFF							;                                      
			NOP				TPH1C<0xFFF7		TPH2C<0xF7FF							;                                      
			NOP				TPH1D<0xFFEF		TPH2D<0xEFFF							;                                      
			NOP				D5<0xFFDF			D6<0xDFFF								;                                      
			NOP				D7<0xFFBF			D8<0xBFFF  								;                                    
			NOP																			;                                                 
			NOP																			;                                                 
			NOP						TP<TP&TPH1		TP2<TP2&TPH2						;                                                 
			NOP			W R			TP<TP&TPH1A		TP2<TP2&TPH2A						;                                                 
			NOP			W R			TP<TP&TPH1B		TP2<TP2&TPH2B						;                                                 
			NOP			W R			TP<TP&TPH1C		TP2<TP2&TPH2C						;                                                 
			NOP			W R			TP<TP&TPH1D		TP2<TP2&TPH2D						;                                                 
			NOP			W R			TP<TP&D5		TP2<TP2&D6							;                                                 
			NOP			W R			TP<TP&D7		TP2<TP2&D8							;                                                 
			NOP			W R			TP<0xFFFF       TP2<0xFFFF							;                                                 
			NOP						TP<TP&TPH1		TP2<TP2&TPH2						;                                                 
			NOP			W R			TP<TP&TPH1A		TP2<TP2&TPH2A						;                                                 
			NOP			W R			TP<TP&TPH1B		TP2<TP2&TPH2B						;                                                 
			NOP			W R			TP<TP&TPH1C		TP2<TP2&TPH2C						;                                                 
			NOP			W R			TP<TP&TPH1D		TP2<TP2&TPH2D						;                                                 
			NOP			W R			TP<TP&D5		TP2<TP2&D6							;                                                 
			NOP			W R			TP<TP&D7		TP2<TP2&D8							;                                                 
			NOP			W R			TP<0            TP2<0xffff							;                                     
			NOP			W R			TP<0xffff       TP2<0x0								;                                     
			NOP			W R																;                                     
			%INSERT DUMMY.asc
  		STPS																		    ;																				 
	END  	

			START TP_ALU_TORA_00;
			%INSERT DUMMY.asc
			NOP				TP<0x0000           TP2<0x0000								;                                                 
			NOP				TPH1<0x0001			TPH2<0x0002								;                                      
			NOP				TPH1A<0x0010		TPH2A<0x0020							;                                      
			NOP				TPH1B<0x0100		TPH2B<0x0200							;                                      
			NOP				TPH1C<0x1000		TPH2C<0x2000							;                                      
			NOP				TPH1D<0x0002		TPH2D<0x0004							;                                      
			NOP				D5<0x0020			D6<0x0040								;                                      
			NOP				D7<0x0200			D8<0x0400 								;                                       
			NOP																			;                                                 
			NOP																			;                                                 
			NOP						TP<TP|TPH1		TP2<TP2|TPH2						;                                                 
			NOP			W R			TP<TP|TPH1A		TP2<TP2|TPH2A						;                                                 
			NOP			W R			TP<TP|TPH1B		TP2<TP2|TPH2B						;                                                 
			NOP			W R			TP<TP|TPH1C		TP2<TP2|TPH2C						;                                                 
			NOP			W R			TP<TP|TPH1D		TP2<TP2|TPH2D						;                                                 
			NOP			W R			TP<TP|D5		TP2<TP2|D6							;                                                 
			NOP			W R			TP<TP|D7		TP2<TP2|D8							;                                                 
			NOP			W R			TP<0            TP2<0								;                                                 
			NOP																			;                                     
			NOP						TP<TP|TPH1		TP2<TP2|TPH2						;                                                 
			NOP			W R			TP<TP|TPH1A		TP2<TP2|TPH2A						;                                                 
			NOP			W R			TP<TP|TPH1B		TP2<TP2|TPH2B						;                                                 
			NOP			W R			TP<TP|TPH1C		TP2<TP2|TPH2C						;                                                 
			NOP			W R			TP<TP|TPH1D		TP2<TP2|TPH2D						;                                                 
			NOP			W R			TP<TP|D5		TP2<TP2|D6							;                                                 
			NOP			W R			TP<TP|D7		TP2<TP2|D8							;                                                 
			NOP			W R																;                                                 
			NOP																			;                                     
			NOP			 			TP<0			TP2<0xffff							;                                                 
			NOP			W R			TP<0xffff		TP2<0								;                                                 
			NOP			W R																;                                                 
			%INSERT DUMMY.asc
    		STPS																		;
	END

			START TP_ALU_TMULA_00;
			%INSERT DUMMY.asc
			NOP				TP<0x0001           TP2<0x1001								;                                                 
			NOP				TPH1<0x0002			TPH2<0x0002								;                                      
			NOP				TPH1A<0x0004		TPH2A<0x0004							;                                      
			NOP				TPH1B<0x0008		TPH2B<0x0008							;                                      
			NOP				TPH1C<0x0002		TPH2C<0x0002							;                                      
			NOP				TPH1D<0x0004		TPH2D<0x0004							;                                      
			NOP				D5<0x0008			D6<0x0008								;                                      
			NOP				D7<0x0002			D8<0x0002	 							;                                       
			NOP																			;                                                 
			NOP																			;                                                 
			NOP						TP<TP*2	TP2<TP2*2									;                                                 
			NOP			W R			TP<TP*2	TP2<TP2*2									;                                                 
			NOP			W R			TP<TP*2	TP2<TP2*2									;                                                 
			NOP			W R			TP<TP*2	TP2<TP2*2									;                                                 
			NOP			W R			TP<TP*2	TP2<TP2*2									;                                                 
			NOP			W R			TP<TP*2	TP2<TP2*2									;                                                 
			NOP			W R			TP<TP*2	TP2<TP2*2									;                                                 
			NOP			W R			TP<TP*2	TP2<TP2*2									;                                                 
			NOP			W R			TP<TP*2	TP2<TP2*2									;                                                 
			NOP			W R			TP<TP*2	TP2<TP2*2									;                                                 
			NOP			W R			TP<TP*2	TP2<TP2*2									;                                                 
			NOP			W R			TP<TP*2	TP2<TP2*2									;                                                 
			NOP			W R			TP<TP*2	TP2<TP2*2									;                                                 
			NOP			W R			TP<TP*2	TP2<TP2*2									;                                                 
			NOP			W R			TP<TP*2	TP2<TP2*2									;                                                 
			NOP			W R			TP<TP*2	TP2<TP2*2									;                                                 
			NOP			W R																;                                                 
			%INSERT DUMMY.asc
    		STPS																		;
	END		
			START TP_ALU_TDA_00;
			%INSERT DUMMY.asc
			NOP				TP<0xFFFF           TP2<0x8000								;                                                 
			NOP				TPH1<0x0002			TPH2<0x0002								;                                      
			NOP				TPH1A<0x0004		TPH2A<0x0004							;                                      
			NOP				TPH1B<0x0008		TPH2B<0x0008							;                                      
			NOP				TPH1C<0x0002		TPH2C<0x0002							;                                      
			NOP				TPH1D<0x0004		TPH2D<0x0004							;                                      
			NOP				D5<0x0008			D6<0x0008								;                                      
			NOP				D7<0x0002			D8<0x0002								;                                      
			NOP																			;                                                 
			NOP																			;                                                 
			NOP						TP<TP/2	TP2<TP2/2									;                                                 
			NOP			W R			TP<TP/2	TP2<TP2/2									;                                                 
			NOP			W R			TP<TP/2	TP2<TP2/2									;                                                 
			NOP			W R			TP<TP/2	TP2<TP2/2									;                                                 
			NOP			W R			TP<TP/2	TP2<TP2/2									;                                                 
			NOP			W R			TP<TP/2	TP2<TP2/2									;                                                 
			NOP			W R			TP<TP/2	TP2<TP2/2									;                                                 
			NOP			W R			TP<TP/2	TP2<TP2/2									;                                                 
			NOP			W R			TP<TP/2	TP2<TP2/2									;                                                 
			NOP			W R			TP<TP/2	TP2<TP2/2									;                                                 
			NOP			W R			TP<TP/2	TP2<TP2/2									;                                                 
			NOP			W R			TP<TP/2	TP2<TP2/2									;                                                 
			NOP			W R			TP<TP/2	TP2<TP2/2									;                                                 
			NOP			W R			TP<TP/2	TP2<TP2/2									;                                                 
			NOP			W R			TP<TP/2	TP2<TP2/2									;                                                 
			NOP			W R			TP<TP/2	TP2<TP2/2									;                                                 
			NOP			W R																;                                                 
			%INSERT DUMMY.asc
    		STPS																		;
	END		
			START TP_ALU_Imm_00;
			%INSERT DUMMY.asc
			NOP				TPH1<0x1111													;                                      
			NOP				TPH1A<0x2222												;                                      
			NOP				TPH1B<0x3333												;                                      
			NOP				TPH1C<0x4444												;                                      
			NOP				TPH1D<0x5555												;                                      
			NOP				D5<0x6666													;                                      
			NOP				D7<0x7777   												;                                   
			NOP																			;                                                 
			NOP																			;                                                 
			NOP						TP<0x1		TP2<0x8001								;                                                 
			NOP			W R			TP<0x2		TP2<0x8002								;                                                 
			NOP			W R			TP<0x4		TP2<0x8004								;                                                 
			NOP			W R			TP<0x8		TP2<0x8008								;                                                 
			NOP			W R			TP<0x10		TP2<0x8010								;                                                 
			NOP			W R			TP<0x20		TP2<0x8020								;                                                 
			NOP			W R			TP<0x40     TP2<0x8040                              ;
			NOP			W R																;                                                 
			NOP																			;                                     
			NOP						TP<0x1		TP2<0x8001								;                                                 
			NOP			W R			TP<0x2		TP2<0x8002								;                                                 
			NOP			W R			TP<0x4		TP2<0x8004								;                                                 
			NOP			W R			TP<0x8		TP2<0x8008								;                                                 
			NOP			W R			TP<0x10		TP2<0x8010								;                                                 
			NOP			W R			TP<0x20		TP2<0x8020								;                                                 
			NOP			W R			TP<0x40     TP2<0x8040                              ;
			NOP			W R																;                                                 
			NOP																			;                                     
			NOP						TP<0x1111		TP2<0x3333							;                                                 
			NOP			W R			 													;                                                 
			NOP																			;                                     
			NOP						TP<0x2222		TP2<0x4444							;                                                 
			NOP			W R			 													;                                                 
			%INSERT DUMMY.asc
    		STPS																		;
	END		
	
			START TP_ALU_TPX_00;
			%INSERT DUMMY.asc
			NOP				TPH1<0x1111													;                                      
			NOP				TPH1A<0x2222												;                                      
			NOP				TPH1B<0x3333												;                                      
			NOP				TPH1C<0x4444												;                                      
			NOP				TPH1D<0x5555												;                                      
			NOP				D5<0x6666													;                                      
			NOP				D7<0x7777													;                                      
			NOP																			;                                                 
			NOP																			;                                                 
			NOP																			;                                                 
			NOP			W R			TPX1<0x1001		TPX2<0x6001							;                                                 
			NOP			W R			TPX1<0x1002		TPX2<0x6002							;                                                 
			NOP			W R			TPX1<0x1004		TPX2<0x6004							;                                                 
			NOP			W R			TPX1<0x1008		TPX2<0x6008							;                                                 
			NOP			W R			TPX1<0x1010		TPX2<0x6010							;                                                 
			NOP			W R			TPX1<0x1020   	TPX2<0x6020                         ;
			NOP			W R			TPX1<0x1040		TPX2<0x6040							;                                                 
			NOP			W R			TPX1<0x1080		TPX2<0x6080							;                                                 
			NOP																			;                                     
			NOP			W R			TPX1<0x2001		TPX2<0x7001							;                                                 
			NOP			W R			TPX1<0x2002		TPX2<0x7002							;                                                 
			NOP			W R			TPX1<0x2004		TPX2<0x7004							;                                                 
			NOP			W R			TPX1<0x2008		TPX2<0x7008							;                                                 
			NOP			W R			TPX1<0x2010		TPX2<0x7010							;                                                 
			NOP			W R			TPX1<0x2020   	TPX2<0x7020                         ;
			NOP			W R			TPX1<0x2040		TPX2<0x7040							;                                                 
			NOP			W R			TPX1<0x2080		TPX2<0x7080							;                                                 
			%INSERT DUMMY.asc
    		STPS																		;
	END			
			START TP_ALU_TPH_00;
			%INSERT DUMMY.asc
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
			NOP			W R			TPH1<TP		TPH2<TP2 								;                                        
			NOP			W R			TP<TP+1		TP2<TP2-1								;                                                 
			NOP			W R			TP<TP+1		TP2<TP2-1								;                                                 
			NOP			W R			TP<TPH1  	TP2<TPH2								;                                                 
			NOP			W R			TP<TP+1     TP2<TP2-1                               ;
			NOP			W R			TP<TP+1     TP2<TP2-1                               ;
			NOP			W R			TP<0x0 		TP2<0x8000								;                                                 
			NOP																			;                                     
			NOP						TP<TP+1		TP2<TP2-1								;                                                 
			NOP			W R			TP<TP+1		TP2<TP2-1								;                                                 
			NOP			W R			TPH1<TP		TPH2<TP2 								;                                        
			NOP			W R			TP<TP+1		TP2<TP2-1								;                                                 
			NOP			W R			TP<TP+1		TP2<TP2-1								;                                                 
			NOP			W R			TP<TPH1  	TP2<TPH2								;                                                 
			NOP			W R			TP<TP+1     TP2<TP2-1                               ;
			NOP			W R			TP<TP+1     TP2<TP2-1                               ;
			NOP			W R			 													;                                                 
			%INSERT DUMMY.asc
    		STPS																		;
	END
			START TP_ALU_INV_00;
			%INSERT DUMMY.asc
			NOP						TP<0x0 TP2<0x8000									;                                      
			NOP																			;                                      
			NOP				 TPH1<0x1111	 TPH2<0xAAAA								;                                      
			NOP				TPH1A<0x2222	TPH2A<0xBBBB								;                                      
			NOP				TPH1B<0x3333	TPH2B<0xCCCC								;                                      
			NOP				TPH1C<0x4444	TPH2C<0xDDDD								;                                      
			NOP				TPH1D<0x5555	TPH2D<0xEEEE								;                                      
			NOP				   D5<0x6666  	   D6<0x8888								;                                      
			NOP				   D7<0x7777	   D8<0x9999  								;                                    
			NOP																			;                                      
			NOP																			;                                      
			NOP																			;                                                 
			NOP																			;                                                 
			NOP						TP<TP+1		TP2<TP2-1								;                                                 
			NOP			W R			TP<TP+1		TP2<TP2-1								;                                                 
			NOP			W R			TP</TP1     TP2</TP2								;                                                 
			NOP			W R			TP</TPH1	TP2</TPH2								;                                                 
			NOP			W R			TP</TPH1A	TP2</TPH2A								;                                                 
			NOP			W R			TP</TPH1B	TP2</TPH2B								;                                                 
			NOP			W R			TP</TPH1C	TP2</TPH2C								;                                                 
			NOP			W R			TP</TPH1D	TP2</TPH2D								;                                                 
			NOP			W R			TP</D5		TP2</D6									;                                                 
			NOP			W R			TP</D7		TP2</D8									;                                                 
			NOP			W R		    TP<0x0 		TP2<0x8000								;                                                 
			NOP						TP<TP+1		TP2<TP2-1								;                                                 
			NOP			W R			TP<TP+1		TP2<TP2-1								;                                                 
			NOP			W R			TP</TP1     TP2</TP2								;                                                 
			NOP			W R			TP</TPH1	TP2</TPH2								;                                                 
			NOP			W R			TP</TPH1A	TP2</TPH2A								;                                                 
			NOP			W R			TP</TPH1B	TP2</TPH2B								;                                                 
			NOP			W R			TP</TPH1C	TP2</TPH2C								;                                                 
			NOP			W R			 													;                                                 
			%INSERT DUMMY.asc
    		STPS																		;
	END			
			START TP_ALU_D7D8_00;
			%INSERT DUMMY.asc
			NOP					TP<0x0        TP2<0x8000								;                                      
			NOP				 TPH1<0x1111	 TPH2<0xAAAA								;                                      
			NOP				TPH1A<0x2222	TPH2A<0xBBBB								;                                      
			NOP				TPH1B<0x3333	TPH2B<0xCCCC								;                                      
			NOP				TPH1C<0x4444	TPH2C<0xDDDD								;                                      
			NOP				TPH1D<0x5555	TPH2D<0xEEEE								;                                      
			NOP				   D5<0x6666  	   D6<0x8888								;                                      
			NOP																			;                                      
			NOP																			;                                                 
			NOP								D7<0  			        D8<0  				;                                                 
			NOP						TP<D7   D7<TPH1  		TP2<D8	D8<TP2  			;                                                 
			NOP			W R			TP<D7   D7<TPH1 		TP2<D8	D8<TPH2 			;                                                 
			NOP			W R			TP<D7   D7<TPH1A  		TP2<D8	D8<TPH2A  			;                                                 
			NOP			W R			TP<D7   D7<TPH1B  		TP2<D8	D8<TPH2B  			;                                                 
			NOP			W R			TP<D7   D7<TPH1C  		TP2<D8	D8<TPH2C  			;                                                 
			NOP			W R			TP<D7   D7<TPH1D 		TP2<D8	D8<TPH2D 			;                                                 
 			NOP			W R			TP<D7   D7<D5  			TP2<D8	D8<D6  				;                                                 
 			NOP			W R			TP<D7   D7<D7+1  		TP2<D8	D8<D8+1  			;                                                 
 			NOP			W R			TP<D7   D7<D7-1  		TP2<D8	D8<D8-1  			;                                                 
 			NOP			W R			TP<D7   D7<D7/2  		TP2<D8	D8<D8/2  			;                                                 
 			NOP			W R			TP<D7   				TP2<D8						;                                                 
 			NOP			W R			 													;                                                 
 			NOP			 				    TP<0x0          TP2<0x8000					;                                                 
 			NOP								D7<0  			        D8<0  				;                                                 
			NOP						TP<D7   D7<TPH1  		TP2<D8	D8<TP2  			;                                                 
			NOP			W R			TP<D7   D7<TPH1 		TP2<D8	D8<TPH2 			;                                                 
			NOP			W R			TP<D7   D7<TPH1A  		TP2<D8	D8<TPH2A  			;                                                 
			NOP			W R			TP<D7   D7<TPH1B  		TP2<D8	D8<TPH2B  			;                                                 
			NOP			W R			TP<D7   D7<TPH1C  		TP2<D8	D8<TPH2C  			;                                                 
			NOP			W R			TP<D7   D7<TPH1D 		TP2<D8	D8<TPH2D 			;                                                 
 			%INSERT DUMMY.asc
   		STPS																		;
	END	
			START TP_ALU_D7D8_01;
			%INSERT DUMMY.asc
			NOP					TP<0x0        TP2<0x8000								;                                      
			NOP				 TPH1<0x0001	 TPH2<0x0010								;                                      
			NOP				TPH1A<0x0002	TPH2A<0x0020								;                                      
			NOP				TPH1B<0x0003	TPH2B<0x0030								;                                      
			NOP				TPH1C<0x0004	TPH2C<0x0040								;                                      
			NOP				TPH1D<0x0005	TPH2D<0x0050								;                                      
			NOP				   D5<0x0006  	   D6<0x0060								;                                      
			NOP																			;                                      
			NOP																			;                                                 
			NOP			     		        D7<D7+TP1  		 		D8<D8+TP2  			;                                                 
 			NOP			     		TP<D7   D7<D7+TPH1  	TP2<D8	D8<D8+TPH2 			;                                                 
 			NOP			W R    		TP<D7   D7<D7+TPH1A		TP2<D8	D8<D8+TPH2A 		;                                                 
 			NOP			W R			TP<D7   D7<D7+TPH1B 	TP2<D8	D8<D8+TPH2B			;                                                 
 			NOP			W R			TP<D7   D7<D7+TPH1C		TP2<D8	D8<D8+TPH2C 		;                                                 
 			NOP			W R			TP<D7   D7<D7+D5  		TP2<D8	D8<D8+D6  			;                                                 
 			NOP			W R			TP<D7   D7<D7+TPH1		TP2<D8	D8<D8+TPH2			;                                                 
 			NOP			W R			TP<D7   D7<D7+TPH1  	TP2<D8	D8<D8+TPH2 			;                                                 
 			NOP			W R			TP<D7   D7<D7+TPH1		TP2<D8	D8<D8+TPH2	 		;                                                 
 			NOP			W R			TP<D7   D7<D7-TP1  		TP2<D8	D8<D8-TP2 			;                                                 
 			NOP			W R			TP<D7   D7<D7-TPH1  	TP2<D8	D8<D8-TPH2 			;                                                 
 			NOP			W R			TP<D7   D7<D7-TPH1A		TP2<D8	D8<D8-TPH2A 		;                                                 
 			NOP			W R			TP<D7   D7<D7-TPH1B 	TP2<D8	D8<D8-TPH2B			;                                                 
 			NOP			W R			TP<D7   D7<D7-TPH1C		TP2<D8	D8<D8-TPH2C 		;                                                 
 			NOP			W R			TP<D7   D7<D7-D5  		TP2<D8	D8<D8-D6  			;                                                 
 			NOP			W R			TP<D7   D7<D7-TPH1		TP2<D8	D8<D8-TPH2			;                                                 
 			NOP			W R			TP<D7   D7<D7-TPH1  	TP2<D8	D8<D8-TPH2 			;                                                 
 			NOP			W R			TP<D7    		;                                                 
			%INSERT DUMMY.asc
    		STPS																		;
	END	   		     	
		START TP_ALU_D7D8_02;
			%INSERT DUMMY.asc
			NOP					TP<0x0        TP2<0x8000								;                                      
			NOP				 TPH1<0x0001	 TPH2<0x0010								;                                      
			NOP				TPH1A<0x0002	TPH2A<0x0020								;                                      
			NOP				TPH1B<0x0003	TPH2B<0x0030								;                                      
			NOP				TPH1C<0x0004	TPH2C<0x0040								;                                      
			NOP				TPH1D<0x0005	TPH2D<0x0050								;                                      
			NOP				   D5<0x0006  	   D6<0x0060								;                                      
			NOP																			;                                      
			NOP																			;                                                 
			NOP			     		        D7<D7+TP1  		 		D8<D8+TP2  			;                                                 
 			NOP			     		TP<D7   D7<D7+TPH1  	TP2<D8	D8<D8+TPH2 			;                                                 
 			NOP			    		TP<D7   D7<D7+TPH1A		TP2<D8	D8<D8+TPH2A 		;                                                 
 			NOP			 			TP<D7   D7<D7+TPH1B 	TP2<D8	D8<D8+TPH2B			;                                                 
 			NOP			W R			TP<D7   D7<D7+TPH1C		TP2<D8	D8<D8+TPH2C 		;                                                 
 			NOP			W R			TP<D7   D7<D7+D5  		TP2<D8	D8<D8+D6  			;                                                 
 			NOP			W R			TP<D7   D7<D7+TPH1		TP2<D8	D8<D8+TPH2			;                                                 
 			NOP			W R			TP<D7   D7<D7+TPH1  	TP2<D8	D8<D8+TPH2 			;                                                 
 			NOP			W R			TP<D7   D7<D7+TPH1		TP2<D8	D8<D8+TPH2	 		;                                                 
 			NOP			W R			TP<D7   D7<D7-TP1  		TP2<D8	D8<D8-TP2 			;                                                 
 			NOP			W R			TP<D7   D7<D7-TPH1  	TP2<D8	D8<D8-TPH2 			;                                                 
 			NOP			W R			TP<D7   D7<D7-TPH1A		TP2<D8	D8<D8-TPH2A 		;                                                 
 			NOP			W R			TP<D7   D7<D7-TPH1B 	TP2<D8	D8<D8-TPH2B			;                                                 
 			NOP			W R			TP<D7   D7<D7-TPH1C		TP2<D8	D8<D8-TPH2C 		;                                                 
 			NOP			W R			TP<D7   D7<D7-D5  		TP2<D8	D8<D8-D6  			;                                                 
 			NOP			W R			TP<D7   D7<D7-TPH1		TP2<D8	D8<D8-TPH2			;                                                 
 			NOP			W R			TP<D7   D7<D7-TPH1  	TP2<D8	D8<D8-TPH2 			;                                                 
 			NOP			W R			TP<D7   D7<D7-TPH1		TP2<D8	D8<D8-TPH2			;                                                 
 			NOP			W R			TP<D7   				TP2<D8						;                                                 
 			NOP			W R			 													;                                                 
			%INSERT DUMMY.asc
    		STPS																		;
	END	   		     	

			START TP_ALU_TPPImm_00;
			%INSERT DUMMY.asc
			NOP						TP<0x0 TP2<0x0000									;                                      
			NOP																			;                                      
			NOP																			;                                      
			NOP																			;                                      
			NOP																			;                                      
			NOP																			;                                      
			NOP																			;                                      
			NOP																			;                                                 
			NOP																			;                                                 
			NOP						TP<TP+0x0001 TP2<TP2+0x1000				            ;                               
			NOP			W R			TP<TP+0x0002 TP2<TP2+0x2000				            ;                               
			NOP			W R			TP<TP+0x0004 TP2<TP2+0x4000				            ;                      
			NOP			W R			TP<TP+0x0008 TP2<TP2+0x8000				            ;                               
			NOP			W R			TP<TP+0x0010 TP2<TP2+0x0100				            ;                               
			NOP			W R			TP<TP+0x0020 TP2<TP2+0x0200				            ;                               
			NOP			W R			TP<TP+0x0040 TP2<TP2+0x0400                         ;
			NOP			W R			TP<TP+0x0080 TP2<TP2+0x0800                         ;
			NOP			W R			TP<TP+0x0100 TP2<TP2+0x0010				            ;                                   
			NOP			W R			TP<TP+0x0200 TP2<TP2+0x0020				            ;                               
			NOP			W R			TP<TP+0x0400 TP2<TP2+0x0040				            ;                      
			NOP			W R			TP<TP+0x0800 TP2<TP2+0x0080				            ;                               
			NOP			W R			TP<TP+0x1000 TP2<TP2+0x0001				            ;                               
			NOP			W R			TP<TP+0x2000 TP2<TP2+0x0002				            ;                               
			NOP			W R			TP<TP+0x4000 TP2<TP2+0x0004                         ;
			NOP			W R			TP<TP+0x8000 TP2<TP2+0x0008                         ;
			NOP			W R			 										            ;                                           
			%INSERT DUMMY.asc
    		STPS															            ;
	END		     	


			START TP_ALU_TPMImm_00;
			%INSERT DUMMY.asc
			NOP						TP<0xFFFF TP2<0xFFFF								;                                      
			NOP																			;                                      
			NOP																			;                                      
			NOP																			;                                      
			NOP																			;                                      
			NOP																			;                                      
			NOP																			;                                      
			NOP																			;                                                 
			NOP																			;                                                 
			NOP						TP<TP-0x0001 TP2<TP2-0x1000				            ;                               
			NOP			W R			TP<TP-0x0002 TP2<TP2-0x2000				            ;                               
			NOP			W R			TP<TP-0x0004 TP2<TP2-0x4000				            ;                      
			NOP			W R			TP<TP-0x0008 TP2<TP2-0x8000				            ;                               
			NOP			W R			TP<TP-0x0010 TP2<TP2-0x0100				            ;                               
			NOP			W R			TP<TP-0x0020 TP2<TP2-0x0200				            ;                               
			NOP			W R			TP<TP-0x0040 TP2<TP2-0x0400                         ;
			NOP			W R			TP<TP-0x0080 TP2<TP2-0x0800                         ;
			NOP			W R			TP<TP-0x0100 TP2<TP2-0x0010				            ;                                   
			NOP			W R			TP<TP-0x0200 TP2<TP2-0x0020				            ;                               
			NOP			W R			TP<TP-0x0400 TP2<TP2-0x0040				            ;                      
			NOP			W R			TP<TP-0x0800 TP2<TP2-0x0080				            ;                               
			NOP			W R			TP<TP-0x1000 TP2<TP2-0x0001				            ;                               
			NOP			W R			TP<TP-0x2000 TP2<TP2-0x0002				            ;                               
			NOP			W R			TP<TP-0x4000 TP2<TP2-0x0004                         ;
			NOP			W R			TP<TP-0x8000 TP2<TP2-0x0008                         ;
			NOP			W R			 										            ;                                           
			%INSERT DUMMY.asc
    		STPS															            ;
	END		     	
	START TP_Vector_00;
			%INSERT DUMMY.asc
			NOP						TP<0x0000 TP2<0x0000								;                                      
			NOP																			;                                      
			NOP																			;                                      
			NOP																			;                                      
			NOP																			;                                      		   																	                                       
			NOP																			;                                      		   																	                                       
			NOP																			;                                      		   																	                                       
			NOP																			;                                      		   																	                                       
			VMPA 0x00																	;                                      		   																	                                       
			MODE VM_ON 								    								;                                              
			NOP																			;                                                 
			NOP																			;                                                 
			NOP																			;                                                 
			NOP																			;                                                 
			NOP																			;                                                 
			NOP																			;                                                 
			NOP														 					;                                                 
			NOP						TP<TP+0x0001 TP2<TP2+0x1000		 	                ;                               
			NOP			W R			TP<TP+0x0002 TP2<TP2+0x2000		 	                       
			NOP			W R			TP<TP+0x0004 TP2<TP2+0x4000		                
			NOP			W R			TP<TP+0x0008 TP2<TP2+0x8000		 VM_INC                              
			NOP			W R			TP<TP+0x0010 TP2<TP2+0x0100		                    
			NOP			W R			TP<TP+0x0020 TP2<TP2+0x0200		                        
			NOP			W R			TP<TP+0x0040 TP2<TP2+0x0400              
			NOP			W R			TP<TP+0x0080 TP2<TP2+0x0800      VM_INC        
			NOP			W R			TP<TP+0x0100 TP2<TP2+0x0010				                       
			NOP			W R			TP<TP+0x0200 TP2<TP2+0x0020				                       
			NOP			W R			TP<TP+0x0400 TP2<TP2+0x0040				              
			NOP			W R			TP<TP+0x0800 TP2<TP2+0x0080		 VM_INC		                       
			NOP			W R			TP<TP+0x1000 TP2<TP2+0x0001				                   
			NOP			W R			TP<TP+0x2000 TP2<TP2+0x0002				            ;                               
			NOP			W R			TP<TP+0x4000 TP2<TP2+0x0004                         ;
			NOP			W R			TP<TP+0x8000 TP2<TP2+0x0008                         ;
			NOP			W R			 										            ;                                           
			NOP																			;                                                 
			NOP																			;                                                 
			NOP																			;                                                 
			MODE VM_OFF																	;                                                 
			NOP																			;                                                 
			NOP																			;                                                 
			%INSERT DUMMY.asc
    		STPS															            ;
	END		     	

			START TP_Vector_01;
			%INSERT DUMMY.asc
			NOP						TP<0x0000 TP2<0x0000								;                                      
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
			NOP																			;                                      		   																	                                       
			VMPA 0xA																	;                                      		   																	                                       
			MODE VM_ON 								    								;                                              
			NOP																			;                                                 
			NOP																			;                                                 
			NOP																			;                                                 
			NOP																			;                                                 
			NOP														 					;                                                 
			NOP						TP<TP+0x0001 TP2<TP2+0x1000		 	                ;                               
			NOP			W R			TP<TP+0x0002 TP2<TP2+0x2000		 	                       
			NOP			W R			TP<TP+0x0004 TP2<TP2+0x4000		                
			NOP			W R			TP<TP+0x0008 TP2<TP2+0x8000		 VM_INC                              
			NOP			W R			TP<TP+0x0010 TP2<TP2+0x0100		                    
			NOP			W R			TP<TP+0x0020 TP2<TP2+0x0200		                        
			NOP			W R			TP<TP+0x0040 TP2<TP2+0x0400              
			NOP			W R			TP<TP+0x0080 TP2<TP2+0x0800      VM_INC        
			NOP			W R			TP<TP+0x0100 TP2<TP2+0x0010				                       
			NOP			W R			TP<TP+0x0200 TP2<TP2+0x0020				                       
			NOP			W R			TP<TP+0x0400 TP2<TP2+0x0040				              
			NOP			W R			TP<TP+0x0800 TP2<TP2+0x0080		 VM_INC		                       
			NOP			W R			TP<TP+0x1000 TP2<TP2+0x0001				                   
			NOP			W R			TP<TP+0x2000 TP2<TP2+0x0002				            ;                               
			NOP			W R			TP<TP+0x4000 TP2<TP2+0x0004                         ;
			NOP			W R			TP<TP+0x8000 TP2<TP2+0x0008                         ;
			NOP			W R			 										            ;                                           
			NOP																			;                                                 
			NOP																			;                                                 
			NOP																			;                                                 
			NOP																			;                                                 
			MODE VM_OFF																	;                                                 
			NOP																			;                                                 
			NOP																			;                                                 
			%INSERT DUMMY.asc
    		STPS															            ;
	END	
     	

			START TP_Vector_02;
			%INSERT DUMMY.asc
			NOP						TP<0xFFFF TP2<0xFFFF								;                                      
			NOP																			;                                      
			NOP																			;                                      
			NOP																			;                                      		   																	                                       
			NOP																			;                                      		   																	                                       
			VMPA 0x0																	;                                      		   																	                                       
			MODE VM_ON 								    								;                                              
			NOP																			;                                                 
			NOP																			;                                                 
			NOP																			;                                                 
			NOP																			;                                                 
			NOP																			;                                                 
			NOP																			;                                                 
			NOP														 					;                                                 
			NOP						TP<TP+0x0001 TP2<TP2+0x1000		VM_INC                               
			NOP			W R			TP<TP+0x0002 TP2<TP2+0x2000		VM_INC      
			NOP			W R			TP<TP+0x0004 TP2<TP2+0x4000		VM_INC     
			NOP			W R			TP<TP+0x0008 TP2<TP2+0x8000		VM_INC                  
			NOP			W R			TP<TP+0x0010 TP2<TP2+0x0100		VM_INC     
			NOP			W R			TP<TP+0x0020 TP2<TP2+0x0200		VM_INC     
			NOP			W R			TP<TP+0x0040 TP2<TP2+0x0400     VM_INC
			NOP			W R			TP<TP+0x0080 TP2<TP2+0x0800     VM_INC     
			NOP			W R			TP<TP+0x0100 TP2<TP2+0x0010		VM_INC      
			NOP			W R			TP<TP+0x0200 TP2<TP2+0x0020		VM_INC      
			NOP			W R			TP<TP+0x0400 TP2<TP2+0x0040		VM_INC     
			NOP			W R			TP<TP+0x0800 TP2<TP2+0x0080		VM_INC             
			NOP			W R			TP<TP+0x1000 TP2<TP2+0x0001		VM_INC     
			NOP			W R			TP<TP+0x2000 TP2<TP2+0x0002		VM_INC                           
			NOP			W R			TP<TP+0x4000 TP2<TP2+0x0004     VM_INC     
			NOP			W R			TP<TP+0x8000 TP2<TP2+0x0008                         ;
			NOP			W R			 										            ;                                           
			NOP																			;                                                 
			NOP																			;                                                 
			NOP																			;                                                 
			MODE VM_OFF																	;                                                 
			NOP																			;                                                 
			NOP																			;                                                 
			%INSERT DUMMY.asc
    		STPS															            ;
	END		     	

			START TP_Vector_03;
			%INSERT DUMMY.asc
			NOP						TP<0xFFFF TP2<0xFFFF								;                                      
			NOP																			;                                      
			NOP																			;                                      
			NOP																			;                                      
			NOP																			;                                      		   																	                                       
			NOP																			;                                      		   																	                                       
			NOP																			;                                      		   																	                                       
			NOP																			;                                      		   																	                                       
			VMPA 0x6																	;                                      		   																	                                       
        	NOP																			;                                      		   																	                                                                         
			NOP																			;                                                 
			NOP																			;                                      
			NOP						TP<0x0 TP2<0x8000									;                                                 
			NOP			W R			TP<TP+1 TP2<TP2+1	         		                ;
			NOP			W R			TP<TP+1 TP2<TP2+1	         		           
			NOP			W R			TP<TP+1 TP2<TP2+1	         		   
			NOP			W R			 						                               
			MODE VM_ON																	;                                              
			NOP																			;                                                 
			NOP																			;                                                 
			NOP			W R			 VM_INC									 	     	                                
			NOP			W R			 						                   			;                               
			NOP			W R			 							                        ;
			NOP			W R			 							                        ;
			NOP																			;                                                 
			NOP																			;                                                 
			NOP																			;                                                 
			MODE VM_OFF																	;     
			NOP																			;                                                 
			NOP																			;                                                 
			NOP																			;                                                 
			NOP																			;                                                 
			NOP						TP<TP+1 TP2<TP2+1									;                                                 
			NOP			W R			TP<TP+1 TP2<TP2+1	         		                ;
			NOP			W R			TP<TP+1 TP2<TP2+1	         		           
			NOP			W R			TP<TP+1 TP2<TP2+1	         		   
			NOP			W R			 						                               
			NOP																			;                                                 
			VMPA 0x10																	;                                      		   																	                                       
        	MODE VM_ON																	;                                              
			NOP																			;                                                 
			NOP														 					;                                                 
			NOP														 					;                                                 
			NOP						TP<TP-0x0001 TP2<TP2-0x1000		VM_INC	        	;                               
			NOP			W R			TP<TP-0x0002 TP2<TP2-0x2000		            	    ;                               
			NOP			W R			TP<TP-0x0004 TP2<TP2-0x4000		VM_INC      		;                      
			NOP			W R			TP<TP-0x0008 TP2<TP2-0x8000		                    ;                               
			NOP			W R			TP<TP-0x0010 TP2<TP2-0x0100		        	   		;                               
			NOP																			;                                                 
			NOP																			;                                                 
			MODE VM_OFF	
			%INSERT DUMMY.asc
    		STPS															        
	END
	
			START TP_INFO_00;
			%INSERT DUMMY.asc
			NOP						TP<0xFFFF TP2<0xFFFF								                                      
			NOP																			                                      
			NOP																			                                      
			NOP																			                                      
			NOP																			                                      		   																	                                       
			NOP																			                                      		   																	                                       
			NOP																			                                      		   																	                                       
			NOP																			                                      		   																	                                       
			VMPA 0x6																	                                      		   																	                                       
        	NOP																			                                      		   																	                                                                         
			NOP																			                                                 
			NOP																			                                      
			NOP						TP<0x0 TP2<0x8000									                                                 
			NOP			W R			TP<TP+1 TP2<TP2+1	         		                 
			NOP			W R			TP<TP+1 TP2<TP2+1	         		           
			NOP			W R			TP<TP+1 TP2<TP2+1	         		   
			NOP			W R			 						                             
			MODE INFO_ON																                              
			NOP																			                             
			NOP																			                             
			NOP			W R			 INFO_INC									 	     	           
			NOP			W R			 						                   			           
			NOP			W R			 							                         
			NOP			W R			 							                         
			NOP																			                             
			NOP																			                             
			NOP																			                             
			MODE INFO_OFF																 
			NOP																			                             
			NOP																			                             
			NOP																			                             
			NOP																			                             
			NOP						TP<TP+1 TP2<TP2+1									                                             
			NOP			W R			TP<TP+1 TP2<TP2+1	         		                 
			NOP			W R			TP<TP+1 TP2<TP2+1	         		           
			NOP			W R			TP<TP+1 TP2<TP2+1	         		   
			NOP			W R			 						                             
			NOP																			                             
			VMPA 0x10																	                  		   																	                                       
        	MODE INFO_ON																                              
			NOP																			                             
			NOP														 					                             
			NOP														 					                             
			NOP						TP<TP-0x0001 TP2<TP2-0x1000		INFO_INC	                       
			NOP			W R			TP<TP-0x0002 TP2<TP2-0x2000		            	               
			NOP			W R			TP<TP-0x0004 TP2<TP2-0x4000		INFO_INC        	      
			NOP			W R			TP<TP-0x0008 TP2<TP2-0x8000		                               
			NOP			W R			TP<TP-0x0010 TP2<TP2-0x0100		        	   		           
			NOP																			                             
			NOP																			                             
			MODE INFO_OFF	
			%INSERT DUMMY.asc
    		STPS															        
	END    	
		
	
;%ENDC 