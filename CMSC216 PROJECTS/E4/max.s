#Yeabsira Tamiru
#117857740

# max(x, y)

max:

# PROLOGUE
    subu    $sp,    $sp,        8       # expand stack by 8 bytes
    sw      $ra,    8($sp)              # push $ra (ret addr, 4 bytes)
    sw      $fp,    4($sp)              # push $fp (4 bytes)
    addu    $fp,    $sp,        8       # set $fp to saved $ra

# BODY
    bgt     $a0,    $a1,        target  # if $a0 >= $a1 then target
    move    $v0,    $a1                 # move the a1 in v0
    j       endif                       # done - value found so jump to the epilogue

target:

    move    $v0,    $a0                 # if a0 is greater
    j       endif

# EPILOGUE
endif:

    move    $sp,    $fp                 # restore $sp
    lw      $ra,    ($fp)               # restore saved $ra
    lw      $fp,    -4($sp)             # restore saved $fp
    j       $ra                         # return to kernel
