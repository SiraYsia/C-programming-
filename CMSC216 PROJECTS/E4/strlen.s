#Yeabsira Tamiru
#117857740

# strlen(str)


strlen:

# PROLOGUE
    subu    $sp,    $sp,        8   # expand stack by 8 bytes
    sw      $ra,    8($sp)          # push $ra (ret addr, 4 bytes)
    sw      $fp,    4($sp)          # push $fp (4 bytes)
    addu    $fp,    $sp,        8   # set $fp to save


# BODY
    li      $t0,    0               # keeps track of total letters read

loop:
    lb      $t1,    0($a0)          # processing each letter
    beqz    $t1,    endloop         # branch if that specfic letter is null
    addi    $t0,    $t0,        1   # incrementing the index by 1 to represent that first letter has been read
    addi    $a0,    $a0,        1   # now will move to the second byte(letter)
    j       loop

endloop:
    move    $a0,    $t0
    li      $v0,    1               # prepare to print an int
    syscall 
    li      $v0,    10              # set up to exit
    sycall  

# EPILOGUE
    move    $sp,    $fp             # restore $sp
    lw      $ra,    ($fp)           # restore saved $ra
    lw      $fp,    -4($sp)         # restore saved $fp
    j       $ra                     # return to kernel

