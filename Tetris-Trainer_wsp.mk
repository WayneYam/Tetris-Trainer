PHONY := all clean

all:
	@echo "----------Building Project:[ tetris-trainer - Release ]----------"
	@cd tetris-trainer && $(MAKE) -f tetris-trainer.mk $@

clean:
	@echo "----------Cleaning Project:[ tetris-trainer - Release ]----------"
	@cd tetris-trainer && $(MAKE) -f tetris-trainer.mk $@


.PHONY: $(PHONY)
