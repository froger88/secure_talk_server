#!/usr/bin/env python
#-*- coding: utf-8 -*-

import sys
import config
import gevent

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

    def close(self):
        pass



class ConnectionHandler(SocketServer.BaseRequestHandler):
    def handle(self):
        self.request.settimeout(300)
        try:
            self.clientip, self.clientport =  self.request.getpeername()
        except Exception, e:
            pass

        data = ""
        try:
            data = self.request.recv(1024)
        except Exception, e:
            if e.message.startswith("timed out"):
                if(timeouts_reached > 1):
                timeouts_reached += 1
            else:
                pass

            if(timeouts_reached * 300 > cnfig.INACTIVE_CLIENT_TIMEOUT):
                self.request.close()
                return

        try:
            self.request.sendall(response)
        except Exception, e:
            pass

class ThreadedTCPServer(SocketServer.ThreadingMixIn, SocketServer.TCPServer):
    pass


def main(argv):
    global SR

    SR = Services()

    server = ThreadedTCPServer(("0.0.0.0", config.service_port), ConnectionHandler)
    server_thread = threading.Thread(target=server.serve_forever)
    server_thread.run()
    server.shutdown()

if __name__ == "__main__":
    main(sys.argv)
