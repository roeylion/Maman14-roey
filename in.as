TEST: add 1, @r1 m1
.entry ADD
ADD: add 4,@r1
DEC: .string "abcdefjjj"
A: .data 1

mcro PRINT kk
prn @r1
    endmcro

;one regiseter
not @r3
    clr @r5
    dec @r1
    dec A

;two regiseters
cmp A ,@r6
add 3,@r3
sub 2,@r4
lea ADD ,@r1
PRINT

mcro m1
red @r1
    endmcro

mcro m3
add @r2 , @r4
    mov 5 ,TEST

endmcro
TEST: add 1, @r1 m1

test: add 2,@r2

m3
