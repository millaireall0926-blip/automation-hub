
######################################
# UART : 
######################################
set_property PACKAGE_PIN W29 [get_ports uart_dbg_rxd]
set_property PACKAGE_PIN W25 [get_ports uart_dbg_txd]

set_property IOSTANDARD LVCMOS33 [get_ports uart_dbg_*]


######################################
# DIFF CLOCK :
######################################
set_property PACKAGE_PIN AH29 [get_ports sys_diff_clock_clk_n]
set_property PACKAGE_PIN AH28 [get_ports sys_diff_clock_clk_p]

set_property IOSTANDARD DIFF_SSTL18_I [get_ports sys_diff_clock_clk_*]


######################################
# BOARD-INFO : 
######################################
set_property PACKAGE_PIN AB26 [get_ports {tpcb_rev_0[0]}]
set_property PACKAGE_PIN AB27 [get_ports {tpcb_rev_0[1]}]
set_property PACKAGE_PIN AA29 [get_ports {tpcb_rev_0[2]}]
set_property PACKAGE_PIN AB29 [get_ports {tpcb_rev_0[3]}]

set_property PACKAGE_PIN AG32 [get_ports {pg_rev_0[0]}]
set_property PACKAGE_PIN AH32 [get_ports {pg_rev_0[1]}]
set_property PACKAGE_PIN AE32 [get_ports {pg_rev_0[2]}]
set_property PACKAGE_PIN AF32 [get_ports {pg_rev_0[3]}]

set_property PACKAGE_PIN R26 [get_ports {slot_id_0[0]}]
set_property PACKAGE_PIN P26 [get_ports {slot_id_0[1]}]
set_property PACKAGE_PIN N26 [get_ports {slot_id_0[2]}]
set_property PACKAGE_PIN M27 [get_ports {slot_id_0[3]}]
set_property PACKAGE_PIN U25 [get_ports {slot_id_0[4]}]
set_property PACKAGE_PIN T25 [get_ports {slot_id_0[5]}]
set_property PACKAGE_PIN P24 [get_ports {slot_id_0[6]}]
set_property PACKAGE_PIN N24 [get_ports {slot_id_0[7]}]

set_property PACKAGE_PIN AC28 [get_ports {led_act_0}]
set_property PACKAGE_PIN AA27 [get_ports {led_err_0}]
set_property PACKAGE_PIN AC29 [get_ports {led_on_0}]
set_property PACKAGE_PIN AA24 [get_ports {led_o_0[0]}]
set_property PACKAGE_PIN AA25 [get_ports {led_o_0[1]}]
set_property PACKAGE_PIN AB24 [get_ports {led_o_0[2]}]
set_property PACKAGE_PIN AB25 [get_ports {led_o_0[3]}]

set_property PACKAGE_PIN L7   [get_ports {bib_chkn_0[0]}]
set_property PACKAGE_PIN U11  [get_ports {bib_chkn_0[1]}]
set_property PACKAGE_PIN AF10 [get_ports {bib_chkn_0[2]}]
set_property PACKAGE_PIN AL8  [get_ports {bib_chkn_0[3]}]

set_property PACKAGE_PIN AA10 [get_ports {bib_sense_sel_0[0]}]
set_property PACKAGE_PIN AA9  [get_ports {bib_sense_sel_0[1]}]
set_property PACKAGE_PIN AB10 [get_ports {bib_sense_sel_0[2]}]
set_property PACKAGE_PIN AB9  [get_ports {bib_sense_sel_0[3]}]

set_property PACKAGE_PIN H12  [get_ports {ctrl_rly_dgs_0[0]}]
set_property PACKAGE_PIN R11  [get_ports {ctrl_rly_dgs_0[1]}]
set_property PACKAGE_PIN AC11 [get_ports {ctrl_rly_dgs_0[2]}]
set_property PACKAGE_PIN AJ9  [get_ports {ctrl_rly_dgs_0[3]}]

