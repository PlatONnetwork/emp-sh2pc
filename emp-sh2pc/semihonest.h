#ifndef SEMIHONEST_H__
#define SEMIHONEST_H__
#include "emp-sh2pc/semihonest_gen.h"
#include "emp-sh2pc/semihonest_eva.h"

#ifdef OT_NP_USE_MIRACL
#include "emp-tool/utils/sm2_params.h"
#else
#include "emp-tool/utils/utils_ec.h"
#endif//

namespace emp {
template<typename IO>
inline void setup_semi_honest(IO* io, int party) {

#ifdef OT_NP_USE_MIRACL
	SM2_Init();
#else
	initialize_relic();
#endif//

	if(party == ALICE) {
		HalfGateGen<IO> * t = new HalfGateGen<IO>(io);
		CircuitExecutionProxy::circ_exec.setup(t);
		ProtocolExecutionProxy::prot_exec.setup(new SemiHonestGen<IO>(io, t));
	} else {
		HalfGateEva<IO> * t = new HalfGateEva<IO>(io);
		CircuitExecutionProxy::circ_exec.setup(t);
		ProtocolExecutionProxy::prot_exec.setup(new SemiHonestEva<IO>(io, t));
	}
}

template<typename IO>
inline void finalize_semi_honest(IO* io, int party) {
	ProtocolExecutionProxy::prot_exec.finalize();
	CircuitExecutionProxy::circ_exec.finalize();
}
}
#endif
