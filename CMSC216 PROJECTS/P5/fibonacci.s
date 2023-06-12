#Yeabsira Tamiru
#117857740

.text
fibonacci:

# PROLOGUE
    subu    $sp,    $sp,        8       # expand stack by 8 bytes
    sw      $ra,    8($sp)              # push $ra (ret addr, 4 bytes)
    sw      $fp,    4($sp)              # push $fp (4 bytes)
    addu    $fp,    $sp,        8       # set $fp to save
    
# BODY
# if a0 is 1 or 0 we have reached the base case so branch to stop
    beq     $a0,        0,          stop
    beq     $a0,        1,          stop
                                            # else
    subu    $sp,        $sp,        4       # grow the stack by one spot
    sw      $a0,        4($sp)              # push a0(n) on the stack
    sub     $a0,        $a0,        1       # a0(n) - 1
    jal     fibonacci                       # calling fibonacci till base case 

    lw      $t1,        -8($fp)             # t1 = the value in current a0
    sub     $a0,        $t1,        2       # a0(n) -2
    subu    $sp,        $sp,        4       # growing the stack by 1 spot
    sw      $v0,        4($sp)              # pushing a0-1 on the stack ->f(n-1)
    jal     fibonacci                       # calling fibonacci

    lw      $t2,        -12($fp)            # now loading a0-1 in t2 ->f(n-1)
    add     $v0,        $t2,        $v0     # adding sub parts fb(n-1) + fb(n-2)
    j       epi

stop:
    move    $v0,        $a0                 # done

# EPILOGUE
epi:
    move    $sp,    $fp                 # restore $sp
    lw      $ra,    ($fp)               # restore saved $ra
    lw      $fp,    -4($sp)             # restore saved $fp
    j       $ra                         # return to kernel

