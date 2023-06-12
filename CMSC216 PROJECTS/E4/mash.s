

# mash(x, y)

mash:

# PROLOGUE
    subu    $sp,    $sp,        8       # expand stack by 8 bytes
    sw      $ra,    8($sp)              # push $ra (ret addr, 4 bytes)
    sw      $fp,    4($sp)              # push $fp (4 bytes)
    addu    $fp,    $sp,        8       # set $fp to save

# BODY
    lw      $t0,    4($fp)              # loading x in the register t0
    lw      $t1,    8($fp)              # loading y in the register t1
    li      $t2,    10                  # loading the number 10 in t2

    mul     $t2,    $t0,        $t2     # x * 10 = t2
    add     $t2,    $t2,        $t1     # y + t2 = t2
    move    $v0,    $t2                 # v0 = t2

# EPILOGUE
    move    $sp,    $fp                 # restore $sp
    lw      $ra,    ($fp)               # restore saved $ra
    lw      $fp,    -4($sp)             # restore saved $fp
    j       $ra                         # return to kernel
