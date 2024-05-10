        lw      0       1       one
        lw      0       2       start
        lw      0       3       ten
        lw      0       4       cnt
loop    beq     4       3       done
        add     2       4       2
        add     1       4       4
        beq     0       0       loop
done    sw      0       2       result
        halt
one     .fill   1
start   .fill   0
ten     .fill   10
cnt     .fill   1
result  .fill   0