set_property PACKAGE_PIN W10 [get_ports {bib_id_0[0]}]
set_property PACKAGE_PIN Y10 [get_ports {bib_id_0[1]}]
set_property PACKAGE_PIN W6  [get_ports {bib_id_0[10]}]
set_property PACKAGE_PIN W1  [get_ports {bib_id_0[11]}]
set_property PACKAGE_PIN Y1  [get_ports {bib_id_0[12]}]
set_property PACKAGE_PIN V2  [get_ports {bib_id_0[13]}]
set_property PACKAGE_PIN V1  [get_ports {bib_id_0[14]}]
set_property PACKAGE_PIN Y3  [get_ports {bib_id_0[15]}]
set_property PACKAGE_PIN Y2  [get_ports {bib_id_0[16]}]
set_property PACKAGE_PIN V3  [get_ports {bib_id_0[17]}]
set_property PACKAGE_PIN W3  [get_ports {bib_id_0[18]}]
set_property PACKAGE_PIN V4  [get_ports {bib_id_0[19]}]
set_property PACKAGE_PIN V9  [get_ports {bib_id_0[2]}]
set_property PACKAGE_PIN W4  [get_ports {bib_id_0[20]}]
set_property PACKAGE_PIN W5  [get_ports {bib_id_0[21]}]
set_property PACKAGE_PIN AA5 [get_ports {bib_id_0[22]}]
set_property PACKAGE_PIN AA4 [get_ports {bib_id_0[23]}]
set_property PACKAGE_PIN AB5 [get_ports {bib_id_0[24]}]
set_property PACKAGE_PIN AB4 [get_ports {bib_id_0[25]}]
set_property PACKAGE_PIN AB2 [get_ports {bib_id_0[26]}]
set_property PACKAGE_PIN AB1 [get_ports {bib_id_0[27]}]
set_property PACKAGE_PIN AA3 [get_ports {bib_id_0[28]}]
set_property PACKAGE_PIN AA2 [get_ports {bib_id_0[29]}]
set_property PACKAGE_PIN V8  [get_ports {bib_id_0[3]}]
set_property PACKAGE_PIN AC2 [get_ports {bib_id_0[30]}]
set_property PACKAGE_PIN AC1 [get_ports {bib_id_0[31]}]
set_property PACKAGE_PIN W9  [get_ports {bib_id_0[4]}]
set_property PACKAGE_PIN W8  [get_ports {bib_id_0[5]}]
set_property PACKAGE_PIN V7  [get_ports {bib_id_0[6]}]
set_property PACKAGE_PIN V6  [get_ports {bib_id_0[7]}]
set_property PACKAGE_PIN Y8  [get_ports {bib_id_0[8]}]
set_property PACKAGE_PIN Y7  [get_ports {bib_id_0[9]}]

set_property IOSTANDARD LVCMOS33 [get_ports tpcb_rev_0*]
set_property IOSTANDARD LVCMOS18 [get_ports pg_rev_0*]
set_property IOSTANDARD LVCMOS18 [get_ports slot_id_0*]
set_property IOSTANDARD LVCMOS33 [get_ports led_*]
set_property IOSTANDARD LVCMOS33 [get_ports bib_chkn_0*]
set_property IOSTANDARD LVCMOS33 [get_ports bib_sense_sel_0*]
set_property IOSTANDARD LVCMOS33 [get_ports ctrl_rly_dgs_0*]
set_property IOSTANDARD LVCMOS33 [get_ports bib_id_0*]

set_property PULLUP TRUE [get_ports tpcb_rev_0*]


######################################
# CFGx8-C2X :
######################################
set_property PACKAGE_PIN AH27 [get_ports {pg_cfg_cclk_0[0]}]
set_property PACKAGE_PIN AF27 [get_ports {pg_cfg_csi_b_0[0]}]
set_property PACKAGE_PIN AF23 [get_ports {pg_cfg_data_0[0]}]
set_property PACKAGE_PIN AG24 [get_ports {pg_cfg_data_0[1]}]
set_property PACKAGE_PIN AH24 [get_ports {pg_cfg_data_0[2]}]
set_property PACKAGE_PIN AC24 [get_ports {pg_cfg_data_0[3]}]
set_property PACKAGE_PIN AD24 [get_ports {pg_cfg_data_0[4]}]
set_property PACKAGE_PIN AF25 [get_ports {pg_cfg_data_0[5]}]
set_property PACKAGE_PIN AG25 [get_ports {pg_cfg_data_0[6]}]
set_property PACKAGE_PIN AD25 [get_ports {pg_cfg_data_0[7]}]
set_property PACKAGE_PIN AG27 [get_ports {pg_cfg_done_0[0]}]
set_property PACKAGE_PIN AC27 [get_ports {pg_cfg_init_b_0[0]}]
set_property PACKAGE_PIN AC26 [get_ports {pg_cfg_pgm_b_0[0]}]
set_property PACKAGE_PIN AE27 [get_ports {pg_cfg_rdwr_b_0[0]}]

