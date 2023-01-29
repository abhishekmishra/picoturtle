.PHONY:	all apidocs prep

all:	prep apidocs

apidocs:
	mkdocs build

gen_samples:
	lua gen-samples.lua

prep:
	cp ../CHANGELOG.md docs/

dev: prep
	mkdocs serve

help:
	@echo "********************************************************"
	@echo "  Makefile to build [picoturtle-www]"
	@echo "********************************************************"
	@echo ""
	@echo "  all:      		Generate all site related files."
	@echo "  prep:     		Get artifacts from other directories."
	@echo "  gen_samples:   Get samples markdown files."
	@echo "  apidocs:  		Generate the turtle lua api docs page(s)."
	@echo "  dev:  		Run dev server."
	@echo "********************************************************"