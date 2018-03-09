import os, os.path
import random
import string

import cherrypy
import subprocess


import mcv_shared_memory as mcsm

start_volume = 1.0 / 5.656854249492

class AudioController(object):

    def __init__(self, audio_controller):
        self.audio_controller = audio_controller

    @cherrypy.expose
    def index(self):
        return open('index.html')

    #"request": "reverb", "reverb_type": 0, "reverb_volume": start_volume, "room_size": 0.1, "damp": 0.1, "dry": 1.0, "wet": 1.0, "width": 1.0
    @cherrypy.expose
    def setFreeReverb(self, request, reverb_enabled, reverb_type, reverb_volume, room_size, damp, dry,wet, width):
        print("received request = ", request)
        mcsm.set_int_shared_property(self.audio_controller, "REVERB_TYPE", int(reverb_type), 0)
        mcsm.set_int_shared_property(self.audio_controller, "REVERB_ENABLED", int(reverb_enabled), 0)
        mcsm.set_float_shared_property(self.audio_controller, "ROOM_SIZE", float(room_size), 0)
        mcsm.set_float_shared_property(self.audio_controller, "DAMP", float(damp), 0)
        mcsm.set_float_shared_property(self.audio_controller, "DRY", float(dry), 0)
        mcsm.set_float_shared_property(self.audio_controller, "WET", float(wet), 0)
        mcsm.set_float_shared_property(self.audio_controller, "REVERB_VOLUME", float(reverb_volume), 0)
        mcsm.set_float_shared_property(self.audio_controller, "WIDTH", float(width), 1)
        return str(request)

    @cherrypy.expose
    def setAudio(self, request, input_volume):
        print("received request = ", request)
        return str(request)


if __name__ == '__main__':
    conf = {
        '/': {
            'tools.sessions.on': True,
            'tools.staticdir.root': os.path.abspath(os.getcwd())
        },
        '/static': {
            'tools.staticdir.on': True,
            'tools.staticdir.dir': './'
        }
    }

    audio_controller = mcsm.create_shared_memory(123456)

    webapp = AudioController(audio_controller)

    cherrypy.config.update({'server.socket_host': '127.0.0.1',
                            'server.socket_port': 8080,
                       })


    cherrypy.quickstart(webapp, '/', conf)

    mcsm.free_shared_memory(audio_controller)