set_property IOSTANDARD LVCMOS18 [get_ports pg_cfg_*]


######################################
# LBx8-C2P : 
######################################
set_property PACKAGE_PIN AG29 [get_ports {pg_lb_clk_0[0]}]
set_property PACKAGE_PIN AD28 [get_ports {pg_lb_error_0[0]}]
set_property PACKAGE_PIN AF34 [get_ports {pg_lb_ad_0[0]}]
set_property PACKAGE_PIN AG34 [get_ports {pg_lb_ad_0[1]}]
set_property PACKAGE_PIN AD33 [get_ports {pg_lb_ad_0[2]}]
set_property PACKAGE_PIN AD34 [get_ports {pg_lb_ad_0[3]}]
set_property PACKAGE_PIN AH33 [get_ports {pg_lb_ad_0[4]}]
set_property PACKAGE_PIN AH34 [get_ports {pg_lb_ad_0[5]}]
set_property PACKAGE_PIN AE33 [get_ports {pg_lb_ad_0[6]}]
set_property PACKAGE_PIN AF33 [get_ports {pg_lb_ad_0[7]}]
set_property PACKAGE_PIN AE30 [get_ports {pg_lb_last_0[0]}]
set_property PACKAGE_PIN AD31 [get_ports {pg_lb_rstn_0[0]}]
set_property PACKAGE_PIN AD30 [get_ports {pg_lb_valid_0[0]}]
#set_property PACKAGE_PIN AD29 [get_ports {pg_lb_status_0[0]}]
set_property PACKAGE_PIN AE31 [get_ports {pg_lb_wr_0[0]}]
                                                    
set_property IOSTANDARD LVCMOS18 [get_ports pg_lb_*]


######################################
# ADC :
######################################
set_property PACKAGE_PIN M30 [get_ports {adc_busy_0}]
set_property PACKAGE_PIN M31 [get_ports {adc_cnv_0}]
set_property PACKAGE_PIN R31 [get_ports {adc_csck_0}]
set_property PACKAGE_PIN N31 [get_ports {adc_csdo_0}]
set_property PACKAGE_PIN P31 [get_ports {adc_csdio_0}]
set_property PACKAGE_PIN U30 [get_ports {adc_csn_0}]
set_property PACKAGE_PIN T30 [get_ports {adc_pd_0}]
set_property PACKAGE_PIN U31 [get_ports {adc_scko_0}]
set_property PACKAGE_PIN M32 [get_ports {adc_scki_0}]
set_property PACKAGE_PIN T32 [get_ports {adc_sdo0_0}]
set_property PACKAGE_PIN R32 [get_ports {adc_sdo1_0}]
set_property PACKAGE_PIN N32 [get_ports {adc_sdo2_0}]
set_property PACKAGE_PIN N33 [get_ports {adc_sdo3_0}]
set_property PACKAGE_PIN T33 [get_ports {adc_sdo4_0}]
set_property PACKAGE_PIN R33 [get_ports {adc_sdo5_0}]
set_property PACKAGE_PIN N34 [get_ports {adc_sdo6_0}]
set_property PACKAGE_PIN M34 [get_ports {adc_sdo7_0}]

set_property PACKAGE_PIN AA8 [get_ports {mux_a_0[0]}]
set_property PACKAGE_PIN AA7 [get_ports {mux_a_0[1]}]
set_property PACKAGE_PIN AC7 [get_ports {mux_a_0[2]}]
set_property PACKAGE_PIN AC6 [get_ports {mux_a_0[3]}]

set_property IOSTANDARD LVCMOS18 [get_ports adc_*]
set_property IOSTANDARD LVCMOS33 [get_ports mux_a_0*]


