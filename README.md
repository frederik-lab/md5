# md5 implementation project in C

## md5 algorithm step by step

### step 1, bitpadding:
take message b and apply padding such until: $(b+padding) \equiv 448 \mod 512$

padding is done by appending a 1 to b and then appending 0s until the padded message is congruent to 448 modulo 512

### step 2, :



# scripts
## precalc script: 
This scripts calculates the table T values in step 4 of the RFC 
`gcc scripts/md5_precalc.c -o bin/gentable -lm`
`./bin/gentable`