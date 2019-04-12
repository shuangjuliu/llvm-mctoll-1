//===- ARMRaiserBase.h ------------------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the declaration of the ARMRaiserBase class. This class
// is a base class of other ARM raisers, it supports some basic utilities for
// sub ARM raisers.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TOOLS_LLVM_MCTOLL_ARM_ARMRAISERBASE_H
#define LLVM_TOOLS_LLVM_MCTOLL_ARM_ARMRAISERBASE_H

#include "ARMModuleRaiser.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/Pass.h"

using namespace llvm;

class ARMRaiserBase : public FunctionPass {
protected:
  ARMRaiserBase() = delete;
  ARMRaiserBase(char &PassID, ARMModuleRaiser &mr)
      : FunctionPass(PassID), MR(&mr) {
    PrintPass =
        (cl::getRegisteredOptions()["print-after-all"]->getNumOccurrences() >
         0);
    M = MR->getModule();
  }
  ~ARMRaiserBase() override {}
  virtual void init(MachineFunction *mf = nullptr, Function *rf = nullptr) {
    if (mf)
      MF = mf;
    if (rf)
      RF = rf;
  }
  virtual bool runOnMachineFunction(MachineFunction &mf) { return false; }
  bool runOnFunction(Function &Func) override {
    RF = &Func;
    MF = MR->getMachineFunction(&Func);
    return runOnMachineFunction(*MF);
  }

  /// Get current raised llvm::Function.
  Function *getCRF() { return RF; }

  bool PrintPass;
  ARMModuleRaiser *MR;
  /// Current raised llvm::Function.
  Function *RF;
  MachineFunction *MF;
  Module *M;
};

#endif // LLVM_TOOLS_LLVM_MCTOLL_ARM_ARMRAISERBASE_H