######################################
# DAC :
######################################
set_property PACKAGE_PIN N27 [get_ports {dac_ldacn_0[0]}]
set_property PACKAGE_PIN R28 [get_ports {dac_ldacn_0[1]}]
set_property PACKAGE_PIN U29 [get_ports {dac_ldacn_0[2]}]
set_property PACKAGE_PIN P28 [get_ports {dac_rstn_0}]
set_property PACKAGE_PIN T27 [get_ports {iic_dac_0_scl_io}]
set_property PACKAGE_PIN N28 [get_ports {iic_dac_1_scl_io}]
set_property PACKAGE_PIN N29 [get_ports {iic_dac_2_scl_io}]
set_property PACKAGE_PIN R27 [get_ports {iic_dac_0_sda_io}]
set_property PACKAGE_PIN T28 [get_ports {iic_dac_1_sda_io}]
set_property PACKAGE_PIN M29 [get_ports {iic_dac_2_sda_io}]

set_property IOSTANDARD LVCMOS18 [get_ports dac_*]
set_property IOSTANDARD LVCMOS18 [get_ports iic_dac_*]


######################################
# DPS : 
# - RUN 0/1-IO0/1; HVDPS_EN 0/1 - IO 0/1
# - nALERT - IO2
# - DPS SCL/SDA - IO3/4 
# - nFAULT - IO5
# - INA 0/1 SCL/SDA - IO 6/7 8/9
######################################
set_property PACKAGE_PIN M24 [get_ports {dps_run_0[0]}]
set_property PACKAGE_PIN L24 [get_ports {dps_run_0[1]}]
set_property PACKAGE_PIN K23 [get_ports {dps_nalert_0[0]}]
set_property PACKAGE_PIN J23 [get_ports {iic_dps_0_scl_io}]
set_property PACKAGE_PIN G24 [get_ports {iic_dps_0_sda_io}]
set_property PACKAGE_PIN G25 [get_ports {dps_nfault_0[0]}]
set_property PACKAGE_PIN K25 [get_ports {iic_ina_0_scl_io}]
set_property PACKAGE_PIN J25 [get_ports {iic_ina_0_sda_io}]
set_property PACKAGE_PIN M25 [get_ports {iic_ina_1_scl_io}]
set_property PACKAGE_PIN L25 [get_ports {iic_ina_1_sda_io}]

set_property PACKAGE_PIN H27 [get_ports {dps_run_0[2]}]
set_property PACKAGE_PIN G27 [get_ports {dps_run_0[3]}]
set_property PACKAGE_PIN H26 [get_ports {dps_nalert_0[1]}]
set_property PACKAGE_PIN G26 [get_ports {iic_dps_1_scl_io}]
set_property PACKAGE_PIN L27 [get_ports {iic_dps_1_sda_io}]
set_property PACKAGE_PIN K27 [get_ports {dps_nfault_0[1]}]
set_property PACKAGE_PIN K26 [get_ports {iic_ina_2_scl_io}]
set_property PACKAGE_PIN J26 [get_ports {iic_ina_2_sda_io}]
set_property PACKAGE_PIN L28 [get_ports {iic_ina_3_scl_io}]
set_property PACKAGE_PIN K28 [get_ports {iic_ina_3_sda_io}]

set_property PACKAGE_PIN J29 [get_ports {dps_run_0[4]}]
set_property PACKAGE_PIN H29 [get_ports {dps_run_0[5]}]
set_property PACKAGE_PIN K30 [get_ports {dps_nalert_0[2]}]
set_property PACKAGE_PIN J30 [get_ports {iic_dps_2_scl_io}]
set_property PACKAGE_PIN G29 [get_ports {iic_dps_2_sda_io}]
set_property PACKAGE_PIN G30 [get_ports {dps_nfault_0[2]}]
set_property PACKAGE_PIN K31 [get_ports {iic_ina_4_scl_io}]
set_property PACKAGE_PIN J31 [get_ports {iic_ina_4_sda_io}]
set_property PACKAGE_PIN H31 [get_ports {iic_ina_5_scl_io}]
set_property PACKAGE_PIN G31 [get_ports {iic_ina_5_sda_io}]

