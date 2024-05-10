start   add  0   0   1
        add  1   0   2
        nor  1   2   3
        lw   0   4   data
        sw   2   4   data
        beq  3   1   done
        lw   0   5   addr
        jalr 5   6
        noop
        noop
next    jalr 4   4
        noop
        noop
done    halt
data    .fill   5400
        .fill   0
one     .fill   1
addr    .fill   next
