build:
	docker build . -t lkp

connect:
	docker run -ti -p 5900:5900 -p 6000:6000 -v "`pwd`/scripts:/scripts" -v "`pwd`/demos:/demos" -v "`pwd`:/repo" --rm lkp:latest /bin/bash 
