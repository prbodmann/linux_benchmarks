#!/usr/bin/python
import threading
import socket
import time
import os
from datetime import datetime
import sys
import requests
import json
from enum import Enum


class ErrorCodes(Enum):
    """
    Error codes used to identify status in this test
    """
    SUCCESS = 0

    # Codes for Machine class
    WAITING = 1
    REBOOTING = 2
    BOOT_PROBLEM = 3
    MAX_SEQ_REBOOT_REACHED = 4
    TURN_ON = 5

    # Codes for RebootMachine
    GENERAL_ERROR = 6
    HTTP_ERROR = 7
    CONNECTION_ERROR = 8
    TIMEOUT_ERROR = 9


class Switch():
     # Switches status, only used in this class
    __ON = "ON"
    __OFF = "OFF"
    def __init__(self, switch_model, switch_port, switch_ip, rebooting_sleep):
        self.__switch_port = switch_port
        self.__switch_model= switch_model
        self.__switch_ip = switch_ip
        self.__reboot_status = ErrorCodes.SUCCESS
        self.__rebooting_sleep = rebooting_sleep

    def reset_board(self):
        self.off()
        time.sleep(1)
        self.on()
        time.sleep(self.__rebooting_sleep)

    def __select_command_on_switch(self, status):
        if self.__switch_model == "default":
            self.__common_switch_command(status)
        elif self.__switch_model == "lindy":
            #print("lindy\n")
            self.__lindy_switch(status)
        elif self.__switch_model == "debug":
            #print("lindy\n")
            self.__debug_switch(status)
        else:
            raise ValueError("Incorrect switch switch_model")

    def get_reboot_status(self):
        return self.__reboot_status

    def on(self):
        self.__select_command_on_switch(self.__ON)

    def off(self):
        self.__select_command_on_switch(self.__OFF)

    def __lindy_switch(self, status):
        to_change = "000000000000000000000000"
        led = f"{to_change[:(self.__switch_port - 1)]}1{to_change[self.__switch_port:]}"

        if status == self.__ON:
            url = f'http://{self.__switch_ip}/ons.cgi?led={led}'
        else:
            url = f'http://{self.__switch_ip}/offs.cgi?led={led}'
        payload = {
            "led": led,
        }
        headers = {
            "Host": self.__switch_ip,
            "User-Agent": "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.12; rv:56.0) Gecko/20100101 Firefox/56.0",
            "Accept": "*/*",
            "Accept-Language": "en-US,en;q=0.5",
            "Accept-Encoding": "gzip, deflate",
            "Referer": f"http://{self.__switch_ip}/outlet.htm",
            "Authorization": "Basic c25tcDoxMjM0",
            "Connection": "keep-alive",
            "Content-Length": "0",
        }

        #print(url)
        #print(headers)
        try:
            requests_status = requests.post(url, data=json.dumps(payload), headers=headers)
            requests_status.raise_for_status()           
        except requests.exceptions.HTTPError as http_error:
            self.__reboot_status = ErrorCodes.HTTP_ERROR
        except requests.exceptions.ConnectionError as connection_error:
            self.__reboot_status = ErrorCodes.CONNECTION_ERROR
        except requests.exceptions.Timeout as timeout_error:
            self.__reboot_status = ErrorCodes.TIMEOUT_ERROR
        except requests.exceptions.RequestException as general_error:
            self.__reboot_status = ErrorCodes.GENERAL_ERROR

    def __common_switch_command(self, status):
        port_default_cmd = 'pw%1dName=&P6%1d=%%s&P6%1d_TS=&P6%1d_TC=&' % (
            self.__switch_port, self.__switch_port - 1, self.__switch_port - 1, self.__switch_port - 1)

        cmd = 'curl --data \"'
        cmd += port_default_cmd % ("On" if status == self.__ON else "Off")
        cmd += '&Apply=Apply\" '
        cmd += f'http://%s/tgi/iocontrol.tgi {self.__switch_ip}'
        cmd += '-o /dev/null '
        self.__reboot_status = self.__execute_command(cmd)
        
    def __debug_switch(self, status):
        return
    def __execute_command(cmd):
        tmp_file = "/tmp/server_error_execute_command"
        result = os.system(f"{cmd} 2>{tmp_file}")
        with open(tmp_file) as err:
            if len(err.readlines()) != 0 or result != 0:
                return ErrorCodes.GENERAL_ERROR
        return ErrorCodes.SUCCESS


#s = Switch("192.168.1.100", 4)
#print "Port 1"
#s.cmd(int(1), 'On')
#s.cmd(int(1), 'Off')
#print "Port 2"


