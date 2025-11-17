FROM gcc:latest

RUN git clone -q https://github.com/DenisPetuhow/Module_3.git /app

WORKDIR /app

RUN g++ -o program Source.cpp

CMD ["./program"]