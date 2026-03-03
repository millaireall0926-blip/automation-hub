
// (* dont_touch = "true" *)	(* mark_debug = "true" *)

`timescale 1 ns / 1 ps

  module cfgx8_c2x_v1_0_S00_AXI #
  (
    // Users to add parameters here
    parameter IP_NAME           = "CFGx8-C2X       ",
    parameter integer MAJOR_VER = 00,
    parameter integer MINOR_VER = 62,
    parameter integer YEAR      = 24,
    parameter integer MONTH     = 02,
    parameter integer DAY       = 07,
    
    parameter integer TG_NUM    = 1,
    parameter integer PG_NUM    = 1,
    // User parameters ends
    // Do not modify the parameters beyond this line

    // Width of S_AXI data bus
    parameter integer C_S_AXI_DATA_WIDTH  = 32,
    // Width of S_AXI address bus
    parameter integer C_S_AXI_ADDR_WIDTH  = 6
  )
  (
    // Users to add ports here
    output  wire          all_cfg_done,
    
    output  wire          cfg_buf_clk,
    output  wire          cfg_buf_rst,
    output  reg           cfg_buf_en,
    output  wire  [ 3:0]  cfg_buf_we,
    output  reg   [31:0]  cfg_buf_addr,
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
  reg [C_S_AXI_ADDR_WIDTH-1 : 0]   axi_awaddr;
  reg    axi_awready;
  reg    axi_wready;
  reg [1 : 0]   axi_bresp;
  reg    axi_bvalid;
  reg [C_S_AXI_ADDR_WIDTH-1 : 0]   axi_araddr;
  reg    axi_arready;
  reg [C_S_AXI_DATA_WIDTH-1 : 0]   axi_rdata;
  reg [1 : 0]   axi_rresp;
  reg    axi_rvalid;

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
  reg [C_S_AXI_DATA_WIDTH-1:0]  slv_reg0;
  reg [C_S_AXI_DATA_WIDTH-1:0]  slv_reg1;
  reg [C_S_AXI_DATA_WIDTH-1:0]  slv_reg2;
  reg [C_S_AXI_DATA_WIDTH-1:0]  slv_reg3;
  reg [C_S_AXI_DATA_WIDTH-1:0]  slv_reg4;
  reg [C_S_AXI_DATA_WIDTH-1:0]  slv_reg5;
  reg [C_S_AXI_DATA_WIDTH-1:0]  slv_reg6;
  reg [C_S_AXI_DATA_WIDTH-1:0]  slv_reg7;
  reg [C_S_AXI_DATA_WIDTH-1:0]  slv_reg8;
  reg [C_S_AXI_DATA_WIDTH-1:0]  slv_reg9;
  reg [C_S_AXI_DATA_WIDTH-1:0]  slv_reg10;
  reg [C_S_AXI_DATA_WIDTH-1:0]  slv_reg11;
  reg [C_S_AXI_DATA_WIDTH-1:0]  slv_reg12;
  reg [C_S_AXI_DATA_WIDTH-1:0]  slv_reg13;
  reg [C_S_AXI_DATA_WIDTH-1:0]  slv_reg14;
  reg [C_S_AXI_DATA_WIDTH-1:0]  slv_reg15;
  wire   slv_reg_rden;
  wire   slv_reg_wren;
  reg [C_S_AXI_DATA_WIDTH-1:0]   reg_data_out;
  integer   byte_index;
  reg   aw_en;

  // Add user logic here
	reg           cfg_init_b;
	reg				    cfg_done;
  
  localparam tPROGRAM  = 5000;    // Over than 250ns
  
  assign cfg_buf_clk = S_AXI_ACLK;
  assign cfg_buf_rst = ~S_AXI_ARESETN;
  assign cfg_buf_we = 4'b0;
  assign cfg_buf_datao = 32'b0;
  
  reg           init_err_flag;
  reg           done_err_flag;
  reg           pgm_done_flag;
  reg           pwait_done_flag;
  reg           iwait_done_flag;
  reg           eos_done_flag;
  reg           cfg_done_flag;
  reg           busy_flag;
  
  reg           cfg_start;
  reg           cfg_last;
  reg   [15:0]  cfg_bl;
  
  wire          ip_rst;
  wire          ip_rst_clr;
  reg   [ 7:0]  ip_rst_sr;
  wire          ip_rstn;
  
  assign ip_rst = slv_reg8[31];
  
  always @ ( posedge cfg_buf_clk ) begin
    if ( cfg_buf_rst == 1'b1 ) begin
      ip_rst_sr <= 8'b0;
    end
    else begin
      ip_rst_sr[7:0] <= {ip_rst_sr[6:0], ip_rst};
    end
  end
  
  assign ip_rst_clr = ip_rst_sr[4];
  
  assign ip_rstn = ~( ip_rst && ( ~ip_rst_sr[4] ) );

  wire          cfg_rstn;
  assign cfg_rstn = S_AXI_ARESETN && ip_rstn;
  
  wire  [31:0]  cfg_cmd;
  
  assign cfg_cmd = slv_reg8;
  
  localparam IDLE_ST    = 4'h0;
  localparam PGM_ST     = 4'h1;
  localparam INIT_ST    = 4'h2;
  localparam DATA_ST    = 4'h3;
  localparam CLK_H_ST   = 4'h4;
  localparam CLK_L_ST   = 4'h5;
  localparam CLK_WAIT_ST= 4'h6;
  localparam UPDATE_ST  = 4'h8;
  
  localparam PGM_WAIT_ST = 4'h7;
  localparam INIT_WAIT_ST= 4'h9;
  
  localparam WAIT_ST    = 4'hA;
  localparam LAST_ST    = 4'hB;
  localparam HOLD_ST    = 4'hC;
  localparam EOS_ST     = 4'hD;
  localparam ERROR_ST   = 4'hE;
  
  localparam DONE_ST    = 4'hF;

(* dont_touch = "true" *) reg   [ 3:0]  cur_st, nxt_st;
(* dont_touch = "true" *) reg   [31:0]  st_last_cnt;
(* dont_touch = "true" *) reg           cclk;
 reg           pgm_b;
 reg           rdwr_b;
 reg           csi_b;
 reg   [ 7:0]  datao;
  
  always @ ( posedge cfg_buf_clk ) begin
    if ( cfg_rstn == 1'b0 )
      cur_st <= IDLE_ST;
    else
      cur_st <= nxt_st;
  end
  
  always @ ( * ) begin
    case ( cur_st )
      IDLE_ST : begin
        if ( cfg_start == 1'b1 )
          nxt_st = PGM_ST;
        else
          nxt_st = IDLE_ST;
      end
      PGM_ST : begin
        if ( pgm_done_flag == 1'b1 )
          nxt_st = PGM_WAIT_ST;
        else
          nxt_st = PGM_ST;
      end
      PGM_WAIT_ST : begin
        if ( pwait_done_flag == 1'b1 )
          nxt_st = INIT_ST;
        else
          nxt_st = PGM_WAIT_ST;
      end
      INIT_ST : begin
        if ( cfg_init_b == 1'b1 ) begin
          if ( cfg_start == 1'b1 )
            nxt_st = INIT_WAIT_ST;
          else
            nxt_st = INIT_ST;
        end
        else
          nxt_st = INIT_ST;
      end
      INIT_WAIT_ST : begin
        if ( iwait_done_flag == 1'b1 )
          nxt_st = DATA_ST;
        else
          nxt_st = INIT_WAIT_ST;
      end
      DATA_ST : begin
        if ( cfg_init_b == 1'b1 )
          nxt_st = CLK_H_ST;
        else
          nxt_st = ERROR_ST;
      end
      CLK_H_ST : begin
        /*if ( clk_h_flag == 1'b1 )    // remove due to CCLK( 16.6MHz -> 33.3MHz )
          nxt_st = CLK_L_ST;
        else
          nxt_st = CLK_H_ST;*/
        nxt_st = CLK_WAIT_ST;
      end
      CLK_WAIT_ST : begin
        nxt_st = CLK_L_ST;
      end
      CLK_L_ST : begin
        /*if ( buf_addr == ( {16'b0, cfg_bl} + 32'b1 ) )    // remove due to CCLK( 16.6MHz -> 33.3MHz )
          nxt_st = UPDATE_ST;
        else if ( clk_l_flag == 1'b1 )
          nxt_st = DATA_ST;
        else
          nxt_st = CLK_L_ST;*/
        if ( cfg_buf_addr == ( {16'b0, cfg_bl} + 32'b1 ) )
          nxt_st = UPDATE_ST;
        else
          nxt_st = DATA_ST;
      end
      UPDATE_ST : begin
        if ( cfg_last == 1'b1 )
          nxt_st = LAST_ST;
        else
          nxt_st = WAIT_ST;
      end
      WAIT_ST : begin
        if ( cfg_init_b == 1'b1 ) begin
          if ( ( cfg_start == 1'b1 ) || ( cfg_last == 1'b1 ) )
            nxt_st = DATA_ST;
          else
            nxt_st = WAIT_ST;
        end
        else
          nxt_st = ERROR_ST;
      end
      LAST_ST : begin
        if ( cfg_init_b == 1'b1 ) begin
          if ( done_err_flag == 1'b1 )
            nxt_st = IDLE_ST;
          else if ( cfg_done == 1'b1 )
            nxt_st = HOLD_ST;
          else
            nxt_st = LAST_ST;
        end
        else
          nxt_st = ERROR_ST;
      end
      HOLD_ST : nxt_st = EOS_ST;
      EOS_ST : begin
        if ( cfg_init_b == 1'b1 ) begin
          if ( eos_done_flag == 1'b1 )
            nxt_st = DONE_ST;
          else
            nxt_st = EOS_ST;
        end
        else
          nxt_st = ERROR_ST;
      end
      ERROR_ST : begin
        nxt_st = IDLE_ST;
      end
      DONE_ST : nxt_st = IDLE_ST;
      default : nxt_st = IDLE_ST;
    endcase
  end
  
  always @ ( posedge cfg_buf_clk ) begin
    if ( cfg_rstn == 1'b0 ) begin
      pgm_b <= 1'b1;
      rdwr_b <= 1'b0;
      csi_b <= 1'b1;
      cclk <= 1'b0;
      datao <= 8'b0;
      
      cfg_buf_en <= 1'b0;
      cfg_buf_addr <= 32'b0;

      cfg_last <= 1'b0;
      cfg_start <= 1'b0;
      cfg_bl <= 16'b0;
      
      //clk_h_flag <= 1'b0;
      //clk_l_flag <= 1'b0;
      
      init_err_flag <= 1'b0;
      done_err_flag <= 1'b0;
      pgm_done_flag <= 1'b0;
      pwait_done_flag <= 1'b0;
      iwait_done_flag <= 1'b0;
      //data_done_flag <= 1'b0;
      //hold_done_flag <= 1'b0;
      eos_done_flag <= 1'b0;
      cfg_done_flag <= 1'b0;
      busy_flag <= 1'b0;
      
      st_last_cnt <= 16'b0;
    end
    else begin
      case ( cur_st )
        IDLE_ST : begin
          pgm_b <= 1'b1;
          rdwr_b <= 1'b0;
          csi_b <= 1'b1;
          cclk <= 1'b0;
          datao <= 8'b0;
          
          cfg_buf_en <= 1'b0;
          cfg_buf_addr <= 32'b0;

          cfg_last <= cfg_cmd[25];
          cfg_start <= cfg_cmd[24];
          cfg_bl <= cfg_cmd[15:0];
          
          //clk_h_flag <= 1'b0;
          //clk_l_flag <= 1'b0;
          
          pgm_done_flag <= 1'b0;
          pwait_done_flag <= 1'b0;
          iwait_done_flag <= 1'b0;
          //data_done_flag <= 1'b0;
          //hold_done_flag <= 1'b0;
          eos_done_flag <= 1'b0;
          busy_flag <= 1'b0;
        
          init_err_flag <= init_err_flag;
          done_err_flag <= done_err_flag;
          cfg_done_flag <= cfg_done_flag;
          
          st_last_cnt <= 16'b0;
        end
        PGM_ST : begin
          if ( st_last_cnt > tPROGRAM ) begin
            pgm_b <= 1'b1;
            pgm_done_flag <= 1'b1;
          end
          else begin
            pgm_b <= 1'b0;
            pgm_done_flag <= 1'b0;
            st_last_cnt <= st_last_cnt + 16'b1;
          end
          
          csi_b <= 1'b0;
          
          cfg_buf_en <= 1'b1;
          cfg_buf_addr <= 32'b0;
          
          pwait_done_flag <= 1'b0;
          iwait_done_flag <= 1'b0;
          init_err_flag <= 1'b0;
          done_err_flag <= 1'b0;
          cfg_done_flag <= 1'b0;
          busy_flag <= 1'b1;
        end
        PGM_WAIT_ST : begin
          if ( st_last_cnt > ( 100 + tPROGRAM ) ) begin
            pwait_done_flag <= 1'b1;
          end
          else begin
            pwait_done_flag <= 1'b0;
            st_last_cnt <= st_last_cnt + 16'b1;
          end
          
          pgm_b <= 1'b1;
          
          cfg_buf_en <= 1'b1;
          cfg_buf_addr <= 32'b0;
          
          init_err_flag <= 1'b0;
          done_err_flag <= 1'b0;
          cfg_done_flag <= 1'b0;
          busy_flag <= 1'b1;
        end
        INIT_ST : begin
          cfg_buf_en <= 1'b1;
          cfg_buf_addr <= 32'b0;
          
          pgm_done_flag <= 1'b0;
          pwait_done_flag <= 1'b0;
          iwait_done_flag <= 1'b0;
          busy_flag <= 1'b1;
          
          st_last_cnt <= 16'b0;
        end
        INIT_WAIT_ST : begin
          if ( st_last_cnt > 100 ) begin
            iwait_done_flag <= 1'b1;
          end
          else begin
            iwait_done_flag <= 1'b0;
            st_last_cnt <= st_last_cnt + 16'b1;
          end
          cfg_buf_en <= 1'b1;
          cfg_buf_addr <= 32'b0;
          
          init_err_flag <= 1'b0;
          done_err_flag <= 1'b0;
          cfg_done_flag <= 1'b0;
          busy_flag <= 1'b1;
        end
        DATA_ST : begin
          cclk <= 1'b0;
          
          case ( cfg_buf_addr[1:0] )
            2'b00 : datao <= cfg_buf_datai[7:0];
            2'b01 : datao <= cfg_buf_datai[15:8];
            2'b10 : datao <= cfg_buf_datai[23:16];
            2'b11 : datao <= cfg_buf_datai[31:24];
            default : datao <= datao;
          endcase
          
          pwait_done_flag <= 1'b0;
          iwait_done_flag <= 1'b0;
          //clk_h_flag <= 1'b0;
          //clk_l_flag <= 1'b0;
          //data_done_flag <= 1'b0;
          busy_flag <= 1'b1;
          
          st_last_cnt <= 16'b0;
        end
        CLK_H_ST : begin
          cclk <= 1'b1;
          
          cfg_buf_addr <= cfg_buf_addr + 32'b1;    // add due to CCLK( 16.6MHz -> 33.3MHz )
          
          //clk_h_flag <= 1'b1;
        end
        CLK_WAIT_ST : begin
          cclk <= 1'b1;
        end
        CLK_L_ST : begin
          cclk <= 1'b0;
          
          busy_flag <= 1'b1;
          
          /*if ( st_last_cnt == 16'b1 )    // remove due to CCLK( 16.6MHz -> 33.3MHz )
            clk_l_flag <= 1'b1;
          else begin
            cfg_buf_addr <= cfg_buf_addr + 32'b1;
          
            st_last_cnt <= 16'b1;
          end*/
        end
        UPDATE_ST : begin
          cfg_last <= 1'b0;
          cfg_start <= 1'b0;
          
          //data_done_flag <= 1'b1;
          busy_flag <= 1'b1;
          
          st_last_cnt <= 16'b0;
        end
        WAIT_ST : begin
          cfg_buf_en <= 1'b1;
          cfg_buf_addr <= 32'b0;
          
          cfg_last <= cfg_cmd[25];
          cfg_start <= cfg_cmd[24];
          cfg_bl <= cfg_cmd[15:0];
          
          //data_done_flag <= 1'b0;
          busy_flag <= 1'b0;
          
          st_last_cnt <= 16'b0;
        end
        LAST_ST : begin
          //cclk <= ~cclk;
          cclk <= ~st_last_cnt[1];
          
          busy_flag <= 1'b1;
          
          if ( st_last_cnt > 16'hFFF0 )
            done_err_flag <= 1'b1;
            
          st_last_cnt <= st_last_cnt + 16'b1;
        end
        HOLD_ST : begin
          //cclk <= ~cclk;
          csi_b <= 1'b1;
          cclk <= ~st_last_cnt[1];
          st_last_cnt <= st_last_cnt + 16'b1;
        end
        EOS_ST : begin
          //cclk <= ~cclk;
          //if ( st_last_cnt < 16'h24 )
          //if ( st_last_cnt < 16'h104 )
          //if ( st_last_cnt < 16'h1004 ) begin
          //  cclk <= st_last_cnt[1];
          //end
          /*else begin
            cclk <= 1'b0;
          end*/
          
          cclk <= ~st_last_cnt[1];
          st_last_cnt <= st_last_cnt + 16'b1;
          
          if ( st_last_cnt > 20'h1_2000 ) begin
            busy_flag <= 1'b0;
            eos_done_flag <= 1'b1;
          end
          else begin
            busy_flag <= 1'b1;
            eos_done_flag <= 1'b0;
          end
        end
        ERROR_ST : begin
          init_err_flag <= 1'b1;
          busy_flag <= 1'b1;
        end
        DONE_ST : begin
          pgm_b <= 1'b1;
          rdwr_b <= 1'b0;
          csi_b <= 1'b1;
          cclk <= 1'b0;
          datao <= 8'b0;
          
          cfg_buf_en <= 1'b0;
          cfg_buf_addr <= 32'b0;

          cfg_last <= 1'b0;
          cfg_start <= 1'b0;
          cfg_bl <= 16'b0;
          
          //clk_h_flag <= 1'b0;
          //clk_l_flag <= 1'b0;
          
          init_err_flag <= 1'b0;
          done_err_flag <= 1'b0;
          pgm_done_flag <= 1'b0;
          //data_done_flag <= 1'b0;
          eos_done_flag <= 1'b0;
          cfg_done_flag <= 1'b1;
          busy_flag <= 1'b0;
          
          st_last_cnt <= 16'b0;
        end
        default : begin
          pgm_b <= 1'b1;
          rdwr_b <= 1'b0;
          csi_b <= 1'b1;
          cclk <= 1'b0;
          datao <= 8'b0;
          
          cfg_buf_en <= 1'b0;
          cfg_buf_addr <= 32'b0;

          cfg_last <= 1'b0;
          cfg_start <= 1'b0;
          cfg_bl <= 16'b0;
          
          //clk_h_flag <= 1'b0;
          //clk_l_flag <= 1'b0;
          
          init_err_flag <= 1'b0;
          done_err_flag <= 1'b0;
          pgm_done_flag <= 1'b0;
          //data_done_flag <= 1'b0;
          eos_done_flag <= 1'b0;
          cfg_done_flag <= 1'b0;
          busy_flag <= 1'b0;
          
          st_last_cnt <= 16'b0;
        end
      endcase
    end
  end
  
  wire  [ 7:0]  cfg_data;
  assign cfg_data = {datao[0], datao[1], datao[2], datao[3], datao[4], datao[5], datao[6], datao[7]};
  
  wire  [TG_NUM-1:0]  tg_sel;
  wire  [PG_NUM-1:0]  pg_sel;
  
  assign tg_sel = slv_reg8[TG_NUM-1+16:16];
  assign pg_sel = slv_reg8[PG_NUM-1+20:20];
  
  genvar        i, j;
  
  wire  [TG_NUM-1:0]    tg_cfg_pgm_b_o;
  wire  [TG_NUM-1:0]    tg_cfg_rdwr_b_o;
  wire  [TG_NUM-1:0]    tg_cfg_csi_b_o;
  wire  [TG_NUM-1:0]    tg_cfg_cclk_o;
  wire  [TG_NUM*8-1:0]  tg_cfg_data_o;

  wire  [PG_NUM-1:0]    pg_cfg_pgm_b_o;
  wire  [PG_NUM-1:0]    pg_cfg_rdwr_b_o;
  wire  [PG_NUM-1:0]    pg_cfg_csi_b_o;
  wire  [PG_NUM-1:0]    pg_cfg_cclk_o;
  wire  [PG_NUM*8-1:0]  pg_cfg_data_o;
  
  generate
    for ( i=0; i<TG_NUM; i=i+1 ) begin
      assign tg_cfg_pgm_b_o[i]  = ( tg_sel[i] == 1'b1 ) ?    pgm_b : 1'b1;
      assign tg_cfg_rdwr_b_o[i] = ( tg_sel[i] == 1'b1 ) ?   rdwr_b : 1'b1;
      assign tg_cfg_csi_b_o[i]  = ( tg_sel[i] == 1'b1 ) ?    csi_b : 1'b1;
      //assign tg_cfg_cclk_o[i]   = ( tg_sel[i] == 1'b1 ) ?     cclk : 1'b0;
      assign tg_cfg_cclk_o[i]   = cclk;
      assign tg_cfg_data_o[8*i+7:8*i] = ( tg_sel[i] == 1'b1 ) ? cfg_data : 8'b0;
      
      ODDR #(
        .DDR_CLK_EDGE("SAME_EDGE"), // "OPPOSITE_EDGE" or "SAME_EDGE" 
        .INIT(1'b0),    // Initial value of Q: 1'b0 or 1'b1
        .SRTYPE("SYNC") // Set/Reset type: "SYNC" or "ASYNC" 
      ) ODDR_tg_pgm_b (
        .Q(tg_cfg_pgm_b[i]),   // 1-bit DDR output
        .C(cfg_buf_clk),   // 1-bit clock input
        .CE(1'b1), // 1-bit clock enable input
        .D1(tg_cfg_pgm_b_o[i]), // 1-bit data input (positive edge)
        .D2(tg_cfg_pgm_b_o[i]), // 1-bit data input (negative edge)
        .R(1'b0),   // 1-bit reset
        .S(1'b0)    // 1-bit set
      );

      ODDR #(
        .DDR_CLK_EDGE("SAME_EDGE"), // "OPPOSITE_EDGE" or "SAME_EDGE" 
        .INIT(1'b0),    // Initial value of Q: 1'b0 or 1'b1
        .SRTYPE("SYNC") // Set/Reset type: "SYNC" or "ASYNC" 
      ) ODDR_tg_rdwr_b (
        .Q(tg_cfg_rdwr_b[i]),   // 1-bit DDR output
        .C(cfg_buf_clk),   // 1-bit clock input
        .CE(1'b1), // 1-bit clock enable input
        .D1(tg_cfg_rdwr_b_o[i]), // 1-bit data input (positive edge)
        .D2(tg_cfg_rdwr_b_o[i]), // 1-bit data input (negative edge)
        .R(1'b0),   // 1-bit reset
        .S(1'b0)    // 1-bit set
      );
      
      ODDR #(
        .DDR_CLK_EDGE("SAME_EDGE"), // "OPPOSITE_EDGE" or "SAME_EDGE" 
        .INIT(1'b0),    // Initial value of Q: 1'b0 or 1'b1
        .SRTYPE("SYNC") // Set/Reset type: "SYNC" or "ASYNC" 
      ) ODDR_tg_csi_b (
        .Q(tg_cfg_csi_b[i]),   // 1-bit DDR output
        .C(cfg_buf_clk),   // 1-bit clock input
        .CE(1'b1), // 1-bit clock enable input
        .D1(tg_cfg_csi_b_o[i]), // 1-bit data input (positive edge)
        .D2(tg_cfg_csi_b_o[i]), // 1-bit data input (negative edge)
        .R(1'b0),   // 1-bit reset
        .S(1'b0)    // 1-bit set
      );
            
      ODDR #(
        .DDR_CLK_EDGE("SAME_EDGE"), // "OPPOSITE_EDGE" or "SAME_EDGE" 
        .INIT(1'b0),    // Initial value of Q: 1'b0 or 1'b1
        .SRTYPE("SYNC") // Set/Reset type: "SYNC" or "ASYNC" 
      ) ODDR_tg_cclk (
        .Q(tg_cfg_cclk[i]),   // 1-bit DDR output
        .C(cfg_buf_clk),   // 1-bit clock input
        .CE(1'b1), // 1-bit clock enable input
        .D1(tg_cfg_cclk_o[i]), // 1-bit data input (positive edge)
        .D2(tg_cfg_cclk_o[i]), // 1-bit data input (negative edge)
        .R(1'b0),   // 1-bit reset
        .S(1'b0)    // 1-bit set
      );
            
      for ( j=0; j<8; j=j+1 ) begin
        ODDR #(
          .DDR_CLK_EDGE("SAME_EDGE"), // "OPPOSITE_EDGE" or "SAME_EDGE" 
          .INIT(1'b0),    // Initial value of Q: 1'b0 or 1'b1
          .SRTYPE("SYNC") // Set/Reset type: "SYNC" or "ASYNC" 
        ) ODDR_tg_data (
          .Q(tg_cfg_data[8*i+j]),   // 1-bit DDR output
          .C(cfg_buf_clk),   // 1-bit clock input
          .CE(1'b1), // 1-bit clock enable input
          .D1(tg_cfg_data_o[8*i+j]), // 1-bit data input (positive edge)
          .D2(tg_cfg_data_o[8*i+j]), // 1-bit data input (negative edge)
          .R(1'b0),   // 1-bit reset
          .S(1'b0)    // 1-bit set
        );
      end
    end
  endgenerate
  
  generate
    for ( i=0; i<PG_NUM; i=i+1 ) begin
      assign pg_cfg_pgm_b_o[i]  = ( pg_sel[i] == 1'b1 ) ?    pgm_b : 1'b1;
      assign pg_cfg_rdwr_b_o[i] = ( pg_sel[i] == 1'b1 ) ?   rdwr_b : 1'b1;
      assign pg_cfg_csi_b_o[i]  = ( pg_sel[i] == 1'b1 ) ?    csi_b : 1'b1;
      //assign pg_cfg_cclk_o[i]   = ( pg_sel[i] == 1'b1 ) ?     cclk : 1'b0;
      assign pg_cfg_cclk_o[i]   = cclk;
      assign pg_cfg_data_o[8*i+7:8*i] = ( pg_sel[i] == 1'b1 ) ? cfg_data : 8'b0;

      ODDR #(
        .DDR_CLK_EDGE("SAME_EDGE"), // "OPPOSITE_EDGE" or "SAME_EDGE" 
        .INIT(1'b0),    // Initial value of Q: 1'b0 or 1'b1
        .SRTYPE("SYNC") // Set/Reset type: "SYNC" or "ASYNC" 
      ) ODDR_pg_pgm_b (
        .Q(pg_cfg_pgm_b[i]),   // 1-bit DDR output
        .C(cfg_buf_clk),   // 1-bit clock input
        .CE(1'b1), // 1-bit clock enable input
        .D1(pg_cfg_pgm_b_o[i]), // 1-bit data input (positive edge)
        .D2(pg_cfg_pgm_b_o[i]), // 1-bit data input (negative edge)
        .R(1'b0),   // 1-bit reset
        .S(1'b0)    // 1-bit set
      );
      
      ODDR #(
        .DDR_CLK_EDGE("SAME_EDGE"), // "OPPOSITE_EDGE" or "SAME_EDGE" 
        .INIT(1'b0),    // Initial value of Q: 1'b0 or 1'b1
        .SRTYPE("SYNC") // Set/Reset type: "SYNC" or "ASYNC" 
      ) ODDR_pg_rdwr_b (
        .Q(pg_cfg_rdwr_b[i]),   // 1-bit DDR output
        .C(cfg_buf_clk),   // 1-bit clock input
        .CE(1'b1), // 1-bit clock enable input
        .D1(pg_cfg_rdwr_b_o[i]), // 1-bit data input (positive edge)
        .D2(pg_cfg_rdwr_b_o[i]), // 1-bit data input (negative edge)
        .R(1'b0),   // 1-bit reset
        .S(1'b0)    // 1-bit set
      );
      
      ODDR #(
        .DDR_CLK_EDGE("SAME_EDGE"), // "OPPOSITE_EDGE" or "SAME_EDGE" 
        .INIT(1'b0),    // Initial value of Q: 1'b0 or 1'b1
        .SRTYPE("SYNC") // Set/Reset type: "SYNC" or "ASYNC" 
      ) ODDR_pg_csi_b (
        .Q(pg_cfg_csi_b[i]),   // 1-bit DDR output
        .C(cfg_buf_clk),   // 1-bit clock input
        .CE(1'b1), // 1-bit clock enable input
        .D1(pg_cfg_csi_b_o[i]), // 1-bit data input (positive edge)
        .D2(pg_cfg_csi_b_o[i]), // 1-bit data input (negative edge)
        .R(1'b0),   // 1-bit reset
        .S(1'b0)    // 1-bit set
      );
      
      ODDR #(
        .DDR_CLK_EDGE("SAME_EDGE"), // "OPPOSITE_EDGE" or "SAME_EDGE" 
        .INIT(1'b0),    // Initial value of Q: 1'b0 or 1'b1
        .SRTYPE("SYNC") // Set/Reset type: "SYNC" or "ASYNC" 
      ) ODDR_pg_cclk (
        .Q(pg_cfg_cclk[i]),   // 1-bit DDR output
        .C(cfg_buf_clk),   // 1-bit clock input
        .CE(1'b1), // 1-bit clock enable input
        .D1(pg_cfg_cclk_o[i]), // 1-bit data input (positive edge)
        .D2(pg_cfg_cclk_o[i]), // 1-bit data input (negative edge)
        .R(1'b0),   // 1-bit reset
        .S(1'b0)    // 1-bit set
      );
      
      for ( j=0; j<8; j=j+1 ) begin
        ODDR #(
          .DDR_CLK_EDGE("SAME_EDGE"), // "OPPOSITE_EDGE" or "SAME_EDGE" 
          .INIT(1'b0),    // Initial value of Q: 1'b0 or 1'b1
          .SRTYPE("SYNC") // Set/Reset type: "SYNC" or "ASYNC" 
        ) ODDR_pg_data (
          .Q(pg_cfg_data[8*i+j]),   // 1-bit DDR output
          .C(cfg_buf_clk),   // 1-bit clock input
          .CE(1'b1), // 1-bit clock enable input
          .D1(pg_cfg_data_o[8*i+j]), // 1-bit data input (positive edge)
          .D2(pg_cfg_data_o[8*i+j]), // 1-bit data input (negative edge)
          .R(1'b0),   // 1-bit reset
          .S(1'b0)    // 1-bit set
        );
      end
    end
  endgenerate
  // assign pg_cfg_cclk[0] = ( ( pg_sel[0] == 1'b1 ) || pg_sel[1] == 1'b1 ) ) ? cclk : 1'b0;
  
  always @ ( * ) begin
    if ( |tg_sel ) begin cfg_init_b = ( tg_sel == tg_cfg_init_b ); cfg_done = ( tg_sel == tg_cfg_done ); end
    
    else if ( |pg_sel ) begin cfg_init_b = ( pg_sel == pg_cfg_init_b ); cfg_done = ( pg_sel == pg_cfg_done ); end
    
    else begin cfg_init_b = 1'b1; cfg_done = 1'b0; end
  end
  
  wire  [ 3:0]  tg_done;
  wire  [ 3:0]  pg_done;
  
  assign tg_done = tg_cfg_done;
  assign pg_done = pg_cfg_done;
  assign all_cfg_done = ( &tg_cfg_done ) && ( &pg_cfg_done );
  // User logic ends
  
  // I/O Connections assignments

  assign S_AXI_AWREADY  = axi_awready;
  assign S_AXI_WREADY  = axi_wready;
  assign S_AXI_BRESP  = axi_bresp;
  assign S_AXI_BVALID  = axi_bvalid;
  assign S_AXI_ARREADY  = axi_arready;
  assign S_AXI_RDATA  = axi_rdata;
  assign S_AXI_RRESP  = axi_rresp;
  assign S_AXI_RVALID  = axi_rvalid;
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
        
        slv_reg12 <= { 7'b0, all_cfg_done, pg_done, tg_done, 8'b0, {init_err_flag, done_err_flag, cfg_init_b, cfg_done, cfg_done_flag, 2'b0, busy_flag} };
        
        if ( busy_flag == 1'b1 ) begin
          slv_reg8 <= slv_reg8 & 32'h00FF_FFFF;
        
          if ( ip_rst_clr == 1'b1 )
            slv_reg8[31] <= 1'b0;
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
