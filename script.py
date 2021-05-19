# Date: Sept 2017

from __future__ import print_function

#################################################
#################### IMPORTS ####################
#################################################

from array import *
import binascii
from datetime import datetime
import io
import os
import re
import smtplib
import struct
from subprocess import Popen
import sys
import time
from time import sleep
import subprocess
import switch
import telnetlib
import time
import socket
import fcntl
import struct
import errno
from telnetlib import Telnet

#################################################
############### GLOBAL VARIABLES ################
#################################################

from threading import Timer
USER="carol"
PSSWD="R@diation"
PATH="/home/" + USER + "/benchmarks/"
#arg_dic={"qsort":" /tmp/benchmark/input_large.dat /tmp/benchmark/input_large.dat"
arg_dic={"lud":" 1024 "+PATH+ "input_1024_th_1 "+PATH+"gold_1024_th_1",
        "lud_small":" 512 "+PATH+ "input_512_th_1 "+PATH+"gold_512_th_1",
        "lavamd":" 5 "+PATH+"input_distance_1_5 "+PATH+"input_charges_1_5 "+PATH+"output_gold_1_5",
        "hotspot_64":" 256 256 100  "+PATH+"temp_256 "+PATH+"power_256 "+PATH+"gold_256",
        "l1_test":"",
        "fft":"21 "+PATH+"fft_input.bin "+PATH+"fft_gold.bin",
        "qsort":"2000000 "+PATH+"qsort_input_2000000.bin "+PATH+"qsort_gold_2000000.bin ",
        "qsort_small":"200000 "+PATH+"qsort_input_200000.bin "+PATH+"qsort_gold_200000.bin ",
        "matmul_400": " "+PATH+"matmul_input_400.bin"+" "+PATH+"matmul_gold_400.bin 400"

        }
mess_size_dic={
        "lud":5,
        "lud_small":5,
        "lavamd":10,
        "hotspot_64":5,
        "l1_test":3,
        "fft":4,
        "qsort":2,
        "matmul_400":4
        }


switch_serial=None
DD_flag=0;
NUM_TRIES=4

log_outf = None # initialize log file descriptor


# initialize variables
isAA = False    #alive indicando X execucoes com sucesso
isAF = 0    #alive watchdog maquina de estados dut
isDD = 0        #SDC
double_DD=0
prev_DD_message=""
sock=None

s=None

switch_port=0

#################################################
#################### METHODS ####################
#################################################
def get_ip_address():
    global subnet

    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        s.connect((subnet, 1027))
    except socket.error:
        return None
        
    #return s.getsockname()[0]
    return s.getsockname()[0] #change_env_3
    #s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    #return socket.inet_ntoa(fcntl.ioctl(
    #    s.fileno(),
    #    0x8915,  # SIOCGIFADDR
    #    struct.pack('256s', ifname[:15])
    #)[20:24])



def start_app():
    global tn
    global board_ip
    global exec_code
    global USER
    try:
        #tn = telnetlib.Telnet(board_ip,23,90) #change_env_2 (changing switch_port) #change_essential_1
        tn = telnetlib.Telnet(board_ip,23,30)
        
        
        #change_env_2 Note that ZedBoard should terminate original telnet daemon and reset_board /usr/sbin/telnetd -p 23
        tn.read_until(b"login: ",timeout=30)#change_env_2
        tn.write(bytes(USER +"\n",'ascii'))#change_env_2
        tn.read_until(b"Password:",timeout=30)#change_env_2
        tn.write(bytes(PSSWD +"\n",'ascii'))#change_env_2


        #message=b"nohup /tmp/benchmark/myloop \n"
        #print(message)  
        #tn.write(message)
        
        l=tn.read_very_eager()
        message="pkill "+exec_code+"\n" #change_env_3
        tn.write(message.encode('ascii'))
        l=tn.read_very_eager()
        #print(l)
        message="nohup "+PATH+""+exec_code+" "+get_ip_address()+" "+str(pc_port)+" "+ arg_dic[exec_code]+" &\n exit\n" #change_env_3
        print(message)
        tn.write(message.encode('ascii'))
        
        
        
        l=tn.read_very_eager()
        print(l)
        #tn.close()
    except OSError as e:
        print(e)
        if(e.errno==errno.EHOSTUNREACH):
            return 2
        return 1
    except EOFError as e:
        print(e)        
        return 1
    return 0

def repeatedErrors():
    global prev_DD_message
    global current_DD_message
    prev_DD_message=""
    current_DD_message=""
    write_logs(getTime() + " " +board_ip +" [INFO] Double DD, restarting application")
    start_app()

    return

