
SDEF INIT     XB<0 XC<0 YB<0 YC<0 Z<0 TP<TPH1 TP2<TPH2 D3<D3B D4<D4B 
SDEF BCLR     XB<0 YB<0
SDEF CCLR     XC<0 YC<0
SDEF XCINC    XC<XC+1
SDEF YCINC    YC<YC+1
SDEF XINC     XC<XC+1
SDEF YINC     YC<YC+1
SDEF XBINC    XB<XB+1
SDEF YBINC    YB<YB+1
SDEF XCDEC    XC<XC-1
SDEF YCDEC    YC<YC-1
SDEF XDEC     XC<XC-1
SDEF YDEC     YC<YC-1
SDEF XBDEC    XB<XB-1
SDEF YBDEC    YB<YB-1
SDEF XYCINC   XC<XC+1 YC<YC+1
SDEF XYCDEC   XC<XC-1 YC<YC-1
SDEF ZINC     Z<Z+1
SDEF ZDEC     Z<Z-1

SDEF XCYC     X<XC Y<YC
SDEF YCXC     X<YC Y<XC
SDEF XBYB     X<XB Y<YB
SDEF YBXB     X<YB Y<XB
SDEF XBYC     X<XB Y<YC
SDEF XSYS     X<XS Y<YS
SDEF XCYS     X<XC Y<YS
SDEF TPA5     TP<TP+D5
SDEF TPD5     TP<TP-D5
;*****************************************************************************
;                      ACT 	 CS  		/R   /C   /W   AP
;                      C19 	C30 		C23  C22  C21  C10
;*****************************************************************************    			
SDEF NOP      CYP0         		
SDEF MRSJ     CYP0         	C30 		C23  C22  C21 ASCROFF DSCROFF
SDEF MRSS     CYP0         	C30 		C23  C22  C21 ASCROFF DSCROFF
SDEF REFR     CYP0         	C30 		C23  C22 
SDEF BSTP     CYP0         	C30  	              C21
                                                                                                    ;;BA0 BA1 BG0 BG1 
SDEF ACTV     CYP1     C19 	C30             				                					    ;;Bank 0
SDEF ACTV0    CYP1     C19 	C30             				                   					    ;;Bank 0
SDEF ACTV1    CYP1     C19 	C30             					C13             					;;Bank 1
SDEF ACTV2    CYP1     C19 	C30             					     C14         					;;Bank 2
SDEF ACTV3    CYP1     C19 	C30             					C13  C14         					;;Bank 3
SDEF ACTV4    CYP1     C19 	C30             					          C15      					;;Bank 4
SDEF ACTV5    CYP1     C19 	C30             					C13       C15      					;;Bank 5
SDEF ACTV6    CYP1     C19 	C30             					     C14  C15      					;;Bank 6
SDEF ACTV7    CYP1     C19 	C30             					C13  C14  C15      					;;Bank 7
SDEF ACTV8    CYP1     C19 	C30             					               C16  				;;Bank 8
SDEF ACTV9    CYP1     C19 	C30             					C13            C16  				;;Bank 9
SDEF ACTV10   CYP1     C19 	C30             					     C14       C16  				;;Bank 10
SDEF ACTV11   CYP1     C19 	C30             					C13  C14       C16  				;;Bank 11
SDEF ACTV12   CYP1     C19 	C30             					          C15  C16  				;;Bank 12
SDEF ACTV13   CYP1     C19 	C30             					C13       C15  C16  				;;Bank 13
SDEF ACTV14   CYP1     C19 	C30             					     C14  C15  C16  				;;Bank 14
SDEF ACTV15   CYP1     C19 	C30             					C13  C14  C15  C16  				;;Bank 15

