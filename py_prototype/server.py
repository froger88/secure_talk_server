#!/usr/bin/env python
#-*- coding: utf-8 -*-

import sys
import config
import SocketServer
import threading

class Session(object):
    def __init__(self):
        self.session_id = None
        self.secure_token_list = []
        self.session_owner = None
        
    def create(self, client):
        self.session_owner = client
        self.session_id = generate_session_id(client.nickname)

    def add_client(self):
        pass

    def del_client(self):
        pass

    def invite(self):
        pass
    
    def close(self):
        pass

    

class Client(object):
    def __init__(self):
        self.fd = -1

    def join(self, session_id, secure_token, nickname):
        pass

    def crete_session():
        pass
    
    def close(self):
        pass


class ConnectionHandler(SocketServer.BaseRequestHandler):
    def handle(self):
        self.request.settimeout(config.INACTIVE_CLIENT_TIMEOUT)
        try:
            self.clientip, self.clientport =  self.request.getpeername()
        except Exception, e:
            pass

        data = ""
        response = ""
        timeouts_reached = 0
        while True:
            try:
                data = self.request.recv(config.MAX_DATA_SIZE)
            except Exception, e:
                if e.message.startswith("timed out"):
                    self.request.close()
                else:
                    print e
                self.request.close()
                return

            if len(data) == 0:
                print "connection broken by client"
                self.request.close()
                return

            print "%s:%d: %s" % (self.clientip, self.clientport, data.rstrip())
            
            try:
                self.request.sendall(response)
            except Exception, e:
                print e
                self.request.close()
                return

class ThreadedTCPServer(SocketServer.ThreadingMixIn, SocketServer.TCPServer):
    pass


def main(argv):
    server = ThreadedTCPServer((config.IP, config.PORT), ConnectionHandler)
    server_thread = threading.Thread(target=server.serve_forever)
    server_thread.run()
    server.shutdown()

if __name__ == "__main__":
    main(sys.argv)