set_property PACKAGE_PIN H32 [get_ports {dps_run_0[6]}]
set_property PACKAGE_PIN G32 [get_ports {dps_run_0[7]}]
set_property PACKAGE_PIN K33 [get_ports {dps_nalert_0[3]}]
set_property PACKAGE_PIN J34 [get_ports {iic_dps_3_scl_io}]
set_property PACKAGE_PIN H33 [get_ports {iic_dps_3_sda_io}]
set_property PACKAGE_PIN G34 [get_ports {dps_nfault_0[3]}]
set_property PACKAGE_PIN L32 [get_ports {iic_ina_6_scl_io}]
set_property PACKAGE_PIN K32 [get_ports {iic_ina_6_sda_io}]
set_property PACKAGE_PIN J33 [get_ports {iic_ina_7_scl_io}]
set_property PACKAGE_PIN H34 [get_ports {iic_ina_7_sda_io}]

set_property PACKAGE_PIN AL34 [get_ports {dps_run_0[8]}]
set_property PACKAGE_PIN AM34 [get_ports {dps_run_0[9]}]
set_property PACKAGE_PIN AJ33 [get_ports {dps_nalert_0[4]}]
set_property PACKAGE_PIN AJ34 [get_ports {iic_dps_4_scl_io}]
set_property PACKAGE_PIN AN34 [get_ports {iic_dps_4_sda_io}]
set_property PACKAGE_PIN AP34 [get_ports {dps_nfault_0[4]}]
set_property PACKAGE_PIN AK33 [get_ports {iic_ina_8_scl_io}]
set_property PACKAGE_PIN AL33 [get_ports {iic_ina_8_sda_io}]
set_property PACKAGE_PIN AN33 [get_ports {iic_ina_9_scl_io}]
set_property PACKAGE_PIN AP33 [get_ports {iic_ina_9_sda_io}]

set_property PACKAGE_PIN AJ31 [get_ports {dps_run_0[10]}]
set_property PACKAGE_PIN AK32 [get_ports {dps_run_0[11]}]
set_property PACKAGE_PIN AM31 [get_ports {dps_nalert_0[5]}]
set_property PACKAGE_PIN AN32 [get_ports {iic_dps_5_scl_io}]
set_property PACKAGE_PIN AJ30 [get_ports {iic_dps_5_sda_io}]
set_property PACKAGE_PIN AK31 [get_ports {dps_nfault_0[5]}]
set_property PACKAGE_PIN AN31 [get_ports {iic_ina_10_scl_io}]
set_property PACKAGE_PIN AP31 [get_ports {iic_ina_10_sda_io}]
set_property PACKAGE_PIN AJ29 [get_ports {iic_ina_11_scl_io}]
set_property PACKAGE_PIN AK30 [get_ports {iic_ina_11_sda_io}]

set_property PACKAGE_PIN AL28 [get_ports {dps_run_0[12]}]
set_property PACKAGE_PIN AL29 [get_ports {dps_run_0[13]}]
set_property PACKAGE_PIN AJ28 [get_ports {dps_nalert_0[6]}]
set_property PACKAGE_PIN AK28 [get_ports {iic_dps_6_scl_io}]
set_property PACKAGE_PIN AP29 [get_ports {iic_dps_6_sda_io}]
set_property PACKAGE_PIN AP30 [get_ports {dps_nfault_0[6]}]
set_property PACKAGE_PIN AM29 [get_ports {iic_ina_12_scl_io}]
set_property PACKAGE_PIN AN29 [get_ports {iic_ina_12_sda_io}]
set_property PACKAGE_PIN AN28 [get_ports {iic_ina_13_scl_io}]
set_property PACKAGE_PIN AP28 [get_ports {iic_ina_13_sda_io}]

set_property PACKAGE_PIN AJ25 [get_ports {dps_run_0[14]}]
set_property PACKAGE_PIN AK25 [get_ports {dps_run_0[15]}]
set_property PACKAGE_PIN AJ26 [get_ports {dps_nalert_0[7]}]
set_property PACKAGE_PIN AK26 [get_ports {iic_dps_7_scl_io}]
set_property PACKAGE_PIN AM26 [get_ports {iic_dps_7_sda_io}]
set_property PACKAGE_PIN AN26 [get_ports {dps_nfault_0[7]}]
set_property PACKAGE_PIN AM27 [get_ports {iic_ina_14_scl_io}]
set_property PACKAGE_PIN AN27 [get_ports {iic_ina_14_sda_io}]
set_property PACKAGE_PIN AL25 [get_ports {iic_ina_15_scl_io}]
set_property PACKAGE_PIN AM25 [get_ports {iic_ina_15_sda_io}]

