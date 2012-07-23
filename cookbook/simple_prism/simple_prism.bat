

:: Generate a regular grid, pipe it to all the computation programs,
:: and write the result to output.txt

tessgrd -r0/20000/0/20000 -b50/50 -z1000 | ^
prismpot model.txt | ^
prismgx model.txt | prismgy model.txt | prismgz model.txt | ^
prismgxx model.txt  | prismgxy model.txt  | ^
prismgxz model.txt  | prismgyy model.txt  | ^
prismgyz model.txt  | prismgzz model.txt > output.txt
