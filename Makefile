FIND=$(shell which gfind find | head -1)

.PHONY: test
test:
	@$(MAKE) -C test run

.PHONY: format
format:
	$(FIND) src/ -type f \( -name '*.c' -o -name '*.h' \) -exec clang-format -i {} +
