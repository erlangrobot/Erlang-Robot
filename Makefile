all:
	ecc robot.erl
	elink robot.beam
	cc  port.c -o port
