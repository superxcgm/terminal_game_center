
all:
	@$(MAKE) -C snake
	@$(MAKE) -C bounce

.PHONY: format
format:
	find . -iname *.h -o -iname *.cpp | xargs clang-format -i -style=file

.PHONY: lint
lint:
	cpplint --recursive .
