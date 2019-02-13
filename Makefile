.PHONY: report clean

run: compil
	./src/main

compil:
	gcc -O0 -g3 -Wall src/main.c -o src/main

clean:
	rm -rf src/main
	rm -rf report/*.aux report/*.log report/*.out report/*.toc report/svg-inkscape report/_minted-report report/_minted-src

report:
	cd report && pdflatex --shell-escape *.tex && pdflatex --shell-escape *.tex
