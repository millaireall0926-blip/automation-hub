#ifndef __MACRO__
#define __MACRO__

#include "STAR_TEST.h"

#define BIB(ROW, FORMAT...) macro_BIB(ROW, #FORMAT)
#define PDS(DEST, SOURCE...) macro_PDS(DEST, #SOURCE)
#define SCRAM(VAL) macro_SCRAM(#VAL)

#define STEP(NAME)	void run##NAME()
#define RUN_STEP(NAME)	start_step(#NAME);	if (!_bAddStepMap) run##NAME();	else _mapStepFunc[#NAME]=run##NAME;	end_step()

#define STEPI(NAME, ARG)	void runi##NAME(int ARG)
#define RUN_STEPI(NAME, ARG)	start_step(#NAME);	if (!_bAddStepMap) runi##NAME(_mapStepIArg[#NAME]);	else{ _mapStepIFunc[#NAME]=runi##NAME; _mapStepIArg[#NAME]=ARG;}	end_step()



#endif
