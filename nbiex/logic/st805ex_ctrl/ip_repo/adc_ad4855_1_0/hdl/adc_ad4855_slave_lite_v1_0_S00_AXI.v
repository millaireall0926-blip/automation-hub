
`timescale 1 ns / 1 ps

	module adc_ad4855_slave_lite_v1_0_S00_AXI #
	(
		// Users to add parameters here
    parameter IP_NAME           = "ADC-AD4855      ",
    parameter integer MAJOR_VER = 00,
    parameter integer MINOR_VER = 50,
    parameter integer YEAR      = 25,
    parameter integer MONTH     = 12,
    parameter integer DAY       = 17,
		// User parameters ends
		// Do not modify the parameters beyond this line

		// Width of S_AXI data bus
		parameter integer C_S_AXI_DATA_WIDTH	= 32,
		// Width of S_AXI address bus
		parameter integer C_S_AXI_ADDR_WIDTH	= 10
	)
	(
		// Users to add ports here
		output  wire          adc_pd,
		
    output  wire          adc_csn,
    output  wire          adc_csck,
    inout   wire          adc_csdio,
    input   wire          adc_csdo,
                          
    input   wire          adc_busy,
    output  wire          adc_cnv,
    output  wire          adc_scki,
    input   wire          adc_scko,
    input   wire          adc_sdo0,
    input   wire          adc_sdo1,
    input   wire          adc_sdo2,
    input   wire          adc_sdo3,
    input   wire          adc_sdo4,
    input   wire          adc_sdo5,
    input   wire          adc_sdo6,
    input   wire          adc_sdo7,
    
    output  wire  [ 3:0]  mux_a,
		// User ports ends
		// Do not modify the ports beyond this line

		// Global Clock Signal
		input wire  S_AXI_ACLK,
		// Global Reset Signal. This Signal is Active LOW
		input wire  S_AXI_ARESETN,
		// Write address (issued by master, acceped by Slave)
		input wire [C_S_AXI_ADDR_WIDTH-1 : 0] S_AXI_AWADDR,
		// Write channel Protection type. This signal indicates the
    		// privilege and security level of the transaction, and whether
    		// the transaction is a data access or an instruction access.
		input wire [2 : 0] S_AXI_AWPROT,
		// Write address valid. This signal indicates that the master signaling
    		// valid write address and control information.
		input wire  S_AXI_AWVALID,
		// Write address ready. This signal indicates that the slave is ready
    		// to accept an address and associated control signals.
		output wire  S_AXI_AWREADY,
		// Write data (issued by master, acceped by Slave) 
		input wire [C_S_AXI_DATA_WIDTH-1 : 0] S_AXI_WDATA,
		// Write strobes. This signal indicates which byte lanes hold
    		// valid data. There is one write strobe bit for each eight
    		// bits of the write data bus.    
		input wire [(C_S_AXI_DATA_WIDTH/8)-1 : 0] S_AXI_WSTRB,
		// Write valid. This signal indicates that valid write
    		// data and strobes are available.
		input wire  S_AXI_WVALID,
		// Write ready. This signal indicates that the slave
    		// can accept the write data.
		output wire  S_AXI_WREADY,
		// Write response. This signal indicates the status
    		// of the write transaction.
		output wire [1 : 0] S_AXI_BRESP,
		// Write response valid. This signal indicates that the channel
    		// is signaling a valid write response.
		output wire  S_AXI_BVALID,
		// Response ready. This signal indicates that the master
    		// can accept a write response.
		input wire  S_AXI_BREADY,
		// Read address (issued by master, acceped by Slave)
		input wire [C_S_AXI_ADDR_WIDTH-1 : 0] S_AXI_ARADDR,
		// Protection type. This signal indicates the privilege
    		// and security level of the transaction, and whether the
    		// transaction is a data access or an instruction access.
		input wire [2 : 0] S_AXI_ARPROT,
		// Read address valid. This signal indicates that the channel
    		// is signaling valid read address and control information.
		input wire  S_AXI_ARVALID,
		// Read address ready. This signal indicates that the slave is
    		// ready to accept an address and associated control signals.
		output wire  S_AXI_ARREADY,
		// Read data (issued by slave)
		output wire [C_S_AXI_DATA_WIDTH-1 : 0] S_AXI_RDATA,
		// Read response. This signal indicates the status of the
    		// read transfer.
		output wire [1 : 0] S_AXI_RRESP,
		// Read valid. This signal indicates that the channel is
    		// signaling the required read data.
		output wire  S_AXI_RVALID,
		// Read ready. This signal indicates that the master can
    		// accept the read data and response information.
		input wire  S_AXI_RREADY
	);

	// AXI4LITE signals
	reg [C_S_AXI_ADDR_WIDTH-1 : 0] 	axi_awaddr;
	reg  	axi_awready;
	reg  	axi_wready;
	reg [1 : 0] 	axi_bresp;
	reg  	axi_bvalid;
	reg [C_S_AXI_ADDR_WIDTH-1 : 0] 	axi_araddr;
	reg  	axi_arready;
	reg [1 : 0] 	axi_rresp;
	reg  	axi_rvalid;

	// Example-specific design signals
	// local parameter for addressing 32 bit / 64 bit C_S_AXI_DATA_WIDTH
	// ADDR_LSB is used for addressing 32/64 bit registers/memories
	// ADDR_LSB = 2 for 32 bits (n downto 2)
	// ADDR_LSB = 3 for 64 bits (n downto 3)
	localparam integer ADDR_LSB = (C_S_AXI_DATA_WIDTH/32) + 1;
	localparam integer OPT_MEM_ADDR_BITS = 7;
	//----------------------------------------------
	//-- Signals for user logic register space example
	//------------------------------------------------
	//-- Number of Slave Registers 256
	localparam integer REG_NUM = 256;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg[0:REG_NUM-1];
	integer	 byte_index;

  // Add user logic here
  integer j;
  
  wire          clk;
	wire          rstn;
  
  assign clk  = S_AXI_ACLK;
  assign rstn = S_AXI_ARESETN;
  
  wire          set;
  wire          auto;
  wire          mode;
  wire          rwreg;
  wire  [ 3:0]  mux;
  wire  [23:0]  cmd;
  
  wire          os;
  wire  [ 3:0]  ratio;
  wire  [31:0]  settling;
  
  assign set      = slv_reg[8][31];
  assign auto     = 1'b1; // slv_reg[8][30];
  assign mode     = 1'b0; // 0 for 3-wire mode, 1 for 4-wire mode
  assign rwreg    = slv_reg[8][28];
  assign mux      = slv_reg[8][27:24];
  assign cmd      = slv_reg[8][23:0];
  
  assign os       = slv_reg[9][7];
  assign ratio    = slv_reg[9][3:0];
  assign settling = slv_reg[10];
  
  
  wire          wip;
  wire          wic;
  wire          err;
  
  wire          reg_valid;
  wire  [ 7:0]  reg_value;
  wire          adc_valid;
  wire  [23:0]  adc_value_0;
  wire  [23:0]  adc_value_1;
  wire  [23:0]  adc_value_2;
  wire  [23:0]  adc_value_3;
  wire  [23:0]  adc_value_4;
  wire  [23:0]  adc_value_5;
  wire  [23:0]  adc_value_6;
  wire  [23:0]  adc_value_7;
  
  reg   [ 1:0]  cmd_st;
  reg   [31:0]  cmd_cycle;
  
  reg           cur_op; // 1 for manual set
  reg           cur_set;
  reg           cur_mode;
  reg           cur_rwreg;
  reg   [23:0]  cur_cmd;
  reg   [ 3:0]  cur_mux;
  
  localparam CMD_IDLE = 2'h0;
  localparam CMD_ING  = 2'h1;
  localparam CMD_DONE = 2'h2;
  localparam CMD_WAIT = 2'h3;
  
  always @ ( posedge clk ) begin
    if ( rstn == 1'b0 ) begin
      cmd_st    <= CMD_IDLE;
      cmd_cycle <= 0;
      
      cur_op    <= 0;
      cur_set   <= 0;
      cur_mode  <= 0;
      cur_rwreg <= 0;
      cur_mux   <= 0;
      cur_cmd   <= 0;
    end
    else begin
      case ( cmd_st )
        CMD_IDLE : begin
          if ( set == 1'b1 ) begin
            cmd_st    <= CMD_ING;
            cmd_cycle <= 0;
            
            cur_op    <= 1;
            cur_set   <= 1;
            cur_mode  <= 0;
            cur_rwreg <= rwreg;
            cur_mux   <= mux;
            cur_cmd   <= cmd;
          end
          else if ( auto == 1'b1 ) begin
            cmd_st    <= CMD_ING;
            cmd_cycle <= 0;
            
            cur_op    <= 0;
            cur_set   <= 1;
            cur_mode  <= 0;
            cur_rwreg <= 0;
            cur_mux   <= cur_mux + 1;
            cur_cmd   <= 0;
          end
          else begin
            cmd_st    <= CMD_IDLE;
            cmd_cycle <= 0;
            
            cur_op    <= 0;
            cur_set   <= 0;
            cur_mode  <= 0;
            cur_rwreg <= 0;
            // cur_mux   <= 0;
            cur_cmd   <= 0;
          end
        end
        
        CMD_ING : begin
          cur_set   <= 0;
          
          if ( wic == 1'b1 ) begin
            cmd_st    <= CMD_DONE;
          end
          // exit when ING over 100ms
          else if ( cmd_cycle >= 10000000 ) begin
            cmd_st    <= CMD_DONE;
          end
          else begin
            cmd_cycle <= cmd_cycle + 1;
          end
        end
        CMD_DONE : begin
          cmd_st    <= CMD_WAIT;
          
          cur_op    <= 0;
          cmd_cycle <= 0;
        end
        CMD_WAIT : begin
          if ( cmd_cycle > 100 ) begin
            cmd_st    <= CMD_IDLE;
            
            cur_op    <= 0;
          end
          else begin
            cmd_cycle <= cmd_cycle + 1;
          end
        end
        default : begin
          cmd_st    <= CMD_IDLE;
          cmd_cycle <= 0;
          
          cur_op    <= 0;
          cur_set   <= 0;
          cur_mode  <= 0;
          cur_rwreg <= 0;
          cur_mux   <= 0;
          cur_cmd   <= 0;
        end
      endcase
    end
  end
  
  
	adc_ad4855_spi_if u_adc_ad4855_spi_if (
		.clk            (clk            ),
		.rstn           (rstn           ),
		
		.set            (cur_set        ),
		.mode           (cur_mode       ),
		.rreg           (cur_rwreg      ),
		.cmd            (cur_cmd        ),
		
		.os             (os             ),
		.ratio          (ratio          ),
		.settling       (settling       ),
		
		.wip            (wip            ),
		.wic            (wic            ),
		.err            (err            ),
		
		.reg_valid      (reg_valid      ),
		.reg_value      (reg_value      ),
		
		.adc_valid      (adc_valid      ),
		.adc_value_0    (adc_value_0    ),
		.adc_value_1    (adc_value_1    ),
		.adc_value_2    (adc_value_2    ),
		.adc_value_3    (adc_value_3    ),
		.adc_value_4    (adc_value_4    ),
		.adc_value_5    (adc_value_5    ),
		.adc_value_6    (adc_value_6    ),
		.adc_value_7    (adc_value_7    ),
		
		.csn            (adc_csn        ),
		.csck           (adc_csck       ),
		.csdio          (adc_csdio      ),
		.csdo           (adc_csdo       ),
		
		.busy           (adc_busy       ),
		.cnv            (adc_cnv        ),
		.scki           (adc_scki       ),
		.scko           (adc_scko       ),
		.sdo0           (adc_sdo0       ),
		.sdo1           (adc_sdo1       ),
		.sdo2           (adc_sdo2       ),
		.sdo3           (adc_sdo3       ),
		.sdo4           (adc_sdo4       ),
		.sdo5           (adc_sdo5       ),
		.sdo6           (adc_sdo6       ),
		.sdo7           (adc_sdo7       )
	);
	
	assign adc_pd = slv_reg[9][16];
	assign mux_a  = cur_mux;
	
  // User logic ends
  
	// I/O Connections assignments

	assign S_AXI_AWREADY	= axi_awready;
	assign S_AXI_WREADY	= axi_wready;
	assign S_AXI_BRESP	= axi_bresp;
	assign S_AXI_BVALID	= axi_bvalid;
	assign S_AXI_ARREADY	= axi_arready;
	assign S_AXI_RRESP	= axi_rresp;
	assign S_AXI_RVALID	= axi_rvalid;
	 //state machine varibles 
	 reg [1:0] state_write;
	 reg [1:0] state_read;
	 //State machine local parameters
	 localparam Idle = 2'b00,Raddr = 2'b10,Rdata = 2'b11 ,Waddr = 2'b10,Wdata = 2'b11;
	// Implement Write state machine
	// Outstanding write transactions are not supported by the slave i.e., master should assert bready to receive response on or before it starts sending the new transaction
	always @(posedge S_AXI_ACLK)                                 
	  begin                                 
	     if (S_AXI_ARESETN == 1'b0)                                 
	       begin                                 
	         axi_awready <= 0;                                 
	         axi_wready <= 0;                                 
	         axi_bvalid <= 0;                                 
	         axi_bresp <= 0;                                 
	         axi_awaddr <= 0;                                 
	         state_write <= Idle;                                 
	       end                                 
	     else                                  
	       begin                                 
	         case(state_write)                                 
	           Idle:                                      
	             begin                                 
	               if(S_AXI_ARESETN == 1'b1)                                  
	                 begin                                 
	                   axi_awready <= 1'b1;                                 
	                   axi_wready <= 1'b1;                                 
	                   state_write <= Waddr;                                 
	                 end                                 
	               else state_write <= state_write;                                 
	             end                                 
	           Waddr:        //At this state, slave is ready to receive address along with corresponding control signals and first data packet. Response valid is also handled at this state                                 
	             begin                                 
	               if (S_AXI_AWVALID && S_AXI_AWREADY)                                 
	                  begin                                 
	                    axi_awaddr <= S_AXI_AWADDR;                                 
	                    if(S_AXI_WVALID)                                  
	                      begin                                   
	                        axi_awready <= 1'b1;                                 
	                        state_write <= Waddr;                                 
	                        axi_bvalid <= 1'b1;                                 
	                      end                                 
	                    else                                  
	                      begin                                 
	                        axi_awready <= 1'b0;                                 
	                        state_write <= Wdata;                                 
	                        if (S_AXI_BREADY && axi_bvalid) axi_bvalid <= 1'b0;                                 
	                      end                                 
	                  end                                 
	               else                                  
	                  begin                                 
	                    state_write <= state_write;                                 
	                    if (S_AXI_BREADY && axi_bvalid) axi_bvalid <= 1'b0;                                 
	                   end                                 
	             end                                 
	          Wdata:        //At this state, slave is ready to receive the data packets until the number of transfers is equal to burst length                                 
	             begin                                 
	               if (S_AXI_WVALID)                                 
	                 begin                                 
	                   state_write <= Waddr;                                 
	                   axi_bvalid <= 1'b1;                                 
	                   axi_awready <= 1'b1;                                 
	                 end                                 
	                else                                  
	                 begin                                 
	                   state_write <= state_write;                                 
	                   if (S_AXI_BREADY && axi_bvalid) axi_bvalid <= 1'b0;                                 
	                 end                                              
	             end                                 
	          endcase                                 
	        end                                 
	      end                                 

	// Implement memory mapped register select and write logic generation
	// The write data is accepted and written to memory mapped registers when
	// axi_awready, S_AXI_WVALID, axi_wready and S_AXI_WVALID are asserted. Write strobes are used to
	// select byte enables of slave registers while writing.
	// These registers are cleared when reset (active low) is applied.
	// Slave register write enable is asserted when valid address and data are available
	// and the slave is ready to accept the write address and write data.
	 

	always @( posedge S_AXI_ACLK )
	begin
	  if ( S_AXI_ARESETN == 1'b0 )
	    begin
        for ( j = 0; j < REG_NUM; j = j + 1 ) begin
            slv_reg[j] <= 0;
        end
	    end 
	  else begin
	    if (S_AXI_WVALID)
	      begin
	        if ( S_AXI_AWVALID ) begin
            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
                // Respective byte enables are asserted as per write strobes 
                // Slave register 0
                slv_reg[S_AXI_AWADDR[ADDR_LSB+OPT_MEM_ADDR_BITS:ADDR_LSB]][(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
              end
	        end
	        else begin
            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
                // Respective byte enables are asserted as per write strobes 
                // Slave register 0
                slv_reg[axi_awaddr[ADDR_LSB+OPT_MEM_ADDR_BITS:ADDR_LSB]][(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
              end
	        end
	      end
	      
      	// Add user logic here
      	else begin
          slv_reg[0] <= {IP_NAME[103:96], IP_NAME[111:104], IP_NAME[119:112], IP_NAME[127:120]};
          slv_reg[1] <= {IP_NAME[71:64], IP_NAME[79:72], IP_NAME[87:80], IP_NAME[95:88]};
          slv_reg[2] <= {IP_NAME[39:32], IP_NAME[47:40], IP_NAME[55:48], IP_NAME[63:56]};
          slv_reg[3] <= {IP_NAME[ 7: 0], IP_NAME[15: 8], IP_NAME[23:16], IP_NAME[31:24]};
          slv_reg[4] <= {16'b0, MAJOR_VER[7:0], MINOR_VER[7:0]};
          slv_reg[5] <= {8'b0, YEAR[7:0], MONTH[7:0], DAY[7:0]};
          
          // 
          if ( cur_op && wip ) begin
            slv_reg[ 8][31] <= 1'b0;
          end
          
          if ( set == 1'b1 ) begin
            slv_reg[12][31] <= 1'b0;
          end
          else if ( cur_op && wic ) begin
            slv_reg[12][31] <= 1'b1;
          end
          
          slv_reg[12][28:0] <= slv_reg[ 8][28:0];
          slv_reg[12][30] <= err;
          
          if ( cur_op && reg_valid ) begin
            slv_reg[12][7:0] <= reg_value;
          end
          
          slv_reg[13][0] <= adc_busy;
          slv_reg[13][11:8] <= mux_a;
          
          if ( cur_op && adc_valid ) begin
            slv_reg[16][15: 0] <= adc_value_0[23:8];
            slv_reg[16][31:16] <= adc_value_1[23:8];
            slv_reg[17][15: 0] <= adc_value_2[23:8];
            slv_reg[17][31:16] <= adc_value_3[23:8];
            slv_reg[18][15: 0] <= adc_value_4[23:8];
            slv_reg[18][31:16] <= adc_value_5[23:8];
            slv_reg[19][15: 0] <= adc_value_6[23:8];
            slv_reg[19][31:16] <= adc_value_7[23:8];
          end
          
          // auto meas val update
          if ( adc_valid ) begin
            slv_reg[64 + 4 * cur_mux][15: 0] <= adc_value_0[23:8];
            slv_reg[64 + 4 * cur_mux][31:16] <= adc_value_1[23:8];
            slv_reg[65 + 4 * cur_mux][15: 0] <= adc_value_2[23:8];
            slv_reg[65 + 4 * cur_mux][31:16] <= adc_value_3[23:8];
            slv_reg[66 + 4 * cur_mux][15: 0] <= adc_value_4[23:8];
            slv_reg[66 + 4 * cur_mux][31:16] <= adc_value_5[23:8];
            slv_reg[67 + 4 * cur_mux][15: 0] <= adc_value_6[23:8];
            slv_reg[67 + 4 * cur_mux][31:16] <= adc_value_7[23:8];
          end
          
        end
      	// User logic ends
	  end
	end    

	// Implement read state machine
	  always @(posedge S_AXI_ACLK)                                       
	    begin                                       
	      if (S_AXI_ARESETN == 1'b0)                                       
	        begin                                       
	         //asserting initial values to all 0's during reset                                       
	         axi_arready <= 1'b0;                                       
	         axi_rvalid <= 1'b0;                                       
	         axi_rresp <= 1'b0;                                       
	         state_read <= Idle;                                       
	        end                                       
	      else                                       
	        begin                                       
	          case(state_read)                                       
	            Idle:     //Initial state inidicating reset is done and ready to receive read/write transactions                                       
	              begin                                                
	                if (S_AXI_ARESETN == 1'b1)                                        
	                  begin                                       
	                    state_read <= Raddr;                                       
	                    axi_arready <= 1'b1;                                       
	                  end                                       
	                else state_read <= state_read;                                       
	              end                                       
	            Raddr:        //At this state, slave is ready to receive address along with corresponding control signals                                       
	              begin                                       
	                if (S_AXI_ARVALID && S_AXI_ARREADY)                                       
	                  begin                                       
	                    state_read <= Rdata;                                       
	                    axi_araddr <= S_AXI_ARADDR;                                       
	                    axi_rvalid <= 1'b1;                                       
	                    axi_arready <= 1'b0;                                       
	                  end                                       
	                else state_read <= state_read;                                       
	              end                                       
	            Rdata:        //At this state, slave is ready to send the data packets until the number of transfers is equal to burst length                                       
	              begin                                           
	                if (S_AXI_RVALID && S_AXI_RREADY)                                       
	                  begin                                       
	                    axi_rvalid <= 1'b0;                                       
	                    axi_arready <= 1'b1;                                       
	                    state_read <= Raddr;                                       
	                  end                                       
	                else state_read <= state_read;                                       
	              end                                       
	           endcase                                       
	          end                                       
	        end                                         
	// Implement memory mapped register select and read logic generation
	  assign S_AXI_RDATA = slv_reg[axi_araddr[ADDR_LSB+OPT_MEM_ADDR_BITS:ADDR_LSB]];
	// Add user logic here

	// User logic ends

	endmodule
	
	module adc_ad4855_spi_if (
		input		wire					clk,
		input		wire					rstn,
		
		input		wire					set,
		input		wire					mode,
		input		wire					rreg,
		input		wire	[23:0]	cmd,
		
		input   wire          os,
		input   wire  [ 3:0]  ratio,
		input		wire	[31:0]	settling,
		
		output  reg           wip,
		output  reg           wic,
		output  reg           err,
		
		output  reg           reg_valid,
		output	wire	[ 7:0]	reg_value,
		
		output  reg           adc_valid,
		output	wire	[23:0]	adc_value_0,
		output	wire	[23:0]	adc_value_1,
		output	wire	[23:0]	adc_value_2,
		output	wire	[23:0]	adc_value_3,
		output	wire	[23:0]	adc_value_4,
		output	wire	[23:0]	adc_value_5,
		output	wire	[23:0]	adc_value_6,
		output	wire	[23:0]	adc_value_7,
		
		// SPI I/F
		output	reg						csn,
		output	reg						csck,		// up to 25MHz
		inout   wire          csdio,
		input   wire          csdo,
		
		// ADC I/F
		input   wire          busy,
		output  reg           cnv,
		output  reg           scki,
		input   wire          scko,
		input   wire          sdo0,
		input   wire          sdo1,
		input   wire          sdo2,
		input   wire          sdo3,
		input   wire          sdo4,
		input   wire          sdo5,
		input   wire          sdo6,
		input   wire          sdo7
	);
	
	// parameter
	localparam ADDR_WIDTH	= 16; // RnW, addr[14:0],
	localparam DATA_WIDTH	= 24; // RnW, addr[14:0], data[7:0]
	localparam SCLK_DUTY	= 3;	// 2^3 -> SCLK = clk / (2^3) = 12.5MHz
	localparam SCLK_CYCLE	= 16;
	
	localparam tREAD_WRITE = DATA_WIDTH * ( 1 << SCLK_DUTY );
	localparam tREAD_START = ADDR_WIDTH * ( 1 << SCLK_DUTY );
	
	localparam tCYCLE      = 400; // 4000ns
	
	// CPOL = 0
	// wdata : launch @ falling, capture @ rising
	// rdata : launch @ falling, capture @ rising
	// mode  : default 0 for 3-wire, 1 for 4-wire
	// cmd   : {23.rnw, 23-8.A14-A0, 7-0.D7-D0}
	
	// state machine
	localparam IDLE_ST		= 4'h0;
	localparam WRITE_ST	  = 4'h1;
	localparam READ_ST		= 4'h2;
	
	localparam OS_ST	  	= 4'h9;
	localparam CNV_ST	  	= 4'hA;
	localparam BUSY_ST    = 4'hB;
	localparam SHIFT_ST   = 4'hC;
	localparam LAST_ST    = 4'hE;
	
	localparam DONE_ST		= 4'hD;
	
  reg   [ 3:0]  cur_st;
  reg   [23:0]  cur_cmd;

  reg           ren;
  reg           sdat_o;
  wire          sdat_i;
	
	assign sdat_i = ( mode == 1'b0 ) ? csdio : csdo;
	assign csdio = ( ren == 1'b0 ) ? sdat_o : 1'bz;
		
  reg		[DATA_WIDTH-1:0]	sr_write;
  reg		[DATA_WIDTH-1:0]	sr_read;
  
  reg		[DATA_WIDTH-1:0]	sr_adc_0;
  reg		[DATA_WIDTH-1:0]	sr_adc_1;
  reg		[DATA_WIDTH-1:0]	sr_adc_2;
  reg		[DATA_WIDTH-1:0]	sr_adc_3;
  reg		[DATA_WIDTH-1:0]	sr_adc_4;
  reg		[DATA_WIDTH-1:0]	sr_adc_5;
  reg		[DATA_WIDTH-1:0]	sr_adc_6;
  reg		[DATA_WIDTH-1:0]	sr_adc_7;
  
  reg		[SCLK_CYCLE-1:0]	cycle_cnt;
  reg   [31:0]  delay_cnt;
  reg   [31:0]  os_cycle;
	    
	always @ ( posedge clk ) begin
		if ( rstn == 1'b0 ) begin
		  cur_st <= IDLE_ST;
		  cur_cmd <= 0;
		  
		  wip <= 0;
		  wic <= 0;
		  err <= 0;
		  
			csn <= 1'b1;
			csck <= 1'b0;
			ren <= 1'b0;
			sdat_o <= 1'b0;
			
			cnv <= 1'b0;
			scki <= 1'b0;
			
			reg_valid <= 1'b0;
			adc_valid <= 1'b0;
			
			sr_write <= 0;
			sr_read <= 0;
			
			sr_adc_0 <= 0;
			sr_adc_1 <= 0;
			sr_adc_2 <= 0;
			sr_adc_3 <= 0;
			sr_adc_4 <= 0;
			sr_adc_5 <= 0;
			sr_adc_6 <= 0;
			sr_adc_7 <= 0;
			
			cycle_cnt <= 0;
			delay_cnt <= 0;
			os_cycle <= 0;
		end
		else begin
			case (cur_st)
				IDLE_ST : begin
					if ( ( set == 1'b1 ) && ( rreg == 1'b1 ) ) begin
					  if ( cmd[23] == 1'b1 ) begin
					    cur_st <= READ_ST;
					  end
					  else begin
					    cur_st <= WRITE_ST;
					  end
					  
					  wip <= 1;
					  err <= 0;
      			csn <= 1'b0;
					end
					else if ( ( set == 1'b1 ) && ( rreg == 1'b0 ) ) begin
					  cur_st <= CNV_ST;
					  
					  wip <= 1;
					  err <= 0;
      			csn <= 1'b0;
					end
					else begin
					  err <= err;
      			csn <= 1'b1;
					end
					
    		  wic <= 0;
    		  
    			csck <= 1'b0;
    			ren <= 1'b0;
    			sdat_o <= 1'b0;
    			
    			cnv <= 1'b0;
    			scki <= 1'b0;
					
					reg_valid <= 1'b0;
					adc_valid <= 1'b0;
					
					cur_cmd <= cmd;
					sr_write <= cmd; 
					sr_read <= sr_read;
					cycle_cnt <= 0;
					delay_cnt <= 0;
					os_cycle <= 0;
				end
				WRITE_ST : begin
				  wip <= 1;
				  
					if ( cycle_cnt >= tREAD_WRITE ) begin
						cur_st <= LAST_ST;
						
						csck <= 0;
					end
					else begin
						csn <= 1'b0;
						csck <= cycle_cnt[SCLK_DUTY-1];
						ren <= 1'b0;
						
						if ( cycle_cnt[SCLK_DUTY-1:0] == {SCLK_DUTY{1'b0}} ) begin
							sdat_o <= sr_write[DATA_WIDTH-1];
							sr_write[DATA_WIDTH-1:0] <= {sr_write[DATA_WIDTH-2:0], 1'b0};
						end
						
						sr_read <= 0;
						cycle_cnt <= cycle_cnt + 1;
					end
				end
				READ_ST : begin
				  wip <= 1;
				  
					if ( cycle_cnt >= tREAD_WRITE ) begin
					  cur_st <= LAST_ST;
					  
					  csck <= 0;
					end
					else if ( cycle_cnt < tREAD_START ) begin
						csn <= 1'b0;
						csck <= cycle_cnt[SCLK_DUTY-1];
						ren <= 1'b0;
						
						if ( cycle_cnt[SCLK_DUTY-1:0] == {SCLK_DUTY{1'b0}} ) begin
							sdat_o <= sr_write[DATA_WIDTH-1];
							sr_write[DATA_WIDTH-1:0] <= {sr_write[DATA_WIDTH-2:0], 1'b0};
						end
						
						cycle_cnt <= cycle_cnt + 1'b1;
					end
					else if ( cycle_cnt < tREAD_WRITE ) begin
						csn <= 1'b0;
						csck <= cycle_cnt[SCLK_DUTY-1];
						ren <= 1'b1;
						sdat_o <= 1'b0;
						
						if ( csck && ( cycle_cnt[SCLK_DUTY-1:0] == 3'b101 ) ) begin
							sr_read[DATA_WIDTH-1:0] <= {sr_read[DATA_WIDTH-2:0], sdat_i};
						end
						
						cycle_cnt <= cycle_cnt + 1'b1;
					end
				end
				OS_ST : begin
				  cycle_cnt <= 1;
				  if ( delay_cnt >= tCYCLE ) begin
				    cur_st <= CNV_ST;
				    
  				  os_cycle <= os_cycle + 1;
  				end
  				else begin
  				  delay_cnt <= delay_cnt + 1;
  				end
				end
				CNV_ST : begin
				  if ( cycle_cnt >= 10 ) begin
				    delay_cnt <= 0;
				    
				    if ( os == 1'b1 ) begin
				      if ( os_cycle <= ( 1 << ( ( ratio + 1 ) - 1 ) ) ) begin
				        cur_st <= OS_ST;
				      end
				      else begin
				        cur_st <= BUSY_ST;
				      end
				    end
				    else begin
				      cur_st <= BUSY_ST;
				    end
				    
				    cnv <= 1'b0;
				  end
				  else begin
  				  if ( ( delay_cnt >= settling ) || ( cycle_cnt != 0 ) ) begin
  				    cnv <= 1'b1;
  				    cycle_cnt <= cycle_cnt + 1;
  				    delay_cnt <= 0;
  				  end
  				  else begin
        		  wic <= 0;
        			
        			cnv <= 1'b0;
        			scki <= 1'b0;
    					
    					cycle_cnt <= 0;
  				    delay_cnt <= delay_cnt + 1;
				    end
				  end
				  
					sr_adc_0 <= 0;
					sr_adc_1 <= 0;
					sr_adc_2 <= 0;
					sr_adc_3 <= 0;
					sr_adc_4 <= 0;
					sr_adc_5 <= 0;
					sr_adc_6 <= 0;
					sr_adc_7 <= 0;
				end
				BUSY_ST : begin
				  cycle_cnt <= 0;
				  
				  if ( busy == 1'b0 ) begin
				    cur_st <= SHIFT_ST;
				  end
				  else begin
				    if ( delay_cnt > 1000 ) begin
				      cur_st <= LAST_ST;
				      
				      err <= 1;
				    end
				    else begin
  				    delay_cnt <= delay_cnt + 1;
  				  end
				  end
				end
				SHIFT_ST : begin
					if ( cycle_cnt >= tREAD_WRITE ) begin
						cur_st <= LAST_ST;
						
						scki <= 0;
					end
					else begin
						scki <= cycle_cnt[SCLK_DUTY-1];
						
						if ( scki && ( cycle_cnt[SCLK_DUTY-1:0] == 3'b101 ) ) begin
							sr_adc_0[DATA_WIDTH-1:0] <= {sr_adc_0[DATA_WIDTH-2:0], sdo0};
							sr_adc_1[DATA_WIDTH-1:0] <= {sr_adc_1[DATA_WIDTH-2:0], sdo1};
							sr_adc_2[DATA_WIDTH-1:0] <= {sr_adc_2[DATA_WIDTH-2:0], sdo2};
							sr_adc_3[DATA_WIDTH-1:0] <= {sr_adc_3[DATA_WIDTH-2:0], sdo3};
							sr_adc_4[DATA_WIDTH-1:0] <= {sr_adc_4[DATA_WIDTH-2:0], sdo4};
							sr_adc_5[DATA_WIDTH-1:0] <= {sr_adc_5[DATA_WIDTH-2:0], sdo5};
							sr_adc_6[DATA_WIDTH-1:0] <= {sr_adc_6[DATA_WIDTH-2:0], sdo6};
							sr_adc_7[DATA_WIDTH-1:0] <= {sr_adc_7[DATA_WIDTH-2:0], sdo7};
						end
						
						cycle_cnt <= cycle_cnt + 1;
					end
				end
				LAST_ST : begin
					cur_st <= DONE_ST;
					
					wip <= 0;
					wic <= 1;
					
					csn <= 1'b0;
					csck <= 1'b0;
					ren <= 1'b0;
					sdat_o <= 1'b0;
					
    			cnv <= 1'b0;
    			scki <= 1'b0;
					
					if ( rreg == 1'b1 ) begin
					  reg_valid <= cur_cmd[23];
					end
					else begin
					  adc_valid <= 1;
					end
					
					sr_write <= 0;
					sr_read <= sr_read;
				end
				DONE_ST : begin
					cur_st <= IDLE_ST;
					
					wip <= 0;
					wic <= 1;
					
					csn <= 1'b0;
					csck <= 1'b0;
					ren <= 1'b0;
					sdat_o <= 1'b0;
					
    			cnv <= 1'b0;
    			scki <= 1'b0;
				
				  reg_valid <= 0;
				  adc_valid <= 0;
				
					sr_write <= 0;
					
    			cycle_cnt <= 0;
    			delay_cnt <= 0;
    			os_cycle <= 0;
				end
				default : begin
					cur_st <= IDLE_ST;
					
					wip <= 0;
					wic <= 0;
					err <= 0;
					
					csn <= 1'b1;
					csck <= 1'b0;
					ren <= 1'b0;
					sdat_o <= 1'b0;
					
    			cnv <= 1'b0;
    			scki <= 1'b0;
					
					reg_valid <= 1'b0;
					adc_valid <= 1'b0;
					
					sr_write <= 0;
					sr_read <= 0;
					
					sr_adc_0 <= 0;
					sr_adc_1 <= 0;
					sr_adc_2 <= 0;
					sr_adc_3 <= 0;
					sr_adc_4 <= 0;
					sr_adc_5 <= 0;
					sr_adc_6 <= 0;
					sr_adc_7 <= 0;
    			
    			cycle_cnt <= 0;
    			delay_cnt <= 0;
    			os_cycle <= 0;
				end
			endcase
		end
	end
	
	
	assign reg_value = sr_read[7:0];
	
	assign adc_value_0 = sr_adc_0;
	assign adc_value_1 = sr_adc_1;
	assign adc_value_2 = sr_adc_2;
	assign adc_value_3 = sr_adc_3;
	assign adc_value_4 = sr_adc_4;
	assign adc_value_5 = sr_adc_5;
	assign adc_value_6 = sr_adc_6;
	assign adc_value_7 = sr_adc_7;
	
	// 
  (* dont_touch = "true" *)	(* mark_debug = "true" *)reg   [23:0]	cap_value_0;
  (* dont_touch = "true" *)	(* mark_debug = "true" *)reg   [23:0]	cap_value_1;
  (* dont_touch = "true" *)	(* mark_debug = "true" *)reg   [23:0]	cap_value_2;
  (* dont_touch = "true" *)	(* mark_debug = "true" *)reg   [23:0]	cap_value_3;
  (* dont_touch = "true" *)	(* mark_debug = "true" *)reg   [23:0]	cap_value_4;
  (* dont_touch = "true" *)	(* mark_debug = "true" *)reg   [23:0]	cap_value_5;
  (* dont_touch = "true" *)	(* mark_debug = "true" *)reg   [23:0]	cap_value_6;
  (* dont_touch = "true" *)	(* mark_debug = "true" *)reg   [23:0]	cap_value_7;
	
	always @ ( posedge clk ) begin
	  if ( rstn == 1'b0 ) begin
	    cap_value_0 <= 0;
	    cap_value_1 <= 0;
	    cap_value_2 <= 0;
	    cap_value_3 <= 0;
	    cap_value_4 <= 0;
	    cap_value_5 <= 0;
	    cap_value_6 <= 0;
	    cap_value_7 <= 0;
	  end
	  else begin
	    if ( cnv ) begin
  	    cap_value_0 <= 0;
  	    cap_value_1 <= 0;
  	    cap_value_2 <= 0;
  	    cap_value_3 <= 0;
  	    cap_value_4 <= 0;
  	    cap_value_5 <= 0;
  	    cap_value_6 <= 0;
  	    cap_value_7 <= 0;
	    end
	    else begin
				if ( scko && ( cycle_cnt[SCLK_DUTY-1:0] == 3'b101 ) ) begin
					cap_value_0[DATA_WIDTH-1:0] <= {cap_value_0[DATA_WIDTH-2:0], sdo0};
					cap_value_1[DATA_WIDTH-1:0] <= {cap_value_1[DATA_WIDTH-2:0], sdo1};
					cap_value_2[DATA_WIDTH-1:0] <= {cap_value_2[DATA_WIDTH-2:0], sdo2};
					cap_value_3[DATA_WIDTH-1:0] <= {cap_value_3[DATA_WIDTH-2:0], sdo3};
					cap_value_4[DATA_WIDTH-1:0] <= {cap_value_4[DATA_WIDTH-2:0], sdo4};
					cap_value_5[DATA_WIDTH-1:0] <= {cap_value_5[DATA_WIDTH-2:0], sdo5};
					cap_value_6[DATA_WIDTH-1:0] <= {cap_value_6[DATA_WIDTH-2:0], sdo6};
					cap_value_7[DATA_WIDTH-1:0] <= {cap_value_7[DATA_WIDTH-2:0], sdo7};
				end
	    end
	  end
	end
	
	endmodule
	
	// (* dont_touch = "true" *)	(* mark_debug = "true" *)
	