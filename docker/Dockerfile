FROM archlinux
RUN pacman -Sy --noconfirm boost glfw-x11
COPY ./server /shepherd_server
COPY ./netsi_includes /usr/local/include/netsi
COPY ./libnetsi.so /usr/local/lib/libnetsi.so
ENV LD_LIBRARY_PATH /usr/local/lib
EXPOSE 1350/udp
CMD ["/shepherd_server"]
