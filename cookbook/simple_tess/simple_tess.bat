:: Generate a regular grid, pipe it to all the computation programs,
:: and write the result to output.txt

tessgrd -r-45/45/-45/45 -b101/101 -z260e03 | ^
tesspot model.txt | ^
tessgx model.txt | tessgy model.txt | tessgz model.txt | ^
tessgxx model.txt  | tessgxy model.txt  | ^
tessgxz model.txt  | tessgyy model.txt  | ^
tessgyz model.txt  | tessgzz model.txt -v -llog.txt > output.txt

:: Make a plot with the columns of output.txt
python plot.py output.txt 101 101
