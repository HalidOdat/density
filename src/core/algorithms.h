/*
 * Centaurean Density
 *
 * Copyright (c) 2015, Guillaume Voirin
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     1. Redistributions of source code must retain the above copyright notice, this
 *        list of conditions and the following disclaimer.
 *
 *     2. Redistributions in binary form must reproduce the above copyright notice,
 *        this list of conditions and the following disclaimer in the documentation
 *        and/or other materials provided with the distribution.
 *
 *     3. Neither the name of the copyright holder nor the names of its
 *        contributors may be used to endorse or promote products derived from
 *        this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * 3/02/15 19:51
 */

#ifndef DENSITY_ALGORITHMS_H
#define DENSITY_ALGORITHMS_H

#include "../globals.h"

typedef enum {
    DENSITY_ALGORITHMS_EXIT_STATUS_FINISHED = 0,
    DENSITY_ALGORITHMS_EXIT_STATUS_ERROR_DURING_PROCESSING,
    DENSITY_ALGORITHMS_EXIT_STATUS_INPUT_STALL,
    DENSITY_ALGORITHMS_EXIT_STATUS_OUTPUT_STALL,
    DENSITY_ALGORITHMS_EXIT_STATUS_USER_INTERRUPT
} density_algorithm_exit_status;

typedef struct {
    void *dictionary;
    uint_fast8_t copy_penalty;
    uint_fast8_t copy_penalty_start;
    uint_fast64_t counter;
    uint_fast32_t user_defined_interrupt;
    bool return_from_interrupt;
} density_algorithm_state;

#define DENSITY_ALGORITHM_COPY(work_block_size)\
            DENSITY_MEMCPY(*out, *in, work_block_size);\
            *in += work_block_size;\
            *out += work_block_size;\
            if (!(--state->copy_penalty))\
                state->copy_penalty_start++;

#define DENSITY_ALGORITHM_CHECK_USER_INTERRUPT\
            if (state->user_defined_interrupt) {\
                if (!(state->counter & state->user_defined_interrupt)) {\
                    if (state->return_from_interrupt)\
                        state->return_from_interrupt = false;\
                    else {\
                        state->return_from_interrupt = true;\
                        return DENSITY_ALGORITHMS_EXIT_STATUS_USER_INTERRUPT;\
                    }\
                }\
            }

DENSITY_WINDOWS_EXPORT void density_algorithms_prepare_state(density_algorithm_state *const, void *const);

DENSITY_WINDOWS_EXPORT void density_algorithm_copy(density_algorithm_state *const, const uint8_t **, uint8_t **, const uint_fast16_t);

#endif