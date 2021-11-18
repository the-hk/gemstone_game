TARGET_01        = solution1
SOURCE_01        = solution1.c

TARGET_02        = solution2
SOURCE_02        = solution2.c

TARGET_UTIL_GENERATE_BUCKET = utilities/generate_gemstone_bucket
SOURCE_UTIL_GENERATE_BUCKET = utilities/generate_gemstone_bucket.c

SEED			= 123
BUCKET_FILE_COUNT = 5

CC = gcc
CFLAGS = -Wall -g3 -gdwarf-2 -DDEBUG

.PHONY: all
all: ans1 ans2
ans1: $(TARGET_01)
ans2: $(TARGET_02)

$(TARGET_01): $(SOURCE_01)
$(TARGET_02): $(SOURCE_02)

.PHONY: clean
clean:
	rm -f $(TARGET_01) $(TARGET_02) "$(TARGET_UTIL_GENERATE_BUCKET)" $(OBJS)

.PHONY: tests
tests: clean test1 test2

test1: ans1
	$(eval RECEIVED_OUTPUT = $(shell ./$(TARGET_01) | grep  -ioP "result\s*\K\d+" ))
	$(eval EXPECTED_ANS = $(shell \
	cat buckets/0.txt | grep -o [dser] | sort | uniq -c | awk '{if ($$2 == "d") value=$$1*3500;  else if ($$2 == "s") value=$$1*1200 ; else if ($$2 == "e") value=$$1*800; else if ($$2 == "r") value=$$1*50; total+=value} END {print total}' \
	))
	@test $(RECEIVED_OUTPUT) = $(EXPECTED_ANS) \
    || { echo $(RECEIVED_OUTPUT) received but $(EXPECTED_ANS) was expected; exit 2; } \
    && { echo $(RECEIVED_OUTPUT) is correct.; } 
test2: ans2
	$(eval RECEIVED_OUTPUT = $(shell ./$(TARGET_02) | grep  -ioP "result\s*\K\d+" ))
	$(eval EXPECTED_ANS = $(shell \
	cat buckets/0.txt buckets/1.txt buckets/2.txt buckets/3.txt buckets/4.txt | grep -o [dser] | sort | uniq -c | awk '{if ($$2 == "d") value=$$1*3500;  else if ($$2 == "s") value=$$1*1200 ; else if ($$2 == "e") value=$$1*800; else if ($$2 == "r") value=$$1*50; total+=value} END {print total}' \
	))
	@test $(RECEIVED_OUTPUT) = $(EXPECTED_ANS) \
    || { echo $(RECEIVED_OUTPUT) received but $(EXPECTED_ANS) was expected; exit 2; } \
    && { echo $(RECEIVED_OUTPUT) is correct.; }

	


.PHONY: generate
generate: run_generate
run_generate: $(TARGET_UTIL_GENERATE_BUCKET)
	@./$(TARGET_UTIL_GENERATE_BUCKET) $(SEED) $(BUCKET_FILE_COUNT)
	@echo "generated with known seeds"

$(TARGET_UTIL_GENERATE_BUCKET): $(SOURCE_UTIL_GENERATE_BUCKET)

generate_with_random_seeds:  $(TARGET_UTIL_GENERATE_BUCKET)
	$(eval RANDOM_SEED = $(shell date +"%N"))
	@./$(TARGET_UTIL_GENERATE_BUCKET) $(RANDOM_SEED) $(BUCKET_FILE_COUNT)
	@echo "generated generate_with_random_seeds"