FROM alpine:latest

RUN apk --update add doxygen graphviz git git-lfs &&\
    rm -rf /var/cache/apk/*

CMD ["doxygen", "-v"]

WORKDIR /tmp
