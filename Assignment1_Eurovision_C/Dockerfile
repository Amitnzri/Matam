FROM gcc:latest
RUN apt-get update -yq && apt-get install -y --no-install-recommends expect valgrind && rm -rf /var/lib/apt/lists/*
COPY /src ~/src
WORKDIR ~/src
