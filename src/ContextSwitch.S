.global _ZN3PCB6setjmpEPm
.type _ZN3PCB6setjmpEPm, @function
_ZN3PCB6setjmpEPm:

    #a0 - buffer[0]
    .irp index, 1,2,3,4,5,6,7,8,9,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    sd x\index, \index * 8(a0)
    .endr
    li x10, 0

    ret

.global _ZN3PCB7longjmpEPm #_ZN3PCB7longjmpEPmi
.type _ZN3PCB7longjmpEPm, @function
_ZN3PCB7longjmpEPm:
    #a0 - buffer->x

    .irp index, 1,2,3,4,5,6,7,8,9,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    ld x\index, \index * 8(a0)
    .endr
    li a0, 1

    ret

