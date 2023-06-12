#Yeabsira Tamiru
#117857740

.text
reverse_prefix_sum:

# PROLOGUE
    subu    $sp,                $sp,        8       # expand stack by 8 bytes
    sw      $ra,                8($sp)              # push $ra (ret addr, 4 bytes)
    sw      $fp,                4($sp)              # push $fp (4 bytes)
    addu    $fp,                $sp,        8       # set $fp to save

# BODY
    subu    $sp,                $sp,        8       # expanding the stack by 2 spots
    sw      $s0,                8($sp)              # pusing s0 on the stack
    sw      $a0,                4($sp)              # pushing a0 on the stack

    li      $v0,                0
    lw      $s0,                0($a0)              # s0 is the value at the
                                                    # first index of the array
    beq     $s0,                -1,         epi     # if s0(*arr) = -1 branch to
                                                    # the epilogue else contunue
    add     $a0,                $a0,        4       # a0 now should start from
                                                    # the next digit
    jal     reverse_prefix_sum                      # reverse_prefix_sum(arr+1) + 
                                                    # (uint32_t)*arr
    addu    $v0,                $v0,        $s0     # v0+=s0
    lw      $a0,                4($sp)
    sw      $v0,                0($a0)              # saving the added answer to v0

epi:
    lw      $s0,                8($sp)              # storing every pushed value 
                                                    # from the stack on s0
    move    $sp,                $fp                 # restore $sp
    lw      $ra,                ($fp)               # restore saved $ra
    lw      $fp,                -4($sp)             # restore saved $fp
    j       $ra                                     # return to kernel
