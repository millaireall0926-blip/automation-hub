
`timescale 1 ns / 1 ps

	module cfgx8_c2x_v1_0 #
	(
		// Users to add parameters here
		parameter integer TG_NUM  = 1,
		parameter integer PG_NUM  = 1,
		// User parameters ends
		// Do not modify the parameters beyond this line


		// Parameters of Axi Slave Bus Interface S00_AXI
		parameter integer C_S00_AXI_DATA_WIDTH	= 32,
		parameter integer C_S00_AXI_ADDR_WIDTH	= 6
	)
	(
		// Users to add ports here
		output  wire          all_cfg_done,
		
		output  wire          cfg_buf_clk,
		output  wire          cfg_buf_rst,
		output  wire          cfg_buf_en,
		output  wire  [ 3:0]  cfg_buf_we,
		output  wire  [31:0]  cfg_buf_addr,
		input   wire  [31:0]  cfg_buf_datai,
		output  wire  [31:0]  cfg_buf_datao,
		
		input   wire  [TG_NUM-1:0]    tg_cfg_init_b,
		input   wire  [TG_NUM-1:0]    tg_cfg_done,
		output  wire  [TG_NUM-1:0]    tg_cfg_csi_b,
		output  wire  [TG_NUM-1:0]    tg_cfg_cclk,
		output  wire  [TG_NUM-1:0]    tg_cfg_pgm_b,
		output  wire  [TG_NUM-1:0]    tg_cfg_rdwr_b,
		output  wire  [TG_NUM*8-1:0]  tg_cfg_data,
		
		input   wire  [PG_NUM-1:0]    pg_cfg_init_b,
		input   wire  [PG_NUM-1:0]    pg_cfg_done,
		output  wire  [PG_NUM-1:0]    pg_cfg_csi_b,
		output  wire  [PG_NUM-1:0]    pg_cfg_cclk,
		output  wire  [PG_NUM-1:0]    pg_cfg_pgm_b,
		output  wire  [PG_NUM-1:0]    pg_cfg_rdwr_b,
		output  wire  [PG_NUM*8-1:0]  pg_cfg_data,
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
	cfgx8_c2x_v1_0_S00_AXI # (
		// Users to add parameters here
		.TG_NUM (TG_NUM ),
		.PG_NUM (PG_NUM ),
		// User parameters ends
		.C_S_AXI_DATA_WIDTH(C_S00_AXI_DATA_WIDTH),
		.C_S_AXI_ADDR_WIDTH(C_S00_AXI_ADDR_WIDTH)
	) cfgx8_c2x_v1_0_S00_AXI_inst (
		// Users to add ports here
		.all_cfg_done   (all_cfg_done   ),
		
		.cfg_buf_clk    (cfg_buf_clk    ),
		.cfg_buf_rst    (cfg_buf_rst    ),
		.cfg_buf_en     (cfg_buf_en     ),
		.cfg_buf_we     (cfg_buf_we     ),
		.cfg_buf_addr   (cfg_buf_addr   ),
		.cfg_buf_datai  (cfg_buf_datai  ),
		.cfg_buf_datao  (cfg_buf_datao  ),
		
		.tg_cfg_init_b  (tg_cfg_init_b  ),
		.tg_cfg_done    (tg_cfg_done    ),
		.tg_cfg_csi_b   (tg_cfg_csi_b   ),
		.tg_cfg_cclk    (tg_cfg_cclk    ),
		.tg_cfg_pgm_b   (tg_cfg_pgm_b   ),
		.tg_cfg_rdwr_b  (tg_cfg_rdwr_b  ),
		.tg_cfg_data    (tg_cfg_data    ),
		
		.pg_cfg_init_b  (pg_cfg_init_b  ),
		.pg_cfg_done    (pg_cfg_done    ),
		.pg_cfg_csi_b   (pg_cfg_csi_b   ),
		.pg_cfg_cclk    (pg_cfg_cclk    ),
		.pg_cfg_pgm_b   (pg_cfg_pgm_b   ),
		.pg_cfg_rdwr_b  (pg_cfg_rdwr_b  ),
		.pg_cfg_data    (pg_cfg_data    ),
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
