
`timescale 1 ns / 1 ps

	module lbx8_c2p_v1_0 #
	(
		// Users to add parameters here
		parameter integer PG_NUM  = 1,
		// User parameters ends
		// Do not modify the parameters beyond this line


		// Parameters of Axi Slave Bus Interface S00_AXI
		parameter integer C_S00_AXI_DATA_WIDTH	= 32,
		parameter integer C_S00_AXI_ADDR_WIDTH	= 6
	)
	(
		// Users to add ports here
		input   wire          clk200,
		
    output  wire          lb_pg_buf_clk,
    output  wire          lb_pg_buf_rst,
    output  wire          lb_pg_buf_en,
    output  wire  [ 3:0]  lb_pg_buf_we,
    output  wire  [31:0]  lb_pg_buf_addr,
    input   wire  [31:0]  lb_pg_buf_datai,
    output  wire  [31:0]  lb_pg_buf_datao,
    
    output  wire          ddr_en,
    output  wire  [ 1:0]  ddr_msb,
    
    input   wire  [PG_NUM-1:0]    pg_lb_valid,
    input   wire  [PG_NUM-1:0]    pg_lb_last,
    input   wire  [PG_NUM-1:0]    pg_lb_error,
    output  wire  [PG_NUM-1:0]    pg_lb_clk,
    output  wire  [PG_NUM-1:0]    pg_lb_rstn,
    output  wire  [PG_NUM-1:0]    pg_lb_wr,
    inout   wire  [PG_NUM*8-1:0]  pg_lb_ad,
		// User ports ends
		// Do not modify the ports beyond this line


		// Ports of Axi Slave Bus Interface S00_AXI
		input wire  s00_axi_aclk,
		input wire  s00_axi_aresetn,
		input wire [C_S00_AXI_ADDR_WIDTH-1 : 0] s00_axi_awaddr,
		input wire [2 : 0] s00_axi_awprot,
		input wire  s00_axi_awvalid,
		output wire  s00_axi_awready,
		input wire [C_S00_AXI_DATA_WIDTH-1 : 0] s00_axi_wdata,
		input wire [(C_S00_AXI_DATA_WIDTH/8)-1 : 0] s00_axi_wstrb,
		input wire  s00_axi_wvalid,
		output wire  s00_axi_wready,
		output wire [1 : 0] s00_axi_bresp,
		output wire  s00_axi_bvalid,
		input wire  s00_axi_bready,
		input wire [C_S00_AXI_ADDR_WIDTH-1 : 0] s00_axi_araddr,
		input wire [2 : 0] s00_axi_arprot,
		input wire  s00_axi_arvalid,
		output wire  s00_axi_arready,
		output wire [C_S00_AXI_DATA_WIDTH-1 : 0] s00_axi_rdata,
		output wire [1 : 0] s00_axi_rresp,
		output wire  s00_axi_rvalid,
		input wire  s00_axi_rready
	);
// Instantiation of Axi Bus Interface S00_AXI
	lbx8_c2p_v1_0_S00_AXI # ( 
		// Users to add parameters here
		.PG_NUM (PG_NUM ),
		// User parameters ends
		.C_S_AXI_DATA_WIDTH(C_S00_AXI_DATA_WIDTH),
		.C_S_AXI_ADDR_WIDTH(C_S00_AXI_ADDR_WIDTH)
	) lbx8_c2p_v1_0_S00_AXI_inst (
		// Users to add ports here
		.clk200           (clk200           ),
		
    .lb_pg_buf_clk    (lb_pg_buf_clk    ),
    .lb_pg_buf_rst    (lb_pg_buf_rst    ),
    .lb_pg_buf_en     (lb_pg_buf_en     ),
    .lb_pg_buf_we     (lb_pg_buf_we     ),
    .lb_pg_buf_addr   (lb_pg_buf_addr   ),
    .lb_pg_buf_datai  (lb_pg_buf_datai  ),
    .lb_pg_buf_datao  (lb_pg_buf_datao  ),
    
    .ddr_en           (ddr_en           ),
    .ddr_msb          (ddr_msb          ),
    
    .pg_lb_valid      (pg_lb_valid      ),
    .pg_lb_last       (pg_lb_last       ),
    .pg_lb_error      (pg_lb_error      ),
    .pg_lb_clk        (pg_lb_clk        ),
    .pg_lb_rstn       (pg_lb_rstn       ),
    .pg_lb_wr         (pg_lb_wr         ),
    .pg_lb_ad         (pg_lb_ad         ),
		// User ports ends
		.S_AXI_ACLK(s00_axi_aclk),
		.S_AXI_ARESETN(s00_axi_aresetn),
		.S_AXI_AWADDR(s00_axi_awaddr),
		.S_AXI_AWPROT(s00_axi_awprot),
		.S_AXI_AWVALID(s00_axi_awvalid),
		.S_AXI_AWREADY(s00_axi_awready),
		.S_AXI_WDATA(s00_axi_wdata),
		.S_AXI_WSTRB(s00_axi_wstrb),
		.S_AXI_WVALID(s00_axi_wvalid),
		.S_AXI_WREADY(s00_axi_wready),
		.S_AXI_BRESP(s00_axi_bresp),
		.S_AXI_BVALID(s00_axi_bvalid),
		.S_AXI_BREADY(s00_axi_bready),
		.S_AXI_ARADDR(s00_axi_araddr),
		.S_AXI_ARPROT(s00_axi_arprot),
		.S_AXI_ARVALID(s00_axi_arvalid),
		.S_AXI_ARREADY(s00_axi_arready),
		.S_AXI_RDATA(s00_axi_rdata),
		.S_AXI_RRESP(s00_axi_rresp),
		.S_AXI_RVALID(s00_axi_rvalid),
		.S_AXI_RREADY(s00_axi_rready)
	);

	// Add user logic here

	// User logic ends

	endmodule
