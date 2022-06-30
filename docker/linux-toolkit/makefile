build:
	docker build .  -t ltk

connect:
	docker run -ti -p 5900:5900 -v `pwd`:/local  --rm ltk:latest /bin/bash 
