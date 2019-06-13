/*
 *  Copyright 2019 Sergey Khabarov, sergeykhbr@gmail.com
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef __DEBUGGER_RIVERLIB_CORE_FPU_D_FADD_D_H__
#define __DEBUGGER_RIVERLIB_CORE_FPU_D_FADD_D_H__

#include <systemc.h>
#include "../../river_cfg.h"

namespace debugger {

SC_MODULE(DoubleAdd) {
    sc_in<bool> i_clk;
    sc_in<bool> i_nrst;
    sc_in<bool> i_ena;
    sc_in<bool> i_add;
    sc_in<bool> i_sub;
    sc_in<bool> i_eq;
    sc_in<bool> i_lt;
    sc_in<bool> i_le;
    sc_in<bool> i_max;
    sc_in<bool> i_min;
    sc_in<sc_uint<64>> i_a;        // Operand 1
    sc_in<sc_uint<64>> i_b;        // Operand 2
    sc_out<sc_uint<64>> o_res;     // Result
    sc_out<bool> o_except;         // Overflow | nanA | nanB
    sc_out<bool> o_valid;          // Result is valid
    sc_out<bool> o_busy;           // Multiclock instruction under processing

    void comb();
    void registers();

    SC_HAS_PROCESS(DoubleAdd);

    DoubleAdd(sc_module_name name_);

    void generateVCD(sc_trace_file *i_vcd, sc_trace_file *o_vcd);

 private:
    uint64_t compute_reference(bool addEna, bool subEna,
                               bool eqEna, bool ltEna, bool leEna,
                               bool maxEna, bool minEna,
                               uint64_t a, uint64_t b);

    struct RegistersType {
        sc_signal<bool> busy;
        sc_signal<sc_uint<4>> ena;
        sc_signal<sc_uint<64>> a;
        sc_signal<sc_uint<64>> b;
        sc_signal<sc_uint<64>> result;
        sc_signal<bool> add;
        sc_signal<bool> sub;
        sc_signal<bool> eq;
        sc_signal<bool> lt;
        sc_signal<bool> le;
        sc_signal<bool> max;
        sc_signal<bool> min;

        sc_uint<RISCV_ARCH> a_dbg;
        sc_uint<RISCV_ARCH> b_dbg;
        sc_uint<RISCV_ARCH> reference_res;          // Used for run-time comparision
    } v, r;

    void R_RESET(RegistersType &iv) {
        iv.busy = 0;
        iv.ena = 0;
        iv.a = 0;
        iv.b = 0;
        iv.result = 0;
        iv.add = 0;
        iv.sub = 0;
        iv.eq = 0;
        iv.lt = 0;
        iv.le = 0;
        iv.max = 0;
        iv.min = 0;

        iv.a_dbg = 0;
        iv.b_dbg = 0;
        iv.reference_res = 0;
    }
};

}  // namespace debugger

#endif  // __DEBUGGER_RIVERLIB_CORE_FPU_D_FADD_D_H__
