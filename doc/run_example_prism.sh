#!/bin/bash

tessgrd -r0/10e03/0/10e03 -b100/100 -z1e03 | prismgz prism_model.txt | \
prismgxx prism_model.txt | prismgxy prism_model.txt | prismgxz prism_model.txt | \
prismgyy prism_model.txt | prismgyz prism_model.txt | prismgzz prism_model.txt > prism_output.txt