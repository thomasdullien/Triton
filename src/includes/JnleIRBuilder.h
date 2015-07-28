/*
**  Copyright (C) - Triton
**
**  This program is under the terms of the LGPLv3 License.
*/

#ifndef JNLEIRBUILDER_H
#define JNLEIRBUILDER_H

#include "BaseIRBuilder.h"
#include "EflagsBuilder.h"
#include "Inst.h"
#include "OneOperandTemplate.h"


class JnleIRBuilder: public BaseIRBuilder, public OneOperandTemplate {

  public:
    JnleIRBuilder(uint64 address, const std::string &disassembly);

    // From BaseIRBuilder
    virtual Inst *process(AnalysisProcessor &ap) const;

    // From OneOperandTemplate
    virtual void none(AnalysisProcessor &ap, Inst &inst) const;

    virtual void reg(AnalysisProcessor &ap, Inst &inst) const;

    virtual void imm(AnalysisProcessor &ap, Inst &inst) const;

    virtual void mem(AnalysisProcessor &ap, Inst &inst) const;
};

#endif // JNLEIRBUILDER_H