SDEF WRIT     CYP2       	C30    			C22 	C21		                    	
SDEF WRIT0    CYP2       	C30    			C22 	C21		                    	
SDEF WRIT1    CYP2       	C30    			C22 	C21		    C13             	
SDEF WRIT2    CYP2       	C30    			C22 	C21		         C14         	
SDEF WRIT3    CYP2       	C30    			C22 	C21		    C13  C14         	
SDEF WRIT4    CYP2       	C30    			C22 	C21		              C15      	
SDEF WRIT5    CYP2       	C30    			C22 	C21		    C13       C15      	
SDEF WRIT6    CYP2       	C30    			C22 	C21		         C14  C15      	
SDEF WRIT7    CYP2       	C30    			C22 	C21		    C13  C14  C15      	
SDEF WRIT8    CYP2       	C30    			C22 	C21		                   C16  
SDEF WRIT9    CYP2       	C30    			C22 	C21		    C13            C16  
SDEF WRIT10   CYP2       	C30    			C22 	C21		         C14       C16  
SDEF WRIT11   CYP2       	C30    			C22 	C21		    C13  C14       C16  
SDEF WRIT12   CYP2       	C30    			C22 	C21		              C15  C16  
SDEF WRIT13   CYP2       	C30    			C22 	C21		    C13       C15  C16  
SDEF WRIT14   CYP2       	C30    			C22 	C21		         C14  C15  C16  
SDEF WRIT15   CYP2       	C30    			C22 	C21		    C13  C14  C15  C16  
       
SDEF WRITA    CYP2      	C30    			C22 	C21  C10 	                	
SDEF WRITA0   CYP2      	C30    			C22 	C21  C10 	                	
SDEF WRITA1   CYP2      	C30    			C22 	C21  C10 	C13             	
SDEF WRITA2   CYP2      	C30    			C22 	C21  C10 	     C14         	
SDEF WRITA3   CYP2      	C30    			C22 	C21  C10 	C13  C14         	
SDEF WRITA4   CYP2      	C30    			C22 	C21  C10 	          C15      	
SDEF WRITA5   CYP2      	C30    			C22 	C21  C10 	C13       C15      	
SDEF WRITA6   CYP2      	C30    			C22 	C21  C10 	     C14  C15      	
SDEF WRITA7   CYP2      	C30    			C22 	C21  C10 	C13  C14  C15      	
SDEF WRITA8   CYP2      	C30    			C22 	C21  C10 	               C16  
SDEF WRITA9   CYP2      	C30    			C22 	C21  C10 	C13            C16  
SDEF WRITA10  CYP2      	C30    			C22 	C21  C10 	     C14       C16  
SDEF WRITA11  CYP2      	C30    			C22 	C21  C10 	C13  C14       C16  
SDEF WRITA12  CYP2      	C30    			C22 	C21  C10 	          C15  C16  
SDEF WRITA13  CYP2      	C30    			C22 	C21  C10 	C13       C15  C16  
SDEF WRITA14  CYP2      	C30    			C22 	C21  C10 	     C14  C15  C16  
SDEF WRITA15  CYP2      	C30    			C22 	C21  C10 	C13  C14  C15  C16  
       
SDEF READ     CYP2       	C30    			C22        					                	
SDEF READ0    CYP2       	C30    			C22        			                	
SDEF READ1    CYP2       	C30    			C22        			C13             	
SDEF READ2    CYP2       	C30    			C22        			     C14         	
SDEF READ3    CYP2       	C30    			C22        			C13  C14         	
SDEF READ4    CYP2       	C30    			C22        			          C15      	
SDEF READ5    CYP2       	C30    			C22        			C13       C15      	
SDEF READ6    CYP2       	C30    			C22        			     C14  C15      	
SDEF READ7    CYP2       	C30    			C22        			C13  C14  C15      	
SDEF READ8    CYP2       	C30    			C22        			               C16  
SDEF READ9    CYP2       	C30    			C22        			C13            C16  
SDEF READ10   CYP2       	C30    			C22        			     C14       C16  
SDEF READ11   CYP2       	C30    			C22        			C13  C14       C16  
SDEF READ12   CYP2       	C30    			C22        			          C15  C16  
SDEF READ13   CYP2       	C30    			C22        			C13       C15  C16  
SDEF READ14   CYP2       	C30    			C22        			     C14  C15  C16  
SDEF READ15   CYP2       	C30    			C22        			C13  C14  C15  C16  
       
