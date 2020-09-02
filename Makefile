DIR= achievement2/

all:
	$(MAKE) -C $(DIR) all
	cp $(DIR)/project .
	./project

test:
	$(MAKE) -C $(DIR) test
	cp $(DIR)/testexec .
	./testexec

clean:
	$(MAKE) -C $(DIR) clean cleano

.PHONY: all clean
