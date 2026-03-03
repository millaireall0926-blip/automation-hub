
extern void initAdcCalData();
extern void initDacCalData();
extern void initDpsCalData();

extern void makeAdcCalData();
extern void makeDacCalData();
extern void makeDpsCalData();

extern bool adc_cal();
extern bool dps_cal();
extern bool dps_i_cal();
extern bool dutvtt_cal();
extern bool iovtt_cal();
extern bool dutvref_cal();
extern bool pg_drio_cal();
extern bool pg_vth_cal();

extern bool sys_volt_diag();
extern bool adc_diag();
extern bool dps_diag(int Mode = 0);
extern bool dps_i_diag(int Mode = 0);
extern bool dutvtt_diag(int Mode = 0);
extern bool iovtt_diag(int Mode = 0);
extern bool dutvref_diag(int Mode = 0);
extern bool pg_drio_diag(int Mode = 0);
extern bool pg_vth_diag(int Mode = 0);
