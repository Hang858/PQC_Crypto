#include <stddef.h>
#include "polarlac_params.h"
#include "operator_interface.h"

const polarlac_params_t *const polarlac_params_list[POLARLAC_LEVEL_COUNT] = {
    &laclight_params,
    &lac128_params,
    &lac256_params,
};

polarlac_level_t g_polarlac_level = POLARLAC_DEFAULT_LEVEL;

static int32_t polarlac_level_is_valid(polarlac_level_t level)
{
    return level >= POLARLAC_LEVEL_LIGHT && level <= POLARLAC_LEVEL_256;
}

const polarlac_params_t *polarlac_get_params(polarlac_level_t level)
{
    if (!polarlac_level_is_valid(level)) {
        return NULL;
    }

    return polarlac_params_list[level];
}

const polarlac_params_t *polarlac_current_params(void)
{
    return polarlac_get_params(g_polarlac_level);
}

int32_t polarlac_set_level(polarlac_level_t level)
{
    const polarlac_params_t *params = polarlac_get_params(level);
    if (params == NULL) {
        return -1;
    }

    if (params->ntt_omega == NULL) {
        return -2;
    }

    if (OP_ntt256_init(params->ntt_omega, params->ntt_omega_count,
                       params->ntt_q, params->ntt_inv_q,
                       params->ntt_inv_n, params->ntt_k) != OP_SUCCESS) {
        return -3;
    }

    g_polarlac_level = level;
    return 0;
}

polarlac_level_t polarlac_get_level(void)
{
    return g_polarlac_level;
}
