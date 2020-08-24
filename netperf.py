#!/usr/bin/env python3
# File    : netperf.py - monitor meraki gear and update ESP32 via MQTT
# Author  : Joe McManus josephmc@alumni.cmu.edu
# Version : 0.1 08/23/2020
# Copyright (C) 2020 Joe McManus

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



config=configparser.ConfigParser()
config.read('netperf.conf')
apiKey=config['apiKey']['key']
mxSerial=config['mx']['serial']
msSerial=config['ms']['serial']
orgId=config['org']['orgId']
mqttServer=config['mqtt']['server']

def updateLED(server, topic, message):
	pub.single(topic, message, hostname=server)

def clearAll(mqttserver):
	updateLED(mqttServer, 'ledOne', 0)
	time.sleep(0.5)
	updateLED(mqttServer, 'ledTwo', 0)
	time.sleep(0.5)
	updateLED(mqttServer, 'ledThree', 0)
#quick way to clear the display
if len(sys.argv) == 2:
	clearAll(mqttServer)
	sys.exit()

dashboard = meraki.DashboardAPI(apiKey)

while True: 
	#Get the "performance" of the Meraki firewall and send it to the led
	response = dashboard.appliance.getDeviceAppliancePerformance(mxSerial)
	mxPerf=round(response['perfScore']/10)
	updateLED(mqttServer, 'ledOne', mxPerf)

	#Org client security events
	response = dashboard.appliance.getOrganizationApplianceSecurityEvents( orgId, total_pages='all')
	updateLED(mqttServer, 'ledTwo', len(response))

	#Port usage
	response = dashboard.switch.getDeviceSwitchPortsStatuses(msSerial,timespan=300)
	#pp = pprint.PrettyPrinter(indent=4)
	#pp.pprint(response[0])
	bandwidth=(response[0]['trafficInKbps']['total'])
	bandwidthMb=round(bandwidth / 1024)
	updateLED(mqttServer, 'ledThree', bandwidthMb)

	time.sleep(30)

