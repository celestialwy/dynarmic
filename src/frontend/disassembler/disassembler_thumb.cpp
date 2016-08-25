/* This file is part of the dynarmic project.
 * Copyright (c) 2016 MerryMage
 * This software may be used and distributed according to the terms of the GNU
 * General Public License version 2 or any later version.
 */

#include <cstdlib>
#include <string>

#include <fmt/format.h>

#include "common/bit_util.h"
#include "common/string_util.h"
#include "frontend/arm_types.h"
#include "frontend/decoder/thumb16.h"

namespace Dynarmic {
namespace Arm {

class DisassemblerVisitor {
public:
    std::string thumb16_LSL_imm(Imm5 imm5, Reg m, Reg d) {
        return fmt::format("lsls {}, {}, #{}", RegToString(d), RegToString(m), imm5);
    }

    std::string thumb16_LSR_imm(Imm5 imm5, Reg m, Reg d) {
        return fmt::format("lsrs {}, {}, #{}", RegToString(d), RegToString(m), imm5 != 0 ? imm5 : 32);
    }

    std::string thumb16_ASR_imm(Imm5 imm5, Reg m, Reg d) {
        return fmt::format("asrs {}, {}, #{}", RegToString(d), RegToString(m), imm5 != 0 ? imm5 : 32);
    }

    std::string thumb16_ADD_reg_t1(Reg m, Reg n, Reg d) {
        return fmt::format("adds {}, {}, {}", RegToString(d), RegToString(n), RegToString(m));
    }

    std::string thumb16_SUB_reg(Reg m, Reg n, Reg d) {
        return fmt::format("subs {}, {}, {}", RegToString(d), RegToString(n), RegToString(m));
    }

    std::string thumb16_ADD_imm_t1(Imm3 imm3, Reg n, Reg d) {
        return fmt::format("adds {}, {}, #{}", RegToString(d), RegToString(n), imm3);
    }

    std::string thumb16_SUB_imm_t1(Imm3 imm3, Reg n, Reg d) {
        return fmt::format("subs {}, {}, #{}", RegToString(d), RegToString(n), imm3);
    }

    std::string thumb16_MOV_imm(Reg d, Imm8 imm8) {
        return fmt::format("movs {}, #{}", RegToString(d), imm8);
    }

    std::string thumb16_CMP_imm(Reg n, Imm8 imm8) {
        return fmt::format("cmp {}, #{}", RegToString(n), imm8);
    }

    std::string thumb16_ADD_imm_t2(Reg d_n, Imm8 imm8) {
        return fmt::format("adds {}, #{}", RegToString(d_n), imm8);
    }

    std::string thumb16_SUB_imm_t2(Reg d_n, Imm8 imm8) {
        return fmt::format("subs {}, #{}", RegToString(d_n), imm8);
    }

    std::string thumb16_AND_reg(Reg m, Reg d_n) {
        return fmt::format("ands {}, {}", RegToString(d_n), RegToString(m));
    }

    std::string thumb16_EOR_reg(Reg m, Reg d_n) {
        return fmt::format("eors {}, {}", RegToString(d_n), RegToString(m));
    }

    std::string thumb16_LSL_reg(Reg m, Reg d_n) {
        return fmt::format("lsls {}, {}", RegToString(d_n), RegToString(m));
    }

    std::string thumb16_LSR_reg(Reg m, Reg d_n) {
        return fmt::format("lsrs {}, {}", RegToString(d_n), RegToString(m));
    }

    std::string thumb16_ASR_reg(Reg m, Reg d_n) {
        return fmt::format("asrs {}, {}", RegToString(d_n), RegToString(m));
    }

    std::string thumb16_ADC_reg(Reg m, Reg d_n) {
        return fmt::format("adcs {}, {}", RegToString(d_n), RegToString(m));
    }

    std::string thumb16_SBC_reg(Reg m, Reg d_n) {
        return fmt::format("sbcs {}, {}", RegToString(d_n), RegToString(m));
    }

    std::string thumb16_ROR_reg(Reg m, Reg d_n) {
        return fmt::format("rors {}, {}", RegToString(d_n), RegToString(m));
    }

    std::string thumb16_TST_reg(Reg m, Reg n) {
        return fmt::format("tst {}, {}", RegToString(n), RegToString(m));
    }

    std::string thumb16_RSB_imm(Reg n, Reg d) {
        // Pre-UAL syntax: NEGS <Rd>, <Rn>
        return fmt::format("rsbs {}, {}, #0", RegToString(d), RegToString(n));
    }

    std::string thumb16_CMP_reg_t1(Reg m, Reg n) {
        return fmt::format("cmp {}, {}", RegToString(n), RegToString(m));
    }

