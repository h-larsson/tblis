#include "configs.hpp"
#include "configs/include_configs.hpp"

namespace tblis
{

namespace
{

enum config_t
{
#define FOREACH_CONFIG(config) config##_value,
#include "configs/foreach_config.h"
    num_configs
};

const config* const configs[num_configs] =
{
#define FOREACH_CONFIG(config) &config::instance(),
#include "configs/foreach_config.h"
};

struct default_config
{
    const config* value = nullptr;

    default_config()
    {
        int priority = -1;

        for (int cfg = 0;cfg < num_configs;cfg++)
        {
            TBLIS_ASSERT(configs[cfg]->check);
            int cur_prio = configs[cfg]->check();
            if (cur_prio > priority)
            {
                priority = cur_prio;
                value = configs[cfg];
            }

            if (get_verbose() >= 1)
            {
                printf("tblis: Configuration %s assigned priority %d.\n",
                       configs[cfg]->name, cur_prio);
            }
        }

        if (!value)
            tblis_abort_with_message(nullptr,
                "tblis: No usable configuration enabled, aborting!");

        if (get_verbose() >= 1)
        {
            printf("tblis: Using configuration %s.\n", value->name);
        }
    }
};

}

const config& get_default_config()
{
    static default_config def;
    return *def.value;
}

const config& get_config(const tblis_config* cfg)
{
    return (cfg ? *reinterpret_cast<const config*>(cfg) : get_default_config());
}

const config& get_config(const std::string& name)
{
    for (int cfg = 0;cfg < num_configs;cfg++)
    {
        if (configs[cfg]->name == name)
        {
            if (configs[cfg]->check() == -1)
                tblis_abort_with_message(nullptr,
                    "tblis: Configuration %s cannot be used!", name.c_str());
            return *configs[cfg];
        }
    }

    tblis_abort_with_message(nullptr,
        "tblis: No configuration named %s!", name.c_str());
}

}
