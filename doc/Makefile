all: Doxyfile
	doxygen
	cp images/* build/html
	cp tabs.css build/html
	cp ../examples/dem_brasil/dem_brasil_example.pdf build/html
# 	cp images/tab_*.png build/html
# 	cp images/subtab_*.png build/html
# 	cp images/logo.ico build/html

win: Doxyfile
	doxygen
	copy images\* build\html
	copy tabs.css build\html
	copy ..\examples\dem_brasil\dem_brasil_example.pdf build\html
# 	copy images\tab_*.png build\html
# 	copy images\subtab_*.png build\html
# 	copy images\logo.ico build\html
