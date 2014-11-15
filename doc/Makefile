# Makefile for Sphinx documentation
# You can set these variables from the command line.
SPHINXOPTS    =
SPHINXBUILD   = sphinx-build
PAPER         =
BUILDDIR      = _build
# Internal variables.
ALLSPHINXOPTS   = -d $(BUILDDIR)/doctrees $(SPHINXOPTS) .

.PHONY: help clean html linkcheck

all: html

help:
	@echo "Please use \`make <target>' where <target> is one of"
	@echo "  html       to make standalone HTML files"
	@echo "  linkcheck  to check all external links for integrity"
	@echo "  serve      serve the documentation on port 8008"

clean:
	-rm -rf $(BUILDDIR)

html:
	$(SPHINXBUILD) -b html $(ALLSPHINXOPTS) $(BUILDDIR)/html
	@echo
	@echo "Build finished. The HTML pages are in $(BUILDDIR)/html."

linkcheck:
	$(SPHINXBUILD) -b linkcheck $(ALLSPHINXOPTS) $(BUILDDIR)/linkcheck
	@echo
	@echo "Link check complete; look for any errors in the above output " \
	      "or in $(BUILDDIR)/linkcheck/output.txt."

serve:
	@cd _build/html && python -m SimpleHTTPServer 8008