    std::string thumb16_CMN_reg(Reg m, Reg n) {
        return fmt::format("cmn {}, {}", RegToString(n), RegToString(m));
    }

    std::string thumb16_ORR_reg(Reg m, Reg d_n) {
        return fmt::format("orrs {}, {}", RegToString(d_n), RegToString(m));
    }

    std::string thumb16_BIC_reg(Reg m, Reg d_n) {
        return fmt::format("bics {}, {}", RegToString(d_n), RegToString(m));
    }

    std::string thumb16_MVN_reg(Reg m, Reg d) {
        return fmt::format("mvns {}, {}", RegToString(d), RegToString(m));
    }

    std::string thumb16_ADD_reg_t2(bool d_n_hi, Reg m, Reg d_n_lo) {
        Reg d_n = d_n_hi ? (d_n_lo + 8) : d_n_lo;
        return fmt::format("add {}, {}", RegToString(d_n), RegToString(m));
    }

    std::string thumb16_CMP_reg_t2(bool n_hi, Reg m, Reg n_lo) {
        Reg n = n_hi ? (n_lo + 8) : n_lo;
        return fmt::format("cmp {}, {}", RegToString(n), RegToString(m));
    }

    std::string thumb16_MOV_reg(bool d_hi, Reg m, Reg d_lo) {
        Reg d = d_hi ? (d_lo + 8) : d_lo;
        return fmt::format("mov {}, {}", RegToString(d), RegToString(m));
    }

    std::string thumb16_LDR_literal(Reg t, Imm8 imm8) {
        u32 imm32 = imm8 << 2;
        return fmt::format("ldr {}, [pc, #{}]", RegToString(t), imm32);
    }

    std::string thumb16_STR_reg(Reg m, Reg n, Reg t) {
        return fmt::format("str {}, [{}, {}]", RegToString(t), RegToString(n), RegToString(m));
    }

    std::string thumb16_STRH_reg(Reg m, Reg n, Reg t) {
        return fmt::format("strh {}, [{}, {}]", RegToString(t), RegToString(n), RegToString(m));
    }

    std::string thumb16_STRB_reg(Reg m, Reg n, Reg t) {
        return fmt::format("strb {}, [{}, {}]", RegToString(t), RegToString(n), RegToString(m));
    }

    std::string thumb16_LDRSB_reg(Reg m, Reg n, Reg t) {
        return fmt::format("ldrsb {}, [{}, {}]", RegToString(t), RegToString(n), RegToString(m));
    }

    std::string thumb16_LDR_reg(Reg m, Reg n, Reg t) {
        return fmt::format("ldr {}, [{}, {}]", RegToString(t), RegToString(n), RegToString(m));
    }

    std::string thumb16_LDRH_reg(Reg m, Reg n, Reg t) {
        return fmt::format("ldrh {}, [%s, %s]", RegToString(t), RegToString(n), RegToString(m));
    }

    std::string thumb16_LDRB_reg(Reg m, Reg n, Reg t) {
        return fmt::format("ldrb {}, [{}, {}]", RegToString(t), RegToString(n), RegToString(m));
    }

    std::string thumb16_LDRSH_reg(Reg m, Reg n, Reg t) {
        return fmt::format("ldrsh {}, [{}, {}]", RegToString(t), RegToString(n), RegToString(m));
    }

    std::string thumb16_STR_imm_t1(Imm5 imm5, Reg n, Reg t) {
        u32 imm32 = imm5 << 2;
        return fmt::format("str {}, [{}, #{}]", RegToString(t), RegToString(n), imm32);
    }

    std::string thumb16_LDR_imm_t1(Imm5 imm5, Reg n, Reg t) {
        u32 imm32 = imm5 << 2;
        return fmt::format("ldr {}, [{}, #{}]", RegToString(t), RegToString(n), imm32);
    }

    std::string thumb16_STRB_imm(Imm5 imm5, Reg n, Reg t) {
        u32 imm32 = imm5;
        return fmt::format("strb {}, [{}, #{}]", RegToString(t), RegToString(n), imm32);
    }

    std::string thumb16_LDRB_imm(Imm5 imm5, Reg n, Reg t) {
        u32 imm32 = imm5;
        return fmt::format("ldrb {}, [{}, #{}]", RegToString(t), RegToString(n), imm32);
    }

    std::string thumb16_STRH_imm(Imm5 imm5, Reg n, Reg t) {
        u32 imm32 = imm5 << 1;
        return fmt::format("strh {}, [{}, #{}]", RegToString(t), RegToString(n), imm32);
    }

    std::string thumb16_LDRH_imm(Imm5 imm5, Reg n, Reg t) {
        u32 imm32 = imm5 << 1;
        return fmt::format("ldrh {}, [{}, #{}]", RegToString(t), RegToString(n), imm32);
    }

