
`timescale 1 ns / 1 ps

	module lbx8_c2t_v1_0_S00_AXI #
	(
		// Users to add parameters here
    parameter IP_NAME           = "LBx8-C2T        ",
    parameter integer MAJOR_VER = 00,
    parameter integer MINOR_VER = 50,
    parameter integer YEAR      = 21,
    parameter integer MONTH     = 09,
    parameter integer DAY       = 22,
    
    parameter integer TG_NUM    = 1,
    // User parameters ends
    // Do not modify the parameters beyond this line

    // Width of S_AXI data bus
    parameter integer C_S_AXI_DATA_WIDTH  = 32,
    // Width of S_AXI address bus
    parameter integer C_S_AXI_ADDR_WIDTH  = 6
  )
  (
    // Users to add ports here
    output  wire          lb_tg_buf_clk,
    output  wire          lb_tg_buf_rst,
    output  reg           lb_tg_buf_en,
    output  reg   [ 3:0]  lb_tg_buf_we,
    output  reg   [31:0]  lb_tg_buf_addr,
    input   wire  [31:0]  lb_tg_buf_datai,
    output  reg   [31:0]  lb_tg_buf_datao,
    
    input   wire  [TG_NUM-1:0]    tg_lb_valid,
    input   wire  [TG_NUM-1:0]    tg_lb_last,
    input   wire  [TG_NUM-1:0]    tg_lb_error,
    output  wire  [TG_NUM-1:0]    tg_lb_clk,
    output  wire  [TG_NUM-1:0]    tg_lb_rstn,
    output  wire  [TG_NUM-1:0]    tg_lb_wr,
    inout   wire  [TG_NUM*8-1:0]  tg_lb_ad,
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
	reg [C_S_AXI_DATA_WIDTH-1 : 0] 	axi_rdata;
	reg [1 : 0] 	axi_rresp;
	reg  	axi_rvalid;

	// Example-specific design signals
	// local parameter for addressing 32 bit / 64 bit C_S_AXI_DATA_WIDTH
	// ADDR_LSB is used for addressing 32/64 bit registers/memories
	// ADDR_LSB = 2 for 32 bits (n downto 2)
	// ADDR_LSB = 3 for 64 bits (n downto 3)
	localparam integer ADDR_LSB = (C_S_AXI_DATA_WIDTH/32) + 1;
	localparam integer OPT_MEM_ADDR_BITS = 3;
	//----------------------------------------------
	//-- Signals for user logic register space example
	//------------------------------------------------
	//-- Number of Slave Registers 16
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg0;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg1;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg2;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg3;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg4;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg5;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg6;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg7;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg8;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg9;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg10;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg11;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg12;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg13;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg14;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg15;
	wire	 slv_reg_rden;
	wire	 slv_reg_wren;
	reg [C_S_AXI_DATA_WIDTH-1:0]	 reg_data_out;
	integer	 byte_index;
	reg	 aw_en;

  // Add user logic here
  wire buf_rst = ~S_AXI_ARESETN;
  wire buf_clk = S_AXI_ACLK;
  
  genvar        j, k;
  
  wire          lb8_rstn;
  wire  [TG_NUM-1:0]  lb_clk;
  wire  [TG_NUM-1:0]  lb_wr, l_wr;
  wire  [TG_NUM-1:0]  lb_valid, l_valid;
  
  wire  [ 7:0]  l_rdata[0:TG_NUM-1];
  wire  [ 7:0]  l_wdata[0:TG_NUM-1];
  wire  [ 7:0]  lb_rdata[0:TG_NUM-1];
  wire  [ 7:0]  lb_wdata[0:TG_NUM-1];
  
  /*generate
  
    for ( j=0; j<TG_NUM; j=j+1 ) begin
      OBUF OBUF_LB_RSTN ( .O(tg_lb_rstn[j]), .I(lb8_rstn      ) );
      OBUF OBUF_LB_CLK  ( .O(tg_lb_clk[j] ), .I(lb_clk[j]     ) );
      OBUF OBUF_LB_WR   ( .O(tg_lb_wr[j]  ), .I(lb_wr[j]      ) );
      
      IBUF IBUF_LB_VALID( .O(lb_valid[j]  ), .I(tg_lb_valid[j]) );
      
      ODDR #(
        .DDR_CLK_EDGE("OPPOSITE_EDGE"), // "OPPOSITE_EDGE" or "SAME_EDGE" 
        .INIT(1'b0),    // Initial value of Q: 1'b0 or 1'b1
        .SRTYPE("SYNC") // Set/Reset type: "SYNC" or "ASYNC" 
      ) ODDR_LB_CLK (
        .Q(lb_clk[j]),   // 1-bit output: Data output to IOB
        .C(buf_clk),   // 1-bit input: High-speed clock input
        .CE(1'b1), // 1-bit clock enable input
        .D1(1'b1), // 1-bit input: Parallel data input 1
        .D2(1'b0), // 1-bit input: Parallel data input 2
        .R(1'b0),   // 1-bit reset
        .S(1'b0)    // 1-bit set
      );
      
      ODDR #(
        .DDR_CLK_EDGE("OPPOSITE_EDGE"), // "OPPOSITE_EDGE" or "SAME_EDGE" 
        .INIT(1'b0),    // Initial value of Q: 1'b0 or 1'b1
        .SRTYPE("SYNC") // Set/Reset type: "SYNC" or "ASYNC" 
      ) ODDR_LB_WR (
        .Q(lb_wr[j]),   // 1-bit output: Data output to IOB
        .C(buf_clk),   // 1-bit input: High-speed clock input
        .CE(1'b1), // 1-bit clock enable input
        .D1(l_wr[j]), // 1-bit input: Parallel data input 1
        .D2(l_wr[j]), // 1-bit input: Parallel data input 2
        .R(1'b0),   // 1-bit reset
        .S(1'b0)    // 1-bit set
      );
      
      IDDR #(
        //.DDR_CLK_EDGE("OPPOSITE_EDGE"), // IDDRE1 mode (OPPOSITE_EDGE, SAME_EDGE, SAME_EDGE_PIPELINED)
        .DDR_CLK_EDGE("OPPOSITE_EDGE"), // IDDRE1 mode (OPPOSITE_EDGE, SAME_EDGE, SAME_EDGE_PIPELINED)
        .INIT_Q1(1'b0), // Initial value of Q1: 1'b0 or 1'b1
        .INIT_Q2(1'b0), // Initial value of Q2: 1'b0 or 1'b1
        .SRTYPE("SYNC") // Set/Reset type: "SYNC" or "ASYNC" 
      ) IDDR_LB_VALID (
        .Q1(l_valid[j]), // 1-bit output: Registered parallel output 1
        .Q2(), // 1-bit output: Registered parallel output 2
        .C(buf_clk),   // 1-bit input: High-speed clock
        .CE(1'b1), // 1-bit clock enable input
        .D(lb_valid[j]),   // 1-bit input: Serial Data Input
        .R(1'b0),   // 1-bit reset
        .S(1'b0)    // 1-bit set
      );
      
      for ( k=0; k<8; k=k+1 ) begin
        ODDR #(
        .DDR_CLK_EDGE("OPPOSITE_EDGE"), // "OPPOSITE_EDGE" or "SAME_EDGE" 
        .INIT(1'b0),    // Initial value of Q: 1'b0 or 1'b1
        .SRTYPE("SYNC") // Set/Reset type: "SYNC" or "ASYNC" 
        ) ODDR_LB_WDATA (
          .Q(lb_wdata[j][k]),   // 1-bit output: Data output to IOB
          .C(buf_clk),   // 1-bit input: High-speed clock input
          .CE(1'b1), // 1-bit clock enable input
          .D1(l_wdata[j][k]), // 1-bit input: Parallel data input 1
          .D2(l_wdata[j][k]), // 1-bit input: Parallel data input 2
          .R(1'b0),   // 1-bit reset
          .S(1'b0)    // 1-bit set
        );
        
        IDDR #(
          //.DDR_CLK_EDGE("OPPOSITE_EDGE"), // IDDRE1 mode (OPPOSITE_EDGE, SAME_EDGE, SAME_EDGE_PIPELINED)
          .DDR_CLK_EDGE("OPPOSITE_EDGE"), // IDDRE1 mode (OPPOSITE_EDGE, SAME_EDGE, SAME_EDGE_PIPELINED)
          .INIT_Q1(1'b0), // Initial value of Q1: 1'b0 or 1'b1
          .INIT_Q2(1'b0), // Initial value of Q2: 1'b0 or 1'b1
          .SRTYPE("SYNC") // Set/Reset type: "SYNC" or "ASYNC" 
        ) IDDR_LB_RDATA (
          .Q1(l_rdata[j][k]), // 1-bit output: Registered parallel output 1
          .Q2(), // 1-bit output: Registered parallel output 2
          .C(buf_clk),   // 1-bit input: High-speed clock
          .CE(1'b1), // 1-bit clock enable input
          .D(lb_rdata[j][k]),   // 1-bit input: Serial Data Input
          .R(1'b0),   // 1-bit reset
          .S(1'b0)    // 1-bit set
        );
        
        IOBUF IOBUF_LB_AD (
          .O(lb_rdata[j][k]), // Buffer output
          .IO(tg_lb_ad[8*j+k]),  // Buffer inout port (connect directly to top-level port)
          .I(lb_wdata[j][k]), // Buffer input
          .T(lb_valid[j])   // 3-state enable input, high=input, low=output
        );
      end

    end
    
  endgenerate*/


  wire          m_rst, m_rst_clr, m_rstn;
  reg   [ 7:0]  m_rst_sr;
  
  assign m_rst = slv_reg9[31];

  always @ ( posedge buf_clk ) begin
    if ( buf_rst == 1'b1 ) begin
      m_rst_sr <= 8'b0;
    end
    else begin
      m_rst_sr[7:0] <= {m_rst_sr[6:0], m_rst};
    end
  end
  
  assign m_rst_clr = m_rst_sr[4];
  
  assign m_rstn = ~( m_rst && ( ~m_rst_sr[4] ) );
  assign lb8_rstn = ( ~buf_rst ) && m_rstn;
  
  /* LOCAL BUS COM */
  wire  [ 1:0]  l_sel;
  wire  [ 7:0]  l_cmd[0:TG_NUM-1];
  wire  [15:0]  l_bl[0:TG_NUM-1];
  wire  [31:0]  l_addr[0:TG_NUM-1];
  wire  [ 7:0]  l_rl[0:TG_NUM-1];
  
  wire  [TG_NUM-1:0]  l_error;
  wire  [TG_NUM-1:0]  l_last;
  
  assign l_error = tg_lb_error;
  assign l_last = tg_lb_last;
  
  assign l_sel  = slv_reg9[TG_NUM-1+16:16];
  
  wire  [TG_NUM-1:0]  l_done, l_wc_err, l_wd_err, l_rc_err, l_rd_err;
  wire  [TG_NUM-1:0]  busy_flag;
  wire  [ 7:0]  lb_status[0:1];
  
	wire	[ 1:0]	lb8_buf_en;
	wire  [ 3:0]	lb8_buf_we[0:1];
	wire  [31:0]	lb8_buf_addr[0:1];
	wire  [31:0]	lb8_buf_datao[0:1];
	wire  [31:0]	lb8_buf_datai[0:1];
  
  assign lb_tg_buf_clk = buf_clk;
  assign lb_tg_buf_rst = buf_rst;
  
  always @ ( * ) begin
    if ( l_sel[0] == 1'b1 ) begin
      lb_tg_buf_en = lb8_buf_en[0];
      lb_tg_buf_we = lb8_buf_we[0];
      lb_tg_buf_addr = lb8_buf_addr[0];
      lb_tg_buf_datao = lb8_buf_datao[0];
    end
    else if ( l_sel[1] == 1'b1 ) begin
      lb_tg_buf_en = lb8_buf_en[1];
      lb_tg_buf_we = lb8_buf_we[1];
      lb_tg_buf_addr = lb8_buf_addr[1];
      lb_tg_buf_datao = lb8_buf_datao[1];
    end
    else begin
      lb_tg_buf_en = 1'b0;
      lb_tg_buf_we = 4'b0;
      lb_tg_buf_addr = 32'b0;
      lb_tg_buf_datao = 32'b0;
    end
  end
  
	generate
	  
	  for ( j=0; j<TG_NUM; j=j+1 ) begin
  	  assign l_bl[j]    = ( l_sel[j] == 1'b1 ) ? slv_reg9[15:0] : 16'b0;
      assign l_cmd[j]   = ( l_sel[j] == 1'b1 ) ? {6'b0, slv_reg9[25:24]} : 8'b0;
      assign l_addr[j]  = ( l_sel[j] == 1'b1 ) ? slv_reg8 : 32'b0;
      assign l_rl[j]    = ( l_sel[j] == 1'b1 ) ? slv_reg11[7:0] : 8'b0;
      
      assign lb8_buf_datai[j] = lb_tg_buf_datai;
      
      assign lb_status[j] = {l_wc_err[j], l_wd_err[j], l_rc_err[j], l_rd_err[j], l_done[j], 2'b00, busy_flag[j]};
      
      lbx8_c2p_master # ( 
        .C_S_BUF_DATA_WIDTH(32),
        .C_S_LB_DATA_WIDTH(8)
      ) u_lbx8_c2t_master (
        .clk      (buf_clk      ),
        .rstn     (lb8_rstn     ),
        
        .lb_addr  (l_addr[j]    ),
        .lb_bl    (l_bl[j]      ),
        .lb_cmd   (l_cmd[j]     ),
        
        .lb_error (l_error[j]   ),
        .lb_blast (l_last[j]    ),
        .lb_valid (l_valid[j]   ),
        .lb_rdata (l_rdata[j]   ),
        .lb_wr    (l_wr[j]      ),
        .lb_wdata (l_wdata[j]   ),
        
        .lb_rl    (l_rl[j]      ),
        
        .lb_clr   (1'b0         ),
        .lb_done  (l_done[j]    ),
        .lb_wc_err(l_wc_err[j]  ),
        .lb_wd_err(l_wd_err[j]  ),
        .lb_rc_err(l_rc_err[j]  ),
        .lb_rd_err(l_rd_err[j]  ),
        .busy_flag(busy_flag[j] ),
        
        .cur_st_o (),
        .rd_cnt_o (),
    
        .buf_en   (lb8_buf_en[j]    ),
        .buf_we   (lb8_buf_we[j]    ),
        .buf_addr (lb8_buf_addr[j]  ),
        .buf_datao(lb8_buf_datao[j] ),
        
        .buf_datai(lb8_buf_datai[j] )
      );
		end
		
	endgenerate
  // User logic ends
  
	// I/O Connections assignments

	assign S_AXI_AWREADY	= axi_awready;
	assign S_AXI_WREADY	= axi_wready;
	assign S_AXI_BRESP	= axi_bresp;
	assign S_AXI_BVALID	= axi_bvalid;
	assign S_AXI_ARREADY	= axi_arready;
	assign S_AXI_RDATA	= axi_rdata;
	assign S_AXI_RRESP	= axi_rresp;
	assign S_AXI_RVALID	= axi_rvalid;
	// Implement axi_awready generation
	// axi_awready is asserted for one S_AXI_ACLK clock cycle when both
	// S_AXI_AWVALID and S_AXI_WVALID are asserted. axi_awready is
	// de-asserted when reset is low.

	always @( posedge S_AXI_ACLK )
	begin
	  if ( S_AXI_ARESETN == 1'b0 )
	    begin
	      axi_awready <= 1'b0;
	      aw_en <= 1'b1;
	    end 
	  else
	    begin    
	      if (~axi_awready && S_AXI_AWVALID && S_AXI_WVALID && aw_en)
	        begin
	          // slave is ready to accept write address when 
	          // there is a valid write address and write data
	          // on the write address and data bus. This design 
	          // expects no outstanding transactions. 
	          axi_awready <= 1'b1;
	          aw_en <= 1'b0;
	        end
	        else if (S_AXI_BREADY && axi_bvalid)
	            begin
	              aw_en <= 1'b1;
	              axi_awready <= 1'b0;
	            end
	      else           
	        begin
	          axi_awready <= 1'b0;
	        end
	    end 
	end       

	// Implement axi_awaddr latching
	// This process is used to latch the address when both 
	// S_AXI_AWVALID and S_AXI_WVALID are valid. 

	always @( posedge S_AXI_ACLK )
	begin
	  if ( S_AXI_ARESETN == 1'b0 )
	    begin
	      axi_awaddr <= 0;
	    end 
	  else
	    begin    
	      if (~axi_awready && S_AXI_AWVALID && S_AXI_WVALID && aw_en)
	        begin
	          // Write Address latching 
	          axi_awaddr <= S_AXI_AWADDR;
	        end
	    end 
	end       

	// Implement axi_wready generation
	// axi_wready is asserted for one S_AXI_ACLK clock cycle when both
	// S_AXI_AWVALID and S_AXI_WVALID are asserted. axi_wready is 
	// de-asserted when reset is low. 

	always @( posedge S_AXI_ACLK )
	begin
	  if ( S_AXI_ARESETN == 1'b0 )
	    begin
	      axi_wready <= 1'b0;
	    end 
	  else
	    begin    
	      if (~axi_wready && S_AXI_WVALID && S_AXI_AWVALID && aw_en )
	        begin
	          // slave is ready to accept write data when 
	          // there is a valid write address and write data
	          // on the write address and data bus. This design 
	          // expects no outstanding transactions. 
	          axi_wready <= 1'b1;
	        end
	      else
	        begin
	          axi_wready <= 1'b0;
	        end
	    end 
	end       

	// Implement memory mapped register select and write logic generation
	// The write data is accepted and written to memory mapped registers when
	// axi_awready, S_AXI_WVALID, axi_wready and S_AXI_WVALID are asserted. Write strobes are used to
	// select byte enables of slave registers while writing.
	// These registers are cleared when reset (active low) is applied.
	// Slave register write enable is asserted when valid address and data are available
	// and the slave is ready to accept the write address and write data.
	assign slv_reg_wren = axi_wready && S_AXI_WVALID && axi_awready && S_AXI_AWVALID;

	always @( posedge S_AXI_ACLK )
	begin
	  if ( S_AXI_ARESETN == 1'b0 )
	    begin
	      slv_reg0 <= 0;
	      slv_reg1 <= 0;
	      slv_reg2 <= 0;
	      slv_reg3 <= 0;
	      slv_reg4 <= 0;
	      slv_reg5 <= 0;
	      slv_reg6 <= 0;
	      slv_reg7 <= 0;
	      slv_reg8 <= 0;
	      slv_reg9 <= 0;
	      slv_reg10 <= 0;
	      slv_reg11 <= 0;
	      slv_reg12 <= 0;
	      slv_reg13 <= 0;
	      slv_reg14 <= 0;
	      slv_reg15 <= 0;
	    end 
	  else begin
	    if (slv_reg_wren)
	      begin
	        case ( axi_awaddr[ADDR_LSB+OPT_MEM_ADDR_BITS:ADDR_LSB] )
	          4'h0:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 0
	                slv_reg0[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          4'h1:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 1
	                slv_reg1[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          4'h2:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 2
	                slv_reg2[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          4'h3:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 3
	                slv_reg3[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          4'h4:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 4
	                slv_reg4[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          4'h5:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 5
	                slv_reg5[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          4'h6:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 6
	                slv_reg6[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          4'h7:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 7
	                slv_reg7[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          4'h8:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 8
	                slv_reg8[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          4'h9:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 9
	                slv_reg9[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          4'hA:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 10
	                slv_reg10[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          4'hB:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 11
	                slv_reg11[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          4'hC:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 12
	                slv_reg12[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          4'hD:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 13
	                slv_reg13[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          4'hE:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 14
	                slv_reg14[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          4'hF:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 15
	                slv_reg15[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          default : begin
	                      slv_reg0 <= slv_reg0;
	                      slv_reg1 <= slv_reg1;
	                      slv_reg2 <= slv_reg2;
	                      slv_reg3 <= slv_reg3;
	                      slv_reg4 <= slv_reg4;
	                      slv_reg5 <= slv_reg5;
	                      slv_reg6 <= slv_reg6;
	                      slv_reg7 <= slv_reg7;
	                      slv_reg8 <= slv_reg8;
	                      slv_reg9 <= slv_reg9;
	                      slv_reg10 <= slv_reg10;
	                      slv_reg11 <= slv_reg11;
	                      slv_reg12 <= slv_reg12;
	                      slv_reg13 <= slv_reg13;
	                      slv_reg14 <= slv_reg14;
	                      slv_reg15 <= slv_reg15;
	                    end
	        endcase
	      end
	      
      // Add user logic here
      else begin
        slv_reg0 <= {IP_NAME[103:96], IP_NAME[111:104], IP_NAME[119:112], IP_NAME[127:120]};
        slv_reg1 <= {IP_NAME[71:64], IP_NAME[79:72], IP_NAME[87:80], IP_NAME[95:88]};
        slv_reg2 <= {IP_NAME[39:32], IP_NAME[47:40], IP_NAME[55:48], IP_NAME[63:56]};
        slv_reg3 <= {IP_NAME[ 7: 0], IP_NAME[15: 8], IP_NAME[23:16], IP_NAME[31:24]};
        slv_reg4 <= {16'b0, MAJOR_VER[7:0], MINOR_VER[7:0]};
        slv_reg5 <= {8'b0, YEAR[7:0], MONTH[7:0], DAY[7:0]};
        
        slv_reg12 <= {16'b0, lb_status[1], lb_status[0]};
        
        if ( busy_flag != 0 ) begin
          slv_reg9 <= slv_reg9 & 32'h00FF_FFFF;
        end
        
        if ( m_rst_clr == 1'b1 ) begin
          slv_reg9[31] <= 1'b0;
        end
      end
      // User logic ends
      
	  end
	end    

	// Implement write response logic generation
	// The write response and response valid signals are asserted by the slave 
	// when axi_wready, S_AXI_WVALID, axi_wready and S_AXI_WVALID are asserted.  
	// This marks the acceptance of address and indicates the status of 
	// write transaction.

	always @( posedge S_AXI_ACLK )
	begin
	  if ( S_AXI_ARESETN == 1'b0 )
	    begin
	      axi_bvalid  <= 0;
	      axi_bresp   <= 2'b0;
	    end 
	  else
	    begin    
	      if (axi_awready && S_AXI_AWVALID && ~axi_bvalid && axi_wready && S_AXI_WVALID)
	        begin
	          // indicates a valid write response is available
	          axi_bvalid <= 1'b1;
	          axi_bresp  <= 2'b0; // 'OKAY' response 
	        end                   // work error responses in future
	      else
	        begin
	          if (S_AXI_BREADY && axi_bvalid) 
	            //check if bready is asserted while bvalid is high) 
	            //(there is a possibility that bready is always asserted high)   
	            begin
	              axi_bvalid <= 1'b0; 
	            end  
	        end
	    end
	end   

	// Implement axi_arready generation
	// axi_arready is asserted for one S_AXI_ACLK clock cycle when
	// S_AXI_ARVALID is asserted. axi_awready is 
	// de-asserted when reset (active low) is asserted. 
	// The read address is also latched when S_AXI_ARVALID is 
	// asserted. axi_araddr is reset to zero on reset assertion.

	always @( posedge S_AXI_ACLK )
	begin
	  if ( S_AXI_ARESETN == 1'b0 )
	    begin
	      axi_arready <= 1'b0;
	      axi_araddr  <= 32'b0;
	    end 
	  else
	    begin    
	      if (~axi_arready && S_AXI_ARVALID)
	        begin
	          // indicates that the slave has acceped the valid read address
	          axi_arready <= 1'b1;
	          // Read address latching
	          axi_araddr  <= S_AXI_ARADDR;
	        end
	      else
	        begin
	          axi_arready <= 1'b0;
	        end
	    end 
	end       

	// Implement axi_arvalid generation
	// axi_rvalid is asserted for one S_AXI_ACLK clock cycle when both 
	// S_AXI_ARVALID and axi_arready are asserted. The slave registers 
	// data are available on the axi_rdata bus at this instance. The 
	// assertion of axi_rvalid marks the validity of read data on the 
	// bus and axi_rresp indicates the status of read transaction.axi_rvalid 
	// is deasserted on reset (active low). axi_rresp and axi_rdata are 
	// cleared to zero on reset (active low).  
	always @( posedge S_AXI_ACLK )
	begin
	  if ( S_AXI_ARESETN == 1'b0 )
	    begin
	      axi_rvalid <= 0;
	      axi_rresp  <= 0;
	    end 
	  else
	    begin    
	      if (axi_arready && S_AXI_ARVALID && ~axi_rvalid)
	        begin
	          // Valid read data is available at the read data bus
	          axi_rvalid <= 1'b1;
	          axi_rresp  <= 2'b0; // 'OKAY' response
	        end   
	      else if (axi_rvalid && S_AXI_RREADY)
	        begin
	          // Read data is accepted by the master
	          axi_rvalid <= 1'b0;
	        end                
	    end
	end    

	// Implement memory mapped register select and read logic generation
	// Slave register read enable is asserted when valid address is available
	// and the slave is ready to accept the read address.
	assign slv_reg_rden = axi_arready & S_AXI_ARVALID & ~axi_rvalid;
	always @(*)
	begin
	      // Address decoding for reading registers
	      case ( axi_araddr[ADDR_LSB+OPT_MEM_ADDR_BITS:ADDR_LSB] )
          4'h0   : reg_data_out <= slv_reg0;
          4'h1   : reg_data_out <= slv_reg1;
          4'h2   : reg_data_out <= slv_reg2;
          4'h3   : reg_data_out <= slv_reg3;
          4'h4   : reg_data_out <= slv_reg4;
          4'h5   : reg_data_out <= slv_reg5;
	        4'h6   : reg_data_out <= slv_reg6;
	        4'h7   : reg_data_out <= slv_reg7;
	        4'h8   : reg_data_out <= slv_reg8;
	        4'h9   : reg_data_out <= slv_reg9;
	        4'hA   : reg_data_out <= slv_reg10;
	        4'hB   : reg_data_out <= slv_reg11;
	        4'hC   : reg_data_out <= slv_reg12;
	        4'hD   : reg_data_out <= slv_reg13;
	        4'hE   : reg_data_out <= slv_reg14;
	        4'hF   : reg_data_out <= slv_reg15;
	        default : reg_data_out <= 0;
	      endcase
	end

	// Output register or memory read data
	always @( posedge S_AXI_ACLK )
	begin
	  if ( S_AXI_ARESETN == 1'b0 )
	    begin
	      axi_rdata  <= 0;
	    end 
	  else
	    begin    
	      // When there is a valid read address (S_AXI_ARVALID) with 
	      // acceptance of read address by the slave (axi_arready), 
	      // output the read dada 
	      if (slv_reg_rden)
	        begin
	          axi_rdata <= reg_data_out;     // register read data
	        end   
	    end
	end    

	// Add user logic here

	// User logic ends

	endmodule

  module lbx8_c2t_master #
  (
    // Width of BUF data bus
    parameter integer C_S_BUF_DATA_WIDTH      = 32,
    // Width of LB data bus
    parameter integer C_S_LB_DATA_WIDTH        = 8
  )
  (
    input   wire          clk,
    input   wire          rstn,
    
    input   wire  [31:0]  lb_addr,
    input   wire  [15:0]  lb_bl,
    input   wire  [ 7:0]  lb_cmd,
            
    input   wire          lb_error,
    input   wire          lb_blast,
    input   wire          lb_valid,
    output  reg           lb_wr,
    input   wire  [C_S_LB_DATA_WIDTH-1:0]  lb_rdata,
    output  reg   [C_S_LB_DATA_WIDTH-1:0]  lb_wdata,
    
    input   wire  [ 7:0]  lb_rl,
    
    input   wire          lb_clr,
    output  reg           lb_done,
    output  reg           lb_wc_err,
    output  reg           lb_wd_err,
    output  reg           lb_rc_err,
    output  reg           lb_rd_err,
    output  reg           busy_flag,
    
    output  wire  [ 3:0]  cur_st_o,
    output  wire  [ 3:0]  rd_cnt_o,
    
    output  reg           buf_en,
    output  reg   [ 3:0]  buf_we,
    output  reg   [31:0]  buf_addr,
    output  reg   [C_S_BUF_DATA_WIDTH-1:0]  buf_datao,
    input   wire  [C_S_BUF_DATA_WIDTH-1:0]  buf_datai
    );
    
    localparam IDLE_ST      = 4'h0;
    localparam OP_ST        = 4'h1;
    localparam BL_H_ST      = 4'h2;
    localparam BL_L_ST      = 4'h9;
    localparam ADDR_HH_ST    = 4'h3;
    localparam ADDR_HL_ST    = 4'h4;
    localparam ADDR_LH_ST    = 4'h5;
    localparam ADDR_LL_ST    = 4'h6;
    localparam CRC_OP_ST    = 4'h7;
    
    localparam WR_DATA_ST    = 4'h8;
    
    localparam RD_DATA_ST    = 4'hA;
    
    localparam CRC_DATA_ST  = 4'hE;
    localparam DONE_ST      = 4'hF;
    
    localparam OP_RD        = 8'hAD;
    localparam OP_WR        = 8'hBC;
    localparam OP_RDIMM        = 8'hAE;
    localparam OP_WDIMM        = 8'hBF;
    
    localparam CTRL_START    = 8'h5A;
    
    reg    [ 3:0]  cur_st, nxt_st;
        reg    [31:0]  wr_cnt, rd_cnt;
    reg    [31:0]  wr_idx;
    reg    [31:0]  rd_idx;
    
        reg            rd_st;
        reg            wr_st;
        reg            dimm_st;
            
    wire  [ 7:0]  lb_op;
    
    assign lb_op = ( rd_st == 1'b1 ) ? ( ( dimm_st == 1'b1 ) ? OP_RDIMM : OP_RD ) : 
                  ( ( wr_st == 1'b1 ) ? ( ( dimm_st == 1'b1 ) ? OP_WDIMM : OP_WR ) : 8'b0 );
                  
    assign cur_st_o = cur_st;
    assign rd_cnt_o = rd_cnt[3:0];
                  
    reg    [ 3:0]  l_blast_sr;
    reg            l_blast;
    
    wire    [15:0]  lb_bl_tmp;
    assign lb_bl_tmp = ( rd_st == 1'b1 ) ? ( lb_bl + {8'b0, lb_rl} ) : lb_bl;
    
    always @ (posedge clk) begin
      if ( rstn == 1'b0 ) begin
        l_blast_sr <= 4'b0;
        l_blast <= 1'b0;
      end
      else begin
        l_blast_sr <= {l_blast_sr[2:0], lb_blast};
        
        if ( cur_st == DONE_ST ) begin
          l_blast <= l_blast_sr[1];
        end
        else begin
          l_blast <= 1'b0;
        end
      end
    end

    reg    [ 3:0]  l_error_sr;
    reg            l_error;
    
    always @ ( posedge clk ) begin
      if ( rstn == 1'b0 ) begin
        l_error_sr <= 4'b0;
        l_error <= 1'b0;
      end
      else begin
        l_error_sr <= {l_error_sr[2:0], lb_error};
        l_error <= l_error_sr[1] && ( ~l_error_sr[3] );
      end
    end
    
    reg            st_start, st_start_p1;
    
    always @ ( posedge clk ) begin
      if ( rstn == 1'b0 ) begin
        st_start_p1 <= 1'b0;
        st_start <= 1'b0;
      end else begin
        if ( cur_st == IDLE_ST ) begin
          st_start_p1 <= ( |lb_cmd[3:0] );
          st_start <= st_start_p1;
        end
        else begin
          st_start_p1 <= 1'b0;
          st_start <= 1'b0;
        end
      end
    end
    
    always @ ( posedge clk ) begin
      if ( rstn == 1'b0 )
        cur_st <= IDLE_ST;
      else
        cur_st <= nxt_st;
    end
    
    always @ ( * ) begin
      case ( cur_st )
        IDLE_ST : begin
          if ( st_start == 1'b1 )
            nxt_st = OP_ST;
          else
            nxt_st = IDLE_ST;
        end
        
        OP_ST : nxt_st = BL_H_ST;
        
        BL_H_ST : nxt_st = BL_L_ST;
        BL_L_ST : nxt_st = ADDR_HH_ST;
        
        ADDR_HH_ST : nxt_st = ADDR_HL_ST;
        ADDR_HL_ST : nxt_st = ADDR_LH_ST;
        ADDR_LH_ST : nxt_st = ADDR_LL_ST;
        ADDR_LL_ST : nxt_st = CRC_OP_ST;
        
        CRC_OP_ST : begin
          if ( wr_st )
            nxt_st = WR_DATA_ST;
          else if ( rd_st )
            nxt_st = RD_DATA_ST;
          else
            nxt_st = IDLE_ST;
        end
        
        WR_DATA_ST : begin
          if ( l_error != 1 ) begin
            if ( wr_cnt == 1 )
              nxt_st = CRC_DATA_ST;
            else
              nxt_st = WR_DATA_ST;
          end
          else
            nxt_st = IDLE_ST;
        end

        RD_DATA_ST : begin
          if ( l_error != 1 ) begin
            if ( ( rd_cnt == 0 ) && ( lb_valid == 1'b1 ) )
              nxt_st = CRC_DATA_ST;
            else
              nxt_st = RD_DATA_ST;
          end
          else
            nxt_st = IDLE_ST;
        end
        
        CRC_DATA_ST : begin
          nxt_st = DONE_ST;
        end
        
        DONE_ST :  begin
          if ( ( l_blast == 1'b1 ) || ( l_error == 1'b1 ) )
            nxt_st = IDLE_ST;
          else
            nxt_st = DONE_ST;
        end
        default : nxt_st = IDLE_ST;
      endcase
    end
    
    reg    [C_S_LB_DATA_WIDTH-1:0]  crc_data;
    reg    [ 1:0]  hw_cnt;
    
    always @ ( posedge clk ) begin
      if ( rstn == 1'b0 ) begin
        rd_st <= 1'b0;
        wr_st <= 1'b0;
        dimm_st <= 1'b0;
        lb_wr <= 1'b0;
        lb_wdata <= 8'b0;
        lb_done <= 1'b0;
        wr_cnt <= 32'b0;
        rd_cnt <= 32'b0;
        wr_idx <= 32'b0;
        rd_idx <= 32'b0;
        hw_cnt <= 2'b0;
        crc_data <= 8'b0;
        lb_wc_err <= 1'b0;
        lb_wd_err <= 1'b0;
        lb_rc_err <= 1'b0;
        lb_rd_err <= 1'b0;
        busy_flag <= 1'b0;
        
        buf_en <= 1'b0;
        buf_we <= 4'b0;
        buf_addr <= 32'b0;
        buf_datao <= 32'b0;
      end
      else begin
        case ( cur_st )
          IDLE_ST : begin
            rd_st <= lb_cmd[0];
            wr_st <= lb_cmd[1];
            dimm_st <= lb_cmd[4];
        
            lb_wr <= 1'b0;
            lb_wdata <= 8'b0;
            wr_cnt <= 0;
            rd_cnt <= 0;
            wr_idx <= 32'b0;
            rd_idx <= 32'b0;
            hw_cnt <= 2'b0;
            crc_data <= 8'b0;
            if ( lb_clr == 1'b1 ) begin
              lb_done <= 1'b0;
              lb_wc_err <= 1'b0;
              lb_wd_err <= 1'b0;
              lb_rc_err <= 1'b0;
              lb_rd_err <= 1'b0;
            end
            else begin
              lb_done <= lb_done;
              lb_wc_err <= lb_wc_err;
              lb_wd_err <= lb_wd_err;
              lb_rc_err <= lb_rc_err;
              lb_rd_err <= lb_rd_err;
            end
            busy_flag <= 1'b0;
            
            buf_en <= 1'b0;
            buf_we <= 4'b0;
            buf_addr <= 32'b0;
            buf_datao <= 32'b0;
          end
          
          OP_ST : begin
            lb_wr <= 1'b1;
            lb_wdata <= lb_op;
            lb_done <= 1'b0;
            lb_wc_err <= 1'b0;
            lb_wd_err <= 1'b0;
            lb_rc_err <= 1'b0;
            lb_rd_err <= 1'b0;
            busy_flag <= 1'b1;
            
            if ( wr_st ) begin
              wr_cnt <= lb_bl + 1'b1;
              rd_cnt <= 0;
            end
            else if ( rd_st ) begin
              wr_cnt <= 0;
              rd_cnt <= lb_bl + {8'b0, lb_rl};
            end
            else begin
              wr_cnt <= 0;
              rd_cnt <= 0;
            end
            
            wr_idx <= 32'b0;
            rd_idx <= 32'b0;
            crc_data <= lb_op;
          end
          
          BL_H_ST : begin
            lb_wr <= 1'b1;
            lb_wdata <= lb_bl_tmp[15:8];                
            wr_cnt <= wr_cnt;
            rd_cnt <= rd_cnt;
            wr_idx <= 32'b0;
            rd_idx <= 32'b0;
            crc_data <= crc_data ^ lb_bl_tmp[15:8];
          end
          BL_L_ST : begin
            lb_wr <= 1'b1;
            lb_wdata <= lb_bl_tmp[7:0];                
            wr_cnt <= wr_cnt;
            rd_cnt <= rd_cnt;
            wr_idx <= 32'b0;
            rd_idx <= 32'b0;
            crc_data <= crc_data ^ lb_bl_tmp[7:0];
          end
          
          ADDR_HH_ST : begin
            lb_wr <= 1'b1;
            lb_wdata <= lb_addr[31:24];
            wr_cnt <= wr_cnt;
            rd_cnt <= rd_cnt;
            wr_idx <= 32'b0;
            rd_idx <= 32'b0;
            crc_data <= crc_data ^ lb_addr[31:24];
          end
          ADDR_HL_ST : begin
            lb_wr <= 1'b1;
            lb_wdata <= lb_addr[23:16];
            wr_cnt <= wr_cnt;
            rd_cnt <= rd_cnt;
            wr_idx <= 32'b0;
            rd_idx <= 32'b0;
            crc_data <= crc_data ^ lb_addr[23:16];
          end
          ADDR_LH_ST : begin
            lb_wr <= 1'b1;
            lb_wdata <= lb_addr[15:8];
            wr_cnt <= wr_cnt;
            rd_cnt <= rd_cnt;
            wr_idx <= 32'b0;
            rd_idx <= 32'b0;
            crc_data <= crc_data ^ lb_addr[15:8];
          end
          ADDR_LL_ST : begin
            lb_wr <= 1'b1;
            lb_wdata <= lb_addr[7:0];
            wr_cnt <= wr_cnt;
            rd_cnt <= rd_cnt;
            wr_idx <= 32'b0;
            rd_idx <= 32'b0;
            crc_data <= crc_data ^ lb_addr[7:0];
            
            buf_en <= 1'b1;
            buf_we <= 4'b0;
            buf_addr <= 32'b0;
          end
          
          CRC_OP_ST : begin
            lb_wr <= 1'b1;
            lb_wdata <= crc_data;
            wr_cnt <= wr_cnt;
            rd_cnt <= rd_cnt;
            wr_idx <= 32'b0;
            rd_idx <= 32'b0;
            crc_data <= crc_data;
          end
          
          WR_DATA_ST : begin
            lb_wr <= 1'b1;
            lb_wc_err <= l_error;
            
            case ( hw_cnt )
              2'b00 : begin lb_wdata <= buf_datai[7:0];  crc_data <= crc_data ^ buf_datai[7:0]; end
              2'b01 : begin lb_wdata <= buf_datai[15:8]; crc_data <= crc_data ^ buf_datai[15:8]; end
              2'b10 : begin lb_wdata <= buf_datai[23:16]; crc_data <= crc_data ^ buf_datai[23:16]; end
              2'b11 : begin lb_wdata <= buf_datai[31:24]; crc_data <= crc_data ^ buf_datai[31:24]; end
              default : begin lb_wdata <= lb_wdata; crc_data <= crc_data; end
            endcase
            
            wr_cnt <= wr_cnt - 16'b1;
            hw_cnt <= hw_cnt + 2'b1;
            
            buf_en <= 1'b1;
            buf_we <= 4'b0;
            buf_addr <= wr_idx;
            
            if ( hw_cnt == 2'b01 ) begin
              wr_idx <= wr_idx + 16'h4;
            end
          end      
          
          RD_DATA_ST : begin
            lb_wr <= 1'b0;
            lb_wdata <= 8'b0;
            lb_done <= 1'b0;
            
            lb_rc_err <= l_error;
            
            wr_cnt <= 32'b0;
            wr_idx <= 32'b0;
            
            if ( lb_valid == 1'b1 ) begin
              rd_cnt <= rd_cnt - 16'b1;
              crc_data <= crc_data ^ lb_rdata;
              
              if ( rd_cnt <= lb_bl ) begin
                  hw_cnt <= hw_cnt + 2'b1;
                  
                  buf_en <= 1'b1;
                  buf_addr <= rd_idx;
                  case ( hw_cnt )
                    2'b00 : begin buf_we <= 4'h1; buf_datao <= {24'b0, lb_rdata}; end
                    2'b01 : begin buf_we <= 4'h2; buf_datao <= {16'b0, lb_rdata, 8'b0}; end
                    2'b10 : begin buf_we <= 4'h4; buf_datao <= {8'b0, lb_rdata, 16'b0}; end
                    2'b11 : begin buf_we <= 4'h8; buf_datao <= {lb_rdata, 24'b0}; end
                    default : begin buf_we <= buf_we; buf_datao <= buf_datao; end
                  endcase
                  
                  if ( hw_cnt == 2'b11 ) begin
                    rd_idx <= rd_idx + 16'h4;
                  end
                end
            end
            else begin
              rd_idx <= rd_idx;
              rd_cnt <= rd_cnt;
              
              buf_en <= 1'b0;
              buf_we <= 4'b0;
            end
          end
          
          CRC_DATA_ST : begin
            buf_en <= 1'b0;
            buf_we <= 4'b0;
            if ( wr_st ) begin
              lb_wr <= 1'b1;
              lb_wdata <= crc_data;
            end
            else if ( rd_st ) begin
              if ( crc_data != lb_rdata )
                lb_rd_err <= 1'b1;
            end
          end
          
          DONE_ST : begin
            lb_wr <= 1'b0;
            lb_wdata <= 8'b0;
            
            lb_wd_err <= l_error && wr_st;
            
            if ( ( wr_st ) && ( l_blast == 1'b1 ) )
              lb_done <= 1'b1;
            else if ( rd_st )
              lb_done <= 1'b1;
            else
              lb_done <= 1'b0;
              
            wr_cnt <= 32'b0;
            rd_cnt <= 32'b0;
            wr_idx <= 32'b0;
            rd_idx <= 32'b0;
            hw_cnt <= 2'b0;
            crc_data <= 8'b0;
            
            busy_flag <= 1'b1;
            
            buf_en <= 1'b0;
            buf_we <= 4'b0;
            buf_addr <= 32'b0;
            buf_datao <= 32'b0;
          end
          default : begin
            lb_wr <= 1'b0;
            lb_wdata <= 8'b0;
            lb_done <= 1'b0;
            wr_cnt <= 32'b0;
            rd_cnt <= 32'b0;
            wr_idx <= 32'b0;
            rd_idx <= 32'b0;
            hw_cnt <= 2'b0;
            crc_data <= 8'b0;
            lb_wc_err <= 1'b0;
            lb_wd_err <= 1'b0;
            lb_rc_err <= 1'b0;
            lb_rd_err <= 1'b0;
            busy_flag <= 1'b0;
            
            buf_en <= 1'b0;
            buf_we <= 4'b0;
            buf_addr <= 32'b0;
            buf_datao <= 32'b0;
          end
        endcase
      end
    end
    
  endmodule
  