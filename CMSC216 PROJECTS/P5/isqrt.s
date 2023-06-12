#Yeabsira Tamiru
#117857740

.text
isqrt:

# PROLOGUE
    subu    $sp,    $sp,        8       # expand stack by 8 bytes
    sw      $ra,    8($sp)              # push $ra (ret addr, 4 bytes)
    sw      $fp,    4($sp)              # push $fp (4 bytes)
    addu    $fp,    $sp,        8       # set $fp to save
# BODY

    li      $t7,    2
    subu    $sp,    $sp,        4       # growing the stack for n
    sw      $a0,    4($sp)              # pushing n on the stack
    move    $v0,    $a0
    bgt     $t7,    $a0,        epi     # jump to the epilogue & return n if 2>n
    srl     $a0,    $a0,        2       # n>>2 = $a0
    jal     isqrt                       # calling isqrt on a0

    sll     $t0,    $v0,        1       # $v0<<1 = small
    add     $t1,    $t0,        1       # small + 1 = large
    mul     $t2,    $t1,        $t1     # $t2 = large * large
    lw      $a0,    4($sp)

    bgt     $t2,    $a0,        small   # if large * large > n branch to less
    j       large                       # else jump to large

small:
    move    $v0,    $t0                 # returning small
    j       epi

large:                                  # returning large
    move    $v0,    $t1
# EPILOGUE
epi:
    move    $sp,    $fp                 # restore $sp
    lw      $ra,    ($fp)               # restore saved $ra
    lw      $fp,    -4($sp)             # restore saved $fp
    j       $ra                         # return to kernel
