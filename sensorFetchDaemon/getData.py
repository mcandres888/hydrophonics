import mysql.connector
import os
import time
import binascii
import serial
import termios
import datetime


CMD = {}
CMD['PHLevel'] = "p"
CMD['UVLevel'] = "u"
CMD['TEMPData'] = "t"



def getTimeStamp (): 
    ts = time.time()
    st = datetime.datetime.fromtimestamp(ts).strftime('%Y-%m-%d %H:%M:%S')

    #now = (datetime.datetime.utcnow() + datetime.timedelta(hours=8)).strftime("%Y-%m-%d %H:%M:%S")
    return st


def infoLog ( data ):
    st = getTimeStamp()
    os.system("echo '[%s]%s'" % (st,data))

class MySqlDBConn:
    def __init__(self, server, dbname, user, password):
        self.infoLog("init")
        self.counter = 0
        self.config = {
            'host': server,
            'port': 3306,
            'database': dbname,
            'user': user,
            'password': password,
            'charset': 'utf8',
            'use_unicode': True,
            'get_warnings': True,
        }
        self.infoLog(self.config)
        self.cnx = mysql.connector.Connect(**self.config)
        self.cur = self.cnx.cursor()

    def exec_query (self, query_str):
        self.infoLog("[EXEC] %s" % query_str)
        self.cur.execute(query_str)
        self.cnx.commit()

    def fetch_query (self, query_str):
        self.infoLog("[FETCH] %s" % query_str)
        self.cur.execute(query_str)
        rows = self.cur.fetchall()
        return rows
    
    def getDelaySettings (self ):
        query_str = "SELECT data FROM settings WHERE id=1"
        rows = self.fetch_query(query_str)
        return rows[0][0]

    def insertSensorData ( self, table , data ):
        query_str = "INSERT INTO %s (data) VALUES (%f)" % (table, data)
        retval = self.exec_query(query_str)
        return retval 




    def convertToTimestamp (self, epoch_time ):
        return  datetime.datetime.fromtimestamp(epoch_time).strftime('%Y-%m-%d %H:%M:%S')

    def infoLog(self, data):
        print data

def getSensorData ( ser_port , data ):
    ser_port.write(data) 
    ser_port.flush() 
    time.sleep(0.5) 
    out = ser.readline()
    out = out.strip()
    
    infoLog("Out = %s" % out)

    if out != '':
        if out == "Ready":
            out = 0.0
        else:
            temp = out.split(":")
            out = float(temp[1])
            if out < 0.0 :
                out = 0.0 
    else :
        out = 0.0

    infoLog("Data = %f" % out)

    return out


DB = MySqlDBConn('localhost', 'hydrophonics', 'root','root')

delay_interval = DB.getDelaySettings()

infoLog("starting daemon with delay interval of %d" % delay_interval )

ser = serial.Serial(
    port='/dev/ttyUSB0',
    baudrate=9600,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS
)



time.sleep(1)
while(1):
    ph_level = getSensorData(ser,CMD['PHLevel'])
    DB.insertSensorData("ph_sensor", ph_level)
    uv_level = getSensorData(ser,CMD['UVLevel'])
    DB.insertSensorData("uv_sensor", uv_level)
    temp_data = getSensorData(ser,CMD['TEMPData'])
    DB.insertSensorData("temp_sensor", temp_data)
    while ser.inWaiting() > 0:
        out = ser.readline()
        if out != '':
            print  out
    infoLog("Sleeping for %d seconds" % delay_interval )
    time.sleep(delay_interval)




