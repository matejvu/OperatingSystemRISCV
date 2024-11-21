.extern _ZN5RiscV16interruptHandlerEv
.align 4
.global _ZN5RiscV16interruptRoutineEv
.type _ZN5RiscV16interruptRoutineEv, @function


_ZN5RiscV16interruptRoutineEv:


    j _ZN5RiscV17interruptRoutine2Ev
    j _ZN5RiscV13timerRoutine2Ev
    sret
    sret
    sret
    sret
    sret
    sret
    sret
    j _ZN5RiscV15consoleRoutine2Ev