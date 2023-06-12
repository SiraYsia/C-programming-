

.text
is_palindrome:

# PROLOGUE
    subu    $sp,        $sp,        8       # expand stack by 8 bytes
    sw      $ra,        8($sp)              # push $ra (ret addr, 4 bytes)
    sw      $fp,        4($sp)              # push $fp (4 bytes)
    addu    $fp,        $sp,        8       # set $fp to save

# BODY
    li      $t0,        0                   # setting the counter to 0
    sw      $a0,        12($sp)             # pushing the word on the stack
    jal     strlen

    lw      $a0,        4($fp)
    move    $t1,        $v0                 # setting $t1 to the length
    move    $t2,        $a0                 # need the origional word

    div     $t3,        $t1,        2       # t/4 strlen/2

# $t0 - the counter i
# $t1 - length of the string -> strlen(string)
# $t2 - the origional word -> string
# $t3 - the condition  -> len / 2
# $t4 - the charachter in the left to be comapred -> string[i]
# $t5 - the index of the right most (len - i - 1)
# $t6 - the address of the right side char to be compared
# $t7 - the first charachter in t6 -> string[len - i - 1])


loop:
                                            # setting loop condition.
    beq     $t0,        $t3,        yes     # Branch if t0 = t3
    lb      $t4,        0($a0)              # first left most char of $a0

    sub     $t5,        $t1,        $t0     # $t5 = len - i
    sub     $t5,        $t5,        1       # $t5 = $t5 - 1

    add     $t6,        $t5,        $t2     # $t6= $t2 + $t5 (right side)

    lb      $t7,        0($t6)
    bne     $t4,        $t7,        no      # branch to no if $t4 != $t7
    add     $t0,        $t0,        1       # increment i by 1
    add     $a0,        $a0,        1       # pointer to the next letter
    j       loop

# not a palindrome
no:
    li      $v0,        0                   # returning 0
    j       epi

# is a palindrome
yes:
    li      $v0,        1                   # returning 1

# EPILOGUE
epi:
    move    $sp,        $fp                 # restore $sp
    lw      $ra,        ($fp)               # restore saved $ra
    lw      $fp,        -4($sp)             # restore saved $fp
    j       $ra                             # return to kernel

strlen:
    li      $v0,        0                   # keeps track of total letters read

loop_str:
    lb      $t1,        0($a0)              # loading & processing letter by letter
    beqz    $t1,        endloop             # branch if that specfic letter is null
    addi    $v0,        $v0,        1       # incrementing the index by 1
    addi    $a0,        $a0,        1       # now will move to the second byte
    j       loop_str

endloop:
    jr      $ra                             # return to kernell
