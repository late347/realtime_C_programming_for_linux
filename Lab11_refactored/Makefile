src = $(wildcard *.c)
obj = $(src:.c=.o)
dep = $(obj:.o=.d)  # one dependency file for each source

LDFLAGS = -lrt


all: Eleven_RealtimeSignal.exe

Eleven_RealtimeSignal.exe: $(obj)
	$(CC) -o $@ $^ $(LDFLAGS)

-include $(dep)   # include all dep files in the makefile

# rule to generate a dep file by using the C preprocessor
# (see man cpp for details on the -MM and -MT options)
%.d: %.c
	@$(CPP) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@


.PHONY: clean
clean:
	rm -f $(obj) Eleven_RealtimeSignal.exe

.PHONY: cleandep
cleandep:
	rm -f $(dep)

#.PHONY: cleanall
#cleanall:
#	rm -f $(obj $(dep) Eleven_RealtimeSignal.exe