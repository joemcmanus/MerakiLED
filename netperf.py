#!/usr/bin/env python3
# File    : netperf.py - monitor meraki gear and update ESP32 via MQTT
# Author  : Joe McManus josephmc@alumni.cmu.edu
# Version : 0.4 06/01/2021
# Copyright (C) 2021 Joe McManus

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.


import meraki
import time
import configparser
import paho.mqtt.publish as pub
import pprint
import sys
from prettytable import PrettyTable
import sqlite3
from os import path
import datetime

config=configparser.ConfigParser()
config.read('netperf.conf')
apiKey=config['apiKey']['key']
mxSerial=config['mx']['serial']
msSerial=config['ms']['serial']
orgId=config['org']['orgId']
mqttServer=config['mqtt']['server']
bandwidthMax=int(config['local']['bandwidth'])
dbStatus=config['db']['status']
dbFile=config['db']['file']
startTime=int(config['opHours']['startTime'])
endTime=int(config['opHours']['endTime'])

def updateLED(server, topic, message):
	now = datetime.datetime.now()
	afterEnd=now.replace(hour=endTime, minute=0, second=0, microsecond=0)
	beforeStart=now.replace(hour=startTime, minute=0, second=0, microsecond=0)

	if now < beforeStart or now > afterEnd:
		try:
			pub.single('ledOne', 0, hostname=server)
			time.sleep(1)
			pub.single('ledTwo', 0, hostname=server)
			time.sleep(1)
			pub.single('ledThree', 0, hostname=server)
			time.sleep(1)
		except:
			pass
	else:
		try:
			pub.single(topic, message, hostname=server)
		except:
			pass

def clearAll(mqttserver):
	try:
		updateLED(mqttServer, 'ledOne', 0)
		time.sleep(1)
		updateLED(mqttServer, 'ledTwo', 0)
		time.sleep(1)
		updateLED(mqttServer, 'ledThree', 0)
	except:
		pass

def storeValue(val):
    t=(val,)
    query='insert into bandwidth(timestamp, bandwidth) values(datetime(),?)'
    cursor=db.cursor()
    cursor.execute(query, t)
    db.commit()

#quick way to clear the display
if len(sys.argv) == 2:
    clearAll(mqttServer)
    sys.exit()

if dbStatus: 
    #print(dbFile)
    if path.exists(dbFile):
        print("DB File Found")
        db = sqlite3.connect(dbFile)
        db.row_factory = sqlite3.Row    
    if not path.exists(dbFile):
        print("DB File not found, creating")
        db = sqlite3.connect(dbFile)
        db.row_factory = sqlite3.Row    
        query="CREATE TABLE bandwidth (timestamp datetime NOT NULL, bandwidth integer)"
        cursor=db.cursor()
        cursor.execute(query)
        db.commit()


dashboard = meraki.DashboardAPI(apiKey)

while True: 
	try: 
		table=PrettyTable(["check", "value"])
		#Get the "performance" of the Meraki firewall and send it to the led
		response = dashboard.appliance.getDeviceAppliancePerformance(mxSerial)
		mxPerf=round(response['perfScore']/10)
		if mxPerf == 0 :
			mxPerf =1 
		#Org client security events
		response = dashboard.appliance.getOrganizationApplianceSecurityEvents( orgId, total_pages='all')
		secAlerts=len(response)
		
		#Port usage

		response = dashboard.switch.getDeviceSwitchPortsStatuses(msSerial,timespan=300)
		bandwidth=(response[0]['trafficInKbps']['total'])
		bandwidthMb=round(bandwidth / 1024)
		bandwidthPerc=round(bandwidthMb/bandwidthMax*10)

		updateLED(mqttServer, 'ledOne', mxPerf)
		table.add_row(["Perf", mxPerf]) 
		updateLED(mqttServer, 'ledTwo', secAlerts)
		table.add_row(["Sec Evt", secAlerts]) 

		updateLED(mqttServer, 'ledThree', bandwidthPerc)
		table.add_row(["Bandwidth", bandwidthMb]) 

		#show table of results
		print(table)
		if dbStatus:
			storeValue(bandwidthMb)
	except:
		print("ERROR: Unable to connect to Meraki Dashboard")

	time.sleep(30)

