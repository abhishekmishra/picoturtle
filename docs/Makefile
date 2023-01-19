.PHONY:	all apidocs

all:	apidocs

apidocs:
	pandoc -s --embed-resources --toc --number-sections -o api.html -M document-css=false --template ../templates/pandoc-www-template.html ../help.md

help:
	@echo "********************************************************"
	@echo "  Makefile to build [picoturtle-www]"
	@echo "********************************************************"
	@echo ""
	@echo "  all:      Generate all site related files."
	@echo "  apidocs:  Generate the turtle lua api docs page(s)."
	@echo "********************************************************"