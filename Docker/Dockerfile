FROM debian:jessie 



RUN apt-get update && apt-get -y install locales nasm llvm clang gcc qemu make ssh git



WORKDIR /root

VOLUME ["/root/TP2"]

CMD ["bash"]