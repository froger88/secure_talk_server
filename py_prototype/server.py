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



def main(argv):
    pass

if __name__ == "__main__":
    main(sys.argv)