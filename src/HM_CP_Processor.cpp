#include "HM_CP_Processor.h"


bool HMCPProcessor::process_reached_cp(const hm_supervised_entity_id_t, const hm_checkpoint_id_t)
{
    std::cout << "Processing the received checkpoint..." << std::endl;
}
