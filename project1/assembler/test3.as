        add     0   0   4
        add     0   0   5
        nor     0   0   6
        lw      0   4   data1
        sw      0   4   data2
        jalr    4   5
        noop
        halt
data1   .fill   data1
data2   .fill   0
