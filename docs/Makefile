.PHONY:	all apidocs prep

all:	prep apidocs

apidocs:
	mkdocs build

prep:
	cp ../CHANGELOG.md docs/

help:
	@echo "********************************************************"
	@echo "  Makefile to build [picoturtle-www]"
	@echo "********************************************************"
	@echo ""
	@echo "  all:      Generate all site related files."
	@echo "  prep:     Get artifacts from other directories."
	@echo "  apidocs:  Generate the turtle lua api docs page(s)."
	@echo "********************************************************"