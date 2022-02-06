#include <iostream>
#include <string>

#include "HM_Types.h"
#include "HM_Cfg_Types.h"
#include "HM_Mode_Config.h"
#include "Health_Manager.h"


int main()
{
    std::cout << "count is: " << HMModeConfig::get_count() << std::endl;

    return 0;
}