    std::string thumb16_STR_imm_t2(Reg t, Imm5 imm5) {
        u32 imm32 = imm5 << 2;
        return fmt::format("str {}, [sp, #{}]", RegToString(t), imm32);
    }

    std::string thumb16_LDR_imm_t2(Reg t, Imm5 imm5) {
        u32 imm32 = imm5 << 2;
        return fmt::format("ldr {}, [sp, #{}]", RegToString(t), imm32);
    }

    std::string thumb16_ADR(Reg d, Imm8 imm8) {
        u32 imm32 = imm8 << 2;
        return fmt::format("adr {}, +#{}", RegToString(d), imm32);
    }

    std::string thumb16_ADD_sp_t1(Reg d, Imm8 imm8) {
        u32 imm32 = imm8 << 2;
        return fmt::format("add {}, sp, #{}", RegToString(d), imm32);
    }

    std::string thumb16_ADD_sp_t2(Imm7 imm7) {
        u32 imm32 = imm7 << 2;
        return fmt::format("add sp, sp, #{}", imm32);
    }

    std::string thumb16_SUB_sp(Imm7 imm7) {
        u32 imm32 = imm7 << 2;
        return fmt::format("sub sp, sp, #{}", imm32);
    }

    std::string thumb16_SXTH(Reg m, Reg d) {
        return fmt::format("sxth {}, {}", RegToString(d), RegToString(m));
    }

    std::string thumb16_SXTB(Reg m, Reg d) {
        return fmt::format("sxtb {}, {}", RegToString(d), RegToString(m));
    }

    std::string thumb16_UXTH(Reg m, Reg d) {
        return fmt::format("uxth {}, {}", RegToString(d), RegToString(m));
    }

    std::string thumb16_UXTB(Reg m, Reg d) {
        return fmt::format("uxtb {}, {}", RegToString(d), RegToString(m));
    }

    std::string thumb16_PUSH(bool M, RegList reg_list) {
        if (M) reg_list |= 1 << 14;
        return "push " + RegListToString(reg_list);
    }

    std::string thumb16_POP(bool P, RegList reg_list) {
        if (P) reg_list |= 1 << 15;
        return "pop " + RegListToString(reg_list);
    }

    std::string thumb16_SETEND(bool E) {
        return fmt::format("setend {}", E ? "BE" : "LE");
    }

    std::string thumb16_REV(Reg m, Reg d) {
        return fmt::format("rev {}, {}", RegToString(d), RegToString(m));
    }

    std::string thumb16_REV16(Reg m, Reg d) {
        return fmt::format("rev16 {}, {}", RegToString(d), RegToString(m));
    }

    std::string thumb16_REVSH(Reg m, Reg d) {
        return fmt::format("revsh {}, {}", RegToString(d), RegToString(m));
    }

    std::string thumb16_STMIA(Reg n, RegList reg_list) {
        return fmt::format("stm {}!, {}", RegToString(n), RegListToString(reg_list));
    }

    std::string thumb16_LDMIA(Reg n, RegList reg_list) {
        bool write_back = !Common::Bit(static_cast<size_t>(n), reg_list);
        return fmt::format("ldm {}{}, {}", RegToString(n), write_back ? "!" : "", RegListToString(reg_list));
    }

    std::string thumb16_BX(Reg m) {
        return fmt::format("bx {}", RegToString(m));
    }

    std::string thumb16_BLX_reg(Reg m) {
        return fmt::format("blx {}", RegToString(m));
    }

    std::string thumb16_UDF() {
        return fmt::format("udf");
    }

    std::string thumb16_SVC(Imm8 imm8) {
        return fmt::format("svc #{}", imm8);
    }

    std::string thumb16_B_t1(Cond cond, Imm8 imm8) {
        s32 imm32 = Common::SignExtend<9, s32>(imm8 << 1) + 4;
        return fmt::format("b{} {}#{}", CondToString(cond), Common::SignToChar(imm32), abs(imm32));
    }

    std::string thumb16_B_t2(Imm11 imm11) {
        s32 imm32 = Common::SignExtend<12, s32>(imm11 << 1) + 4;
        return fmt::format("b {}#{}", Common::SignToChar(imm32), abs(imm32));
    }
};

std::string DisassembleThumb16(u16 instruction) {
    DisassemblerVisitor visitor;
    auto decoder = DecodeThumb16<DisassemblerVisitor>(instruction);
    return !decoder ? fmt::format("UNKNOWN: {:x}", instruction) : decoder->call(visitor, instruction);
}

} // namespace Arm
} // namespace Dynarmic
