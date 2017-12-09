#! /usr/bin/env/python3
# -*- coding: utf-8 -*-

"""
A very simple Python HTTP-TCP server. Only purpose is to echo strings passed
to the server.
"""

import socket
import multiprocessing


def client_socket_function(client_socket):
    """
    The function assigned to each connected client. Simples:
    Read from socket, write input back to socket.

    Argument:
        client_socket: The socket assigned to the connected client.
    """

    CLIENT_BUFFER_SIZE = 4096

    connection_terminated = 0
    while not connection_terminated:
        # Get message from socket.
        msg = client_socket.recv(CLIENT_BUFFER_SIZE)
        msg_decoded = msg.decode('ascii')
        # Strip trailing newlines from message.
        msg_decoded_and_stripped = msg_decoded.rstrip('\r\n')
        if msg_decoded_and_stripped == 'test':
            client_socket.send(b'You tested the server\r\n')
        client_socket.send(b'Message received: ' + msg + b'\0')

    if connection_terminated:
        client_socket.close()


def main():
    """We set up the server socket and run the server."""

    HOST_NAME = 'localhost'
    SERVER_PORT = 5000

    # Setup an INET, STREAMin server.
    serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # Bind the socket to the machines hostname and a well known port.
    # Dev-notes: (string, int) pair is the argument because we use AF_INET.
    serversocket.bind((HOST_NAME, SERVER_PORT))
    # Setup the socket to handle at most 5 connections.
    serversocket.listen(5)
    
    print "Serving on " + HOST_NAME + ":" + str(SERVER_PORT)

    # The server loop, connections are accepted.
    while 1:
        # Accept connection
        clientsocket, _ = serversocket.accept()
        # Create a multiprocessing process to handle the client.
        client_process = multiprocessing.Process(target=client_socket_function,
                                                 args=(clientsocket,))
        client_process.start()


if __name__ == '__main__':
    main()