def resetBoard():
    global wd
    global DD_flag
    global prev_DD_message
    global current_DD_message
    global pc_port
    global sock
    global s
    prev_DD_message=""
    current_DD_message=""
    DD_flag=0
    write_logs(getTime() + " " +board_ip +" [INFO] BOARD HARD RESET...")
    s.reset_board()
    counter=0
    while(1):
        ret_value=start_app()
        if (ret_value!=0):
            if(counter==NUM_TRIES or ret_value==2):                
                s.reset_board()
                counter=0
            else:
                sleep(1)
                counter+=1
            write_logs(getTime() + " " +board_ip +" [INFO] CONNECTION TO BOARD FAILED RETRYING")
        else:
            break

    write_logs(getTime() + " " +board_ip +" [INFO] EXPERIMENT RESUMED")
    return

def write_logs(str):
    print(str, end = '\n')
    log_outf.write(str+'\n')
    log_outf.flush()
    return

def getTime():
    return datetime.now().strftime("%d-%m-%Y_%H-%M-%S")

def openLog():
    global log_outf
    global exec_code
    date=getTime()
    filename_inj = "./logs/"+exec_code+'/log'+'_' + date +"_" +board_ip+'.txt'
    filemode_inj = 'a'
    log_outf = open(filename_inj, filemode_inj)
    write_logs(getTime() +" " +board_ip +" [INFO] starting radiation experiment...")
    return

def unexpectedOutputFunc():
    global isReconfigure
    global string


    write_logs(getTime() +" " +board_ip + " [INFO] unexpected output: " + string)
    #isReconfigure = True

    return

def read_word(data,pos):
    string=""
    for i in range(4*pos,4*(pos+1)):
        #print("0x{:02x}".format(data[i])[2:])
        string="0x{:02x}".format(data[i])[2:]+string
    return string

def main():
    global sock
    global string
    global wd
    global DD_flag
    global current_DD_message
    global prev_DD_message
    global DATA_SIZE
    while True:        
        printTimeout = True
        
        try:
            data, addr = sock.recvfrom(4*DATA_SIZE)
        except socket.timeout:
            start_app_ret=start_app()
            if (start_app_ret != 0):
                write_logs(getTime() + " " +board_ip +" [INFO] TIMEOUT... LINUX CRASHED")
                resetBoard()
            else:
                write_logs(getTime() +" " +board_ip+ " [INFO] TIMEOUT... APP CRASHED")
                write_logs(getTime() + " " +board_ip +" [INFO] EXPERIMENT RESUMED")               
        else:
            if(prev_DD_message==current_DD_message and prev_DD_message!=""):
                    #print("sao iguais\n")
                    repeatedErrors()
            string=read_word(data,0)
            #print(data)
            #print("current")
            #print(len(current_DD_message))
            #print("prev")
            #print(len(prev_DD_message))
            if(string[:2]=="aa"):
                write_logs(getTime() +" " +board_ip + " [PL] AA")
                DD_flag=0
                prev_DD_message=""
                current_DD_message=""
            elif(string[:2]=="dd"):
                #print(prev_DD_message+" - "+current_DD_message)
                prev_DD_message=current_DD_message[:]

                current_DD_message=""
                for i in range(1,DATA_SIZE):
                    current_DD_message+=read_word(data,i)
                    #print(string)

                write_logs(getTime() + " " +board_ip +" [PL] DD "+current_DD_message)
                #print(DD_flag)
            elif(string[:2]=="cc"):
                string=""
                for i in range(1,DATA_SIZE):
                    string+=read_word(data,i)
                    #print(string)
                write_logs(getTime() + " " +board_ip +" [PL] CC "+string)
                current_DD_message+=string
                #print("current2")
                #print(current_DD_message)
                #print("prev2")
                #print(prev_DD_message)           
            else:
                unexpectedOutputFunc()
    return

def tryMain():

    global exec_code
    global sock
    global s
    global switch_port
    global tn
    global board_ip
    global pc_port
    global wd
    global DATA_SIZE
    global rest_args
    global subnet

    board_ip= sys.argv[1]
    pc_port = int(sys.argv[2])
    exec_code= sys.argv[3]   
    DATA_SIZE = mess_size_dic[exec_code]
    switch_type= sys.argv[4]
    switch_ip=sys.argv[5] #change_env_1
    switch_port= int(sys.argv[6]) #change_env_1
    sleep_time= int(sys.argv[7]) #change_env_1
    subnet = '.'.join(board_ip.split('.')[:3]) + '.1'
    #wd=Watchdog(30)
    s = switch.Switch(switch_type,switch_port,switch_ip, sleep_time) #change_env_1
    tn = None

    openLog()
    resetBoard()
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    
    
    a= get_ip_address()

    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sock.bind((get_ip_address(), pc_port))
    sock.settimeout(15) #change_essential_1
    
    
    
    try:
        main()
    except KeyboardInterrupt as err:
        write_logs(getTime() +" " +board_ip+ " [INFO] keyborad interruption. exiting...")
        #wd.stop()
        print(err)
        sys.exit(0)
    #except Exception as err:
    #    print(err)
    #    write_logs(getTime() + " [INFO] unexpected script interruption...")
    #    isReconfigure = True


#serial_port = '/dev/boards/uart/1-1.2'

#configureZynq()

while True:
    tryMain()