SDEF READA    CYP2       	C30    			C22    		C10		                 	
SDEF READA0   CYP2       	C30    			C22    		C10		                 	
SDEF READA1   CYP2       	C30    			C22    		C10		 C13             	
SDEF READA2   CYP2       	C30    			C22    		C10		      C14         	
SDEF READA3   CYP2       	C30    			C22    		C10		 C13  C14         	
SDEF READA4   CYP2       	C30    			C22    		C10		           C15      	
SDEF READA5   CYP2       	C30    			C22    		C10		 C13       C15      	
SDEF READA6   CYP2       	C30    			C22    		C10		      C14  C15      	
SDEF READA7   CYP2       	C30    			C22    		C10		 C13  C14  C15      	
SDEF READA8   CYP2       	C30    			C22    		C10		                C16  
SDEF READA9   CYP2       	C30    			C22    		C10		 C13            C16  
SDEF READA10  CYP2       	C30    			C22    		C10		      C14       C16  
SDEF READA11  CYP2       	C30    			C22    		C10		 C13  C14       C16  
SDEF READA12  CYP2       	C30    			C22    		C10		           C15  C16  
SDEF READA13  CYP2       	C30    			C22    		C10		 C13       C15  C16  
SDEF READA14  CYP2       	C30    			C22    		C10		      C14  C15  C16  
SDEF READA15  CYP2       	C30    			C22    		C10		 C13  C14  C15  C16  
       
SDEF PREC     CYP2       	C30 	C23    		   C21     		                 	
SDEF PREC0    CYP2       	C30 	C23    		   C21     		                 	
SDEF PREC1    CYP2       	C30 	C23    		   C21     		 C13             	
SDEF PREC2    CYP2       	C30 	C23    		   C21     		      C14         	
SDEF PREC3    CYP2       	C30 	C23    		   C21     		 C13  C14         	
SDEF PREC4    CYP2       	C30 	C23    		   C21     		           C15      	
SDEF PREC5    CYP2       	C30 	C23    		   C21     		 C13       C15      	
SDEF PREC6    CYP2       	C30 	C23    		   C21     		      C14  C15      	
SDEF PREC7    CYP2       	C30 	C23    		   C21     		 C13  C14  C15      	
SDEF PREC8    CYP2       	C30 	C23    		   C21     		                C16  
SDEF PREC9    CYP2       	C30 	C23    		   C21     		 C13            C16  
SDEF PREC10   CYP2       	C30 	C23    		   C21     		      C14       C16  
SDEF PREC11   CYP2       	C30 	C23    		   C21     		 C13  C14       C16  
SDEF PREC12   CYP2       	C30 	C23    		   C21     		           C15  C16  
SDEF PREC13   CYP2       	C30 	C23    		   C21     		 C13       C15  C16  
SDEF PREC14   CYP2       	C30 	C23    		   C21     		      C14  C15  C16  
SDEF PREC15   CYP2       	C30 	C23    		   C21     		 C13  C14  C15  C16  

SDEF PREA     CYP2       	C30 	C23    		   C21  C10
SDEF PRECA    CYP2       	C30 	C23    		   C21  C10
 
SDEF ZQCL     CYP2       	C30 				   C21  C10 
SDEF ZQCS     CYP2       	C30 				   C21      
;*****************************************************************************
;                      CK_t   CK_c   CKE0   CKE1  RESET  ODT0  ODT1   BC
;                      C0     C1     C2     C3    C20    C17   C18    C12
;*****************************************************************************    			
SDEF BLK               C0     C1          						    		        ;CKE(C7)=L  /Reset=L
SDEF BLKB              C0     C1							
SDEF DLK               C0     C1                  C20 								;CKE(C7)=L  /Reset=H
SDEF DLKB              C0     C1                  C20								
SDEF CLK                      C1     C2     C3    C20 								;CKE(C7)=H  /Reset=H
SDEF CLKB              C0     C1     C2     C3    C20								
SDEF CKE                             C2     C3            
SDEF ODT                                                 C17   C18
SDEF BC                                                               C12
;*****************************************************************************
;                      DQS0_t   DQS0_c    	  
;                      C4/C24   C5/C25   C6/C26   C7/C27       
;*****************************************************************************    			
SDEF DQSLL          
SDEF DQSLH                C24      C25      C26      C27   
SDEF DQSHL             C4       C5       C6       C7  
SDEF DQSHH             C4 C24   C5 C25   C6 C26   C7 C27

SDEF DT     
SDEF DB       /D
SDEF DT2    
SDEF DB2      /D2

SDEF W 	C28
SDEF R 	C29


