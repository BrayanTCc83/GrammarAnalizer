.PHONY: compile test

SOURCE=src
HEADERS=$(SOURCE)/headers
MAIN=$(SOURCE)/main.c
TEST=test
TEST_MAIN=$(TEST)/main.c
GLOBAL=$(SOURCE)/global.c
STRUCTS=$(SOURCE)/structs
SET=$(STRUCTS)/set/*.c
LIST=$(STRUCTS)/list/*.c
HASHTABLE=$(STRUCTS)/hashtable/*.c

DISTRIBUTION=dist
OUTPUT=$(DISTRIBUTION)/GrammarEvaluator

CC:=gcc

compile: $(MAIN) $(GLOBAL) $(SET) $(LIST) $(HASHTABLE) $(HEADERS)/*.h | $(DISTRIBUTION)
	@echo Compile program.
	$(CC) -o $(OUTPUT) $(MAIN) $(GLOBAL) $(SET) $(LIST) $(HASHTABLE) -I $(HEADERS) 

test: $(TEST_MAIN) $(GLOBAL) $(SET) $(LIST) $(HASHTABLE) $(HEADERS)/*.h | $(DISTRIBUTION)
	@echo Compile program.
	$(CC) -o $(OUTPUT) $(TEST_MAIN) $(GLOBAL) $(SET) $(LIST) $(HASHTABLE) -I $(HEADERS)  -DDEV

$(DISTRIBUTION):
	@echo Making directory $(DISTRIBUTION).
	mkdir $(DISTRIBUTION)