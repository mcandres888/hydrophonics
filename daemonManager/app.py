from bottle import *
import bottle
import subprocess
import os


@route('/restart' )
def restart ():

    retval = {}
    retval['status'] = 1
    os.system("/usr/sbin/service mattdaemon restart &") 
    return retval

run(host='0.0.0.0', port=8888, debug=True)


