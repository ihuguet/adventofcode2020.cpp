ifeq ($P,)
$(error Usage `make P=puzzleFolder`)
endif

default:
	make -C $P
