start   add     0   0   1
        add     0   1   2
        lw      0   3   target
        beq     1   2   branch
        jalr    3   4
branch  add     1   2   1
        beq     1   4   done
        jalr    3   4
done    halt
target  .fill   branch
