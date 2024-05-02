.PHONY: help build build2 build3 test test2 test3 clean

help:
	@grep -E '^[a-zA-Z0-9_%/-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

build: ## Build driver executable using rbtree.c
	$(MAKE) -C src driver

build2: ## Build driver2 executable using rbtree2.c
	$(MAKE) -C src driver2

build3: ## Build driver3 executable using rbtree3.c
	$(MAKE) -C src driver3

test: ## Test rbtree implementation
	$(MAKE) -C test test

test2: ## Test rbtree2 implementation
	$(MAKE) -C test test2

test3: ## Test rbtree3 implementation
	$(MAKE) -C test test3

clean: ## Clear build environment
	$(MAKE) -C src clean
	$(MAKE) -C test clean
