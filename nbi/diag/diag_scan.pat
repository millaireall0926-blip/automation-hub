
;%IFE A .AND. B .AND.((C.OR.D).AND.E)

%INSERT Header/define.h
  
REGISTER
			XMAX=0xffffff;
			YMAX=0xffffff;
			ZMAX=0xff;
			XOS=0x0;
			YOS=0x0;	
ENDREGISTER		


			START S_ALU_SA_00;
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

			NOP					SA<0x11													;                                      
			NOP					SB<0x22													;                                                 
			NOP					 														;                                                 
			NOP								SA<0			 							;    //기대값      // read된 값	                                             
			NOP			W R			S<SA	SA<0x44		     							;    //	0x0		//	0x44		                                             
			NOP			W R			S<SA	SA<0x10			 							;    //	0x44	//	0x10		                                             
			NOP			W R			S<SA	SA<S1A			 							;    // 0x10	//	0x33		                                             
			NOP			W R			S<SA	SA<SA+0x20		 							;    // 0x33	//	0x53		                                             
			NOP			W R			S<SA	SA<SA-0x10		 							;    // 0x53	//	0x43		                                             
			NOP			W R			S<SA	SA<SB+0x40		 							;    // 0x43	//	0x62		                                             
			NOP			W R			S<SA	SA<SB-0x30		 							;    // 0x62	//	0xf2		                                             
			NOP			W R			S<SA					 							;    // 0x32	//	0xf2		                                             

			NOP																			;                                     
			NOP								SA<0			 							;    //기대값      // read된 값	                                             
			NOP			W R			S<SA	SA<0x44		     							;    //	0x0		//	0x44		                                             
			NOP			W R			S<SA	SA<0x10			 							;    //	0x44	//	0x10		                                             
			NOP			W R			S<SA	SA<S1A			 							;    // 0x10	//	0x33		                                             
			NOP			W R			S<SA	SA<SA+0x20		 							;    // 0x33	//	0x53		                                             
			NOP			W R			S<SA	SA<SA-0x10		 							;    // 0x53	//	0x43		                                             
			NOP			W R			S<SA	SA<SB+0x40		 							;    // 0x43	//	0x62		                                             
			NOP			W R			S<SA	SA<SB-0x30		 							;    // 0x62	//	0xf2		                                             
			NOP			W R			S<SA					 							;    // 0x32	//	0xf2		                                             

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
 			START S_ALU_SB_00;
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

			NOP					SA<0x22													;                                      
			NOP					SB<0x11													;                                                 
			NOP					 														;                                                 
			NOP								SB<0			 							;    //기대값      // read된 값	                                             
			NOP			W R			S<SB	SB<0x44		     							;    //	0x0		//	0x44		                                             
			NOP			W R			S<SB	SB<0x10			 							;    //	0x44	//	0x10		                                             
			NOP			W R			S<SB	SB<S1A			 							;    // 0x10	//	0x33		                                             
			NOP			W R			S<SB	SB<SB+0x20		 							;    // 0x33	//	0x53		                                             
			NOP			W R			S<SB	SB<SB-0x10		 							;    // 0x53	//	0x43		                                             
			NOP			W R			S<SB	SB<SB+0x40		 							;    // 0x43	//	0x62		                                             
			NOP			W R			S<SB	SB<SB-0x30		 							;    // 0x62	//	0xf2		                                             
			NOP			W R			S<SB					 							;    // 0x32	//	0xf2		                                             
			NOP																			;                                     
			NOP																			;                                     
			NOP								SB<0			 							;    //기대값      // read된 값	                                             
			NOP			W R			S<SB	SB<0x44		     							;    //	0x0		//	0x44		                                             
			NOP			W R			S<SB	SB<0x10			 							;    //	0x44	//	0x10		                                             
			NOP			W R			S<SB	SB<S1A			 							;    // 0x10	//	0x33		                                             
			NOP			W R			S<SB	SB<SB+0x20		 							;    // 0x33	//	0x53		                                             
			NOP			W R			S<SB	SB<SB-0x10		 							;    // 0x53	//	0x43		                                             
			NOP			W R			S<SB	SB<SB+0x40		 							;    // 0x43	//	0x62		                                             
			NOP			W R			S<SB	SB<SB-0x30		 							;    // 0x62	//	0xf2		                                             
			NOP			W R			S<SB					 							;    // 0x32	//	0xf2		                                             

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

	START S_OUT_CNT_00;
			NOP					 														;                                                 
			NOP					 														;                                                 
			NOP		 			 														;                                                 
			NOP					 														;                                                 
			NOP					 														;                                                 
			NOP					 														;                                                 
			NOP					 														;                                                 
			NOP					 														;                                                 
			NOP					 														;                                                 
			NOP					 														;                                                 

		//	OUT 4					                          								;                                      
			NOP					                          								;                                                 
			NOP					                          								;                                                 
			NOP					                          	 							;    //기대값      // read된 값	                                             
			NOP			W R		                              							;    //	0x0		//	0x44		                                             
			NOP			W R		                          	 							;    //	0x44	//	0x10		                                             
			NOP			W R		                          	 							;    // 0x10	//	0x33		                                             
			NOP			W R		                          	 							;    // 0x33	//	0x53		                                             
			NOP			W R		                          	 							;    // 0x53	//	0x43		                                             
			NOP			W R		                          	 							;    // 0x43	//	0x62		                                             
			NOP			W R		                          	 							;    // 0x62	//	0xf2		                                             
			NOP			W R		                          	 							;    // 0x32	//	0xf2		                                             
			NOP					                          								;                                     
			NOP					                          								;                                     
			NOP					                          	 							;    //기대값      // read된 값	                                             
			NOP			W R		                              							;    //	0x0		//	0x44		                                             
			NOP			W R		                          	 							;    //	0x44	//	0x10		                                             
			NOP			W R		                          	 							;    // 0x10	//	0x33		                                             
			NOP			W R		                          	 							;    // 0x33	//	0x53		                                             
			NOP			W R		                          	 							;    // 0x53	//	0x43		                                             
			NOP			W R		                          	 							;    // 0x43	//	0x62		                                             
			NOP			W R		                          	 							;    // 0x62	//	0xf2		                                             
			NOP			W R		                          	 							;    // 0x32	//	0xf2		                                             

			NOP					 														;                                                 
			NOP					 														;                                                 
			OUT 4				 														;                                                 
			NOP					 														;                                                 
			NOP					 														;                                                 

			NOP			W R		                              							;    //	0x0		//	0x44		                                             
			NOP			W R		                          	 							;    //	0x44	//	0x10		                                             
			NOP			W R		                          	 							;    // 0x10	//	0x33		                                             
			NOP			W R		                          	 							;    // 0x33	//	0x53		                                             
			NOP			W R		                          	 							;    // 0x53	//	0x43		                                             
			NOP			W R		                          	 							;    // 0x43	//	0x62		                                             
			NOP			W R		                          	 							;    // 0x62	//	0xf2		                                             
			NOP			W R		                          	 							;    // 0x32	//	0xf2		                                             
			NOP					                          								;                                     
			NOP					                          								;                                     
			NOP					                          	 							;    //기대값      // read된 값	                                             
			NOP			W R		                              							;    //	0x0		//	0x44		                                             
			NOP			W R		                          	 							;    //	0x44	//	0x10		                                             
			NOP			W R		                          	 							;    // 0x10	//	0x33		                                             
			NOP			W R		                          	 							;    // 0x33	//	0x53		                                             
			NOP			W R		                          	 							;    // 0x53	//	0x43		                                             
			NOP			W R		                          	 							;    // 0x43	//	0x62		                                             
			NOP			W R		                          	 							;    // 0x62	//	0xf2		                                             
			NOP			W R		                          	 							;    // 0x32	//	0xf2		                                             

			NOP					 														;                                                 
			NOP					 														;                                                 
			NOP					 														;                                                 
			NOP					 														;                                                 
			NOP					 														;                                                 
    		STPS																		;
	END
	

;%ENDC
