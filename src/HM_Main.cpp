#include <iostream>

#include "HM_Alive_Supervision_Cfg.h"


int main(int argc, char** argv)
{
    // for testing
    hm_alive_supervision_param_t alive_supervision_params{0u, 0u, 0u, 0u, 0u, 0u, 10u};
    HMAliveSupervisionCfg alive_sup_cfg{alive_supervision_params};

    std::cout << "Alive supervision ID: " << alive_sup_cfg.get_supervision_id() << std::endl;
    
    return 0;
}
