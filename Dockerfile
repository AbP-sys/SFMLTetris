FROM ubuntu

RUN apt-get update
RUN apt-get install -y g++
RUN apt-get install -y libsfml-dev
RUN apt-get -y install xauth
RUN apt install -y pulseaudio
COPY . .
EXPOSE 8887
CMD /launch
