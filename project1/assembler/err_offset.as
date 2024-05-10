        lw      0   1   one
        nor     0   0   2
        lw      0   3   50000
        lw      3   4   0
        lw      3   5   1
        add     1   3   6
        add     1   6   6
        add     2   4   7
        sw      6   7   0
        add     1   4   7
        sw      6   7   1
        add     2   5   7
        sw      6   7   2
        add     1   5   7
        sw      6   7   3
        noop
        halt
data    .fill   150
        .fill   250
        .fill   0
        .fill   0
        .fill   0
        .fill   0
one     .fill   1
addr    .fill   data