# HV-DPS
set_property PACKAGE_PIN V31 [get_ports {hv_dps_en_0[0]}]
set_property PACKAGE_PIN V32 [get_ports {hv_dps_en_0[1]}]
set_property PACKAGE_PIN Y32 [get_ports {iic_hv_ina_0_scl_io}]
set_property PACKAGE_PIN Y33 [get_ports {iic_hv_ina_0_sda_io}]
set_property PACKAGE_PIN W30 [get_ports {iic_hv_ina_1_scl_io}]
set_property PACKAGE_PIN W31 [get_ports {iic_hv_ina_1_sda_io}]

set_property PACKAGE_PIN AA30 [get_ports {hv_dps_en_0[2]}]
set_property PACKAGE_PIN AB30 [get_ports {hv_dps_en_0[3]}]
set_property PACKAGE_PIN AA32 [get_ports {iic_hv_ina_2_scl_io}]
set_property PACKAGE_PIN AA33 [get_ports {iic_hv_ina_2_sda_io}]
set_property PACKAGE_PIN AC31 [get_ports {iic_hv_ina_3_scl_io}]
set_property PACKAGE_PIN AC32 [get_ports {iic_hv_ina_3_sda_io}]

set_property PACKAGE_PIN AL24 [get_ports {dps_cal_en_0}]
set_property PACKAGE_PIN M26  [get_ports {hv_dps_cal_en_0}]

set_property PACKAGE_PIN AB7 [get_ports {vref_en_0[0]}]
set_property PACKAGE_PIN AB6 [get_ports {vref_en_0[1]}]
set_property PACKAGE_PIN AC9 [get_ports {vref_en_0[2]}]
set_property PACKAGE_PIN AC8 [get_ports {vref_en_0[3]}]

set_property IOSTANDARD LVCMOS33 [get_ports dps_run_0*]
set_property IOSTANDARD LVCMOS33 [get_ports dps_nalert_0*]
set_property IOSTANDARD LVCMOS33 [get_ports iic_dps_*]
set_property IOSTANDARD LVCMOS33 [get_ports dps_nfault_0*]
set_property IOSTANDARD LVCMOS33 [get_ports iic_ina_*]

set_property IOSTANDARD LVCMOS33 [get_ports hv_dps_en_0*]
set_property IOSTANDARD LVCMOS33 [get_ports iic_hv_ina_*]

set_property IOSTANDARD LVCMOS33 [get_ports dps_cal_en_0*]
set_property IOSTANDARD LVCMOS33 [get_ports hv_dps_cal_en_0*]
set_property IOSTANDARD LVCMOS33 [get_ports vref_en_0*]


######################################
# EEP-I2C : 
######################################
set_property PACKAGE_PIN L33 [get_ports iic_eep_scl_io]
set_property PACKAGE_PIN L34 [get_ports iic_eep_sda_io]

set_property IOSTANDARD LVCMOS33 [get_ports iic_eep_*]


######################################
# CFG-PROM : SPI
######################################
set_property PACKAGE_PIN V28 [get_ports spi_prom_io0_io]
set_property PACKAGE_PIN V29 [get_ports spi_prom_io1_io]
set_property PACKAGE_PIN V26 [get_ports spi_prom_io2_io]
set_property PACKAGE_PIN V27 [get_ports spi_prom_io3_io]
set_property PACKAGE_PIN Y27 [get_ports spi_prom_ss_io]

set_property IOSTANDARD LVCMOS33 [get_ports spi_prom_*]


######################################
# PCIe :
######################################
set_property PACKAGE_PIN G18 [get_ports {pcie_diff_clock_clk_n[0]}]
set_property PACKAGE_PIN H18 [get_ports {pcie_diff_clock_clk_p[0]}]

create_clock -period 10.000 -name pcie_clk [get_ports {pcie_diff_clock_clk_p[0]}]


set_property PACKAGE_PIN AP26 [get_ports perstn]

set_property IOSTANDARD LVCMOS33 [get_ports perstn]
