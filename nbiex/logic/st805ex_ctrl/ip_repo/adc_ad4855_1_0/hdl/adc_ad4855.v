
`timescale 1 ns / 1 ps

	module adc_ad4855 #
	(
		// Users to add parameters here

		// User parameters ends
		// Do not modify the parameters beyond this line


		// Parameters of Axi Slave Bus Interface S00_AXI
		parameter integer C_S00_AXI_DATA_WIDTH	= 32,
		parameter integer C_S00_AXI_ADDR_WIDTH	= 10
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
	adc_ad4855_slave_lite_v1_0_S00_AXI # ( 
		.C_S_AXI_DATA_WIDTH(C_S00_AXI_DATA_WIDTH),
		.C_S_AXI_ADDR_WIDTH(C_S00_AXI_ADDR_WIDTH)
	) adc_ad4855_slave_lite_v1_0_S00_AXI_inst (
		// Users to add ports here
		.adc_pd     (adc_pd     ),
		
    .adc_csn    (adc_csn    ),
    .adc_csck   (adc_csck   ),
    .adc_csdio  (adc_csdio  ),
    .adc_csdo   (adc_csdo   ),
    
    .adc_busy   (adc_busy   ),
    .adc_cnv    (adc_cnv    ),
    .adc_scki   (adc_scki   ),
    .adc_scko   (adc_scko   ),
    .adc_sdo0   (adc_sdo0   ),
    .adc_sdo1   (adc_sdo1   ),
    .adc_sdo2   (adc_sdo2   ),
    .adc_sdo3   (adc_sdo3   ),
    .adc_sdo4   (adc_sdo4   ),
    .adc_sdo5   (adc_sdo5   ),
    .adc_sdo6   (adc_sdo6   ),
    .adc_sdo7   (adc_sdo7   ),
    
    .mux_a      (mux_a      ),
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
