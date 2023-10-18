build:
	docker build --platform amd64 . -t lkp 


connect:
	docker run -ti -p 5900:5900 -p 6000:6000 -v "`pwd`/scripts:/scripts" -v "`pwd`/demos:/demos" -v "`pwd`:/repo" --rm lkp:latest /bin/bash 

# Note, this must be a snapshot -dev container created with docker commit
connect-dev:
	docker run -ti -p 5900:5900 -p 6000:6000 -v "`pwd`/scripts:/scripts" -v "`pwd`/demos:/demos" -v "`pwd`:/repo" --rm lkp-dev /bin/bash 
