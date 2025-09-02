
.globl cas

lrsc_ins:
cas:
	lr.w t0, (a0)
	bne t0, a1, fail
	sc.w a0, a2, (a0)
	jr ra
fail:
	li a0, 1
	jr ra

.globl amoadd_ins
	#a0 data0:*p
	#a1 data1
	#a0 result
amoadd_ins:
    //amoadd.w a0, a1, (a0)
    jr ra

